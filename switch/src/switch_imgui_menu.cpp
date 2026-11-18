#include "switch_imgui_menu.h"
#include "imgui.h"
#include <switch.h>
#include <cstdio>

namespace {

bool g_imgui_ready = false;
ImFont *g_menu_font = nullptr;
ImFont *g_small_font = nullptr;

struct ThemePalette {
    const char *name;
    ImVec4 accent;
    ImVec4 accent_soft;
};

static const ThemePalette kGoldTheme = {
    "Gold", ImVec4(0.95f, 0.76f, 0.25f, 1.0f), ImVec4(0.95f, 0.76f, 0.25f, 0.20f)
};

} // namespace

bool SwitchImGui_Init(uint64_t font_tex_id, const uint8_t **out_rgba, int *out_w, int *out_h) {
    if (g_imgui_ready) return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.DisplaySize = ImVec2(1280.0f, 720.0f);
    io.DeltaTime = 1.0f / 60.0f;

    // Load Switch System Font via pl service
    bool loaded_sys_font = false;
    Result pl_rc = plInitialize(PlServiceType_User);
    if (R_SUCCEEDED(pl_rc)) {
        PlFontData font{};
        if (R_SUCCEEDED(plGetSharedFontByType(&font, PlSharedFontType_Standard)) &&
            font.address && font.size > 0) {
            ImFontConfig cfg;
            cfg.FontDataOwnedByAtlas = false;
            cfg.OversampleH = 2;
            cfg.OversampleV = 2;
            g_menu_font = io.Fonts->AddFontFromMemoryTTF(font.address, (int)font.size, 24.0f, &cfg);
            g_small_font = io.Fonts->AddFontFromMemoryTTF(font.address, (int)font.size, 18.0f, &cfg);
            loaded_sys_font = (g_menu_font != nullptr);
            printf("[ModMenuUI] Loaded Switch System Font (%u bytes)\n", (unsigned)font.size);
        }
        plExit();
    }
    if (!loaded_sys_font) {
        g_menu_font = io.Fonts->AddFontDefault();
        g_small_font = g_menu_font;
        printf("[ModMenuUI] Fallback to ImGui default font (pl_rc=0x%x)\n", pl_rc);
    }

    unsigned char *pixels = nullptr;
    int w = 0, h = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);
    io.Fonts->SetTexID((ImTextureID)(uintptr_t)font_tex_id);

    *out_rgba = pixels;
    *out_w = w;
    *out_h = h;
    g_imgui_ready = true;
    return (pixels != nullptr && w > 0 && h > 0);
}

ImDrawData *SwitchImGui_BuildModMenu(
    bool unlock_all,
    bool nintendo_layout,
    bool hd_textures,
    bool hd_avail,
    bool hd_mipmaps,
    int aniso_level,
    bool handheld_ssaa,
    int cursor,
    bool applied)
{
    if (!g_imgui_ready) return nullptr;

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(1280.0f, 720.0f);
    io.DeltaTime = 1.0f / 60.0f;

    const ThemePalette &theme = kGoldTheme;

    ImGui::NewFrame();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 14.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(26.0f, 22.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.06f, 0.09f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(theme.accent.x, theme.accent.y, theme.accent.z, 0.85f));

    if (g_menu_font) ImGui::PushFont(g_menu_font);

    const float win_w = 800.0f;
    const float win_h = 500.0f;
    ImGui::SetNextWindowPos(ImVec2((1280.0f - win_w) * 0.5f, (720.0f - win_h) * 0.5f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(win_w, win_h), ImGuiCond_Always);

    ImGui::Begin("##SC2ModMenu", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoSavedSettings);

    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 win_pos = ImGui::GetWindowPos();
    ImVec2 win_sz = ImGui::GetWindowSize();

    // Header
    ImGui::TextColored(theme.accent, "SOULCALIBUR II NX");
    const char *badge = applied ? "UNLOCKED 300/300" : "MOD MENU";
    ImVec2 badge_sz = ImGui::CalcTextSize(badge);
    ImGui::SameLine(win_w - 26.0f - badge_sz.x);
    ImGui::TextColored(applied ? ImVec4(0.35f, 0.92f, 0.48f, 1.0f) : ImVec4(0.68f, 0.72f, 0.80f, 1.0f), "%s", badge);

    ImGui::Dummy(ImVec2(0.0f, 4.0f));
    float sep_y = ImGui::GetCursorScreenPos().y;
    dl->AddLine(ImVec2(win_pos.x + 26.0f, sep_y), ImVec2(win_pos.x + win_w - 26.0f, sep_y),
                ImGui::ColorConvertFloat4ToU32(ImVec4(theme.accent.x, theme.accent.y, theme.accent.z, 0.35f)), 1.5f);
    ImGui::Dummy(ImVec2(0.0f, 12.0f));

    struct RowInfo {
        const char *label;
        const char *value;
        ImVec4 value_col;
        const char *desc;
    };

    static const char *kAnisoLabels[] = { "< OFF (1x) >", "< 4x >", "< 8x >", "< 16x >" };
    const char *aniso_val = kAnisoLabels[(aniso_level >= 0 && aniso_level < 4) ? aniso_level : 3];
    ImVec4 aniso_col = (aniso_level > 0)
        ? ImVec4(0.35f, 0.92f, 0.48f, 1.0f)
        : ImVec4(0.92f, 0.42f, 0.42f, 1.0f);

    const char *hd_val = !hd_avail ? "< OFF (No Textures) >" : (hd_textures ? "< ON >" : "< OFF >");
    ImVec4 hd_col = !hd_avail
        ? ImVec4(0.55f, 0.55f, 0.60f, 1.0f)
        : (hd_textures ? ImVec4(0.35f, 0.92f, 0.48f, 1.0f) : ImVec4(0.92f, 0.42f, 0.42f, 1.0f));

    const RowInfo rows[] = {
        {
            "Unlock All (300/300)",
            unlock_all ? "< ON >" : "< OFF >",
            unlock_all ? ImVec4(0.35f, 0.92f, 0.48f, 1.0f) : ImVec4(0.92f, 0.42f, 0.42f, 1.0f),
            "Unlocks all fighters, weapons, stages, costumes, and Museum gallery items."
        },
        {
            "Button Layout",
            nintendo_layout ? "< NINTENDO (A=Right) >" : "< XBOX (A=Bottom) >",
            theme.accent,
            "Swaps A/B in menus for Nintendo controllers while keeping Kick/Guard in combat."
        },
        {
            "HD Textures (DDS)",
            hd_val,
            hd_col,
            hd_avail
                ? "Replaces textures with 4x HD DDS files from sdmc:/switch/soulcalibur2/textures/."
                : "No textures detected in sdmc:/switch/soulcalibur2/textures/"
        },
        {
            "HD Texture Mipmaps",
            hd_mipmaps ? "< ON (Trilinear) >" : "< OFF >",
            hd_mipmaps ? ImVec4(0.35f, 0.92f, 0.48f, 1.0f) : ImVec4(0.92f, 0.42f, 0.42f, 1.0f),
            "Uses mipmap chains and trilinear filtering to eliminate texture shimmering."
        },
        {
            "Anisotropic Filtering",
            aniso_val,
            aniso_col,
            "Sharpens floors and textures viewed at steep angles (16x recommended)."
        },
        {
            "Handheld Resolution",
            handheld_ssaa ? "< 1600x900 (SSAA) >" : "< 1280x720 (Native) >",
            handheld_ssaa ? theme.accent : ImVec4(0.88f, 0.90f, 0.95f, 1.0f),
            "Supersamples Handheld mode at 1600x900 for cleaner edges, or runs native 720p."
        },
    };
    constexpr int kNumRows = (int)(sizeof(rows) / sizeof(rows[0]));

    const float row_h = 44.0f;
    const float row_w = win_w - 52.0f;
    for (int i = 0; i < kNumRows; ++i) {
        bool sel = (i == cursor);
        ImVec2 p0 = ImGui::GetCursorScreenPos();
        ImVec2 p1 = ImVec2(p0.x + row_w, p0.y + row_h);

        if (sel) {
            dl->AddRectFilled(p0, p1, ImGui::ColorConvertFloat4ToU32(theme.accent_soft), 8.0f);
            dl->AddRect(p0, p1, ImGui::ColorConvertFloat4ToU32(ImVec4(theme.accent.x, theme.accent.y, theme.accent.z, 0.85f)),
                        8.0f, 0, 1.5f);
            dl->AddRectFilled(ImVec2(p0.x + 2.0f, p0.y + 8.0f), ImVec2(p0.x + 6.0f, p1.y - 8.0f),
                              ImGui::ColorConvertFloat4ToU32(theme.accent), 2.0f);
        } else {
            dl->AddRectFilled(p0, p1, IM_COL32(24, 27, 36, 255), 8.0f);
        }

        ImGui::SetCursorScreenPos(ImVec2(p0.x + 18.0f, p0.y + 10.0f));
        ImGui::TextColored(sel ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.80f, 0.83f, 0.88f, 1.0f),
                           "%s", rows[i].label);

        ImVec2 val_sz = ImGui::CalcTextSize(rows[i].value);
        ImGui::SetCursorScreenPos(ImVec2(p1.x - 16.0f - val_sz.x, p0.y + 10.0f));
        ImGui::TextColored(rows[i].value_col, "%s", rows[i].value);

        ImGui::SetCursorScreenPos(ImVec2(p0.x, p1.y + 6.0f));
    }

    // Description card
    int safe_cur = (cursor >= 0 && cursor < kNumRows) ? cursor : 0;
    ImVec2 desc_p0 = ImVec2(win_pos.x + 26.0f, win_pos.y + win_sz.y - 96.0f);
    ImVec2 desc_p1 = ImVec2(win_pos.x + win_w - 26.0f, win_pos.y + win_sz.y - 46.0f);
    dl->AddRectFilled(desc_p0, desc_p1, IM_COL32(10, 12, 18, 255), 8.0f);
    dl->AddRect(desc_p0, desc_p1, IM_COL32(255, 255, 255, 36), 8.0f, 0, 1.0f);

    if (g_small_font) ImGui::PushFont(g_small_font);
    ImGui::SetCursorScreenPos(ImVec2(desc_p0.x + 14.0f, desc_p0.y + 15.0f));
    ImVec4 desc_col = (safe_cur == 2 && !hd_avail)
        ? ImVec4(0.95f, 0.55f, 0.35f, 1.0f)
        : ImVec4(0.76f, 0.80f, 0.88f, 1.0f);
    ImGui::TextColored(desc_col, "%s", rows[safe_cur].desc);

    // Footer controls
    ImGui::SetCursorScreenPos(ImVec2(win_pos.x + 26.0f, win_pos.y + win_sz.y - 34.0f));
    ImGui::TextColored(ImVec4(0.58f, 0.62f, 0.70f, 1.0f),
                       "[Up/Down] Navigate    [Left/Right/Confirm] Change    [Hold -] Save & Resume");
    if (g_small_font) ImGui::PopFont();

    ImGui::End();

    if (g_menu_font) ImGui::PopFont();
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(5);

    ImGui::Render();
    return ImGui::GetDrawData();
}
