// Streaming disc image parsers for Soulcalibur II NX Web Installer.
// All parsers read via File.slice() to avoid loading multi-GB ISOs into RAM.

export const DiscParsers = (() => {
  const SECTOR_SIZE = 2048;

  async function readSlice(file, offset, length) {
    const end = Math.min(file.size, offset + length);
    if (offset >= file.size || end <= offset) return new ArrayBuffer(0);
    return await file.slice(offset, end).arrayBuffer();
  }

  function decodeAscii(bytes) {
    let s = "";
    for (let i = 0; i < bytes.length; i++) {
      if (bytes[i] === 0) break;
      s += String.fromCharCode(bytes[i]);
    }
    return s;
  }

  // ---- 1. Xbox XISO Parser ----
  async function parseXboxIso(file) {
    const MAGIC = "MICROSOFT*XBOX*MEDIA";
    const candidateBases = [0x0, 0x18300000, 0x0fd90000];
    let baseOffset = -1;
    let rootSector = 0;
    let rootSize = 0;

    for (const base of candidateBases) {
      const hdrOff = base + 32 * SECTOR_SIZE;
      if (hdrOff + 32 > file.size) continue;
      const buf = await readSlice(file, hdrOff, 32);
      if (buf.byteLength < 32) continue;
      const u8 = new Uint8Array(buf);
      const magic = decodeAscii(u8.subarray(0, 20));
      if (magic === MAGIC) {
        const dv = new DataView(buf);
        baseOffset = base;
        rootSector = dv.getUint32(20, true);
        rootSize = dv.getUint32(24, true);
        break;
      }
    }

    if (baseOffset < 0) {
      throw new Error(
        "No se encontró cabecera XISO válida ('MICROSOFT*XBOX*MEDIA'). Asegúrate de usar una ISO de Xbox (NTSC-U)."
      );
    }

    const files = {};
    async function walkXisoDir(dirSector, dirSize, prefix) {
      if (dirSector === 0 || dirSize === 0 || dirSize > 16 * 1024 * 1024) return;
      const dirBuf = await readSlice(file, baseOffset + dirSector * SECTOR_SIZE, dirSize);
      const dv = new DataView(dirBuf);
      const u8 = new Uint8Array(dirBuf);
      const visited = new Set();

      async function visitNode(wordOffset) {
        const byteOff = wordOffset * 4;
        if (wordOffset === 0xffff || byteOff + 14 > dirBuf.byteLength || visited.has(byteOff)) return;
        visited.add(byteOff);

        const left = dv.getUint16(byteOff + 0, true);
        const right = dv.getUint16(byteOff + 2, true);
        const sector = dv.getUint32(byteOff + 4, true);
        const size = dv.getUint32(byteOff + 8, true);
        const attrs = u8[byteOff + 12];
        const nameLen = u8[byteOff + 13];
        if (byteOff + 14 + nameLen > dirBuf.byteLength) return;
        const name = decodeAscii(u8.subarray(byteOff + 14, byteOff + 14 + nameLen));

        if (left !== 0xffff) await visitNode(left);

        const fullPath = prefix ? `${prefix}/${name}` : name;
        if (attrs & 0x10) {
          await walkXisoDir(sector, size, fullPath);
        } else if (name) {
          const offset = baseOffset + sector * SECTOR_SIZE;
          files[fullPath.toLowerCase()] = { name, path: fullPath, offset, size };
        }

        if (right !== 0xffff) await visitNode(right);
      }

      await visitNode(0);
    }

    await walkXisoDir(rootSector, rootSize, "");
    return files;
  }

  // ---- 2. GameCube GCM/ISO Parser ----
  async function parseGameCubeIso(file) {
    const hdrBuf = await readSlice(file, 0, 0x440);
    if (hdrBuf.byteLength < 0x440) {
      throw new Error("El archivo es demasiado pequeño para ser una ISO de GameCube.");
    }
    const dv = new DataView(hdrBuf);
    const magic = dv.getUint32(0x1c, false);
    if (magic !== 0xc2339f3d) {
      throw new Error("Cabecera de GameCube inválida (magic != 0xC2339F3D).");
    }

    const dolOffset = dv.getUint32(0x420, false);
    const fstOffset = dv.getUint32(0x424, false);
    const fstSize = dv.getUint32(0x428, false);

    // Compute boot.dol exact size from DOL section headers
    const dolHdrBuf = await readSlice(file, dolOffset, 0x100);
    const dolDv = new DataView(dolHdrBuf);
    let dolSize = 0x100;
    for (let i = 0; i < 18; i++) {
      const secOff = dolDv.getUint32(i * 4, false);
      const secSize = dolDv.getUint32(0x90 + i * 4, false);
      if (secSize > 0 && secOff + secSize > dolSize) {
        dolSize = secOff + secSize;
      }
    }

    const fstBuf = await readSlice(file, fstOffset, fstSize);
    const fstDv = new DataView(fstBuf);
    const fstU8 = new Uint8Array(fstBuf);
    const numEntries = fstDv.getUint32(8, false);
    const strTableOff = numEntries * 12;

    const files = {
      "boot.dol": { name: "boot.dol", path: "boot.dol", offset: dolOffset, size: dolSize },
    };

    for (let i = 1; i < numEntries; i++) {
      const off = i * 12;
      const flagsAndName = fstDv.getUint32(off, false);
      const isDir = (flagsAndName >>> 24) !== 0;
      const nameOff = flagsAndName & 0x00ffffff;
      if (isDir) continue;
      const entryOffset = fstDv.getUint32(off + 4, false);
      const entryLength = fstDv.getUint32(off + 8, false);
      const name = decodeAscii(fstU8.subarray(strTableOff + nameOff));
      if (name) {
        files[name.toLowerCase()] = { name, path: name, offset: entryOffset, size: entryLength };
      }
    }
    return files;
  }

  // ---- 3. PS2 ISO9660 Parser ----
  async function parsePs2Iso(file) {
    const pvdBuf = await readSlice(file, 16 * SECTOR_SIZE, SECTOR_SIZE);
    if (pvdBuf.byteLength < SECTOR_SIZE) {
      throw new Error("El archivo es demasiado pequeño para ser una ISO de PS2.");
    }
    const pvdU8 = new Uint8Array(pvdBuf);
    const pvdDv = new DataView(pvdBuf);
    if (pvdU8[0] !== 1 || decodeAscii(pvdU8.subarray(1, 6)) !== "CD001") {
      throw new Error("Cabecera ISO9660 inválida (no se encontró PVD 'CD001').");
    }

    const rootExtent = pvdDv.getUint32(156 + 2, true);
    const rootSize = pvdDv.getUint32(156 + 10, true);
    const files = {};

    async function walkIsoDir(extent, size, prefix) {
      const buf = await readSlice(file, extent * SECTOR_SIZE, size);
      const dv = new DataView(buf);
      const u8 = new Uint8Array(buf);
      let pos = 0;
      while (pos < buf.byteLength) {
        const recLen = u8[pos];
        if (recLen === 0) {
          pos = Math.floor(pos / SECTOR_SIZE + 1) * SECTOR_SIZE;
          continue;
        }
        const recExtent = dv.getUint32(pos + 2, true);
        const recSize = dv.getUint32(pos + 10, true);
        const flags = u8[pos + 25];
        const idLen = u8[pos + 32];
        let name = decodeAscii(u8.subarray(pos + 33, pos + 33 + idLen));
        const semi = name.indexOf(";");
        if (semi >= 0) name = name.slice(0, semi);

        if (name && name !== "\x00" && name !== "\x01") {
          const fullPath = prefix ? `${prefix}/${name}` : name;
          if (flags & 0x02) {
            await walkIsoDir(recExtent, recSize, fullPath);
          } else {
            files[fullPath.toLowerCase()] = {
              name,
              path: fullPath,
              offset: recExtent * SECTOR_SIZE,
              size: recSize,
            };
          }
        }
        pos += recLen;
      }
    }

    await walkIsoDir(rootExtent, rootSize, "");
    return files;
  }

  return {
    parseXboxIso,
    parseGameCubeIso,
    parsePs2Iso,
  };
})();
