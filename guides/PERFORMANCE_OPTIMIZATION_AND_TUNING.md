# Performance Optimization & Tuning Playbook

A rigorous, measurement-first methodology for squeezing maximum performance out of the Nintendo Switch hardware (Cortex-A57 ARM64 @ 1020 MHz, Maxwell Tegra X1 GPU @ 307.2 / 768 MHz).

---

## 1. The Two-Track Optimization Discipline

Adopted from **Katamari-NX** and **Bejeweled3-NX**:

> [!IMPORTANT]
> **Track 1 (Console Profile-Driven): Hand-Written Optimizations**
> - Hand-written function rewrites must **only** come from real console profile data (`perfil_pilas.flag`, SaltyNX, or hardware trace).
> - Never rewrite a function by hand just because it "looks hot on PC/Ryujinx" or "looks inefficient".
> - Every hand-written function incurs ongoing maintenance, testing burden, and bug risk; it must justify itself with real console milliseconds saved.
>
> **Track 2 (Systematic): Mechanical & Compiler Optimizations**
> - Mechanical transformations (compiler flags, inlining, struct packing, PGO, LTO) can and should be applied aggressively across the entire codebase.

---

## 2. Compiler Toolchain & Linker Tuning

### 2.1 Essential AArch64 GCC/Clang Flags
In devkitA64:
```makefile
ARCH_FLAGS := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mcpu=cortex-a57
OPT_FLAGS  := -O3 -fomit-frame-pointer -ffast-math -ftree-vectorize
LTO_FLAGS  := -flto=auto -fuse-linker-plugin
```

- **`-mcpu=cortex-a57`**: Instructs the instruction scheduler to optimize pipeline hazards for the 2-way superscalar out-of-order Cortex-A57 core.
- **`-ffast-math`**: Enables reciprocal approximations and eliminates NaN checks in physics/vector loops (ensure game physics does not rely on strict IEEE exceptions).
- **`-ftree-vectorize`**: Emits 128-bit ARM NEON vector instructions automatically.

### 2.2 Profile-Guided Optimization (PGO)
PGO yields a substantial 5–15% CPU improvement by optimizing branch predictions and instruction cache locality:

1. **Instrumentation Build**:
   ```makefile
   CFLAGS += -fprofile-generate=/switch/game_name/profiles
   ```
2. **Profile Generation**:
   Run the game on console or Ryujinx across several levels, battle rounds, or gameplay loops to write out `.gcda` files.
3. **Optimized Build**:
   ```makefile
   CFLAGS += -fprofile-use=/switch/game_name/profiles -fprofile-correction
   ```

---

## 3. High-Impact CPU Function Rewrites

When a function appears hot in console stack sampling:

### 3.1 Matrix & Vector Math (ARM NEON)
Replace software vector loops with explicit 128-bit NEON intrinsics (`<arm_neon.h>`):
```cpp
// 4x4 Matrix Multiply via NEON
void Mat4x4_Multiply_NEON(const float* a, const float* b, float* out) {
    float32x4_t row0 = vld1q_f32(b + 0);
    float32x4_t row1 = vld1q_f32(b + 4);
    float32x4_t row2 = vld1q_f32(b + 8);
    float32x4_t row3 = vld1q_f32(b + 12);

    for (int i = 0; i < 4; i++) {
        float32x4_t a_elem = vld1q_dup_f32(a + (i * 4) + 0);
        float32x4_t res = vmulq_f32(row0, a_elem);
        a_elem = vld1q_dup_f32(a + (i * 4) + 1);
        res = vfmaq_f32(res, row1, a_elem);
        a_elem = vld1q_dup_f32(a + (i * 4) + 2);
        res = vfmaq_f32(res, row2, a_elem);
        a_elem = vld1q_dup_f32(a + (i * 4) + 3);
        res = vfmaq_f32(res, row3, a_elem);
        vst1q_f32(out + (i * 4), res);
    }
}
```

### 3.2 Division Elimination & Fast Reciprocals
Integer and floating-point divisions are expensive on Cortex-A57 (10–30 cycles):
- Precalculate `1.0f / divisor` outside loops and multiply inside.
- Use NEON `vrecpeq_f32` (reciprocal estimate) + `vrecpsq_f32` (Newton-Raphson step) for batch operations.

### 3.3 Fast Endian Swaps
For recompiled big-endian GameCube or Xbox 360 data structures, use native ARM64 byte swap instructions:
```cpp
#define BSWAP16(x) __builtin_bswap16(x) // emits REV16
#define BSWAP32(x) __builtin_bswap32(x) // emits REV
#define BSWAP64(x) __builtin_bswap64(x) // emits REV (64-bit)
```

---

## 4. Memory Subsystem & Cache Optimization

The Nintendo Switch memory bus (EMC) is clocked at 1331 MHz in handheld and 1600 MHz in docked, shared between the 4 CPU cores and the GPU:

1. **Avoid CPU-GPU Contention**:
   - Minimize CPU reads from GPU uncached memory (`DkMemPoolFlags_CpuUncached`). CPU reads from uncached or write-combined memory bypass L1/L2 caches and can take 200+ cycles per byte.
   - For write-only data (uniform buffers, vertex staging), write sequentially.
2. **Struct Alignment & Packing**:
   - Group frequently accessed fields together on the same 64-byte cache line.
   - Align arrays to 16 bytes for vector loads (`alignas(16)`).
3. **Eliminate Heap Allocations in Tick Loops**:
   - Replace `std::vector`, `std::string`, and `new` calls in frame loops with static ring buffers, stack storage, or fixed arrays.

---

## 5. Benchmarking & Target Stock Clock Budgets

To achieve a solid **60 FPS**, the total frame budget is **16.66 ms**:

| Subsystem | Target Budget (Handheld 1020/307 MHz) | Notes |
|---|---|---|
| **Game Simulation / Recomp CPU** | $\le$ 7.0 ms | Logic, physics, animation, AI |
| **Render Feeder / Deko3D** | $\le$ 2.5 ms | State filtering, command recording |
| **Audio & Streaming Thread** | $\le$ 1.5 ms | Runs asynchronously on secondary core |
| **GPU Rasterization / Shaders** | $\le$ 14.0 ms | Render targets, post-processing |
| **Frame Safety Margin** | $\ge$ 2.0 ms | Prevents drops during sudden particle bursts |

> [!TIP]
> If handheld framerate dips while docked framerate is rock-solid 60 FPS, the bottleneck is almost always GPU fillrate or texture bandwidth (307 MHz vs 768 MHz). Reduce internal resolution slightly, optimize fragment shaders, or disable excessive alpha blend overdraw.
