#include "switch_core_manager.h"

#include <switch.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>
#include <sys/stat.h>

namespace {

#include "forwarder_npdm.inc"

std::atomic<int> g_sys_state{0};       // 0 = uninitialized, 1 = initialized
std::atomic<unsigned> g_process_mask{0x7};
std::atomic<int> g_system_core{-1};    // 3 when 4 cores granted, else -1
std::atomic<unsigned> g_strip_mask{0}; // bitmask stripped from multi-core masks (0x8 when 4 cores)
std::mutex g_sys_mutex;

void ReadSystemCore(void) {
    u64 mask = 0;
    if (R_FAILED(svcGetInfo(&mask, InfoType_CoreMask, CUR_PROCESS_HANDLE, 0)) || mask == 0) {
        mask = 0x7;
    }
    mask &= 0xF;
    const int top = (__builtin_popcountll(mask) >= 4) ? (63 - __builtin_clzll(mask)) : -1;
    g_process_mask.store(static_cast<unsigned>(mask), std::memory_order_relaxed);
    g_system_core.store(top, std::memory_order_relaxed);
    g_strip_mask.store(top >= 0 ? (1u << top) : 0u, std::memory_order_relaxed);
    g_sys_state.store(1, std::memory_order_release);
}

inline void EnsureSystemCore(void) {
    if (g_sys_state.load(std::memory_order_acquire) == 0) {
        std::lock_guard<std::mutex> lock(g_sys_mutex);
        if (g_sys_state.load(std::memory_order_relaxed) == 0) {
            ReadSystemCore();
        }
    }
}

void ComputeSphairaIds(const std::string& path, u64& out_hb, u64& out_ng) {
    Sha256Context ctx;
    u8 hash[SHA256_HASH_SIZE]{};
    sha256ContextCreate(&ctx);
    sha256ContextUpdate(&ctx, path.data(), path.size());
    sha256ContextUpdate(&ctx, path.data(), path.size());
    sha256ContextGetHash(&ctx, hash);

    u64 v = 0;
    std::memcpy(&v, hash, sizeof(v));
    out_hb = (v & 0x00FFFFFFFFFFF000ULL) | 0x0500000000000000ULL;
    out_ng = (v & 0x00FFFFFFFFFFE000ULL) | 0x0100000000000000ULL;
}

bool MatchesSphairaForwarder(u64 program_id, const char* argv0) {
    std::vector<std::string> paths;
    if (argv0 && argv0[0] != '\0') {
        std::string p(argv0);
        paths.push_back(p);
        if (p.rfind("sdmc:", 0) == 0) {
            paths.push_back(p.substr(5));
        } else if (p[0] == '/') {
            paths.push_back(std::string("sdmc:") + p);
        }
    } else {
        paths.push_back("sdmc:/switch/soulcalibur2/soulcalibur2.nro");
        paths.push_back("/switch/soulcalibur2/soulcalibur2.nro");
    }

    for (const auto& p : paths) {
        u64 hb = 0, ng = 0;
        ComputeSphairaIds(p, hb, ng);
        if (program_id == hb || program_id == ng) {
            return true;
        }
    }
    return false;
}

const char* EnsureForwarderNpdm(u64 program_id, const char* argv0, char* status_buf, size_t status_len) {
    if (program_id == 0) {
        std::snprintf(status_buf, status_len, "skipped (ProgramId=0)");
        return status_buf;
    }

    if (!MatchesSphairaForwarder(program_id, argv0)) {
        std::snprintf(status_buf, status_len,
                      "not written (running as %016llX, not a matching Sphaira forwarder for '%s')",
                      static_cast<unsigned long long>(program_id),
                      argv0 ? argv0 : "<null>");
        return status_buf;
    }

    unsigned char npdm[sizeof(kForwarderNpdm)];
    std::memcpy(npdm, kForwarderNpdm, sizeof(npdm));
    std::memcpy(npdm + kForwarderNpdmAci0 + 0x10, &program_id, 8);
    std::memcpy(npdm + kForwarderNpdmAcid + 0x210, &program_id, 8);
    std::memcpy(npdm + kForwarderNpdmAcid + 0x218, &program_id, 8);

    char dir[96];
    char path[128];
    std::snprintf(dir, sizeof(dir), "sdmc:/atmosphere/contents/%016llX/exefs",
                  static_cast<unsigned long long>(program_id));
    std::snprintf(path, sizeof(path), "%s/main.npdm", dir);

    if (FILE* f = std::fopen(path, "rb")) {
        unsigned char have[sizeof(npdm) + 1];
        const size_t n = std::fread(have, 1, sizeof(have), f);
        std::fclose(f);
        if (n == sizeof(npdm) && std::memcmp(have, npdm, sizeof(npdm)) == 0) {
            std::snprintf(status_buf, status_len, "in place: %s", path);
            return status_buf;
        }
    }

    mkdir("sdmc:/atmosphere", 0777);
    mkdir("sdmc:/atmosphere/contents", 0777);
    char part[96];
    std::snprintf(part, sizeof(part), "sdmc:/atmosphere/contents/%016llX",
                  static_cast<unsigned long long>(program_id));
    mkdir(part, 0777);
    mkdir(dir, 0777);

    FILE* f = std::fopen(path, "wb");
    const bool ok = f && std::fwrite(npdm, 1, sizeof(npdm), f) == sizeof(npdm);
    if (f) {
        std::fclose(f);
    }

    std::snprintf(status_buf, status_len, "%s: %s",
                  ok ? "written (relaunch forwarder for 4 cores)" : "FAILED to write",
                  path);
    return status_buf;
}

} // namespace

extern "C" {

Result __real_svcSetThreadCoreMask(Handle handle, s32 preferred_core, u32 affinity_mask);

Result __wrap_svcSetThreadCoreMask(Handle handle, s32 preferred_core, u32 affinity_mask) {
    EnsureSystemCore();
    const u32 strip = g_strip_mask.load(std::memory_order_relaxed);
    if (strip != 0 && (affinity_mask & strip) != 0 && (affinity_mask & ~strip) != 0) {
        affinity_mask &= ~strip;
        if (preferred_core >= 0 && (strip & (1u << preferred_core)) != 0) {
            preferred_core = -1;
        }
    }
    return __real_svcSetThreadCoreMask(handle, preferred_core, affinity_mask);
}

void SwitchCore_Init(const char* argv0) {
    {
        std::lock_guard<std::mutex> lock(g_sys_mutex);
        ReadSystemCore();
    }

    const u32 strip = g_strip_mask.load(std::memory_order_relaxed);
    if (strip != 0) {
        s32 preferred = 0;
        u64 mask = 0;
        if (R_SUCCEEDED(svcGetThreadCoreMask(&preferred, &mask, CUR_THREAD_HANDLE)) &&
            (mask & strip) != 0 && (mask & ~static_cast<u64>(strip)) != 0) {
            svcSetThreadCoreMask(CUR_THREAD_HANDLE, -1, static_cast<u32>(mask));
        }
    }

    u64 program_id = 0;
    svcGetInfo(&program_id, InfoType_ProgramId, CUR_PROCESS_HANDLE, 0);

    char npdm_status[256];
    EnsureForwarderNpdm(program_id, argv0, npdm_status, sizeof(npdm_status));

    const unsigned pmask = g_process_mask.load(std::memory_order_relaxed);
    const int sys_core = g_system_core.load(std::memory_order_relaxed);
    const unsigned app_mask = pmask & ~g_strip_mask.load(std::memory_order_relaxed);

    printf("[SwitchCore] process_cores=0x%X, system_core=%d, app_cores=0x%X, program_id=%016llX, npdm=%s\n",
           pmask, sys_core, app_mask, static_cast<unsigned long long>(program_id), npdm_status);
}

int SwitchCore_GetSystemCore(void) {
    EnsureSystemCore();
    return g_system_core.load(std::memory_order_relaxed);
}

unsigned SwitchCore_GetProcessCoreMask(void) {
    EnsureSystemCore();
    return g_process_mask.load(std::memory_order_relaxed);
}

bool SwitchCore_MoveCurrentThreadToSystemCore(int priority) {
    const int core = SwitchCore_GetSystemCore();
    if (core < 0) return false;
    if (R_FAILED(svcSetThreadCoreMask(CUR_THREAD_HANDLE, core, 1u << core))) {
        return false;
    }
    if (priority >= 0) {
        svcSetThreadPriority(CUR_THREAD_HANDLE, priority);
    }
    return true;
}

} // extern "C"
