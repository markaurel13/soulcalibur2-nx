#include <switch.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <atomic>
#include <sys/stat.h>
#include "input_model.h"
#include "switch_mod_menu.h"
#include "sc2_training.h"
#include "d3d_texture_replace.h"

extern "C" uint32_t sc2_scene;
extern "C" uint8_t *recomp_memory(uint32_t guest_address, size_t width);
extern "C" bool Switch_IsAppExiting(void);

#ifdef SWITCH_PGO_GENERATE
extern "C" void __gcov_dump(void);
#endif

static const char *MOD_CONFIG_PATH = "sdmc:/switch/soulcalibur2/save/config.txt";
static const char *HD_TEXTURES_DIR = "sdmc:/switch/soulcalibur2/textures";

static std::atomic<bool> s_mod_menu_open{false};
static std::atomic<bool> s_mod_unlock_all{false};
static std::atomic<bool> s_mod_nintendo_layout{false};
static std::atomic<bool> s_mod_hd_textures{false};
static std::atomic<bool> s_mod_hd_available{false};
static std::atomic<int>  s_mod_cursor{0};
static std::atomic<bool> s_mod_unlock_applied{false};
static bool s_mod_config_loaded = false;

static bool probeHdTexturesAvailable(void) {
    mkdir("sdmc:/switch/soulcalibur2", 0777);
    mkdir(HD_TEXTURES_DIR, 0777);
    recomp_d3d_textures_configure(HD_TEXTURES_DIR, nullptr);
    bool avail = recomp_d3d_textures_active();
    s_mod_hd_available.store(avail, std::memory_order_release);
    return avail;
}

static void applyHdTexturesConfig(bool enabled) {
    mkdir("sdmc:/switch/soulcalibur2", 0777);
    mkdir(HD_TEXTURES_DIR, 0777);
    bool avail = s_mod_hd_available.load(std::memory_order_acquire);
    bool active_req = enabled && avail;
    recomp_d3d_textures_configure(active_req ? HD_TEXTURES_DIR : nullptr, nullptr);
    printf("[ModMenu] HD Textures %s (dir=%s, available=%d, active=%d)\n",
           active_req ? "ENABLED" : "DISABLED", HD_TEXTURES_DIR,
           avail ? 1 : 0, recomp_d3d_textures_active() ? 1 : 0);
}

static void saveModConfig(bool unlock_all, bool nintendo_layout, bool hd_textures) {
    mkdir("sdmc:/switch/soulcalibur2", 0777);
    mkdir("sdmc:/switch/soulcalibur2/save", 0777);
    FILE *f = fopen(MOD_CONFIG_PATH, "w");
    if (f) {
        fprintf(f, "unlock_all=%s\n", unlock_all ? "ON" : "OFF");
        fprintf(f, "button_layout=%s\n", nintendo_layout ? "NINTENDO" : "XBOX");
        fprintf(f, "hd_textures=%s\n", hd_textures ? "ON" : "OFF");
        fclose(f);
    }
}

static void loadOrCreateModConfig(void) {
    if (s_mod_config_loaded) return;
    s_mod_config_loaded = true;
    bool hd_avail = probeHdTexturesAvailable();
    FILE *f = fopen(MOD_CONFIG_PATH, "r");
    if (!f) {
        s_mod_unlock_all.store(false, std::memory_order_release);
        s_mod_nintendo_layout.store(false, std::memory_order_release);
        s_mod_hd_textures.store(hd_avail, std::memory_order_release);
        saveModConfig(false, false, hd_avail);
        applyHdTexturesConfig(hd_avail);
        printf("[ModMenu] Created default %s (unlock_all=OFF, button_layout=XBOX, hd_textures=%s)\n",
               MOD_CONFIG_PATH, hd_avail ? "ON" : "OFF");
        return;
    }
    char line[128];
    bool val = false;
    bool nintendo = false;
    bool hd_tex = hd_avail;
    bool saw_hd_tex = false;
    bool hd_tex_in_file = false;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "unlock_all=", 11) == 0) {
            const char *p = line + 11;
            if (strncmp(p, "ON", 2) == 0 || strncmp(p, "on", 2) == 0 || p[0] == '1') {
                val = true;
            }
        } else if (strncmp(line, "button_layout=", 14) == 0) {
            const char *p = line + 14;
            if (strncmp(p, "NINTENDO", 8) == 0 || strncmp(p, "nintendo", 8) == 0 || p[0] == '1') {
                nintendo = true;
            }
        } else if (strncmp(line, "hd_textures=", 12) == 0) {
            const char *p = line + 12;
            saw_hd_tex = true;
            if (strncmp(p, "OFF", 3) == 0 || strncmp(p, "off", 3) == 0 || p[0] == '0') {
                hd_tex = false;
                hd_tex_in_file = false;
            } else if (strncmp(p, "ON", 2) == 0 || strncmp(p, "on", 2) == 0 || p[0] == '1') {
                hd_tex = true;
                hd_tex_in_file = true;
            }
        }
    }
    fclose(f);
    if (!hd_avail) {
        hd_tex = false;
    }
    if (!saw_hd_tex || (hd_tex != hd_tex_in_file)) {
        saveModConfig(val, nintendo, hd_tex);
    }
    s_mod_unlock_all.store(val, std::memory_order_release);
    s_mod_nintendo_layout.store(nintendo, std::memory_order_release);
    s_mod_hd_textures.store(hd_tex, std::memory_order_release);
    applyHdTexturesConfig(hd_tex);
    printf("[ModMenu] Loaded %s (unlock_all=%s, button_layout=%s, hd_textures=%s, hd_avail=%d)\n",
           MOD_CONFIG_PATH, val ? "ON" : "OFF", nintendo ? "NINTENDO" : "XBOX",
           hd_tex ? "ON" : "OFF", hd_avail ? 1 : 0);
}

static std::atomic<bool> s_ui_unlock_all{false};
static std::atomic<bool> s_ui_nintendo_layout{false};
static std::atomic<bool> s_ui_hd_textures{true};

extern "C" bool SwitchModMenu_IsOpen(void) {
    return s_mod_menu_open.load(std::memory_order_acquire);
}

extern "C" bool SwitchModMenu_GetUnlockAll(void) {
    return (s_mod_menu_open.load(std::memory_order_acquire) ? s_ui_unlock_all : s_mod_unlock_all)
        .load(std::memory_order_acquire);
}

extern "C" bool SwitchModMenu_GetNintendoLayout(void) {
    return (s_mod_menu_open.load(std::memory_order_acquire) ? s_ui_nintendo_layout : s_mod_nintendo_layout)
        .load(std::memory_order_acquire);
}

extern "C" bool SwitchModMenu_GetHdTextures(void) {
    return (s_mod_menu_open.load(std::memory_order_acquire) ? s_ui_hd_textures : s_mod_hd_textures)
        .load(std::memory_order_acquire);
}

extern "C" bool SwitchModMenu_HasHdTextures(void) {
    return s_mod_hd_available.load(std::memory_order_acquire);
}

extern "C" int SwitchModMenu_GetCursor(void) {
    return s_mod_cursor.load(std::memory_order_acquire);
}

extern "C" bool SwitchModMenu_WasApplied(void) {
    return s_mod_unlock_applied.load(std::memory_order_acquire);
}

static bool s_gpu_vtx_enabled = true;
static bool s_fps_overlay_enabled = false;

extern "C" bool Switch_IsGpuVtxEnabled(void) {
    return s_gpu_vtx_enabled;
}

extern "C" void Switch_SetGpuVtxEnabled(bool enabled) {
    s_gpu_vtx_enabled = enabled;
}

extern "C" bool Switch_IsFpsOverlayEnabled(void) {
    return s_fps_overlay_enabled;
}

extern "C" void Switch_SetFpsOverlayEnabled(bool enabled) {
    s_fps_overlay_enabled = enabled;
}

namespace SwitchInput {

PadState s_pads[4];
bool s_initialized = false;

void Initialize() {
    if (s_initialized) return;

    // Solo mandos completos (Handheld, Pro Controller, Joy-Con dual); sin Joy-Con horizontal individual.
    padConfigureInput(4, HidNpadStyleSet_NpadFullCtrl);

    padInitializeDefault(&s_pads[0]);
    for (int i = 1; i < 4; i++) {
        padInitialize(&s_pads[i], (HidNpadIdType)i);
    }

    s_initialized = true;
    printf("[SwitchInput] Subsystem initialized for 4 players.\n");
}

void ShowControllerApplet(int minPlayers, int maxPlayers) {
    printf("[SwitchInput] Invoking Horizon Controller Support Applet (%d-%d players)...\n", minPlayers, maxPlayers);

    HidLaControllerSupportArg arg;
    hidLaCreateControllerSupportArg(&arg);
    arg.hdr.player_count_min = (int8_t)minPlayers;
    arg.hdr.player_count_max = (int8_t)maxPlayers;
    arg.hdr.enable_take_over_connection = true;
    arg.hdr.enable_left_justify = true;

    hidLaShowControllerSupport(nullptr, &arg);
}

} // namespace SwitchInput

static u64 s_pause_prev_held = 0;
static bool s_pause_prev_stick_lr = false;
static bool s_pause_prev_stick_ud = false;
static bool s_pause_minus_released = false;
static uint64_t s_pause_close_minus_start = 0;
static uint64_t s_minus_hold_start = 0;
static bool s_minus_toggled = false;

static bool applyFull300UnlockAll(void) {
    if (!sc2::TrainingUnlockAll()) return false;
    uint8_t *items = recomp_memory(0x3bb368u, 300u);
    std::memset(items, 3u, 300u);
    uint8_t *packed = recomp_memory(0x3bb1f0u, 75u);
    for (uint32_t n = 0; n < 75u; ++n) {
        packed[n] = uint8_t((items[4u * n] & 3u) |
                            ((items[4u * n + 1u] & 3u) << 2) |
                            ((items[4u * n + 2u] & 3u) << 4) |
                            ((items[4u * n + 3u] & 3u) << 6));
    }
    return true;
}

extern "C" void SwitchModMenu_ResetPauseState(void) {
    s_ui_unlock_all.store(s_mod_unlock_all.load(std::memory_order_acquire), std::memory_order_release);
    s_ui_nintendo_layout.store(s_mod_nintendo_layout.load(std::memory_order_acquire), std::memory_order_release);
    s_ui_hd_textures.store(s_mod_hd_textures.load(std::memory_order_acquire), std::memory_order_release);
    padUpdate(&SwitchInput::s_pads[0]);
    s_pause_prev_held = padGetButtons(&SwitchInput::s_pads[0]);
    HidAnalogStickState st = padGetStickPos(&SwitchInput::s_pads[0], 0);
    s_pause_prev_stick_lr = (st.x < -16000) || (st.x > 16000);
    s_pause_prev_stick_ud = (st.y < -16000) || (st.y > 16000);
    s_pause_minus_released = false;
    s_pause_close_minus_start = 0;
}

extern "C" bool SwitchModMenu_TickPause(void) {
    padUpdate(&SwitchInput::s_pads[0]);
    u64 cur_held = padGetButtons(&SwitchInput::s_pads[0]);
    HidAnalogStickState cur_stick = padGetStickPos(&SwitchInput::s_pads[0], 0);
    u64 kDown = cur_held & ~s_pause_prev_held;
    s_pause_prev_held = cur_held;

    if (!(cur_held & HidNpadButton_Minus)) {
        s_pause_minus_released = true;
        s_pause_close_minus_start = 0;
    } else if (s_pause_minus_released) {
        uint64_t t_now = armGetSystemTick();
        if (s_pause_close_minus_start == 0) {
            s_pause_close_minus_start = t_now;
        } else if ((t_now - s_pause_close_minus_start) >= 19200000ULL) {
            s_mod_menu_open.store(false, std::memory_order_release);
            return false;
        }
    }

    bool stick_lr = (cur_stick.x < -16000) || (cur_stick.x > 16000);
    bool stick_lr_down = stick_lr && !s_pause_prev_stick_lr;
    s_pause_prev_stick_lr = stick_lr;

    bool stick_up = (cur_stick.y > 16000);
    bool stick_dn = (cur_stick.y < -16000);
    bool stick_ud = stick_up || stick_dn;
    bool stick_up_down = stick_up && !s_pause_prev_stick_ud;
    bool stick_dn_down = stick_dn && !s_pause_prev_stick_ud;
    s_pause_prev_stick_ud = stick_ud;

    bool need_refresh = false;
    constexpr int kNumOptions = 3;
    if ((kDown & HidNpadButton_Up) || stick_up_down) {
        int c = s_mod_cursor.load(std::memory_order_acquire);
        s_mod_cursor.store((c + kNumOptions - 1) % kNumOptions, std::memory_order_release);
        need_refresh = true;
    }
    if ((kDown & HidNpadButton_Down) || stick_dn_down) {
        int c = s_mod_cursor.load(std::memory_order_acquire);
        s_mod_cursor.store((c + 1) % kNumOptions, std::memory_order_release);
        need_refresh = true;
    }
    u64 confirm_btn = s_mod_nintendo_layout.load(std::memory_order_acquire) ? HidNpadButton_A : HidNpadButton_B;
    if ((kDown & (confirm_btn | HidNpadButton_Left | HidNpadButton_Right)) || stick_lr_down) {
        int c = s_mod_cursor.load(std::memory_order_acquire);
        if (c == 0) {
            bool next_val = !s_ui_unlock_all.load(std::memory_order_acquire);
            s_ui_unlock_all.store(next_val, std::memory_order_release);
            need_refresh = true;
        } else if (c == 1) {
            bool next_layout = !s_ui_nintendo_layout.load(std::memory_order_acquire);
            s_ui_nintendo_layout.store(next_layout, std::memory_order_release);
            need_refresh = true;
        } else if (c == 2) {
            if (s_mod_hd_available.load(std::memory_order_acquire)) {
                bool next_hd = !s_ui_hd_textures.load(std::memory_order_acquire);
                s_ui_hd_textures.store(next_hd, std::memory_order_release);
                need_refresh = true;
            }
        }
    }
    return need_refresh;
}

extern "C" void SwitchModMenu_WaitReleaseOnExit(void) {
    bool prev_unlock = s_mod_unlock_all.load(std::memory_order_acquire);
    bool prev_nintendo = s_mod_nintendo_layout.load(std::memory_order_acquire);
    bool prev_hd = s_mod_hd_textures.load(std::memory_order_acquire);
    bool new_unlock = s_ui_unlock_all.load(std::memory_order_acquire);
    bool new_nintendo = s_ui_nintendo_layout.load(std::memory_order_acquire);
    bool new_hd = s_ui_hd_textures.load(std::memory_order_acquire);
    bool unlock_changed = (new_unlock != prev_unlock);
    bool layout_changed = (new_nintendo != prev_nintendo);
    bool hd_changed = (new_hd != prev_hd);

    s_mod_unlock_all.store(new_unlock, std::memory_order_release);
    s_mod_nintendo_layout.store(new_nintendo, std::memory_order_release);
    s_mod_hd_textures.store(new_hd, std::memory_order_release);

    if (unlock_changed || layout_changed || hd_changed) {
        saveModConfig(new_unlock, new_nintendo, new_hd);
        if (unlock_changed) {
            if (new_unlock) {
                bool ok = (sc2_scene != 0u) && applyFull300UnlockAll();
                if (ok) s_mod_unlock_applied.store(true, std::memory_order_release);
                printf("[ModMenu] Exit commit: applyFull300UnlockAll() -> %s (scene=%u)\n",
                       ok ? "OK" : "DEFERRED", sc2_scene);
            } else {
                s_mod_unlock_applied.store(false, std::memory_order_release);
            }
        }
        if (hd_changed) {
            applyHdTexturesConfig(new_hd);
        }
        printf("[ModMenu] Exit saved: unlock_all=%s, button_layout=%s, hd_textures=%s (%s)\n",
               new_unlock ? "ON" : "OFF", new_nintendo ? "NINTENDO" : "XBOX",
               new_hd ? "ON" : "OFF", MOD_CONFIG_PATH);
    }

    while (!Switch_IsAppExiting()) {
        padUpdate(&SwitchInput::s_pads[0]);
        u64 cur_held = padGetButtons(&SwitchInput::s_pads[0]);
        if ((cur_held & (HidNpadButton_A | HidNpadButton_B | HidNpadButton_Plus)) == 0) {
            break;
        }
        svcSleepThread(16000000ULL);
    }
    s_minus_hold_start = 0;
    s_minus_toggled = true;
    printf("[ModMenu] CLOSED (game resumed)\n");
}

extern "C" {

bool recomp_input_host_sample(uint32_t port, RecompInputGamepad *gamepad) {
    if (!gamepad || port >= 4) return false;
    memset(gamepad, 0, sizeof(*gamepad));
    
    if (!SwitchInput::s_initialized) {
        SwitchInput::Initialize();
    }

    padUpdate(&SwitchInput::s_pads[port]);
    if (port > 0 && !padIsConnected(&SwitchInput::s_pads[port])) {
        return false;
    }
    u64 kHeld = padGetButtons(&SwitchInput::s_pads[port]);
    HidAnalogStickState lStick = padGetStickPos(&SwitchInput::s_pads[port], 0);
    HidAnalogStickState rStick = padGetStickPos(&SwitchInput::s_pads[port], 1);

    if (port == 0) {
        loadOrCreateModConfig();

        // Auto-apply Unlock All once past boot/logos (cur_scene >= 5 and not movie/boot),
        // and re-apply automatically if a save load overwrites 0x3BB368.
        if (s_mod_unlock_all.load(std::memory_order_acquire)) {
            uint32_t cur_scene = *reinterpret_cast<const uint32_t *>(recomp_memory(0x398700u, 4u));
            if (cur_scene >= 5u && cur_scene != 11u && cur_scene != 29u && cur_scene != 34u) {
                const uint8_t *items = recomp_memory(0x3bb368u, 300u);
                bool any_locked = false;
                for (uint32_t i = 0; i < 300u; ++i) {
                    if (items[i] != 3u) {
                        any_locked = true;
                        break;
                    }
                }
                if (any_locked) {
                    if (applyFull300UnlockAll()) {
                        s_mod_unlock_applied.store(true, std::memory_order_release);
                        printf("[ModMenu] applyFull300UnlockAll() applied (cur_scene=%u, req_scene=%u).\n",
                               cur_scene, sc2_scene);
                    }
                } else if (!s_mod_unlock_applied.load(std::memory_order_acquire)) {
                    s_mod_unlock_applied.store(true, std::memory_order_release);
                }
            }
        }

        static uint64_t s_last_fps_toggle_tick = 0;
        uint64_t now_tick = armGetSystemTick();

        // Hold Minus (-) for 1.0 second to open Mod Menu (pause happens cleanly at PRESENT on D3D worker)
        if (kHeld & HidNpadButton_Minus) {
            if (s_minus_hold_start == 0) {
                s_minus_hold_start = now_tick;
                s_minus_toggled = false;
            } else if (!s_minus_toggled && (now_tick - s_minus_hold_start) >= 19200000ULL) {
                s_minus_toggled = true;
                s_ui_unlock_all.store(s_mod_unlock_all.load(std::memory_order_acquire), std::memory_order_release);
                s_ui_nintendo_layout.store(s_mod_nintendo_layout.load(std::memory_order_acquire), std::memory_order_release);
                s_ui_hd_textures.store(s_mod_hd_textures.load(std::memory_order_acquire), std::memory_order_release);
                s_mod_menu_open.store(true, std::memory_order_release);
                printf("[ModMenu] OPENED (pausing at frame boundary)\n");
                return true; // *gamepad stays zeroed
            }
        } else {
            s_minus_hold_start = 0;
            s_minus_toggled = false;
        }

        // L + R + DPad Down OR ZL + ZR + DPad Down: Toggle FPS Overlay
        if (now_tick - s_last_fps_toggle_tick > 9600000ULL) { // 0.5s debounce
            bool combo1 = (kHeld & HidNpadButton_L) && (kHeld & HidNpadButton_R) && (kHeld & HidNpadButton_Down);
            bool combo2 = (kHeld & HidNpadButton_ZL) && (kHeld & HidNpadButton_ZR) && (kHeld & HidNpadButton_Down);
            if (combo1 || combo2) {
                s_fps_overlay_enabled = !s_fps_overlay_enabled;
                s_last_fps_toggle_tick = now_tick;
                printf("[SwitchInput] >>> FPS OVERLAY: %s <<<\n",
                       s_fps_overlay_enabled ? "ENABLED" : "DISABLED");
            }
        }
#ifdef SWITCH_PGO_GENERATE
        // R3 (StickR): Dump PGO .gcda files to SD while in-game
        static uint64_t s_last_pgo_dump_tick = 0;
        if (now_tick - s_last_pgo_dump_tick > 38400000ULL) { // 2.0s debounce
            if (kHeld & HidNpadButton_StickR) {
                s_last_pgo_dump_tick = now_tick;
                mkdir(SWITCH_PGO_OUT_DIR, 0777);
                printf("[PGO] Volcando perfil .gcda a %s...\n", SWITCH_PGO_OUT_DIR);
                fflush(stdout);
                __gcov_dump();
                printf("[PGO] Volcado completado.\n");
                fflush(stdout);
            }
        }
#endif

        // (Mod Menu runs modally above and freezes the game thread until closed.)
    }

    gamepad->thumb_lx = (int16_t)lStick.x;
    gamepad->thumb_ly = (int16_t)lStick.y;
    gamepad->thumb_rx = (int16_t)rStick.x;
    gamepad->thumb_ry = (int16_t)rStick.y;

    // Map Switch buttons to Xbox buttons (XINPUT format)
    uint16_t b = 0;
    if (kHeld & HidNpadButton_Up) b |= (1 << 0);
    if (kHeld & HidNpadButton_Down) b |= (1 << 1);
    if (kHeld & HidNpadButton_Left) b |= (1 << 2);
    if (kHeld & HidNpadButton_Right) b |= (1 << 3);
    if (kHeld & HidNpadButton_Plus) b |= (1 << 4);  // Start
    // Minus (-) is reserved solely for toggling the Mod Menu (no Back action in-game)
    if (kHeld & HidNpadButton_StickL) b |= (1 << 6);
    if (kHeld & HidNpadButton_StickR) b |= (1 << 7);
    gamepad->buttons = b;

    // Analog buttons: A, B, X, Y, Black (L), White (R), LTrigger (ZL), RTrigger (ZR)
    // Xbox layout (default): Switch B (bottom) -> Xbox A, Switch A (right) -> Xbox B
    // Nintendo layout: swaps A/B in menus (cur_scene != 7), keeps physical Kick/Guard in combat (cur_scene == 7)
    uint32_t cur_scene = *reinterpret_cast<const uint32_t *>(recomp_memory(0x398700u, 4u));
    const bool fight_paused = (*reinterpret_cast<const uint32_t *>(recomp_memory(0x0039899Cu, 4u)) != 0u);
    const bool in_fight = (cur_scene == 7u) && !fight_paused;
    if (s_mod_nintendo_layout.load(std::memory_order_acquire) && !in_fight) {
        if (kHeld & HidNpadButton_A) gamepad->analog_buttons[0] = 255;
        if (kHeld & HidNpadButton_B) gamepad->analog_buttons[1] = 255;
    } else {
        if (kHeld & HidNpadButton_B) gamepad->analog_buttons[0] = 255;
        if (kHeld & HidNpadButton_A) gamepad->analog_buttons[1] = 255;
    }
    if (kHeld & HidNpadButton_Y) gamepad->analog_buttons[2] = 255;
    if (kHeld & HidNpadButton_X) gamepad->analog_buttons[3] = 255;
    if (kHeld & HidNpadButton_L) gamepad->analog_buttons[4] = 255;
    if (kHeld & HidNpadButton_R) gamepad->analog_buttons[5] = 255;
    if (kHeld & HidNpadButton_ZL) gamepad->analog_buttons[6] = 255;
    if (kHeld & HidNpadButton_ZR) gamepad->analog_buttons[7] = 255;

    return true;
}

// SDL_GamepadButton bit positions used by upstream recomp runtime
enum {
    SDL_BTN_SOUTH = 0,
    SDL_BTN_EAST = 1,
    SDL_BTN_WEST = 2,
    SDL_BTN_NORTH = 3,
    SDL_BTN_BACK = 4,
    SDL_BTN_GUIDE = 5,
    SDL_BTN_START = 6,
    SDL_BTN_LEFT_STICK = 7,
    SDL_BTN_RIGHT_STICK = 8,
    SDL_BTN_LEFT_SHOULDER = 9,
    SDL_BTN_RIGHT_SHOULDER = 10,
    SDL_BTN_DPAD_UP = 11,
    SDL_BTN_DPAD_DOWN = 12,
    SDL_BTN_DPAD_LEFT = 13,
    SDL_BTN_DPAD_RIGHT = 14,
};

uint64_t recomp_input_host_pad_buttons(void) {
    if (!SwitchInput::s_initialized) SwitchInput::Initialize();
    padUpdate(&SwitchInput::s_pads[0]);
    u64 kHeld = padGetButtons(&SwitchInput::s_pads[0]);

    uint64_t mask = 0;
    if (kHeld & HidNpadButton_B) mask |= (1ULL << SDL_BTN_SOUTH);
    if (kHeld & HidNpadButton_A) mask |= (1ULL << SDL_BTN_EAST);
    if (kHeld & HidNpadButton_Y) mask |= (1ULL << SDL_BTN_WEST);
    if (kHeld & HidNpadButton_X) mask |= (1ULL << SDL_BTN_NORTH);
    // No exponer Minus como SDL_BTN_BACK al runtime de PC (sc2_menu::OpenHeld / runner):
    // en Switch el menú de PC no se renderiza y congelaba la emulación y el audio.
    // El juego original sigue recibiendo Minus como Xbox Back vía recomp_input_host_sample().
    if (kHeld & HidNpadButton_Plus) mask |= (1ULL << SDL_BTN_START);
    if (kHeld & HidNpadButton_StickL) mask |= (1ULL << SDL_BTN_LEFT_STICK);
    if (kHeld & HidNpadButton_StickR) mask |= (1ULL << SDL_BTN_RIGHT_STICK);
    if (kHeld & (HidNpadButton_L | HidNpadButton_ZL)) mask |= (1ULL << SDL_BTN_LEFT_SHOULDER);
    if (kHeld & (HidNpadButton_R | HidNpadButton_ZR)) mask |= (1ULL << SDL_BTN_RIGHT_SHOULDER);
    if (kHeld & HidNpadButton_Up) mask |= (1ULL << SDL_BTN_DPAD_UP);
    if (kHeld & HidNpadButton_Down) mask |= (1ULL << SDL_BTN_DPAD_DOWN);
    if (kHeld & HidNpadButton_Left) mask |= (1ULL << SDL_BTN_DPAD_LEFT);
    if (kHeld & HidNpadButton_Right) mask |= (1ULL << SDL_BTN_DPAD_RIGHT);

    return mask;
}

bool recomp_input_scancode_held(int code) {
    (void)code;
    return false;
}

void recomp_input_host_set_vibration(uint32_t port, uint16_t left, uint16_t right) {
    (void)port; (void)left; (void)right;
}

void recomp_input_host_stop_vibration(void) {
}

}
