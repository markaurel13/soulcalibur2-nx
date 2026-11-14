#include <switch.h>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cstring>
#include <cstdio>

namespace SwitchFS {

static bool s_romfs_mounted = false;

bool Initialize() {
    Result rc = romfsInit();
    if (R_SUCCEEDED(rc)) {
        s_romfs_mounted = true;
        printf("[SwitchFS] RomFS mounted successfully.\n");
        return true;
    }
    printf("[SwitchFS] Warning: Failed to mount RomFS (0x%x). Fallback to SD card.\n", rc);
    return false;
}

void Shutdown() {
    if (s_romfs_mounted) {
        romfsExit();
        s_romfs_mounted = false;
    }
}

std::string ResolvePath(const std::string& inputPath) {
    std::string path = inputPath;
    // Replace Windows backslashes with forward slashes
    std::replace(path.begin(), path.end(), '\\', '/');

    // Remove leading slash if present
    if (!path.empty() && path[0] == '/') {
        path = path.substr(1);
    }

    // Try RomFS first if available
    std::string candidateRomfs = "romfs:/" + path;
    if (std::filesystem::exists(candidateRomfs)) {
        return candidateRomfs;
    }

    // Try relative SD card path: sdmc:/switch/soulcalibur2/
    std::string candidateSD = "sdmc:/switch/soulcalibur2/" + path;
    if (std::filesystem::exists(candidateSD)) {
        return candidateSD;
    }

    // Case-insensitive lookup fallback
    std::filesystem::path p(candidateRomfs);
    std::filesystem::path parent = p.parent_path();
    std::string filename = p.filename().string();

    if (std::filesystem::exists(parent)) {
        for (const auto& entry : std::filesystem::directory_iterator(parent)) {
            if (strcasecmp(entry.path().filename().string().c_str(), filename.c_str()) == 0) {
                return entry.path().string();
            }
        }
    }

    return candidateRomfs;
}

} // namespace SwitchFS

extern "C" {
void *__real_CreateFileA(
    const char *lpFileName,
    uint32_t dwDesiredAccess,
    uint32_t dwShareMode,
    void *lpSecurityAttributes,
    uint32_t dwCreationDisposition,
    uint32_t dwFlagsAndAttributes,
    void *hTemplateFile);

/* On Nintendo Switch (libnx sdmc:), stat()/fstat() does not provide unique
   st_ino values across files, so win32_compat.c's (st_dev, st_ino) sharing
   check falsely collides save-file writes against open read-only disc files
   (e.g. guests.olk / movie.afs) and returns ERROR_SHARING_VIOLATION (0xc0000043).
   Guest-level sharing for profile paths is still enforced by kernel_file.c's
   profile_sharing_conflict() using host_path. */
void *__wrap_CreateFileA(
    const char *lpFileName,
    uint32_t dwDesiredAccess,
    uint32_t dwShareMode,
    void *lpSecurityAttributes,
    uint32_t dwCreationDisposition,
    uint32_t dwFlagsAndAttributes,
    void *hTemplateFile)
{
    const uint32_t share_all = dwShareMode | 0x7u; /* FILE_SHARE_READ | WRITE | DELETE */
    void *res = __real_CreateFileA(
        lpFileName, dwDesiredAccess, share_all,
        lpSecurityAttributes, dwCreationDisposition,
        dwFlagsAndAttributes, hTemplateFile);
    if (lpFileName && (dwDesiredAccess & 0x40000000u) != 0u &&
        (strstr(lpFileName, "sc2_") != nullptr || strstr(lpFileName, "SaveMeta") != nullptr)) {
        printf("[SwitchFS] Save write CreateFileA('%s', disp=%u, orig_share=0x%x) -> %p\n",
               lpFileName, dwCreationDisposition, dwShareMode, res);
    }
    return res;
}
}
