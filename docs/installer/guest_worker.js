/* Web Worker: runs Pyodide with streaming FileReaderSync Blob-backed Olnk
 * so 3x ~650 MB root.olk files never exhaust 32-bit WASM linear memory. */

let pyodide = null;

self.onmessage = async (ev) => {
  const { xbRoot, gcRoot, gcDol, ps2Root, ps2Elf, pyFiles } = ev.data;
  try {
    const postProgress = (pct, label) => self.postMessage({ type: "progress", pct, label });
    const postLog = (msg, level = "info") => self.postMessage({ type: "log", msg, level });

    postProgress(15, "Loading Python runtime (Pyodide) in background worker...");
    if (!pyodide) {
      importScripts("https://cdn.jsdelivr.net/pyodide/v0.26.4/full/pyodide.js");
      pyodide = await loadPyodide();
    }

    pyodide.FS.mkdirTree("/work/scripts");
    if (gcDol) pyodide.FS.mkdirTree("/work/gc");
    if (ps2Elf) pyodide.FS.mkdirTree("/work/ps2");

    postProgress(30, "Loading conversion scripts (tools/guests)...");
    for (const [name, code] of Object.entries(pyFiles)) {
      pyodide.FS.writeFile(`/work/scripts/${name}`, code);
    }

    // Only boot.dol (~2.8 MB) and SLUS_206.43 (~5 MB) go into MEMFS;
    // the three huge root.olk files are read on-demand via FileReaderSync!
    if (gcDol) {
      pyodide.FS.writeFile("/work/gc/boot.dol", new Uint8Array(await gcDol.arrayBuffer()));
    }
    if (ps2Elf) {
      pyodide.FS.writeFile("/work/ps2/SLUS_206.43", new Uint8Array(await ps2Elf.arrayBuffer()));
    }

    const blobs = {
      xb: xbRoot,
      gc: gcRoot || null,
      ps2: ps2Root || null,
    };
    const reader = new FileReaderSync();

    self.__readBlobSlice = (key, offset, length) => {
      const b = blobs[key];
      if (!b || length <= 0) return new Uint8Array(0);
      return new Uint8Array(reader.readAsArrayBuffer(b.slice(offset, offset + length)));
    };

    let outputParts = null;
    self.__emitGuestsParts = (pyParts) => {
      const jsParts = [];
      const arr = pyParts.toJs();
      for (const item of arr) {
        if (Array.isArray(item)) {
          const [key, off, len] = item;
          jsParts.push(blobs[key].slice(off, off + len));
        } else {
          jsParts.push(new Uint8Array(item));
        }
      }
      pyParts.destroy();
      outputParts = jsParts;
    };

    postProgress(55, "Building guests.olk (Link & Heihachi)...");
    postLog("Running roster.py (streaming Blob-backed Olnk)...");

    await pyodide.runPythonAsync(`
import sys, struct, builtins
import js
sys.path.insert(0, "/work/scripts")
import olnk
import roster

def _read_blob(key, off, length):
    if length <= 0:
        return b""
    return bytes(js.__readBlobSlice(key, int(off), int(length)).to_py())

class BlobSlice:
    def __init__(self, key, at, size):
        self.key = key
        self.at = at
        self.size = size
    def __len__(self):
        return self.size
    def __bool__(self):
        return self.size > 0

class BlobParts:
    def __init__(self, parts, total_size):
        self.parts = parts
        self.total_size = total_size
    def __len__(self):
        return self.total_size
    def __bool__(self):
        return self.total_size > 0

class StreamOlnk:
    def __init__(self, key, base=0, size=None):
        self.key = key
        self.base = base
        hdr = _read_blob(key, base, 0x20)
        if len(hdr) < 0x20 or hdr[4:8] != b"olnk":
            raise ValueError("not an olnk container at %#x" % base)
        self.count, = struct.unpack_from("<I", hdr, 0)
        self.alignment, = struct.unpack_from("<I", hdr, 8)
        self.header_size, self.data_size, self.time = struct.unpack_from("<III", hdr, 0x10)
        self.size = size if size is not None else (self.header_size + self.data_size)
        self._table = _read_blob(key, base + 0x20, self.count * 16)

    def entry(self, index):
        if not 0 <= index < self.count:
            raise IndexError(index)
        offset, size, time, _ = struct.unpack_from("<IIII", self._table, index * 16)
        return self.base + self.header_size + offset, size, time

    def bytes(self, index):
        at, size, _ = self.entry(index)
        return _read_blob(self.key, at, size)

    def is_container(self, index):
        at, size, _ = self.entry(index)
        if size < 0x20:
            return False
        return _read_blob(self.key, at + 4, 4) == b"olnk"

    def child(self, index):
        at, size, _ = self.entry(index)
        return StreamOlnk(self.key, at, size)

    def entries(self):
        out_entries = []
        out_times = []
        for i in range(self.count):
            at, size, t = self.entry(i)
            out_entries.append(BlobSlice(self.key, at, size) if size else None)
            out_times.append(t)
        return out_entries, out_times

    def get(self, path):
        parts = [int(p) for p in str(path).strip("/").split("/")]
        node = self
        for p in parts[:-1]:
            node = node.child(p)
        return node.bytes(parts[-1])

_PATH_TO_KEY = {
    "/work/xb/root.olk": "xb",
    "/work/gc/root.olk": "gc",
    "/work/ps2/root.olk": "ps2",
}

@classmethod
def _stream_open(cls, path):
    key = _PATH_TO_KEY.get(path)
    if key is None:
        return cls(open(path, "rb").read())
    return StreamOlnk(key)

def _stream_build(entries, alignment=0x800, time=0, times=None):
    count = len(entries)
    header = (0x20 + 16 * count + alignment - 1) // alignment * alignment
    index = []
    parts = []
    body_len = 0
    for e in entries:
        if not e:
            index.append((0, 0))
            continue
        sz = len(e)
        index.append((body_len, sz))
        if isinstance(e, BlobParts):
            parts.extend(e.parts)
        elif isinstance(e, BlobSlice):
            parts.append((e.key, e.at, e.size))
        else:
            parts.append(bytes(e))
        body_len += sz
        pad = -body_len % alignment
        if pad:
            parts.append(b"\\x00" * pad)
            body_len += pad
    out = bytearray(header)
    struct.pack_into("<I4sII", out, 0, count, b"olnk", alignment, 0)
    struct.pack_into("<III", out, 0x10, header, body_len, time)
    for i, (offset, size) in enumerate(index):
        struct.pack_into("<IIII", out, 0x20 + 16 * i, offset, size, times[i] if times else time, 0)
    return BlobParts([bytes(out)] + parts, header + body_len)

olnk.Olnk.open = _stream_open
olnk.build = _stream_build
roster.Olnk = olnk.Olnk
roster.build = _stream_build

_orig_open = builtins.open
_captured_parts = None

class _GuestSink:
    def __enter__(self):
        return self
    def __exit__(self, *a):
        return False
    def write(self, data):
        global _captured_parts
        if isinstance(data, BlobParts):
            _captured_parts = data.parts
        else:
            _captured_parts = [bytes(data)]

def _hook_open(path, mode="r", *a, **kw):
    if path == "/work/guests.olk" and "w" in mode:
        return _GuestSink()
    return _orig_open(path, mode, *a, **kw)

builtins.open = _hook_open
try:
    roster.build_roster(
        "/work/xb/root.olk",
        "/work/gc/root.olk" if ${gcRoot ? "True" : "False"} else None,
        "/work/ps2/root.olk" if ${ps2Root ? "True" : "False"} else None,
        "/work/guests.olk",
    )
finally:
    builtins.open = _orig_open

js.__emitGuestsParts(_captured_parts)
`);

    const guestsBlob = new Blob(outputParts, { type: "application/octet-stream" });
    self.postMessage({ type: "done", guestsBlob });
  } catch (err) {
    self.postMessage({ type: "error", message: err && err.message ? err.message : String(err) });
  }
};
