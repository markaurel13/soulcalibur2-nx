#include <switch.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <atomic>
#include <time.h>
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
    uint64_t t_shadow_push = 0;
    uint64_t t_vblank_wait = 0;
    uint64_t t_present_wait = 0;
    uint64_t t_recomp_lookup = 0;
    uint64_t t_x87_math = 0;
    uint32_t count_d3d8_submits = 0;
    uint32_t count_dsound_pumps = 0;
    uint32_t count_hrtf_renders = 0;
    uint32_t count_draw_calls = 0;
    uint32_t count_shadow_draws = 0;
    uint32_t count_vblank_waits = 0;
    uint32_t count_present_waits = 0;
    uint32_t count_lookups = 0;
    uint32_t count_x87_sin = 0;
    uint32_t count_x87_cos = 0;
    uint32_t count_x87_atan2 = 0;
};

static GuestPerfStats s_prof;
static GuestPerfStats s_prof_core0; // Core 0 local accumulation (Zero-Mutex contention)
static Mutex s_prof_mutex;

extern "C" {

// ---- Wrap recomp_d3d8_submit ----
void Switch_NotifyActivity(uint32_t frame);

void __real_recomp_d3d8_submit(const RecompD3dPresenterCommand *command);
void __wrap_recomp_d3d8_submit(const RecompD3dPresenterCommand *command) {
    uint64_t t0 = armGetSystemTick();
    __real_recomp_d3d8_submit(command);
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_d3d8_submit += dt;
    s_prof_core0.count_d3d8_submits++;
}

// ---- Wrap recomp_dsound_buffers_pump ----
void __real_recomp_dsound_buffers_pump(void);
void __wrap_recomp_dsound_buffers_pump(void) {
    uint64_t t0 = armGetSystemTick();
    __real_recomp_dsound_buffers_pump();
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_dsound_pump += dt;
    s_prof_core0.count_dsound_pumps++;
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
    s_prof_core0.t_dsound_hrtf += dt;
    s_prof_core0.count_hrtf_renders++;
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
        s_prof_core0.t_vblank_wait += dt;
        s_prof_core0.count_vblank_waits++;
    }
}


// ---- Fase 0 (telemetria): coste de recomp_lookup_manual / dispatch indirecto ----
// Cada RECOMP_ABI_CALL del codigo generado llama a recomp_lookup_manual; un resultado NULL
// (funcion de juego normal) NO se cachea y recorre toda la cadena de 8 tablas.
// Se publica un pseudo-"sub_" para que FuncProf separe este coste del de las funciones llamantes.
#include "sc2_prof.h"
static uint32_t s_lk_calls = 0, s_lk_null = 0, s_ind_calls = 0;
RecompFunction __real_recomp_lookup_manual(uint32_t guest_address);
// Fase 1: cache negativo. Las 8 tablas de la cadena son estaticas (switch con constantes), salvo
// free_camera (VIEW_BUILD/VIEW_BUILD_LEVEL, dependiente de enabled()): esas dos direcciones jamas se marcan nulas.
// Bitmap de 256 KB sobre los primeros 2 MB de codigo guest.
static uint8_t s_lk_neg[0x200000u / 8u];
RecompFunction __wrap_recomp_lookup_manual(uint32_t guest_address) {
    if (guest_address < 0x200000u && (s_lk_neg[guest_address >> 3] & (1u << (guest_address & 7u)))) {
        return NULL;
    }
#ifdef SC2_FUNC_PROFILE
    uint32_t prev = sc2_prof_cur;
    sc2_prof_cur = 0x0017F000u;
    RecompFunction fn = __real_recomp_lookup_manual(guest_address);
    sc2_prof_cur = prev;
#else
    RecompFunction fn = __real_recomp_lookup_manual(guest_address);
#endif
    if (fn == NULL && guest_address < 0x200000u &&
        guest_address != 0x0001CC50u && guest_address != 0x0001CCB0u)
        s_lk_neg[guest_address >> 3] |= (uint8_t)(1u << (guest_address & 7u));
    return fn;
}
void __real_recomp_dispatch_indirect_site(uint32_t a, uint32_t e, const char *m, int l);
void __wrap_recomp_dispatch_indirect_site(uint32_t a, uint32_t e, const char *m, int l) {
    ++s_ind_calls;
    __real_recomp_dispatch_indirect_site(a, e, m, l);
}
static void sc2_phase0_report(void);

// ---- PreDraw callee profiler (inclusive ticks via cntpct_el0) ----
#define SC2_PREDRAW_PROF_LIST(X) \
    X(0004B630) X(0004D090) X(00069370) \
    X(0003EE30) X(000343A0) X(0003D9A0) X(0003D9F0) \
    X(00046EA0) X(000423B0) X(0003FCF0) X(00042A80) \
    X(00048780) X(00048C00) X(00041120) X(00041550) \
    X(000496E0) X(00040670)

struct PreDrawProfEntry {
    const char *name;
    uint64_t ticks;
    uint32_t calls;
};

#define SC2_PD_DECL(fn) \
    static PreDrawProfEntry s_pd_##fn = { "sub_" #fn, 0, 0 }; \
    void __real_sub_##fn(void); \
    void __wrap_sub_##fn(void) { \
        uint64_t t0 = armGetSystemTick(); \
        __real_sub_##fn(); \
        s_pd_##fn.ticks += (armGetSystemTick() - t0); \
        s_pd_##fn.calls++; \
    }
SC2_PREDRAW_PROF_LIST(SC2_PD_DECL)
#undef SC2_PD_DECL

static PreDrawProfEntry* const s_pd_entries[] = {
#define SC2_PD_PTR(fn) &s_pd_##fn,
SC2_PREDRAW_PROF_LIST(SC2_PD_PTR)
#undef SC2_PD_PTR
};

static void sc2_phase0_report(void) {
    static uint32_t frames = 0;
    if (++frames < 120u) return;
    FILE *f = Switch_GetMainLogFile();
    if (f) {
        double ticks_per_ms = (double)armGetSystemTickFreq() / 1000.0;
        fprintf(f, "[Phase0] per-frame: lookup_manual calls=%u (negcache-hits(NULL)=%u) indirect_calls=%u\n",
                s_lk_calls / 120u, s_lk_null / 120u, s_ind_calls / 120u);
        fprintf(f, "[ProfPreDraw] (120f avg):");
        for (size_t i = 0; i < sizeof(s_pd_entries) / sizeof(s_pd_entries[0]); ++i) {
            PreDrawProfEntry *e = s_pd_entries[i];
            double ms = (double)e->ticks / (ticks_per_ms * 120.0);
            if (e->calls > 0) {
                fprintf(f, " %s=%.2fms(%u/f)", e->name + 4, ms, e->calls / 120u);
            }
            e->ticks = 0;
            e->calls = 0;
        }
        fprintf(f, "\n");
        fflush(f);
    } else {
        for (size_t i = 0; i < sizeof(s_pd_entries) / sizeof(s_pd_entries[0]); ++i) {
            s_pd_entries[i]->ticks = 0;
            s_pd_entries[i]->calls = 0;
        }
    }
    frames = 0; s_lk_calls = s_lk_null = s_ind_calls = 0;
}

// ---- Wrap recomp_d3d_wait_present ----
void __real_recomp_d3d_wait_present(void);
void __wrap_recomp_d3d_wait_present(void) {
    uint64_t t0 = armGetSystemTick();
    __real_recomp_d3d_wait_present();
    sc2_phase0_report();
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_present_wait += dt;
    s_prof_core0.count_present_waits++;
}

// ---- Wrap recomp_lookup ----
extern uint32_t recomp_last_dispatch_address;
RecompFunction __real_recomp_lookup(uint32_t xbox_va);
RecompFunction __wrap_recomp_lookup(uint32_t xbox_va) {
    recomp_last_dispatch_address = xbox_va;
    return __real_recomp_lookup(xbox_va);
}

// ---- Wrap x87 math functions ----
double __real_sc2_x87_sin(double x);
double __wrap_sc2_x87_sin(double x) {
    uint64_t t0 = armGetSystemTick();
    double res = __real_sc2_x87_sin(x);
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_x87_math += dt;
    s_prof_core0.count_x87_sin++;
    return res;
}

double __real_sc2_x87_cos(double x);
double __wrap_sc2_x87_cos(double x) {
    uint64_t t0 = armGetSystemTick();
    double res = __real_sc2_x87_cos(x);
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_x87_math += dt;
    s_prof_core0.count_x87_cos++;
    return res;
}

double __real_sc2_x87_atan2(double y, double x);
double __wrap_sc2_x87_atan2(double y, double x) {
    uint64_t t0 = armGetSystemTick();
    double res = __real_sc2_x87_atan2(y, x);
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_x87_math += dt;
    s_prof_core0.count_x87_atan2++;
    return res;
}

// ---- Intercept D3D8 draw entry points ----
static RecompFunction s_real_draw_up = nullptr;
static RecompFunction s_real_draw_indexed = nullptr;
static RecompFunction s_real_end_push = nullptr;

static void prof_draw_up(void) {
    uint64_t t0 = armGetSystemTick();
    s_real_draw_up();
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_draw_adapter += dt;
    s_prof_core0.count_draw_calls++;
}

static void prof_draw_indexed(void) {
    uint64_t t0 = armGetSystemTick();
    s_real_draw_indexed();
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_draw_adapter += dt;
    s_prof_core0.count_draw_calls++;
}

static void prof_end_push(void) {
    uint64_t t0 = armGetSystemTick();
    s_real_end_push();
    uint64_t dt = armGetSystemTick() - t0;
    s_prof_core0.t_shadow_push += dt;
    s_prof_core0.count_shadow_draws++;
}

RecompFunction __real_recomp_d3d8_draw_lookup_manual(uint32_t address);
RecompFunction __wrap_recomp_d3d8_draw_lookup_manual(uint32_t address) {
    RecompFunction fn = __real_recomp_d3d8_draw_lookup_manual(address);
    if (address == 0x00152560u) { // end_push (shadow stencil volume draws)
        s_real_end_push = fn;
        return prof_end_push;
    }
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

// Guest-memory wrappers moved to switch_fast_mem_wrap.c (LTO-enabled for inlining).



// Forward declarations
void recomp_kernel_thread_times(char *out, size_t size);
size_t recomp_dispatch_flat_bytes(void);

// ---- Statistics query ----
void Switch_TakeGuestPerf(uint64_t *d3d8_submit, uint64_t *dsound_pump,
                          uint64_t *dsound_hrtf, uint32_t *submits,
                          uint32_t *pumps, uint32_t *hrtfs,
                          uint64_t *draw_adapter, uint32_t *draw_calls,
                          uint64_t *shadow_push, uint32_t *shadow_draws,
                          uint64_t *vblank_wait, uint32_t *vblank_waits,
                          uint64_t *present_wait, uint32_t *present_waits,
                          uint64_t *recomp_lookup_time, uint32_t *recomp_lookups,
                          uint64_t *x87_math_time, uint32_t *x87_sin_cnt,
                          uint32_t *x87_cos_cnt, uint32_t *x87_atan2_cnt,
                          size_t *flat_table_bytes,
                          char *thread_times_buf, size_t thread_times_size) {
    mutexLock(&s_prof_mutex);
    // Flush Core 0 local accumulation
    s_prof.t_d3d8_submit += s_prof_core0.t_d3d8_submit;
    s_prof.t_dsound_pump += s_prof_core0.t_dsound_pump;
    s_prof.t_dsound_hrtf += s_prof_core0.t_dsound_hrtf;
    s_prof.t_draw_adapter += s_prof_core0.t_draw_adapter;
    s_prof.t_shadow_push += s_prof_core0.t_shadow_push;
    s_prof.t_vblank_wait += s_prof_core0.t_vblank_wait;
    s_prof.t_present_wait += s_prof_core0.t_present_wait;
    s_prof.t_recomp_lookup += s_prof_core0.t_recomp_lookup;
    s_prof.t_x87_math += s_prof_core0.t_x87_math;
    s_prof.count_d3d8_submits += s_prof_core0.count_d3d8_submits;
    s_prof.count_dsound_pumps += s_prof_core0.count_dsound_pumps;
    s_prof.count_hrtf_renders += s_prof_core0.count_hrtf_renders;
    s_prof.count_draw_calls += s_prof_core0.count_draw_calls;
    s_prof.count_shadow_draws += s_prof_core0.count_shadow_draws;
    s_prof.count_vblank_waits += s_prof_core0.count_vblank_waits;
    s_prof.count_present_waits += s_prof_core0.count_present_waits;
    s_prof.count_lookups += s_prof_core0.count_lookups;
    s_prof.count_x87_sin += s_prof_core0.count_x87_sin;
    s_prof.count_x87_cos += s_prof_core0.count_x87_cos;
    s_prof.count_x87_atan2 += s_prof_core0.count_x87_atan2;
    s_prof_core0 = GuestPerfStats{};

    if (d3d8_submit) *d3d8_submit = s_prof.t_d3d8_submit;
    if (dsound_pump) *dsound_pump = s_prof.t_dsound_pump;
    if (dsound_hrtf) *dsound_hrtf = s_prof.t_dsound_hrtf;
    if (submits) *submits = s_prof.count_d3d8_submits;
    if (pumps) *pumps = s_prof.count_dsound_pumps;
    if (hrtfs) *hrtfs = s_prof.count_hrtf_renders;

    if (draw_adapter) *draw_adapter = s_prof.t_draw_adapter;
    if (draw_calls) *draw_calls = s_prof.count_draw_calls;
    if (shadow_push) *shadow_push = s_prof.t_shadow_push;
    if (shadow_draws) *shadow_draws = s_prof.count_shadow_draws;
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

static Thread s_pc_sampler_thread;
alignas(0x1000) static char s_pc_sampler_stack[0x10000];
static std::atomic<bool> s_pc_sampler_running{false};
#define SC2_PC_SLOTS 0x20000u /* 32 MB .text @ 256B granularity */
static uint32_t s_pc_hist[SC2_PC_SLOTS];

struct PcCallerSlot {
    uint32_t pc_off;
    uint32_t lr_off;
    uint32_t count;
};
static PcCallerSlot s_svc_callers[32];
static PcCallerSlot s_mem_callers[32];

static void record_caller(PcCallerSlot *table, uint32_t pc_off, uint32_t lr_off) {
    for (int i = 0; i < 32; ++i) {
        if (table[i].count == 0) {
            table[i].pc_off = pc_off;
            table[i].lr_off = lr_off;
            table[i].count = 1;
            return;
        }
        if (table[i].pc_off == pc_off && table[i].lr_off == lr_off) {
            table[i].count++;
            return;
        }
    }
}

void Switch_StartWatchdog(void);

static void pc_sampler_worker(void *arg) {
    (void)arg;
    Handle main_h = envGetMainThreadHandle();
    if (main_h == INVALID_HANDLE) return;

    MemoryInfo mem_info{};
    u32 page_info = 0;
    if (R_FAILED(svcQueryMemory(&mem_info, &page_info, (u64)&Switch_StartWatchdog))) return;
    const uint64_t text_base = mem_info.addr;
    const uint64_t svc_off = (uint64_t)(uintptr_t)&svcSendSyncRequest - text_base;
    const uint64_t memcpy_off = (uint64_t)(uintptr_t)&memcpy - text_base;

    uint64_t window_start = armGetSystemTick();
    uint32_t total = 0, out_of_range = 0;
    while (s_pc_sampler_running.load(std::memory_order_relaxed)) {
        svcSleepThread(1500000ULL); // ~1.5ms (~667 Hz)
        Result rc = svcSetThreadActivity(main_h, ThreadActivity_Paused);
        if (R_FAILED(rc)) {
            FILE *f = Switch_GetMainLogFile();
            if (f) { fprintf(f, "[PcProf] svcSetThreadActivity failed: 0x%x\n", rc); fflush(f); }
            break;
        }
        ThreadContext ctx{};
        rc = svcGetThreadContext3(&ctx, main_h);
        svcSetThreadActivity(main_h, ThreadActivity_Runnable);
        if (R_FAILED(rc)) continue;

        total++;
        uint64_t pc = ctx.pc.x;
        if (pc < text_base) { out_of_range++; continue; }
        uint64_t rel = pc - text_base;
        uint64_t slot = rel >> 8;
        if (slot >= SC2_PC_SLOTS) { out_of_range++; continue; }
        s_pc_hist[slot]++;

        uint32_t lr_off = (ctx.lr >= text_base && ctx.lr - text_base < 0x2000000ULL) ? (uint32_t)(ctx.lr - text_base) : 0u;
        if (rel + 0x140ULL >= svc_off && rel < svc_off + 0x100ULL) {
            record_caller(s_svc_callers, (uint32_t)rel, lr_off);
        } else if (rel >= memcpy_off && rel < memcpy_off + 0x400ULL) {
            record_caller(s_mem_callers, (uint32_t)rel, lr_off);
        }

        uint64_t now = armGetSystemTick();
        if ((now - window_start) / 19200ULL >= 10000ULL) {
            FILE *f = Switch_GetMainLogFile();
            if (f) {
                fprintf(f, "[PcProf] window=10s samples=%u other=%u text_base=0x%llx ref_off=0x%llx\n",
                        total, out_of_range, (unsigned long long)text_base,
                        (unsigned long long)((uint64_t)&Switch_StartWatchdog - text_base));
                for (int rank = 0; rank < 20; ++rank) {
                    uint32_t best = 0, best_i = 0;
                    for (uint32_t i = 0; i < SC2_PC_SLOTS; ++i) {
                        if (s_pc_hist[i] > best) { best = s_pc_hist[i]; best_i = i; }
                    }
                    if (best == 0) break;
                    fprintf(f, "[PcProf] #%d +0x%08X %.2f%% (%u)\n", rank, best_i << 8,
                            100.0 * best / (double)total, best);
                    s_pc_hist[best_i] = 0;
                }
                for (int i = 0; i < 32 && s_svc_callers[i].count > 0; ++i) {
                    fprintf(f, "[PcProf-SVC] pc=+0x%08X lr=+0x%08X hits=%u\n",
                            s_svc_callers[i].pc_off, s_svc_callers[i].lr_off, s_svc_callers[i].count);
                }
                for (int i = 0; i < 32 && s_mem_callers[i].count > 0; ++i) {
                    fprintf(f, "[PcProf-MEM] pc=+0x%08X lr=+0x%08X hits=%u\n",
                            s_mem_callers[i].pc_off, s_mem_callers[i].lr_off, s_mem_callers[i].count);
                }
                fflush(f);
            }
            memset(s_pc_hist, 0, sizeof(s_pc_hist));
            memset(s_svc_callers, 0, sizeof(s_svc_callers));
            memset(s_mem_callers, 0, sizeof(s_mem_callers));
            total = 0; out_of_range = 0;
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
#elif defined(SC2_PC_PROFILE)
        s_pc_sampler_running.store(true, std::memory_order_relaxed);
        if (R_SUCCEEDED(threadCreate(&s_pc_sampler_thread, pc_sampler_worker, NULL, s_pc_sampler_stack, sizeof(s_pc_sampler_stack), 0x28, 1))) {
            threadStart(&s_pc_sampler_thread);
        }
#endif
    } else {
        printf("[Main] [!] Failed to create watchdog thread: 0x%x\n", rc);
    }
}

void Switch_StopWatchdog(void) {
    if (s_pc_sampler_running.exchange(false)) {
        threadWaitForExit(&s_pc_sampler_thread);
        threadClose(&s_pc_sampler_thread);
    }
    if (s_sampler_running.exchange(false)) {
        threadWaitForExit(&s_sampler_thread);
        threadClose(&s_sampler_thread);
    }
    if (s_watchdog_running.exchange(false)) {
        threadWaitForExit(&s_watchdog_thread);
        threadClose(&s_watchdog_thread);
    }
}

int __real_nanosleep(const struct timespec *req, struct timespec *rem);
int __wrap_nanosleep(const struct timespec *req, struct timespec *rem) {
    return __real_nanosleep(req, rem);
}

void sc2_fast_sub_000843D0(void);
void __wrap_sub_000843D0(void) {
    sc2_fast_sub_000843D0();
}

void __real_recomp_audio_output_reset_voice(uint32_t slot);
void __wrap_recomp_audio_output_reset_voice(uint32_t slot) {
    __real_recomp_audio_output_reset_voice(slot);
}

void __real_recomp_audio_output_submit(
    uint32_t slot, const uint8_t *pcm, uint32_t bytes,
    uint32_t sample_rate, uint32_t channels, uint32_t bits_per_sample,
    int32_t volume_hundredth_db);
void __wrap_recomp_audio_output_submit(
    uint32_t slot, const uint8_t *pcm, uint32_t bytes,
    uint32_t sample_rate, uint32_t channels, uint32_t bits_per_sample,
    int32_t volume_hundredth_db) {
    __real_recomp_audio_output_submit(
        slot, pcm, bytes, sample_rate, channels, bits_per_sample, volume_hundredth_db);
}

} // extern "C"
