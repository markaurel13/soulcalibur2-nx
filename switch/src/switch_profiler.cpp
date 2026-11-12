#include <switch.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <atomic>
#include "runtime.h"

extern "C" FILE* Switch_GetMainLogFile(void);

struct RecompD3dPresenterCommand;
struct RecompHrtfVoice;
typedef void (*RecompFunction)(void);

struct GuestPerfStats {
    uint64_t t_d3d8_submit = 0;
    uint64_t t_dsound_pump = 0;
    uint64_t t_dsound_hrtf = 0;
    uint64_t t_draw_adapter = 0;
    uint64_t t_vblank_wait = 0;
    uint64_t t_present_wait = 0;
    uint64_t t_recomp_lookup = 0;
    uint64_t t_x87_math = 0;
    uint32_t count_d3d8_submits = 0;
    uint32_t count_dsound_pumps = 0;
    uint32_t count_hrtf_renders = 0;
    uint32_t count_draw_calls = 0;
    uint32_t count_vblank_waits = 0;
    uint32_t count_present_waits = 0;
    uint32_t count_lookups = 0;
    uint32_t count_x87_sin = 0;
    uint32_t count_x87_cos = 0;
    uint32_t count_x87_atan2 = 0;
};

static GuestPerfStats s_prof;
static Mutex s_prof_mutex;

extern "C" {

// ---- Wrap recomp_d3d8_submit ----
void Switch_NotifyActivity(uint32_t frame);

void __real_recomp_d3d8_submit(const RecompD3dPresenterCommand *command);
void __wrap_recomp_d3d8_submit(const RecompD3dPresenterCommand *command) {
    uint64_t t0 = armGetSystemTick();
    __real_recomp_d3d8_submit(command);
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_d3d8_submit += dt;
    s_prof.count_d3d8_submits++;
    mutexUnlock(&s_prof_mutex);
}

// ---- Wrap recomp_dsound_buffers_pump ----
void __real_recomp_dsound_buffers_pump(void);
void __wrap_recomp_dsound_buffers_pump(void) {
    uint64_t t0 = armGetSystemTick();
    __real_recomp_dsound_buffers_pump();
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_dsound_pump += dt;
    s_prof.count_dsound_pumps++;
    mutexUnlock(&s_prof_mutex);
}

// ---- Wrap recomp_hrtf_render ----
uint32_t __real_recomp_hrtf_render(RecompHrtfVoice *voice, const int16_t *input,
                                   uint32_t input_frames, uint32_t source_rate,
                                   float *out, uint32_t out_frames);
uint32_t __wrap_recomp_hrtf_render(RecompHrtfVoice *voice, const int16_t *input,
                                   uint32_t input_frames, uint32_t source_rate,
                                   float *out, uint32_t out_frames) {
    uint64_t t0 = armGetSystemTick();
    uint32_t res = __real_recomp_hrtf_render(voice, input, input_frames, source_rate, out, out_frames);
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_dsound_hrtf += dt;
    s_prof.count_hrtf_renders++;
    mutexUnlock(&s_prof_mutex);
    return res;
}

// ---- Wrap recomp_d3d_wait_vblank ----
uint32_t recomp_kernel_current_thread_id(void);
extern uint32_t sc2_frame_thread;

void __real_recomp_d3d_wait_vblank(void);
void __wrap_recomp_d3d_wait_vblank(void) {
    uint32_t tid = recomp_kernel_current_thread_id();
    uint64_t t0 = armGetSystemTick();
    __real_recomp_d3d_wait_vblank();
    uint64_t dt = armGetSystemTick() - t0;
    // Solo contar el tiempo de espera si es el hilo de juego principal
    if (sc2_frame_thread == 0 || tid == sc2_frame_thread) {
        mutexLock(&s_prof_mutex);
        s_prof.t_vblank_wait += dt;
        s_prof.count_vblank_waits++;
        mutexUnlock(&s_prof_mutex);
    }
}

// ---- Wrap recomp_d3d_wait_present ----
void __real_recomp_d3d_wait_present(void);
void __wrap_recomp_d3d_wait_present(void) {
    uint64_t t0 = armGetSystemTick();
    __real_recomp_d3d_wait_present();
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_present_wait += dt;
    s_prof.count_present_waits++;
    mutexUnlock(&s_prof_mutex);
}

// ---- Wrap recomp_lookup ----
extern uint32_t recomp_last_dispatch_address;
RecompFunction __real_recomp_lookup(uint32_t xbox_va);
RecompFunction __wrap_recomp_lookup(uint32_t xbox_va) {
    recomp_last_dispatch_address = xbox_va;
    uint64_t t0 = armGetSystemTick();
    RecompFunction fn = __real_recomp_lookup(xbox_va);
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_recomp_lookup += dt;
    s_prof.count_lookups++;
    mutexUnlock(&s_prof_mutex);
    return fn;
}

// ---- Wrap x87 math functions ----
double __real_sc2_x87_sin(double x);
double __wrap_sc2_x87_sin(double x) {
    uint64_t t0 = armGetSystemTick();
    double res = __real_sc2_x87_sin(x);
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_x87_math += dt;
    s_prof.count_x87_sin++;
    mutexUnlock(&s_prof_mutex);
    return res;
}

double __real_sc2_x87_cos(double x);
double __wrap_sc2_x87_cos(double x) {
    uint64_t t0 = armGetSystemTick();
    double res = __real_sc2_x87_cos(x);
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_x87_math += dt;
    s_prof.count_x87_cos++;
    mutexUnlock(&s_prof_mutex);
    return res;
}

double __real_sc2_x87_atan2(double y, double x);
double __wrap_sc2_x87_atan2(double y, double x) {
    uint64_t t0 = armGetSystemTick();
    double res = __real_sc2_x87_atan2(y, x);
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_x87_math += dt;
    s_prof.count_x87_atan2++;
    mutexUnlock(&s_prof_mutex);
    return res;
}

// ---- Intercept D3D8 draw entry points ----
static RecompFunction s_real_draw_up = nullptr;
static RecompFunction s_real_draw_indexed = nullptr;

static void prof_draw_up(void) {
    uint64_t t0 = armGetSystemTick();
    s_real_draw_up();
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_draw_adapter += dt;
    s_prof.count_draw_calls++;
    mutexUnlock(&s_prof_mutex);
}

static void prof_draw_indexed(void) {
    uint64_t t0 = armGetSystemTick();
    s_real_draw_indexed();
    uint64_t dt = armGetSystemTick() - t0;
    mutexLock(&s_prof_mutex);
    s_prof.t_draw_adapter += dt;
    s_prof.count_draw_calls++;
    mutexUnlock(&s_prof_mutex);
}

RecompFunction __real_recomp_d3d8_draw_lookup_manual(uint32_t address);
RecompFunction __wrap_recomp_d3d8_draw_lookup_manual(uint32_t address) {
    RecompFunction fn = __real_recomp_d3d8_draw_lookup_manual(address);
    if (address == 0x00152be0u) { // draw_up
        s_real_draw_up = fn;
        return prof_draw_up;
    }
    if (address == 0x00152d50u) { // draw_indexed
        s_real_draw_indexed = fn;
        return prof_draw_indexed;
    }
    return fn;
}

// ---- Wrap recomp_memory, recomp_memory_u32_checked, and guest memory transfers ----
extern uint8_t *recomp_fast_ram;
uint8_t *__real_recomp_memory(uint32_t guest_address, size_t width);
uint32_t *__real_recomp_memory_u32_checked(uint32_t guest_address);
void __real_recomp_guest_load(void *destination, uint32_t source, size_t size);
void __real_recomp_guest_store(uint32_t destination, const void *source, size_t size);
void __real_recomp_guest_memcpy(uint32_t destination, uint32_t source, size_t size);
void __real_recomp_guest_memset(uint32_t destination, int value, size_t size);

static inline bool sc2_is_guest_ram(uint32_t guest_address, size_t width) {
    // Exclude device MMIO (USB OHCI 0xfd000000..0xfdffffff and APU 0xfe800000..0xfeffffff)
    if (guest_address >= 0xfd000000u && guest_address < 0xff000000u) {
        return false;
    }
    uint32_t offset = guest_address & 0x03ffffffu;
    return (offset + width) <= 0x04000000u;
}

static inline uint32_t sc2_normalize_guest_address(uint32_t guest_address) {
    if (guest_address >= 0xfd000000u && guest_address < 0xff000000u) {
        return guest_address;
    }
    return 0x80000000u | (guest_address & 0x03ffffffu);
}

uint8_t *__wrap_recomp_memory(uint32_t guest_address, size_t width) {
    if (sc2_is_guest_ram(guest_address, width)) {
        uint32_t offset = guest_address & 0x03ffffffu;
        if (recomp_fast_ram != nullptr) {
            return recomp_fast_ram + offset;
        }
        guest_address = 0x80000000u | offset;
    }
    return __real_recomp_memory(guest_address, width);
}

uint32_t *__wrap_recomp_memory_u32_checked(uint32_t guest_address) {
    if (sc2_is_guest_ram(guest_address, 4u)) {
        uint32_t offset = guest_address & 0x03ffffffu;
        if (recomp_fast_ram != nullptr) {
            return (uint32_t *)(void *)(recomp_fast_ram + offset);
        }
        guest_address = 0x80000000u | offset;
    }
    return __real_recomp_memory_u32_checked(guest_address);
}

void __wrap_recomp_guest_load(void *destination, uint32_t source, size_t size) {
    if (size == 0) return;
    if (sc2_is_guest_ram(source, size)) {
        uint32_t offset = source & 0x03ffffffu;
        if (recomp_fast_ram != nullptr) {
            memcpy(destination, recomp_fast_ram + offset, size);
            return;
        }
        source = 0x80000000u | offset;
    }
    __real_recomp_guest_load(destination, source, size);
}

void __wrap_recomp_guest_store(uint32_t destination, const void *source, size_t size) {
    if (size == 0) return;
    if (sc2_is_guest_ram(destination, size)) {
        uint32_t offset = destination & 0x03ffffffu;
        if (recomp_fast_ram != nullptr) {
            memcpy(recomp_fast_ram + offset, source, size);
            return;
        }
        destination = 0x80000000u | offset;
    }
    __real_recomp_guest_store(destination, source, size);
}

void __wrap_recomp_guest_memcpy(uint32_t destination, uint32_t source, size_t size) {
    if (size == 0) return;
    destination = sc2_normalize_guest_address(destination);
    source = sc2_normalize_guest_address(source);
    __real_recomp_guest_memcpy(destination, source, size);
}

void __wrap_recomp_guest_memset(uint32_t destination, int value, size_t size) {
    if (size == 0) return;
    destination = sc2_normalize_guest_address(destination);
    __real_recomp_guest_memset(destination, value, size);
}



// Forward declarations
void recomp_kernel_thread_times(char *out, size_t size);
size_t recomp_dispatch_flat_bytes(void);

// ---- Statistics query ----
void Switch_TakeGuestPerf(uint64_t *d3d8_submit, uint64_t *dsound_pump,
                          uint64_t *dsound_hrtf, uint32_t *submits,
                          uint32_t *pumps, uint32_t *hrtfs,
                          uint64_t *draw_adapter, uint32_t *draw_calls,
                          uint64_t *vblank_wait, uint32_t *vblank_waits,
                          uint64_t *present_wait, uint32_t *present_waits,
                          uint64_t *recomp_lookup_time, uint32_t *recomp_lookups,
                          uint64_t *x87_math_time, uint32_t *x87_sin_cnt,
                          uint32_t *x87_cos_cnt, uint32_t *x87_atan2_cnt,
                          size_t *flat_table_bytes,
                          char *thread_times_buf, size_t thread_times_size) {
    mutexLock(&s_prof_mutex);
    if (d3d8_submit) *d3d8_submit = s_prof.t_d3d8_submit;
    if (dsound_pump) *dsound_pump = s_prof.t_dsound_pump;
    if (dsound_hrtf) *dsound_hrtf = s_prof.t_dsound_hrtf;
    if (submits) *submits = s_prof.count_d3d8_submits;
    if (pumps) *pumps = s_prof.count_dsound_pumps;
    if (hrtfs) *hrtfs = s_prof.count_hrtf_renders;

    if (draw_adapter) *draw_adapter = s_prof.t_draw_adapter;
    if (draw_calls) *draw_calls = s_prof.count_draw_calls;
    if (vblank_wait) *vblank_wait = s_prof.t_vblank_wait;
    if (vblank_waits) *vblank_waits = s_prof.count_vblank_waits;
    if (present_wait) *present_wait = s_prof.t_present_wait;
    if (present_waits) *present_waits = s_prof.count_present_waits;

    if (recomp_lookup_time) *recomp_lookup_time = s_prof.t_recomp_lookup;
    if (recomp_lookups) *recomp_lookups = s_prof.count_lookups;
    if (x87_math_time) *x87_math_time = s_prof.t_x87_math;
    if (x87_sin_cnt) *x87_sin_cnt = s_prof.count_x87_sin;
    if (x87_cos_cnt) *x87_cos_cnt = s_prof.count_x87_cos;
    if (x87_atan2_cnt) *x87_atan2_cnt = s_prof.count_x87_atan2;

    s_prof = GuestPerfStats{};
    mutexUnlock(&s_prof_mutex);

    if (flat_table_bytes) {
        *flat_table_bytes = recomp_dispatch_flat_bytes();
    }

    if (thread_times_buf && thread_times_size > 0) {
        recomp_kernel_thread_times(thread_times_buf, thread_times_size);
    }
}

// ---- Diagnostic Hang Watchdog ----
extern "C" {
FILE* Switch_GetMainLogFile(void);
void Switch_GetPresenterThreadState(int *pending, int *front, int *open, int *status);
void Switch_GetDeko3DState(int *current_slot, int *cur_cmd_slot, uint32_t *frame_counter);
extern uint32_t sc2_scene;
extern uint32_t sc2_scene_switches;
}

static Thread s_watchdog_thread;
alignas(0x1000) static u8 s_watchdog_stack[0x8000];
static std::atomic<bool> s_watchdog_running{false};
static std::atomic<uint64_t> s_last_activity_tick{0};
static std::atomic<uint32_t> s_activity_frame{0};

void Switch_NotifyActivity(uint32_t frame) {
    s_last_activity_tick.store(armGetSystemTick(), std::memory_order_relaxed);
    if (frame != 0) s_activity_frame.store(frame, std::memory_order_relaxed);
}

static void watchdog_worker(void *arg) {
    (void)arg;
    uint32_t stall_count = 0;
    uint32_t heartbeat_sec = 0;
    FILE *stall_file = nullptr;

    while (s_watchdog_running.load(std::memory_order_relaxed)) {
        svcSleepThread(1000000000ULL); // 1 sec
        heartbeat_sec++;

        uint64_t last = s_last_activity_tick.load(std::memory_order_relaxed);
        if (last == 0) continue;
        uint64_t now = armGetSystemTick();
        uint64_t diff_ms = (now - last) / 19200ULL;

        if (diff_ms >= 2000ULL) {
            stall_count++;

            // Sample EIP 5 times with 5ms delays to determine if CPU is in a loop or completely halted
            uint32_t eip_samples[5];
            for (int i = 0; i < 5; ++i) {
                eip_samples[i] = recomp_last_dispatch_address;
                if (i < 4) svcSleepThread(5000000ULL); // 5ms
            }

            uint32_t cur_fr = s_activity_frame.load(std::memory_order_relaxed);
            uint32_t scene_req = sc2_scene;
            uint32_t scene_cur = 0;
            uint32_t scene_next = 0;
            if (recomp_fast_ram) {
                scene_cur = *(const uint32_t*)(recomp_fast_ram + (0x00398700u & 0x03ffffff));
                scene_next = *(const uint32_t*)(recomp_fast_ram + (0x00398704u & 0x03ffffff));
            }

            int p_pending = 0, p_front = 0, p_open = 0, p_status = 0;
            Switch_GetPresenterThreadState(&p_pending, &p_front, &p_open, &p_status);

            int d_curr = 0, d_cmd = 0;
            uint32_t d_frames = 0;
            Switch_GetDeko3DState(&d_curr, &d_cmd, &d_frames);

            const RecompRegisters &regs = recomp_runtime.registers;
            uint32_t stack_peek[8] = {0};
            if (recomp_fast_ram && regs.esp >= 0x00010000u && regs.esp < 0x04000000u) {
                const uint32_t *sp = (const uint32_t*)(recomp_fast_ram + (regs.esp & 0x03ffffff));
                for (int k = 0; k < 8; ++k) stack_peek[k] = sp[k];
            }

            char stall_buf[1024];
            int len = snprintf(stall_buf, sizeof(stall_buf),
                "\n=======================================================\n"
                "[WATCHDOG STALL #%u] %llu ms without display present!\n"
                "  Frame: PresenterFrame=%u DekoFrame=%u ActivityFrame=%u\n"
                "  Scene: Current=%u Target=%u Requested=%u Switches=%u\n"
                "  PresenterThread: pending=%d front=%d open=%d status=%d\n"
                "  Deko3D: current_slot=%d cur_cmd_slot=%d\n"
                "  Guest x86 EIP Samples (5ms gap): [0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x] (%s)\n"
                "  Guest Regs: EAX=0x%08x EBX=0x%08x ECX=0x%08x EDX=0x%08x\n"
                "              ESI=0x%08x EDI=0x%08x EBP=0x%08x ESP=0x%08x\n"
                "  Guest Stack [ESP..ESP+28]: [0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x]\n"
                "=======================================================\n",
                stall_count, (unsigned long long)diff_ms,
                cur_fr, d_frames, cur_fr,
                scene_cur, scene_next, scene_req, sc2_scene_switches,
                p_pending, p_front, p_open, p_status,
                d_curr, d_cmd,
                eip_samples[0], eip_samples[1], eip_samples[2], eip_samples[3], eip_samples[4],
                (eip_samples[0] == eip_samples[4]) ? "STATIC / HALTED" : "LOOPING / ACTIVE",
                regs.eax, regs.ebx, regs.ecx, regs.edx,
                regs.esi, regs.edi, regs.ebp, regs.esp,
                stack_peek[0], stack_peek[1], stack_peek[2], stack_peek[3],
                stack_peek[4], stack_peek[5], stack_peek[6], stack_peek[7]);

            // 1. Direct write to dedicated stall.log
            if (!stall_file) {
                stall_file = fopen("sdmc:/switch/soulcalibur2/stall.log", "a");
            }
            if (stall_file) {
                fwrite(stall_buf, 1, len, stall_file);
                fflush(stall_file);
                fsync(fileno(stall_file));
            }

            // 2. Also write to main soulcalibur2.log if open
            FILE *main_log = Switch_GetMainLogFile();
            if (main_log) {
                fwrite(stall_buf, 1, len, main_log);
                fflush(main_log);
                fsync(fileno(main_log));
            }
        } else {
            stall_count = 0;
            // Write a periodic heartbeat every 5 seconds so we know the system is alive
            if (heartbeat_sec % 5 == 0) {
                FILE *main_log = Switch_GetMainLogFile();
                if (main_log) {
                    uint32_t cur_fr = s_activity_frame.load(std::memory_order_relaxed);
                    uint32_t scene_req = sc2_scene;
                    fprintf(main_log, "[Watchdog Heartbeat] Alive at %llu ms | Frame=%u Scene=%u LastPresent=%llums ago\n",
                            (unsigned long long)(now / 19200ULL), cur_fr, scene_req, (unsigned long long)diff_ms);
                    fflush(main_log);
                    fsync(fileno(main_log));
                }
            }
        }
    }

    if (stall_file) {
        fclose(stall_file);
    }
}


// ---- Muestreador de funciones guest (self time) ----
volatile uint32_t sc2_prof_cur = 0;

#define SC2_PROF_SLOTS 0x60000u
static uint32_t s_prof_hist[SC2_PROF_SLOTS];
static Thread s_sampler_thread;
alignas(0x1000) static char s_sampler_stack[0x10000];
static std::atomic<bool> s_sampler_running{false};

static void sampler_worker(void *arg) {
    (void)arg;
    uint64_t window_start = armGetSystemTick();
    uint32_t total = 0, host = 0;
    while (s_sampler_running.load(std::memory_order_relaxed)) {
        svcSleepThread(150000ULL); // ~150us
        uint32_t a = sc2_prof_cur;
        total++;
        if (a == 0 || (a >> 2) >= SC2_PROF_SLOTS) { host++; continue; }
        s_prof_hist[a >> 2]++;
        uint64_t now = armGetSystemTick();
        if ((now - window_start) / 19200ULL >= 10000ULL) {
            FILE *f = Switch_GetMainLogFile();
            if (f) {
                fprintf(f, "[FuncProf] window=10s samples=%u host(no guest func)=%u\n", total, host);
                for (int rank = 0; rank < 25; ++rank) {
                    uint32_t best = 0, best_i = 0;
                    for (uint32_t i = 0; i < SC2_PROF_SLOTS; ++i) {
                        if (s_prof_hist[i] > best) { best = s_prof_hist[i]; best_i = i; }
                    }
                    if (best == 0) break;
                    fprintf(f, "[FuncProf] #%d sub_%08X self=%.2f%% (%u)\n", rank, best_i << 2,
                            100.0 * best / (double)total, best);
                    s_prof_hist[best_i] = 0;
                }
                fflush(f);
            }
            memset(s_prof_hist, 0, sizeof(s_prof_hist));
            total = 0; host = 0;
            window_start = now;
        }
    }
}

void Switch_StartWatchdog(void) {
    s_last_activity_tick.store(armGetSystemTick(), std::memory_order_relaxed);
    s_watchdog_running.store(true, std::memory_order_relaxed);
    Result rc = threadCreate(&s_watchdog_thread, watchdog_worker, NULL, s_watchdog_stack, sizeof(s_watchdog_stack), 0x20, 2);
    if (R_SUCCEEDED(rc)) {
        threadStart(&s_watchdog_thread);
        printf("[Main] [+] Diagnostic Watchdog Thread started on Core 2 (Prio 0x20).\n");
#ifdef SC2_FUNC_PROFILE
        s_sampler_running.store(true, std::memory_order_relaxed);
        if (R_SUCCEEDED(threadCreate(&s_sampler_thread, sampler_worker, NULL, s_sampler_stack, sizeof(s_sampler_stack), 0x2C, 2))) {
            threadStart(&s_sampler_thread);
        }
#endif
    } else {
        printf("[Main] [!] Failed to create watchdog thread: 0x%x\n", rc);
    }
}

void Switch_StopWatchdog(void) {
    if (s_sampler_running.exchange(false)) {
        threadWaitForExit(&s_sampler_thread);
        threadClose(&s_sampler_thread);
    }
    if (s_watchdog_running.exchange(false)) {
        threadWaitForExit(&s_watchdog_thread);
        threadClose(&s_watchdog_thread);
    }
}

} // extern "C"
