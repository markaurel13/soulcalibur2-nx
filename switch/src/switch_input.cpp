#include <switch.h>
#include <cstdio>
#include <cstdint>

namespace SwitchInput {

static PadState s_pads[4];
static bool s_initialized = false;

void Initialize() {
    if (s_initialized) return;

    // Configure support for up to 4 players (Handheld, Pro Controller, Dual Joy-Con, Single Joy-Con)
    padConfigureInput(4, HidNpadStyleSet_NpadStandard | HidNpadStyleTag_NpadJoyDual | 
                         HidNpadStyleTag_NpadJoyLeft | HidNpadStyleTag_NpadJoyRight);

    for (int i = 0; i < 4; i++) {
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

void Poll(int playerIndex, uint32_t* buttonsDown, uint32_t* buttonsHeld, float* leftStickX, float* leftStickY) {
    if (!s_initialized || playerIndex < 0 || playerIndex >= 4) return;

    padUpdate(&s_pads[playerIndex]);

    u64 kDown = padGetButtonsDown(&s_pads[playerIndex]);
    u64 kHeld = padGetButtons(&s_pads[playerIndex]);
    HidAnalogStickState stick = padGetStickPos(&s_pads[playerIndex], 0);

    if (buttonsDown) *buttonsDown = (uint32_t)kDown;
    if (buttonsHeld) *buttonsHeld = (uint32_t)kHeld;
    if (leftStickX) *leftStickX = (float)stick.x / 32767.0f;
    if (leftStickY) *leftStickY = (float)stick.y / 32767.0f;
}

} // namespace SwitchInput
