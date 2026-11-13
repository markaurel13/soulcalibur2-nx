#include "switch_d3d_worker.h"
#include "d3d_draw_model.h"

#if defined(__SWITCH__)
#include <switch.h>
#endif
#include <arm_neon.h>

#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>

namespace {

enum {
    MAX_WORKER_JOBS = 4096,
    PUSH_ARENA_BYTES = 4 * 1024 * 1024,
    MAX_INLINE_BYTES = 256 * 1024,
    NV_BEGIN_END = 0x17fcu,
    NV_INLINE_ARRAY = 0x1818u,
};

enum JobType {
    JOB_DIRECT = 0,
    JOB_GATHER_DRAW,
    JOB_PUSH_SHADOW,
    JOB_RELEASE_MEM,
};

struct WorkerJob {
    JobType type;
    RecompD3dPresenter *presenter;
    union {
        RecompD3dPresenterCommand direct;
        SwitchGatherDrawJob gather;
        SwitchPushShadowJob push;
        struct {
            uint32_t base;
            uint32_t size;
            bool is_release;
        } rel;
    } data;
};

struct WorkerState {
    std::thread thread;
    std::mutex mutex;
    std::condition_variable cv_work;
    std::condition_variable cv_done;

    WorkerJob *jobs = nullptr;
    alignas(64) std::atomic<uint32_t> head{0};
    alignas(64) std::atomic<uint32_t> tail{0};
    alignas(64) std::atomic<bool> busy{false};
    std::atomic<bool> sleeping{false};
    std::atomic<bool> flush_waiter{false};
    std::atomic<bool> frame_waiter{false};
    std::atomic<bool> shutdown{false};
    bool active = false;

    uint8_t *push_arena = nullptr;
    uint32_t push_arena_used = 0;
    uint32_t active_arena = 0;
    std::atomic<uint64_t> frames_submitted{0};
    std::atomic<uint64_t> frames_completed{0};

    uint8_t *inline_scratch = nullptr;
    uint16_t *seq_indices = nullptr;
    uint16_t *quad_indices = nullptr;
};

static WorkerState g_worker;

static inline __attribute__((always_inline)) void copy_part_inline(uint8_t *dst, const uint8_t *src, uint32_t sz)
{
    if (sz == 12u) {
        uint64_t w0;
        uint32_t w1;
        __builtin_memcpy(&w0, src, 8);
        __builtin_memcpy(&w1, src + 8, 4);
        __builtin_memcpy(dst, &w0, 8);
        __builtin_memcpy(dst + 8, &w1, 4);
    } else if (sz == 8u) {
        uint64_t w;
        __builtin_memcpy(&w, src, 8);
        __builtin_memcpy(dst, &w, 8);
    } else if (sz == 16u) {
        vst1q_u8(dst, vld1q_u8(src));
    } else if (sz == 4u) {
        uint32_t w;
        __builtin_memcpy(&w, src, 4);
        __builtin_memcpy(dst, &w, 4);
    } else if (sz == 24u) {
        vst1q_u8(dst, vld1q_u8(src));
        uint64_t w;
        __builtin_memcpy(&w, src + 16, 8);
        __builtin_memcpy(dst + 16, &w, 8);
    } else {
        std::memcpy(dst, src, sz);
    }
}

static inline uint64_t worker_cntpct(void)
{
    uint64_t val;
    __asm__ volatile("mrs %0, cntpct_el0" : "=r"(val));
    return val;
}

static uint32_t s_prof_direct_cnt, s_prof_gather_cnt, s_prof_push_cnt;
static uint64_t s_prof_exec_ticks, s_prof_flush_ticks, s_prof_enqueue_ticks;
static uint32_t s_prof_wakeups, s_prof_full_spins;

static inline void fastCopyNeon(uint8_t * __restrict__ dst, const uint8_t * __restrict__ src, size_t sz)
{
    while (sz >= 64u) {
        __builtin_prefetch(src + 128, 0, 0);
        vst1q_u8(dst + 0, vld1q_u8(src + 0));
        vst1q_u8(dst + 16, vld1q_u8(src + 16));
        vst1q_u8(dst + 32, vld1q_u8(src + 32));
        vst1q_u8(dst + 48, vld1q_u8(src + 48));
        dst += 64;
        src += 64;
        sz -= 64;
    }
    while (sz >= 16u) {
        vst1q_u8(dst, vld1q_u8(src));
        dst += 16;
        src += 16;
        sz -= 16;
    }
    if (sz >= 8u) {
        uint64_t v;
        __builtin_memcpy(&v, src, 8);
        __builtin_memcpy(dst, &v, 8);
        dst += 8;
        src += 8;
        sz -= 8;
    }
    if (sz >= 4u) {
        uint32_t v;
        __builtin_memcpy(&v, src, 4);
        __builtin_memcpy(dst, &v, 4);
        dst += 4;
        src += 4;
        sz -= 4;
    }
    if (sz >= 2u) {
        uint16_t v;
        __builtin_memcpy(&v, src, 2);
        __builtin_memcpy(dst, &v, 2);
        dst += 2;
        src += 2;
        sz -= 2;
    }
    if (sz > 0u) {
        *dst = *src;
    }
}

static inline void copyPresenterCommand(RecompD3dPresenterCommand *dst, const RecompD3dPresenterCommand *src)
{
    dst->type = src->type;
    if (src->type != RECOMP_D3D_PRESENTER_COMMAND_DRAW) {
        switch (src->type) {
        case RECOMP_D3D_PRESENTER_COMMAND_CLEAR: dst->data.clear = src->data.clear; break;
        case RECOMP_D3D_PRESENTER_COMMAND_PRESENT: dst->data.present = src->data.present; break;
        case RECOMP_D3D_PRESENTER_COMMAND_GAMMA: std::memcpy(dst->data.gamma, src->data.gamma, sizeof(dst->data.gamma)); break;
        case RECOMP_D3D_PRESENTER_COMMAND_VISIBILITY_BEGIN:
        case RECOMP_D3D_PRESENTER_COMMAND_VISIBILITY_END: dst->data.visibility_index = src->data.visibility_index; break;
        case RECOMP_D3D_PRESENTER_COMMAND_COPY_MAIN_COLOR: dst->data.copy_destination = src->data.copy_destination; break;
        case RECOMP_D3D_PRESENTER_COMMAND_COPY_RECT: dst->data.copy_rect = src->data.copy_rect; break;
        default: *dst = *src; break;
        }
        return;
    }
    const RecompD3dPresenterDrawCommand &s = src->data.draw;
    RecompD3dPresenterDrawCommand &d = dst->data.draw;
    fastCopyNeon(reinterpret_cast<uint8_t *>(&d), reinterpret_cast<const uint8_t *>(&s),
                 offsetof(RecompD3dPresenterDrawCommand, program));
    if (s.program_count > 0u) {
        const uint32_t pc = s.program_count < 136u ? s.program_count : 136u;
        std::memcpy(d.program, s.program, pc * sizeof(d.program[0]));
        std::memcpy(d.program_constants, s.program_constants, sizeof(d.program_constants));
        d.program_alpha_mask = s.program_alpha_mask;
        d.program_mask_lod_bias = s.program_mask_lod_bias;
    } else {
        d.program_alpha_mask = false;
        d.program_mask_lod_bias = 0.0f;
    }
    if (s.directional.enabled) {
        d.directional = s.directional;
    } else {
        d.directional.enabled = false;
    }
    const size_t tail_off = offsetof(RecompD3dPresenterDrawCommand, vertex_bytes);
    fastCopyNeon(reinterpret_cast<uint8_t *>(&d) + tail_off,
                 reinterpret_cast<const uint8_t *>(&s) + tail_off,
                 sizeof(RecompD3dPresenterDrawCommand) - tail_off);
}

void executeGatherDraw(RecompD3dPresenter *presenter, SwitchGatherDrawJob &job)
{
    RecompD3dPresenterCommand cmd;
    copyPresenterCommand(&cmd, &job.cmd);
    RecompD3dPresenterDrawCommand *draw = &cmd.data.draw;

    uint16_t *dest_to_free = nullptr;
    if (job.smallest != 0u) {
        uint16_t *dest = static_cast<uint16_t *>(Switch_PresenterAllocPayload(job.index_count * sizeof(uint16_t)));
        if (!dest) {
            dest = static_cast<uint16_t *>(std::malloc(job.index_count * sizeof(uint16_t)));
            dest_to_free = dest;
        }
        const uint16_t *src_idx = job.indices;
        uint32_t i = 0u;
        uint16x8_t vsub = vdupq_n_u16(static_cast<uint16_t>(job.smallest));
        for (; i + 8u <= job.index_count; i += 8u) {
            uint16x8_t v = vld1q_u16(&src_idx[i]);
            vst1q_u16(&dest[i], vsubq_u16(v, vsub));
        }
        for (; i < job.index_count; ++i) {
            dest[i] = static_cast<uint16_t>(src_idx[i] - job.smallest);
        }
        draw->index_bytes = dest;
        if (job.owned_sequential) {
            std::free(job.owned_sequential);
            job.owned_sequential = nullptr;
        }
    } else {
        draw->index_bytes = job.indices;
    }

    const size_t total_vtx_bytes = static_cast<size_t>(job.vertex_stride) * job.vertex_count;
    uint8_t *out = static_cast<uint8_t *>(Switch_PresenterAllocPayload(total_vtx_bytes));
    bool free_out = false;
    if (!out) {
        out = static_cast<uint8_t *>(std::malloc(total_vtx_bytes));
        free_out = true;
    }

    if (job.vertex_count > 0u) {
        const uint32_t num_parts = job.num_parts;
        const uint32_t count = job.vertex_count;
        const uint32_t stride = job.vertex_stride;

        if (num_parts == 2u) {
            const uint8_t *src0 = job.parts[0].source;
            const uint8_t *src1 = job.parts[1].source;
            const uint32_t step0 = job.parts[0].step, step1 = job.parts[1].step;
            const uint32_t sz0 = job.parts[0].size, sz1 = job.parts[1].size;
            const uint32_t off1 = job.parts[1].offset;
            uint8_t *dst = out;
            for (uint32_t v = 0u; v < count; ++v, dst += stride, src0 += step0, src1 += step1) {
                copy_part_inline(dst, src0, sz0);
                copy_part_inline(dst + off1, src1, sz1);
            }
        } else if (num_parts == 3u) {
            const uint8_t *src0 = job.parts[0].source;
            const uint8_t *src1 = job.parts[1].source;
            const uint8_t *src2 = job.parts[2].source;
            const uint32_t step0 = job.parts[0].step, step1 = job.parts[1].step, step2 = job.parts[2].step;
            const uint32_t sz0 = job.parts[0].size, sz1 = job.parts[1].size, sz2 = job.parts[2].size;
            const uint32_t off1 = job.parts[1].offset, off2 = job.parts[2].offset;
            uint8_t *dst = out;
            for (uint32_t v = 0u; v < count; ++v, dst += stride, src0 += step0, src1 += step1, src2 += step2) {
                copy_part_inline(dst, src0, sz0);
                copy_part_inline(dst + off1, src1, sz1);
                copy_part_inline(dst + off2, src2, sz2);
            }
        } else {
            uint8_t *dst = out;
            for (uint32_t v = 0u; v < count; ++v, dst += stride) {
                for (uint32_t p = 0u; p < num_parts; ++p) {
                    copy_part_inline(dst + job.parts[p].offset,
                                     job.parts[p].source + static_cast<size_t>(v) * job.parts[p].step,
                                     job.parts[p].size);
                }
            }
        }
    }

    draw->vertex_bytes = out;
    Switch_PresenterSubmitDirect(presenter, &cmd);

    if (dest_to_free) std::free(dest_to_free);
    if (free_out) std::free(out);
    if (job.owned_sequential) std::free(job.owned_sequential);
}

void executePushShadow(RecompD3dPresenter *presenter, const SwitchPushShadowJob &job)
{
    const uint32_t *words = reinterpret_cast<const uint32_t *>(g_worker.push_arena + job.push_offset);
    const uint32_t word_count = job.push_bytes / 4u;

    RecompD3dVertexLayout layout;
    if (!recomp_d3d_fvf_layout(job.fvf, &layout) || layout.stride == 0u) return;

    uint32_t inline_prim = 0u;
    uint32_t inline_used = 0u;

    for (uint32_t at = 0u; at < word_count;) {
        const uint32_t header = words[at];
        const uint32_t method = header & 0x1ffcu;
        const uint32_t count = (header >> 18u) & 0x7ffu;
        const bool repeat = (header & 0x40000000u) != 0u;

        if (repeat && method == NV_INLINE_ARRAY) {
            const uint32_t bytes = count * 4u;
            if (inline_used + bytes <= MAX_INLINE_BYTES) {
                std::memcpy(g_worker.inline_scratch + inline_used, &words[at + 1u], bytes);
                inline_used += bytes;
            }
            at += 1u + count;
            continue;
        }

        for (uint32_t i = 0u; i < count; ++i) {
            const uint32_t target = repeat ? method : method + i * 4u;
            const uint32_t value = words[at + 1u + i];
            if (target == NV_INLINE_ARRAY) {
                if (inline_used + 4u <= MAX_INLINE_BYTES) {
                    std::memcpy(g_worker.inline_scratch + inline_used, &value, 4u);
                    inline_used += 4u;
                }
            } else if (target == NV_BEGIN_END) {
                if (value != 0u) {
                    inline_prim = value;
                    inline_used = 0u;
                } else if (inline_prim != 0u && inline_used > 0u) {
                    uint32_t prim = inline_prim;
                    uint32_t vtx_count = inline_used / layout.stride;
                    const bool quads = (prim == 8u);
                    if (quads) vtx_count &= ~3u;
                    if (prim == 9u) { prim = 6u; vtx_count &= ~1u; }
                    if (vtx_count > 0u) {
                        const uint32_t triangles = quads ? (vtx_count / 2u) : recomp_d3d_draw_triangle_count(prim, vtx_count);
                        if (triangles > 0u) {
                            RecompD3dPresenterCommand cmd{};
                            cmd.type = RECOMP_D3D_PRESENTER_COMMAND_DRAW;
                            RecompD3dPresenterDrawCommand *draw = &cmd.data.draw;
                            draw->fvf = job.fvf;
                            draw->target = job.target;
                            draw->vertex_count = vtx_count;
                            draw->vertex_stride = layout.stride;
                            draw->vertex_bytes = g_worker.inline_scratch;
                            if (quads) {
                                draw->primitive_type = 5u;
                                draw->index_count = triangles * 3u;
                                draw->index_bytes = g_worker.quad_indices;
                            } else {
                                draw->primitive_type = prim;
                                draw->index_count = vtx_count;
                                draw->index_bytes = g_worker.seq_indices;
                            }
                            draw->triangle_count = triangles;
                            draw->blend = job.blend;
                            draw->depth = job.depth;
                            draw->cull_mode = job.cull_mode;
                            draw->depth_bias = job.depth_bias;
                            draw->slope_scaled_depth_bias = job.slope_scaled_depth_bias;
                            draw->texture_factor = job.texture_factor;
                            draw->has_transform = job.has_transform;
                            if (job.has_transform) {
                                std::memcpy(draw->transform, job.transform, sizeof(job.transform));
                            }
                            std::memcpy(draw->viewport, job.viewport, sizeof(job.viewport));
                            std::memcpy(draw->screen_space_offset, job.screen_space_offset, sizeof(job.screen_space_offset));
                            draw->has_scissor = true;
                            std::memcpy(draw->scissor, job.scissor, sizeof(job.scissor));
                            draw->pixel_program.kind = RECOMP_D3D_PIXEL_COMBINERS;
                            draw->pixel_program.combiners.control = 0u;
                            Switch_PresenterSubmitDirect(presenter, &cmd);
                        }
                    }
                    inline_prim = 0u;
                    inline_used = 0u;
                }
            }
        }
        at += 1u + count;
    }
}

extern "C" void Switch_RegisterD3dWorkerThread(void);

void workerLoop()
{
#if defined(__SWITCH__)
    svcSetThreadPriority(CUR_THREAD_HANDLE, 0x2B);
    svcSetThreadCoreMask(CUR_THREAD_HANDLE, 2, 1ULL << 2);
#endif
    Switch_RegisterD3dWorkerThread();

    for (;;) {
        const uint32_t h = g_worker.head.load(std::memory_order_relaxed);
        uint32_t t = g_worker.tail.load(std::memory_order_acquire);
        if (h == t) {
            if (g_worker.shutdown.load(std::memory_order_acquire)) break;
            bool got_work = false;
            for (int s = 0; s < 64; ++s) {
                t = g_worker.tail.load(std::memory_order_acquire);
                if (h != t) { got_work = true; break; }
            }
            if (!got_work) {
                std::unique_lock<std::mutex> lock(g_worker.mutex);
                g_worker.sleeping.store(true, std::memory_order_seq_cst);
                g_worker.cv_work.wait(lock, [&] {
                    return g_worker.head.load(std::memory_order_relaxed) != g_worker.tail.load(std::memory_order_acquire)
                        || g_worker.shutdown.load(std::memory_order_acquire);
                });
                g_worker.sleeping.store(false, std::memory_order_release);
                continue;
            }
        }

        g_worker.busy.store(true, std::memory_order_release);
        const uint64_t te0 = worker_cntpct();
        WorkerJob &job = g_worker.jobs[h];
        switch (job.type) {
        case JOB_DIRECT: {
            ++s_prof_direct_cnt;
            const bool is_present = (job.data.direct.type == RECOMP_D3D_PRESENTER_COMMAND_PRESENT);
            Switch_PresenterSubmitDirect(job.presenter, &job.data.direct);
            if (is_present) {
                g_worker.frames_completed.fetch_add(1u, std::memory_order_release);
                if (g_worker.frame_waiter.load(std::memory_order_seq_cst)) {
                    std::lock_guard<std::mutex> lock(g_worker.mutex);
                    g_worker.cv_done.notify_all();
                }
            }
            break;
        }
        case JOB_GATHER_DRAW:
            ++s_prof_gather_cnt;
            executeGatherDraw(job.presenter, job.data.gather);
            break;
        case JOB_PUSH_SHADOW:
            ++s_prof_push_cnt;
            executePushShadow(job.presenter, job.data.push);
            break;
        case JOB_RELEASE_MEM:
            Switch_PresenterReleaseMemoryDirect(job.presenter, job.data.rel.base,
                                                job.data.rel.size, job.data.rel.is_release);
            break;
        }
        s_prof_exec_ticks += (worker_cntpct() - te0);
        g_worker.head.store((h + 1u) & (MAX_WORKER_JOBS - 1u), std::memory_order_seq_cst);
        g_worker.busy.store(false, std::memory_order_seq_cst);

        if (g_worker.flush_waiter.load(std::memory_order_seq_cst) &&
            g_worker.head.load(std::memory_order_relaxed) == g_worker.tail.load(std::memory_order_acquire)) {
            std::lock_guard<std::mutex> lock(g_worker.mutex);
            g_worker.cv_done.notify_all();
        }
    }
}

} // namespace

extern "C" {

void Switch_D3dWorkerInit(void)
{
    if (g_worker.active) return;
    g_worker.jobs = static_cast<WorkerJob *>(std::calloc(MAX_WORKER_JOBS, sizeof(WorkerJob)));
    g_worker.push_arena = static_cast<uint8_t *>(std::malloc(PUSH_ARENA_BYTES * 2u));
    g_worker.inline_scratch = static_cast<uint8_t *>(std::malloc(MAX_INLINE_BYTES));
    g_worker.seq_indices = static_cast<uint16_t *>(std::malloc(65536u * sizeof(uint16_t)));
    g_worker.quad_indices = static_cast<uint16_t *>(std::malloc(65536u * 3u * sizeof(uint16_t) / 2u));

    for (uint32_t i = 0u; i < 65536u; ++i) {
        g_worker.seq_indices[i] = static_cast<uint16_t>(i);
    }
    static const uint16_t order[6] = {0u, 1u, 2u, 0u, 2u, 3u};
    for (uint32_t i = 0u; i < 16384u; ++i) {
        for (uint32_t j = 0u; j < 6u; ++j) {
            g_worker.quad_indices[i * 6u + j] = static_cast<uint16_t>(i * 4u + order[j]);
        }
    }

    g_worker.head.store(0u, std::memory_order_relaxed);
    g_worker.tail.store(0u, std::memory_order_relaxed);
    g_worker.busy.store(false, std::memory_order_relaxed);
    g_worker.sleeping.store(false, std::memory_order_relaxed);
    g_worker.flush_waiter.store(false, std::memory_order_relaxed);
    g_worker.frame_waiter.store(false, std::memory_order_relaxed);
    g_worker.shutdown.store(false, std::memory_order_relaxed);
    g_worker.push_arena_used = 0;
    g_worker.active_arena = 0;
    g_worker.frames_submitted.store(0u, std::memory_order_relaxed);
    g_worker.frames_completed.store(0u, std::memory_order_relaxed);
    g_worker.active = true;
    g_worker.thread = std::thread(workerLoop);
}

void Switch_D3dWorkerShutdown(void)
{
    if (!g_worker.active) return;
    {
        std::lock_guard<std::mutex> lock(g_worker.mutex);
        g_worker.shutdown.store(true, std::memory_order_seq_cst);
    }
    g_worker.cv_work.notify_all();
    if (g_worker.thread.joinable()) g_worker.thread.join();
    std::free(g_worker.jobs);
    std::free(g_worker.push_arena);
    std::free(g_worker.inline_scratch);
    std::free(g_worker.seq_indices);
    std::free(g_worker.quad_indices);
    g_worker.jobs = nullptr;
    g_worker.push_arena = nullptr;
    g_worker.inline_scratch = nullptr;
    g_worker.seq_indices = nullptr;
    g_worker.quad_indices = nullptr;
    g_worker.active = false;
}

bool Switch_D3dWorkerIsActive(void)
{
    return g_worker.active;
}

static inline uint32_t acquireJobSlot(void)
{
    const uint32_t t = g_worker.tail.load(std::memory_order_relaxed);
    const uint32_t next_t = (t + 1u) & (MAX_WORKER_JOBS - 1u);
    while (next_t == g_worker.head.load(std::memory_order_acquire)) {
        ++s_prof_full_spins;
        if (g_worker.sleeping.load(std::memory_order_acquire)) {
            std::lock_guard<std::mutex> lock(g_worker.mutex);
            g_worker.cv_work.notify_one();
        }
#if defined(__SWITCH__)
        svcSleepThread(10000);
#else
        std::this_thread::yield();
#endif
    }
    return t;
}

static inline void commitJobSlot(uint32_t t)
{
    const uint32_t next_t = (t + 1u) & (MAX_WORKER_JOBS - 1u);
    g_worker.tail.store(next_t, std::memory_order_release);
    if ((next_t & 31u) == 0u && g_worker.sleeping.load(std::memory_order_acquire)) {
        ++s_prof_wakeups;
        std::lock_guard<std::mutex> lock(g_worker.mutex);
        g_worker.cv_work.notify_one();
    }
}

bool Switch_D3dWorkerEnqueueDirect(RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *cmd)
{
    if (!g_worker.active) return false;
    const uint64_t tq0 = worker_cntpct();
    const uint32_t slot = acquireJobSlot();
    WorkerJob &job = g_worker.jobs[slot];
    job.type = JOB_DIRECT;
    job.presenter = presenter;
    copyPresenterCommand(&job.data.direct, cmd);
    if (cmd->type == RECOMP_D3D_PRESENTER_COMMAND_DRAW) {
        RecompD3dPresenterDrawCommand &d = job.data.direct.data.draw;
        const size_t vtx_bytes = static_cast<size_t>(d.vertex_count) * d.vertex_stride;
        if (d.vertex_bytes != nullptr && vtx_bytes > 0u) {
            if (vtx_bytes > PUSH_ARENA_BYTES) return false;
            uint32_t offset = 0u;
            uint8_t *dst = Switch_D3dWorkerAllocPushBytes(static_cast<uint32_t>(vtx_bytes), &offset);
            if (dst == nullptr) return false;
            fastCopyNeon(dst, static_cast<const uint8_t *>(d.vertex_bytes), vtx_bytes);
            d.vertex_bytes = dst;
        }
        const size_t idx_bytes = static_cast<size_t>(d.index_count) * sizeof(uint16_t);
        if (d.index_bytes != nullptr && idx_bytes > 0u) {
            if (idx_bytes > PUSH_ARENA_BYTES) return false;
            uint32_t offset = 0u;
            uint8_t *dst = Switch_D3dWorkerAllocPushBytes(static_cast<uint32_t>(idx_bytes), &offset);
            if (dst == nullptr) return false;
            fastCopyNeon(dst, static_cast<const uint8_t *>(d.index_bytes), idx_bytes);
            d.index_bytes = dst;
        }
    }
    commitJobSlot(slot);
    s_prof_enqueue_ticks += (worker_cntpct() - tq0);
    return true;
}

bool Switch_D3dWorkerEnqueueGatherDraw(RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *cmd, const SwitchGatherDrawJob *gjob)
{
    if (!g_worker.active) return false;
    const uint64_t tq0 = worker_cntpct();
    const uint32_t slot = acquireJobSlot();
    WorkerJob &job = g_worker.jobs[slot];
    job.type = JOB_GATHER_DRAW;
    job.presenter = presenter;
    copyPresenterCommand(&job.data.gather.cmd, cmd);
    const size_t rest_off = offsetof(SwitchGatherDrawJob, num_parts);
    std::memcpy(reinterpret_cast<uint8_t *>(&job.data.gather) + rest_off,
                reinterpret_cast<const uint8_t *>(gjob) + rest_off,
                sizeof(SwitchGatherDrawJob) - rest_off);
    commitJobSlot(slot);
    s_prof_enqueue_ticks += (worker_cntpct() - tq0);
    return true;
}

uint8_t *Switch_D3dWorkerAllocPushBytes(uint32_t bytes, uint32_t *out_offset)
{
    if (!g_worker.active || g_worker.push_arena == nullptr) return nullptr;
    const uint32_t aligned = (bytes + 15u) & ~15u;
    if (g_worker.push_arena_used + aligned > PUSH_ARENA_BYTES) {
        return nullptr;
    }
    const uint32_t base = g_worker.active_arena * PUSH_ARENA_BYTES;
    *out_offset = base + g_worker.push_arena_used;
    uint8_t *ptr = g_worker.push_arena + base + g_worker.push_arena_used;
    g_worker.push_arena_used += aligned;
    return ptr;
}

bool Switch_D3dWorkerEnqueuePushShadow(RecompD3dPresenter *presenter, const SwitchPushShadowJob *pjob)
{
    if (!g_worker.active) return false;
    const uint64_t tq0 = worker_cntpct();
    const uint32_t slot = acquireJobSlot();
    WorkerJob &job = g_worker.jobs[slot];
    job.type = JOB_PUSH_SHADOW;
    job.presenter = presenter;
    job.data.push = *pjob;
    commitJobSlot(slot);
    s_prof_enqueue_ticks += (worker_cntpct() - tq0);
    return true;
}

bool Switch_D3dWorkerEnqueueReleaseMemory(RecompD3dPresenter *presenter, uint32_t base, uint32_t size, bool is_release)
{
    if (!g_worker.active) return false;
    const uint64_t tq0 = worker_cntpct();
    const uint32_t slot = acquireJobSlot();
    WorkerJob &job = g_worker.jobs[slot];
    job.type = JOB_RELEASE_MEM;
    job.presenter = presenter;
    job.data.rel.base = base;
    job.data.rel.size = size;
    job.data.rel.is_release = is_release;
    commitJobSlot(slot);
    s_prof_enqueue_ticks += (worker_cntpct() - tq0);
    return true;
}

void Switch_D3dWorkerFlush(void)
{
    if (!g_worker.active) return;
    const uint64_t tf0 = worker_cntpct();
    if (g_worker.sleeping.load(std::memory_order_acquire)) {
        std::lock_guard<std::mutex> lock(g_worker.mutex);
        g_worker.cv_work.notify_one();
    }
    if (g_worker.head.load(std::memory_order_acquire) != g_worker.tail.load(std::memory_order_acquire) ||
        g_worker.busy.load(std::memory_order_acquire)) {
        std::unique_lock<std::mutex> lock(g_worker.mutex);
        g_worker.flush_waiter.store(true, std::memory_order_seq_cst);
        g_worker.cv_done.wait(lock, [] {
            return g_worker.head.load(std::memory_order_acquire) == g_worker.tail.load(std::memory_order_acquire)
                && !g_worker.busy.load(std::memory_order_acquire);
        });
        g_worker.flush_waiter.store(false, std::memory_order_release);
    }
    g_worker.push_arena_used = 0;
    s_prof_flush_ticks += (worker_cntpct() - tf0);
}

void Switch_D3dWorkerEndFrame(void)
{
    if (!g_worker.active) return;
    const uint64_t tf0 = worker_cntpct();
    if (g_worker.sleeping.load(std::memory_order_acquire)) {
        std::lock_guard<std::mutex> lock(g_worker.mutex);
        g_worker.cv_work.notify_one();
    }
    const uint64_t sub = g_worker.frames_submitted.fetch_add(1u, std::memory_order_relaxed) + 1u;
    if ((sub - g_worker.frames_completed.load(std::memory_order_acquire)) >= 2u) {
        std::unique_lock<std::mutex> lock(g_worker.mutex);
        g_worker.frame_waiter.store(true, std::memory_order_seq_cst);
        g_worker.cv_done.wait(lock, [sub] {
            return (sub - g_worker.frames_completed.load(std::memory_order_acquire)) < 2u;
        });
        g_worker.frame_waiter.store(false, std::memory_order_release);
    }
    g_worker.active_arena ^= 1u;
    g_worker.push_arena_used = 0u;
    s_prof_flush_ticks += (worker_cntpct() - tf0);
}

void Switch_TakeWorkerProf(uint32_t *direct_cnt, uint32_t *gather_cnt, uint32_t *push_cnt,
    uint64_t *exec_ticks, uint64_t *flush_ticks, uint64_t *enqueue_ticks,
    uint32_t *wakeups, uint32_t *full_spins)
{
    if (direct_cnt) *direct_cnt = s_prof_direct_cnt;
    if (gather_cnt) *gather_cnt = s_prof_gather_cnt;
    if (push_cnt) *push_cnt = s_prof_push_cnt;
    if (exec_ticks) *exec_ticks = s_prof_exec_ticks;
    if (flush_ticks) *flush_ticks = s_prof_flush_ticks;
    if (enqueue_ticks) *enqueue_ticks = s_prof_enqueue_ticks;
    if (wakeups) *wakeups = s_prof_wakeups;
    if (full_spins) *full_spins = s_prof_full_spins;
    s_prof_direct_cnt = 0;
    s_prof_gather_cnt = 0;
    s_prof_push_cnt = 0;
    s_prof_exec_ticks = 0;
    s_prof_flush_ticks = 0;
    s_prof_enqueue_ticks = 0;
    s_prof_wakeups = 0;
    s_prof_full_spins = 0;
}

} // extern "C"
