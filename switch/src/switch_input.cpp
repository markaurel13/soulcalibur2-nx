#include <switch.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "input_model.h"

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

    padConfigureInput(4, HidNpadStyleSet_NpadStandard);

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

extern "C" {

bool recomp_input_host_sample(uint32_t port, RecompInputGamepad *gamepad) {
    if (!gamepad || port >= 4) return false;
    memset(gamepad, 0, sizeof(*gamepad));
    
    if (!SwitchInput::s_initialized) {
        SwitchInput::Initialize();
    }

    padUpdate(&SwitchInput::s_pads[port]);
    u64 kHeld = padGetButtons(&SwitchInput::s_pads[port]);
    HidAnalogStickState lStick = padGetStickPos(&SwitchInput::s_pads[port], 0);
    HidAnalogStickState rStick = padGetStickPos(&SwitchInput::s_pads[port], 1);

    if (port == 0) {
        static uint64_t s_last_vtx_toggle_tick = 0;
        static uint64_t s_last_fps_toggle_tick = 0;
        uint64_t now_tick = armGetSystemTick();

        // L + R + DPad Up: Toggle Vertex Pipeline (GPU vs CPU)
        if (now_tick - s_last_vtx_toggle_tick > 9600000ULL) { // 0.5s debounce
            if ((kHeld & HidNpadButton_L) && (kHeld & HidNpadButton_R) && (kHeld & HidNpadButton_Up)) {
                s_gpu_vtx_enabled = !s_gpu_vtx_enabled;
                s_last_vtx_toggle_tick = now_tick;
                printf("[Deko3D] >>> TOGGLED VERTEX PIPELINE: %s <<<\n",
                       s_gpu_vtx_enabled ? "GPU (Fast Fixed-Function)" : "CPU (Legacy Fallback)");
            }
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
    if (kHeld & HidNpadButton_Minus) b |= (1 << 5); // Back
    if (kHeld & HidNpadButton_StickL) b |= (1 << 6);
    if (kHeld & HidNpadButton_StickR) b |= (1 << 7);
    gamepad->buttons = b;

    // Analog buttons: A, B, X, Y, Black (L), White (R), LTrigger (ZL), RTrigger (ZR)
    // Map Switch layout to Xbox layout:
    // Switch B (bottom) -> Xbox A
    // Switch A (right)  -> Xbox B
    // Switch Y (left)   -> Xbox X
    // Switch X (top)    -> Xbox Y
    if (kHeld & HidNpadButton_B) gamepad->analog_buttons[0] = 255;
    if (kHeld & HidNpadButton_A) gamepad->analog_buttons[1] = 255;
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
    if (kHeld & HidNpadButton_Minus) mask |= (1ULL << SDL_BTN_BACK);
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
