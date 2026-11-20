#pragma once

#include <cstdint>

struct ImDrawData;

// Initializes Dear ImGui context + Switch System Font (once) and returns the RGBA32 font atlas.
// Caller uploads the returned RGBA32 buffer (width * height * 4 bytes) to `font_tex_id`.
bool SwitchImGui_Init(uint64_t font_tex_id, const uint8_t **out_rgba, int *out_w, int *out_h);

// Builds a single ImGui frame for the Soulcalibur II NX Mod Menu at logical 1280x720.
ImDrawData *SwitchImGui_BuildModMenu(
    bool unlock_all,
    bool nintendo_layout,
    bool hd_textures,
    bool hd_avail,
    bool hd_mipmaps,
    int aniso_level,
    int msaa_level,
    int cursor,
    bool applied);
