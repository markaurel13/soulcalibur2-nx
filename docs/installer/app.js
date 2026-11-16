import { DiscParsers } from "./xiso.js?v=20261115b";
import { downloadZip } from "https://cdn.jsdelivr.net/npm/client-zip@2.4.5/+esm";

const REPO_OWNER = "markaurel13";
const REPO_NAME = "soulcalibur2-nx";
const REPO_BRANCH = "switch-port";

const GUEST_PY_FILES = [
  "build_guest.py",
  "gs.py",
  "mesh.py",
  "motion.py",
  "olnk.py",
  "ps2tex.py",
  "records.py",
  "roster.py",
  "sound.py",
  "textures.py",
  "vmg.py",
  "vmg2vmx.py",
  "vmp.py",
  "vmp2vmx.py",
  "vmx.py",
];

const state = {
  nroBlob: null,
  xbox: {
    "default.xbe": null, // { name, blob }
    "root.olk": null,
    "movie.afs": null,
    "bgm.afs": null,
  },
  gc: {
    "root.olk": null,
    "boot.dol": null,
  },
  ps2: {
    "root.olk": null,
    "slus_206.43": null,
  },
  guestsBlob: null,
  pyodide: null,
  isBuilding: false,
};

const $ = (id) => document.getElementById(id);

function formatBytes(bytes) {
  if (!bytes) return "0 B";
  const units = ["B", "KB", "MB", "GB"];
  let idx = 0;
  let val = bytes;
  while (val >= 1024 && idx < units.length - 1) {
    val /= 1024;
    idx++;
  }
  return `${val.toFixed(idx === 0 ? 0 : 1)} ${units[idx]}`;
}

function log(msg, level = "info") {
  const box = $("log-output");
  const line = document.createElement("div");
  line.className = `log-line log-${level}`;
  const ts = new Date().toLocaleTimeString();
  line.textContent = `[${ts}] ${msg}`;
  box.appendChild(line);
  box.scrollTop = box.scrollHeight;
}

function showRegionPopup(title, message) {
  $("modal-title").textContent = title;
  $("modal-message").textContent = message;
  $("region-modal").hidden = false;
}

function setProgress(pct, label) {
  $("progress-wrap").hidden = false;
  $("progress-bar").style.width = `${Math.max(0, Math.min(100, pct))}%`;
  $("progress-label").textContent = label;
}

function updateUI() {
  const reqKeys = ["default.xbe", "root.olk", "movie.afs", "bgm.afs"];
  const readyXbox = reqKeys.every((k) => state.xbox[k] !== null);
  const readyGc = state.gc["root.olk"] !== null && state.gc["boot.dol"] !== null;
  const readyPs2 = state.ps2["root.olk"] !== null && state.ps2["slus_206.43"] !== null;

  $("xbox-status").innerHTML = reqKeys
    .map((k) => {
      const item = state.xbox[k];
      return item
        ? `<span class="pill ok">✓ ${item.name} (${formatBytes(item.blob.size)})</span>`
        : `<span class="pill missing">✗ ${k}</span>`;
    })
    .join("");

  $("gc-status").innerHTML = readyGc
    ? `<span class="pill ok">✓ Link ready (~20.3 MB)</span>`
    : `<span class="pill missing">✗ GameCube ISO (Link)</span>`;

  $("ps2-status").innerHTML = readyPs2
    ? `<span class="pill ok">✓ Heihachi ready (~15.9 MB)</span>`
    : `<span class="pill missing">✗ PS2 ISO (Heihachi)</span>`;

  const canBuild = readyXbox && readyGc && readyPs2;
  $("btn-build-zip").disabled = !canBuild || state.isBuilding;

  const hint = $("final-size-hint");
  if (readyXbox) {
    const xboxOtherBytes = ["default.xbe", "movie.afs", "bgm.afs"].reduce(
      (acc, k) => acc + state.xbox[k].blob.size,
      0
    );
    const guestsBytes = state.guestsBlob
      ? state.guestsBlob.size
      : state.xbox["root.olk"].blob.size + 38 * 1024 * 1024;
    const nroBytes = state.nroBlob ? state.nroBlob.blob.size : 23 * 1024 * 1024;
    const prefix = state.guestsBlob ? "Final package size" : "Estimated final size";
    hint.textContent = `${prefix}: ~${formatBytes(xboxOtherBytes + guestsBytes + nroBytes)}`;
  } else {
    hint.textContent = "Estimated final size: ~1.34 GB";
  }
}

// ---- 1. Handle Xbox Disc ----
async function handleXboxInput(fileList) {
  const file = fileList[0];
  if (!file) return;
  const low = file.name.toLowerCase();
  if (!low.endsWith(".iso") && !low.endsWith(".xiso")) {
    log(`Unsupported file '${file.name}'. Please provide an Xbox .iso or .xiso file.`, "error");
    return;
  }

  log(`Reading XISO structure of '${file.name}' (${formatBytes(file.size)})...`);
  try {
    const entries = await DiscParsers.parseXboxIso(file);
    const mapping = {
      "default.xbe": "Default.xbe",
      "root.olk": "root.olk",
      "movie.afs": "movie.afs",
      "bgm.afs": "bgm.afs",
    };
    let foundCount = 0;
    for (const [key, outName] of Object.entries(mapping)) {
      const entry = entries[key];
      state.xbox[key] = entry
        ? { name: outName, blob: file.slice(entry.offset, entry.offset + entry.size) }
        : null;
      if (entry) foundCount++;
    }
    if (foundCount === 0) {
      log("Default.xbe / root.olk / movie.afs / bgm.afs were not found inside the ISO.", "error");
    } else {
      const xbeEntry = entries["default.xbe"];
      if (xbeEntry) {
        const xbeHdr = await file.slice(xbeEntry.offset, xbeEntry.offset + Math.min(xbeEntry.size, 8192)).arrayBuffer();
        if (xbeHdr.byteLength >= 0x120) {
          const dv = new DataView(xbeHdr);
          const magic = String.fromCharCode(...new Uint8Array(xbeHdr, 0, 4));
          if (magic === "XBEH") {
            const baseAddr = dv.getUint32(0x104, true);
            const certAddr = dv.getUint32(0x118, true);
            const certOff = certAddr - baseAddr;
            if (certAddr >= baseAddr && certOff + 0xa4 <= xbeHdr.byteLength) {
              const gameRegion = dv.getUint32(certOff + 0xa0, true);
              if ((gameRegion & 0x00000001) === 0) {
                for (const k of Object.keys(state.xbox)) state.xbox[k] = null;
                const msg = "This Xbox ISO is from another region (PAL or Japan). Please provide a USA (NTSC-U) copy of Soulcalibur II.";
                log(msg, "error");
                showRegionPopup("Unsupported Xbox Region", msg);
                updateUI();
                return;
              }
            }
          }
        }
      }
      log(`Xbox ISO parsed: ${foundCount}/4 files ready.`, "ok");
    }
  } catch (err) {
    log(`Failed to read Xbox ISO: ${err.message}`, "error");
  }
  updateUI();
}

// ---- 2. Handle GameCube / PS2 Discs ----
async function handleGameCubeInput(fileList) {
  const file = fileList[0];
  if (!file) return;
  const low = file.name.toLowerCase();
  if (!low.endsWith(".iso") && !low.endsWith(".gcm") && !low.endsWith(".ciso")) {
    log(`Unsupported file '${file.name}'. Please provide a GameCube .iso, .gcm, or .ciso file.`, "error");
    return;
  }

  log(`Reading GameCube ISO '${file.name}'...`);
  try {
    const entries = await DiscParsers.parseGameCubeIso(file);
    if (!entries["root.olk"] || !entries["boot.dol"]) {
      throw new Error("Missing root.olk or boot.dol in the GameCube ISO.");
    }
    state.gc["root.olk"] = {
      name: "root.olk",
      blob: entries["root.olk"].blob,
    };
    state.gc["boot.dol"] = {
      name: "boot.dol",
      blob: entries["boot.dol"].blob,
    };
    state.guestsBlob = null;
    log("GameCube ISO ready to extract Link.", "ok");
  } catch (err) {
    log(`Failed to read GameCube ISO: ${err.message}`, "error");
    if (err.message.includes("USA (NTSC-U)")) {
      showRegionPopup(
        "Unsupported GameCube Region",
        "This GameCube ISO is not a USA (NTSC-U) copy (GRSEAF). Please provide a USA copy of Soulcalibur II."
      );
    }
  }
  updateUI();
}

async function handlePs2Input(fileList) {
  const file = fileList[0];
  if (!file) return;
  const low = file.name.toLowerCase();
  if (!low.endsWith(".iso")) {
    log(`Unsupported file '${file.name}'. Please provide a PS2 .iso file.`, "error");
    return;
  }

  log(`Reading PS2 ISO '${file.name}'...`);
  try {
    const entries = await DiscParsers.parsePs2Iso(file);
    if (!entries["root.olk"] || !entries["slus_206.43"]) {
      throw new Error("Only USA (NTSC-U) PS2 copies are supported (ROOT.OLK or SLUS_206.43 not found).");
    }
    state.ps2["root.olk"] = {
      name: "root.olk",
      blob: file.slice(entries["root.olk"].offset, entries["root.olk"].offset + entries["root.olk"].size),
    };
    state.ps2["slus_206.43"] = {
      name: "SLUS_206.43",
      blob: file.slice(entries["slus_206.43"].offset, entries["slus_206.43"].offset + entries["slus_206.43"].size),
    };
    state.guestsBlob = null;
    log("PS2 ISO ready to extract Heihachi.", "ok");
  } catch (err) {
    log(`Failed to read PS2 ISO: ${err.message}`, "error");
    if (err.message.includes("USA (NTSC-U)")) {
      showRegionPopup(
        "Unsupported PS2 Region",
        "This PS2 ISO is not a USA (NTSC-U) copy (SLUS_206.43). Please provide a USA copy of Soulcalibur II."
      );
    }
  }
  updateUI();
}

// ---- 4. Build guests.olk in Pyodide (if GC/PS2 provided) ----
async function fetchGuestScript(filename) {
  const localUrl = `../tools/guests/${filename}`;
  const rawUrl = `https://raw.githubusercontent.com/${REPO_OWNER}/${REPO_NAME}/${REPO_BRANCH}/tools/guests/${filename}`;
  for (const url of [localUrl, rawUrl]) {
    try {
      const r = await fetch(url);
      if (r.ok) return await r.text();
    } catch (_) {
      // try next
    }
  }
  throw new Error(`Could not fetch script tools/guests/${filename}`);
}

async function buildGuestsIfNeeded() {
  const hasGc = state.gc["root.olk"] && state.gc["boot.dol"];
  const hasPs2 = state.ps2["root.olk"] && state.ps2["slus_206.43"];
  if (!hasGc && !hasPs2) return null;
  if (state.guestsBlob) return state.guestsBlob;

  setProgress(10, "Fetching conversion scripts (tools/guests)...");
  log("Initializing Pyodide worker to build guests.olk (streaming, low-RAM)...");

  const pyFiles = {};
  await Promise.all(
    GUEST_PY_FILES.map(async (f) => {
      pyFiles[f] = await fetchGuestScript(f);
    })
  );

  const guestsBlob = await new Promise((resolve, reject) => {
    const worker = new Worker("./installer/guest_worker.js?v=20261115h");
    worker.onmessage = (ev) => {
      const msg = ev.data;
      if (msg.type === "progress") {
        setProgress(msg.pct, msg.label);
      } else if (msg.type === "log") {
        log(msg.msg, msg.level || "info");
      } else if (msg.type === "done") {
        worker.terminate();
        resolve(msg.guestsBlob);
      } else if (msg.type === "error") {
        worker.terminate();
        reject(new Error(msg.message));
      }
    };
    worker.onerror = (err) => {
      worker.terminate();
      reject(new Error(err.message || "Guest builder worker failed."));
    };
    worker.postMessage({
      xbRoot: state.xbox["root.olk"].blob,
      gcRoot: hasGc ? state.gc["root.olk"].blob : null,
      gcDol: hasGc ? state.gc["boot.dol"].blob : null,
      ps2Root: hasPs2 ? state.ps2["root.olk"].blob : null,
      ps2Elf: hasPs2 ? state.ps2["slus_206.43"].blob : null,
      pyFiles,
    });
  });

  state.guestsBlob = guestsBlob;
  log(`guests.olk built successfully (${formatBytes(state.guestsBlob.size)}).`, "ok");
  return state.guestsBlob;
}

// ---- 5. Streaming File Writer (Directory Picker) ----
async function writeBlobStreaming(dirHandle, filename, blob, onChunk) {
  const fileHandle = await dirHandle.getFileHandle(filename, { create: true });
  const writable = await fileHandle.createWritable();
  const CHUNK_SIZE = 16 * 1024 * 1024;
  let offset = 0;
  try {
    while (offset < blob.size) {
      const end = Math.min(blob.size, offset + CHUNK_SIZE);
      const chunk = await blob.slice(offset, end).arrayBuffer();
      await writable.write(chunk);
      offset = end;
      if (onChunk) onChunk(offset);
    }
  } finally {
    await writable.close();
  }
}

async function exportToDirectory() {
  if (!window.showDirectoryPicker) {
    alert("Your browser does not support direct folder writing. Use Chrome/Edge/Brave or 'Generate Individual Downloads'.");
    return;
  }

  try {
    const picked = await window.showDirectoryPicker({ mode: "readwrite" });
    let targetDir = picked;
    if (picked.name.toLowerCase() !== "soulcalibur2") {
      const switchDir =
        picked.name.toLowerCase() === "switch"
          ? picked
          : await picked.getDirectoryHandle("switch", { create: true });
      targetDir = await switchDir.getDirectoryHandle("soulcalibur2", { create: true });
    }
    await targetDir.getDirectoryHandle("save", { create: true });

    const guests = await buildGuestsIfNeeded();

    const queue = [
      state.xbox["default.xbe"],
      state.xbox["root.olk"],
      state.xbox["movie.afs"],
      state.xbox["bgm.afs"],
    ];
    if (guests) queue.push({ name: "guests.olk", blob: guests });
    if (state.nroBlob) queue.push(state.nroBlob);

    const totalBytes = queue.reduce((acc, f) => acc + f.blob.size, 0);
    let writtenBytes = 0;

    for (const item of queue) {
      log(`Writing ${item.name} (${formatBytes(item.blob.size)}) to switch/soulcalibur2/...`);
      await writeBlobStreaming(targetDir, item.name, item.blob, (chunkEnd) => {
        const pct = ((writtenBytes + chunkEnd) / totalBytes) * 100;
        setProgress(pct, `Copying ${item.name}... (${pct.toFixed(0)}%)`);
      });
      writtenBytes += item.blob.size;
    }

    setProgress(100, "Installation complete in switch/soulcalibur2/!");
    log("Done! Insert your microSD card into your Nintendo Switch and launch Soulcalibur II NX.", "ok");
  } catch (err) {
    if (err.name !== "AbortError") {
      log(`Error exporting files: ${err.message}`, "error");
    }
  }
}

async function buildSwitchPackageZip() {
  if (state.isBuilding) return;
  state.isBuilding = true;
  const btn = $("btn-build-zip");
  btn.textContent = "⏳ Building Switch Package...";
  updateUI();
  try {
    const guests = await buildGuestsIfNeeded();
    if (!guests) {
      throw new Error("Both GameCube and PS2 discs are required to build guests.olk.");
    }

    const entries = [
      { name: "switch/soulcalibur2/Default.xbe", input: state.xbox["default.xbe"].blob },
      { name: "switch/soulcalibur2/movie.afs", input: state.xbox["movie.afs"].blob },
      { name: "switch/soulcalibur2/bgm.afs", input: state.xbox["bgm.afs"].blob },
      { name: "switch/soulcalibur2/guests.olk", input: guests },
    ];
    if (state.nroBlob) {
      entries.push({ name: `switch/soulcalibur2/${state.nroBlob.name}`, input: state.nroBlob.blob });
    }

    const totalBytes = entries.reduce((acc, e) => acc + e.input.size, 0);
    setProgress(75, "Packaging switch/soulcalibur2/ into ZIP...");
    log(`Packaging Switch ZIP (${formatBytes(totalBytes)})...`);

    const zipResponse = downloadZip(entries);

    if (window.showSaveFilePicker) {
      const handle = await window.showSaveFilePicker({
        suggestedName: "soulcalibur2_nx_package.zip",
        types: [{ description: "ZIP Archive", accept: { "application/zip": [".zip"] } }],
      });
      const writable = await handle.createWritable();
      let written = 0;
      const reader = zipResponse.body.getReader();
      try {
        while (true) {
          const { done, value } = await reader.read();
          if (done) break;
          await writable.write(value);
          written += value.byteLength;
          const pct = 75 + Math.min(25, (written / totalBytes) * 25);
          setProgress(pct, `Saving soulcalibur2_nx_package.zip... (${formatBytes(written)})`);
        }
      } finally {
        await writable.close();
      }
      setProgress(100, "Switch package saved! Extract it to the root of your microSD card.");
      log("Done! soulcalibur2_nx_package.zip saved (switch/soulcalibur2/ structure).", "ok");
    } else {
      const zipBlob = await zipResponse.blob();
      const url = URL.createObjectURL(zipBlob);
      const a = document.createElement("a");
      a.href = url;
      a.download = "soulcalibur2_nx_package.zip";
      document.body.appendChild(a);
      a.click();
      a.remove();
      setTimeout(() => URL.revokeObjectURL(url), 60000);
      setProgress(100, "Switch package downloaded! Extract it to the root of your microSD card.");
      log("Done! soulcalibur2_nx_package.zip download started.", "ok");
    }
  } catch (err) {
    if (err.name !== "AbortError") {
      log(`Error building Switch package: ${err.message}`, "error");
    }
  } finally {
    state.isBuilding = false;
    btn.textContent = "⚡ Build Switch Package";
    updateUI();
  }
}

function wireDropzone(zoneId, inputId, handler) {
  const zone = $(zoneId);
  const input = $(inputId);
  zone.addEventListener("click", () => input.click());
  input.addEventListener("change", (e) => {
    if (e.target.files.length) handler(e.target.files);
  });
  zone.addEventListener("dragover", (e) => {
    e.preventDefault();
    zone.classList.add("dragover");
  });
  zone.addEventListener("dragleave", () => zone.classList.remove("dragover"));
  zone.addEventListener("drop", (e) => {
    e.preventDefault();
    zone.classList.remove("dragover");
    if (e.dataTransfer.files.length) handler(e.dataTransfer.files);
  });
}

document.addEventListener("DOMContentLoaded", () => {
  wireDropzone("drop-xbox", "file-xbox", handleXboxInput);
  wireDropzone("drop-gc", "file-gc", handleGameCubeInput);
  wireDropzone("drop-ps2", "file-ps2", handlePs2Input);
  $("btn-build-zip").addEventListener("click", buildSwitchPackageZip);
  $("modal-close").addEventListener("click", () => ($("region-modal").hidden = true));
  $("region-modal").addEventListener("click", (e) => {
    if (e.target === $("region-modal")) $("region-modal").hidden = true;
  });
  document.addEventListener("keydown", (e) => {
    if (e.key === "Escape") $("region-modal").hidden = true;
  });
  updateUI();
});
