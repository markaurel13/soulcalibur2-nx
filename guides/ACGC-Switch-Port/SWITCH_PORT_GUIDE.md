# GameCube decomp → Switch homebrew: the Animal Crossing gcNX playbook

How Animal Crossing (USA, GAFE01) went from a decompiled PC port to a native Switch `.nro` running at 60 fps in
handheld at stock clocks. Use it as the starting point for the next game you own, so the port starts fast and skips the
mistakes already paid for. It is for personal backups of discs you own, played on your own console.

- Repo: github.com/digdat0/ACGC-Switch-Port, cloned to `<repo>`
- Lineage: [ACreTeam/ac-decomp](https://github.com/ACreTeam/ac-decomp) (CC0 decompilation) →
  [flyngmt/ACGC-PC-Port](https://github.com/flyngmt/ACGC-PC-Port) (GX → OpenGL, MIT) →
  [zwaetschge/ACGC-Android-Port](https://github.com/zwaetschge/ACGC-Android-Port) (arm64, GLES 3) → gcNX
- Booted and played on hardware the same day the port started (2026-10-10). v1.0.0-beta, then v1.0.1.
- Starting files for the next game: this repo's `switch/` folder (CMakeLists, build.sh, switch_glue.c,
  operator-new rename, Mesa patches + build script). Copy it and replace the app name and paths.

---

## 0. Ground rules (day one, before any code)

1. **The player supplies the disc.** Never commit or ship game code, game data, disc images, or logs/dumps/
   screenshots that embed assets. Never delete your disc images while cleaning up.
2. **`.gitignore` in the first commit:** `*.iso *.gcm *.ciso`, `build*/`, `*.nro`, `*.elf`, `debug/`, logs,
   `save/`, `cache/`, `settings.ini`.
3. **Commit identity fixed per repo** with a local `git config`: `<github-user> <id+user@users.noreply.github.com>`,
   never a personal email. A fresh clone (including a wiki clone) silently uses the global identity.
4. **Stamp every build** (version + build time in `boot.txt` and the first log line). Check the stamp before reading
   any hardware log, because an old `.nro` left on the SD card explains many "still broken" reports.
5. **Keep the `.elf` of every build that goes to hardware**, and always the release one. Crash addresses only mean
   something against the exact build. Prune old test ELFs to the Recycle Bin (never hard delete).
6. **Ask before commit / push / release / posting to issues.** Test on Switch hardware; copy each set of logs to `<log-folder>` on the PC.
7. **Licences:** decomp CC0, port layers MIT; the Mesa shader-cache patches from centollOS/SwitchWaker are MPL-2.0
   (file-level copyleft: keep them as separate patch files). Check upstream for "no AI contributions" policies before
   proposing anything there. List everything in `THIRD_PARTY.md`.

---

## 1. Picking the next game

The gcNX route works when a game is **100% decompiled and linked** (decomp.dev, GC/Wii filter) **and already has a
PC port**, but no Switch port. Adapting an SDL/OpenGL PC port takes days; writing a PC port from a raw decomp takes
months.

Check and tell the user before starting:
- **Pointer width.** A 32-bit-only PC port (MinGW i686) needs 64-bit work first. An **Android arm64 or 64-bit Linux
  port is the best base**: same CPU family and GLES as the Switch. (AC's 64-bit build still had one memory overrun
  that left invisible solid objects in towns, see §7. Expect a few.)
- **Graphics API.** OpenGL 3.x / GLES 3 maps onto Switch Mesa (nouveau). Vulkan/D3D ports don't.
- **Disc revision** the decomp targets (AC: GAFE01 Rev 0 only). Other regions are separate work.
- **Licences** of decomp and port layer.

---

## 2. Toolchain on Windows

Install devkitPro with `switch-dev switch-sdl2 switch-mesa switch-libdrm_nouveau switch-tools switch-libjpeg-turbo`
(check `<devkitPro>/portlibs/switch/lib` for `libSDL2.a libEGL.a libGLESv2.a libglapi.a libdrm_nouveau.a`).
Build with one command: `./switch/build.sh` (`-k` keeps going after errors).

| Problem | Fix |
|---|---|
| `CMake must be installed and launched from msys2` | Use devkitPro's msys2 cmake/make, not a Windows CMake. |
| Env vars don't cross from Git Bash into msys2; gcc writes temp files to `C:\Windows` and fails | build.sh runs cmake via `<devkitPro>/msys2/usr/bin/env.exe PATH=… TMPDIR=<windows-temp> TMP=… TEMP=…`. |
| `-include/unix/path` not converted to `C:/…` | msys converts standalone args only: `"SHELL:-include ${path}"`. |
| Link: "argument list too long" | `CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS ON` (and CXX). |
| `GLES3/gl3.h`, `SDL2/SDL.h` not found | Add `${DEVKITPRO}/portlibs/switch/include` + `link_directories(.../lib)`; link order `SDL2 EGL GLESv2 glapi drm_nouveau nx m`. |
| Conflicting `bcmp/bcopy/bzero`; `strlen` undeclared in C++ | `switch_decls.h` forced include; guard the decomp's own declarations. |
| No `sys/mman.h`, `dlfcn.h`, `elf.h`, signals | Guard the includes; crash handling via `__libnx_exception_handler`. |
| `nx_create_nro` rejects the icon | 256×256 JPEG composited on white. |
| `python` in Git Bash is devkitPro's msys2 Python and drops inline env vars | Use `py -3` and argv flags. |
| Heredocs in the Bash tool turn `\n` into real newlines, even quoted | Write patch scripts to a file, run `py -3 -I script.py`; assert each replaced string occurs once; preserve CRLF. |
| `cmd //c mklink` → "Invalid switch" | PowerShell `New-Item -ItemType Junction`. |
| A CMake or version change rebuilds everything (10–20 min) | Batch such changes; run long builds in the background. |

Flags: the GX → GL layer and display-list interpreter at `-O3`; decomp code at `-O2 -fwrapv -fno-strict-aliasing`.

**Hotfix worktree:** when a feature branch is dirty and an issue comes in, `git worktree add <fix-worktree>
<release-branch>`, junction the expensive shared build dir (`build-switch-mesa`, the custom Mesa) into it, then fix, build and
release from there without touching the feature branch. (v1.0.1 was built this way.)

---

## 3. Project layout

- Add `switch/` beside the port. Change shared code only under `#ifdef TARGET_SWITCH` so upstream merges stay easy.
- Compile with **`TARGET_ANDROID` + `TARGET_SWITCH`** and implement the Android glue API in `switch/switch_glue.c`:
  SD paths, the desktop-GLSL → GLSL ES rewrite, an empty touch overlay. You inherit the ES 3 context, widescreen UI
  and aspect handling; override only JNI, touch, `CLOCK_BOOTTIME` (→ `SDL_GetPerformanceCounter`) and the back key.
- **Embed shaders** at configure time (`file(READ … HEX)` → generated header), so the SD folder needs only the
  `.nro` and the disc.
- Version in one place: `set(AC_APP_VERSION "1.0.1")` in `switch/CMakeLists.txt`.

| file | role |
|---|---|
| `switch/CMakeLists.txt` | devkitA64 build mirroring the port's source list, response files, shader embedding, `.nro` + SD layout |
| `switch/build.sh` | one-command build with the msys2 workarounds |
| `switch/switch_glue.c` | bootstrap, SD dirs, logs, GLSL rewrite, crash handler → `crash.log`, loading screen, frame stats |
| `switch/switch_decls.h` | forced include for what newlib lacks |
| `switch/cxx_launcher.sh` + `game_new.syms` | renames the game's global `operator new/delete` (§4) |
| `switch/mesa/build_mesa.sh` + `patches/` | Mesa with a persistent shader cache (§6.4) |

---

## 4. The #1 crash: the game's `operator new` captures the GL driver

Symptom: crash in `eglCreateContext` (`nv50_ir::TargetNVC0` / `nv50_ir_get_target_library`, null `this`), in every
launch mode. Cause: the engine (JSystem's `JKRHeap.cpp` and similar) replaces global `::operator new/delete`, which
returns NULL before the game's heaps exist. On PC/Android the driver is a separate DLL/.so; on Switch Mesa is linked
into the same image, so it allocates through the game.

Fix: `cxx_launcher.sh` as `CMAKE_CXX_COMPILER_LAUNCHER` runs `objcopy --redefine-syms=game_new.syms` on every game
object, so the game keeps its allocator under a renamed symbol and Mesa/libstdc++ get malloc. Verify:
`nm -C AnimalCrossing.elf | grep "operator new"`: plain `operator new(unsigned long)` must come from libstdc++.

---

## 5. Port-specific bugs to expect

### 5.1 Memory and pointers
- Arena via an oversized `malloc`/`aligned_alloc`. If the port rebuilds 64-bit pointers from 32-bit fields, reject
  blocks straddling a 4 GiB boundary **and** keep the arena's low 32 bits well above 0 (carve `hi:0x10000000`), or
  small junk integers "recover" into arena pointers and crash the display-list walker.
- Under PIE `__start__` is 0 at runtime; get the image base with `svcQueryMemory` on one of your own functions
  (needed for crash backtraces).
- 64-bit struct size changes cause overruns. Before v1.0, one left house-plot sign markers without their sign
  (invisible solid "rocks"). gcNX fixes the cause **and repairs existing saves at start-up**. When a bug has already
  corrupted saves, ship a repair alongside the fix.

### 5.2 Buttons: SDL2's Switch mapping is positional
devkitPro SDL2 2.28.5 maps the Switch pad Xbox-style (`a:b1, b:b0, x:b3, y:b2`), so **A/B and X/Y are swapped**
versus the labels (issue #2). Fix right after `SDL_Init` in `pc/src/pc_main.c`:

```c
#ifdef TARGET_SWITCH
    if (SDL_GameControllerAddMapping("000038f853776974636820436f6e7400,Switch Controller,"
        "a:b0,b:b1,back:b11,dpdown:b15,dpleft:b12,dpright:b14,dpup:b13,leftshoulder:b6,leftstick:b4,"
        "lefttrigger:b8,leftx:a0,lefty:a1,rightshoulder:b7,rightstick:b5,righttrigger:b9,rightx:a2,"
        "righty:a3,start:b10,x:b2,y:b3,") < 0)
        SWLOG("controller mapping not set: %s\n", SDL_GetError());
#endif
```
libnx bit order: A0 B1 X2 Y3 LS4 RS5 L6 R7 ZL8 ZR9 Plus10 Minus11 Left12 Up13 Right14 Down15. Check both the game and
the port's − menu on hardware. Don't also read HID directly for the same buttons (menus toggle twice). When defaults
change, tell users who remapped in `keybindings.ini` to undo it or delete the file.

### 5.3 Pointers handed to async systems (the town tune bug)
Issue #3: the town tune changed after a reboot. `Na_MelodyStart` gave the audio sequence a *pointer* to the caller's
note buffer (`subtrack->sfx_state`, via `Nap_SetPtr` command 0x10), and sequence ops `0xA0–0xA3` can **write back**
through it. Pressing OK while the tune played saved a modified first note. Fix in
`src/static/jaudio_NES/game/melody.c`: copy the 16 notes into a private static buffer per subtrack before starting.
Storage reference: `Save.melody` is a u64 at 0x20F08, one nibble per note (0–12 notes, 13 random, 14 rest, 15 off).

Lesson: **when game code hands a pointer to audio, DMA or the GPU, assume it may be read later or written through.**
The port's timing differs from the GameCube's, so copy on hand-off.

Technique that found it in one hardware run: temporary `[Tune]` prints at save, load and every write site (scripted
in, reverted before release).

### 5.4 Things the PC port does that the Switch can't
- **HD texture packs** (Dolphin format): a full pack added ~30 s to every start and drew some areas black. gcNX
  ignores `texture_pack/`.
- **Compressed images** (RVZ, WIA, GCZ, NKit): not supported; tell users to convert to ISO in Dolphin.
- Never offer "save anywhere" from the port menu; saving is done in the game (risk of corrupt saves).

---

## 6. Performance on Switch (GL on Mesa nouveau, ~1 GHz A57)

Measure first: the profiler/frame stats at stock clocks, walking around for minutes, read from `stdout.log`.
Split frame time into game logic / display-list translation / GL submit / waits.

### 6.1 Check these first
- **Framebuffer size and MSAA.** `SDL_WINDOW_FULLSCREEN_DESKTOP` (the Android path) gives 1920×1080 even in handheld,
  and the PC port defaulted to 4x MSAA: **130 ms/frame of GPU** with ~140 draws. Create the window at 1280×720
  without fullscreen flags and force MSAA off on Switch, including in the settings-apply path.
- **CPU boost off before gameplay.** `ApmCpuBoostMode_FastLoad` (1785 MHz CPU) drops the GPU to 76.8 MHz. End it at
  the first presented game frame, not after N frames: AC's intro ran at 2–3 fps for 4 minutes because of this.
- **Handheld GPU profile.** Homebrew runs handheld at the system default 0x00020003 (GPU 307.2 MHz).
  `apmSetPerformanceConfiguration(ApmPerformanceMode_Normal, 0x92220008)` (CPU 1020, GPU 460.8, EMC 1331.2) is the
  retail handheld profile, not an overclock (fall back to 0x00020004, GPU 384). Restore the saved configuration at
  exit and re-apply it on operation/performance mode changes and resume. Log real clocks with `clkrstGetClockRate`.

### 6.2 Driver traps
- **Never `glBufferData` per draw.** Replacing a buffer frees the old nouveau bo, and `nouveau_bo_del` waits for the
  GPU, so CPU and GPU serialise (a long first GL call each frame). Stream into one big VBO with
  `GL_MAP_UNSYNCHRONIZED_BIT`, one fence per quarter, `glDrawElementsBaseVertex` / `glDrawArrays(first)`. The same
  applies to textures/buffers deleted right after use.
- **`glFlush()` every ~24 draws.** Nouveau's 512 KB pushbuffer is only kicked when full or at swap. A GameCube frame
  fits, so the GPU idles while the CPU builds the frame (frame time = CPU + GPU, drops even at the lowest resolution).
- **No `glFinish` every frame**, diagnostics included: sample 1 frame in 60.
- **GPU timers** (`GL_EXT_disjoint_timer_query`) read 1/1.6276 of real time; multiply. Each query makes nvc0 wait for
  everything before it, so per-draw numbers are noise (the same draw reads 0.15 or 5 ms): use them to rank, never as
  costs. Drive dynamic resolution from frame time vs CPU time, not per-draw queries.
- Cost model: ~2.4 ms GPU per full 720p screen of fragments at 460.8 MHz (~3.6 ms at 307.2). Count overdraw.

### 6.3 What paid off in AC
- **Mipmaps.** The PC port uploaded only level 0 with `GL_LINEAR`; minified tiled ground and walls without mips were
  the costliest draws (one 54-vertex ground draw = 14 ms GPU). `glGenerateMipmap` + `GL_LINEAR_MIPMAP_LINEAR` for
  repeat/mirror-wrapped or game-mipmapped textures ≥16 px.
- **Per-vertex lighting** in the vertex shader, as GX does. Faithful and a large fragment-cost cut on lit, fogged
  scenery.
- **Compact texture formats** instead of expanding everything to RGBA8.
- **Texture cache with hash buckets** on the data pointer and single-entry LRU eviction. The old one was a linear scan
  over up to 2048 entries per bind, never cleared, evicting half at once: slower the longer you played.
- **GPU-side screen copies** (GXCopyTex). The PC port did `glReadPixels` + CPU re-encode, a full GPU drain on every
  menu open and scene wipe. Blit the region into a GL texture per destination buffer (flip Y), write a stamp into the
  destination memory, and use the GL texture while the stamp is intact (`pc_gx_efb_copy_gpu`).
- **Texture decode bursts** (entering areas, dialogs): decode on the main thread up to ~2.5 ms a frame, then hand
  source + palette to a worker on core 2; skip draws that sample it until the main thread uploads the result.
- **Dynamic resolution:** render into a 1920×1080 FBO allocated once, use its bottom-left W×H, `glBlitFramebuffer`
  to the screen. Drive it by dropped frames (>1.4× the budget), not averages, and back off failed step-ups. Scissor
  the per-frame clear to the used area (+2 px for the blit filter): nouveau has no fast clear. Result: busy scenes
  432p–540p, quiet ones up to 720p, higher docked.
- **Refresh-locked frame pacing.** Advancing game time by CPU-measured delta made "60 fps" feel laggy (gaps wobble
  around 16.7 ms). Round the delta to whole refreshes and carry the remainder (`graph.c`). A locked 30 fps option
  (`switch_fps`) gives the GameCube's own, very even rate.
- **Audio thread on core 1** (`svcSetThreadCoreMask(CUR_THREAD_HANDLE, 1, 1<<1)`, priority 0x2B; main is 0x2C).
  SDL creates threads on the caller's core, and sound lagged while rendering was heavy.

### 6.4 Start-up: the shader cache
Mesa compiles GLSL on the CPU at ~0.3–0.4 s per shader, so precompiling hundreds of variants took a minute every boot.
devkitPro's Mesa 20.1 has its disk cache compiled out and 0 program binary formats, so **rebuild switch-mesa**:
- `switch/mesa/build_mesa.sh` (~8 min) applies centollOS's patches (SwitchWakerHD `tools/switch/mesa` @ 207349b):
  newlib `timespec_get`, compile stats, single-file shader cache, nvc0 codegen cache, mako check for Python 3.12.
- Native in devkitPro's MSYS2, no Docker: `pacman -S meson ninja bison flex python python-mako patch libexpat`; run
  meson with `MSYSTEM=MSYS`; write the cross file yourself (flags `-march=armv8-a+crc+crypto -mtune=cortex-a57
  -mtp=soft -fPIC -ftls-model=local-exec -D__SWITCH__`, system horizon).
- Link the prefix's libEGL/libGLESv2/libglapi; `setenv("MESA_SHADER_CACHE_DIR", "sdmc:/switch/<App>/cache")` before
  `SDL_Init`. Tell players to keep `cache/` on updates (losing it only costs one slow start).
- First start still takes about a minute: show progress on a loading screen, and use the CPU boost only there.
- Mid-play compiles (~0.5 s each) feel like the game jumping: draw new configs with the uber shader, record the key
  for the next boot's precompile, compile only during load bursts.
- Validate GLSL offline with devkitPro's `uam.exe -s vert|frag` (only its deko3d-specific errors are expected).

### 6.5 Logging without hitches
- **SD writes stall ~260 ms now and then.** Buffer stdout (64 KB), flush at loads, slow frames, exit and in the crash
  handler. Logging is **off by default** in gcNX (Gameplay tab → Enable logging), so nothing is written during
  normal play.
- **stdio is not thread-safe here**: the audio thread printing while the main thread printed spliced random memory into
  `stdout.log`. Keep periodic prints on one thread.

---

## 7. The Switch shell

### 7.1 Boot checks (before anything slow)
- Right after the window exists: `appletGetAppletType()` must be Application/SystemApplication. **Title takeover
  (hold R on a game)** gives full memory and a 39-bit address space; Album/applet mode doesn't. The disc must exist,
  parse and carry GAFE01. On failure draw a message and wait for + before `SDL_Quit(); exit(0)`.
- **HOME during a loading screen reboots the console** unless every start-up loop that presents frames calls
  `SDL_PumpEvents()` (runs `appletMainLoop`).
- Docking: SDL2 resizes the window on a dock change only if it's RESIZABLE; a fixed 1280×720 window keeps its buffers
  and the system scales.

### 7.2 SD layout
`sdmc:/switch/AnimalCrossing-gcNX/` (any folder name) holding `AnimalCrossing-gcNX.nro`, one disc image (`.iso`,
`.gcm`, `.ciso`, any file name; the port scans extensions), `save/` (Dolphin-compatible GCI), `cache/`, `settings.ini`,
logs. The build only rewrites the `.nro`, so updating means replacing just that file.

### 7.3 Logs players can send
- `boot.txt`: `main()` was reached, plus the build stamp.
- `stdout.log` / `stderr.log` (when logging is on): step markers, GL vendor/version, timings, profiler; the previous
  run's are kept as `*.prev.log`. Tell players to copy them **before** restarting.
- `crash.log`: written even with logging off. Registers and backtrace as image offsets.
- Atmosphère's `sdmc:/atmosphere/crash_reports/*.log`.
- Purpose-built dumps: ZL + ZR + L3 (logging on) writes a map of the ground around the player (item ids, collision,
  holes). It made the invisible-object reports actionable.

### 7.4 Crash triage
1. Pick the `.elf` whose GNU build ID (`readelf -n`) matches the report's Module Id, then run
   `aarch64-none-elf-addr2line -f -C -i -p -e <that.elf> <offsets>` on the backtrace offsets.
2. 2168-0002 data abort at a small address = null pointer; 2345-xxxx = libnx abort (desc 18 = window/gfx init); `vi` /
   `omm` reports next to yours died as a consequence.
3. **Reboot after any crash before the next test**: a crash can take down `vi`, and the next launch dies in
   `__nx_win_init` with 2345-0018 and a black screen. That is not a new bug.
4. No `boot.txt` = died before `main()`; short `stdout.log` = the last step marker shows where. A report that doesn't
   match may be an old one: compare its Module Id.

### 7.5 Settings and menu
- The − menu from the PC port, with a Switch **Video** tab: Frame rate (60/30, `switch_fps`), Dynamic res
  (`switch_dynamic_res`), Max resolution 480p–720p (`switch_resolution`). Changes apply immediately and save to
  `settings.ini`.
- When an option's meaning changes, rename its key so old values are ignored instead of misread.

---

## 8. Bring-up checklist for the next game

Each step has an exit test; don't start the next until it passes.

1. **Inventory:** disc ID/revision, decomp %, the PC port (prefer Android arm64), licences.
2. **Build skeleton:** `switch/` from the templates, build.sh, CMake mirroring the port's source list. *Exit:* links.
3. **Boot:** window + GLES context, `operator new` rename, arena rules, boot checks. *Exit:* `boot.txt`, first frame.
4. **Graphics correct:** 1280×720, MSAA off, shaders embedded, GLSL ES rewrite. *Exit:* title, menus and gameplay
   look right.
5. **Input, saves, audio:** by-label button mapping, GCI saves, audio thread pinned. *Exit:* play → save → reboot →
   load; buttons match the labels in game and menu.
6. **Shell:** SD layout, logs, crash handler, HOME during loading, sleep, dock changes, clean exit.
7. **Hardware pass:** 30 minutes of mixed play; read the logs every time.
8. **Performance:** §6 in order: framebuffer/MSAA/boost, driver traps, mipmaps, lighting, copies, dynamic res,
   pacing, shader cache. One change at a time, on hardware.
9. **Docs and release** (§9).

---

## 9. Releases and issues

- Bump `AC_APP_VERSION` → full rebuild → check the `boot.txt` stamp → hardware test → user confirms.
- Release notes template (v1.0.1):
  1. Bold first line: no game assets included; requires a USA disc image (GAFE01, ISO/GCM/CISO); keep save backups.
  2. How to update: replace only the `.nro`; saves and settings are kept.
  3. Fixes, each linked to its issue, with any action users must take (undo a `keybindings.ini` workaround; fix an
     already-affected tune once in the editor).
  4. Install steps: folder under `switch/`, disc next to the `.nro`, hold R on a game, start from the Homebrew Menu.
  5. Reporting problems: − → Gameplay → Enable logging → reproduce → attach `stdout.log` and `stderr.log`.
  6. "Unofficial fan project, not affiliated with or endorsed by Nintendo."
- `gh release create vX.Y.Z <nro> --notes-file notes.md` (pre-release while in beta). Keep the release `.elf`.
- Comment on each issue with the cause, the fix and the version; close them once the user agrees.
- Housekeeping: old debug ELFs to the Recycle Bin; never touch disc images.

---

## 10. Working habits that paid off

- One hardware question per build: name the build, say exactly what to test and which log to send back.
- Confirm a "fix" on hardware before shipping it (the button mapping was assumed right until a user report).
- Temporary diagnostic logging gets a unique tag and is reverted before release.
- Keep shared-code edits small and marked `TARGET_SWITCH`; platform code lives in `switch/`.
- Note addresses, offsets and save layouts as you learn them; they're the expensive part to rediscover.
- Lead reports to the user with what works on hardware and what the logs prove; keep toolchain detail short.

---

## Credits
ACreTeam/ac-decomp, flyngmt/ACGC-PC-Port, zwaetschge/ACGC-Android-Port; centollOS/SwitchWaker for the Mesa
shader-cache patches and the Switch GL performance study; devkitPro (libnx, devkitA64, switch-mesa, SDL2).
