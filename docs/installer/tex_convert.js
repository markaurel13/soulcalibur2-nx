// Browser-side Dolphin (GRSEAF) -> Xbox/Switch HD .dds texture mapper & converter.
// Supports both pre-converted <xxh64>.dds packs and raw Dolphin tex1_*.dds / .png packs.

export async function loadTextureMap() {
  const r = await fetch("./installer/GRSEAF.map");
  if (!r.ok) throw new Error(`Failed to load GRSEAF.map (HTTP ${r.status})`);
  const text = await r.text();
  const validHashes = new Set();
  const rules = [];
  for (const raw of text.split(/\r?\n/)) {
    const line = raw.trim();
    if (!line || line.startsWith("#")) continue;
    const p = line.split(/\s+/);
    if (p.length < 5) continue;
    const hash = p[0].toLowerCase();
    const kind = p[1];
    const [iw, ih] = p[2].split("x").map(Number);
    const [sw, sh] = p[3].split("x").map(Number);
    const src0 = p[4].toLowerCase().split("|");
    const src1 = p.length >= 6 && (kind === "alpha" || kind === "palette" || kind === "font")
      ? p[5].toLowerCase().split("|")
      : null;
    validHashes.add(hash);
    rules.push({ hash, kind, iw, ih, sw, sh, src0, src1 });
  }
  return { validHashes, rules };
}

function findMatchingFile(fileMap, alts) {
  if (!alts) return null;
  for (const alt of alts) {
    const f = fileMap.get(alt);
    if (f) return f;
  }
  return null;
}

export function planTexturePack(fileArray, mapData) {
  const { validHashes, rules } = mapData;
  const directByHash = new Map();
  const packByStem = new Map();
  let totalDdsOrPng = 0;

  for (const file of fileArray) {
    const base = file.name.split("/").pop().split("\\").pop();
    const low = base.toLowerCase();
    if (!low.endsWith(".dds") && !low.endsWith(".png")) continue;
    totalDdsOrPng++;
    const stem = low.slice(0, -4);
    if (low.endsWith(".dds") && validHashes.has(stem)) {
      directByHash.set(stem, { hash: stem, name: `${stem}.dds`, kind: "direct", file });
    } else {
      packByStem.set(stem, file);
    }
  }

  // Prefer already-converted <xxh64>.dds if present; otherwise resolve Dolphin rules.
  const jobs = new Map(directByHash);
  let directCopyCount = directByHash.size;
  let convertCount = 0;

  for (const rule of rules) {
    if (jobs.has(rule.hash)) continue;
    const f0 = findMatchingFile(packByStem, rule.src0);
    if (rule.kind === "copy") {
      if (!f0) continue;
      const isDds = f0.name.toLowerCase().endsWith(".dds");
      if (isDds && rule.iw === rule.sw && rule.ih === rule.sh) {
        jobs.set(rule.hash, { hash: rule.hash, name: `${rule.hash}.dds`, kind: "direct", file: f0 });
        directCopyCount++;
      } else {
        jobs.set(rule.hash, { hash: rule.hash, name: `${rule.hash}.dds`, kind: "copy_pad", rule, f0 });
        convertCount++;
      }
    } else if (rule.kind === "alpha" || rule.kind === "palette") {
      const f1 = findMatchingFile(packByStem, rule.src1);
      if (!f0 || !f1) continue;
      jobs.set(rule.hash, { hash: rule.hash, name: `${rule.hash}.dds`, kind: rule.kind, rule, f0, f1 });
      convertCount++;
    } else if (rule.kind === "font") {
      const f = f0 || findMatchingFile(packByStem, rule.src1);
      if (!f) continue;
      jobs.set(rule.hash, { hash: rule.hash, name: `${rule.hash}.dds`, kind: "copy_pad", rule, f0: f });
      convertCount++;
    }
  }

  return {
    jobs: Array.from(jobs.values()),
    directCount: directCopyCount,
    convertCount,
    skippedCount: Math.max(0, totalDdsOrPng - jobs.size),
  };
}

// ---- DDS / PNG Decode & DXT5 Encode ----

function decode565(c) {
  const r = (c >> 11) & 31;
  const g = (c >> 5) & 63;
  const b = c & 31;
  return [
    (r << 3) | (r >> 2),
    (g << 2) | (g >> 4),
    (b << 3) | (b >> 2),
  ];
}

function decodeDds(buf) {
  const dv = new DataView(buf);
  if (dv.byteLength < 128 || dv.getUint32(0, true) !== 0x20534444) {
    throw new Error("Invalid DDS header");
  }
  const h = dv.getUint32(12, true);
  const w = dv.getUint32(16, true);
  const pfFlags = dv.getUint32(80, true);
  const fourCC = dv.getUint32(84, true);
  const rgba = new Uint8Array(w * h * 4);
  const src = new Uint8Array(buf, 128);

  if (pfFlags & 0x4) {
    // Compressed
    const isDxt1 = fourCC === 0x31545844; // 'DXT1'
    const isDxt3 = fourCC === 0x33545844; // 'DXT3'
    const isDxt5 = fourCC === 0x35545844; // 'DXT5'
    if (!isDxt1 && !isDxt3 && !isDxt5) {
      throw new Error(`Unsupported DDS FourCC: 0x${fourCC.toString(16)}`);
    }
    const bw = (w + 3) >> 2;
    const bh = (h + 3) >> 2;
    const blockBytes = isDxt1 ? 8 : 16;
    let off = 0;
    const c = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0]];
    const aTable = new Uint8Array(8);

    for (let by = 0; by < bh; by++) {
      for (let bx = 0; bx < bw; bx++) {
        let aOff = off;
        let cOff = off + (isDxt1 ? 0 : 8);

        if (isDxt5) {
          const a0 = src[aOff];
          const a1 = src[aOff + 1];
          aTable[0] = a0;
          aTable[1] = a1;
          if (a0 > a1) {
            for (let k = 1; k <= 6; k++) aTable[k + 1] = ((7 - k) * a0 + k * a1 + 3) / 7 | 0;
          } else {
            for (let k = 1; k <= 4; k++) aTable[k + 1] = ((5 - k) * a0 + k * a1 + 2) / 5 | 0;
            aTable[6] = 0;
            aTable[7] = 255;
          }
        }

        const c0 = src[cOff] | (src[cOff + 1] << 8);
        const c1 = src[cOff + 2] | (src[cOff + 3] << 8);
        c[0] = decode565(c0);
        c[1] = decode565(c1);
        if (c0 > c1 || !isDxt1) {
          c[2] = [(2 * c[0][0] + c[1][0] + 1) / 3 | 0, (2 * c[0][1] + c[1][1] + 1) / 3 | 0, (2 * c[0][2] + c[1][2] + 1) / 3 | 0];
          c[3] = [(c[0][0] + 2 * c[1][0] + 1) / 3 | 0, (c[0][1] + 2 * c[1][1] + 1) / 3 | 0, (c[0][2] + 2 * c[1][2] + 1) / 3 | 0];
        } else {
          c[2] = [(c[0][0] + c[1][0]) >> 1, (c[0][1] + c[1][1]) >> 1, (c[0][2] + c[1][2]) >> 1];
          c[3] = [0, 0, 0];
        }

        const cBits = src[cOff + 4] | (src[cOff + 5] << 8) | (src[cOff + 6] << 16) | (src[cOff + 7] << 24);
        let aLo = 0, aHi = 0;
        if (isDxt5) {
          aLo = src[aOff + 2] | (src[aOff + 3] << 8) | (src[aOff + 4] << 16);
          aHi = src[aOff + 5] | (src[aOff + 6] << 8) | (src[aOff + 7] << 16);
        }

        for (let py = 0; py < 4; py++) {
          const y = by * 4 + py;
          if (y >= h) continue;
          for (let px = 0; px < 4; px++) {
            const x = bx * 4 + px;
            if (x >= w) continue;
            const pi = py * 4 + px;
            const ci = (cBits >>> (pi * 2)) & 3;
            let alpha = 255;
            if (isDxt1) {
              if (c0 <= c1 && ci === 3) alpha = 0;
            } else if (isDxt3) {
              const bVal = src[aOff + (pi >> 1)];
              const nib = (pi & 1) ? (bVal >> 4) : (bVal & 15);
              alpha = (nib << 4) | nib;
            } else if (isDxt5) {
              const bitPos = pi * 3;
              const ai = bitPos < 24
                ? (bitPos <= 21 ? ((aLo >>> bitPos) & 7) : (((aLo >>> bitPos) | (aHi << (24 - bitPos))) & 7))
                : ((aHi >>> (bitPos - 24)) & 7);
              alpha = aTable[ai];
            }
            const dst = (y * w + x) * 4;
            rgba[dst] = c[ci][0];
            rgba[dst + 1] = c[ci][1];
            rgba[dst + 2] = c[ci][2];
            rgba[dst + 3] = alpha;
          }
        }
        off += blockBytes;
      }
    }
  } else {
    // Uncompressed BGRA8/RGBA8
    const bpp = dv.getUint32(88, true);
    const rMask = dv.getUint32(92, true);
    const isBgra = bpp === 32 && rMask === 0x00ff0000;
    const n = w * h;
    for (let i = 0; i < n; i++) {
      const s = i * 4;
      if (isBgra) {
        rgba[s] = src[s + 2];
        rgba[s + 1] = src[s + 1];
        rgba[s + 2] = src[s];
        rgba[s + 3] = src[s + 3];
      } else {
        rgba[s] = src[s];
        rgba[s + 1] = src[s + 1];
        rgba[s + 2] = src[s + 2];
        rgba[s + 3] = src[s + 3];
      }
    }
  }
  return { w, h, rgba };
}

async function decodeImageFile(file) {
  if (file.name.toLowerCase().endsWith(".dds")) {
    const buf = await file.arrayBuffer();
    return decodeDds(buf);
  }
  const bmp = await createImageBitmap(file);
  const w = bmp.width;
  const h = bmp.height;
  const canvas = new OffscreenCanvas(w, h);
  const ctx = canvas.getContext("2d");
  ctx.drawImage(bmp, 0, 0);
  bmp.close();
  const imgData = ctx.getImageData(0, 0, w, h);
  return { w, h, rgba: imgData.data };
}

function rgbTo565(r, g, b) {
  return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

function encodeDxt5(w, h, rgba) {
  const bw = Math.max(1, (w + 3) >> 2);
  const bh = Math.max(1, (h + 3) >> 2);
  const out = new Uint8Array(128 + bw * bh * 16);
  const dv = new DataView(out.buffer);
  // DDS Header
  dv.setUint32(0, 0x20534444, true); // "DDS "
  dv.setUint32(4, 124, true);
  dv.setUint32(8, 0x1 | 0x2 | 0x4 | 0x1000 | 0x80000, true);
  dv.setUint32(12, h, true);
  dv.setUint32(16, w, true);
  dv.setUint32(20, bw * bh * 16, true);
  dv.setUint32(28, 1, true); // mipMapCount = 1
  dv.setUint32(76, 32, true);
  dv.setUint32(80, 0x4, true); // DDPF_FOURCC
  dv.setUint32(84, 0x35545844, true); // "DXT5"
  dv.setUint32(108, 0x1000, true); // DDSCAPS_TEXTURE

  let off = 128;
  const blk = new Uint8Array(64);

  for (let by = 0; by < bh; by++) {
    for (let bx = 0; bx < bw; bx++) {
      let minA = 255, maxA = 0;
      let minR = 255, minG = 255, minB = 255;
      let maxR = 0, maxG = 0, maxB = 0;
      let minL = 1e9, maxL = -1;
      let loR = 0, loG = 0, loB = 0, hiR = 0, hiG = 0, hiB = 0;

      for (let py = 0; py < 4; py++) {
        const y = Math.min(h - 1, by * 4 + py);
        for (let px = 0; px < 4; px++) {
          const x = Math.min(w - 1, bx * 4 + px);
          const si = (y * w + x) * 4;
          const di = (py * 4 + px) * 4;
          const r = rgba[si], g = rgba[si + 1], b = rgba[si + 2], a = rgba[si + 3];
          blk[di] = r; blk[di + 1] = g; blk[di + 2] = b; blk[di + 3] = a;
          if (a < minA) minA = a;
          if (a > maxA) maxA = a;
          if (r < minR) minR = r; if (r > maxR) maxR = r;
          if (g < minG) minG = g; if (g > maxG) maxG = g;
          if (b < minB) minB = b; if (b > maxB) maxB = b;
          const lum = r * 77 + g * 150 + b * 29;
          if (lum < minL) { minL = lum; loR = r; loG = g; loB = b; }
          if (lum > maxL) { maxL = lum; hiR = r; hiG = g; hiB = b; }
        }
      }

      // Alpha block (8 bytes)
      out[off] = maxA;
      out[off + 1] = minA;
      let aLo = 0, aHi = 0;
      if (maxA > minA) {
        const span = maxA - minA;
        for (let pi = 0; pi < 16; pi++) {
          const a = blk[pi * 4 + 3];
          // Map a in [minA, maxA] to 6-step DXT5 alpha index
          let idx;
          const t = ((maxA - a) * 7 + (span >> 1)) / span | 0;
          if (t <= 0) idx = 0;
          else if (t >= 7) idx = 1;
          else idx = t + 1;
          const bitPos = pi * 3;
          if (bitPos < 24) {
            aLo |= idx << bitPos;
            if (bitPos > 21) aHi |= idx >>> (24 - bitPos);
          } else {
            aHi |= idx << (bitPos - 24);
          }
        }
      }
      out[off + 2] = aLo & 255;
      out[off + 3] = (aLo >>> 8) & 255;
      out[off + 4] = (aLo >>> 16) & 255;
      out[off + 5] = aHi & 255;
      out[off + 6] = (aHi >>> 8) & 255;
      out[off + 7] = (aHi >>> 16) & 255;

      // Color block (8 bytes) using luminance endpoints
      let c0 = rgbTo565(hiR, hiG, hiB);
      let c1 = rgbTo565(loR, loG, loB);
      if (c0 < c1) { const tmp = c0; c0 = c1; c1 = tmp; }
      out[off + 8] = c0 & 255;
      out[off + 9] = (c0 >>> 8) & 255;
      out[off + 10] = c1 & 255;
      out[off + 11] = (c1 >>> 8) & 255;

      let cBits = 0;
      if (c0 !== c1) {
        const r0 = (c0 >> 11) & 31, g0 = (c0 >> 5) & 63, b0 = c0 & 31;
        const p0r = (r0 << 3) | (r0 >> 2), p0g = (g0 << 2) | (g0 >> 4), p0b = (b0 << 3) | (b0 >> 2);
        const r1 = (c1 >> 11) & 31, g1 = (c1 >> 5) & 63, b1 = c1 & 31;
        const p1r = (r1 << 3) | (r1 >> 2), p1g = (g1 << 2) | (g1 >> 4), p1b = (b1 << 3) | (b1 >> 2);
        const p2r = ((2 * p0r + p1r + 1) / 3) | 0, p2g = ((2 * p0g + p1g + 1) / 3) | 0, p2b = ((2 * p0b + p1b + 1) / 3) | 0;
        const p3r = ((p0r + 2 * p1r + 1) / 3) | 0, p3g = ((p0g + 2 * p1g + 1) / 3) | 0, p3b = ((p0b + 2 * p1b + 1) / 3) | 0;
        for (let pi = 0; pi < 16; pi++) {
          const di = pi * 4;
          const r = blk[di], g = blk[di + 1], b = blk[di + 2];
          let dr = r - p0r, dg = g - p0g, db = b - p0b;
          let bestErr = dr * dr + dg * dg + db * db, best = 0;
          dr = r - p1r; dg = g - p1g; db = b - p1b;
          let err = dr * dr + dg * dg + db * db;
          if (err < bestErr) { bestErr = err; best = 1; }
          dr = r - p2r; dg = g - p2g; db = b - p2b;
          err = dr * dr + dg * dg + db * db;
          if (err < bestErr) { bestErr = err; best = 2; }
          dr = r - p3r; dg = g - p3g; db = b - p3b;
          err = dr * dr + dg * dg + db * db;
          if (err < bestErr) { best = 3; }
          cBits |= best << (pi * 2);
        }
      }
      out[off + 12] = cBits & 255;
      out[off + 13] = (cBits >>> 8) & 255;
      out[off + 14] = (cBits >>> 16) & 255;
      out[off + 15] = (cBits >>> 24) & 255;

      off += 16;
    }
  }
  return out;
}

export async function buildTextureDdsBytes(job) {
  if (job.kind === "direct") return new Uint8Array(await job.file.arrayBuffer());

  const { rule } = job;
  const img0 = await decodeImageFile(job.f0);
  const scaleX = img0.w / rule.iw;
  const scaleY = img0.h / rule.ih;
  const outW = Math.max(4, Math.round(rule.sw * scaleX));
  const outH = Math.max(4, Math.round(rule.sh * scaleY));
  const outRgba = new Uint8Array(outW * outH * 4);

  if (job.kind === "copy_pad") {
    for (let y = 0; y < img0.h && y < outH; y++) {
      const sRow = y * img0.w * 4;
      const dRow = y * outW * 4;
      const copyLen = Math.min(img0.w, outW) * 4;
      outRgba.set(img0.rgba.subarray(sRow, sRow + copyLen), dRow);
    }
  } else if (job.kind === "alpha") {
    const img1 = await decodeImageFile(job.f1);
    for (let y = 0; y < img0.h && y < outH; y++) {
      const sy1 = Math.min(img1.h - 1, Math.floor((y * img1.h) / img0.h));
      for (let x = 0; x < img0.w && x < outW; x++) {
        const sx1 = Math.min(img1.w - 1, Math.floor((x * img1.w) / img0.w));
        const s0 = (y * img0.w + x) * 4;
        const s1 = (sy1 * img1.w + sx1) * 4;
        const d = (y * outW + x) * 4;
        outRgba[d] = img0.rgba[s0];
        outRgba[d + 1] = img0.rgba[s0 + 1];
        outRgba[d + 2] = img0.rgba[s0 + 2];
        outRgba[d + 3] = ((img1.rgba[s1] * 77 + img1.rgba[s1 + 1] * 150 + img1.rgba[s1 + 2] * 29) >> 8);
      }
    }
  } else if (job.kind === "palette") {
    const img1 = await decodeImageFile(job.f1);
    for (let y = 0; y < img0.h && y < outH; y++) {
      const sy1 = Math.min(img1.h - 1, Math.floor((y * img1.h) / img0.h));
      for (let x = 0; x < img0.w && x < outW; x++) {
        const sx1 = Math.min(img1.w - 1, Math.floor((x * img1.w) / img0.w));
        const s0 = (y * img0.w + x) * 4;
        const s1 = (sy1 * img1.w + sx1) * 4;
        const d = (y * outW + x) * 4;
        outRgba[d] = img0.rgba[s0];
        outRgba[d + 1] = img0.rgba[s0 + 3];
        outRgba[d + 2] = img1.rgba[s1];
        outRgba[d + 3] = img1.rgba[s1 + 3];
      }
    }
  }

  return encodeDxt5(outW, outH, outRgba);
}

export async function buildTextureDdsBlob(job) {
  if (job.kind === "direct") return job.file;
  const bytes = await buildTextureDdsBytes(job);
  return new Blob([bytes], { type: "application/octet-stream" });
}
