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

    self.__swizzlePaletted = (indicesProxy, width, height, size) => {
      const buf = indicesProxy.getBuffer("u8");
      try {
        const indices = buf.data;
        const pixels = new Uint8Array(size * size);
        for (let y = 0; y < size; y++) {
          let sy = 0;
          for (let b = 0; b < 12; b++) sy |= ((y >> b) & 1) << (2 * b + 1);
          const row = Math.floor((y * height) / size) * width;
          for (let x = 0; x < size; x++) {
            let sx = 0;
            for (let b = 0; b < 12; b++) sx |= ((x >> b) & 1) << (2 * b);
            pixels[sx | sy] = indices[row + Math.floor((x * width) / size)];
          }
        }
        return pixels;
      } finally {
        buf.release();
      }
    };

    self.__convertRgba8Image = (vgtProxy, width, height, sw, sh) => {
      const buf = vgtProxy.getBuffer("u8");
      try {
        const src = buf.data;
        const pixels = new Uint8Array(sw * sh * 4);
        const bw = Math.floor((width + 3) / 4);
        const bh = Math.floor((height + 3) / 4);
        let at = 0;
        for (let ty = 0; ty < bh; ty++) {
          for (let tx = 0; tx < bw; tx++) {
            for (let i = 0; i < 16; i++) {
              const x = tx * 4 + (i & 3);
              const y = ty * 4 + (i >> 2);
              const a = src[at + i * 2];
              const r = src[at + i * 2 + 1];
              const g = src[at + 32 + i * 2];
              const b = src[at + 32 + i * 2 + 1];
              if (x < width && y < height) {
                let swz = 0, bit = 0, xx = x, yy = y, ww = sw, hh = sh;
                while (ww > 1 || hh > 1) {
                  if (ww > 1) { swz |= (xx & 1) << bit++; xx >>= 1; ww >>= 1; }
                  if (hh > 1) { swz |= (yy & 1) << bit++; yy >>= 1; hh >>= 1; }
                }
                const dst = swz * 4;
                pixels[dst] = b;
                pixels[dst + 1] = g;
                pixels[dst + 2] = r;
                pixels[dst + 3] = a;
              }
            }
            at += 64;
          }
        }
        return pixels;
      } finally {
        buf.release();
      }
    };

    const PS_F0 = new Int32Array([0, 60, 115, 98, 122]);
    const PS_F1 = new Int32Array([0, 0, -52, -55, -60]);

    self.__psAdpcmDecode = (dataProxy) => {
      const buf = dataProxy.getBuffer("u8");
      try {
        const data = buf.data;
        const maxFrames = Math.floor(data.length / 16);
        const out = new Int16Array(maxFrames * 28);
        let outLen = 0, s1 = 0, s2 = 0;
        for (let at = 0; at + 15 < data.length; at += 16) {
          const header = data[at];
          const flags = data[at + 1];
          const shift = header & 15;
          const predictor = Math.min(4, header >> 4);
          const f0 = PS_F0[predictor];
          const f1 = PS_F1[predictor];
          for (let i = 2; i < 16; i++) {
            const b = data[at + i];
            for (let k = 0; k < 2; k++) {
              const nibble = k === 0 ? (b & 15) : (b >> 4);
              let t = (nibble >= 8 ? nibble - 16 : nibble) << 12;
              t >>= shift;
              let value = t + ((s1 * f0 + s2 * f1) >> 6);
              if (value < -32768) value = -32768;
              else if (value > 32767) value = 32767;
              out[outLen++] = value;
              s2 = s1;
              s1 = value;
            }
          }
          if (flags & 1) break;
        }
        return out.subarray(0, outLen);
      } finally {
        buf.release();
      }
    };

    self.__encodeRgbaRowsToDxt = (rowsProxy, width, height) => {
      const rows = rowsProxy.toJs();
      let opaque = true;
      for (let y = 0; y < height && opaque; y++) {
        const row = rows[y];
        for (let x = 0; x < width; x++) {
          if (row[x][3] !== 255) { opaque = false; break; }
        }
      }
      const bw = Math.floor((width + 3) / 4);
      const bh = Math.floor((height + 3) / 4);
      const stride = opaque ? 8 : 16;
      const out = new Uint8Array(bw * bh * stride);
      const texR = new Uint8Array(16), texG = new Uint8Array(16), texB = new Uint8Array(16), texA = new Uint8Array(16);
      const palR = new Int32Array(4), palG = new Int32Array(4), palB = new Int32Array(4);
      const levels = new Int32Array(8);
      let dst = 0;

      for (let by = 0; by < height; by += 4) {
        for (let bx = 0; bx < width; bx += 4) {
          let k = 0;
          let minR = 255, maxR = 0, minG = 255, maxG = 0, minB = 255, maxB = 0;
          let hiA = 0, loA = 255;
          for (let y = 0; y < 4; y++) {
            const row = rows[Math.min(by + y, height - 1)];
            for (let x = 0; x < 4; x++) {
              const px = row[Math.min(bx + x, width - 1)];
              const r = px[0], g = px[1], b = px[2], a = px[3];
              texR[k] = r; texG[k] = g; texB[k] = b; texA[k] = a;
              k++;
              if (r < minR) minR = r; if (r > maxR) maxR = r;
              if (g < minG) minG = g; if (g > maxG) maxG = g;
              if (b < minB) minB = b; if (b > maxB) maxB = b;
              if (a > hiA) hiA = a; if (a < loA) loA = a;
            }
          }

          if (!opaque) {
            out[dst] = hiA;
            out[dst + 1] = loA;
            if (hiA !== loA) {
              levels[0] = hiA; levels[1] = loA;
              for (let i = 1; i < 7; i++) levels[i + 1] = Math.floor(((7 - i) * hiA + i * loA) / 7);
              let bLo = 0, bHi = 0;
              for (let n = 0; n < 16; n++) {
                const v = texA[n];
                let bestI = 0, bestD = Math.abs(levels[0] - v);
                for (let i = 1; i < 8; i++) {
                  const d = Math.abs(levels[i] - v);
                  if (d < bestD) { bestD = d; bestI = i; }
                }
                const bp = 3 * n;
                if (bp < 24) bLo |= (bestI << bp);
                else bHi |= (bestI << (bp - 24));
              }
              out[dst + 2] = bLo & 0xff; out[dst + 3] = (bLo >> 8) & 0xff; out[dst + 4] = (bLo >> 16) & 0xff;
              out[dst + 5] = bHi & 0xff; out[dst + 6] = (bHi >> 8) & 0xff; out[dst + 7] = (bHi >> 16) & 0xff;
            }
            dst += 8;
          }

          const spanR = maxR - minR, spanG = maxG - minG, spanB = maxB - minB;
          const texC = (spanR >= spanG && spanR >= spanB) ? texR : (spanG >= spanB ? texG : texB);
          let maxIdx = 0, minIdx = 0;
          for (let i = 1; i < 16; i++) {
            if (texC[i] > texC[maxIdx]) maxIdx = i;
            if (texC[i] < texC[minIdx]) minIdx = i;
          }
          let c0 = (((texR[maxIdx] * 31 + 127) / 255) | 0) << 11 | (((texG[maxIdx] * 63 + 127) / 255) | 0) << 5 | (((texB[maxIdx] * 31 + 127) / 255) | 0);
          let c1 = (((texR[minIdx] * 31 + 127) / 255) | 0) << 11 | (((texG[minIdx] * 63 + 127) / 255) | 0) << 5 | (((texB[minIdx] * 31 + 127) / 255) | 0);
          if (c0 < c1) { const tmp = c0; c0 = c1; c1 = tmp; }
          out[dst] = c0 & 0xff; out[dst + 1] = (c0 >> 8) & 0xff;
          out[dst + 2] = c1 & 0xff; out[dst + 3] = (c1 >> 8) & 0xff;
          if (c0 !== c1) {
            const r0 = (c0 >> 11) & 31, g0 = (c0 >> 5) & 63, b0 = c0 & 31;
            const r1 = (c1 >> 11) & 31, g1 = (c1 >> 5) & 63, b1 = c1 & 31;
            palR[0] = (r0 << 3) | (r0 >> 2); palG[0] = (g0 << 2) | (g0 >> 4); palB[0] = (b0 << 3) | (b0 >> 2);
            palR[1] = (r1 << 3) | (r1 >> 2); palG[1] = (g1 << 2) | (g1 >> 4); palB[1] = (b1 << 3) | (b1 >> 2);
            palR[2] = Math.floor((2 * palR[0] + palR[1]) / 3); palG[2] = Math.floor((2 * palG[0] + palG[1]) / 3); palB[2] = Math.floor((2 * palB[0] + palB[1]) / 3);
            palR[3] = Math.floor((palR[0] + 2 * palR[1]) / 3); palG[3] = Math.floor((palG[0] + 2 * palG[1]) / 3); palB[3] = Math.floor((palB[0] + 2 * palB[1]) / 3);
            let bits = 0;
            for (let n = 0; n < 16; n++) {
              const tr = texR[n], tg = texG[n], tb = texB[n];
              let best = 0, bestErr = 1e9;
              for (let i = 0; i < 4; i++) {
                const dr = palR[i] - tr, dg = palG[i] - tg, db = palB[i] - tb;
                const err = dr * dr + dg * dg + db * db;
                if (err < bestErr) { bestErr = err; best = i; }
              }
              bits |= (best << (2 * n));
            }
            out[dst + 4] = bits & 0xff;
            out[dst + 5] = (bits >>> 8) & 0xff;
            out[dst + 6] = (bits >>> 16) & 0xff;
            out[dst + 7] = (bits >>> 24) & 0xff;
          }
          dst += 8;
        }
      }
      return [opaque ? 0x0c : 0x0f, out];
    };

    self.__swizzleRgba8Rows = (rowsProxy, width, height, sw, sh) => {
      const rows = rowsProxy.toJs();
      const pixels = new Uint8Array(sw * sh * 4);
      for (let y = 0; y < height; y++) {
        const row = rows[y];
        for (let x = 0; x < width; x++) {
          const px = row[x];
          let swz = 0, bit = 0, xx = x, yy = y, ww = sw, hh = sh;
          while (ww > 1 || hh > 1) {
            if (ww > 1) { swz |= (xx & 1) << bit++; xx >>= 1; ww >>= 1; }
            if (hh > 1) { swz |= (yy & 1) << bit++; yy >>= 1; hh >>= 1; }
          }
          const dst = swz * 4;
          pixels[dst] = px[2];
          pixels[dst + 1] = px[1];
          pixels[dst + 2] = px[0];
          pixels[dst + 3] = px[3];
        }
      }
      return pixels;
    };

    // ---- PS2 GS Memory & ps2tex V8 JIT accelerators ----
    const GS_BLOCK32 = [
      [0, 1, 4, 5, 16, 17, 20, 21],
      [2, 3, 6, 7, 18, 19, 22, 23],
      [8, 9, 12, 13, 24, 25, 28, 29],
      [10, 11, 14, 15, 26, 27, 30, 31],
    ];
    const GS_BLOCK4 = [
      [0, 2, 8, 10], [1, 3, 9, 11], [4, 6, 12, 14], [5, 7, 13, 15],
      [16, 18, 24, 26], [17, 19, 25, 27], [20, 22, 28, 30], [21, 23, 29, 31],
    ];
    const GS_COLUMN32 = [
      [0, 1, 4, 5, 8, 9, 12, 13],
      [2, 3, 6, 7, 10, 11, 14, 15],
      [16, 17, 20, 21, 24, 25, 28, 29],
      [18, 19, 22, 23, 26, 27, 30, 31],
      [32, 33, 36, 37, 40, 41, 44, 45],
      [34, 35, 38, 39, 42, 43, 46, 47],
      [48, 49, 52, 53, 56, 57, 60, 61],
      [50, 51, 54, 55, 58, 59, 62, 63],
    ];
    const GS_COL8_BASE = [
      [0, 4, 16, 20, 32, 36, 48, 52, 2, 6, 18, 22, 34, 38, 50, 54],
      [8, 12, 24, 28, 40, 44, 56, 60, 10, 14, 26, 30, 42, 46, 58, 62],
      [33, 37, 1, 5, 49, 53, 17, 21, 35, 39, 3, 7, 51, 55, 19, 23],
      [41, 45, 9, 13, 57, 61, 25, 29, 43, 47, 11, 15, 59, 63, 27, 31],
      [96, 100, 112, 116, 64, 68, 80, 84, 98, 102, 114, 118, 66, 70, 82, 86],
      [104, 108, 120, 124, 72, 76, 88, 92, 106, 110, 122, 126, 74, 78, 90, 94],
      [65, 69, 81, 85, 97, 101, 113, 117, 67, 71, 83, 87, 99, 103, 115, 119],
      [73, 77, 89, 93, 105, 109, 121, 125, 75, 79, 91, 95, 107, 111, 123, 127],
    ];
    const GS_COLUMN8 = GS_COL8_BASE.concat(GS_COL8_BASE.map((r) => r.map((b) => b + 128)));
    const GS_COLUMN4 = (() => {
      const pair = (starts, steps) => {
        const row = [];
        for (const s of starts) for (const k of steps) row.push(s + k);
        return [row, row.map((n) => n + 16)];
      };
      const c0 = pair([0, 2, 4, 6], [0, 8, 32, 40, 64, 72, 96, 104]).concat(
        pair([1, 3, 5, 7], [64, 72, 96, 104, 0, 8, 32, 40])
      );
      const c1 = c0.map((row) => row.map((n) => (n ^ 64) + 128));
      const rows = c0.concat(c1);
      return rows.concat(rows.map((r) => r.map((n) => n + 256)));
    })();

    const gsMems = [];
    self._gsAlloc = () => {
      const id = gsMems.length;
      gsMems.push(new Uint8Array(4 << 20));
      return id;
    };

    self._gsUpload = (id, psm, bp, bw, x0, y0, w, h, pixelsProxy) => {
      const d = gsMems[id];
      const buf = pixelsProxy.getBuffer("u8");
      try {
        const src = buf.data;
        const size = psm === 0 ? w * h * 4 : psm === 0x13 ? w * h : (w * h + 1) >> 1;
        let i = 0;
        if (psm === 0) {
          for (let y = y0; y < y0 + h; y++) {
            const pageY = (y >> 5) * bw;
            const bRow = GS_BLOCK32[(y >> 3) & 3];
            const cRow = GS_COLUMN32[y & 7];
            for (let x = x0; x < x0 + w; x++) {
              const block = bp + (pageY + (x >> 6)) * 32 + bRow[(x >> 3) & 7];
              const a = ((block * 64 + cRow[x & 7]) & 0xfffff) * 4;
              if (i + 3 < src.length && i < size) {
                d[a] = src[i]; d[a + 1] = src[i + 1]; d[a + 2] = src[i + 2]; d[a + 3] = src[i + 3];
              } else {
                d[a] = i < src.length && i < size ? src[i] : 0;
                d[a + 1] = i + 1 < src.length && i + 1 < size ? src[i + 1] : 0;
                d[a + 2] = i + 2 < src.length && i + 2 < size ? src[i + 2] : 0;
                d[a + 3] = i + 3 < src.length && i + 3 < size ? src[i + 3] : 0;
              }
              i += 4;
            }
          }
        } else if (psm === 0x13) {
          const bwHalf = bw >> 1;
          for (let y = y0; y < y0 + h; y++) {
            const pageY = (y >> 6) * bwHalf;
            const bRow = GS_BLOCK32[(y >> 4) & 3];
            const cRow = GS_COLUMN8[y & 15];
            for (let x = x0; x < x0 + w; x++) {
              const block = bp + (pageY + (x >> 7)) * 32 + bRow[(x >> 4) & 7];
              const a = (block * 256 + cRow[x & 15]) & 0x3fffff;
              d[a] = i < src.length && i < size ? src[i] : 0;
              i++;
            }
          }
        } else if (psm === 0x14) {
          const bwHalf = bw >> 1;
          for (let y = y0; y < y0 + h; y++) {
            const pageY = (y >> 7) * bwHalf;
            const bRow = GS_BLOCK4[(y >> 4) & 7];
            const cRow = GS_COLUMN4[y & 15];
            for (let x = x0; x < x0 + w; x++) {
              const byteIdx = i >> 1;
              const srcByte = byteIdx < src.length && byteIdx < size ? src[byteIdx] : 0;
              const v = (srcByte >> (4 * (i & 1))) & 15;
              const block = bp + (pageY + (x >> 7)) * 32 + bRow[(x >> 5) & 3];
              const n = (block * 512 + cRow[x & 31]) & 0x7fffff;
              const dByte = n >> 1;
              const shift = 4 * (n & 1);
              d[dByte] = (d[dByte] & (0xf0 >> shift)) | (v << shift);
              i++;
            }
          }
        }
      } finally {
        buf.release();
      }
    };

    self._gsRead32 = (id, bp, bw, w, h) => {
      const d = gsMems[id];
      const out = new Uint8Array(w * h * 4);
      let dst = 0;
      for (let y = 0; y < h; y++) {
        const pageY = (y >> 5) * bw;
        const bRow = GS_BLOCK32[(y >> 3) & 3];
        const cRow = GS_COLUMN32[y & 7];
        for (let x = 0; x < w; x++) {
          const block = bp + (pageY + (x >> 6)) * 32 + bRow[(x >> 3) & 7];
          const a = ((block * 64 + cRow[x & 7]) & 0xfffff) * 4;
          out[dst++] = d[a]; out[dst++] = d[a + 1]; out[dst++] = d[a + 2]; out[dst++] = d[a + 3];
        }
      }
      return out;
    };

    self._gsRead8 = (id, bp, bw, w, h) => {
      const d = gsMems[id];
      const out = new Uint8Array(w * h);
      const bwHalf = bw >> 1;
      let dst = 0;
      for (let y = 0; y < h; y++) {
        const pageY = (y >> 6) * bwHalf;
        const bRow = GS_BLOCK32[(y >> 4) & 3];
        const cRow = GS_COLUMN8[y & 15];
        for (let x = 0; x < w; x++) {
          const block = bp + (pageY + (x >> 7)) * 32 + bRow[(x >> 4) & 7];
          out[dst++] = d[(block * 256 + cRow[x & 15]) & 0x3fffff];
        }
      }
      return out;
    };

    self._gsRead4 = (id, bp, bw, w, h) => {
      const d = gsMems[id];
      const out = new Uint8Array(w * h);
      const bwHalf = bw >> 1;
      let dst = 0;
      for (let y = 0; y < h; y++) {
        const pageY = (y >> 7) * bwHalf;
        const bRow = GS_BLOCK4[(y >> 4) & 7];
        const cRow = GS_COLUMN4[y & 15];
        for (let x = 0; x < w; x++) {
          const block = bp + (pageY + (x >> 7)) * 32 + bRow[(x >> 5) & 3];
          const n = (block * 512 + cRow[x & 31]) & 0x7fffff;
          out[dst++] = (d[n >> 1] >> (4 * (n & 1))) & 15;
        }
      }
      return out;
    };

    self.__ps2RawToSwizzledBgra = (rawProxy, storedW, width, height, sw, sh) => {
      const buf = rawProxy.getBuffer("u8");
      try {
        const raw = buf.data;
        const pixels = new Uint8Array(sw * sh * 4);
        for (let y = 0; y < height; y++) {
          const rowBase = y * storedW * 4;
          for (let x = 0; x < width; x++) {
            const p = rowBase + x * 4;
            let swz = 0, bit = 0, xx = x, yy = y, ww = sw, hh = sh;
            while (ww > 1 || hh > 1) {
              if (ww > 1) { swz |= (xx & 1) << bit++; xx >>= 1; ww >>= 1; }
              if (hh > 1) { swz |= (yy & 1) << bit++; yy >>= 1; hh >>= 1; }
            }
            const dst = swz * 4;
            pixels[dst] = raw[p + 2];
            pixels[dst + 1] = raw[p + 1];
            pixels[dst + 2] = raw[p];
            const a = raw[p + 3] * 2;
            pixels[dst + 3] = a > 255 ? 255 : a;
          }
        }
        return pixels;
      } finally {
        buf.release();
      }
    };

    postProgress(35, "Building guests.olk (Link & Heihachi)...");
    postLog("Running roster.py (streaming Blob-backed Olnk + JS JIT accelerators)...");

    await pyodide.runPythonAsync(`
import sys, struct, builtins
import js
sys.path.insert(0, "/work/scripts")
import olnk
import gs
import ps2tex
import sound
import textures
import vmp2vmx
import build_guest
import roster

class _FastGsMemory(gs.Memory):
    def __init__(self):
        self._id = int(js._gsAlloc())
    def upload32(self, bp, bw, x0, y0, w, h, pixels):
        js._gsUpload(self._id, 0, bp, bw, x0, y0, w, h, pixels)
    def upload(self, psm, bp, bw, x0, y0, w, h, pixels):
        js._gsUpload(self._id, psm, bp, bw, x0, y0, w, h, pixels)
    def read32(self, bp, bw, w, h):
        return bytes(js._gsRead32(self._id, bp, bw, w, h).to_py())
    def read8(self, bp, bw, w, h):
        return bytes(js._gsRead8(self._id, bp, bw, w, h).to_py())
    def read4(self, bp, bw, w, h):
        return bytes(js._gsRead4(self._id, bp, bw, w, h).to_py())

gs.Memory = _FastGsMemory
ps2tex.gs.Memory = _FastGsMemory

class _FastPs2RgbaRows:
    def __init__(self, raw, sw, w, h):
        self.raw = raw
        self.sw = sw
        self.w = w
        self.h = h
    def __len__(self):
        return self.h
    def __getitem__(self, y):
        raw, sw, w = self.raw, self.sw, self.w
        base = y * sw * 4
        return [(raw[base + 4 * x], raw[base + 4 * x + 1], raw[base + 4 * x + 2], min(255, raw[base + 4 * x + 3] * 2)) for x in range(w)]

_orig_ps2tex_rows = ps2tex.Texture.rows
def _fast_ps2tex_rows(self):
    if self.psm == 0:
        sw, sh = self.stored
        raw = self.memory.read32(self.tbp, self.tbw, sw, sh)
        return _FastPs2RgbaRows(raw, sw, self.width, self.height)
    return _orig_ps2tex_rows(self)

ps2tex.Texture.rows = _fast_ps2tex_rows

sound.dsp_decode = lambda data, coefs, samples: js.__dspDecode(data, coefs, samples)
sound.ps_adpcm_decode = lambda data: js.__psAdpcmDecode(data)
sound.xbox_encode = lambda samples: bytes(js.__xboxEncode(samples).to_py())

def _fast_vmp_dxt(rows, width, height):
    if isinstance(rows, _FastPs2RgbaRows):
        rows = [rows[y] for y in range(rows.h)]
    res = js.__encodeRgbaRowsToDxt(rows, width, height)
    return int(res[0]), bytes(res[1].to_py())

vmp2vmx._dxt = _fast_vmp_dxt

def _fast_rgba8_bank(images):
    count = len(images)
    table = 0x14 + 0x24 * count
    data_at = (table + 0x7f) & ~0x7f
    out = bytearray(b"VXT." + bytes((2, 4, 2, 0)) + struct.pack("<IIII", count, 0x14, table, 0))
    entries_b, data_b = bytearray(), bytearray()
    for entry_flags, width, height, rows in images:
        sw, sh = 1 << (width - 1).bit_length(), 1 << (height - 1).bit_length()
        if isinstance(rows, _FastPs2RgbaRows):
            pixels = bytes(js.__ps2RawToSwizzledBgra(rows.raw, rows.sw, width, height, sw, sh).to_py())
        else:
            pixels = bytes(js.__swizzleRgba8Rows(rows, width, height, sw, sh).to_py())
        entries_b += struct.pack("<9I", entry_flags, 0, height << 16 | width, data_at + len(data_b), textures.D3D_A8R8G8B8,
                                 sh << 16 | sw, 1, 0, 0)
        data_b += pixels
        data_b += bytes(-len(data_b) & 0x7f)
    out = (out + entries_b)[:table]
    out += bytes(data_at - len(out))
    return bytes(out + data_b)

textures.rgba8_bank = _fast_rgba8_bank

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

def _fast_paletted_bank(images, size=512):
    count = len(images)
    table = 0x14 + 0x24 * count
    descriptors = table
    palettes_at = (descriptors + 12 * count + 0x7f) & ~0x7f
    data_at = (palettes_at + 0x400 * count + 0x7f) & ~0x7f
    out = bytearray(data_at + size * size * count)
    out[0:0x14] = b"VXT." + bytes((2, 4, 2, 0)) + struct.pack("<III", count, 0x14, descriptors + 12 * count)
    dims = size << 16 | size
    for k, (entry_flags, width, height, palette, indices) in enumerate(images):
        out[palettes_at + 0x400 * k:palettes_at + 0x400 * (k + 1)] = palette
        pixels = bytes(js.__swizzlePaletted(indices, width, height, size).to_py())
        base = data_at + size * size * k
        out[base:base + size * size] = pixels
        word0 = descriptors + 12 * k
        struct.pack_into("<9I", out, 0x14 + 0x24 * k, word0, entry_flags, 0, dims, base, textures.D3D_P8, dims, 1, 0)
        struct.pack_into("<3I", out, descriptors + 12 * k, palettes_at + 0x400 * k, 0x100, 0)
    return bytes(out)

def _fast_convert_rgba8_bank(vgt, flags=None):
    if vgt[:4] != b"VGT.":
        raise ValueError("not a VGT bank")
    count = struct.unpack_from(">I", vgt, 8)[0]
    table = 0x14 + 0x24 * count
    data_at = (table + 0x7f) & ~0x7f
    out = bytearray(b"VXT." + bytes((2, 4, 2, 0)) + struct.pack("<IIII", count, 0x14, table, 0))
    entries_b, data_b = bytearray(), bytearray()
    mv = memoryview(vgt)
    for k in range(count):
        entry = struct.unpack_from(">17I", vgt, 0x18 + k * 0x44)
        fmt, width, height = textures._gx(entry[3])
        if fmt != textures.RGBA8:
            raise ValueError("texture format %d" % fmt)
        entry_flags = flags if flags is not None else textures._MODES.get(entry[1], (0x241b, False))[0]
        sw, sh = 1 << (width - 1).bit_length(), 1 << (height - 1).bit_length()
        sz = textures.rgba8_size(width, height)
        pixels = bytes(js.__convertRgba8Image(mv[entry[4]:entry[4] + sz], width, height, sw, sh).to_py())
        entries_b += struct.pack("<9I", entry_flags, 0, height << 16 | width, data_at + len(data_b), textures.D3D_A8R8G8B8,
                                 sh << 16 | sw, 1, 0, 0)
        data_b += pixels
        data_b += bytes(-len(data_b) & 0x7f)
    out = (out + entries_b)[:table]
    out += bytes(data_at - len(out))
    return bytes(out + data_b)

textures.paletted_bank = _fast_paletted_bank
textures.convert_rgba8_bank = _fast_convert_rgba8_bank

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
    def read(self):
        return _read_blob(self.key, self.at, self.size)

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

def _wrap_hei_ui(x2, p2, work, *a, **kw):
    js.__postProgress(72, "Converting Heihachi (UI & gallery textures)...")
    for idx in set(i for i, _, _, _ in build_guest.HEIHACHI_TEXTURES):
        if idx < len(work) and isinstance(work[idx], BlobSlice):
            work[idx] = work[idx].read()
    return _orig_hei_ui(x2, p2, work, *a, **kw)

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
