import { DiscParsers } from "./xiso.js";

const REPO_OWNER = "markaurel13";
const REPO_NAME = "soulcalibur2-nx";
const REPO_BRANCH = "switch-port";
const RELEASE_API = `https://api.github.com/repos/${REPO_OWNER}/${REPO_NAME}/releases/latest`;

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
  release: null, // { tag, nroName, nroUrl, nroSize }
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
    ? `<span class="pill ok">✓ Link listo (${formatBytes(state.gc["root.olk"].blob.size)})</span>`
    : `<span class="pill muted">Opcional: ISO de GameCube para Link</span>`;

  $("ps2-status").innerHTML = readyPs2
    ? `<span class="pill ok">✓ Heihachi listo (${formatBytes(state.ps2["root.olk"].blob.size)})</span>`
    : `<span class="pill muted">Opcional: ISO de PS2 para Heihachi</span>`;

  const canBuild = readyXbox;
  $("btn-export-sd").disabled = !canBuild;
  $("btn-download-list").disabled = !canBuild;

  if (!window.showDirectoryPicker) {
    $("btn-export-sd").title = "Tu navegador no soporta escritura directa a carpeta; usa Chrome/Edge/Brave o la descarga individual.";
  }
}

// ---- 1. Fetch latest GitHub Release (.nro) ----
async function loadLatestRelease() {
  const badge = $("release-info");
  const dlBtn = $("btn-download-nro");
  try {
    const res = await fetch(RELEASE_API, { headers: { Accept: "application/vnd.github+json" } });
    if (!res.ok) throw new Error(`HTTP ${res.status}`);
    const data = await res.json();
    const assets = Array.isArray(data.assets) ? data.assets : [];
    const nro = assets.find((a) => a.name.toLowerCase().endsWith(".nro"));

    if (!nro) {
      badge.textContent = `Release ${data.tag_name || ""} encontrado, pero sin archivo .nro adjunto aún.`;
      dlBtn.hidden = true;
      return;
    }

    state.release = {
      tag: data.tag_name,
      nroName: nro.name,
      nroUrl: nro.browser_download_url,
      nroSize: nro.size,
    };

    badge.innerHTML = `Última versión: <strong>${data.tag_name}</strong> · <code>${nro.name}</code> (${formatBytes(nro.size)})`;
    dlBtn.href = nro.browser_download_url;
    dlBtn.download = nro.name;
    dlBtn.hidden = false;
    log(`Release detectado: ${data.tag_name} (${nro.name}, ${formatBytes(nro.size)})`, "ok");
  } catch (err) {
    badge.textContent = "Sin releases publicados aún (o límite de API de GitHub). Puedes cargar tu soulcalibur2.nro manualmente.";
    dlBtn.hidden = true;
    log(`No se pudo consultar el último Release: ${err.message}`, "warn");
  }
}

// ---- 2. Handle Xbox Disc / Files ----
async function handleXboxInput(fileList) {
  for (const file of fileList) {
    const low = file.name.toLowerCase();
    if (low.endsWith(".iso") || low.endsWith(".xiso")) {
      log(`Leyendo estructura XISO de '${file.name}' (${formatBytes(file.size)})...`);
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
          if (entry) {
            state.xbox[key] = {
              name: outName,
              blob: file.slice(entry.offset, entry.offset + entry.size),
            };
            foundCount++;
          }
        }
        if (foundCount === 0) {
          log("No se encontraron Default.xbe / root.olk / movie.afs / bgm.afs dentro de la ISO.", "error");
        } else {
          log(`ISO de Xbox analizada: ${foundCount}/4 archivos listos (sin copiar a RAM).`, "ok");
        }
      } catch (err) {
        log(`Error al leer ISO de Xbox: ${err.message}`, "error");
      }
    } else if (low === "default.xbe" || low === "root.olk" || low === "movie.afs" || low === "bgm.afs") {
      const canonical = {
        "default.xbe": "Default.xbe",
        "root.olk": "root.olk",
        "movie.afs": "movie.afs",
        "bgm.afs": "bgm.afs",
      }[low];
      state.xbox[low] = { name: canonical, blob: file };
      log(`Cargado archivo individual: ${canonical} (${formatBytes(file.size)})`, "ok");
    } else if (low.endsWith(".nro")) {
      state.nroBlob = { name: file.name, blob: file };
      log(`NRO local cargado: ${file.name} (${formatBytes(file.size)})`, "ok");
    }
  }
  updateUI();
}

// ---- 3. Handle GameCube / PS2 Discs ----
async function handleGameCubeInput(fileList) {
  for (const file of fileList) {
    const low = file.name.toLowerCase();
    if (low.endsWith(".iso") || low.endsWith(".gcm")) {
      log(`Leyendo ISO de GameCube '${file.name}'...`);
      try {
        const entries = await DiscParsers.parseGameCubeIso(file);
        if (!entries["root.olk"] || !entries["boot.dol"]) {
          throw new Error("Faltan root.olk o boot.dol en la ISO de GameCube.");
        }
        state.gc["root.olk"] = {
          name: "root.olk",
          blob: file.slice(entries["root.olk"].offset, entries["root.olk"].offset + entries["root.olk"].size),
        };
        state.gc["boot.dol"] = {
          name: "boot.dol",
          blob: file.slice(entries["boot.dol"].offset, entries["boot.dol"].offset + entries["boot.dol"].size),
        };
        log("ISO de GameCube lista para extraer a Link.", "ok");
      } catch (err) {
        log(`Error al leer ISO de GameCube: ${err.message}`, "error");
      }
    } else if (low === "root.olk" || low === "boot.dol") {
      state.gc[low] = { name: low, blob: file };
      log(`GameCube: cargado ${low} (${formatBytes(file.size)})`, "ok");
    }
  }
  updateUI();
}

async function handlePs2Input(fileList) {
  for (const file of fileList) {
    const low = file.name.toLowerCase();
    if (low.endsWith(".iso")) {
      log(`Leyendo ISO de PS2 '${file.name}'...`);
      try {
        const entries = await DiscParsers.parsePs2Iso(file);
        if (!entries["root.olk"] || !entries["slus_206.43"]) {
          throw new Error("Faltan ROOT.OLK o SLUS_206.43 en la ISO de PS2 (asegúrate de usar Soulcalibur II USA).");
        }
        state.ps2["root.olk"] = {
          name: "root.olk",
          blob: file.slice(entries["root.olk"].offset, entries["root.olk"].offset + entries["root.olk"].size),
        };
        state.ps2["slus_206.43"] = {
          name: "SLUS_206.43",
          blob: file.slice(entries["slus_206.43"].offset, entries["slus_206.43"].offset + entries["slus_206.43"].size),
        };
        log("ISO de PS2 lista para extraer a Heihachi.", "ok");
      } catch (err) {
        log(`Error al leer ISO de PS2: ${err.message}`, "error");
      }
    } else if (low === "root.olk" || low === "slus_206.43") {
      state.ps2[low] = { name: file.name, blob: file };
      log(`PS2: cargado ${file.name} (${formatBytes(file.size)})`, "ok");
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
  throw new Error(`No se pudo descargar el script tools/guests/${filename}`);
}

async function buildGuestsIfNeeded() {
  const hasGc = state.gc["root.olk"] && state.gc["boot.dol"];
  const hasPs2 = state.ps2["root.olk"] && state.ps2["slus_206.43"];
  if (!hasGc && !hasPs2) return null;
  if (state.guestsBlob) return state.guestsBlob;

  setProgress(10, "Cargando motor Python (Pyodide) para Link / Heihachi...");
  log("Inicializando Pyodide en el navegador para generar guests.olk...");

  if (!state.pyodide) {
    if (!window.loadPyodide) {
      await new Promise((resolve, reject) => {
        const s = document.createElement("script");
        s.src = "https://cdn.jsdelivr.net/pyodide/v0.26.4/full/pyodide.js";
        s.onload = resolve;
        s.onerror = () => reject(new Error("No se pudo cargar Pyodide desde CDN."));
        document.head.appendChild(s);
      });
    }
    state.pyodide = await window.loadPyodide();
  }

  const py = state.pyodide;
  py.FS.mkdirTree("/work/scripts");
  py.FS.mkdirTree("/work/xb");
  py.FS.mkdirTree("/work/gc");
  py.FS.mkdirTree("/work/ps2");

  setProgress(25, "Cargando scripts de conversión (tools/guests)...");
  for (const f of GUEST_PY_FILES) {
    const code = await fetchGuestScript(f);
    py.FS.writeFile(`/work/scripts/${f}`, code);
  }

  setProgress(40, "Copiando archivos de discos al entorno temporal...");
  py.FS.writeFile("/work/xb/root.olk", new Uint8Array(await state.xbox["root.olk"].blob.arrayBuffer()));

  const args = ["/work/xb/root.olk"];
  if (hasGc) {
    py.FS.writeFile("/work/gc/root.olk", new Uint8Array(await state.gc["root.olk"].blob.arrayBuffer()));
    py.FS.writeFile("/work/gc/boot.dol", new Uint8Array(await state.gc["boot.dol"].blob.arrayBuffer()));
    args.push("--gamecube", "/work/gc/root.olk");
  }
  if (hasPs2) {
    py.FS.writeFile("/work/ps2/root.olk", new Uint8Array(await state.ps2["root.olk"].blob.arrayBuffer()));
    py.FS.writeFile("/work/ps2/SLUS_206.43", new Uint8Array(await state.ps2["slus_206.43"].blob.arrayBuffer()));
    args.push("--ps2", "/work/ps2/root.olk");
  }
  args.push("/work/guests.olk");

  setProgress(60, "Construyendo guests.olk (Link / Heihachi)...");
  log(`Ejecutando roster.py (${hasGc ? "Link " : ""}${hasPs2 ? "Heihachi" : ""})...`);

  await py.runPythonAsync(`
import sys
sys.path.insert(0, "/work/scripts")
import roster
sys.argv = ["roster.py", *${JSON.stringify(args)}]
roster.main()
`);

  const outBytes = py.FS.readFile("/work/guests.olk");
  state.guestsBlob = new Blob([outBytes], { type: "application/octet-stream" });
  log(`guests.olk generado exitosamente (${formatBytes(state.guestsBlob.size)}).`, "ok");

  // Clean up Pyodide VFS to release RAM
  for (const p of [
    "/work/xb/root.olk",
    "/work/gc/root.olk",
    "/work/gc/boot.dol",
    "/work/ps2/root.olk",
    "/work/ps2/SLUS_206.43",
    "/work/guests.olk",
  ]) {
    try {
      py.FS.unlink(p);
    } catch (_) {}
  }

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

async function resolveNroBlob() {
  if (state.nroBlob) return state.nroBlob;
  if (!state.release || !state.release.nroUrl) return null;
  try {
    log(`Descargando ${state.release.nroName} desde GitHub Releases...`);
    const res = await fetch(state.release.nroUrl);
    if (!res.ok) throw new Error(`HTTP ${res.status}`);
    const blob = await res.blob();
    return { name: state.release.nroName, blob };
  } catch (err) {
    log(
      `GitHub bloqueó la descarga directa por CORS (${err.message}). Usa el botón "Descargar soulcalibur2.nro" de arriba y cópialo junto a estos archivos.`,
      "warn"
    );
    return null;
  }
}

async function exportToDirectory() {
  if (!window.showDirectoryPicker) {
    alert("Tu navegador no soporta escritura directa a carpetas. Usa Chrome/Edge/Brave o el botón 'Descargar archivos'.");
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
    const nro = await resolveNroBlob();

    const queue = [
      state.xbox["default.xbe"],
      state.xbox["root.olk"],
      state.xbox["movie.afs"],
      state.xbox["bgm.afs"],
    ];
    if (guests) queue.push({ name: "guests.olk", blob: guests });
    if (nro) queue.push(nro);

    const totalBytes = queue.reduce((acc, f) => acc + f.blob.size, 0);
    let writtenBytes = 0;

    for (const item of queue) {
      log(`Escribiendo ${item.name} (${formatBytes(item.blob.size)}) en switch/soulcalibur2/...`);
      await writeBlobStreaming(targetDir, item.name, item.blob, (chunkEnd) => {
        const pct = ((writtenBytes + chunkEnd) / totalBytes) * 100;
        setProgress(pct, `Copiando ${item.name}... (${pct.toFixed(0)}%)`);
      });
      writtenBytes += item.blob.size;
    }

    setProgress(100, "¡Instalación completada en switch/soulcalibur2/!");
    log("¡Listo! Ya puedes insertar la microSD en tu Nintendo Switch y abrir Soulcalibur II NX.", "ok");
  } catch (err) {
    if (err.name !== "AbortError") {
      log(`Error exportando archivos: ${err.message}`, "error");
    }
  }
}

async function showManualDownloads() {
  try {
    const guests = await buildGuestsIfNeeded();
    const container = $("manual-downloads");
    container.hidden = false;
    container.innerHTML = "";

    const items = [
      state.xbox["default.xbe"],
      state.xbox["root.olk"],
      state.xbox["movie.afs"],
      state.xbox["bgm.afs"],
    ];
    if (guests) items.push({ name: "guests.olk", blob: guests });

    for (const item of items) {
      const a = document.createElement("a");
      a.className = "dl-chip";
      a.href = URL.createObjectURL(item.blob);
      a.download = item.name;
      a.textContent = `⬇ ${item.name} (${formatBytes(item.blob.size)})`;
      container.appendChild(a);
    }

    if (state.release && state.release.nroUrl) {
      const a = document.createElement("a");
      a.className = "dl-chip dl-nro";
      a.href = state.release.nroUrl;
      a.download = state.release.nroName;
      a.textContent = `⬇ ${state.release.nroName} (${state.release.tag})`;
      container.appendChild(a);
    }

    setProgress(100, "Archivos listos para descargar manualmente a sdmc:/switch/soulcalibur2/");
    log("Enlaces de descarga generados.", "ok");
  } catch (err) {
    log(`Error preparando descargas: ${err.message}`, "error");
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
  loadLatestRelease();
  wireDropzone("drop-xbox", "file-xbox", handleXboxInput);
  wireDropzone("drop-gc", "file-gc", handleGameCubeInput);
  wireDropzone("drop-ps2", "file-ps2", handlePs2Input);
  $("btn-export-sd").addEventListener("click", exportToDirectory);
  $("btn-download-list").addEventListener("click", showManualDownloads);
  updateUI();
});
