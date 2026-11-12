#pragma once

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED 1
#endif

#include <SDL2/SDL.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SDL_GamepadButton {
    SDL_GAMEPAD_BUTTON_INVALID = -1,
    SDL_GAMEPAD_BUTTON_SOUTH = 0,
    SDL_GAMEPAD_BUTTON_EAST = 1,
    SDL_GAMEPAD_BUTTON_WEST = 2,
    SDL_GAMEPAD_BUTTON_NORTH = 3,
    SDL_GAMEPAD_BUTTON_BACK = 4,
    SDL_GAMEPAD_BUTTON_GUIDE = 5,
    SDL_GAMEPAD_BUTTON_START = 6,
    SDL_GAMEPAD_BUTTON_LEFT_STICK = 7,
    SDL_GAMEPAD_BUTTON_RIGHT_STICK = 8,
    SDL_GAMEPAD_BUTTON_LEFT_SHOULDER = 9,
    SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER = 10,
    SDL_GAMEPAD_BUTTON_DPAD_UP = 11,
    SDL_GAMEPAD_BUTTON_DPAD_DOWN = 12,
    SDL_GAMEPAD_BUTTON_DPAD_LEFT = 13,
    SDL_GAMEPAD_BUTTON_DPAD_RIGHT = 14,
    SDL_GAMEPAD_BUTTON_MISC1 = 15,
    SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1 = 16,
    SDL_GAMEPAD_BUTTON_LEFT_PADDLE1 = 17,
    SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2 = 18,
    SDL_GAMEPAD_BUTTON_LEFT_PADDLE2 = 19,
    SDL_GAMEPAD_BUTTON_TOUCHPAD = 20,
    SDL_GAMEPAD_BUTTON_MISC2 = 21,
    SDL_GAMEPAD_BUTTON_COUNT = 22
} SDL_GamepadButton;

typedef enum SDL_GamepadAxis {
    SDL_GAMEPAD_AXIS_INVALID = -1,
    SDL_GAMEPAD_AXIS_LEFTX = 0,
    SDL_GAMEPAD_AXIS_LEFTY = 1,
    SDL_GAMEPAD_AXIS_RIGHTX = 2,
    SDL_GAMEPAD_AXIS_RIGHTY = 3,
    SDL_GAMEPAD_AXIS_LEFT_TRIGGER = 4,
    SDL_GAMEPAD_AXIS_RIGHT_TRIGGER = 5,
    SDL_GAMEPAD_AXIS_COUNT = 6
} SDL_GamepadAxis;

static inline const char *SDL_GetGamepadStringForButton(SDL_GamepadButton button) {
    switch (button) {
        case SDL_GAMEPAD_BUTTON_SOUTH: return "a";
        case SDL_GAMEPAD_BUTTON_EAST: return "b";
        case SDL_GAMEPAD_BUTTON_WEST: return "x";
        case SDL_GAMEPAD_BUTTON_NORTH: return "y";
        case SDL_GAMEPAD_BUTTON_BACK: return "back";
        case SDL_GAMEPAD_BUTTON_GUIDE: return "guide";
        case SDL_GAMEPAD_BUTTON_START: return "start";
        case SDL_GAMEPAD_BUTTON_LEFT_STICK: return "leftstick";
        case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return "rightstick";
        case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return "leftshoulder";
        case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return "rightshoulder";
        case SDL_GAMEPAD_BUTTON_DPAD_UP: return "dpup";
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return "dpdown";
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return "dpleft";
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return "dpright";
        default: return (const char *)0;
    }
}

static inline SDL_GamepadButton SDL_GetGamepadButtonFromString(const char *str) {
    if (!str) return SDL_GAMEPAD_BUTTON_INVALID;
    if (strcmp(str, "a") == 0) return SDL_GAMEPAD_BUTTON_SOUTH;
    if (strcmp(str, "b") == 0) return SDL_GAMEPAD_BUTTON_EAST;
    if (strcmp(str, "x") == 0) return SDL_GAMEPAD_BUTTON_WEST;
    if (strcmp(str, "y") == 0) return SDL_GAMEPAD_BUTTON_NORTH;
    if (strcmp(str, "back") == 0) return SDL_GAMEPAD_BUTTON_BACK;
    if (strcmp(str, "guide") == 0) return SDL_GAMEPAD_BUTTON_GUIDE;
    if (strcmp(str, "start") == 0) return SDL_GAMEPAD_BUTTON_START;
    if (strcmp(str, "leftstick") == 0) return SDL_GAMEPAD_BUTTON_LEFT_STICK;
    if (strcmp(str, "rightstick") == 0) return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
    if (strcmp(str, "leftshoulder") == 0) return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
    if (strcmp(str, "rightshoulder") == 0) return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
    if (strcmp(str, "dpup") == 0) return SDL_GAMEPAD_BUTTON_DPAD_UP;
    if (strcmp(str, "dpdown") == 0) return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
    if (strcmp(str, "dpleft") == 0) return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
    if (strcmp(str, "dpright") == 0) return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
    return SDL_GAMEPAD_BUTTON_INVALID;
}

static inline const char *SDL_GetGamepadStringForAxis(SDL_GamepadAxis axis) {
    switch (axis) {
        case SDL_GAMEPAD_AXIS_LEFTX: return "leftx";
        case SDL_GAMEPAD_AXIS_LEFTY: return "lefty";
        case SDL_GAMEPAD_AXIS_RIGHTX: return "rightx";
        case SDL_GAMEPAD_AXIS_RIGHTY: return "righty";
        case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: return "lefttrigger";
        case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: return "righttrigger";
        default: return (const char *)0;
    }
}

static inline SDL_GamepadAxis SDL_GetGamepadAxisFromString(const char *str) {
    if (!str) return SDL_GAMEPAD_AXIS_INVALID;
    if (strcmp(str, "leftx") == 0) return SDL_GAMEPAD_AXIS_LEFTX;
    if (strcmp(str, "lefty") == 0) return SDL_GAMEPAD_AXIS_LEFTY;
    if (strcmp(str, "rightx") == 0) return SDL_GAMEPAD_AXIS_RIGHTX;
    if (strcmp(str, "righty") == 0) return SDL_GAMEPAD_AXIS_RIGHTY;
    if (strcmp(str, "lefttrigger") == 0) return SDL_GAMEPAD_AXIS_LEFT_TRIGGER;
    if (strcmp(str, "righttrigger") == 0) return SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
    return SDL_GAMEPAD_AXIS_INVALID;
}

#ifndef SDL_SCANCODE_COUNT
#define SDL_SCANCODE_COUNT SDL_NUM_SCANCODES
#endif

#undef SDL_GetTicks
#define SDL_GetTicks SDL_GetTicks64

#ifdef __cplusplus
}
#endif
