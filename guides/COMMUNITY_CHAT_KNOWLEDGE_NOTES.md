# Community Chat Knowledge Notes & Direct Insights

This document preserves the direct advice, practical tips, workflows, and technical lessons shared by experienced Switch homebrew porters (**Stevens**, **ChanseyIsTheBest**, **digdat0**, **Depende3k**, **czn**, **Thorhax**, **ak**, **haze [TICO]**).

---

## 1. Profiling With `perfil_pilas.flag` (NFSMW-NX Sampling Profiler)

**Source:** Stevens (1/11/26)

### What it is
`perfil_pilas.flag` is a runtime switch that activates an in-game stack sampling profiler in the Switch port (originally NFSMW-NX / ReXGlue). The file contains no configuration—it only has to exist on disk.

### How to use it
1. Create an empty file named `perfil_pilas.flag` in `<NRO folder>/logs/rex/` on the Switch SD card. No code rebuild is required.
2. When the port boots, the log will output:
   ```
   Muestreo de pilas: activo (1 ms)
   ```
3. Play the game to capture typical or heavy workloads.
4. Delete `perfil_pilas.flag` when done.

### How it works
- **Every 1 millisecond**, it briefly interrupts each game thread (main thread, graphics thread, audio thread, worker threads) and records the current instruction pointer and call stack.
- **Every 10 seconds**, it aggregates and prints the most frequent call stacks per thread to the log, formatted as percentages (e.g., *"36% of the time, the graphics thread was decoding GPU packets"*).
- **Stutter frames (> 45 ms)**: It logs the exact call stack and thread activity during any frame that exceeded 45 ms.

### Translating Function Addresses
Functions in the log appear as relative hex offsets: `image+0x00abcdef`.
To convert these offsets into function names, file paths, and line numbers:
```bash
aarch64-none-elf-addr2line -f -C -e <build_artifact.elf> <0x00abcdef>
```
> [!IMPORTANT]
> Always archive the exact `.elf` file produced during compilation for the corresponding release build.

### Caution
The sampling profiler introduces interrupt overhead. Overall FPS during profiling sessions will be lower than normal; only the relative stack percentages and stutter traces are meaningful.

---

## 2. Target Performance & Stock Clock Table (No Mandatory Overclocking)

**Source:** Stevens (5/11/26)

The community standard for quality Switch ports (NFSMW-NX, Luigi's Mansion NX, Banjo-Kazooie: Nuts & Bolts NX / renut-nx, Katamari-nx) is to **run stably at default retail clock speeds** without requiring the end-user to overclock.

### Default Nintendo Switch Clock Speeds

| Profile | CPU Clock | GPU Clock | Memory (EMC) Clock | Note |
|---|---|---|---|---|
| **Handheld (Default)** | **1020.0 MHz** | **307.2 MHz** | **1331.2 MHz** | Baseline handheld profile |
| Handheld (Boosted) | 1020.0 MHz | 384.0 / 460.0 MHz | 1600.0 MHz | Allowed in select titles |
| **Docked (Default)** | **1020.0 MHz** | **768.0 MHz** | **1600.0 MHz** | Standard docked profile |

> [!WARNING]
> Do not rely on sys-clk or overclocking as a substitute for optimization. If the port drops below 30 or 60 FPS in handheld mode at 1020 MHz CPU / 307 MHz GPU, identify bottlenecks with sampling profiling and optimize renderer/CPU code paths.

---

## 3. Remote Automation & Debugging Infrastructure

### 3.1 Sys-BotBase & Socket Remote Control
**Repo:** [masagrator/sys-botbase](https://github.com/masagrator/sys-botbase)
- Runs as a background sysmodule on the Switch.
- Listens on TCP sockets to allow:
  - Reading and writing arbitrary game memory spaces.
  - Sending simulated button presses and stick inputs remotely.
  - Automating testing loops, benchmark runs, and AI navigation.

### 3.2 SwitchWakerHD In-Game Debug Server
**Repo:** [centollOS/SwitchWakerHD](https://github.com/centollOS/SwitchWakerHD/blob/main/docs/debug-server.md)
- Embedded TCP debug server listening on port `6543`.
- Provides:
  - Real-time network log streaming (`switchLogTcp`).
  - Remote file reading/writing on the SD card.
  - Triggering scene warps, camera resets, and screenshots.
  - Remote hot deployment: upload updated `.nro` and restart without touching the Switch.

### 3.3 Sys-FTPD-Light
**Repo:** [Thorhax/sys-ftpd-light](https://github.com/Thorhax/sys-ftpd-light)
- Background FTP server built with current devkitPro toolchain.
- Used to pull logs (`sdmc:/switch/<game>/logs/`), upload assets, or toggle `.flag` profiling files without removing the SD card.

### 3.4 Headless & Quiet Ryujinx CI / AI Testing
**Repo:** [ChanseyIsTheBest/ryujinx](https://github.com/ChanseyIsTheBest/ryujinx)
- Custom fork of Ryujinx / Ryubing with:
  - `--quiet-start` / muted audio.
  - Automatic minimized startup.
  - Deko3d and homebrew compatibility fixes.
  - Ideal for autonomous AI agents or local scripts running rapid verification passes on x86_64/Linux.

### 3.5 Disabling Switch Sleep Mode During Development
- On retail/homebrew Switch: System Settings > Sleep Mode > Auto-Sleep (Playing on Console Screen) -> **Never**, and (Connected to TV) -> **Never**.

---

## 4. In-Game HUDs & Performance Overlays

### 4.1 SaltyNX Compatibility
**Guide & Template:** [nfsmw-nx/docs/console-overlays.md](https://github.com/StevensND/nfsmw-nx/blob/main/docs/console-overlays.md) & `switch_saltynx.cpp`
- Provides FPS counters, frame time graphs, CPU/GPU load meters, and resolution metrics through the SaltyNX sysmodule / NX-FPS overlay.

### 4.2 In-Game ImGui Overlay
- Embed [Dear ImGui](https://github.com/ocornut/imgui) on top of the native Deko3d / renderer swapchain.
- Bind the toggle to a controller combo: **L3 + R3** (pressing both thumbsticks simultaneously).
- Export CPU thread times, GPU command processor latency, draw call counts, and memory pool allocations directly to a persistent `.log` file.

---

## 5. Multiplayer Controller Applet Integration

**Source:** ak (20/10/26)
- Always implement the native Horizon controller support applet when supporting local multiplayer.
- Call `appletCreateControllerSupport` / `hidShowControllerSupport` or `appletHolderRequestExit` when entering multiplayer character select or lobby screens.
- This allows players to pair additional Joy-Cons, detach Joy-Cons, or configure horizontal single Joy-Con mode natively without leaving the game.

---

## 6. Memory Access Optimization (`nfsmw-sdk.patch` / Torchlight-NX)

**Source:** czn (5/11/26), repo: [liartes/torchlight-nx](https://github.com/liartes/torchlight-nx)
- For static recompilation runtimes (ReXGlue, 360 recompilers):
  - In register allocation and context management: track functions that retain non-argument registers (`r0`, `r2`, `r11`, `r12`, `f0`, `v32-v63`) across function boundaries (`nonArgumentKeep`).
  - Ring buffer synchronization: ensure the game thread can reset ring buffers asynchronously without races against the GPU worker thread's `read_ptr_index_`.

---

## 7. Mesa vs Deko3D on Horizon OS

**Sources:** Stevens, digdat0, ChanseyIsTheBest
- **Mesa Switch builds:** Mesa 26.2.3 and 26.2.4 unified Horizon SDK (`danfromtico/mesa-switch`, `NaGaa95/mesa-switch`).
- **Verdict:** Mesa (OpenGL ES / Gallium Nouveau) is helpful for initial bringing-up of PC or OpenGL ports. However, shipping with Mesa results in:
  - 5–9 ms CPU overhead per frame in driver validation.
  - Stutters and hitching during shader compilation.
  - Larger binary size (+50 MB).
- **Target Deko3D:** Migrating from Mesa OpenGL to Deko3D reduces CPU render time to ~2.5 ms, stabilizes GPU frame time, eliminates driver stalls, and achieves 60 FPS on stock clocks.

---

## 8. General PC & Console Porting Roadmap Prompt

```text
Port the game [game name] ([year], [source/recomp platform]) to Nintendo Switch using [renderer, e.g. deko3d] as the native renderer. Order of work, one stage at a time, verify each before moving on:
1. Extract, asset structure, and source setup.
2. Asset packaging: Ensure asset paths, case sensitivity, and virtual file system (VFS/RomFS) work correctly on Horizon OS.
3. Analyze core engine dependencies: Software / OpenGL rendering hooks must be decoupled and mapped to [renderer].
4. Check threading requirements (adapt synchronization for Nintendo Switch's multi-core ARM64 architecture).
5. Implement native Switch subsystems: controller applet, audio via SDL2/audren, user save data.
6. Performance profiling and optimization: eliminate bottlenecks to achieve target framerate on stock clocks.
```
