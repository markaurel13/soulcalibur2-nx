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

    self.__postProgress = (pct, label) => {
      postProgress(pct, label);
      postLog(label);
    };

    // ---- V8 JIT accelerators for sound.py & textures.py hotspots (~97% of runtime) ----
    const IMA_STEPS = new Int32Array([
      7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88,
      97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
      724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660,
      4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818,
      18500, 20350, 22385, 24623, 27086, 29794, 32767,
    ]);
    const IMA_INDEX = new Int32Array([-1, -1, -1, -1, 2, 4, 6, 8]);

    self.__dspDecode = (dataProxy, coefsProxy, samples) => {
      const data = dataProxy.getBuffer("u8");
      const coefs = coefsProxy.toJs();
      try {
        const u8 = data.data;
        const out = new Int16Array(samples);
        let outLen = 0, h1 = 0, h2 = 0, frame = 0;
        while (outLen < samples) {
          const header = u8[frame];
          const scale = 1 << (header & 15);
          const index = header >> 4;
          const c1 = coefs[2 * index];
          const c2 = coefs[2 * index + 1];
          for (let i = 0; i < 14 && outLen < samples; i++) {
            const b = u8[frame + 1 + (i >> 1)];
            let nibble = (i & 1) === 0 ? (b >> 4) : (b & 15);
            if (nibble >= 8) nibble -= 16;
            let value = (((nibble * scale) << 11) + 1024 + c1 * h1 + c2 * h2) >> 11;
            if (value < -32768) value = -32768;
            else if (value > 32767) value = 32767;
            out[outLen++] = value;
            h2 = h1;
            h1 = value;
          }
          frame += 8;
        }
        return out;
      } finally {
        data.release();
      }
    };

    self.__xboxEncode = (samplesArg) => {
      let samples;
      if (samplesArg instanceof Int16Array) {
        samples = samplesArg;
      } else {
        samples = samplesArg.toJs();
      }
      const n = samples.length;
      const numBlocks = Math.ceil(n / 64);
      const out = new Uint8Array(numBlocks * 36);
      let index = 0, pos = 0, outOff = 0;
      const block = new Int32Array(65);
      const codes = new Uint8Array(64);
      while (pos < n) {
        const avail = Math.min(65, n - pos);
        for (let i = 0; i < avail; i++) block[i] = samples[pos + i];
        const padVal = avail > 0 ? block[avail - 1] : 0;
        for (let i = avail; i < 65; i++) block[i] = padVal;
        let predictor = block[0];
        out[outOff] = predictor & 0xff;
        out[outOff + 1] = (predictor >> 8) & 0xff;
        out[outOff + 2] = index & 0xff;
        out[outOff + 3] = 0;
        outOff += 4;
        for (let j = 1; j < 65; j++) {
          const s = block[j];
          const step = IMA_STEPS[index];
          let diff = s - predictor;
          let nibble = 0;
          if (diff < 0) { nibble = 8; diff = -diff; }
          if (diff >= step) { nibble |= 4; diff -= step; }
          if (diff >= (step >> 1)) { nibble |= 2; diff -= (step >> 1); }
          if (diff >= (step >> 2)) { nibble |= 1; }
          let pDiff = step >> 3;
          if (nibble & 4) pDiff += step;
          if (nibble & 2) pDiff += step >> 1;
          if (nibble & 1) pDiff += step >> 2;
          predictor = (nibble & 8) ? (predictor - pDiff) : (predictor + pDiff);
          if (predictor < -32768) predictor = -32768;
          else if (predictor > 32767) predictor = 32767;
          index += IMA_INDEX[nibble & 7];
          if (index < 0) index = 0;
          else if (index > 88) index = 88;
          codes[j - 1] = nibble;
        }
        for (let g = 0; g < 64; g += 8) {
          for (let k = 0; k < 8; k += 2) {
            out[outOff++] = codes[g + k] | (codes[g + k + 1] << 4);
          }
        }
        pos += 64;
      }
      return out;
    };

    const REV_PAIRS = new Uint8Array(256);
    for (let i = 0; i < 256; i++) {
      REV_PAIRS[i] = ((i & 0x03) << 6) | ((i & 0x0c) << 2) | ((i & 0x30) >> 2) | ((i & 0xc0) >> 6);
    }

    self.__cmprBlocks = (dataProxy, width, height) => {
      const buf = dataProxy.getBuffer("u8");
      try {
        const src = buf.data;
        const bw = Math.max(1, (width + 3) >> 2);
        const bh = Math.max(1, (height + 3) >> 2);
        const out = new Uint8Array(bw * bh * 8);
        let at = 0;
        for (let ty = 0; ty < bh; ty += 2) {
          for (let tx = 0; tx < bw; tx += 2) {
            for (let sub = 0; sub < 4; sub++) {
              const bx = tx + (sub & 1);
              const by = ty + (sub >> 1);
              if (bx < bw && by < bh) {
                const dst = (by * bw + bx) * 8;
                out[dst] = src[at + 1];
                out[dst + 1] = src[at];
                out[dst + 2] = src[at + 3];
                out[dst + 3] = src[at + 2];
                out[dst + 4] = REV_PAIRS[src[at + 4]];
                out[dst + 5] = REV_PAIRS[src[at + 5]];
                out[dst + 6] = REV_PAIRS[src[at + 6]];
                out[dst + 7] = REV_PAIRS[src[at + 7]];
              }
              at += 8;
            }
          }
        }
        return out;
      } finally {
        buf.release();
      }
    };

    self.__dxt5FromCmprAlpha = (blocks, ablocks, width, height) => {
      const bw = Math.max(1, (width + 3) >> 2);
      const grey = new Uint8Array(width * height);
      const gPal = new Uint8Array(4);
      for (let by = 0; by < height; by += 4) {
        for (let bx = 0; bx < width; bx += 4) {
          const p = ((by >> 2) * bw + (bx >> 2)) * 8;
          const c0 = ablocks[p] | (ablocks[p + 1] << 8);
          const c1 = ablocks[p + 2] | (ablocks[p + 3] << 8);
          const g0_6 = (c0 >> 5) & 63;
          const g1_6 = (c1 >> 5) & 63;
          const g0 = (g0_6 << 2) | (g0_6 >> 4);
          const g1 = (g1_6 << 2) | (g1_6 >> 4);
          gPal[0] = g0;
          gPal[1] = g1;
          if (c0 > c1) {
            gPal[2] = Math.floor((2 * g0 + g1) / 3);
            gPal[3] = Math.floor((g0 + 2 * g1) / 3);
          } else {
            gPal[2] = Math.floor((g0 + g1) / 2);
            gPal[3] = 0;
          }
          const bits = (ablocks[p + 4] | (ablocks[p + 5] << 8) | (ablocks[p + 6] << 16) | (ablocks[p + 7] << 24)) >>> 0;
          for (let ty = 0; ty < 4 && by + ty < height; ty++) {
            const rowOff = (by + ty) * width;
            for (let tx = 0; tx < 4 && bx + tx < width; tx++) {
              const idx = (bits >>> (2 * (4 * ty + tx))) & 3;
              grey[rowOff + bx + tx] = gPal[idx];
            }
          }
        }
      }

      const numBlocks = blocks.length >> 3;
      const out = new Uint8Array(numBlocks * 16);
      const vals = new Uint8Array(16);
      const levels = new Int32Array(8);
      for (let b = 0; b < numBlocks; b++) {
        const bx = (b % bw) * 4;
        const by = Math.floor(b / bw) * 4;
        let hi = 0, lo = 255, k = 0;
        for (let y = 0; y < 4; y++) {
          const cy = Math.min(by + y, height - 1) * width;
          for (let x = 0; x < 4; x++) {
            const v = grey[cy + Math.min(bx + x, width - 1)];
            vals[k++] = v;
            if (v > hi) hi = v;
            if (v < lo) lo = v;
          }
        }
        const dst = b * 16;
        out[dst] = hi;
        out[dst + 1] = lo;
        if (hi !== lo) {
          levels[0] = hi;
          levels[1] = lo;
          for (let i = 1; i < 7; i++) levels[i + 1] = Math.floor(((7 - i) * hi + i * lo) / 7);
          let bitsLow = 0, bitsHigh = 0;
          for (let n = 0; n < 16; n++) {
            const v = vals[n];
            let bestI = 0, bestD = Math.abs(levels[0] - v);
            for (let i = 1; i < 8; i++) {
              const d = Math.abs(levels[i] - v);
              if (d < bestD) { bestD = d; bestI = i; }
            }
            const bitPos = 3 * n;
            if (bitPos < 24) bitsLow |= (bestI << bitPos);
            else bitsHigh |= (bestI << (bitPos - 24));
          }
          out[dst + 2] = bitsLow & 0xff;
          out[dst + 3] = (bitsLow >> 8) & 0xff;
          out[dst + 4] = (bitsLow >> 16) & 0xff;
          out[dst + 5] = bitsHigh & 0xff;
          out[dst + 6] = (bitsHigh >> 8) & 0xff;
          out[dst + 7] = (bitsHigh >> 16) & 0xff;
        }
        const src = b * 8;
        for (let j = 0; j < 8; j++) out[dst + 8 + j] = blocks[src + j];
      }
      return out;
    };

    postProgress(35, "Building guests.olk (Link & Heihachi)...");
    postLog("Running roster.py (streaming Blob-backed Olnk + JS JIT accelerators)...");

    await pyodide.runPythonAsync(`
import sys, struct, builtins
import js
sys.path.insert(0, "/work/scripts")
import olnk
import sound
import textures
import build_guest
import roster

sound.dsp_decode = lambda data, coefs, samples: js.__dspDecode(data, coefs, samples)
sound.xbox_encode = lambda samples: bytes(js.__xboxEncode(samples).to_py())

_orig_convert_texture = textures.convert_texture
def _fast_convert_texture(vgt, base, entry):
    colour, alpha = entry[3], entry[11]
    fmt, width, height = textures._gx(colour)
    afmt = textures._gx(alpha)[0] if alpha else None
    if fmt == textures.CMPR and afmt == textures.CMPR:
        levels = ((entry[2] >> 8) & 0xff) // 16 + 1
        at = base + entry[4]
        alpha_at = base + entry[12]
        out = bytearray()
        mv = memoryview(vgt)
        for level in range(levels):
            w, h = textures._level(width, level), textures._level(height, level)
            sz = textures.cmpr_size(w, h)
            blocks = js.__cmprBlocks(mv[at:at + sz], w, h)
            at += sz
            ablocks = js.__cmprBlocks(mv[alpha_at:alpha_at + sz], w, h)
            alpha_at += sz
            out += bytes(js.__dxt5FromCmprAlpha(blocks, ablocks, w, h).to_py())
        return textures.D3D_DXT5, levels, bytes(out)
    return _orig_convert_texture(vgt, base, entry)

textures.convert_texture = _fast_convert_texture

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

_orig_link_block = build_guest.link_block
_orig_link_costume = build_guest._link_costume
_orig_weapon = build_guest._weapon
_orig_link_rest = build_guest._link_rest
_orig_guest_ui = build_guest.guest_ui
_orig_hei_block = build_guest.heihachi_block
_orig_ps2_weapon = build_guest._ps2_weapon
_orig_hei_ui = build_guest.heihachi_ui

_state_prog = {"costume": 0, "weapon": 0, "ps2_weapon": 0}

def _wrap_link_block(*a, **kw):
    _state_prog["costume"] = 0
    _state_prog["weapon"] = 0
    js.__postProgress(38, "Converting Link (models, moves, sounds)...")
    return _orig_link_block(*a, **kw)

def _wrap_link_costume(*a, **kw):
    _state_prog["costume"] += 1
    n = _state_prog["costume"]
    js.__postProgress(38 + n * 2, f"Converting Link costume {n}/4...")
    return _orig_link_costume(*a, **kw)

def _wrap_weapon(*a, **kw):
    _state_prog["weapon"] += 1
    n = _state_prog["weapon"]
    if n > 4:  # first 4 are inside costumes
        js.__postProgress(46 + min(8, (n - 4) * 0.6), f"Converting Link weapon {n - 4}/12...")
    return _orig_weapon(*a, **kw)

def _wrap_link_rest(*a, **kw):
    js.__postProgress(46, "Converting Link weapons, movesets & sound banks...")
    return _orig_link_rest(*a, **kw)

def _wrap_guest_ui(*a, **kw):
    js.__postProgress(56, "Converting Link (UI & gallery textures)...")
    return _orig_guest_ui(*a, **kw)

def _wrap_hei_block(*a, **kw):
    _state_prog["ps2_weapon"] = 0
    js.__postProgress(60, "Converting Heihachi (models & sounds)...")
    return _orig_hei_block(*a, **kw)

def _wrap_ps2_weapon(*a, **kw):
    _state_prog["ps2_weapon"] += 1
    n = _state_prog["ps2_weapon"]
    js.__postProgress(60 + min(10, n * 0.7), f"Converting Heihachi weapon/part {n}...")
    return _orig_ps2_weapon(*a, **kw)

def _wrap_hei_ui(*a, **kw):
    js.__postProgress(72, "Converting Heihachi (UI & gallery textures)...")
    return _orig_hei_ui(*a, **kw)

build_guest.link_block = _wrap_link_block
build_guest._link_costume = _wrap_link_costume
build_guest._weapon = _wrap_weapon
build_guest._link_rest = _wrap_link_rest
build_guest.guest_ui = _wrap_guest_ui
build_guest.heihachi_block = _wrap_hei_block
build_guest._ps2_weapon = _wrap_ps2_weapon
build_guest.heihachi_ui = _wrap_hei_ui

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

js.__postProgress(74, "Assembling guests.olk...")
js.__emitGuestsParts(_captured_parts)
`);

    const guestsBlob = new Blob(outputParts, { type: "application/octet-stream" });
    self.postMessage({ type: "done", guestsBlob });
  } catch (err) {
    self.postMessage({ type: "error", message: err && err.message ? err.message : String(err) });
  }
};
