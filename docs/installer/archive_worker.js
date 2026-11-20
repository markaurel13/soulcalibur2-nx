import SevenZip from "https://cdn.jsdelivr.net/npm/7z-wasm@1.2.0/7zz.es6.js";

function collectExtractedFiles(sz, dir, validSet, outFiles, onStep) {
  for (const entry of sz.FS.readdir(dir)) {
    if (entry === "." || entry === "..") continue;
    const fullPath = `${dir}/${entry}`;
    const stat = sz.FS.stat(fullPath);
    if (sz.FS.isDir(stat.mode)) {
      collectExtractedFiles(sz, fullPath, validSet, outFiles, onStep);
      try { sz.FS.rmdir(fullPath); } catch {}
    } else {
      const low = entry.toLowerCase();
      if (low.endsWith(".dds") || low.endsWith(".png")) {
        const stem = low.slice(0, -4);
        if (!validSet || validSet.has(stem)) {
          const bytes = sz.FS.readFile(fullPath);
          outFiles.push(new File([bytes], entry, { type: "application/octet-stream" }));
          if (onStep) onStep(outFiles.length);
        }
      }
      try { sz.FS.unlink(fullPath); } catch {}
    }
  }
}

self.onmessage = async (e) => {
  const { archiveFile, validStems } = e.data;
  try {
    self.postMessage({ type: "log", message: `Initializing 7z WASM to extract ${archiveFile.name}...` });
    let lineBuf = "";
    let lastPct = -1;
    const sz = await SevenZip({
      locateFile: (file) => `https://cdn.jsdelivr.net/npm/7z-wasm@1.2.0/${file}`,
      stdout: (code) => {
        if (code === 37) {
          const m = lineBuf.match(/(\d{1,3})$/);
          if (m) {
            const pct = Math.min(100, parseInt(m[1], 10));
            if (pct !== lastPct) {
              lastPct = pct;
              self.postMessage({ type: "progress", pct });
            }
          }
          lineBuf = "";
        } else if (code === 13 || code === 10 || code === 8) {
          lineBuf = "";
        } else if (code >= 32 && code <= 126) {
          lineBuf += String.fromCharCode(code);
          if (lineBuf.length > 32) lineBuf = lineBuf.slice(-16);
        }
      },
      printErr: () => {},
    });

    sz.FS.mkdir("/in");
    sz.FS.mkdir("/out");
    sz.FS.mount(sz.WORKERFS, { files: [archiveFile] }, "/in");

    self.postMessage({ type: "progress", pct: 0 });
    self.postMessage({ type: "log", message: `Extracting .dds/.png textures from ${archiveFile.name}...` });
    sz.callMain(["x", `/in/${archiveFile.name}`, "-o/out", "-y", "-bsp1", "-r", "*.dds", "*.png"]);

    self.postMessage({ type: "log", message: "Indexing extracted HD textures..." });
    const validSet = validStems ? new Set(validStems) : null;
    const outFiles = [];
    collectExtractedFiles(sz, "/out", validSet, outFiles, (count) => {
      if (count % 500 === 0) {
        self.postMessage({ type: "collect", count });
      }
    });

    try { sz.FS.unmount("/in"); } catch {}
    self.postMessage({ type: "done", files: outFiles });
  } catch (err) {
    self.postMessage({
      type: "error",
      error: err && err.message ? err.message : String(err),
    });
  }
};
