import { DiscParsers } from "./xiso.js?v=20261115m";
import { loadTextureMap, planTexturePack, buildTextureDdsBytes } from "./tex_convert.js?v=20261118f";
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
  textureJobs: [],
  textureDirectCount: 0,
  textureConvertCount: 0,
  texturesBytes: 0,
  textureMap: null,
  zipUrl: null,
  texZipUrl: null,
  packageBuilt: false,
  pyodide: null,
  isBuilding: false,
  isScanningTextures: false,
  textureScanText: "",
};

function resetBuiltPackage() {
  state.packageBuilt = false;
  if (state.zipUrl) {
    URL.revokeObjectURL(state.zipUrl);
    state.zipUrl = null;
  }
  if (state.texZipUrl) {
    URL.revokeObjectURL(state.texZipUrl);
    state.texZipUrl = null;
  }
}

function triggerZipDownload(url, filename = "soulcalibur2_nx_package.zip") {
  const a = document.createElement("a");
  a.href = url;
  a.download = filename;
  document.body.appendChild(a);
  a.click();
  a.remove();
}

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

  const hasTextures = state.textureJobs.length > 0;
  if (state.isScanningTextures) {
    $("textures-status").innerHTML = `<span class="pill" style="background: rgba(251,191,36,0.12); border: 1px solid rgba(251,191,36,0.4); color: #fbbf24;">⏳ ${state.textureScanText || "Loading HD textures..."}</span>`;
  } else {
    $("textures-status").innerHTML = hasTextures
      ? `<span class="pill ok">✓ ${state.textureJobs.length} final textures (${state.textureConvertCount > 0 ? "~" : ""}${formatBytes(state.texturesBytes)})</span>`
      : `<span class="pill missing">✗ No HD textures selected</span>`;
  }

  const canBuild = readyXbox && readyGc && readyPs2;
  const btn = $("btn-build-zip");
  btn.disabled = !canBuild || state.isBuilding || state.isScanningTextures;
  if (state.isBuilding) {
    btn.textContent = "⏳ Building Switch Package...";
  } else if (state.isScanningTextures) {
    btn.textContent = "⏳ Loading HD Textures...";
  } else if (state.packageBuilt) {
    btn.textContent = "⬇ Download Again";
  } else {
    btn.textContent = "⚡ Build Switch Package";
  }

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
    hint.textContent = `${prefix}: ~${formatBytes(xboxOtherBytes + guestsBytes + nroBytes + state.texturesBytes)}`;
  } else {
    hint.textContent = state.texturesBytes > 0
      ? `Estimated final size: ~1.34 GB + ${formatBytes(state.texturesBytes)} HD textures`
      : "Estimated final size: ~1.34 GB";
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
    if (foundCount < 4) {
      log(
        foundCount === 0
          ? "Default.xbe / root.olk / movie.afs / bgm.afs were not found inside the ISO."
          : "This Xbox ISO is missing required Soulcalibur II files (Default.xbe, root.olk, movie.afs, bgm.afs).",
        "error"
      );
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
        if (window.crypto && crypto.subtle) {
          const xbeBytes = await file.slice(xbeEntry.offset, xbeEntry.offset + xbeEntry.size).arrayBuffer();
          const hashBuf = await crypto.subtle.digest("SHA-256", xbeBytes);
          const xbeSha = Array.from(new Uint8Array(hashBuf))
            .map((b) => b.toString(16).padStart(2, "0"))
            .join("");
          const EXPECTED_USA_XBE_SHA256 = "7ce12851850ab46766f77c612b251afe7c8bd81f879e11b4836bdf766be542c6";
          if (xbeSha !== EXPECTED_USA_XBE_SHA256) {
            for (const k of Object.keys(state.xbox)) state.xbox[k] = null;
            const msg =
              `This Xbox ISO's Default.xbe does not match Soulcalibur II USA (NTSC-U) Retail ` +
              `(got SHA-256 ${xbeSha.slice(0, 12)}..., expected ${EXPECTED_USA_XBE_SHA256.slice(0, 12)}...). ` +
              `Patched, region-free, PAL, or JPN discs are not supported.`;
            log(msg, "error");
            showRegionPopup("Unsupported Xbox ISO", msg);
            updateUI();
            return;
          }
        }
      }
      log(`Xbox ISO parsed: ${foundCount}/4 files ready.`, "ok");
      state.guestsBlob = null;
      resetBuiltPackage();
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
    resetBuiltPackage();
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
    if (entries["default.xbe"]) {
      throw new Error("This looks like an Xbox ISO, not a PlayStation 2 ISO.");
    }
    if (!entries["root.olk"]) {
      throw new Error("Not a valid Soulcalibur II PS2 ISO (ROOT.OLK not found).");
    }
    if (!entries["slus_206.43"]) {
      const hasPs2Boot = Object.keys(entries).some((k) =>
        /^(sles|slpm|slps|sces|scps|scus)_/.test(k) || k === "system.cnf"
      );
      if (hasPs2Boot) {
        throw new Error("Only USA (NTSC-U) PS2 copies are supported (SLUS_206.43 not found).");
      }
      throw new Error("Not a valid Soulcalibur II PS2 ISO (SLUS_206.43 not found).");
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
    resetBuiltPackage();
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
    const worker = new Worker("./installer/guest_worker.js?v=20261115p");
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

function createTextureConverter() {
  let worker = null;
  let nextId = 1;
  const pending = new Map();
  try {
    worker = new Worker("./installer/tex_worker.js?v=20261118f", { type: "module" });
    worker.onmessage = (e) => {
      const { id, bytes, error } = e.data;
      const entry = pending.get(id);
      if (!entry) return;
      pending.delete(id);
      if (error) entry.reject(new Error(error));
      else entry.resolve(bytes);
    };
    worker.onerror = (e) => {
      const err = new Error(e.message || "Texture worker error");
      for (const entry of pending.values()) entry.reject(err);
      pending.clear();
      worker.terminate();
      worker = null;
    };
  } catch {
    worker = null;
  }

  return {
    convert(job) {
      if (job.kind === "direct") return Promise.resolve(job.file);
      if (!worker) return buildTextureDdsBytes(job);
      return new Promise((resolve, reject) => {
        const id = nextId++;
        pending.set(id, { resolve, reject });
        worker.postMessage({ id, job });
      });
    },
    close() {
      if (worker) {
        worker.terminate();
        worker = null;
      }
      pending.clear();
    },
  };
}

async function buildSwitchPackageZip() {
  if (state.isBuilding) return;
  if (state.packageBuilt && state.zipUrl) {
    triggerZipDownload(state.zipUrl, "soulcalibur2_nx_package.zip");
    if (state.texZipUrl) {
      setTimeout(() => triggerZipDownload(state.texZipUrl, "soulcalibur2_nx_textures.zip"), 500);
      log("Downloading soulcalibur2_nx_package.zip and soulcalibur2_nx_textures.zip again...", "ok");
    } else {
      log("Downloading soulcalibur2_nx_package.zip again...", "ok");
    }
    return;
  }
  state.isBuilding = true;
  updateUI();
  try {
    let saveHandle = null;
    if (window.showSaveFilePicker) {
      try {
        saveHandle = await window.showSaveFilePicker({
          suggestedName: "soulcalibur2_nx_package.zip",
          types: [{ description: "ZIP Archive", accept: { "application/zip": [".zip"] } }],
        });
      } catch (pickerErr) {
        if (pickerErr.name === "AbortError") throw pickerErr;
        log(`Save picker unavailable (${pickerErr.message}); falling back to browser download.`, "warn");
      }
    }

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

    const totalBaseBytes = entries.reduce((acc, e) => acc + e.input.size, 0);
    const totalBytes = totalBaseBytes + state.texturesBytes;
    setProgress(75, "Packaging switch/soulcalibur2/ into ZIP...");
    log(`Packaging Switch ZIP (~${formatBytes(totalBytes)})...`);

    const totTex = state.textureJobs.length;
    const splitForBlobLimit = !saveHandle && totTex > 0 && totalBytes > 1850 * 1024 * 1024;
    const texConverter = state.textureConvertCount > 0 ? createTextureConverter() : null;
    let doneTex = 0;
    let written = 0;
    let nextLogPct = 25;
    let lastUiBytes = 0;

    async function* generateTextureEntries() {
      for (const job of state.textureJobs) {
        const input = job.kind === "direct"
          ? job.file
          : await texConverter.convert(job);
        doneTex++;
        if (doneTex % 50 === 0 || doneTex === totTex) {
          setProgress(
            88 + (doneTex / totTex) * 11,
            `Packaging HD textures: ${doneTex} / ${totTex} (${formatBytes(written)})...`
          );
          if (doneTex % 250 === 0 || doneTex === totTex) {
            log(`Converting/packaging HD textures: ${doneTex} / ${totTex}...`);
          }
        }
        if (doneTex % 64 === 0) {
          await new Promise((r) => setTimeout(r, 0));
        }
        yield { name: `switch/soulcalibur2/textures/${job.name}`, input };
      }
    }

    async function* generateAllEntries(includeBase, includeTex) {
      try {
        if (includeBase) {
          for (const e of entries) yield e;
        }
        if (includeTex && totTex > 0) {
          yield* generateTextureEntries();
        }
      } finally {
        if (texConverter && (!splitForBlobLimit || includeTex)) {
          texConverter.close();
        }
      }
    }

    async function streamZip(entryIter, onChunk) {
      const zipResponse = downloadZip(entryIter);
      const reader = zipResponse.body.getReader();
      let lastYieldBytes = written;
      while (true) {
        const { done, value } = await reader.read();
        if (done) break;
        await onChunk(value);
        written += value.byteLength;
        const zipPct = Math.min(99, (written / totalBytes) * 100);
        if (doneTex === 0 && written - lastUiBytes >= 4 * 1024 * 1024) {
          lastUiBytes = written;
          const baseSpan = totTex > 0 ? 13 : 24;
          const basePct = Math.min(99, (written / totalBaseBytes) * 100);
          setProgress(
            75 + (basePct / 100) * baseSpan,
            `Packaging ZIP: ${formatBytes(written)} (${Math.floor(basePct)}%)`
          );
        }
        while (zipPct >= nextLogPct && nextLogPct <= 75) {
          log(`Packaging Switch ZIP: ${nextLogPct}% (${formatBytes(written)})...`);
          nextLogPct += 25;
        }
        if (written - lastYieldBytes >= 16 * 1024 * 1024) {
          lastYieldBytes = written;
          await new Promise((r) => setTimeout(r, 0));
        }
      }
    }

    async function streamZipToBlob(entryIter) {
      const blobParts = [];
      let batch = [];
      let batchBytes = 0;
      await streamZip(entryIter, (chunk) => {
        batch.push(chunk);
        batchBytes += chunk.byteLength;
        if (batchBytes >= 4 * 1024 * 1024) {
          blobParts.push(new Blob(batch));
          batch = [];
          batchBytes = 0;
        }
      });
      if (batch.length > 0) {
        blobParts.push(new Blob(batch));
        batch = [];
      }
      return new Blob(blobParts, { type: "application/zip" });
    }

    if (saveHandle) {
      const writable = await saveHandle.createWritable();
      try {
        await streamZip(generateAllEntries(true, true), (chunk) => writable.write(chunk));
      } finally {
        await writable.close();
      }
      state.packageBuilt = true;
      setProgress(100, "Switch package saved! Extract it to the root of your microSD card.");
      log("Done! soulcalibur2_nx_package.zip saved (switch/soulcalibur2/ structure).", "ok");
    } else if (splitForBlobLimit) {
      log("Package > 1.85 GB without File System Access API: splitting into 2 ZIPs to avoid browser 2 GB Blob limit.", "warn");
      const baseBlob = await streamZipToBlob(generateAllEntries(true, false));

      if (state.zipUrl) URL.revokeObjectURL(state.zipUrl);
      state.zipUrl = URL.createObjectURL(baseBlob);
      triggerZipDownload(state.zipUrl, "soulcalibur2_nx_package.zip");
      log("Downloading soulcalibur2_nx_package.zip; packaging soulcalibur2_nx_textures.zip next...", "ok");
      await new Promise((r) => setTimeout(r, 1000));

      const texBlob = await streamZipToBlob(generateAllEntries(false, true));

      if (state.texZipUrl) URL.revokeObjectURL(state.texZipUrl);
      state.texZipUrl = URL.createObjectURL(texBlob);
      state.packageBuilt = true;
      triggerZipDownload(state.texZipUrl, "soulcalibur2_nx_textures.zip");
      setProgress(100, "Both ZIPs downloaded! Extract both to the root of your microSD card.");
      log("Done! Downloaded soulcalibur2_nx_package.zip + soulcalibur2_nx_textures.zip.", "ok");
    } else {
      const zipBlob = await streamZipToBlob(generateAllEntries(true, true));
      if (state.zipUrl) URL.revokeObjectURL(state.zipUrl);
      if (state.texZipUrl) {
        URL.revokeObjectURL(state.texZipUrl);
        state.texZipUrl = null;
      }
      state.zipUrl = URL.createObjectURL(zipBlob);
      state.packageBuilt = true;
      triggerZipDownload(state.zipUrl, "soulcalibur2_nx_package.zip");
      setProgress(100, "Switch package downloaded! Extract it to the root of your microSD card.");
      log("Done! soulcalibur2_nx_package.zip download started.", "ok");
    }
  } catch (err) {
    if (err.name !== "AbortError") {
      log(`Error building Switch package: ${err.message}`, "error");
    }
  } finally {
    state.isBuilding = false;
    updateUI();
  }
}

async function readEntryFiles(entry, out) {
  if (entry.isFile) {
    const file = await new Promise((resolve, reject) => entry.file(resolve, reject));
    out.push(file);
  } else if (entry.isDirectory) {
    const reader = entry.createReader();
    while (true) {
      const batch = await new Promise((resolve, reject) => reader.readEntries(resolve, reject));
      if (!batch.length) break;
      for (const child of batch) {
        await readEntryFiles(child, out);
      }
    }
  }
}

function getValidTextureStems(mapData) {
  const stems = new Set(mapData.validHashes);
  for (const r of mapData.rules) {
    for (const s of r.src0) stems.add(s);
    if (r.src1) for (const s of r.src1) stems.add(s);
  }
  return Array.from(stems);
}

function extractTextureArchiveInWorker(archiveFile, mapData) {
  return new Promise((resolve, reject) => {
    const worker = new Worker("./installer/archive_worker.js?v=20261118n", { type: "module" });
    const validStems = getValidTextureStems(mapData);
    let lastLogPct = 0;
    worker.onmessage = (e) => {
      const msg = e.data;
      if (msg.type === "log") {
        log(msg.message);
      } else if (msg.type === "progress") {
        state.textureScanText = `Extracting archive: ${msg.pct}%`;
        updateUI();
        if (msg.pct >= lastLogPct + 25 || msg.pct === 100) {
          lastLogPct = msg.pct;
          log(`Extracting ${archiveFile.name}: ${msg.pct}%...`);
        }
      } else if (msg.type === "collect") {
        state.textureScanText = `Indexing textures (${msg.count})...`;
        updateUI();
      } else if (msg.type === "done") {
        worker.terminate();
        resolve(msg.files);
      } else if (msg.type === "error") {
        worker.terminate();
        reject(new Error(msg.error || "Archive extraction failed"));
      }
    };
    worker.onerror = (e) => {
      worker.terminate();
      reject(new Error(e.message || "Archive worker crashed"));
    };
    worker.postMessage({ archiveFile, validStems });
  });
}

async function applyTextureFiles(fileArray) {
  state.isScanningTextures = true;
  state.textureScanText = "Loading HD textures...";
  updateUI();
  try {
    if (!state.textureMap) {
      log("Loading texture map (GRSEAF.map)...");
      state.textureMap = await loadTextureMap();
    }

    let sourceFiles = fileArray;
    const hasDirectTex = fileArray.some((f) => {
      const l = f.name.toLowerCase();
      return l.endsWith(".dds") || l.endsWith(".png");
    });
    if (!hasDirectTex) {
      const archive = fileArray.find((f) => {
        const l = f.name.toLowerCase();
        return l.endsWith(".7z") || l.endsWith(".zip") || l.endsWith(".rar");
      });
      if (archive) {
        log(`Extracting '${archive.name}' (${formatBytes(archive.size)}) in browser...`);
        sourceFiles = await extractTextureArchiveInWorker(archive, state.textureMap);
      }
    }

    const plan = planTexturePack(sourceFiles, state.textureMap);
    if (plan.jobs.length === 0) {
      log("No matching Soulcalibur II HD textures (.dds/.png) found in selection.", "warn");
      return;
    }
    state.textureJobs = plan.jobs;
    state.textureDirectCount = plan.directCount;
    state.textureConvertCount = plan.convertCount;
    state.texturesBytes = plan.jobs.reduce(
      (acc, j) => acc + (j.kind === "direct" ? j.file.size : j.f0.size),
      0
    );
    resetBuiltPackage();
    log(
      `Ready: ${plan.jobs.length} final textures (~${formatBytes(state.texturesBytes)}; ${plan.directCount} direct, ${plan.convertCount} converted on package, ${plan.skippedCount} unused skipped).`,
      "ok"
    );
  } catch (err) {
    log(`Error scanning HD textures: ${err.message}`, "error");
  } finally {
    state.isScanningTextures = false;
    state.textureScanText = "";
    updateUI();
  }
}

function wireDropzone(zoneId, inputId, handler, onDropTransfer = null) {
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
  zone.addEventListener("drop", async (e) => {
    e.preventDefault();
    zone.classList.remove("dragover");
    if (onDropTransfer) {
      await onDropTransfer(e.dataTransfer);
    } else if (e.dataTransfer.files.length) {
      handler(e.dataTransfer.files);
    }
  });
}

document.addEventListener("DOMContentLoaded", () => {
  wireDropzone("drop-xbox", "file-xbox", handleXboxInput);
  wireDropzone("drop-gc", "file-gc", handleGameCubeInput);
  wireDropzone("drop-ps2", "file-ps2", handlePs2Input);
  wireDropzone("drop-textures", "file-textures", (files) => applyTextureFiles(Array.from(files)));
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
