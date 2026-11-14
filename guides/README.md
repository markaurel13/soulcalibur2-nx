# Switch Recompilation & Native Porting Knowledge Base (`guides/`)

This directory is an authoritative, consolidated technical knowledge base designed specifically for **AI assistants and developers** working on Nintendo Switch native ports and static recompilations (e.g., Soulcalibur II NX, GameCube, Xbox, Xbox 360, and PC recompilations).

When an AI assistant or developer is unsure how to solve an architectural problem, optimize a function, interface with Deko3D, profile CPU stutters, or configure Horizon OS subsystems, **consult the synthesized guides and primary source documents in this directory**.

---

## 📚 Master Synthesized Guides

These core manuals synthesize the collective lessons, best practices, and code patterns across all community Switch ports:

1. [**`COMMUNITY_CHAT_KNOWLEDGE_NOTES.md`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/COMMUNITY_CHAT_KNOWLEDGE_NOTES.md)
   - Transcribed wisdom and practical advice from Stevens, ChanseyIsTheBest, digdat0, Depende3k, czn, ak, etc.
   - Detailed guide on `perfil_pilas.flag` sampling profiler, default stock clock targets (no forced overclocking), Mesa vs Deko3D, controller applet, sys-botbase automation.
2. [**`MASTER_RECOMP_PORTING_PLAYBOOK.md`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/MASTER_RECOMP_PORTING_PLAYBOOK.md)
   - The five-stage recompilation and porting pipeline (Asset extraction, code translation, Horizon OS integration, Deko3D renderer, profiling/tuning).
   - Endianness handling (Big-Endian PPC to Little-Endian ARM64), memory layouts, and thread allocation across 4 CPU cores.
3. [**`DEKO3D_MASTER_RENDERER_GUIDE.md`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/DEKO3D_MASTER_RENDERER_GUIDE.md)
   - Complete architectural guide to devkitPro's low-level Deko3D C API.
   - Devices, queues, memory pools, command buffers, `.dksh` AOT shaders, image/sampler descriptors, swapchains, dynamic uniforms, and state caching.
4. [**`PERFORMANCE_OPTIMIZATION_AND_TUNING.md`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/PERFORMANCE_OPTIMIZATION_AND_TUNING.md)
   - The Two-Track discipline: console-profile-driven hand rewrites vs mechanical compiler optimization.
   - Cortex-A57 compiler tuning (`-O3`, `-flto`, `-mcpu=cortex-a57`, `-ffast-math`), PGO workflows, ARM NEON 128-bit SIMD vectorization, division elimination, and stock clock budgets (Handheld 1020/307 MHz).
5. [**`PROFILING_DEBUGGING_AND_AUTOMATION_TOOLKIT.md`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/PROFILING_DEBUGGING_AND_AUTOMATION_TOOLKIT.md)
   - How to use the `perfil_pilas.flag` 1 ms sampling profiler and decode offsets with `aarch64-none-elf-addr2line`.
   - `SwitchWakerHD` TCP debug server (port 6543) for live log streaming and hot redeployment.
   - `sys-botbase` socket automation (port 6000/6001) for AI-driven testing.
   - Headless quiet Ryujinx CI harness, SaltyNX frame pacing, and Dear ImGui HUD (L3 + R3).
6. [**`SWITCH_HORIZON_PLATFORM_INTEGRATION.md`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/SWITCH_HORIZON_PLATFORM_INTEGRATION.md)
   - Horizon OS case-insensitive RomFS path wrapper.
   - Multi-controller system applet (`hidLaShowControllerSupportForSystem`) for Joy-Con pairing.
   - Low-latency audio (audren / SDL2).
   - Application lifecycle (`appletMainLoop`), sleep mode, and thread core masks.

---

## 🗂️ Primary Source Repositories & Guides

The raw documentation from each upstream repository is archived in dedicated subfolders:

### 1. GameCube Recompilation & Deko3D
- [**`mkdd-nx/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/mkdd-nx/)
  - `RECOMP_PORT_GUIDE.md`: Full guide on porting Mario Kart: Double Dash!! (GameCube) to Switch via static recompilation.
  - `DEKO3D_MIGRATION.md`: Transitioning from OpenGL/Mesa to Deko3D (metrics: CPU time dropped from 5–9 ms to 2.5 ms, locked 60 FPS on stock 307 MHz GPU).
  - `OPTIMIZATION.md`: Practical performance tuning on GameCube recompiled code.
  - `ARCHITECTURE.md` & `DEKO3D_PLAN.md`: System design and graphics command mapping.
- [**`ACGC-Switch-Port/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/ACGC-Switch-Port/)
  - `SWITCH_PORT_GUIDE.md`: Step-by-step GameCube decomp/recomp guide for Animal Crossing.
  - Decompilation basics, Ghidra setup, generating decomp context, `m2c` assembly to C tools.
- [**`SwitchWakerHD/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/SwitchWakerHD/)
  - `debug-server.md`: Implementation and protocol of the TCP in-game debug server on port 6543.

### 2. Xbox & Xbox 360 Static Recompilation
- [**`sa2-nx/docs/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/sa2-nx/docs/)
  - `porting-guide.md`: Complete porting guide for Sonic Adventure 2 (XBLA) with ReXGlue and native Vulkan/Deko3D renderer.
  - `deko3d-renderer-guide.md`: Direct Deko3D renderer setup for Xenos GPU commands.
  - `porting-a-360-game-prompt.md`: AI prompt templates for porting 360 games.
  - `cpu-rounds.md`, `gpu-optimisation-candidates.md`, `switch-testing.md`.
- [**`renut_nx_ultimate_base/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/renut_nx_ultimate_base/)
  - Consolidated Xbox 360 / reNut Switch porting base by `@ChanseyIsTheBest` synthesizing universal fixes and optimizations across 15+ Switch ports (Katamari, SASRT, Blur, Kameo, Halo 3, RR6, Viva Piñata, Bangai-O, etc.).
  - `docs/README.md`: Documentation index.
  - `docs/hand-and-machine-optimising-guide.md` & `docs/optimising-functions.md`: Three-tier optimization discipline (Codegen passes, Machine copies in-place vs hooked, Hand natives), PGO+LTO impact, and console profiling (`switch_hot_pcs.py`, `profile_breakdown.py`).
  - `docs/ultimate-fixes.md` & `docs/off-by-default.md`: Catalog of every universal renderer/codegen/SDK fix with evidence, tests, and measured console trade-offs.
  - `docs/sasrt-batch-cc2.md`, `docs/building.md`, `docs/codegen-crash-fixes.md`, `docs/porting-a-360-game-prompt.md`.
- [**`renut-nx/docs/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/renut-nx/docs/)
  - `deko3d-renderer.md`: In-depth 157 KB documentation of Deko3d renderer architecture.
  - `deko3d-shader-optimisations.md`: Maxwell GPU shader optimizations.
  - `codegen-enhancements.md`: Enhancements to recompiled C code generation.
  - `four-core-mode.md`: Horizon 4-core multi-threading configuration.
  - `pgo-lto.md`: Step-by-step Profile Guided Optimization on Switch.
- [**`nfsmw-nx/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/nfsmw-nx/)
  - `docs/HowToDeko3d.md` & `docs/deko3d-devlog.md`: Low-level Deko3d devlog and transition from Mesa.
  - `docs/console-overlays.md` & `saltynx-template/`: SaltyNX frame pacing and overlay code template.
  - `docs/audio-and-video.md`, `docs/shaders.md`, `docs/measuring.md`.
- [**`katamari-nx/`** & **`bejeweled3-nx/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/katamari-nx/)
  - `optimising-functions.md`: The Two-Track optimization policy (machine vs hand-written) and benchmarking methodology.
- [**`torchlight-nx/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/torchlight-nx/)
  - `nfsmw-sdk.patch`: Patch for high-performance memory and register caching across function calls (`nonArgumentKeep`).
- [**`xboxrecomp/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/xboxrecomp/)
  - Original Xbox x86 binary static recompiler.
- [**`360tools/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/360tools/)
  - Static recompilation toolset for Xbox 360 executables.

### 3. Remote Control, Debugging & Automation Sysmodules
- [**`sys-botbase/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/sys-botbase/)
  - Background sysmodule for remote control via TCP sockets (memory read/write, keypress injection).
- [**`sys-ftpd-light/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/sys-ftpd-light/)
  - Lightweight background FTP daemon updated for modern devkitPro.
- [**`ryujinx/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/ryujinx/)
  - Minimized, muted headless Ryujinx fork for rapid automated testing.
- [**`coreinit-nx/`**, **`mono-nx/`**, **`switch-gdx/`**](file:///home/mark/Proyectos/soulcalibur2_nx/guides/coreinit-nx/)
  - Wii U / Mono / Java runtime layers ported to Horizon OS.

---

## 🔍 Topic Quick Lookup for AI Assistants

| If you are working on... | Read this first |
|---|---|
| Understanding the recompilation pipeline | [`MASTER_RECOMP_PORTING_PLAYBOOK.md`](file:///home/mark/Proyectos/soulcalibur2_nx/guides/MASTER_RECOMP_PORTING_PLAYBOOK.md) |
| Implementing or debugging Deko3D graphics | [`DEKO3D_MASTER_RENDERER_GUIDE.md`](file:///home/mark/Proyectos/soulcalibur2_nx/guides/DEKO3D_MASTER_RENDERER_GUIDE.md) |
| Fixing framerate drops or optimizing a slow function | [`PERFORMANCE_OPTIMIZATION_AND_TUNING.md`](file:///home/mark/Proyectos/soulcalibur2_nx/guides/PERFORMANCE_OPTIMIZATION_AND_TUNING.md) |
| Profiling CPU stutters or interpreting stack traces | [`PROFILING_DEBUGGING_AND_AUTOMATION_TOOLKIT.md`](file:///home/mark/Proyectos/soulcalibur2_nx/guides/PROFILING_DEBUGGING_AND_AUTOMATION_TOOLKIT.md) |
| Joy-Con multiplayer, RomFS case paths, or audio | [`SWITCH_HORIZON_PLATFORM_INTEGRATION.md`](file:///home/mark/Proyectos/soulcalibur2_nx/guides/SWITCH_HORIZON_PLATFORM_INTEGRATION.md) |
| Checking community advice, clock tables, or prompt recipes | [`COMMUNITY_CHAT_KNOWLEDGE_NOTES.md`](file:///home/mark/Proyectos/soulcalibur2_nx/guides/COMMUNITY_CHAT_KNOWLEDGE_NOTES.md) |
