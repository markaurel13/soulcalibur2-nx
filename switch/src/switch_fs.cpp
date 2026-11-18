#include <switch.h>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>

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
extern uint32_t sc2_scene;

void *__real_CreateFileA(
    const char *lpFileName,
    uint32_t dwDesiredAccess,
    uint32_t dwShareMode,
    void *lpSecurityAttributes,
    uint32_t dwCreationDisposition,
    uint32_t dwFlagsAndAttributes,
    void *hTemplateFile);

int __real_ReadFile(
    void *hFile, void *lpBuffer, uint32_t nNumberOfBytesToRead,
    uint32_t *lpNumberOfBytesRead, void *lpOverlapped);

int __real_CloseHandle(void *hObject);

namespace {
std::mutex g_olk_mtx;
std::unordered_set<void *> g_olk_handles;
std::atomic<uint64_t> s_last_disc_io_tick{0};
std::atomic<uint64_t> s_olk_ticks{0};
std::atomic<uint32_t> s_olk_calls{0};
std::atomic<uint64_t> s_olk_bytes{0};

bool isOlkRead(const char *path, uint32_t access) {
    if (!path || (access & 0x40000000u) != 0u) return false;
    return (std::strstr(path, "guests.olk") != nullptr ||
            std::strstr(path, "root.olk") != nullptr);
}

bool isDiscAssetRead(const char *path, uint32_t access) {
    if (!path || (access & 0x40000000u) != 0u) return false;
    return isOlkRead(path, access) ||
           std::strstr(path, "bgm.afs") != nullptr ||
           std::strstr(path, "movie.afs") != nullptr;
}

class OlkSelectCache {
public:
    struct Cell {
        uint64_t offset = 0;
        uint32_t size = 0;
        std::vector<uint8_t> data;
    };

    ~OlkSelectCache() {
        stop_ = true;
        if (worker_.joinable()) worker_.join();
    }

    void maybeStart(const std::string &path) {
        if (path.empty() || started_.load(std::memory_order_acquire)) return;
        std::lock_guard<std::mutex> lk(start_mtx_);
        if (started_.load(std::memory_order_relaxed)) return;
        started_.store(true, std::memory_order_release);
        worker_ = std::thread([this, path]() { preloadWorker(path); });
    }

    bool isReady() const { return ready_.load(std::memory_order_acquire); }

    bool tryRead(uint64_t off, void *dst, uint32_t len) const {
        if (!ready_.load(std::memory_order_acquire) || !dst || len == 0) return false;
        // Binary search by offset
        size_t lo = 0, hi = cells_.size();
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2u;
            if (cells_[mid].offset <= off) lo = mid + 1u;
            else hi = mid;
        }
        if (lo == 0) return false;
        const Cell &c = cells_[lo - 1u];
        if (off < c.offset || off + (uint64_t)len > c.offset + (uint64_t)c.data.size()) return false;
        std::memcpy(dst, c.data.data() + (off - c.offset), len);
        return true;
    }

private:
    static uint32_t readLe32(const uint8_t *p) {
        uint32_t v;
        std::memcpy(&v, p, 4);
        return v;
    }

    static bool readOlnkHeader(FILE *f, uint64_t base, uint32_t &out_count, uint32_t &out_hdr,
                               std::vector<std::pair<uint32_t, uint32_t>> &out_entries) {
        uint8_t head[32];
        if (std::fseek(f, (long)base, SEEK_SET) != 0 || std::fread(head, 1, 32, f) != 32) return false;
        if (std::memcmp(head + 4, "olnk", 4) != 0) return false;
        out_count = readLe32(head + 0);
        out_hdr = readLe32(head + 16);
        if (out_count == 0 || out_count > 4096) return false;
        std::vector<uint8_t> idx((size_t)out_count * 16u);
        if (std::fread(idx.data(), 1, idx.size(), f) != idx.size()) return false;
        out_entries.resize(out_count);
        for (uint32_t i = 0; i < out_count; ++i) {
            out_entries[i].first = readLe32(idx.data() + i * 16u);
            out_entries[i].second = readLe32(idx.data() + i * 16u + 4u);
        }
        return true;
    }

    void preloadWorker(std::string path) {
        FILE *f = std::fopen(path.c_str(), "rb");
        if (!f && path[0] == '/') {
            std::string sdmc = std::string("sdmc:") + path;
            f = std::fopen(sdmc.c_str(), "rb");
            if (f) path = std::move(sdmc);
        }
        if (!f) return;

        uint32_t c0 = 0, h0 = 0;
        std::vector<std::pair<uint32_t, uint32_t>> e0;
        if (!readOlnkHeader(f, 0, c0, h0, e0) || c0 <= 4 || e0[4].second == 0) {
            std::fclose(f);
            return;
        }
        uint64_t b4 = (uint64_t)h0 + e0[4].first;
        uint32_t c4 = 0, h4 = 0;
        std::vector<std::pair<uint32_t, uint32_t>> e4;
        if (!readOlnkHeader(f, b4, c4, h4, e4) || c4 <= 46) {
            std::fclose(f);
            return;
        }

        std::vector<Cell> loaded;
        uint32_t num_chars = (c4 - 46u) / 39u;
        size_t total_bytes = 0;
        uint64_t t0 = armGetSystemTick();

        for (uint32_t b = 0; b < num_chars && !stop_; ++b) {
            uint32_t base_ci = 46u + 39u * b;
            uint32_t use_k = (e4[base_ci + 8u].second > 0u) ? 8u : 0u;
            uint32_t off = e4[base_ci + use_k].first;
            uint32_t sz = e4[base_ci + use_k].second;
            if (sz == 0u) continue;
            uint32_t aligned_sz = (sz + 2047u) & ~2047u;

            Cell cell;
            cell.offset = b4 + (uint64_t)h4 + (uint64_t)off;
            cell.size = aligned_sz;
            cell.data.resize(aligned_sz);

            if (std::fseek(f, (long)cell.offset, SEEK_SET) != 0) continue;
            static constexpr size_t kChunk = 131072u; // 128 KB chunks to avoid starving game I/O
            size_t got_tot = 0;
            while (!stop_ && got_tot < aligned_sz) {
                size_t want = std::min(kChunk, (size_t)aligned_sz - got_tot);
                size_t got = std::fread(cell.data.data() + got_tot, 1, want, f);
                if (got == 0) break;
                got_tot += got;
                svcSleepThread(500000ULL); // 0.5 ms yield
            }
            if (got_tot >= sz) {
                cell.data.resize(got_tot);
                total_bytes += got_tot;
                loaded.push_back(std::move(cell));
            }
        }
        std::fclose(f);

        if (!stop_ && !loaded.empty()) {
            std::sort(loaded.begin(), loaded.end(), [](const Cell &a, const Cell &b) {
                return a.offset < b.offset;
            });
            cells_.swap(loaded);
            ready_.store(true, std::memory_order_release);
            double sec = (double)(armGetSystemTick() - t0) / 19200000.0;
            printf("[OlkSelectCache] Preloaded %zu character select cells (%.2f MB) in %.2fs\n",
                   cells_.size(), (double)total_bytes / (1024.0 * 1024.0), sec);
        }
    }

    std::mutex start_mtx_;
    std::atomic<bool> started_{false};
    std::atomic<bool> stop_{false};
    std::atomic<bool> ready_{false};
    std::thread worker_;
    std::vector<Cell> cells_;
};

OlkSelectCache g_select_cache;
std::string g_olk_path;
} // namespace

uint64_t SwitchFS_LastDiscIoTick(void) {
    return s_last_disc_io_tick.load(std::memory_order_acquire);
}

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
    if (res != (void *)(intptr_t)-1 && isDiscAssetRead(lpFileName, dwDesiredAccess)) {
        s_last_disc_io_tick.store(armGetSystemTick(), std::memory_order_release);
        if (isOlkRead(lpFileName, dwDesiredAccess)) {
            std::lock_guard<std::mutex> lk(g_olk_mtx);
            g_olk_handles.insert(res);
            if (g_olk_path.empty()) g_olk_path = lpFileName;
        }
    }
    return res;
}

int __wrap_CloseHandle(void *hObject) {
    {
        std::lock_guard<std::mutex> lk(g_olk_mtx);
        g_olk_handles.erase(hObject);
    }
    return __real_CloseHandle(hObject);
}

int __wrap_ReadFile(
    void *hFile, void *lpBuffer, uint32_t nNumberOfBytesToRead,
    uint32_t *lpNumberOfBytesRead, void *lpOverlapped)
{
    bool is_olk = false;
    std::string olk_path_copy;
    {
        std::lock_guard<std::mutex> lk(g_olk_mtx);
        is_olk = (g_olk_handles.find(hFile) != g_olk_handles.end());
        if (is_olk && sc2_scene != 0u && !g_olk_path.empty()) {
            olk_path_copy = g_olk_path;
        }
    }
    if (is_olk) {
        if (!olk_path_copy.empty()) {
            g_select_cache.maybeStart(olk_path_copy);
        }
        if (!lpOverlapped && g_select_cache.isReady() && hFile != nullptr && hFile != (void *)(intptr_t)-1) {
            int fd = *(const int *)hFile;
            off_t cur = lseek(fd, 0, SEEK_CUR);
            if (cur >= 0 && g_select_cache.tryRead((uint64_t)cur, lpBuffer, nNumberOfBytesToRead)) {
                lseek(fd, (off_t)(cur + (off_t)nNumberOfBytesToRead), SEEK_SET);
                if (lpNumberOfBytesRead) *lpNumberOfBytesRead = nNumberOfBytesToRead;
                return 1;
            }
        }
        s_last_disc_io_tick.store(armGetSystemTick(), std::memory_order_release);
    }

    uint64_t t0 = is_olk ? armGetSystemTick() : 0;
    int ok = __real_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    if (is_olk) {
        uint64_t dt = armGetSystemTick() - t0;
        uint32_t got = (ok && lpNumberOfBytesRead) ? *lpNumberOfBytesRead : 0u;
        s_olk_ticks.fetch_add(dt, std::memory_order_relaxed);
        s_olk_calls.fetch_add(1, std::memory_order_relaxed);
        s_olk_bytes.fetch_add((uint64_t)got, std::memory_order_relaxed);
        s_last_disc_io_tick.store(armGetSystemTick(), std::memory_order_release);
    }
    return ok;
}

void SwitchFS_TakeReadPerf(uint64_t *ticks, uint32_t *calls, uint64_t *bytes) {
    *ticks = s_olk_ticks.exchange(0, std::memory_order_acq_rel);
    *calls = s_olk_calls.exchange(0, std::memory_order_acq_rel);
    *bytes = s_olk_bytes.exchange(0, std::memory_order_acq_rel);
}
}
