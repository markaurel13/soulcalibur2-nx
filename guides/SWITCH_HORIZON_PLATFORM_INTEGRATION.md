# Switch Horizon OS Platform Integration

A reference guide for core system integration on Nintendo Switch (Horizon OS) using **libnx**.

---

## 1. Filesystem & RomFS Architecture

### Case Sensitivity & Path Normalization
Horizon OS filesystems (SD card FAT32/exFAT and RomFS) can be case-sensitive or case-preserving depending on host creation. Games ported from Windows (Xbox / 360 / PC) frequently use mixed casing or backslashes (`\`).

### The Robust VFS Path Wrapper Pattern:
```cpp
#include <string>
#include <algorithm>
#include <filesystem>

std::string NormalizeSwitchPath(const std::string& inputPath) {
    std::string path = inputPath;
    // 1. Convert backslashes to forward slashes
    std::replace(path.begin(), path.end(), '\\', '/');

    // 2. Remove redundant leading slashes if appending to mount root
    if (!path.empty() && path[0] == '/') {
        path = path.substr(1);
    }

    // 3. Resolve RomFS mount prefix
    std::string fullPath = "romfs:/" + path;

    // 4. Case-insensitive lookup fallback if file doesn't exist
    if (!std::filesystem::exists(fullPath)) {
        // Search parent directory for case-insensitive match
        std::filesystem::path p(fullPath);
        std::filesystem::path parent = p.parent_path();
        std::string filename = p.filename().string();
        
        if (std::filesystem::exists(parent)) {
            for (const auto& entry : std::filesystem::directory_iterator(parent)) {
                if (strcasecmp(entry.path().filename().string().c_str(), filename.c_str()) == 0) {
                    return entry.path().string();
                }
            }
        }
    }
    return fullPath;
}
```

---

## 2. Multi-Controller & Horizon Controller Applet

Crucial for fighting games (such as Soulcalibur II), racing games, and party titles with 1 to 4 players.

### 2.1 Launching the Controller Support Applet
When entering character select, multiplayer vs mode, or when a controller disconnects, invoke the Horizon system applet so players can pair, reassign, or configure single horizontal Joy-Cons:

```c
#include <switch.h>

void ShowControllerApplet(int minPlayers, int maxPlayers) {
    SetSysControllerPermitVibrationFlag(true);

    HidLaControllerSupportArg arg;
    hidLaCreateControllerSupportArg(&arg);
    arg.hdr.player_count_min = minPlayers;
    arg.hdr.player_count_max = maxPlayers;
    arg.hdr.enable_take_over_connection = true;
    arg.hdr.enable_left_justify = true;
    arg.hdr.permit_vibration = true;

    // Show system modal dialog
    hidLaShowControllerSupportForSystem(&arg);
}
```

### 2.2 Controller Polling Loop (Libnx `pad` API)
```c
PadState pad;
padConfigureInput(1, HidNpadStyleSet_NpadStandard);
padInitializeDefault(&pad);

void UpdateInput() {
    padUpdate(&pad);
    u64 kDown = padGetButtonsDown(&pad);
    u64 kHeld = padGetButtons(&pad);
    HidAnalogStickState leftStick = padGetStickPos(&pad, 0);

    if (kDown & HidNpadButton_Plus) {
        // Pause / Start
    }
}
```

---

## 3. Low-Latency Audio Architecture

### Hardware & Service:
Switch provides **audren** (Audio Renderer), a hardware DSP service managed by Horizon.

### Recommended Approaches:
1. **SDL2 Audio with Audren Backend**:
   - Compiling SDL2 with devkitPro provides a native audren driver out-of-the-box.
   - Low CPU overhead, handles sample rate conversion (32 kHz / 44.1 kHz $\rightarrow$ 48 kHz).
2. **Native Audren**:
   - Zero-copy buffer queuing directly to DSP.
   - Recommended buffer size: 3–4 sub-frames of 5.33 ms (total 16–21 ms audio latency) at 48000 Hz stereo.

---

## 4. Application Lifecycle & Horizon Events

A well-behaved Switch title must properly handle OS suspension, sleep, and docked transitions:

```c
while (appletMainLoop()) {
    // 1. Process OS Applet Messages
    AppletMessage msg;
    while (R_SUCCEEDED(appletGetMessage(&msg))) {
        switch (msg) {
            case AppletMessage_OperationModeChanged:
                // Docked <-> Handheld transition:
                // Check if resolution or clock profile changed
                if (appletGetOperationMode() == AppletOperationMode_Docked) {
                    // Update render target / dynamic scaling if needed
                }
                break;
            case AppletMessage_ExitRequested:
                // Graceful game exit requested by OS
                return;
            default:
                break;
        }
    }

    // 2. Run Game Simulation Frame
    UpdateGame();

    // 3. Render Frame
    RenderGame();
}
```

---

## 5. Threading & 4-Core Mode Configuration

In `npdmtool` / application configuration (`.json` or `.npdm`):
- Set `CoreMask` to allow execution across application cores.
- Pin CPU worker threads explicitly with `threadCreate` + `threadSetCoreMask`:
  - Main Game Thread: Core 1
  - Deko3D Feeder: Core 2
  - Audio & Background Asset IO: Core 3
