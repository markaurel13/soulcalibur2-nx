#include "d3d_presenter_backend.h"
#include "d3d_draw_model.h"
#include "sc2_shaders.h"
#include "font8x8.h"
#include "switch_watchdog.h"

#include <switch.h>
#include <deko3d.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>

extern "C" {
void SwitchAudio_FlushPending(void);
void SwitchAudio_TakePerf(uint64_t *update_ticks, uint64_t *submit_ticks, uint64_t *flush_ticks,
                          uint32_t *updates, uint32_t *submits);
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
                          char *thread_times_buf, size_t thread_times_size);
bool Switch_IsGpuVtxEnabled(void);
void Switch_TakeDrawProf(uint64_t *idx, uint64_t *gather_t, uint64_t *mat, uint64_t *shader, uint32_t *draws);
void Switch_TakeDrawShaderProf(uint64_t *tex_t, uint64_t *comb_t, uint64_t *light_t, uint64_t *fog_t);
void Switch_TakePushFailProf(uint32_t out[8]);
bool Switch_IsFpsOverlayEnabled(void);
}
#include "switch_d3d_worker.h"

// ---- Diagnostic perf accounting (19.2 MHz system tick; printed every 120 presented frames) ----
enum GpuPassCategory : uint16_t {
    GPU_CAT_CLEAR = 0,    // Clear color / depth / stencil
    GPU_CAT_PRETRANS,     // 2D UI / HUD / Pretransformed fullscreen quads
    GPU_CAT_SHADOW,       // Stencil shadow volume extrusion (color_write_mask == 0 or stencil write without color)
    GPU_CAT_OPAQUE,       // 3D Opaque meshes (blend_enable == false)
    GPU_CAT_BLEND,        // 3D Transparent / Alpha blended (blend_enable == true)
    GPU_CAT_OVERLAY,      // FPS overlay / UI text
    GPU_CAT_COUNT
};

struct GpuQueryReport {
    uint64_t value;     // counter value (unused for Timestamp)
    uint64_t timestamp; // GPU timer, written at offset 8 by the hardware
};

struct GpuDrawMeta {
    uint16_t vtx_count;
    uint16_t idx_count;
    uint16_t tex_w;
    uint16_t tex_h;
    uint8_t tex_fmt;
    uint8_t tex_mips;
    uint8_t fsh_type; // 0=null, 1=null_alpha, 2=fast/color, 3=comb2, 4=gen/tex
    uint8_t prim_type;
    uint32_t c_control;
    uint32_t c_final0;
    uint32_t c_final1;
    uint32_t c_tex_modes;
    uint32_t c_in[2];
    uint32_t c_out[2];
    uint32_t a_in[2];
    uint32_t a_out[2];
};

#define MAX_GPU_QUERIES 4096
#define QUERY_BUF_SIZE (NUM_CMD_SLOTS * MAX_GPU_QUERIES * sizeof(GpuQueryReport))

struct PerfStats {
    uint64_t t_backend = 0;   // everything inside d3d_backend_submit (CLEAR + DRAW + PRESENT)
    uint64_t t_audio_pump = 0;// pumpAudioIfDue (FlushPending from the render path)
    uint64_t t_vtx = 0;       // CPU vertex transform / lighting / VS-program loop
    uint64_t t_draw_total = 0;// whole DRAW command
    uint64_t t_fence = 0;     // dkFenceWait: waiting for the GPU to finish an older frame
    uint64_t t_acquire = 0;   // dkQueueAcquireImage: vsync / swapchain wait
    uint64_t t_present = 0;   // PRESENT command body
    uint64_t t_tex = 0;       // texture lookup/upload
    uint64_t wall_sum = 0, wall_max = 0, last_present = 0;
    uint32_t frames = 0, draws = 0, verts = 0, idx = 0;
    uint32_t path_pre = 0, path_prog = 0, path_ff = 0, path_skin = 0, path_lit = 0;
    uint32_t verts_pre = 0, verts_prog = 0, verts_ff = 0, verts_skin = 0;
    uint32_t verts_ff_gpu = 0, verts_ff_cpu = 0;
    uint32_t fsh_null = 0, fsh_null_alpha_count = 0, fsh_fast = 0, fsh_comb2 = 0, fsh_gen = 0, fsh_comb_fast_count = 0;
    uint32_t comb_stages[9] = {};
    uint32_t slow_frames = 0; // wall > 20 ms
    uint32_t max_draws = 0;
    uint64_t dyn_bytes = 0;
    uint32_t hist_16_75 = 0, hist_16_95 = 0, hist_20 = 0, hist_33 = 0, hist_50 = 0, hist_gt50 = 0;
    uint64_t prev_t_backend = 0, prev_t_vtx = 0, prev_t_tex = 0, prev_t_audio_pump = 0;
    uint64_t prev_t_fence = 0, prev_t_acquire = 0, prev_t_draw_total = 0, prev_t_present = 0;
    struct PresHitchRec {
        uint32_t frame;
        double wall_ms, guest_ms, back_ms, draw_ms, vtx_ms, tex_ms, aud_ms, wait_ms;
        uint32_t draws, tex_up, dyn_kb;
    } pending_hitches[5] = {};
    uint32_t hitches_logged = 0;
    struct PacingMissRec {
        uint32_t frame;
        double wall_ms, back_ms, draw_ms, vtx_ms, acq_ms, fence_ms, pres_ms;
    } pacing_misses[3] = {};
    uint32_t pacing_misses_logged = 0;

    // Hardware GPU timestamp counters
    uint64_t gpu_cat_ns[GPU_CAT_COUNT] = {};
    uint32_t gpu_cat_draws[GPU_CAT_COUNT] = {};
    uint64_t gpu_total_ns = 0;
    uint32_t gpu_profiled_frames = 0;

    uint64_t gpu_fsh_ns[6] = {};
    uint32_t gpu_fsh_draws[6] = {};

    struct SlowDrawRecord {
        double ms;
        uint16_t cat;
        uint16_t fsh;
        uint16_t vtx;
        uint16_t idx;
        uint16_t tw, th;
        uint8_t fmt, mips;
        uint32_t c_control;
        uint32_t c_final0, c_final1;
        uint32_t c_tex_modes;
        uint32_t c_in[2], c_out[2];
        uint32_t a_in[2], a_out[2];
    };
    SlowDrawRecord worst_draws[5] = {};

    // [FxDiag] counters for missing visual effects investigation (120-frame window)
    uint32_t fx_offscreen_drops = 0;
    uint32_t fx_empty_drops = 0;
    uint32_t fx_bad_fvf_drops = 0;
    uint32_t fx_copy_main = 0;
    uint32_t fx_copy_rect = 0;
    uint32_t fx_prim_pts = 0;
    uint32_t fx_prim_lines = 0;
    uint32_t fx_no_depth = 0;
    uint32_t fx_alpha_test = 0;
};
static PerfStats g_perf;
static inline uint64_t perfNow() { return armGetSystemTick(); }
static inline double perfMs(uint64_t t) { return (double)t / 19200.0; }
struct PerfScope {
    uint64_t *acc; uint64_t t0;
    explicit PerfScope(uint64_t *a) : acc(a), t0(armGetSystemTick()) {}
    ~PerfScope() { *acc += armGetSystemTick() - t0; }
};

static uint64_t s_last_audio_pump_tick = 0;

static void pumpAudioIfDue(void) {
    uint64_t now = armGetSystemTick();
    // 19,200 ticks = 1 ms on Switch; 192,000 ticks = 10 ms
    if (now - s_last_audio_pump_tick >= 192000ULL) {
        s_last_audio_pump_tick = now;
        PerfScope ps(&g_perf.t_audio_pump);
        SwitchAudio_FlushPending();
    }
}

#define NUM_FRAMEBUFFERS 3
#define NUM_CMD_SLOTS 3
#define CMD_BUF_SIZE (4 * 1024 * 1024)
#define DYN_BUF_SIZE (32 * 1024 * 1024)
#define CODE_BUF_SIZE (256 * 1024)
#define DESC_BUF_SIZE (256 * 1024)
#define TEX_POOL_SIZE (160 * 1024 * 1024)

#define MAX_IMAGE_DESCS 2080
#define MAX_SAMPLER_DESCS 32
#define MAX_TEX_CACHE 2048
#define MAX_GPU_COPIES 16
#define GPU_COPY_POOL_SIZE (24 * 1024 * 1024)
#define TEX_HASH_BUCKETS 4096
#define PALETTE_BYTES 1024u

struct DekoVertex {
    float x, y, z, w;
    float r, g, b, a;
    float sr, sg, sb, fog;
    float u0, v0, u1, v1;
    float u2, v2, u3, v3;
};

struct DekoVertexFF {
    float x, y, z;        // 12 bytes
    float nx, ny, nz;     // 12 bytes
    uint32_t color;       //  4 bytes (BGRA8 Unorm)
    float pad;            //  4 bytes
    float u0, v0;         //  8 bytes
    float u1, v1;         //  8 bytes
}; // exactly 48 bytes

struct alignas(256) VtxBlock {
    float u_wvp[4][4];
    float u_norm_mat[4][4];
    float u_world_mat[4][4];
    float u_light_dir[8][4];
    float u_light_pos[8][4];
    float u_light_color[8][4];
    float u_light_ambient[8][4];
    float u_light_specular[8][4];
    float u_light_atten[8][4];
    float u_global_ambient[4];
    float u_mat_diffuse[4];
    float u_mat_ambient[4];
    float u_mat_emissive[4];
    float u_light_view[4];
    uint32_t u_light_ctrl[4];
    float u_fog_params[4];
    float u_fog_planes[4][4];
};

static void initVtxAttribsFF(DkVtxAttribState attribs[5]) {
    memset(attribs, 0, sizeof(DkVtxAttribState) * 5);

    // 0: Position (Float3)
    attribs[0].bufferId = 0;
    attribs[0].isFixed = 0;
    attribs[0].offset = offsetof(DekoVertexFF, x);
    attribs[0].size = DkVtxAttribSize_3x32;
    attribs[0].type = DkVtxAttribType_Float;
    attribs[0].isBgra = 0;

    // 1: Normal (Float3)
    attribs[1].bufferId = 0;
    attribs[1].isFixed = 0;
    attribs[1].offset = offsetof(DekoVertexFF, nx);
    attribs[1].size = DkVtxAttribSize_3x32;
    attribs[1].type = DkVtxAttribType_Float;
    attribs[1].isBgra = 0;

    // 2: Color (Unorm 4x8 BGRA)
    attribs[2].bufferId = 0;
    attribs[2].isFixed = 0;
    attribs[2].offset = offsetof(DekoVertexFF, color);
    attribs[2].size = DkVtxAttribSize_4x8;
    attribs[2].type = DkVtxAttribType_Unorm;
    attribs[2].isBgra = 1;

    // 3: UV0 (Float2)
    attribs[3].bufferId = 0;
    attribs[3].isFixed = 0;
    attribs[3].offset = offsetof(DekoVertexFF, u0);
    attribs[3].size = DkVtxAttribSize_2x32;
    attribs[3].type = DkVtxAttribType_Float;
    attribs[3].isBgra = 0;

    // 4: UV1 (Float2)
    attribs[4].bufferId = 0;
    attribs[4].isFixed = 0;
    attribs[4].offset = offsetof(DekoVertexFF, u1);
    attribs[4].size = DkVtxAttribSize_2x32;
    attribs[4].type = DkVtxAttribType_Float;
    attribs[4].isBgra = 0;
}

static const DkVtxBufferState s_bufferStateFF[] = {
    { sizeof(DekoVertexFF), 0 },
};

static void populateVtxBlock(const RecompD3dPresenterDrawCommand &draw,
                             float dx_offset, float dy_offset,
                             bool has_normal, bool has_fog, bool range_fog,
                             VtxBlock *vbo) {
    memset(vbo, 0, sizeof(*vbo));

    for (int r = 0; r < 4; ++r) {
        vbo->u_wvp[r][0] = draw.transform[r * 4 + 0] + dx_offset * draw.transform[r * 4 + 3];
        vbo->u_wvp[r][1] = draw.transform[r * 4 + 1] + dy_offset * draw.transform[r * 4 + 3];
        vbo->u_wvp[r][2] = draw.transform[r * 4 + 2];
        vbo->u_wvp[r][3] = draw.transform[r * 4 + 3];
    }

    const auto &lt = draw.directional;

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            vbo->u_norm_mat[r][c] = lt.normal_transforms[0][r * 4 + c];
            vbo->u_world_mat[r][c] = lt.world_transforms[0][r * 4 + c];
        }
    }

    uint32_t num_lights = std::min(lt.count, 8u);
    for (uint32_t li = 0; li < num_lights; ++li) {
        for (int c = 0; c < 4; ++c) {
            vbo->u_light_dir[li][c] = lt.directions[li][c];
            vbo->u_light_pos[li][c] = lt.positions[li][c];
            vbo->u_light_color[li][c] = lt.colors[li][c];
            vbo->u_light_ambient[li][c] = lt.ambient[li][c];
            vbo->u_light_specular[li][c] = lt.specular[li][c];
            vbo->u_light_atten[li][c] = lt.attenuation[li][c];
        }
    }

    for (int c = 0; c < 4; ++c) {
        vbo->u_global_ambient[c] = lt.global_ambient[c];
        vbo->u_mat_diffuse[c] = lt.material_diffuse[c];
        vbo->u_mat_ambient[c] = lt.material_ambient[c];
        vbo->u_mat_emissive[c] = lt.material_emissive[c];
        vbo->u_light_view[c] = lt.view[c];
    }

    vbo->u_light_ctrl[0] = num_lights;
    uint32_t flags = 0;
    if (lt.enabled) flags |= 1u;
    if (has_normal) flags |= 2u;
    if (lt.normalize) flags |= 4u;
    if (lt.sources[3] > 0.5f) flags |= 8u; // local viewer
    vbo->u_light_ctrl[1] = flags;

    uint32_t sources = 0;
    if (lt.sources[0] > 0.5f) sources |= 1u;
    if (lt.sources[1] > 0.5f) sources |= 2u;
    if (lt.sources[2] > 0.5f) sources |= 4u;
    vbo->u_light_ctrl[2] = sources;

    vbo->u_light_ctrl[3] = draw.zero_diffuse_rgb ? 1u : 0u;

    vbo->u_fog_params[0] = has_fog ? 1.0f : 0.0f;
    vbo->u_fog_params[1] = draw.general_fog[0][1];
    vbo->u_fog_params[2] = draw.general_fog[0][2];
    vbo->u_fog_params[3] = draw.general_fog[0][3];

    for (int c = 0; c < 4; ++c) {
        vbo->u_fog_planes[0][c] = draw.general_fog[1][c];
        vbo->u_fog_planes[1][c] = draw.general_fog[3][c];
        vbo->u_fog_planes[2][c] = draw.general_fog[4][c];
    }
    vbo->u_fog_planes[3][0] = range_fog ? 1.0f : 0.0f;
    vbo->u_fog_planes[3][1] = draw.general_fog[2][0];
}

struct alignas(256) FragBlock {
    float u_constants[24][4];
    float u_alpha_fog[4];
    uint32_t u_header[4];
    uint32_t u_comb_final[4];
    uint32_t u_stages[4][4];
    uint32_t u_comb_stages[8][4];
};

struct CachedTexture {
    bool used;
    uint32_t data;
    uint32_t format_byte;
    uint32_t width;
    uint32_t height;
    uint64_t fingerprint;
    uint8_t palette[PALETTE_BYTES];
    uint32_t desc_slot; // 1 .. MAX_IMAGE_DESCS - 1
    int hash_next;
    DkImage image;
    DkImageView view;
};

struct RecompD3dPresenter {
    DkDevice device;
    DkQueue queue;
    DkSwapchain swapchain;

    DkMemBlock block_cmd[NUM_CMD_SLOTS];
    DkCmdBuf cmdbuf[NUM_CMD_SLOTS];
    DkFence slot_fences[NUM_CMD_SLOTS];
    bool slot_has_fence[NUM_CMD_SLOTS];

    // Framebuffers (Color)
    DkMemBlock block_framebuffers;
    DkImage framebuffers[NUM_FRAMEBUFFERS];
    DkImage const* framebuffer_ptrs[NUM_FRAMEBUFFERS];
    DkImageView framebuffer_views[NUM_FRAMEBUFFERS];
    DkImageView const* target_ptrs[NUM_FRAMEBUFFERS];

    // Depth Buffer
    DkMemBlock block_depth;
    DkImage depth_image;
    DkImageView depth_view;

    // Shaders
    DkMemBlock block_code;
    DkShader vsh;
    DkShader vsh_ff;
    DkShader fsh_null;
    DkShader fsh_null_alpha;
    DkShader fsh_color;
    DkShader fsh_comb_fast;
    DkShader fsh_comb2;
    DkShader fsh_tex;

    // Descriptor Pool (Images + Samplers)
    DkMemBlock block_desc;
    DkImageDescriptor *img_descs_cpu;
    DkSamplerDescriptor *smp_descs_cpu;
    DkGpuAddr img_descs_gpu;
    DkGpuAddr smp_descs_gpu;

    // Texture Image Pool
    DkMemBlock block_tex;
    uint32_t tex_pool_offset;
    uint32_t next_cache_slot;
    CachedTexture tex_cache[MAX_TEX_CACHE];
    int tex_hash_head[TEX_HASH_BUCKETS];
    DkImage white_image;
    DkImageView white_view;

    // GPU-side render-target / backbuffer copy pool (for CopyRects / CopyMainColor / backbuffer sampling)
    struct GpuCopySlot {
        bool used;
        uint32_t data;
        uint32_t width;
        uint32_t height;
        DkImage image;
        DkImageView view;
        uint32_t desc_slot;
    };
    DkMemBlock block_gpu_copies;
    uint32_t gpu_copy_pool_offset;
    GpuCopySlot gpu_copies[MAX_GPU_COPIES];
    uint32_t next_gpu_copy_slot;
    DkImage backbuffer_snap_image;
    DkImageView backbuffer_snap_view;
    int last_presented_slot;

    // Dynamic vertex/index/uniform/staging buffer ring (double-buffered)
    DkMemBlock block_dyn[NUM_CMD_SLOTS];
    uint8_t *dyn_cpu[NUM_CMD_SLOTS];
    DkGpuAddr dyn_gpu[NUM_CMD_SLOTS];
    uint32_t dyn_offset;
    uint32_t cur_cmd_slot;

    // Guest resolution (640x480 or 720x480) vs Host resolution (1280x720)
    uint32_t guest_width;
    uint32_t guest_height;
    uint32_t width;
    uint32_t height;
    bool immediate_present;
    int current_slot;

    uint32_t frame_counter;
    uint32_t draws_in_frame;
    uint32_t tex_uploads_in_frame;

    // GPU query timestamps for per-category hardware profiling
    DkMemBlock block_queries;
    GpuQueryReport *queries_cpu[NUM_CMD_SLOTS];
    DkGpuAddr queries_gpu[NUM_CMD_SLOTS];
    uint16_t query_cats[NUM_CMD_SLOTS][MAX_GPU_QUERIES];
    GpuDrawMeta query_meta[NUM_CMD_SLOTS][MAX_GPU_QUERIES];
    uint32_t query_count[NUM_CMD_SLOTS];
};

static inline uint32_t hashGuestDataAddr(uint32_t addr) {
    return ((addr >> 4) ^ (addr >> 16)) & (TEX_HASH_BUCKETS - 1);
}

static void clearTexHashTable(RecompD3dPresenter *p) {
    for (int i = 0; i < TEX_HASH_BUCKETS; ++i) {
        p->tex_hash_head[i] = -1;
    }
    for (int i = 0; i < MAX_TEX_CACHE; ++i) {
        p->tex_cache[i].used = false;
        p->tex_cache[i].hash_next = -1;
    }
}

static void removeTexSlotFromHash(RecompD3dPresenter *p, int slot) {
    if (!p->tex_cache[slot].used) return;
    uint32_t h = hashGuestDataAddr(p->tex_cache[slot].data);
    int cur = p->tex_hash_head[h];
    int prev = -1;
    while (cur != -1) {
        if (cur == slot) {
            if (prev == -1) p->tex_hash_head[h] = p->tex_cache[cur].hash_next;
            else p->tex_cache[prev].hash_next = p->tex_cache[cur].hash_next;
            break;
        }
        prev = cur;
        cur = p->tex_cache[cur].hash_next;
    }
    p->tex_cache[slot].hash_next = -1;
    p->tex_cache[slot].used = false;
}

static void initVtxAttribs(DkVtxAttribState attribs[5]) {
    memset(attribs, 0, sizeof(DkVtxAttribState) * 5);

    attribs[0].bufferId = 0;
    attribs[0].isFixed = 0;
    attribs[0].offset = offsetof(DekoVertex, x);
    attribs[0].size = DkVtxAttribSize_4x32;
    attribs[0].type = DkVtxAttribType_Float;
    attribs[0].isBgra = 0;

    attribs[1].bufferId = 0;
    attribs[1].isFixed = 0;
    attribs[1].offset = offsetof(DekoVertex, r);
    attribs[1].size = DkVtxAttribSize_4x32;
    attribs[1].type = DkVtxAttribType_Float;
    attribs[1].isBgra = 0;

    attribs[2].bufferId = 0;
    attribs[2].isFixed = 0;
    attribs[2].offset = offsetof(DekoVertex, sr);
    attribs[2].size = DkVtxAttribSize_4x32;
    attribs[2].type = DkVtxAttribType_Float;
    attribs[2].isBgra = 0;

    attribs[3].bufferId = 0;
    attribs[3].isFixed = 0;
    attribs[3].offset = offsetof(DekoVertex, u0);
    attribs[3].size = DkVtxAttribSize_4x32;
    attribs[3].type = DkVtxAttribType_Float;
    attribs[3].isBgra = 0;

    attribs[4].bufferId = 0;
    attribs[4].isFixed = 0;
    attribs[4].offset = offsetof(DekoVertex, u2);
    attribs[4].size = DkVtxAttribSize_4x32;
    attribs[4].type = DkVtxAttribType_Float;
    attribs[4].isBgra = 0;
}

static const DkVtxBufferState s_bufferState[] = {
    { sizeof(DekoVertex), 0 },
};

static DkCompareOp toDkCompareOp(RecompD3dCompareFunc func) {
    switch (func) {
    case RECOMP_D3D_COMPARE_NEVER:         return DkCompareOp_Never;
    case RECOMP_D3D_COMPARE_LESS:          return DkCompareOp_Less;
    case RECOMP_D3D_COMPARE_EQUAL:         return DkCompareOp_Equal;
    case RECOMP_D3D_COMPARE_LESS_EQUAL:    return DkCompareOp_Lequal;
    case RECOMP_D3D_COMPARE_GREATER:       return DkCompareOp_Greater;
    case RECOMP_D3D_COMPARE_NOT_EQUAL:     return DkCompareOp_NotEqual;
    case RECOMP_D3D_COMPARE_GREATER_EQUAL: return DkCompareOp_Gequal;
    case RECOMP_D3D_COMPARE_ALWAYS:
    default:                               return DkCompareOp_Always;
    }
}

static DkStencilOp toDkStencilOp(RecompD3dStencilOp op) {
    switch (op) {
    case RECOMP_D3D_STENCIL_ZERO:     return DkStencilOp_Zero;
    case RECOMP_D3D_STENCIL_REPLACE:  return DkStencilOp_Replace;
    case RECOMP_D3D_STENCIL_INCRSAT:  return DkStencilOp_Incr;
    case RECOMP_D3D_STENCIL_DECRSAT:  return DkStencilOp_Decr;
    case RECOMP_D3D_STENCIL_INVERT:   return DkStencilOp_Invert;
    case RECOMP_D3D_STENCIL_INCRWRAP: return DkStencilOp_IncrWrap;
    case RECOMP_D3D_STENCIL_DECRWRAP: return DkStencilOp_DecrWrap;
    case RECOMP_D3D_STENCIL_KEEP:
    default:                          return DkStencilOp_Keep;
    }
}

static DkBlendOp toDkBlendOp(RecompD3dBlendOp op) {
    switch (op) {
    case RECOMP_D3D_BLEND_OP_MIN:              return DkBlendOp_Min;
    case RECOMP_D3D_BLEND_OP_MAX:              return DkBlendOp_Max;
    case RECOMP_D3D_BLEND_OP_SUBTRACT:         return DkBlendOp_Sub;
    case RECOMP_D3D_BLEND_OP_REVERSE_SUBTRACT: return DkBlendOp_RevSub;
    case RECOMP_D3D_BLEND_OP_ADD:
    default:                                   return DkBlendOp_Add;
    }
}

static DkBlendFactor toDkBlendFactor(RecompD3dBlendFactor factor, bool alpha_channel) {
    switch (factor) {
    case RECOMP_D3D_BLEND_ZERO:               return DkBlendFactor_Zero;
    case RECOMP_D3D_BLEND_ONE:                return DkBlendFactor_One;
    case RECOMP_D3D_BLEND_SRC_COLOR:          return alpha_channel ? DkBlendFactor_SrcAlpha : DkBlendFactor_SrcColor;
    case RECOMP_D3D_BLEND_INV_SRC_COLOR:      return alpha_channel ? DkBlendFactor_InvSrcAlpha : DkBlendFactor_InvSrcColor;
    case RECOMP_D3D_BLEND_SRC_ALPHA:          return DkBlendFactor_SrcAlpha;
    case RECOMP_D3D_BLEND_INV_SRC_ALPHA:      return DkBlendFactor_InvSrcAlpha;
    case RECOMP_D3D_BLEND_DST_ALPHA:          return DkBlendFactor_DstAlpha;
    case RECOMP_D3D_BLEND_INV_DST_ALPHA:      return DkBlendFactor_InvDstAlpha;
    case RECOMP_D3D_BLEND_DST_COLOR:          return alpha_channel ? DkBlendFactor_DstAlpha : DkBlendFactor_DstColor;
    case RECOMP_D3D_BLEND_INV_DST_COLOR:      return alpha_channel ? DkBlendFactor_InvDstAlpha : DkBlendFactor_InvDstColor;
    case RECOMP_D3D_BLEND_SRC_ALPHA_SATURATE: return DkBlendFactor_SrcAlphaSaturate;
    case RECOMP_D3D_BLEND_CONSTANT_COLOR:     return DkBlendFactor_ConstColor;
    case RECOMP_D3D_BLEND_INV_CONSTANT_COLOR: return DkBlendFactor_InvConstColor;
    case RECOMP_D3D_BLEND_CONSTANT_ALPHA:     return DkBlendFactor_ConstAlpha;
    case RECOMP_D3D_BLEND_INV_CONSTANT_ALPHA: return DkBlendFactor_InvConstAlpha;
    default:                                  return DkBlendFactor_One;
    }
}

// Fast 16-sample fingerprint (~64ns) to detect dynamic in-place texture updates without scanning MBs
static inline uint64_t quickFingerprint(const void *bytes, uint32_t size) {
    if (!bytes || size == 0) return 0;
    const uint8_t *p = static_cast<const uint8_t *>(bytes);
    uint64_t h = 14695981039346656037ULL ^ size;
    uint32_t step = (size > 64u) ? (size / 16u) : 1u;
    for (uint32_t off = 0; off < size; off += step) {
        h = (h ^ p[off]) * 1099511628211ULL;
    }
    return h;
}

static RecompD3dPresenter::GpuCopySlot *getOrAllocGpuCopy(
    RecompD3dPresenter *p, uint32_t data, uint32_t w, uint32_t h)
{
    if (w == 0 || h == 0 || w > 2048 || h > 2048) return nullptr;
    for (uint32_t i = 0; i < MAX_GPU_COPIES; ++i) {
        if (p->gpu_copies[i].used && p->gpu_copies[i].data == data &&
            p->gpu_copies[i].width == w && p->gpu_copies[i].height == h) {
            return &p->gpu_copies[i];
        }
    }

    DkImageLayoutMaker lm;
    dkImageLayoutMakerDefaults(&lm, p->device);
    lm.flags = DkImageFlags_Usage2DEngine | DkImageFlags_UsageRender;
    lm.format = DkImageFormat_RGBA8_Unorm;
    lm.dimensions[0] = w;
    lm.dimensions[1] = h;
    DkImageLayout layout;
    dkImageLayoutInitialize(&layout, &lm);
    uint32_t img_size = (uint32_t)dkImageLayoutGetSize(&layout);
    uint32_t img_align = dkImageLayoutGetAlignment(&layout);

    uint32_t off = (p->gpu_copy_pool_offset + img_align - 1) & ~(img_align - 1);
    if (off + img_size > GPU_COPY_POOL_SIZE) {
        // Reset pool after snap image (keep slot 0..snap_end)
        DkImageLayoutMaker slm;
        dkImageLayoutMakerDefaults(&slm, p->device);
        slm.flags = DkImageFlags_Usage2DEngine | DkImageFlags_UsageRender;
        slm.format = DkImageFormat_RGBA8_Unorm;
        slm.dimensions[0] = p->width;
        slm.dimensions[1] = p->height;
        DkImageLayout slayout;
        dkImageLayoutInitialize(&slayout, &slm);
        uint32_t ssize = (uint32_t)dkImageLayoutGetSize(&slayout);
        uint32_t salign = dkImageLayoutGetAlignment(&slayout);
        p->gpu_copy_pool_offset = (ssize + salign - 1) & ~(salign - 1);
        for (uint32_t i = 0; i < MAX_GPU_COPIES; ++i) p->gpu_copies[i].used = false;
        off = (p->gpu_copy_pool_offset + img_align - 1) & ~(img_align - 1);
        if (off + img_size > GPU_COPY_POOL_SIZE) return nullptr;
    }

    uint32_t slot_idx = p->next_gpu_copy_slot;
    p->next_gpu_copy_slot = (p->next_gpu_copy_slot + 1) % MAX_GPU_COPIES;

    RecompD3dPresenter::GpuCopySlot &c = p->gpu_copies[slot_idx];
    c.used = true;
    c.data = data;
    c.width = w;
    c.height = h;
    c.desc_slot = MAX_TEX_CACHE + 2 + slot_idx;
    p->gpu_copy_pool_offset = off + img_size;

    dkImageInitialize(&c.image, &layout, p->block_gpu_copies, off);
    dkImageViewDefaults(&c.view, &c.image);
    dkImageDescriptorInitialize(&p->img_descs_cpu[c.desc_slot], &c.view, false, false);
    return &c;
}

static inline uint32_t lookupGpuCopySlot(const RecompD3dPresenter *p, uint32_t data, uint32_t w, uint32_t h) {
    for (uint32_t i = 0; i < MAX_GPU_COPIES; ++i) {
        if (p->gpu_copies[i].used && p->gpu_copies[i].data == data &&
            p->gpu_copies[i].width == w && p->gpu_copies[i].height == h) {
            return p->gpu_copies[i].desc_slot;
        }
    }
    return 0;
}

static inline bool isPow2U32(uint32_t v) { return v != 0u && (v & (v - 1u)) == 0u; }

static void buildMortonLUT(uint32_t width, uint32_t height, uint32_t mx[2048], uint32_t my[2048]) {
    uint32_t mask_x = 0u, mask_y = 0u, bit = 1u, slot = 1u;
    while (bit < width || bit < height) {
        if (bit < width)  { mask_x |= slot; slot <<= 1u; }
        if (bit < height) { mask_y |= slot; slot <<= 1u; }
        bit <<= 1u;
    }
    uint32_t vx = 0u;
    for (uint32_t x = 0u; x < width; ++x) {
        mx[x] = vx;
        vx = (vx - mask_x) & mask_x;
    }
    uint32_t vy = 0u;
    for (uint32_t y = 0u; y < height; ++y) {
        my[y] = vy;
        vy = (vy - mask_y) & mask_y;
    }
}

static uint32_t lookupOrUploadSingleTexture(
    RecompD3dPresenter *p,
    const RecompD3dTextureDesc &desc,
    const void *tex_bytes,
    uint32_t tex_byte_count,
    const void *pal_bytes,
    uint32_t pal_byte_count,
    bool force_white)
{
    if (force_white) return 0;
    if (!tex_bytes || tex_byte_count == 0) return 0;
    if (desc.width == 0 || desc.height == 0 || desc.width > 2048 || desc.height > 2048) return 0;
    if (desc.volume_depth != 0 || desc.depth) return 0;

    const uint32_t fmt = desc.format_byte;
    const bool palettized = (fmt == RECOMP_D3D_TEXTURE_FORMAT_P8);
    if (palettized && (!pal_bytes || pal_byte_count < PALETTE_BYTES)) return 0;

    const bool dxt1 = (fmt == RECOMP_D3D_TEXTURE_FORMAT_DXT1);
    const bool dxt3 = (fmt == RECOMP_D3D_TEXTURE_FORMAT_DXT3);
    const bool dxt5 = (fmt == RECOMP_D3D_TEXTURE_FORMAT_DXT5);
    const bool compressed = (dxt1 || dxt3 || dxt5);

    uint64_t t_tex_start = perfNow();
    uint64_t fp = quickFingerprint(tex_bytes, tex_byte_count);
    uint32_t h = hashGuestDataAddr(desc.data);
    for (int idx = p->tex_hash_head[h]; idx != -1; idx = p->tex_cache[idx].hash_next) {
        CachedTexture &ct = p->tex_cache[idx];
        if (ct.used && ct.data == desc.data && ct.format_byte == fmt &&
            ct.width == desc.width && ct.height == desc.height && ct.fingerprint == fp) {
            if (!palettized || memcmp(ct.palette, pal_bytes, PALETTE_BYTES) == 0) {
                return ct.desc_slot;
            }
        }
    }
    uint64_t t_tex_decode_start = perfNow();

    uint32_t slot = p->cur_cmd_slot;
    DkCmdBuf cmd = p->cmdbuf[slot];
    uint8_t *dyn_cpu = p->dyn_cpu[slot];
    DkGpuAddr dyn_gpu = p->dyn_gpu[slot];

    DkImageFormat dk_fmt = DkImageFormat_BGRA8_Unorm;
    uint32_t upload_size = 0;
    uint32_t copy_w = desc.width;
    uint32_t copy_h = desc.height;
    uint32_t copy_row_len = 0;

    if (compressed) {
        uint32_t block_bytes = dxt1 ? 8u : 16u;
        uint32_t bw = (desc.width + 3u) / 4u;
        uint32_t bh = (desc.height + 3u) / 4u;
        upload_size = bw * bh * block_bytes;
        if (upload_size > tex_byte_count) return 0;

        dk_fmt = dxt1 ? DkImageFormat_RGBA_BC1 : (dxt3 ? DkImageFormat_RGBA_BC2 : DkImageFormat_RGBA_BC3);
        copy_w = bw * 4u;
        copy_h = bh * 4u;
        copy_row_len = 0; // Let Deko3D compute BC row stride automatically

        p->dyn_offset = (p->dyn_offset + 255u) & ~255u;
        if (p->dyn_offset + upload_size + 65536u > DYN_BUF_SIZE) {
            return 0;
        }
        uint32_t stage_offset = p->dyn_offset;
        p->dyn_offset += (upload_size + 255u) & ~255u;
        memcpy(dyn_cpu + stage_offset, tex_bytes, upload_size);

        DkImageLayoutMaker lm;
        dkImageLayoutMakerDefaults(&lm, p->device);
        lm.flags = 0;
        lm.format = dk_fmt;
        lm.dimensions[0] = copy_w;
        lm.dimensions[1] = copy_h;

        DkImageLayout layout;
        dkImageLayoutInitialize(&layout, &lm);
        uint32_t img_size = (uint32_t)dkImageLayoutGetSize(&layout);
        uint32_t img_align = dkImageLayoutGetAlignment(&layout);

        p->tex_pool_offset = (p->tex_pool_offset + img_align - 1) & ~(img_align - 1);
        if (p->tex_pool_offset + img_size > TEX_POOL_SIZE) {
            p->tex_pool_offset = 4096;
            clearTexHashTable(p);
            p->tex_pool_offset = (p->tex_pool_offset + img_align - 1) & ~(img_align - 1);
        }
        uint32_t img_offset = p->tex_pool_offset;
        p->tex_pool_offset += img_size;

        uint32_t cache_idx = p->next_cache_slot;
        p->next_cache_slot = (p->next_cache_slot + 1) % MAX_TEX_CACHE;
        removeTexSlotFromHash(p, (int)cache_idx);

        CachedTexture &ct = p->tex_cache[cache_idx];
        ct.used = true;
        ct.data = desc.data;
        ct.format_byte = fmt;
        ct.width = desc.width;
        ct.height = desc.height;
        ct.fingerprint = fp;
        ct.desc_slot = cache_idx + 1;
        ct.hash_next = p->tex_hash_head[h];
        p->tex_hash_head[h] = (int)cache_idx;

        dkImageInitialize(&ct.image, &layout, p->block_tex, img_offset);
        dkImageViewDefaults(&ct.view, &ct.image);
        dkImageDescriptorInitialize(&p->img_descs_cpu[ct.desc_slot], &ct.view, false, false);

        DkCopyBuf copySrc = { dyn_gpu + stage_offset, 0, 0 };
        DkImageRect copyRect = { 0, 0, 0, copy_w, copy_h, 1 };
        dkCmdBufCopyBufferToImage(cmd, &copySrc, &ct.view, &copyRect, 0);
        dkCmdBufBarrier(cmd, DkBarrier_Full, DkInvalidateFlags_Image | DkInvalidateFlags_Descriptors);

        p->tex_uploads_in_frame++;
        return ct.desc_slot;
    }

    // Uncompressed / Swizzled / Palettized -> decode directly into staging buffer as BGRA8
    const size_t texels = (size_t)desc.width * desc.height;
    upload_size = (uint32_t)(texels * 4u);

    p->dyn_offset = (p->dyn_offset + 255u) & ~255u;
    if (p->dyn_offset + upload_size + 65536u > DYN_BUF_SIZE) {
        return 0;
    }
    uint32_t stage_offset = p->dyn_offset;
    uint8_t *dst_bgra = dyn_cpu + stage_offset;

    if (palettized) {
        if (texels > tex_byte_count || !isPow2U32(desc.width) || !isPow2U32(desc.height)) return 0;
        uint32_t mx[2048], my[2048];
        buildMortonLUT(desc.width, desc.height, mx, my);
        const uint8_t *src8 = (const uint8_t *)tex_bytes;
        const uint32_t *pal32 = (const uint32_t *)pal_bytes;
        uint32_t *dst32 = (uint32_t *)dst_bgra;
        const uint32_t w = desc.width;
        for (uint32_t y = 0u; y < desc.height; ++y) {
            const uint32_t by = my[y];
            uint32_t *row = dst32 + (size_t)y * w;
            uint32_t x = 0u;
            for (; x + 4u <= w; x += 4u) {
                row[x + 0u] = pal32[src8[by | mx[x + 0u]]];
                row[x + 1u] = pal32[src8[by | mx[x + 1u]]];
                row[x + 2u] = pal32[src8[by | mx[x + 2u]]];
                row[x + 3u] = pal32[src8[by | mx[x + 3u]]];
            }
            for (; x < w; ++x) {
                row[x] = pal32[src8[by | mx[x]]];
            }
        }
    } else if (fmt == 0x12u || fmt == 0x1eu) { // Linear BGRA8 / BGRX8
        uint32_t pitch = desc.pitch ? desc.pitch : (desc.width * 4u);
        if ((uint64_t)(desc.height - 1u) * pitch + desc.width * 4u > tex_byte_count) return 0;
        for (uint32_t y = 0; y < desc.height; y++) {
            uint8_t *row = dst_bgra + (size_t)y * desc.width * 4u;
            memcpy(row, (const uint8_t *)tex_bytes + (size_t)y * pitch, desc.width * 4u);
            if (fmt == 0x1eu) {
                uint32_t *row32 = (uint32_t *)row;
                for (uint32_t x = 0; x < desc.width; x++) row32[x] |= 0xFF000000u;
            }
        }
    } else if (fmt == RECOMP_D3D_TEXTURE_FORMAT_A8R8G8B8 || fmt == 0x07u) { // Swizzled BGRA8 / BGRX8
        if (texels * 4u > tex_byte_count || !isPow2U32(desc.width) || !isPow2U32(desc.height)) return 0;
        uint32_t mx[2048], my[2048];
        buildMortonLUT(desc.width, desc.height, mx, my);
        const uint32_t *src32 = (const uint32_t *)tex_bytes;
        uint32_t *dst32 = (uint32_t *)dst_bgra;
        const uint32_t or_mask = (fmt == 0x07u) ? 0xFF000000u : 0u;
        const uint32_t w = desc.width;
        for (uint32_t y = 0u; y < desc.height; ++y) {
            const uint32_t by = my[y];
            uint32_t *row = dst32 + (size_t)y * w;
            for (uint32_t x = 0u; x < w; ++x) {
                row[x] = src32[by | mx[x]] | or_mask;
            }
        }
    } else if (fmt == RECOMP_D3D_TEXTURE_FORMAT_A8 || fmt == 0x00u) { // Swizzled A8 or L8
        if (texels > tex_byte_count || !isPow2U32(desc.width) || !isPow2U32(desc.height)) return 0;
        uint32_t mx[2048], my[2048];
        buildMortonLUT(desc.width, desc.height, mx, my);
        const uint8_t *src8 = (const uint8_t *)tex_bytes;
        uint32_t *dst32 = (uint32_t *)dst_bgra;
        const uint32_t w = desc.width;
        const bool is_a8 = (fmt == RECOMP_D3D_TEXTURE_FORMAT_A8);
        for (uint32_t y = 0u; y < desc.height; ++y) {
            const uint32_t by = my[y];
            uint32_t *row = dst32 + (size_t)y * w;
            for (uint32_t x = 0u; x < w; ++x) {
                uint32_t v = src8[by | mx[x]];
                row[x] = is_a8 ? (0x00FFFFFFu | (v << 24)) : (0xFF000000u | (v * 0x010101u));
            }
        }
    } else if (fmt == 0x01u || fmt == 0x02u || fmt == 0x03u || fmt == 0x04u || fmt == 0x05u || fmt == 0x1Au) {
        // Swizzled 16-bit formats (AL88, ARGB1555, XRGB1555, ARGB4444, RGB565)
        if (texels * 2u > tex_byte_count || !isPow2U32(desc.width) || !isPow2U32(desc.height)) return 0;
        uint32_t mx[2048], my[2048];
        buildMortonLUT(desc.width, desc.height, mx, my);
        const uint16_t *src16 = (const uint16_t *)tex_bytes;
        uint32_t *dst32 = (uint32_t *)dst_bgra;
        const uint32_t w = desc.width;
        for (uint32_t y = 0u; y < desc.height; ++y) {
            const uint32_t by = my[y];
            uint32_t *row = dst32 + (size_t)y * w;
            for (uint32_t x = 0u; x < w; ++x) {
                uint16_t c = src16[by | mx[x]];
                uint32_t r, g, b, a;
                if (fmt == 0x01u || fmt == 0x1Au) { // A8L8
                    uint32_t l = (uint32_t)(c & 0xFFu);
                    a = (uint32_t)(c >> 8);
                    row[x] = (a << 24) | (l * 0x010101u);
                } else if (fmt == 0x02u || fmt == 0x03u) { // ARGB1555 / XRGB1555
                    b = ((c & 0x1Fu) * 255u) / 31u;
                    g = (((c >> 5) & 0x1Fu) * 255u) / 31u;
                    r = (((c >> 10) & 0x1Fu) * 255u) / 31u;
                    a = (fmt == 0x02u) ? ((c & 0x8000u) ? 255u : 0u) : 255u;
                    row[x] = (a << 24) | (r << 16) | (g << 8) | b;
                } else if (fmt == 0x04u) { // ARGB4444
                    b = (c & 0xFu) * 17u;
                    g = ((c >> 4) & 0xFu) * 17u;
                    r = ((c >> 8) & 0xFu) * 17u;
                    a = ((c >> 12) & 0xFu) * 17u;
                    row[x] = (a << 24) | (r << 16) | (g << 8) | b;
                } else { // RGB565
                    b = ((c & 0x1Fu) * 255u) / 31u;
                    g = (((c >> 5) & 0x3Fu) * 255u) / 63u;
                    r = (((c >> 11) & 0x1Fu) * 255u) / 31u;
                    row[x] = 0xFF000000u | (r << 16) | (g << 8) | b;
                }
            }
        }
    } else {
        return 0;
    }

    uint64_t t_tex_decode_end = perfNow();
    p->dyn_offset += (upload_size + 255u) & ~255u;

    DkImageLayoutMaker lm;
    dkImageLayoutMakerDefaults(&lm, p->device);
    lm.flags = DkImageFlags_Usage2DEngine;
    lm.format = DkImageFormat_BGRA8_Unorm;
    lm.dimensions[0] = desc.width;
    lm.dimensions[1] = desc.height;

    DkImageLayout layout;
    dkImageLayoutInitialize(&layout, &lm);
    uint32_t img_size = (uint32_t)dkImageLayoutGetSize(&layout);
    uint32_t img_align = dkImageLayoutGetAlignment(&layout);

    p->tex_pool_offset = (p->tex_pool_offset + img_align - 1) & ~(img_align - 1);
    if (p->tex_pool_offset + img_size > TEX_POOL_SIZE) {
        p->tex_pool_offset = 4096;
        clearTexHashTable(p);
        p->tex_pool_offset = (p->tex_pool_offset + img_align - 1) & ~(img_align - 1);
    }
    uint32_t img_offset = p->tex_pool_offset;
    p->tex_pool_offset += img_size;

    uint32_t cache_idx = p->next_cache_slot;
    p->next_cache_slot = (p->next_cache_slot + 1) % MAX_TEX_CACHE;
    removeTexSlotFromHash(p, (int)cache_idx);

    CachedTexture &ct = p->tex_cache[cache_idx];
    ct.used = true;
    ct.data = desc.data;
    ct.format_byte = fmt;
    ct.width = desc.width;
    ct.height = desc.height;
    ct.fingerprint = fp;
    if (palettized) memcpy(ct.palette, pal_bytes, PALETTE_BYTES);
    ct.desc_slot = cache_idx + 1;
    ct.hash_next = p->tex_hash_head[h];
    p->tex_hash_head[h] = (int)cache_idx;

    dkImageInitialize(&ct.image, &layout, p->block_tex, img_offset);
    dkImageViewDefaults(&ct.view, &ct.image);
    dkImageDescriptorInitialize(&p->img_descs_cpu[ct.desc_slot], &ct.view, false, false);

    DkCopyBuf copySrc = { dyn_gpu + stage_offset, desc.width * 4u, desc.height };
    DkImageRect copyRect = { 0, 0, 0, desc.width, desc.height, 1 };
    dkCmdBufCopyBufferToImage(cmd, &copySrc, &ct.view, &copyRect, 0);
    dkCmdBufBarrier(cmd, DkBarrier_Full, DkInvalidateFlags_Image | DkInvalidateFlags_Descriptors);

    p->tex_uploads_in_frame++;
    {
        uint64_t t_tex_end = perfNow();
        double total_ms = perfMs(t_tex_end - t_tex_start);
        if (total_ms > 2.0) {
            printf("[TexSlow] fmt=0x%02x %ux%u bytes=%u total=%.2fms (hash=%.2f decode=%.2f dk=%.2f)\n",
                   fmt, desc.width, desc.height, tex_byte_count, total_ms,
                   perfMs(t_tex_decode_start - t_tex_start),
                   perfMs(t_tex_decode_end - t_tex_decode_start),
                   perfMs(t_tex_end - t_tex_decode_end));
        }
    }
    return ct.desc_slot;
}

// Ultra-fast IEEE-754 specular power approximation for ARM64 CPU (~4 cycles vs ~150 cycles in std::pow)
static inline float fast_pow_specular(float x, float p) {
    if (x <= 0.0f) return 0.0f;
    if (x >= 1.0f || p <= 0.0f) return 1.0f;
    union { float f; int32_t i; } u = { x };
    float log2_approx = (float)(u.i - 0x3f780000) * 1.1920928955078125e-7f;
    float exp_val = p * log2_approx;
    if (exp_val < -12.0f) return 0.0f;
    int32_t exp_int = (int32_t)(exp_val * 8388608.0f) + 0x3f780000;
    if (exp_int <= 0) return 0.0f;
    u.i = exp_int;
    return u.f;
}

// Straight-line NV2A programmable vertex shader interpreter on CPU
static void evalSourceReg(
    uint32_t mux, uint32_t reg, uint32_t swizzle, bool negate,
    uint32_t attr, uint32_t constant,
    const float r[13][4], const float v[16][4], const float vc[192][4],
    float out[4])
{
    const float *src = nullptr;
    static const float zero[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    if (mux == 1 && reg <= 12) src = r[reg];
    else if (mux == 2 && attr < 16) src = v[attr];
    else if (mux == 3 && constant < 192) src = vc[constant];
    else src = zero;

    for (unsigned i = 0; i < 4; ++i) {
        uint32_t comp = (swizzle >> (6u - 2u * i)) & 3u;
        float val = src[comp];
        out[i] = negate ? -val : val;
    }
}

static void writeMasked(float dst[4], uint32_t mask, const float val[4]) {
    for (unsigned i = 0; i < 4; ++i) {
        if (mask & (8u >> i)) dst[i] = val[i];
    }
}

static bool evalVertexProgramCPU(
    const RecompD3dPresenterDrawCommand &draw,
    const float v_in[16][4],
    float r12_out[4],
    float o3_out[4],
    float o4_out[4],
    float &o5_x_out,
    float o9_out[4],
    float o10_out[4],
    float o11_out[4],
    float o12_out[4])
{
    if (!draw.program_count || draw.program_count > 136) return false;
    float r[13][4]{};
    float o[16][4]{};
    // Match d3d_vertex_program.cpp: o3..o12 start at 0.0f!

    for (uint32_t i = 0; i < draw.program_count; ++i) {
        const uint32_t a = draw.program[i][1];
        const uint32_t b = draw.program[i][2];
        const uint32_t c = draw.program[i][3];
        const uint32_t mac = (a >> 21) & 15u;
        const uint32_t ilu = (a >> 25) & 7u;
        const uint32_t attr = (a >> 9) & 15u;
        const uint32_t constant = (a >> 13) & 255u;

        float x[4], y[4], z[4];
        evalSourceReg((b >> 26) & 3u, (b >> 28) & 15u, a & 255u, (a & 256u) != 0, attr, constant, r, v_in, draw.program_constants, x);
        evalSourceReg((b >> 11) & 3u, (b >> 13) & 15u, (b >> 17) & 255u, (b & (1u << 25)) != 0, attr, constant, r, v_in, draw.program_constants, y);
        evalSourceReg((c >> 28) & 3u, ((b & 3u) << 2) | (c >> 30), (b >> 2) & 255u, (b & 1024u) != 0, attr, constant, r, v_in, draw.program_constants, z);

        float m[4] = {0, 0, 0, 0};
        float l[4] = {0, 0, 0, 0};

        switch (mac) {
        case 0: break;
        case 1: // MOV
            for (int k = 0; k < 4; ++k) m[k] = x[k];
            break;
        case 2: // MUL
            for (int k = 0; k < 4; ++k) m[k] = x[k] * y[k];
            break;
        case 3: // ADD
            for (int k = 0; k < 4; ++k) m[k] = x[k] + z[k];
            break;
        case 4: // MAD
            for (int k = 0; k < 4; ++k) m[k] = x[k] * y[k] + z[k];
            break;
        case 5: { // DP3
            float d = x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
            m[0] = m[1] = m[2] = m[3] = d;
            break;
        }
        case 6: { // DPH
            float d = x[0] * y[0] + x[1] * y[1] + x[2] * y[2] + y[3];
            m[0] = m[1] = m[2] = m[3] = d;
            break;
        }
        case 7: { // DP4
            float d = x[0] * y[0] + x[1] * y[1] + x[2] * y[2] + x[3] * y[3];
            m[0] = m[1] = m[2] = m[3] = d;
            break;
        }
        case 8: { // DST
            m[0] = 1.0f;
            m[1] = x[1] * y[1];
            m[2] = x[2];
            m[3] = y[3];
            break;
        }
        case 9: // MIN
            for (int k = 0; k < 4; ++k) m[k] = std::min(x[k], y[k]);
            break;
        case 10: // MAX
            for (int k = 0; k < 4; ++k) m[k] = std::max(x[k], y[k]);
            break;
        case 11: // SLT
            for (int k = 0; k < 4; ++k) m[k] = (x[k] < y[k]) ? 1.0f : 0.0f;
            break;
        case 12: // SGE
            for (int k = 0; k < 4; ++k) m[k] = (x[k] >= y[k]) ? 1.0f : 0.0f;
            break;
        default: break;
        }

        switch (ilu) {
        case 0: break;
        case 1: // MOV
            for (int k = 0; k < 4; ++k) l[k] = z[k];
            break;
        case 2: // RCP
        case 3: { // RCC
            float zx = z[0];
            float inv = (zx != 0.0f) ? (1.0f / zx) : 1.0e19f;
            if (ilu == 3) {
                float s = (zx < 0.0f) ? -1.0f : 1.0f;
                inv = s * std::clamp(std::fabs(inv), 5.42101e-20f, 1.884467e19f);
            }
            l[0] = l[1] = l[2] = l[3] = inv;
            break;
        }
        case 4: { // RSQ
            float ax = std::fabs(z[0]);
            float rsq = (ax > 0.0f) ? (1.0f / std::sqrt(ax)) : 1.0e19f;
            l[0] = l[1] = l[2] = l[3] = rsq;
            break;
        }
        case 5: { // EXP
            float p = std::floor(z[0]);
            l[0] = std::ldexp(1.0f, (int)p);
            l[1] = z[0] - p;
            l[2] = std::exp2(z[0]);
            l[3] = 1.0f;
            break;
        }
        case 6: { // LOG
            float ax = std::fabs(z[0]);
            if (ax > 0.0f) {
                int exp_i = 0;
                float mant = std::frexp(ax, &exp_i) * 2.0f;
                exp_i -= 1;
                l[0] = (float)exp_i;
                l[1] = mant;
                l[2] = std::log2(ax);
            } else {
                l[0] = -1.0e19f;
                l[1] = 1.0f;
                l[2] = -1.0e19f;
            }
            l[3] = 1.0f;
            break;
        }
        case 7: { // LIT
            l[0] = 1.0f;
            l[1] = std::max(z[0], 0.0f);
            l[2] = (z[0] > 0.0f && z[1] > 0.0f) ? fast_pow_specular(z[1], std::clamp(z[3], -127.9961f, 127.9961f)) : 0.0f;
            l[3] = 1.0f;
            break;
        }
        default: break;
        }

        const uint32_t reg = (c >> 20) & 15u;
        const uint32_t mac_mask = (ilu && reg == 1u) ? 0u : ((c >> 24) & 15u);
        const uint32_t ilu_mask = (c >> 16) & 15u;
        if (mac && reg <= 12) writeMasked(r[reg], mac_mask, m);
        if (ilu) {
            uint32_t ilu_reg = mac ? 1u : reg;
            if (ilu_reg <= 12) writeMasked(r[ilu_reg], ilu_mask, l);
        }

        const uint32_t om = (c >> 12) & 15u;
        const uint32_t out_idx = (c >> 3) & 255u;
        if (om && (c & 2048u)) {
            const float *res = (c & 4u) ? l : m;
            if (out_idx == 0) writeMasked(r[12], om, res);
            else if (out_idx < 16) writeMasked(o[out_idx], om, res);
        }
    }

    memcpy(r12_out, r[12], sizeof(float) * 4);
    memcpy(o3_out, o[3], sizeof(float) * 4);
    memcpy(o4_out, o[4], sizeof(float) * 4);
    o5_x_out = o[5][0];
    memcpy(o9_out, o[9], sizeof(float) * 4);
    memcpy(o10_out, o[10], sizeof(float) * 4);
    memcpy(o11_out, o[11], sizeof(float) * 4);
    memcpy(o12_out, o[12], sizeof(float) * 4);
    return true;
}

static float computeFogFactor(const RecompD3dPresenterDrawCommand &draw, float d) {
    if (draw.general_fog[0][0] < 0.5f) return 1.0f;
    float mode = draw.general_fog[0][1];
    float f = d;
    if (mode > 2.5f) {
        float start = draw.general_fog[0][2];
        float end = draw.general_fog[0][3];
        float range = end - start;
        f = (range != 0.0f) ? ((end - d) / range) : ((d < end) ? 1.0f : 0.0f);
    } else if (mode > 1.5f) {
        float e = draw.general_fog[2][0] * d;
        f = std::exp(-e * e);
    } else if (mode > 0.5f) {
        f = std::exp(-draw.general_fog[2][0] * d);
    }
    return std::clamp(f, 0.0f, 1.0f);
}

static void populateFragBlock(const RecompD3dPresenterDrawCommand &draw, FragBlock *fbo, bool &is_fast_path) {
    memset(fbo, 0, sizeof(*fbo));
    const RecompD3dPixelProgram &pp = draw.pixel_program;

    memcpy(fbo->u_constants, pp.constants, sizeof(fbo->u_constants));
    if (pp.kind == RECOMP_D3D_PIXEL_NONE) {
        // Legacy fallback constants if pixel_program.kind == 0
        fbo->u_constants[0][0] = (draw.texture.linear && draw.texture.width != 0u) ? (1.0f / (float)draw.texture.width) : 1.0f;
        fbo->u_constants[0][1] = (draw.texture.linear && draw.texture.height != 0u) ? (1.0f / (float)draw.texture.height) : 1.0f;
        fbo->u_constants[0][2] = (draw.texture.format_byte == RECOMP_D3D_TEXTURE_FORMAT_A8 || draw.texture_alpha_only) ? 1.0f : 0.0f;
        fbo->u_constants[0][3] = (draw.texture.format_byte == 0x07u || draw.texture.format_byte == 0x1eu) ? 1.0f : 0.0f;
        fbo->u_constants[4][0] = ((draw.texture_factor >> 16u) & 0xffu) / 255.0f;
        fbo->u_constants[4][1] = ((draw.texture_factor >> 8u) & 0xffu) / 255.0f;
        fbo->u_constants[4][2] = (draw.texture_factor & 0xffu) / 255.0f;
        fbo->u_constants[4][3] = ((draw.texture_factor >> 24u) & 0xffu) / 255.0f;
    }

    fbo->u_alpha_fog[0] = draw.depth.alpha_ref / 255.0f;
    fbo->u_alpha_fog[1] = pp.constants[23][0];
    fbo->u_alpha_fog[2] = pp.constants[23][1];
    fbo->u_alpha_fog[3] = pp.constants[23][2];

    uint32_t alpha_mode = draw.depth.alpha_test_enable ? ((uint32_t)draw.depth.alpha_func + 1u) : 0u;
    bool fog_enabled = (draw.general_fog[0][0] > 0.5f);

    fbo->u_header[0] = pp.kind;
    fbo->u_header[1] = (pp.kind == RECOMP_D3D_PIXEL_NONE) ? (draw.has_texture ? 1u : 0u) : pp.stage_count;
    fbo->u_header[2] = pp.specular_add ? 1u : 0u;
    fbo->u_header[3] = (alpha_mode & 0xFFu) | ((fog_enabled ? 1u : 0u) << 8u);

    for (uint32_t s = 0; s < 4u; ++s) {
        const RecompD3dPixelStage &st = pp.stages[s];
        fbo->u_stages[s][0] = (st.color_op & 0xFFu)
                            | ((st.color_arg[1] & 0xFFu) << 8u)
                            | ((st.color_arg[2] & 0xFFu) << 16u)
                            | ((st.alpha_op & 0xFFu) << 24u);
        fbo->u_stages[s][1] = (st.alpha_arg[1] & 0xFFu)
                            | ((st.alpha_arg[2] & 0xFFu) << 8u)
                            | ((st.color_arg[0] & 0xFFu) << 16u)
                            | ((st.alpha_arg[0] & 0xFFu) << 24u);
        fbo->u_stages[s][2] = (st.result_arg & 0xFFu)
                            | ((st.texcoord & 0xFFu) << 8u)
                            | ((st.sampled ? 1u : 0u) << 16u)
                            | ((st.alpha_kill ? 1u : 0u) << 24u);
        fbo->u_stages[s][3] = 0u;
    }

    if (pp.kind == RECOMP_D3D_PIXEL_COMBINERS) {
        const RecompD3dCombiners &c = pp.combiners;
        fbo->u_header[1] = c.control & 0xFFu;
        fbo->u_comb_final[0] = c.final0;
        fbo->u_comb_final[1] = c.final1;
        fbo->u_comb_final[2] = c.control;
        fbo->u_comb_final[3] = c.texture_modes;
        for (uint32_t s = 0; s < 8u; ++s) {
            fbo->u_comb_stages[s][0] = c.color_input[s];
            fbo->u_comb_stages[s][1] = c.color_output[s];
            fbo->u_comb_stages[s][2] = c.alpha_input[s];
            fbo->u_comb_stages[s][3] = c.alpha_output[s];
        }
    }

    is_fast_path = (pp.kind == RECOMP_D3D_PIXEL_NONE || pp.kind == RECOMP_D3D_PIXEL_FIXED_FUNCTION)
                && (pp.stage_count <= 2u);
}

static RecompD3dPresenter *s_active_deko_presenter = nullptr;

extern "C" void Switch_GetDeko3DState(int *current_slot, int *cur_cmd_slot, uint32_t *frame_counter) {
    if (s_active_deko_presenter) {
        if (current_slot) *current_slot = s_active_deko_presenter->current_slot;
        if (cur_cmd_slot) *cur_cmd_slot = (int)s_active_deko_presenter->cur_cmd_slot;
        if (frame_counter) *frame_counter = s_active_deko_presenter->frame_counter;
    } else {
        if (current_slot) *current_slot = -2;
        if (cur_cmd_slot) *cur_cmd_slot = -2;
        if (frame_counter) *frame_counter = 0;
    }
}

RecompD3dPresenterError d3d_backend_create(
    const RecompD3dPresenterConfig *config, RecompD3dPresenter **out_presenter) {
    if (!out_presenter) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;

    printf("[Deko3D] Initializing hardware device and graphics queue...\n");

    RecompD3dPresenter *p = (RecompD3dPresenter *)calloc(1, sizeof(RecompD3dPresenter));
    if (!p) return RECOMP_D3D_PRESENTER_OUT_OF_MEMORY;

    clearTexHashTable(p);
    p->guest_width = config && config->width ? config->width : 640;
    p->guest_height = config && config->height ? config->height : 480;
    p->width = 1280;
    p->height = 720;
    p->immediate_present = false;

    // 1. Create Device
    DkDeviceMaker devMaker;
    dkDeviceMakerDefaults(&devMaker);
    devMaker.flags = DkDeviceFlags_OriginUpperLeft | DkDeviceFlags_DepthZeroToOne;
    p->device = dkDeviceCreate(&devMaker);

    // 2. Create Graphics Queue
    DkQueueMaker queueMaker;
    dkQueueMakerDefaults(&queueMaker, p->device);
    queueMaker.flags = DkQueueFlags_Graphics;
    p->queue = dkQueueCreate(&queueMaker);

    // 3. Double-Buffered Command Buffers & CpuCached Dynamic Buffers
    for (uint32_t i = 0; i < NUM_CMD_SLOTS; ++i) {
        DkMemBlockMaker blockMaker;
        dkMemBlockMakerDefaults(&blockMaker, p->device, CMD_BUF_SIZE);
        blockMaker.flags = DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
        p->block_cmd[i] = dkMemBlockCreate(&blockMaker);

        DkCmdBufMaker cmdBufMaker;
        dkCmdBufMakerDefaults(&cmdBufMaker, p->device);
        p->cmdbuf[i] = dkCmdBufCreate(&cmdBufMaker);
        dkCmdBufAddMemory(p->cmdbuf[i], p->block_cmd[i], 0, CMD_BUF_SIZE);

        DkMemBlockMaker dynMaker;
        dkMemBlockMakerDefaults(&dynMaker, p->device, DYN_BUF_SIZE);
        dynMaker.flags = DkMemBlockFlags_CpuCached | DkMemBlockFlags_GpuCached;
        p->block_dyn[i] = dkMemBlockCreate(&dynMaker);
        p->dyn_cpu[i] = (uint8_t*)dkMemBlockGetCpuAddr(p->block_dyn[i]);
        p->dyn_gpu[i] = dkMemBlockGetGpuAddr(p->block_dyn[i]);
    }
    p->cur_cmd_slot = 0;
    p->dyn_offset = 0;

    // Allocate GPU Query Timestamps Buffer
    {
        DkMemBlockMaker queryBlockMaker;
        dkMemBlockMakerDefaults(&queryBlockMaker, p->device, QUERY_BUF_SIZE);
        queryBlockMaker.flags = DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
        p->block_queries = dkMemBlockCreate(&queryBlockMaker);
        uint8_t *queries_base = (uint8_t*)dkMemBlockGetCpuAddr(p->block_queries);
        DkGpuAddr queries_gpu_base = dkMemBlockGetGpuAddr(p->block_queries);
        for (uint32_t i = 0; i < NUM_CMD_SLOTS; ++i) {
            uint32_t offset = i * MAX_GPU_QUERIES * sizeof(GpuQueryReport);
            p->queries_cpu[i] = (GpuQueryReport*)(queries_base + offset);
            p->queries_gpu[i] = queries_gpu_base + offset;
            p->query_count[i] = 0;
        }
    }

    // 4. Calculate Framebuffer Layout and Allocate Color Targets
    DkImageLayoutMaker imageLayoutMaker;
    dkImageLayoutMakerDefaults(&imageLayoutMaker, p->device);
    imageLayoutMaker.flags = DkImageFlags_UsageRender | DkImageFlags_UsagePresent | DkImageFlags_Usage2DEngine | DkImageFlags_HwCompression;
    imageLayoutMaker.format = DkImageFormat_RGBA8_Unorm;
    imageLayoutMaker.dimensions[0] = p->width;
    imageLayoutMaker.dimensions[1] = p->height;

    DkImageLayout framebufferLayout;
    dkImageLayoutInitialize(&framebufferLayout, &imageLayoutMaker);

    uint32_t framebufferSize = dkImageLayoutGetSize(&framebufferLayout);
    uint32_t framebufferAlign = dkImageLayoutGetAlignment(&framebufferLayout);
    framebufferSize = (framebufferSize + framebufferAlign - 1) & ~(framebufferAlign - 1);

    DkMemBlockMaker fbMemBlockMaker;
    dkMemBlockMakerDefaults(&fbMemBlockMaker, p->device, NUM_FRAMEBUFFERS * framebufferSize);
    fbMemBlockMaker.flags = DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image;
    p->block_framebuffers = dkMemBlockCreate(&fbMemBlockMaker);

    for (unsigned i = 0; i < NUM_FRAMEBUFFERS; i++) {
        p->framebuffer_ptrs[i] = &p->framebuffers[i];
        dkImageInitialize(&p->framebuffers[i], &framebufferLayout, p->block_framebuffers, i * framebufferSize);
        dkImageViewDefaults(&p->framebuffer_views[i], &p->framebuffers[i]);
        p->target_ptrs[i] = &p->framebuffer_views[i];
    }

    // 5. Depth Buffer (Z24S8)
    DkImageLayoutMaker depthLayoutMaker;
    dkImageLayoutMakerDefaults(&depthLayoutMaker, p->device);
    depthLayoutMaker.flags = DkImageFlags_UsageRender | DkImageFlags_HwCompression;
    depthLayoutMaker.format = DkImageFormat_Z24S8;
    depthLayoutMaker.dimensions[0] = p->width;
    depthLayoutMaker.dimensions[1] = p->height;

    DkImageLayout depthLayout;
    dkImageLayoutInitialize(&depthLayout, &depthLayoutMaker);

    uint32_t depthSize = dkImageLayoutGetSize(&depthLayout);
    uint32_t depthAlign = dkImageLayoutGetAlignment(&depthLayout);
    depthSize = (depthSize + depthAlign - 1) & ~(depthAlign - 1);

    DkMemBlockMaker depthMemBlockMaker;
    dkMemBlockMakerDefaults(&depthMemBlockMaker, p->device, depthSize);
    depthMemBlockMaker.flags = DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image;
    p->block_depth = dkMemBlockCreate(&depthMemBlockMaker);

    dkImageInitialize(&p->depth_image, &depthLayout, p->block_depth, 0);
    dkImageViewDefaults(&p->depth_view, &p->depth_image);

    // 6. Code Memory Block & Shaders
    DkMemBlockMaker codeMaker;
    dkMemBlockMakerDefaults(&codeMaker, p->device, CODE_BUF_SIZE);
    codeMaker.flags = DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached | DkMemBlockFlags_Code;
    p->block_code = dkMemBlockCreate(&codeMaker);

    uint8_t *code_cpu = (uint8_t*)dkMemBlockGetCpuAddr(p->block_code);
    uint32_t code_offset = 0;

    // Vertex Shader (Basic / CPU fallback)
    memcpy(code_cpu + code_offset, sc2_basic_vsh_dksh, sizeof(sc2_basic_vsh_dksh));
    DkShaderMaker vshMaker;
    dkShaderMakerDefaults(&vshMaker, p->block_code, code_offset);
    dkShaderInitialize(&p->vsh, &vshMaker);
    code_offset += (sizeof(sc2_basic_vsh_dksh) + DK_SHADER_CODE_ALIGNMENT - 1) & ~(DK_SHADER_CODE_ALIGNMENT - 1);

    // Vertex Shader (Fixed-Function GPU)
    memcpy(code_cpu + code_offset, sc2_ff_vsh_dksh, sizeof(sc2_ff_vsh_dksh));
    DkShaderMaker vshFFMaker;
    dkShaderMakerDefaults(&vshFFMaker, p->block_code, code_offset);
    dkShaderInitialize(&p->vsh_ff, &vshFFMaker);
    code_offset += (sizeof(sc2_ff_vsh_dksh) + DK_SHADER_CODE_ALIGNMENT - 1) & ~(DK_SHADER_CODE_ALIGNMENT - 1);

    // Fragment Shader (Null for pure depth/stencil passes with color_mask == 0 and no alpha test)
    memcpy(code_cpu + code_offset, sc2_null_fsh_dksh, sizeof(sc2_null_fsh_dksh));
    DkShaderMaker fshNullMaker;
    dkShaderMakerDefaults(&fshNullMaker, p->block_code, code_offset);
    dkShaderInitialize(&p->fsh_null, &fshNullMaker);
    code_offset += (sizeof(sc2_null_fsh_dksh) + DK_SHADER_CODE_ALIGNMENT - 1) & ~(DK_SHADER_CODE_ALIGNMENT - 1);

    // Fragment Shader (Null Alpha Test for depth/stencil passes with color_mask == 0 and alpha test)
    memcpy(code_cpu + code_offset, sc2_null_alpha_fsh_dksh, sizeof(sc2_null_alpha_fsh_dksh));
    DkShaderMaker fshNullAlphaMaker;
    dkShaderMakerDefaults(&fshNullAlphaMaker, p->block_code, code_offset);
    dkShaderInitialize(&p->fsh_null_alpha, &fshNullAlphaMaker);
    code_offset += (sizeof(sc2_null_alpha_fsh_dksh) + DK_SHADER_CODE_ALIGNMENT - 1) & ~(DK_SHADER_CODE_ALIGNMENT - 1);

    // Fragment Shader (Fast 1-Stage Fixed Function)
    memcpy(code_cpu + code_offset, sc2_color_fsh_dksh, sizeof(sc2_color_fsh_dksh));
    DkShaderMaker fshColorMaker;
    dkShaderMakerDefaults(&fshColorMaker, p->block_code, code_offset);
    dkShaderInitialize(&p->fsh_color, &fshColorMaker);
    code_offset += (sizeof(sc2_color_fsh_dksh) + DK_SHADER_CODE_ALIGNMENT - 1) & ~(DK_SHADER_CODE_ALIGNMENT - 1);

    // Fragment Shader (Fast Direct Combiner for SC2 standard materials)
    memcpy(code_cpu + code_offset, sc2_comb_fast_fsh_dksh, sizeof(sc2_comb_fast_fsh_dksh));
    DkShaderMaker fshCombFastMaker;
    dkShaderMakerDefaults(&fshCombFastMaker, p->block_code, code_offset);
    dkShaderInitialize(&p->fsh_comb_fast, &fshCombFastMaker);
    code_offset += (sizeof(sc2_comb_fast_fsh_dksh) + DK_SHADER_CODE_ALIGNMENT - 1) & ~(DK_SHADER_CODE_ALIGNMENT - 1);

    // Fragment Shader (Specialized 1-or-2-Stage Combiner)
    memcpy(code_cpu + code_offset, sc2_comb2_fsh_dksh, sizeof(sc2_comb2_fsh_dksh));
    DkShaderMaker fshComb2Maker;
    dkShaderMakerDefaults(&fshComb2Maker, p->block_code, code_offset);
    dkShaderInitialize(&p->fsh_comb2, &fshComb2Maker);
    code_offset += (sizeof(sc2_comb2_fsh_dksh) + DK_SHADER_CODE_ALIGNMENT - 1) & ~(DK_SHADER_CODE_ALIGNMENT - 1);

    // Fragment Shader (General Fixed-Function + Register Combiners)
    memcpy(code_cpu + code_offset, sc2_texture_fsh_dksh, sizeof(sc2_texture_fsh_dksh));
    DkShaderMaker fshTexMaker;
    dkShaderMakerDefaults(&fshTexMaker, p->block_code, code_offset);
    dkShaderInitialize(&p->fsh_tex, &fshTexMaker);

    // 7. Descriptor Pool & Texture Memory Pool
    DkMemBlockMaker descMaker;
    dkMemBlockMakerDefaults(&descMaker, p->device, DESC_BUF_SIZE);
    descMaker.flags = DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
    p->block_desc = dkMemBlockCreate(&descMaker);
    uint8_t *desc_cpu = (uint8_t*)dkMemBlockGetCpuAddr(p->block_desc);
    DkGpuAddr desc_gpu = dkMemBlockGetGpuAddr(p->block_desc);

    p->img_descs_cpu = (DkImageDescriptor*)desc_cpu;
    p->img_descs_gpu = desc_gpu;
    uint32_t smp_offset = MAX_IMAGE_DESCS * sizeof(DkImageDescriptor);
    p->smp_descs_cpu = (DkSamplerDescriptor*)(desc_cpu + smp_offset);
    p->smp_descs_gpu = desc_gpu + smp_offset;

    for (uint32_t s = 0; s < 16; ++s) {
        DkSampler smp;
        dkSamplerDefaults(&smp);
        smp.minFilter = DkFilter_Linear;
        smp.magFilter = DkFilter_Linear;
        smp.mipFilter = DkMipFilter_None;
        auto mapWrap = [](uint32_t code) -> DkWrapMode {
            if (code == 1u) return DkWrapMode_ClampToEdge;
            if (code == 2u) return DkWrapMode_MirroredRepeat;
            return DkWrapMode_Repeat;
        };
        smp.wrapMode[0] = mapWrap(s & 3u);
        smp.wrapMode[1] = mapWrap((s >> 2u) & 3u);
        smp.wrapMode[2] = DkWrapMode_Repeat;
        dkSamplerDescriptorInitialize(&p->smp_descs_cpu[s], &smp);
    }

    DkMemBlockMaker texMaker;
    dkMemBlockMakerDefaults(&texMaker, p->device, TEX_POOL_SIZE);
    texMaker.flags = DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image;
    p->block_tex = dkMemBlockCreate(&texMaker);
    p->tex_pool_offset = 0;

    // Create 1x1 white fallback texture at slot 0 and upload 0xFFFFFFFF
    {
        DkImageLayoutMaker wlm;
        dkImageLayoutMakerDefaults(&wlm, p->device);
        wlm.flags = DkImageFlags_Usage2DEngine;
        wlm.format = DkImageFormat_BGRA8_Unorm;
        wlm.dimensions[0] = 1;
        wlm.dimensions[1] = 1;
        DkImageLayout wlayout;
        dkImageLayoutInitialize(&wlayout, &wlm);
        uint32_t wsize = (uint32_t)dkImageLayoutGetSize(&wlayout);
        uint32_t walign = dkImageLayoutGetAlignment(&wlayout);
        p->tex_pool_offset = (wsize + walign - 1) & ~(walign - 1);
        if (p->tex_pool_offset < 4096) p->tex_pool_offset = 4096;

        dkImageInitialize(&p->white_image, &wlayout, p->block_tex, 0);
        dkImageViewDefaults(&p->white_view, &p->white_image);
        dkImageDescriptorInitialize(&p->img_descs_cpu[0], &p->white_view, false, false);

        *(uint32_t*)p->dyn_cpu[0] = 0xFFFFFFFFu;
        dkMemBlockFlushCpuCache(p->block_dyn[0], 0, 256);
        DkCopyBuf copySrc = { p->dyn_gpu[0], 4, 1 };
        DkImageRect copyRect = { 0, 0, 0, 1, 1, 1 };
        dkCmdBufCopyBufferToImage(p->cmdbuf[0], &copySrc, &p->white_view, &copyRect, 0);
        dkCmdBufBarrier(p->cmdbuf[0], DkBarrier_Full, DkInvalidateFlags_Image | DkInvalidateFlags_Descriptors);
        dkQueueSubmitCommands(p->queue, dkCmdBufFinishList(p->cmdbuf[0]));
        dkQueueWaitIdle(p->queue);
        dkCmdBufClear(p->cmdbuf[0]);
        dkCmdBufAddMemory(p->cmdbuf[0], p->block_cmd[0], 0, CMD_BUF_SIZE);
    }

    // 8b. GPU Copy Pool (for CopyRects / CopyMainColor / backbuffer sampling)
    {
        DkMemBlockMaker gcMaker;
        dkMemBlockMakerDefaults(&gcMaker, p->device, GPU_COPY_POOL_SIZE);
        gcMaker.flags = DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image;
        p->block_gpu_copies = dkMemBlockCreate(&gcMaker);

        DkImageLayoutMaker slm;
        dkImageLayoutMakerDefaults(&slm, p->device);
        slm.flags = DkImageFlags_Usage2DEngine | DkImageFlags_UsageRender;
        slm.format = DkImageFormat_RGBA8_Unorm;
        slm.dimensions[0] = p->width;
        slm.dimensions[1] = p->height;
        DkImageLayout slayout;
        dkImageLayoutInitialize(&slayout, &slm);
        uint32_t ssize = (uint32_t)dkImageLayoutGetSize(&slayout);
        uint32_t salign = dkImageLayoutGetAlignment(&slayout);
        p->gpu_copy_pool_offset = (ssize + salign - 1) & ~(salign - 1);
        dkImageInitialize(&p->backbuffer_snap_image, &slayout, p->block_gpu_copies, 0);
        dkImageViewDefaults(&p->backbuffer_snap_view, &p->backbuffer_snap_image);
        dkImageDescriptorInitialize(&p->img_descs_cpu[MAX_TEX_CACHE + 1], &p->backbuffer_snap_view, false, false);
        p->last_presented_slot = -1;
        p->next_gpu_copy_slot = 0;
        memset(p->gpu_copies, 0, sizeof(p->gpu_copies));
    }

    // 9. Swapchain (double-buffered, lazy image acquisition per frame)
    DkSwapchainMaker swapMaker;
    dkSwapchainMakerDefaults(&swapMaker, p->device, nwindowGetDefault(), p->framebuffer_ptrs, NUM_FRAMEBUFFERS);
    p->swapchain = dkSwapchainCreate(&swapMaker);
    p->current_slot = -1;

    printf("[Deko3D] Low-Level 3D Renderer Ready: Guest=%ux%u -> Host=%ux%u (TexPool=160MB, Cache=2048, HW-BC1/2/3)\n",
        p->guest_width, p->guest_height, p->width, p->height);

    s_active_deko_presenter = p;
    *out_presenter = p;
    return RECOMP_D3D_PRESENTER_OK;
}

static bool beginFrameIfNeeded(RecompD3dPresenter *p) {
    if (p->current_slot >= 0) return true;
    if (!appletMainLoop()) return false;

    uint32_t slot = p->cur_cmd_slot;
    if (p->slot_has_fence[slot]) {
        PerfScope ps(&g_perf.t_fence);
        dkFenceWait(&p->slot_fences[slot], -1);

        // Harvest GPU hardware timestamps from previous frame using this slot
        uint32_t count = p->query_count[slot];
        if (count > 1) {
            for (uint32_t q = 0; q < count - 1; ++q) {
                uint64_t t0 = p->queries_cpu[slot][q].timestamp;
                uint64_t t1 = p->queries_cpu[slot][q + 1].timestamp;
                if (t1 >= t0) {
                    uint64_t dt_ns = dkTimestampToNs(t1 - t0);
                    uint16_t cat = p->query_cats[slot][q];
                    if (cat < GPU_CAT_COUNT) {
                        g_perf.gpu_cat_ns[cat] += dt_ns;
                        g_perf.gpu_cat_draws[cat]++;
                        g_perf.gpu_total_ns += dt_ns;

                        const GpuDrawMeta &m = p->query_meta[slot][q];
                        if (m.fsh_type < 6) {
                            g_perf.gpu_fsh_ns[m.fsh_type] += dt_ns;
                            g_perf.gpu_fsh_draws[m.fsh_type]++;
                        }

                        double d_ms = (double)dt_ns / 1000000.0;
                        if (dt_ns > 10 * 1000000ULL) {
                            printf("[HEAVY_DRAW] %.2fms | cat=%u fsh=%u vtx=%u idx=%u tex=%ux%u (fmt=0x%x mips=%u) ctrl=0x%08x fin0=0x%08x fin1=0x%08x tm=0x%08x | s0: cin=0x%08x cout=0x%08x ain=0x%08x aout=0x%08x | s1: cin=0x%08x cout=0x%08x ain=0x%08x aout=0x%08x\n",
                                   d_ms, cat, m.fsh_type, m.vtx_count, m.idx_count, m.tex_w, m.tex_h, m.tex_fmt, m.tex_mips,
                                   m.c_control, m.c_final0, m.c_final1, m.c_tex_modes,
                                   m.c_in[0], m.c_out[0], m.a_in[0], m.a_out[0],
                                   m.c_in[1], m.c_out[1], m.a_in[1], m.a_out[1]);
                        }
                        for (int k = 0; k < 5; ++k) {
                            if (d_ms > g_perf.worst_draws[k].ms) {
                                for (int j = 4; j > k; --j) g_perf.worst_draws[j] = g_perf.worst_draws[j - 1];
                                g_perf.worst_draws[k] = {
                                    .ms = d_ms,
                                    .cat = cat,
                                    .fsh = m.fsh_type,
                                    .vtx = m.vtx_count,
                                    .idx = m.idx_count,
                                    .tw = m.tex_w,
                                    .th = m.tex_h,
                                    .fmt = m.tex_fmt,
                                    .mips = m.tex_mips,
                                    .c_control = m.c_control,
                                    .c_final0 = m.c_final0,
                                    .c_final1 = m.c_final1,
                                    .c_tex_modes = m.c_tex_modes,
                                    .c_in = { m.c_in[0], m.c_in[1] },
                                    .c_out = { m.c_out[0], m.c_out[1] },
                                    .a_in = { m.a_in[0], m.a_in[1] },
                                    .a_out = { m.a_out[0], m.a_out[1] }
                                };
                                break;
                            }
                        }
                    }
                }
            }
            g_perf.gpu_profiled_frames++;
        }
        p->query_count[slot] = 0;
    }
    p->dyn_offset = 0;

    {
        PerfScope ps(&g_perf.t_acquire);
        p->current_slot = dkQueueAcquireImage(p->queue, p->swapchain);
    }
    if (p->current_slot < 0) return false;

    DkCmdBuf cmd = p->cmdbuf[slot];
    dkCmdBufClear(cmd);
    dkCmdBufAddMemory(cmd, p->block_cmd[slot], 0, CMD_BUF_SIZE);
    dkCmdBufBindRenderTargets(cmd, &p->target_ptrs[p->current_slot], 1, &p->depth_view);
    dkCmdBufBindImageDescriptorSet(cmd, p->img_descs_gpu, MAX_IMAGE_DESCS);
    dkCmdBufBindSamplerDescriptorSet(cmd, p->smp_descs_gpu, MAX_SAMPLER_DESCS);
    DkViewport viewport = { 0.0f, 0.0f, (float)p->width, (float)p->height, 0.0f, 1.0f };
    DkScissor scissor = { 0, 0, p->width, p->height };
    dkCmdBufSetViewports(cmd, 0, &viewport, 1);
    dkCmdBufSetScissors(cmd, 0, &scissor, 1);

    // Initial frame hardware timestamp (T0)
    dkCmdBufReportCounter(cmd, DkCounter_Timestamp, p->queries_gpu[slot]);
    p->query_count[slot] = 1;

    return true;
}

static void drawFpsOverlay(RecompD3dPresenter *p, DkCmdBuf cmd, double fps, double ms) {
    if (p->current_slot < 0) return;

    char text[64];
    snprintf(text, sizeof(text), "FPS: %.1f (%.1f ms) [%s]",
             fps, ms, Switch_IsGpuVtxEnabled() ? "GPU" : "CPU");

    const uint32_t max_vtx = 2048;
    const uint32_t vtx_size = max_vtx * sizeof(DekoVertex);
    const uint32_t ubo_size = sizeof(FragBlock);

    p->dyn_offset = (p->dyn_offset + 255u) & ~255u;
    if (p->dyn_offset + vtx_size + ubo_size + 1024u > DYN_BUF_SIZE) {
        return;
    }

    uint32_t slot = p->cur_cmd_slot;
    uint8_t *dyn_cpu = (uint8_t*)p->dyn_cpu[slot];
    DkGpuAddr dyn_gpu = p->dyn_gpu[slot];

    uint32_t ubo_off = p->dyn_offset;
    p->dyn_offset += (ubo_size + 255u) & ~255u;

    uint32_t vtx_off = p->dyn_offset;
    p->dyn_offset += (vtx_size + 255u) & ~255u;

    FragBlock *fbo = (FragBlock*)(dyn_cpu + ubo_off);
    memset(fbo, 0, sizeof(*fbo));
    fbo->u_header[0] = RECOMP_D3D_PIXEL_FIXED_FUNCTION;
    fbo->u_header[1] = 0; // stage_count = 0 (in_color)

    DekoVertex *vtx_dest = (DekoVertex*)(dyn_cpu + vtx_off);
    uint32_t vtx_count = 0;

    auto emit_quad = [&](float px0, float py0, float px1, float py1,
                         float r, float g, float b, float a) {
        if (vtx_count + 6 > max_vtx) return;
        float x0 = (px0 / (float)p->width) * 2.0f - 1.0f;
        float x1 = (px1 / (float)p->width) * 2.0f - 1.0f;
        float y0 = 1.0f - (py0 / (float)p->height) * 2.0f;
        float y1 = 1.0f - (py1 / (float)p->height) * 2.0f;

        DekoVertex v[6];
        for (int k = 0; k < 6; k++) {
            v[k].z = 0.0f; v[k].w = 1.0f;
            v[k].r = r; v[k].g = g; v[k].b = b; v[k].a = a;
            v[k].sr = 0.0f; v[k].sg = 0.0f; v[k].sb = 0.0f; v[k].fog = 1.0f;
            v[k].u0 = 0.0f; v[k].v0 = 0.0f; v[k].u1 = 0.0f; v[k].v1 = 0.0f;
            v[k].u2 = 0.0f; v[k].v2 = 0.0f; v[k].u3 = 0.0f; v[k].v3 = 0.0f;
        }
        v[0].x = x0; v[0].y = y0;
        v[1].x = x1; v[1].y = y0;
        v[2].x = x0; v[2].y = y1;

        v[3].x = x1; v[3].y = y0;
        v[4].x = x1; v[4].y = y1;
        v[5].x = x0; v[5].y = y1;

        memcpy(&vtx_dest[vtx_count], v, sizeof(v));
        vtx_count += 6;
    };

    const float scale = 2.5f;
    const float char_step = 18.0f;
    const size_t text_len = strlen(text);
    const float box_w = (float)text_len * char_step + 24.0f;
    const float box_h = 32.0f;
    const float start_x = 24.0f;
    const float start_y = 24.0f;

    // Background pill (dark translucent)
    emit_quad(start_x, start_y, start_x + box_w, start_y + box_h, 0.05f, 0.05f, 0.08f, 0.80f);

    // Text color based on FPS
    float fg_r = 0.2f, fg_g = 1.0f, fg_b = 0.3f, fg_a = 1.0f;
    if (fps < 25.0) {
        fg_r = 1.0f; fg_g = 0.35f; fg_b = 0.35f;
    } else if (fps < 50.0) {
        fg_r = 1.0f; fg_g = 0.95f; fg_b = 0.2f;
    }

    float cur_x = start_x + 12.0f;
    float cur_y = start_y + 6.0f;

    for (size_t i = 0; i < text_len; ++i) {
        unsigned char c = (unsigned char)text[i];
        if (c >= 32 && c <= 126) {
            const uint8_t *glyph = s_font8x8[c - 32];
            for (int row = 0; row < 8; ++row) {
                uint8_t b = glyph[row];
                if (!b) continue;
                int col = 0;
                while (col < 8) {
                    if (b & (1 << (7 - col))) {
                        int start_col = col;
                        while (col < 8 && (b & (1 << (7 - col)))) {
                            col++;
                        }
                        emit_quad(cur_x + (float)start_col * scale,
                                  cur_y + (float)row * scale,
                                  cur_x + (float)col * scale,
                                  cur_y + (float)(row + 1) * scale,
                                  fg_r, fg_g, fg_b, fg_a);
                    } else {
                        col++;
                    }
                }
            }
        }
        cur_x += char_step;
    }

    if (vtx_count == 0) return;

    dkCmdBufBindRenderTargets(cmd, &p->target_ptrs[p->current_slot], 1, nullptr);

    DkViewport viewport = { 0.0f, 0.0f, (float)p->width, (float)p->height, 0.0f, 1.0f };
    dkCmdBufSetViewports(cmd, 0, &viewport, 1);
    DkScissor scissor = { 0, 0, p->width, p->height };
    dkCmdBufSetScissors(cmd, 0, &scissor, 1);

    DkShader const* shaders[] = { &p->vsh, &p->fsh_color };
    dkCmdBufBindShaders(cmd, DkStageFlag_GraphicsMask, shaders, 2);

    DkResHandle tex_handles[1] = { dkMakeTextureHandle(0, 0) };
    dkCmdBufBindTextures(cmd, DkStage_Fragment, 0, tex_handles, 1);
    dkCmdBufBindUniformBuffer(cmd, DkStage_Fragment, 0, dyn_gpu + ubo_off, sizeof(FragBlock));

    DkRasterizerState rasterizerState;
    dkRasterizerStateDefaults(&rasterizerState);
    rasterizerState.cullMode = DkFace_None;
    dkCmdBufBindRasterizerState(cmd, &rasterizerState);

    DkDepthStencilState depthState;
    dkDepthStencilStateDefaults(&depthState);
    depthState.depthTestEnable = false;
    depthState.depthWriteEnable = false;
    depthState.depthCompareOp = DkCompareOp_Always;
    dkCmdBufBindDepthStencilState(cmd, &depthState);

    DkBlendState blendState;
    dkBlendStateDefaults(&blendState);
    blendState.srcColorBlendFactor = DkBlendFactor_SrcAlpha;
    blendState.dstColorBlendFactor = DkBlendFactor_InvSrcAlpha;
    blendState.srcAlphaBlendFactor = DkBlendFactor_One;
    blendState.dstAlphaBlendFactor = DkBlendFactor_Zero;
    dkCmdBufBindBlendStates(cmd, 0, &blendState, 1);

    DkColorState colorState;
    dkColorStateDefaults(&colorState);
    colorState.blendEnableMask = 0x01;
    dkCmdBufBindColorState(cmd, &colorState);

    DkVtxAttribState attribState[5];
    initVtxAttribs(attribState);
    dkCmdBufBindVtxAttribState(cmd, attribState, 5);
    dkCmdBufBindVtxBufferState(cmd, s_bufferState, 1);
    dkCmdBufBindVtxBuffer(cmd, 0, dyn_gpu + vtx_off, vtx_count * sizeof(DekoVertex));

    dkCmdBufDraw(cmd, DkPrimitive_Triangles, vtx_count, 1, 0, 0);
}

RecompD3dPresenterError d3d_backend_submit(
    RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *command) {
    if (!presenter || !command) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;

    if (!appletMainLoop()) {
        return RECOMP_D3D_PRESENTER_CLOSED;
    }

    PerfScope perf_backend(&g_perf.t_backend);
    pumpAudioIfDue();

    if (command->type == RECOMP_D3D_PRESENTER_COMMAND_CLEAR) {
        if (command->data.clear.target.offscreen) {
            return RECOMP_D3D_PRESENTER_OK;
        }
        if (!beginFrameIfNeeded(presenter)) {
            return RECOMP_D3D_PRESENTER_CLOSED;
        }
        uint32_t slot = presenter->cur_cmd_slot;
        DkCmdBuf cmd = presenter->cmdbuf[slot];

        DkScissor full_scissor = { 0, 0, presenter->width, presenter->height };
        dkCmdBufSetScissors(cmd, 0, &full_scissor, 1);

        if (command->data.clear.clear_color) {
            uint32_t c = command->data.clear.color;
            float a = ((c >> 24) & 0xFF) / 255.0f;
            float r = ((c >> 16) & 0xFF) / 255.0f;
            float g = ((c >> 8) & 0xFF) / 255.0f;
            float b = (c & 0xFF) / 255.0f;
            dkCmdBufClearColorFloat(cmd, 0, DkColorMask_RGBA, r, g, b, a);
        }
        if (!command->data.clear.target.no_depth &&
            (command->data.clear.clear_depth || command->data.clear.clear_stencil)) {
            dkCmdBufClearDepthStencil(cmd,
                command->data.clear.clear_depth,
                command->data.clear.z,
                command->data.clear.clear_stencil ? 0xFF : 0x00,
                command->data.clear.stencil);
        }

        if (presenter->query_count[slot] < MAX_GPU_QUERIES) {
            uint32_t q = presenter->query_count[slot];
            presenter->query_cats[slot][q - 1] = GPU_CAT_CLEAR;
            dkCmdBufReportCounter(cmd, DkCounter_Timestamp, presenter->queries_gpu[slot] + q * sizeof(GpuQueryReport));
            presenter->query_count[slot] = q + 1;
        }
    } else if (command->type == RECOMP_D3D_PRESENTER_COMMAND_DRAW) {
        const RecompD3dPresenterDrawCommand &draw = command->data.draw;
        PerfScope perf_draw(&g_perf.t_draw_total);
        if (draw.target.offscreen) {
            g_perf.fx_offscreen_drops++;
            return RECOMP_D3D_PRESENTER_OK;
        }
        if (!draw.vertex_bytes || draw.vertex_count == 0) {
            g_perf.fx_empty_drops++;
            return RECOMP_D3D_PRESENTER_OK;
        }
        if (!beginFrameIfNeeded(presenter)) {
            return RECOMP_D3D_PRESENTER_CLOSED;
        }
        uint32_t slot = presenter->cur_cmd_slot;
        DkCmdBuf cmd = presenter->cmdbuf[slot];
        uint8_t *dyn_cpu = presenter->dyn_cpu[slot];
        DkGpuAddr dyn_gpu = presenter->dyn_gpu[slot];

        RecompD3dVertexLayout layout{};
        if (!recomp_d3d_fvf_layout(draw.fvf, &layout)) {
            g_perf.fx_bad_fvf_drops++;
            return RECOMP_D3D_PRESENTER_OK;
        }

        auto addrCode = [](uint32_t addr) -> uint32_t {
            if (addr == 2u) return 2u; // MIRROR
            if (addr >= 3u && addr <= 5u) return 1u; // CLAMP / BORDER / CLAMPTOEDGE
            return 0u; // WRAP
        };

        // Lookup/upload stage textures 0..2
        uint64_t perf_tex_t0 = perfNow();
        auto resolveDrawTex = [&](const RecompD3dTextureDesc &desc, const void *bytes, uint32_t byte_count,
                                  const void *pal, uint32_t pal_count, bool force_white,
                                  bool is_backbuffer, bool is_front_buffer) -> uint32_t {
            if (force_white) return 0;
            if (is_front_buffer && presenter->last_presented_slot >= 0) {
                uint32_t cslot = presenter->cur_cmd_slot;
                DkCmdBuf ccmd = presenter->cmdbuf[cslot];
                DkImageRect rect = { 0, 0, 0, presenter->width, presenter->height, 1 };
                dkCmdBufBarrier(ccmd, DkBarrier_Full, DkInvalidateFlags_Image);
                dkCmdBufBlitImage(ccmd, &presenter->framebuffer_views[presenter->last_presented_slot], &rect,
                                  &presenter->backbuffer_snap_view, &rect, DkBlitFlag_FilterNearest, 0);
                dkCmdBufBarrier(ccmd, DkBarrier_Full, DkInvalidateFlags_Descriptors | DkInvalidateFlags_Image);
                return MAX_TEX_CACHE + 1;
            }
            if (is_backbuffer && presenter->current_slot >= 0) {
                uint32_t cslot = presenter->cur_cmd_slot;
                DkCmdBuf ccmd = presenter->cmdbuf[cslot];
                DkImageRect rect = { 0, 0, 0, presenter->width, presenter->height, 1 };
                dkCmdBufBarrier(ccmd, DkBarrier_Full, DkInvalidateFlags_Image);
                dkCmdBufBlitImage(ccmd, &presenter->framebuffer_views[presenter->current_slot], &rect,
                                  &presenter->backbuffer_snap_view, &rect, DkBlitFlag_FilterNearest, 0);
                dkCmdBufBarrier(ccmd, DkBarrier_Full, DkInvalidateFlags_Descriptors | DkInvalidateFlags_Image);
                return MAX_TEX_CACHE + 1;
            }
            uint32_t gpu_slot = lookupGpuCopySlot(presenter, desc.data, desc.width, desc.height);
            if (gpu_slot != 0) return gpu_slot;
            return lookupOrUploadSingleTexture(presenter, desc, bytes, byte_count, pal, pal_count, false);
        };

        uint32_t tex_handles[3] = {};
        if (draw.pixel_program.kind != RECOMP_D3D_PIXEL_NONE) {
            for (uint32_t s = 0; s < 3u; ++s) {
                uint32_t tslot = 0;
                uint32_t smp_id = 0;
                const auto &st_tex = draw.stage_textures[s];
                if (draw.pixel_program.stages[s].sampled) {
                    if (st_tex.bytes && st_tex.byte_count > 0) {
                        tslot = resolveDrawTex(st_tex.desc, st_tex.bytes, st_tex.byte_count,
                                               st_tex.palette, st_tex.palette_byte_count, st_tex.white,
                                               s == 0 && draw.texture_is_backbuffer, s == 0 && draw.texture_is_front_buffer);
                        smp_id = addrCode(st_tex.address_u) | (addrCode(st_tex.address_v) << 2u);
                    } else if (s == 0 && draw.has_texture && (draw.texture_bytes || draw.texture_is_backbuffer || draw.texture_is_front_buffer)) {
                        tslot = resolveDrawTex(draw.texture, draw.texture_bytes, draw.texture_byte_count,
                                               draw.palette_bytes, draw.palette_byte_count, false,
                                               draw.texture_is_backbuffer, draw.texture_is_front_buffer);
                        smp_id = addrCode(draw.address_u) | (addrCode(draw.address_v) << 2u);
                    }
                }
                tex_handles[s] = dkMakeTextureHandle(tslot, smp_id);
            }
        } else {
            uint32_t tslot = 0;
            uint32_t smp_id = 0;
            if (draw.has_texture && (draw.texture_bytes || draw.texture_is_backbuffer || draw.texture_is_front_buffer)) {
                tslot = resolveDrawTex(draw.texture, draw.texture_bytes, draw.texture_byte_count,
                                       draw.palette_bytes, draw.palette_byte_count, false,
                                       draw.texture_is_backbuffer, draw.texture_is_front_buffer);
                smp_id = addrCode(draw.address_u) | (addrCode(draw.address_v) << 2u);
            }
            tex_handles[0] = dkMakeTextureHandle(tslot, smp_id);
            tex_handles[1] = dkMakeTextureHandle(0, 0);
            tex_handles[2] = dkMakeTextureHandle(0, 0);
        }

        g_perf.t_tex += perfNow() - perf_tex_t0;

        const uint32_t stride = draw.vertex_stride ? draw.vertex_stride : layout.stride;
        const float guest_w = (float)presenter->guest_width;
        const float guest_h = (float)presenter->guest_height;
        const bool sub_viewport = draw.has_transform && !draw.program_count && draw.viewport[2] > 0.0f;
        const float vp_w = sub_viewport ? draw.viewport[2] : guest_w;
        const float vp_h = sub_viewport ? draw.viewport[3] : guest_h;
        const float dx_offset = (vp_w > 0.0f) ? (2.0f * draw.screen_space_offset[0] / vp_w) : 0.0f;
        const float dy_offset = (vp_h > 0.0f) ? (-2.0f * draw.screen_space_offset[1] / vp_h) : 0.0f;

        const bool has_normal = (layout.normal_offset != RECOMP_D3D_FVF_ABSENT && layout.normal_offset + 12 <= stride);
        const bool has_diffuse = (layout.diffuse_offset != RECOMP_D3D_FVF_ABSENT && layout.diffuse_offset + 4 <= stride);
        const bool has_fog = (draw.general_fog[0][0] > 0.5f);
        const bool range_fog = (draw.general_fog[2][1] > 0.5f);
        const bool is_skin = !layout.pretransformed && draw.program_count == 0 && draw.blend_weight_count > 0 && layout.blend_weight_count > 0;

        const bool use_gpu_vtx = Switch_IsGpuVtxEnabled() &&
                                 !layout.pretransformed &&
                                 draw.program_count == 0 &&
                                 !is_skin &&
                                 layout.texcoord_count <= 2;

        uint32_t vtx_size = draw.vertex_count * (use_gpu_vtx ? sizeof(DekoVertexFF) : sizeof(DekoVertex));
        uint32_t idx_size = (draw.index_bytes && draw.index_count > 0) ? (draw.index_count * sizeof(uint16_t)) : 0;
        uint32_t ubo_size = sizeof(FragBlock);
        uint32_t vbo_size = use_gpu_vtx ? sizeof(VtxBlock) : 0;

        presenter->dyn_offset = (presenter->dyn_offset + 255u) & ~255u;
        if (presenter->dyn_offset + vtx_size + idx_size + ubo_size + vbo_size + 1024u > DYN_BUF_SIZE) {
            return RECOMP_D3D_PRESENTER_OK;
        }

        uint32_t ubo_offset = presenter->dyn_offset;
        presenter->dyn_offset += (ubo_size + 255u) & ~255u;

        uint32_t vbo_offset = 0;
        if (use_gpu_vtx) {
            vbo_offset = presenter->dyn_offset;
            presenter->dyn_offset += (vbo_size + 255u) & ~255u;
        }

        uint32_t vtx_offset = presenter->dyn_offset;
        presenter->dyn_offset += (vtx_size + 255u) & ~255u;

        uint32_t idx_offset = presenter->dyn_offset;
        if (idx_size > 0) {
            presenter->dyn_offset += (idx_size + 255u) & ~255u;
        }

        // 1. Fragment Uniforms
        FragBlock *fbo = (FragBlock*)(dyn_cpu + ubo_offset);
        bool is_fast_path = false;
        populateFragBlock(draw, fbo, is_fast_path);

        if (use_gpu_vtx) {
            VtxBlock *vbo = (VtxBlock*)(dyn_cpu + vbo_offset);
            populateVtxBlock(draw, dx_offset, dy_offset, has_normal, has_fog, range_fog, vbo);
        }

        ++g_perf.draws; g_perf.verts += draw.vertex_count; g_perf.idx += draw.index_count;
        g_perf.dyn_bytes += (uint64_t)vtx_size;
        if (layout.pretransformed) { ++g_perf.path_pre; g_perf.verts_pre += draw.vertex_count; }
        else if (draw.program_count > 0) { ++g_perf.path_prog; g_perf.verts_prog += draw.vertex_count; }
        else if (is_skin) { ++g_perf.path_skin; g_perf.verts_skin += draw.vertex_count; }
        else { ++g_perf.path_ff; g_perf.verts_ff += draw.vertex_count; }
        if (!layout.pretransformed && draw.program_count == 0 && has_normal && draw.directional.enabled) ++g_perf.path_lit;

        const uint64_t perf_vtx_t0 = perfNow();

        if (use_gpu_vtx) {
            ++g_perf.verts_ff_gpu;
            DekoVertexFF *vtx_dest = (DekoVertexFF*)(dyn_cpu + vtx_offset);
            const uint8_t *src_base = (const uint8_t*)draw.vertex_bytes;
            const uint32_t pos_off = layout.position_offset;
            const uint32_t norm_off = layout.normal_offset;
            const uint32_t diff_off = layout.diffuse_offset;
            const uint32_t uv0_off = layout.texcoord_offset;
            const uint32_t uv1_off = layout.texcoord_offset + 8u;
            const bool has_norm = has_normal;
            const bool has_diff = has_diffuse;
            const bool has_uv0 = (layout.texcoord_offset != RECOMP_D3D_FVF_ABSENT && layout.texcoord_count > 0);
            const bool has_uv1 = (layout.texcoord_offset != RECOMP_D3D_FVF_ABSENT && layout.texcoord_count > 1);

            if (has_norm) {
                if (has_diff) {
                    for (uint32_t i = 0; i < draw.vertex_count; ++i) {
                        const uint8_t *src = src_base + (size_t)i * stride;
                        vtx_dest[i].x = *(const float*)(src + pos_off + 0);
                        vtx_dest[i].y = *(const float*)(src + pos_off + 4);
                        vtx_dest[i].z = *(const float*)(src + pos_off + 8);
                        vtx_dest[i].nx = *(const float*)(src + norm_off + 0);
                        vtx_dest[i].ny = *(const float*)(src + norm_off + 4);
                        vtx_dest[i].nz = *(const float*)(src + norm_off + 8);
                        vtx_dest[i].color = *(const uint32_t*)(src + diff_off);
                        vtx_dest[i].pad = 0.0f;
                        vtx_dest[i].u0 = has_uv0 ? *(const float*)(src + uv0_off + 0) : 0.0f;
                        vtx_dest[i].v0 = has_uv0 ? *(const float*)(src + uv0_off + 4) : 0.0f;
                        vtx_dest[i].u1 = has_uv1 ? *(const float*)(src + uv1_off + 0) : 0.0f;
                        vtx_dest[i].v1 = has_uv1 ? *(const float*)(src + uv1_off + 4) : 0.0f;
                    }
                } else {
                    for (uint32_t i = 0; i < draw.vertex_count; ++i) {
                        const uint8_t *src = src_base + (size_t)i * stride;
                        vtx_dest[i].x = *(const float*)(src + pos_off + 0);
                        vtx_dest[i].y = *(const float*)(src + pos_off + 4);
                        vtx_dest[i].z = *(const float*)(src + pos_off + 8);
                        vtx_dest[i].nx = *(const float*)(src + norm_off + 0);
                        vtx_dest[i].ny = *(const float*)(src + norm_off + 4);
                        vtx_dest[i].nz = *(const float*)(src + norm_off + 8);
                        vtx_dest[i].color = 0xFFFFFFFFu;
                        vtx_dest[i].pad = 0.0f;
                        vtx_dest[i].u0 = has_uv0 ? *(const float*)(src + uv0_off + 0) : 0.0f;
                        vtx_dest[i].v0 = has_uv0 ? *(const float*)(src + uv0_off + 4) : 0.0f;
                        vtx_dest[i].u1 = has_uv1 ? *(const float*)(src + uv1_off + 0) : 0.0f;
                        vtx_dest[i].v1 = has_uv1 ? *(const float*)(src + uv1_off + 4) : 0.0f;
                    }
                }
            } else {
                if (has_diff) {
                    for (uint32_t i = 0; i < draw.vertex_count; ++i) {
                        const uint8_t *src = src_base + (size_t)i * stride;
                        vtx_dest[i].x = *(const float*)(src + pos_off + 0);
                        vtx_dest[i].y = *(const float*)(src + pos_off + 4);
                        vtx_dest[i].z = *(const float*)(src + pos_off + 8);
                        vtx_dest[i].nx = 0.0f;
                        vtx_dest[i].ny = 0.0f;
                        vtx_dest[i].nz = 1.0f;
                        vtx_dest[i].color = *(const uint32_t*)(src + diff_off);
                        vtx_dest[i].pad = 0.0f;
                        vtx_dest[i].u0 = has_uv0 ? *(const float*)(src + uv0_off + 0) : 0.0f;
                        vtx_dest[i].v0 = has_uv0 ? *(const float*)(src + uv0_off + 4) : 0.0f;
                        vtx_dest[i].u1 = has_uv1 ? *(const float*)(src + uv1_off + 0) : 0.0f;
                        vtx_dest[i].v1 = has_uv1 ? *(const float*)(src + uv1_off + 4) : 0.0f;
                    }
                } else {
                    for (uint32_t i = 0; i < draw.vertex_count; ++i) {
                        const uint8_t *src = src_base + (size_t)i * stride;
                        vtx_dest[i].x = *(const float*)(src + pos_off + 0);
                        vtx_dest[i].y = *(const float*)(src + pos_off + 4);
                        vtx_dest[i].z = *(const float*)(src + pos_off + 8);
                        vtx_dest[i].nx = 0.0f;
                        vtx_dest[i].ny = 0.0f;
                        vtx_dest[i].nz = 1.0f;
                        vtx_dest[i].color = 0xFFFFFFFFu;
                        vtx_dest[i].pad = 0.0f;
                        vtx_dest[i].u0 = has_uv0 ? *(const float*)(src + uv0_off + 0) : 0.0f;
                        vtx_dest[i].v0 = has_uv0 ? *(const float*)(src + uv0_off + 4) : 0.0f;
                        vtx_dest[i].u1 = has_uv1 ? *(const float*)(src + uv1_off + 0) : 0.0f;
                        vtx_dest[i].v1 = has_uv1 ? *(const float*)(src + uv1_off + 4) : 0.0f;
                    }
                }
            }
        } else {
            ++g_perf.verts_ff_cpu;
            DekoVertex *vtx_dest = (DekoVertex*)(dyn_cpu + vtx_offset);
            for (uint32_t i = 0; i < draw.vertex_count; i++) {
                const uint8_t *src = (const uint8_t*)draw.vertex_bytes + (size_t)i * stride;
                DekoVertex &dv = vtx_dest[i];

                const float px = *(const float*)(src + layout.position_offset + 0);
                const float py = *(const float*)(src + layout.position_offset + 4);
                const float pz = *(const float*)(src + layout.position_offset + 8);

                float cr = 1.0f, cg = 1.0f, cb = 1.0f, ca = 1.0f;
                if (has_diffuse) {
                    uint32_t c = *(const uint32_t*)(src + layout.diffuse_offset);
                    ca = ((c >> 24) & 0xFF) / 255.0f;
                    cr = ((c >> 16) & 0xFF) / 255.0f;
                    cg = ((c >> 8) & 0xFF) / 255.0f;
                    cb = (c & 0xFF) / 255.0f;
                }

                float uv[4][2] = {};
                if (layout.texcoord_offset != RECOMP_D3D_FVF_ABSENT) {
                    for (uint32_t tc = 0; tc < layout.texcoord_count && tc < 4u; ++tc) {
                        uint32_t tc_off = layout.texcoord_offset + tc * 8u;
                        if (tc_off + 8 <= stride) {
                            uv[tc][0] = *(const float*)(src + tc_off + 0);
                            uv[tc][1] = *(const float*)(src + tc_off + 4);
                        }
                    }
                }

                dv.sr = 0.0f; dv.sg = 0.0f; dv.sb = 0.0f; dv.fog = 1.0f;

                if (layout.pretransformed) {
                    float rhw = *(const float*)(src + layout.position_offset + 12);
                    float w = (std::isfinite(rhw) && std::fabs(rhw) > 1e-6f) ? (1.0f / rhw) : 1.0f;
                    float c00 = 2.0f / guest_w;
                    float c11 = -2.0f / guest_h;
                    float c30 = -1.0f + 0.5f * c00;
                    float c31 = 1.0f + 0.5f * c11;
                    float ndc_x = px * c00 + c30 + dx_offset;
                    float ndc_y = py * c11 + c31 + dy_offset;
                    float ndc_z = std::clamp(pz, 0.0f, 1.0f);
                    dv.x = ndc_x * w;
                    dv.y = ndc_y * w;
                    dv.z = ndc_z * w;
                    dv.w = w;
                    dv.r = cr; dv.g = cg; dv.b = cb; dv.a = ca;
                    dv.u0 = uv[0][0]; dv.v0 = uv[0][1];
                    dv.u1 = uv[1][0]; dv.v1 = uv[1][1];
                    dv.u2 = uv[2][0]; dv.v2 = uv[2][1];
                    dv.u3 = uv[3][0]; dv.v3 = uv[3][1];
                } else if (draw.program_count > 0) {
                    float v_in[16][4]{};
                    v_in[0][0] = px; v_in[0][1] = py; v_in[0][2] = pz; v_in[0][3] = 1.0f;
                    if (has_normal) {
                        v_in[2][0] = *(const float*)(src + layout.normal_offset + 0);
                        v_in[2][1] = *(const float*)(src + layout.normal_offset + 4);
                        v_in[2][2] = *(const float*)(src + layout.normal_offset + 8);
                        v_in[2][3] = 1.0f;
                    }
                    v_in[3][0] = cr; v_in[3][1] = cg; v_in[3][2] = cb; v_in[3][3] = ca;
                    for (uint32_t tc = 0; tc < layout.texcoord_count && tc < 4u; ++tc) {
                        v_in[9 + tc][0] = uv[tc][0];
                        v_in[9 + tc][1] = uv[tc][1];
                        v_in[9 + tc][2] = 0.0f;
                        v_in[9 + tc][3] = 1.0f;
                    }
                    float r12[4] = {}, o3[4] = {}, o4[4] = {}, o9[4] = {}, o10[4] = {}, o11[4] = {}, o12[4] = {};
                    float o5_x = 0.0f;
                    evalVertexProgramCPU(draw, v_in, r12, o3, o4, o5_x, o9, o10, o11, o12);

                    const float sx = draw.program_constants[58][0];
                    const float sy = draw.program_constants[58][1];
                    const float sz = draw.program_constants[58][2];
                    const float ox = draw.program_constants[59][0];
                    const float oy = draw.program_constants[59][1];
                    const float oz = draw.program_constants[59][2];

                    const float c00 = 2.0f / guest_w;
                    const float c11 = -2.0f / guest_h;
                    const float c30 = -1.0f + 0.5f * c00;
                    const float c31 = 1.0f + 0.5f * c11;

                    float w = (std::isfinite(r12[3]) && std::fabs(r12[3]) > 1e-6f) ? r12[3] : 1.0f;
                    float gx = (std::isfinite(sx) && sx != 0.0f) ? ((r12[0] - ox) / sx) : r12[0];
                    float gy = (std::isfinite(sy) && sy != 0.0f) ? ((r12[1] - oy) / sy) : r12[1];
                    float ndc_x = gx * c00 + c30;
                    float ndc_y = gy * c11 + c31;
                    float ndc_z = (std::isfinite(sz) && sz > 0.0f) ? std::clamp((r12[2] - oz) / sz, 0.0f, 1.0f) : 0.5f;

                    dv.x = ndc_x * w;
                    dv.y = ndc_y * w;
                    dv.z = ndc_z * w;
                    dv.w = w;
                    dv.r = std::clamp(o3[0], 0.0f, 1.0f);
                    dv.g = std::clamp(o3[1], 0.0f, 1.0f);
                    dv.b = std::clamp(o3[2], 0.0f, 1.0f);
                    dv.a = std::clamp(o3[3], 0.0f, 1.0f);
                    dv.sr = std::clamp(o4[0], 0.0f, 1.0f);
                    dv.sg = std::clamp(o4[1], 0.0f, 1.0f);
                    dv.sb = std::clamp(o4[2], 0.0f, 1.0f);
                    dv.fog = has_fog ? computeFogFactor(draw, o5_x) : 1.0f;

                    float q0 = (draw.pixel_program.kind == RECOMP_D3D_PIXEL_NONE && std::isfinite(o9[3]) && std::fabs(o9[3]) > 1e-6f) ? o9[3] : 1.0f;
                    dv.u0 = o9[0] / q0;
                    dv.v0 = o9[1] / q0;
                    dv.u1 = o10[0];
                    dv.v1 = o10[1];
                    dv.u2 = o11[0];
                    dv.v2 = o11[1];
                    dv.u3 = o12[0];
                    dv.v3 = o12[1];
                } else {
                    // Fixed-function 3D (single matrix or skinned)
                    float clip[4] = {0, 0, 0, 0};
                    auto mulRow = [](float x, float y, float z, const float m[16], float out[4]) {
                        for (unsigned col = 0; col < 4; ++col) {
                            out[col] = x * m[col] + y * m[4 + col] + z * m[8 + col] + m[12 + col];
                        }
                    };
                    if (draw.blend_weight_count > 0 && layout.blend_weight_count > 0) {
                        const float *weights = (const float*)(src + 12);
                        float rem = 1.0f;
                        for (uint32_t wi = 0; wi < draw.blend_weight_count && wi < 3u; ++wi) {
                            float w_i = weights[wi];
                            rem -= w_i;
                            const float *m = (wi == 0) ? draw.transform : draw.blend_transforms[wi - 1];
                            float tmp[4];
                            mulRow(px, py, pz, m, tmp);
                            for (int c = 0; c < 4; ++c) clip[c] += w_i * tmp[c];
                        }
                        const float *m_last = (draw.blend_weight_count == 0) ? draw.transform : draw.blend_transforms[draw.blend_weight_count - 1];
                        float tmp[4];
                        mulRow(px, py, pz, m_last, tmp);
                        for (int c = 0; c < 4; ++c) clip[c] += rem * tmp[c];
                    } else {
                        mulRow(px, py, pz, draw.transform, clip);
                    }

                    clip[0] += dx_offset * clip[3];
                    clip[1] += dy_offset * clip[3];
                    dv.x = clip[0];
                    dv.y = clip[1];
                    dv.z = clip[2];
                    dv.w = clip[3];

                    if (has_fog && layout.blend_weight_count == 0u) {
                        auto dot4 = [](float x, float y, float z, const float v[4]) {
                            return x * v[0] + y * v[1] + z * v[2] + v[3];
                        };
                        float fog_c = 0.0f;
                        if (range_fog) {
                            float ex = dot4(px, py, pz, draw.general_fog[3]);
                            float ey = dot4(px, py, pz, draw.general_fog[4]);
                            float ez = dot4(px, py, pz, draw.general_fog[1]);
                            fog_c = std::sqrt(ex * ex + ey * ey + ez * ez);
                        } else {
                            fog_c = dot4(px, py, pz, draw.general_fog[1]);
                        }
                        dv.fog = computeFogFactor(draw, fog_c);
                    }

                    // General D3D fixed-function lighting (runs even when has_diffuse is true!)
                    if (has_normal && layout.blend_weight_count == 0u && draw.directional.enabled) {
                        const float nx_in = *(const float*)(src + layout.normal_offset + 0);
                        const float ny_in = *(const float*)(src + layout.normal_offset + 4);
                        const float nz_in = *(const float*)(src + layout.normal_offset + 8);
                        const auto &lt = draw.directional;
                        const float *nm = lt.normal_transforms[0];
                        float nx = nx_in * nm[0] + ny_in * nm[4] + nz_in * nm[8];
                        float ny = nx_in * nm[1] + ny_in * nm[5] + nz_in * nm[9];
                        float nz = nx_in * nm[2] + ny_in * nm[6] + nz_in * nm[10];
                        float len2 = nx * nx + ny * ny + nz * nz;
                        if (lt.normalize && len2 > 1e-8f) {
                            float inv = 1.0f / std::sqrt(len2);
                            nx *= inv; ny *= inv; nz *= inv;
                        }

                        const float *wm = lt.world_transforms[0];
                        float wx = px * wm[0] + py * wm[4] + pz * wm[8] + wm[12];
                        float wy = px * wm[1] + py * wm[5] + pz * wm[9] + wm[13];
                        float wz = px * wm[2] + py * wm[6] + pz * wm[10] + wm[14];

                        float md[4] = {
                            lt.sources[0] > 0.5f ? cr : lt.material_diffuse[0],
                            lt.sources[0] > 0.5f ? cg : lt.material_diffuse[1],
                            lt.sources[0] > 0.5f ? cb : lt.material_diffuse[2],
                            lt.sources[0] > 0.5f ? ca : lt.material_diffuse[3],
                        };
                        float ma[3] = {
                            lt.sources[1] > 0.5f ? cr : lt.material_ambient[0],
                            lt.sources[1] > 0.5f ? cg : lt.material_ambient[1],
                            lt.sources[1] > 0.5f ? cb : lt.material_ambient[2],
                        };
                        float me[3] = {
                            lt.sources[2] > 0.5f ? cr : lt.material_emissive[0],
                            lt.sources[2] > 0.5f ? cg : lt.material_emissive[1],
                            lt.sources[2] > 0.5f ? cb : lt.material_emissive[2],
                        };

                        float lr = me[0] + lt.global_ambient[0] * ma[0];
                        float lg = me[1] + lt.global_ambient[1] * ma[1];
                        float lb = me[2] + lt.global_ambient[2] * ma[2];
                        float sr = 0.0f, sg = 0.0f, sb = 0.0f;

                        float vx = lt.view[0], vy = lt.view[1], vz = lt.view[2];
                        if (lt.sources[3] > 0.5f) {
                            vx -= wx; vy -= wy; vz -= wz;
                            float vlen2 = vx * vx + vy * vy + vz * vz;
                            if (vlen2 > 1e-8f) {
                                float inv = 1.0f / std::sqrt(vlen2);
                                vx *= inv; vy *= inv; vz *= inv;
                            } else {
                                vx = vy = vz = 0.0f;
                            }
                        }

                        for (uint32_t li = 0; li < lt.count && li < 8u; ++li) {
                            float lx = lt.directions[li][0];
                            float ly = lt.directions[li][1];
                            float lz = lt.directions[li][2];
                            float atten = 1.0f;
                            if (lt.positions[li][3] > 0.5f) {
                                lx = lt.positions[li][0] - wx;
                                ly = lt.positions[li][1] - wy;
                                lz = lt.positions[li][2] - wz;
                                float d = std::sqrt(lx * lx + ly * ly + lz * lz);
                                float denom = lt.attenuation[li][0] + lt.attenuation[li][1] * d + lt.attenuation[li][2] * d * d;
                                atten = (d <= lt.attenuation[li][3] && denom > 0.0f) ? (1.0f / denom) : 0.0f;
                                if (d > 1e-8f) {
                                    float inv = 1.0f / d;
                                    lx *= inv; ly *= inv; lz *= inv;
                                } else {
                                    lx = ly = lz = 0.0f;
                                }
                            }
                            float lambert = nx * lx + ny * ly + nz * lz;
                            float pos_lambert = std::max(0.0f, lambert);
                            lr += (md[0] * lt.colors[li][0] * pos_lambert + lt.ambient[li][0] * ma[0]) * atten;
                            lg += (md[1] * lt.colors[li][1] * pos_lambert + lt.ambient[li][1] * ma[1]) * atten;
                            lb += (md[2] * lt.colors[li][2] * pos_lambert + lt.ambient[li][2] * ma[2]) * atten;

                            if (lt.view[3] > 0.0f && lambert > 0.0f) {
                                float hx = lx + vx, hy = ly + vy, hz = lz + vz;
                                float hlen2 = hx * hx + hy * hy + hz * hz;
                                if (hlen2 > 1e-8f) {
                                    float inv = 1.0f / std::sqrt(hlen2);
                                    hx *= inv; hy *= inv; hz *= inv;
                                }
                                float ndoth = std::max(1e-6f, nx * hx + ny * hy + nz * hz);
                                float sp = atten * fast_pow_specular(ndoth, lt.view[3]);
                                sr += lt.specular[li][0] * sp;
                                sg += lt.specular[li][1] * sp;
                                sb += lt.specular[li][2] * sp;
                            }
                        }
                        cr = std::clamp(lr, 0.0f, 1.0f);
                        cg = std::clamp(lg, 0.0f, 1.0f);
                        cb = std::clamp(lb, 0.0f, 1.0f);
                        ca = md[3];
                        dv.sr = std::clamp(sr, 0.0f, 1.0f);
                        dv.sg = std::clamp(sg, 0.0f, 1.0f);
                        dv.sb = std::clamp(sb, 0.0f, 1.0f);
                    }
                    if (draw.zero_diffuse_rgb) {
                        cr = cg = cb = 0.0f;
                    }
                    dv.r = cr; dv.g = cg; dv.b = cb; dv.a = ca;
                    dv.u0 = uv[0][0]; dv.v0 = uv[0][1];
                    dv.u1 = uv[1][0]; dv.v1 = uv[1][1];
                    dv.u2 = uv[2][0]; dv.v2 = uv[2][1];
                    dv.u3 = uv[3][0]; dv.v3 = uv[3][1];
                }
            }
        }

        g_perf.t_vtx += perfNow() - perf_vtx_t0;

        // 3. Copy Indices
        if (idx_size > 0) {
            memcpy(dyn_cpu + idx_offset, draw.index_bytes, idx_size);
        }

        // 4. Viewport & Scissor (scaled from guest 640x480 to host 1280x720)
        const float scale_x = (float)presenter->width / guest_w;
        const float scale_y = (float)presenter->height / guest_h;
        DkViewport viewport;
        if (sub_viewport) {
            viewport.x = draw.viewport[0] * scale_x;
            viewport.y = draw.viewport[1] * scale_y;
            viewport.width = draw.viewport[2] * scale_x;
            viewport.height = draw.viewport[3] * scale_y;
            viewport.near = draw.viewport[4];
            viewport.far = draw.viewport[5];
        } else {
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)presenter->width;
            viewport.height = (float)presenter->height;
            viewport.near = 0.0f;
            viewport.far = 1.0f;
        }
        dkCmdBufSetViewports(cmd, 0, &viewport, 1);

        DkScissor scissor = { 0, 0, presenter->width, presenter->height };
        if (draw.has_scissor && draw.scissor[2] > draw.scissor[0] && draw.scissor[3] > draw.scissor[1]) {
            uint32_t sx0 = (uint32_t)std::clamp((int)(draw.scissor[0] * scale_x + 0.5f), 0, (int)presenter->width);
            uint32_t sy0 = (uint32_t)std::clamp((int)(draw.scissor[1] * scale_y + 0.5f), 0, (int)presenter->height);
            uint32_t sx1 = (uint32_t)std::clamp((int)(draw.scissor[2] * scale_x + 0.5f), (int)sx0, (int)presenter->width);
            uint32_t sy1 = (uint32_t)std::clamp((int)(draw.scissor[3] * scale_y + 0.5f), (int)sy0, (int)presenter->height);
            scissor = { sx0, sy0, sx1 - sx0, sy1 - sy0 };
        }
        dkCmdBufSetScissors(cmd, 0, &scissor, 1);

        // 5. Bind Shaders & Stage Textures 0..2
        uint32_t color_mask = draw.blend.color_write_mask & 0xFu;
        uint32_t alpha_func = (uint32_t)draw.depth.alpha_func;
        bool has_real_alpha_test = !draw.target.no_depth
                                && draw.depth.alpha_test_enable
                                && alpha_func != 7u /* RECOMP_D3D_CMP_ALWAYS */
                                && draw.has_texture;
        const RecompD3dPixelProgram &pp = draw.pixel_program;

        DkShader const *chosen_fsh = nullptr;
        uint32_t num_textures = 3;

        if (pp.kind == RECOMP_D3D_PIXEL_COMBINERS) {
            uint32_t st = pp.combiners.control & 0xFFu;
            if (st <= 8u) ++g_perf.comb_stages[st];
            else ++g_perf.comb_stages[8];
        }

        static auto isSc2FastCombiner = [](const RecompD3dCombiners &c) -> bool {
            bool valid_c0 = (c.color_input[0] == 0xc4c8c5c8u) || (c.color_input[0] == 0xc420c520u);
            bool valid_a0 = (c.alpha_input[0] == 0xd4d81010u) || (c.alpha_input[0] == 0xd4d11010u);
            if (!valid_c0 || c.color_output[0] != 0x00010c00u ||
                !valid_a0 || c.alpha_output[0] != 0x000000c0u) {
                return false;
            }
            if (c.final0 != 0x130c0300u || c.final1 != 0x00001c80u) {
                return false;
            }
            uint32_t st_count = c.control & 0xFFu;
            if (st_count <= 1u) return true;
            if (st_count == 2u) {
                if (c.color_input[1] == 0u && c.color_output[1] == 0u) return true;
                if ((c.color_input[1] == 0xd8c920ccu || c.color_input[1] == 0x18c9cc38u) &&
                    c.color_output[1] == 0x00000c00u &&
                    c.alpha_input[1] == 0xdcd11010u && c.alpha_output[1] == 0x000000c0u) return true;
            }
            return false;
        };

        if (color_mask == 0u) {
            if (!has_real_alpha_test) {
                chosen_fsh = &presenter->fsh_null;
                num_textures = 0;
                ++g_perf.fsh_null;
            } else {
                chosen_fsh = &presenter->fsh_null_alpha;
                num_textures = 1;
                ++g_perf.fsh_null_alpha_count;
            }
        } else if (is_fast_path) {
            chosen_fsh = &presenter->fsh_color;
            num_textures = 2;
            ++g_perf.fsh_fast;
        } else if (pp.kind == RECOMP_D3D_PIXEL_COMBINERS && (pp.combiners.control & 0xFFu) <= 2u) {
            if (isSc2FastCombiner(pp.combiners)) {
                chosen_fsh = &presenter->fsh_comb_fast;
                num_textures = 2;
                ++g_perf.fsh_comb_fast_count;
            } else {
                chosen_fsh = &presenter->fsh_comb2;
                num_textures = 3;
                ++g_perf.fsh_comb2;
            }
        } else {
            chosen_fsh = &presenter->fsh_tex;
            num_textures = 3;
            ++g_perf.fsh_gen;
        }

        DkShader const* shaders[] = { use_gpu_vtx ? &presenter->vsh_ff : &presenter->vsh, chosen_fsh };
        dkCmdBufBindShaders(cmd, DkStageFlag_GraphicsMask, shaders, 2);
        if (num_textures > 0) {
            dkCmdBufBindTextures(cmd, DkStage_Fragment, 0, tex_handles, num_textures);
        }

        // 6. Bind Pipeline State (Rasterizer, Depth/Stencil, Blend)
        DkRasterizerState rasterizerState;
        dkRasterizerStateDefaults(&rasterizerState);
        if (draw.cull_mode == RECOMP_D3D_CULL_NONE || draw.primitive_type == 2) {
            rasterizerState.cullMode = DkFace_None;
        } else {
            rasterizerState.cullMode = DkFace_Back;
            rasterizerState.frontFace = (draw.cull_mode == RECOMP_D3D_CULL_CLOCKWISE)
                ? DkFrontFace_CCW : DkFrontFace_CW;
        }
        dkCmdBufBindRasterizerState(cmd, &rasterizerState);

        DkDepthStencilState depthState;
        dkDepthStencilStateDefaults(&depthState);
        if (!draw.target.no_depth && draw.depth.depth_test_enable) {
            depthState.depthTestEnable = true;
            depthState.depthWriteEnable = draw.depth.depth_write_enable;
            depthState.depthCompareOp = toDkCompareOp(draw.depth.depth_func);
        } else {
            depthState.depthTestEnable = false;
            depthState.depthWriteEnable = false;
            depthState.depthCompareOp = DkCompareOp_Always;
        }
        if (!draw.target.no_depth && draw.depth.stencil_enable) {
            depthState.stencilTestEnable = true;
            depthState.stencilFrontFailOp = toDkStencilOp(draw.depth.stencil_fail);
            depthState.stencilFrontPassOp = toDkStencilOp(draw.depth.stencil_pass);
            depthState.stencilFrontDepthFailOp = toDkStencilOp(draw.depth.stencil_zfail);
            depthState.stencilFrontCompareOp = toDkCompareOp(draw.depth.stencil_func);
            depthState.stencilBackFailOp = depthState.stencilFrontFailOp;
            depthState.stencilBackPassOp = depthState.stencilFrontPassOp;
            depthState.stencilBackDepthFailOp = depthState.stencilFrontDepthFailOp;
            depthState.stencilBackCompareOp = depthState.stencilFrontCompareOp;
            dkCmdBufSetStencil(cmd, DkFace_FrontAndBack,
                (uint8_t)draw.depth.stencil_write_mask,
                (uint8_t)draw.depth.stencil_ref,
                (uint8_t)draw.depth.stencil_read_mask);
        } else {
            depthState.stencilTestEnable = false;
        }
        dkCmdBufBindDepthStencilState(cmd, &depthState);

        // CRITICAL FIX: Respect color_write_mask == 0 for 3D stencil shadow volume extrusions!
        DkColorWriteState colorWrite;
        dkColorWriteStateDefaults(&colorWrite);
        uint32_t mask = draw.blend.color_write_mask & 0xFu;
        dkColorWriteStateSetMask(&colorWrite, 0, mask);
        dkCmdBufBindColorWriteState(cmd, &colorWrite);

        DkBlendState blendState;
        dkBlendStateDefaults(&blendState);
        if (draw.blend.blend_enable) {
            dkBlendStateSetOps(&blendState, toDkBlendOp(draw.blend.op), toDkBlendOp(draw.blend.op));
            dkBlendStateSetFactors(&blendState,
                toDkBlendFactor(draw.blend.src_factor, false),
                toDkBlendFactor(draw.blend.dst_factor, false),
                toDkBlendFactor(draw.blend.src_factor, true),
                toDkBlendFactor(draw.blend.dst_factor, true));
            uint32_t cc = draw.blend.constant_color;
            dkCmdBufSetBlendConst(cmd,
                ((cc >> 16u) & 0xFF) / 255.0f,
                ((cc >> 8u) & 0xFF) / 255.0f,
                (cc & 0xFF) / 255.0f,
                ((cc >> 24u) & 0xFF) / 255.0f);
        }
        dkCmdBufBindBlendStates(cmd, 0, &blendState, 1);

        DkColorState colorState;
        dkColorStateDefaults(&colorState);
        colorState.blendEnableMask = draw.blend.blend_enable ? 0x01 : 0x00;
        dkCmdBufBindColorState(cmd, &colorState);

        // 7. Bind Buffers & Issue Draw
        DkGpuAddr uboGpu = dyn_gpu + ubo_offset;
        DkGpuAddr vtxGpu = dyn_gpu + vtx_offset;
        DkGpuAddr idxGpu = dyn_gpu + idx_offset;

        dkCmdBufBindUniformBuffer(cmd, DkStage_Fragment, 0, uboGpu, sizeof(FragBlock));

        if (use_gpu_vtx) {
            DkVtxAttribState attribState[5];
            initVtxAttribsFF(attribState);
            dkCmdBufBindVtxAttribState(cmd, attribState, 5);
            dkCmdBufBindVtxBufferState(cmd, s_bufferStateFF, 1);
            dkCmdBufBindVtxBuffer(cmd, 0, vtxGpu, vtx_size);

            DkGpuAddr vboGpu = dyn_gpu + vbo_offset;
            dkCmdBufBindUniformBuffer(cmd, DkStage_Vertex, 0, vboGpu, sizeof(VtxBlock));
        } else {
            DkVtxAttribState attribState[5];
            initVtxAttribs(attribState);
            dkCmdBufBindVtxAttribState(cmd, attribState, 5);
            dkCmdBufBindVtxBufferState(cmd, s_bufferState, 1);
            dkCmdBufBindVtxBuffer(cmd, 0, vtxGpu, vtx_size);
        }

        DkPrimitive prim = DkPrimitive_Triangles;
        if (draw.primitive_type == 1) {
            g_perf.fx_prim_pts++;
        } else if (draw.primitive_type == 2) {
            g_perf.fx_prim_lines++;
            prim = DkPrimitive_Lines;
        } else if (draw.primitive_type == 6) {
            prim = DkPrimitive_TriangleStrip;
        } else if (draw.primitive_type == 7) {
            prim = DkPrimitive_TriangleFan;
        }
        if (draw.target.no_depth) g_perf.fx_no_depth++;
        if (has_real_alpha_test) g_perf.fx_alpha_test++;

        if (idx_size > 0) {
            dkCmdBufBindIdxBuffer(cmd, DkIdxFormat_Uint16, idxGpu);
            dkCmdBufDrawIndexed(cmd, prim, draw.index_count, 1, 0, 0, 0);
        } else {
            dkCmdBufDraw(cmd, prim, draw.vertex_count, 1, 0, 0);
        }
        presenter->draws_in_frame++;

        // Classify pass category for hardware profiling
        GpuPassCategory cat;
        uint32_t color_mask_check = draw.blend.color_write_mask & 0xFu;
        bool has_stencil_write = !draw.target.no_depth && draw.depth.stencil_enable &&
            (draw.depth.stencil_pass != RECOMP_D3D_STENCIL_KEEP ||
             draw.depth.stencil_zfail != RECOMP_D3D_STENCIL_KEEP ||
             draw.depth.stencil_fail != RECOMP_D3D_STENCIL_KEEP);

        if (layout.pretransformed) {
            cat = GPU_CAT_PRETRANS;
        } else if (color_mask_check == 0u || has_stencil_write) {
            cat = GPU_CAT_SHADOW;
        } else if (draw.blend.blend_enable) {
            cat = GPU_CAT_BLEND;
        } else {
            cat = GPU_CAT_OPAQUE;
        }

        if (presenter->query_count[slot] < MAX_GPU_QUERIES) {
            uint32_t q = presenter->query_count[slot];
            presenter->query_cats[slot][q - 1] = cat;

            GpuDrawMeta &meta = presenter->query_meta[slot][q - 1];
            meta.vtx_count = (uint16_t)std::min<uint32_t>(draw.vertex_count, 65535u);
            meta.idx_count = (uint16_t)std::min<uint32_t>(draw.index_count, 65535u);
            meta.prim_type = (uint8_t)draw.primitive_type;
            if (draw.has_texture) {
                meta.tex_w = (uint16_t)draw.texture.width;
                meta.tex_h = (uint16_t)draw.texture.height;
                meta.tex_fmt = (uint8_t)draw.texture.format_byte;
                meta.tex_mips = (uint8_t)draw.texture.mip_levels;
            } else if (draw.stage_textures[0].bytes && draw.stage_textures[0].byte_count > 0) {
                meta.tex_w = (uint16_t)draw.stage_textures[0].desc.width;
                meta.tex_h = (uint16_t)draw.stage_textures[0].desc.height;
                meta.tex_fmt = (uint8_t)draw.stage_textures[0].desc.format_byte;
                meta.tex_mips = (uint8_t)draw.stage_textures[0].desc.mip_levels;
            } else {
                meta.tex_w = 0; meta.tex_h = 0; meta.tex_fmt = 0; meta.tex_mips = 0;
            }
            if (chosen_fsh == &presenter->fsh_comb_fast) meta.fsh_type = 5;
            else if (chosen_fsh == &presenter->fsh_comb2) meta.fsh_type = 3;
            else if (chosen_fsh == &presenter->fsh_color) meta.fsh_type = 2;
            else if (chosen_fsh == &presenter->fsh_tex) meta.fsh_type = 4;
            else if (chosen_fsh == &presenter->fsh_null_alpha) meta.fsh_type = 1;
            else meta.fsh_type = 0;

            if (draw.pixel_program.kind == RECOMP_D3D_PIXEL_COMBINERS) {
                const auto &c = draw.pixel_program.combiners;
                meta.c_control = c.control;
                meta.c_final0 = c.final0;
                meta.c_final1 = c.final1;
                meta.c_tex_modes = c.texture_modes;
                for (int s = 0; s < 2; ++s) {
                    meta.c_in[s] = c.color_input[s];
                    meta.c_out[s] = c.color_output[s];
                    meta.a_in[s] = c.alpha_input[s];
                    meta.a_out[s] = c.alpha_output[s];
                }
            } else {
                meta.c_control = 0;
                meta.c_final0 = 0;
                meta.c_final1 = 0;
                meta.c_tex_modes = 0;
                for (int s = 0; s < 2; ++s) {
                    meta.c_in[s] = 0; meta.c_out[s] = 0;
                    meta.a_in[s] = 0; meta.a_out[s] = 0;
                }
            }

            dkCmdBufReportCounter(cmd, DkCounter_Timestamp, presenter->queries_gpu[slot] + q * sizeof(GpuQueryReport));
            presenter->query_count[slot] = q + 1;
        }
    } else if (command->type == RECOMP_D3D_PRESENTER_COMMAND_COPY_MAIN_COLOR) {
        g_perf.fx_copy_main++;
        if (!beginFrameIfNeeded(presenter)) return RECOMP_D3D_PRESENTER_CLOSED;
        const auto &dest = command->data.copy_destination;
        RecompD3dPresenter::GpuCopySlot *dst = getOrAllocGpuCopy(presenter, dest.data, dest.width, dest.height);
        if (dst && presenter->current_slot >= 0) {
            uint32_t slot = presenter->cur_cmd_slot;
            DkCmdBuf cmd = presenter->cmdbuf[slot];
            DkImageRect srcRect = { 0, 0, 0, presenter->width, presenter->height, 1 };
            DkImageRect dstRect = { 0, 0, 0, dst->width, dst->height, 1 };
            dkCmdBufBarrier(cmd, DkBarrier_Full, DkInvalidateFlags_Image);
            dkCmdBufBlitImage(cmd, &presenter->framebuffer_views[presenter->current_slot], &srcRect,
                              &dst->view, &dstRect, DkBlitFlag_FilterLinear, 0);
            dkCmdBufBarrier(cmd, DkBarrier_Full, DkInvalidateFlags_Descriptors | DkInvalidateFlags_Image);
        }
    } else if (command->type == RECOMP_D3D_PRESENTER_COMMAND_COPY_RECT) {
        g_perf.fx_copy_rect++;
        if (!beginFrameIfNeeded(presenter)) return RECOMP_D3D_PRESENTER_CLOSED;
        const auto &cr = command->data.copy_rect;
        int src_slot = cr.from_previous_frame ? presenter->last_presented_slot : presenter->current_slot;
        RecompD3dPresenter::GpuCopySlot *dst = getOrAllocGpuCopy(presenter, cr.destination.data, cr.destination.width, cr.destination.height);
        if (dst && src_slot >= 0) {
            int32_t l = cr.source[0] < 0 ? 0 : cr.source[0];
            int32_t t = cr.source[1] < 0 ? 0 : cr.source[1];
            int32_t r = cr.source[2] > (int32_t)presenter->guest_width ? (int32_t)presenter->guest_width : cr.source[2];
            int32_t b = cr.source[3] > (int32_t)presenter->guest_height ? (int32_t)presenter->guest_height : cr.source[3];
            uint32_t dx = cr.point[0] < 0 ? 0u : (uint32_t)cr.point[0];
            uint32_t dy = cr.point[1] < 0 ? 0u : (uint32_t)cr.point[1];
            if (r > l && b > t && dx < dst->width && dy < dst->height) {
                uint32_t dw = (uint32_t)(r - l);
                uint32_t dh = (uint32_t)(b - t);
                if (dx + dw > dst->width) dw = dst->width - dx;
                if (dy + dh > dst->height) dh = dst->height - dy;
                uint32_t sx = (uint32_t)l * presenter->width / presenter->guest_width;
                uint32_t sy = (uint32_t)t * presenter->height / presenter->guest_height;
                uint32_t sw = (uint32_t)(r - l) * presenter->width / presenter->guest_width;
                uint32_t sh = (uint32_t)(b - t) * presenter->height / presenter->guest_height;
                uint32_t slot = presenter->cur_cmd_slot;
                DkCmdBuf cmd = presenter->cmdbuf[slot];
                DkImageRect srcRect = { sx, sy, 0, sw, sh, 1 };
                DkImageRect dstRect = { dx, dy, 0, dw, dh, 1 };
                dkCmdBufBarrier(cmd, DkBarrier_Full, DkInvalidateFlags_Image);
                dkCmdBufBlitImage(cmd, &presenter->framebuffer_views[src_slot], &srcRect,
                                  &dst->view, &dstRect, DkBlitFlag_FilterLinear, 0);
                dkCmdBufBarrier(cmd, DkBarrier_Full, DkInvalidateFlags_Descriptors | DkInvalidateFlags_Image);
            }
        }
    } else if (command->type == RECOMP_D3D_PRESENTER_COMMAND_PRESENT) {
        if (presenter->current_slot < 0) {
            // Nothing was cleared or drawn this frame (e.g. skipped logos / waiting for scene load).
            // Do not flip swapchain with an uninitialized or blank buffer, preventing screen flicker!
            presenter->frame_counter++;
            Switch_NotifyActivity(presenter->frame_counter);
            SwitchAudio_FlushPending();
            return RECOMP_D3D_PRESENTER_OK;
        }
        PerfScope perf_present(&g_perf.t_present);
        uint32_t slot = presenter->cur_cmd_slot;
        DkCmdBuf cmd = presenter->cmdbuf[slot];

        // Rolling FPS computation for overlay
        static double s_overlay_fps = 0.0;
        static double s_overlay_ms = 0.0;
        static uint64_t s_last_fps_update_tick = 0;
        static uint32_t s_fps_frames_acc = 0;
        s_fps_frames_acc++;
        uint64_t now_fps_tick = armGetSystemTick();
        if (s_last_fps_update_tick == 0) s_last_fps_update_tick = now_fps_tick;
        if (now_fps_tick - s_last_fps_update_tick >= 4800000ULL) { // 250 ms
            double elapsed = (double)(now_fps_tick - s_last_fps_update_tick) / 19200000.0;
            if (elapsed > 0.0) {
                s_overlay_fps = (double)s_fps_frames_acc / elapsed;
                s_overlay_ms = s_overlay_fps > 0.0 ? (1000.0 / s_overlay_fps) : 0.0;
            }
            s_fps_frames_acc = 0;
            s_last_fps_update_tick = now_fps_tick;
        }

        if (Switch_IsFpsOverlayEnabled()) {
            drawFpsOverlay(presenter, cmd, s_overlay_fps, s_overlay_ms);
            if (presenter->query_count[slot] < MAX_GPU_QUERIES) {
                uint32_t q = presenter->query_count[slot];
                presenter->query_cats[slot][q - 1] = GPU_CAT_OVERLAY;
                dkCmdBufReportCounter(cmd, DkCounter_Timestamp, presenter->queries_gpu[slot] + q * sizeof(GpuQueryReport));
                presenter->query_count[slot] = q + 1;
            }
        }

        if (presenter->dyn_offset > 0) {
            dkMemBlockFlushCpuCache(presenter->block_dyn[slot], 0, (presenter->dyn_offset + 255u) & ~255u);
        }

        dkCmdBufSignalFence(cmd, &presenter->slot_fences[slot], false);
        presenter->slot_has_fence[slot] = true;
        DkCmdList cmdList = dkCmdBufFinishList(cmd);
        dkQueueSubmitCommands(presenter->queue, cmdList);
        dkQueuePresentImage(presenter->queue, presenter->swapchain, presenter->current_slot);
        presenter->last_presented_slot = presenter->current_slot;
        presenter->current_slot = -1;
        presenter->cur_cmd_slot = (slot + 1) % NUM_CMD_SLOTS;

        presenter->frame_counter++;
        Switch_NotifyActivity(presenter->frame_counter);
        {
            static uint32_t s_total_pres_hitches = 0;
            uint64_t now = perfNow();
            uint64_t d_back = g_perf.t_backend - g_perf.prev_t_backend;
            uint64_t d_vtx = g_perf.t_vtx - g_perf.prev_t_vtx;
            uint64_t d_tex = g_perf.t_tex - g_perf.prev_t_tex;
            uint64_t d_aud = g_perf.t_audio_pump - g_perf.prev_t_audio_pump;
            uint64_t d_wait = (g_perf.t_fence - g_perf.prev_t_fence) + (g_perf.t_acquire - g_perf.prev_t_acquire);
            uint64_t d_fence = g_perf.t_fence - g_perf.prev_t_fence;
            uint64_t d_acq = g_perf.t_acquire - g_perf.prev_t_acquire;
            uint64_t d_pres = g_perf.t_present - g_perf.prev_t_present;
            uint64_t d_draw = g_perf.t_draw_total - g_perf.prev_t_draw_total;
            g_perf.prev_t_backend = g_perf.t_backend;
            g_perf.prev_t_vtx = g_perf.t_vtx;
            g_perf.prev_t_tex = g_perf.t_tex;
            g_perf.prev_t_audio_pump = g_perf.t_audio_pump;
            g_perf.prev_t_fence = g_perf.t_fence;
            g_perf.prev_t_acquire = g_perf.t_acquire;
            g_perf.prev_t_present = g_perf.t_present;
            g_perf.prev_t_draw_total = g_perf.t_draw_total;
            if (g_perf.last_present) {
                uint64_t wall = now - g_perf.last_present;
                g_perf.wall_sum += wall;
                if (wall > g_perf.wall_max) g_perf.wall_max = wall;
                if (wall > 20u * 19200u) ++g_perf.slow_frames;
                if (wall <= 325440ULL) ++g_perf.hist_16_95;
                if (wall <= 321600ULL) ++g_perf.hist_16_75;
                else if (wall <= 384000ULL) ++g_perf.hist_20;
                else if (wall <= 641280ULL) ++g_perf.hist_33;
                else if (wall <= 960000ULL) ++g_perf.hist_50;
                else ++g_perf.hist_gt50;
                if (wall > 325440ULL && presenter->draws_in_frame >= 100u && g_perf.pacing_misses_logged < 3u) {
                    auto& pm = g_perf.pacing_misses[g_perf.pacing_misses_logged++];
                    pm.frame = presenter->frame_counter;
                    pm.wall_ms = perfMs(wall);
                    pm.back_ms = perfMs(d_back);
                    pm.draw_ms = perfMs(d_draw);
                    pm.vtx_ms = perfMs(d_vtx);
                    pm.acq_ms = perfMs(d_acq);
                    pm.fence_ms = perfMs(d_fence);
                    pm.pres_ms = perfMs(d_pres);
                }
                if (wall > 384000ULL && presenter->draws_in_frame >= 100u &&
                    g_perf.hitches_logged < 5u && s_total_pres_hitches < 50u) {
                    auto& hr = g_perf.pending_hitches[g_perf.hitches_logged++];
                    ++s_total_pres_hitches;
                    hr.frame = presenter->frame_counter;
                    hr.wall_ms = perfMs(wall);
                    hr.back_ms = perfMs(d_back);
                    hr.guest_ms = (wall > d_back) ? perfMs(wall - d_back) : 0.0;
                    hr.draw_ms = perfMs(d_draw);
                    hr.vtx_ms = perfMs(d_vtx);
                    hr.tex_ms = perfMs(d_tex);
                    hr.aud_ms = perfMs(d_aud);
                    hr.wait_ms = perfMs(d_wait);
                    hr.draws = presenter->draws_in_frame;
                    hr.tex_up = presenter->tex_uploads_in_frame;
                    hr.dyn_kb = presenter->dyn_offset / 1024u;
                }
            }
            g_perf.last_present = now;
            ++g_perf.frames;
            if (presenter->draws_in_frame > g_perf.max_draws) g_perf.max_draws = presenter->draws_in_frame;
        }
        if ((presenter->frame_counter % 120u) == 1u) {
            printf("[Deko3D] Frame %u: draws=%u tex_uploads=%u dyn_kb=%u\n",
                   presenter->frame_counter, presenter->draws_in_frame,
                   presenter->tex_uploads_in_frame, presenter->dyn_offset / 1024u);
            if (g_perf.frames > 1) {
                double n = (double)g_perf.frames;
                double wall_avg = perfMs(g_perf.wall_sum) / (n - 1.0);
                double backend = perfMs(g_perf.t_backend) / n;
                double guest = wall_avg;
                uint64_t au_up, au_sub, au_flush; uint32_t au_nup, au_nsub;
                SwitchAudio_TakePerf(&au_up, &au_sub, &au_flush, &au_nup, &au_nsub);
                printf("[Perf] fps=%.1f wall_avg=%.2fms wall_max=%.2fms slow(>20ms)=%u/%u\n",
                       1000.0 / wall_avg, wall_avg, perfMs(g_perf.wall_max), g_perf.slow_frames, g_perf.frames);
                uint32_t hist_total = g_perf.hist_16_75 + g_perf.hist_20 + g_perf.hist_33 + g_perf.hist_50 + g_perf.hist_gt50;
                double pct_59 = (hist_total > 0u) ? (100.0 * (double)g_perf.hist_16_95 / (double)hist_total) : 0.0;
                printf("[Perf] frame_hist: <=16.75ms=%u <=16.95ms(59+fps)=%u(%.1f%%) 16.75-20ms=%u 20-33.4ms=%u 33.4-50ms=%u >50ms=%u\n",
                       g_perf.hist_16_75, g_perf.hist_16_95, pct_59,
                       g_perf.hist_20, g_perf.hist_33, g_perf.hist_50, g_perf.hist_gt50);
                for (uint32_t mi = 0; mi < g_perf.pacing_misses_logged; ++mi) {
                    const auto& pm = g_perf.pacing_misses[mi];
                    printf("[PacingMiss] f=%u wall=%.2fms back=%.2fms (draw=%.2f vtx=%.2f acq=%.2f fence=%.2f pres=%.2f)\n",
                           pm.frame, pm.wall_ms, pm.back_ms, pm.draw_ms, pm.vtx_ms,
                           pm.acq_ms, pm.fence_ms, pm.pres_ms);
                }
                g_perf.pacing_misses_logged = 0;
                for (uint32_t hi = 0; hi < g_perf.hitches_logged; ++hi) {
                    const auto& hr = g_perf.pending_hitches[hi];
                    printf("[Hitch-Pres] frame=%u wall=%.2fms (guest_net=%.2fms backend=%.2fms [draw=%.2f vtx=%.2f tex=%.2f aud=%.2f wait=%.2f]) draws=%u tex_up=%u dyn_kb=%u\n",
                           hr.frame, hr.wall_ms, hr.guest_ms, hr.back_ms,
                           hr.draw_ms, hr.vtx_ms, hr.tex_ms, hr.aud_ms, hr.wait_ms,
                           hr.draws, hr.tex_up, hr.dyn_kb);
                }
                printf("[Perf] per-frame ms: guest_cpu(wall)=%.2f backend=%.2f [vtx_cpu=%.2f tex=%.2f audio_pump=%.2f "
                       "gpu_fence_wait=%.2f vsync_acquire=%.2f draw_total=%.2f present=%.2f]\n",
                       guest, backend, perfMs(g_perf.t_vtx) / n, perfMs(g_perf.t_tex) / n,
                       perfMs(g_perf.t_audio_pump) / n, perfMs(g_perf.t_fence) / n,
                       perfMs(g_perf.t_acquire) / n, perfMs(g_perf.t_draw_total) / n, perfMs(g_perf.t_present) / n);
                printf("[Perf] per-frame load: draws=%.0f (max %u) verts=%.0f idx=%.0f vtx_mb=%.2f | paths(draws): pre=%u prog=%u ff=%u (gpu=%u cpu=%u) skin=%u lit=%u"
                       " | verts: pre=%u prog=%u ff=%u skin=%u | fsh: null=%u(alpha=%u) fast=%u comb2=%u gen=%u\n",
                       g_perf.draws / n, g_perf.max_draws, g_perf.verts / n, g_perf.idx / n,
                       (double)g_perf.dyn_bytes / n / (1024.0 * 1024.0),
                       g_perf.path_pre, g_perf.path_prog, g_perf.path_ff, g_perf.verts_ff_gpu, g_perf.verts_ff_cpu, g_perf.path_skin, g_perf.path_lit,
                       g_perf.verts_pre, g_perf.verts_prog, g_perf.verts_ff, g_perf.verts_skin,
                       g_perf.fsh_null, g_perf.fsh_null_alpha_count, g_perf.fsh_fast, g_perf.fsh_comb2, g_perf.fsh_gen);
                printf("[Perf] combiners breakdown: 0=%u 1=%u 2=%u 3=%u 4=%u 5=%u 6=%u 7=%u 8=%u\n",
                       g_perf.comb_stages[0], g_perf.comb_stages[1], g_perf.comb_stages[2],
                       g_perf.comb_stages[3], g_perf.comb_stages[4], g_perf.comb_stages[5],
                       g_perf.comb_stages[6], g_perf.comb_stages[7], g_perf.comb_stages[8]);
                printf("[Perf] audio (total in %u frames): updates=%u (%.2f ms total, %.3f ms each) submits=%u (%.2f ms) flush=%.2f ms\n",
                       g_perf.frames, au_nup, perfMs(au_up), au_nup ? perfMs(au_up) / au_nup : 0.0,
                       au_nsub, perfMs(au_sub), perfMs(au_flush));

                uint64_t g_sub = 0, g_pump = 0, g_hrtf = 0, g_draw_adap = 0, g_shadow_push = 0, g_vblank = 0, g_pres_wait = 0;
                uint64_t g_lookup_t = 0, g_x87_t = 0;
                uint32_t g_submits = 0, g_pumps = 0, g_hrtfs = 0, g_draw_calls = 0, g_shadow_draws = 0, g_vblanks = 0, g_pres_waits = 0;
                uint32_t g_lookups = 0, g_sin_cnt = 0, g_cos_cnt = 0, g_atan2_cnt = 0;
                size_t flat_bytes = 0;
                char thread_times[256] = "";
                Switch_TakeGuestPerf(&g_sub, &g_pump, &g_hrtf, &g_submits, &g_pumps, &g_hrtfs,
                                     &g_draw_adap, &g_draw_calls,
                                     &g_shadow_push, &g_shadow_draws,
                                     &g_vblank, &g_vblanks,
                                     &g_pres_wait, &g_pres_waits,
                                     &g_lookup_t, &g_lookups,
                                     &g_x87_t, &g_sin_cnt, &g_cos_cnt, &g_atan2_cnt,
                                     &flat_bytes,
                                     thread_times, sizeof(thread_times));
                double d3d8_sub_ms = perfMs(g_sub) / n;
                double dsound_pump_ms = perfMs(g_pump) / n;
                double dsound_hrtf_ms = perfMs(g_hrtf) / n;
                double d3d8_draw_ms = perfMs(g_draw_adap) / n;
                double d3d8_shadow_ms = perfMs(g_shadow_push) / n;
                double vblank_wait_ms = perfMs(g_vblank) / n;
                double present_wait_ms = perfMs(g_pres_wait) / n;
                double lookup_ms = perfMs(g_lookup_t) / n;
                double x87_ms = perfMs(g_x87_t) / n;
                double total_d3d8_guest_ms = d3d8_draw_ms + d3d8_shadow_ms;
                double pure_logic_ms = guest - (dsound_pump_ms + total_d3d8_guest_ms + vblank_wait_ms + present_wait_ms);
                if (pure_logic_ms < 0.0) pure_logic_ms = 0.0;
                printf("[Perf] guest_cpu detailed breakdown: guest=%.2fms [pure_game=%.2fms d3d8_total=%.2fms (regular_draws=%.0f:%.2fms, shadow_draws=%.0f:%.2fms, submit_copy=%.2fms) dsound=%.2fms (hrtf=%.2fms, calls=%u) vblank_wait=%.2fms (waits=%.1f) pres_wait=%.2fms]\n",
                       guest, pure_logic_ms, total_d3d8_guest_ms, (double)g_draw_calls / n, d3d8_draw_ms, (double)g_shadow_draws / n, d3d8_shadow_ms, d3d8_sub_ms, dsound_pump_ms, dsound_hrtf_ms, g_hrtfs, vblank_wait_ms, (double)g_vblanks / n, present_wait_ms);
                {
                    uint64_t p_idx = 0, p_gath = 0, p_mat = 0, p_shd = 0;
                    uint32_t p_drw = 0;
                    Switch_TakeDrawProf(&p_idx, &p_gath, &p_mat, &p_shd, &p_drw);
                    uint64_t p_tex_t = 0, p_comb_t = 0, p_light_t = 0, p_fog_t = 0;
                    Switch_TakeDrawShaderProf(&p_tex_t, &p_comb_t, &p_light_t, &p_fog_t);
                    printf("[DrawProf] draws=%.1f/f | idx=%.2fms gather=%.2fms mat=%.2fms shader=%.2fms (tex=%.2f comb=%.2f light=%.2f fog=%.2f)\n",
                           (double)p_drw / n, perfMs(p_idx) / n, perfMs(p_gath) / n, perfMs(p_mat) / n, perfMs(p_shd) / n,
                           perfMs(p_tex_t) / n, perfMs(p_comb_t) / n, perfMs(p_light_t) / n, perfMs(p_fog_t) / n);
                    uint32_t w_dir = 0, w_gath = 0, w_push = 0, w_wake = 0, w_spin = 0;
                    uint64_t w_exec = 0, w_flush = 0, w_enq = 0;
                    Switch_TakeWorkerProf(&w_dir, &w_gath, &w_push, &w_exec, &w_flush, &w_enq, &w_wake, &w_spin);
                    uint32_t pf[8] = {};
                    Switch_TakePushFailProf(pf);
                    printf("[WorkerProf] jobs/f(dir=%.1f gath=%.1f push=%.1f) | core2_exec=%.2fms core0_flush=%.2fms core0_enq=%.2fms | wake=%.1f/f spin=%u | pfail=[%u,%u,%u,%u,%u,%u,%u,%u]\n",
                           (double)w_dir / n, (double)w_gath / n, (double)w_push / n,
                           perfMs(w_exec) / n, perfMs(w_flush) / n, perfMs(w_enq) / n,
                           (double)w_wake / n, w_spin,
                           pf[0], pf[1], pf[2], pf[3], pf[4], pf[5], pf[6], pf[7]);
                }
                printf("[Perf] guest CPU internals: recomp_lookup=%.2fms (calls=%.0f, flat_table=%s, %zu KB) | x87_math=%.2fms (sin=%.0f, cos=%.0f, atan2=%.0f)\n",
                       lookup_ms, (double)g_lookups / n, flat_bytes ? "ACTIVE" : "OFF(binary search)", flat_bytes / 1024u,
                       x87_ms, (double)g_sin_cnt / n, (double)g_cos_cnt / n, (double)g_atan2_cnt / n);
                if (thread_times[0] != '\0') {
                    printf("[Perf] guest kernel threads (last %u frames):%s\n", g_perf.frames, thread_times);
                }

                double gn = g_perf.gpu_profiled_frames > 0 ? (double)g_perf.gpu_profiled_frames : 1.0;
                double gpu_ms = (double)g_perf.gpu_total_ns / (gn * 1000000.0);
                double ms_opaque = (double)g_perf.gpu_cat_ns[GPU_CAT_OPAQUE] / (gn * 1000000.0);
                double ms_blend = (double)g_perf.gpu_cat_ns[GPU_CAT_BLEND] / (gn * 1000000.0);
                double ms_shadow = (double)g_perf.gpu_cat_ns[GPU_CAT_SHADOW] / (gn * 1000000.0);
                double ms_pretrans = (double)g_perf.gpu_cat_ns[GPU_CAT_PRETRANS] / (gn * 1000000.0);
                double ms_clear = (double)g_perf.gpu_cat_ns[GPU_CAT_CLEAR] / (gn * 1000000.0);
                double ms_overlay = (double)g_perf.gpu_cat_ns[GPU_CAT_OVERLAY] / (gn * 1000000.0);

                printf("[Perf] GPU HW TIMESTAMPS: active_total=%.2fms | opaque_3d=%.2fms (d=%.0f) blend_transp=%.2fms (d=%.0f) "
                       "shadow_stencil=%.2fms (d=%.0f) pretrans_2d=%.2fms (d=%.0f) clear=%.2fms overlay=%.2fms (profiled_frames=%u)\n",
                       gpu_ms,
                       ms_opaque, (double)g_perf.gpu_cat_draws[GPU_CAT_OPAQUE] / gn,
                       ms_blend, (double)g_perf.gpu_cat_draws[GPU_CAT_BLEND] / gn,
                       ms_shadow, (double)g_perf.gpu_cat_draws[GPU_CAT_SHADOW] / gn,
                       ms_pretrans, (double)g_perf.gpu_cat_draws[GPU_CAT_PRETRANS] / gn,
                       ms_clear, ms_overlay, g_perf.gpu_profiled_frames);

                static const char *const s_fsh_names[6] = { "null", "null_alpha", "color/fast", "comb2", "tex/gen", "comb_fast" };
                static const char *const s_cat_names[GPU_CAT_COUNT] = { "clear", "pretrans", "shadow", "opaque", "blend", "overlay" };
                printf("[Perf] GPU Shaders HW: comb_fast=%.2fms (d=%.0f) comb2=%.2fms (d=%.0f) color=%.2fms (d=%.0f) tex_gen=%.2fms (d=%.0f) null=%.2fms (d=%.0f)\n",
                       (double)g_perf.gpu_fsh_ns[5] / (gn * 1000000.0), (double)g_perf.gpu_fsh_draws[5] / gn,
                       (double)g_perf.gpu_fsh_ns[3] / (gn * 1000000.0), (double)g_perf.gpu_fsh_draws[3] / gn,
                       (double)g_perf.gpu_fsh_ns[2] / (gn * 1000000.0), (double)g_perf.gpu_fsh_draws[2] / gn,
                       (double)g_perf.gpu_fsh_ns[4] / (gn * 1000000.0), (double)g_perf.gpu_fsh_draws[4] / gn,
                       (double)g_perf.gpu_fsh_ns[0] / (gn * 1000000.0), (double)g_perf.gpu_fsh_draws[0] / gn);

                printf("[Perf] TOP 5 SLOWEST GPU DRAWS in block:\n");
                for (int k = 0; k < 5; ++k) {
                    const auto &w = g_perf.worst_draws[k];
                    if (w.ms > 0.05) {
                        const char *cname = (w.cat < GPU_CAT_COUNT) ? s_cat_names[w.cat] : "?";
                        const char *fname = (w.fsh < 6) ? s_fsh_names[w.fsh] : "?";
                        printf("   [TopGpuDraw #%d] %.2fms | cat=%s fsh=%s vtx=%u idx=%u tex=%ux%u (fmt=0x%x, mips=%u)\n",
                               k, w.ms, cname, fname, w.vtx, w.idx, w.tw, w.th, w.fmt, w.mips);
                        if (w.fsh == 3 || w.fsh == 5) {
                            printf("       -> comb: ctrl=0x%08x final0=0x%08x final1=0x%08x tmode=0x%08x | s0(cin=0x%08x cout=0x%08x ain=0x%08x aout=0x%08x) | s1(cin=0x%08x cout=0x%08x ain=0x%08x aout=0x%08x)\n",
                                   w.c_control, w.c_final0, w.c_final1, w.c_tex_modes,
                                   w.c_in[0], w.c_out[0], w.a_in[0], w.a_out[0],
                                   w.c_in[1], w.c_out[1], w.a_in[1], w.a_out[1]);
                        }
                    }
                }
                printf("[FxDiag] 120f drops(offscreen=%u empty=%u bad_fvf=%u) | copy(main=%u rect=%u) | prim(pts=%u lines=%u) | no_depth=%u alpha_test=%u\n",
                       g_perf.fx_offscreen_drops, g_perf.fx_empty_drops, g_perf.fx_bad_fvf_drops,
                       g_perf.fx_copy_main, g_perf.fx_copy_rect,
                       g_perf.fx_prim_pts, g_perf.fx_prim_lines,
                       g_perf.fx_no_depth, g_perf.fx_alpha_test);
            }
            g_perf = PerfStats{ .last_present = g_perf.last_present };
        }
        presenter->draws_in_frame = 0;
        presenter->tex_uploads_in_frame = 0;

        SwitchAudio_FlushPending();
    }

    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError d3d_backend_draw_coverage(
    RecompD3dPresenter *presenter, const RecompD3dPresenterDrawCommand *draw, bool *covered) {
    if (!presenter || !draw || !covered) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    *covered = true;
    return RECOMP_D3D_PRESENTER_OK;
}

D3dBackendVisibilityPoll d3d_backend_poll_visibility(
    RecompD3dPresenter *presenter, uint32_t *index, uint32_t *pixels) {
    (void)presenter;
    if (index) *index = 0;
    if (pixels) *pixels = 0;
    return D3D_BACKEND_VISIBILITY_IDLE;
}

RecompD3dPresenterError d3d_backend_release_memory(
    RecompD3dPresenter *presenter, uint32_t base, uint32_t size, bool targets) {
    (void)targets;
    if (!presenter) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    for (uint32_t i = 0; i < MAX_TEX_CACHE; i++) {
        if (presenter->tex_cache[i].used &&
            presenter->tex_cache[i].data >= base &&
            presenter->tex_cache[i].data < base + size) {
            removeTexSlotFromHash(presenter, (int)i);
        }
    }
    return RECOMP_D3D_PRESENTER_OK;
}

void d3d_backend_set_immediate_present(bool enabled) {
    (void)enabled;
}

RecompD3dPresenterError d3d_backend_read_frame(
    RecompD3dPresenter *presenter, uint32_t *pixels, uint32_t capacity,
    uint32_t *width, uint32_t *height) {
    if (!presenter) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    if (width) *width = presenter->guest_width;
    if (height) *height = presenter->guest_height;
    if (pixels && capacity > 0) {
        memset(pixels, 0, capacity * sizeof(uint32_t));
    }
    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError d3d_backend_show_image(
    RecompD3dPresenter *presenter, const uint32_t *pixels,
    uint32_t width, uint32_t height) {
    (void)presenter; (void)pixels; (void)width; (void)height;
    return RECOMP_D3D_PRESENTER_OK;
}

void d3d_backend_report_draw_textures(void) {
}

RecompD3dPresenterError d3d_backend_destroy(RecompD3dPresenter **presenter) {
    if (!presenter || !*presenter) return RECOMP_D3D_PRESENTER_OK;
    RecompD3dPresenter *p = *presenter;
    if (s_active_deko_presenter == p) s_active_deko_presenter = nullptr;

    if (p->queue) {
        if (p->current_slot >= 0) {
            uint32_t slot = p->cur_cmd_slot;
            DkCmdList cmdList = dkCmdBufFinishList(p->cmdbuf[slot]);
            dkQueueSubmitCommands(p->queue, cmdList);
            dkQueuePresentImage(p->queue, p->swapchain, p->current_slot);
            p->current_slot = -1;
        }
        dkQueueWaitIdle(p->queue);
    }
    if (p->swapchain) dkSwapchainDestroy(p->swapchain);
    for (uint32_t i = 0; i < NUM_CMD_SLOTS; ++i) {
        if (p->cmdbuf[i]) dkCmdBufDestroy(p->cmdbuf[i]);
        if (p->block_dyn[i]) dkMemBlockDestroy(p->block_dyn[i]);
        if (p->block_cmd[i]) dkMemBlockDestroy(p->block_cmd[i]);
    }
    if (p->block_queries) dkMemBlockDestroy(p->block_queries);
    if (p->block_gpu_copies) dkMemBlockDestroy(p->block_gpu_copies);
    if (p->block_tex) dkMemBlockDestroy(p->block_tex);
    if (p->block_desc) dkMemBlockDestroy(p->block_desc);
    if (p->block_code) dkMemBlockDestroy(p->block_code);
    if (p->block_depth) dkMemBlockDestroy(p->block_depth);
    if (p->block_framebuffers) dkMemBlockDestroy(p->block_framebuffers);
    if (p->queue) dkQueueDestroy(p->queue);
    if (p->device) dkDeviceDestroy(p->device);

    free(p);
    *presenter = NULL;
    return RECOMP_D3D_PRESENTER_OK;
}
