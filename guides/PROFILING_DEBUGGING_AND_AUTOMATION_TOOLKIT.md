# Profiling, Debugging & Automation Toolkit

A complete guide to profiling, live remote debugging, and test automation for Nintendo Switch ports.

---

## 1. The Sampling Stack Profiler (`perfil_pilas.flag`)

The sampling stack profiler is the gold standard for diagnosing real-world CPU performance bottlenecks without polluting timing measurements with instrumentation overhead.

### How it Works Internally
1. On startup, the game checks for the existence of:
   ```
   sdmc:/switch/<app_name>/logs/rex/perfil_pilas.flag
   ```
2. If present, it creates a high-priority background timer thread configured with a 1 ms period (`svcSleepThread(1000000)` or OS timer interrupt).
3. Every 1 ms:
   - It pauses the tracked application threads (`main`, `render`, `audio`).
   - Retrieves the thread context (`ThreadContext` / registers: `pc`, `lr`, `fp`).
   - Walks the frame pointer (`fp` / `x29`) back up the stack to record the top callers.
   - Resumes the threads.
4. Every 10 seconds:
   - Aggregates call stack frequencies into percentages.
   - Dumps the top hot call paths to the log file.
5. If a frame takes longer than **45 ms** (a stutter frame), it dumps the complete thread activity trace of that frame.

### Symbol Decoding Workflow
When the log outputs:
```
Thread 1 (Graphics):
  36.2% [0x000000710023a140] -> [0x0000007100239b10] -> [0x00000071001f5420]
```
Translate using `aarch64-none-elf-addr2line`:
```bash
# Calculate offset = address - base_image_load_address
aarch64-none-elf-addr2line -f -C -e build/game.elf 0x23a140
```
Output:
```
Deko3dRenderer::ProcessPacket(unsigned int*)
/home/user/project/src/renderer_deko3d.cpp:412
```

---

## 2. Remote Debug Server (`SwitchWakerHD` Protocol)

Based on the [SwitchWakerHD Debug Server](file:///home/mark/Proyectos/soulcalibur2_nx/guides/SwitchWakerHD/debug-server.md):

An embedded TCP server on port `6543` that connects the running Switch instance to the development PC:

### Features:
1. **Live TCP Log Streaming**:
   Streams stdout/stderr and internal log rings directly to your terminal via `nc <switch_ip> 6543`.
2. **Remote File Access**:
   Read and write files directly to the SD card over TCP without ejecting or mounting FTP.
3. **Hot Deployment & Self-Restart**:
   The PC pushes a new `.nro` binary over the socket; the game overwrites its executable and triggers `envSetNextLoad` + exits to reload immediately.
4. **Scene Warps & Cheats**:
   Trigger character changes, stage warps, and god-mode flags over simple text commands.

---

## 3. Remote Control & Bot Testing (`sys-botbase`)

**Repo:** [masagrator/sys-botbase](file:///home/mark/Proyectos/soulcalibur2_nx/guides/sys-botbase/README.md)

`sys-botbase` runs as a persistent Horizon sysmodule listening on port `6000` (text commands) and `6001` (binary memory dumps):

### Text Protocol Commands (Port 6000):
```text
click A              # Press and release button A
press KEY_L          # Hold button L
release KEY_L        # Release button L
setStick LEFT 0 32767 # Push left stick fully upward
peek <addr> <size>   # Read raw memory from running game title
poke <addr> <val>    # Write memory byte into running game
```

### AI Integration Pattern:
An AI agent can query memory states, detect when combat or loading concludes, press buttons to test menus, and verify stability across continuous play sessions autonomously.

---

## 4. Headless & Quiet Ryujinx CI Testing

**Repo:** [ChanseyIsTheBest/ryujinx](file:///home/mark/Proyectos/soulcalibur2_nx/guides/ryujinx/README.md)

Using the custom headless Ryujinx fork:
```bash
./ryujinx-quiet --quiet-start --headless /path/to/game.nro
```
- Starts minimized and muted.
- Logs all debug prints and Deko3d commands to stdout/file.
- Enables CI pipelines or background test scripts to verify compilation and asset loading without needing physical console interaction.

---

## 5. SaltyNX & Dear ImGui In-Game HUDs

### 5.1 SaltyNX Overlay Integration
Include `switch_saltynx.cpp` and `switch_saltynx.h` (from `guides/nfsmw-nx/saltynx-template/`):
- Connects to SaltyNX shared memory.
- Feeds frame present timestamps to NX-FPS.
- Displays accurate FPS and frame pacing metrics on the system overlay without impacting performance.

### 5.2 Dear ImGui Development HUD
- Hook Dear ImGui into the Deko3D swapchain:
  - Create an ImGui font texture and store in a Deko3D memory pool.
  - Draw ImGui indexed vertex lists in a dedicated render pass just before `dkQueuePresentImage`.
- Toggle hotkey: **L3 + R3** (Both analog stick clicks simultaneously).
- Display tabs:
  - **Performance**: Frame times, CPU core loads, memory pool allocations.
  - **Renderer**: Draw call count, shader bind count, texture swaps.
  - **Game State**: Player positions, hitboxes, animation frame indices.
