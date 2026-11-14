/**
 * Native ARM64 NEON-inlined implementation of sub_000843D0 (cloth/hair constraint solver).
 * Eliminates cross-TU calls for vec3/vec4 math (12550/125A0/126A0/12790/12950/12B00/12BC0)
 * and inlines sub_00082D80 / sub_00082DD0.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include "sc2_fast_mem.h"
#include <math.h>
#include <stdint.h>

#if defined(__aarch64__) || defined(_M_ARM64)
#include <arm_neon.h>
#define SC2_HAS_NEON 1
#else
#define SC2_HAS_NEON 0
#endif

static inline void fast_sub_00012550(void)
{
    const uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#if SC2_HAS_NEON
    vst1q_f32(dst, vaddq_f32(vld1q_f32(a), vld1q_f32(b)));
#else
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
    dst[3] = a[3] + b[3];
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

static inline void fast_sub_000125A0(void)
{
    const uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#if SC2_HAS_NEON
    vst1q_f32(dst, vsubq_f32(vld1q_f32(a), vld1q_f32(b)));
#else
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
    dst[3] = a[3] - b[3];
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

static inline void fast_sub_000126A0(void)
{
    const uint32_t dst_va = ecx;
    const float s = MEMF(esp + 4);
    const float *a = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#if SC2_HAS_NEON
    vst1q_f32(dst, vmulq_n_f32(vld1q_f32(a), s));
#else
    dst[0] = a[0] * s;
    dst[1] = a[1] * s;
    dst[2] = a[2] * s;
    dst[3] = a[3] * s;
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

static inline void fast_sub_00012790(void)
{
    const uint32_t dst_va = ecx;
    const float *m = (const float *)&MEM8(MEM32(esp + 4));
    const float *v = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#if SC2_HAS_NEON
    float32x4_t vv = vld1q_f32(v);
    float32x4_t acc = vld1q_f32(m + 12);
    acc = vfmaq_laneq_f32(acc, vld1q_f32(m + 0), vv, 0);
    acc = vfmaq_laneq_f32(acc, vld1q_f32(m + 4), vv, 1);
    acc = vfmaq_laneq_f32(acc, vld1q_f32(m + 8), vv, 2);
    vst1q_f32(dst, acc);
#else
    const float vx = v[0], vy = v[1], vz = v[2];
    for (int i = 0; i < 4; ++i) {
        dst[i] = m[i] * vx + m[4 + i] * vy + m[8 + i] * vz + m[12 + i];
    }
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

static inline void fast_sub_00012B00(void)
{
    const float *a = (const float *)&MEM8(ecx);
#if SC2_HAS_NEON
    float32x4_t va = vld1q_f32(a);
    float32x4_t sq = vmulq_f32(va, va);
    float d2 = vgetq_lane_f32(sq, 0) + vgetq_lane_f32(sq, 1) + vgetq_lane_f32(sq, 2);
#else
    float d2 = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
#endif
    g_fp_top = (g_fp_top + 7u) & 7u;
    g_fp_stack[g_fp_top] = (double)d2;
    g_eflags &= ~1u;
    esp += 4;
}

static inline void fast_sub_00012BC0(void)
{
    const float *a = (const float *)&MEM8(ecx);
    const float *b = (const float *)&MEM8(MEM32(esp + 4));
#if SC2_HAS_NEON
    float32x4_t d = vsubq_f32(vld1q_f32(a), vld1q_f32(b));
    float32x4_t sq = vmulq_f32(d, d);
    float d2 = vgetq_lane_f32(sq, 0) + vgetq_lane_f32(sq, 1) + vgetq_lane_f32(sq, 2);
#else
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];
    float dz = a[2] - b[2];
    float d2 = dx * dx + dy * dy + dz * dz;
#endif
    g_fp_top = (g_fp_top + 7u) & 7u;
    g_fp_stack[g_fp_top] = (double)d2;
    g_eflags &= ~1u;
    esp += 8;
}

static inline void fast_alloca_probe(void)
{
    esp += 4;
    if (eax != 0u) {
        esp -= eax;
    }
    g_eflags &= ~1u;
}

static inline void fast_allshl(void)
{
    uint32_t shift = ecx & 0xFFu;
    if (shift >= 64u) {
        eax = 0u;
        edx = 0u;
    } else if (shift >= 32u) {
        edx = eax << (shift & 31u);
        eax = 0u;
    } else if (shift > 0u) {
        edx = (edx << shift) | (eax >> (32u - shift));
        eax <<= shift;
    }
    esp += 4;
}

static inline void fast_sub_00082D80(void)
{
    int8_t idx = (int8_t)MEM8(ecx + 0x18);
    if (idx < 0) {
        uint32_t arg2 = MEM32(esp + 0xC);
        uint32_t arg1 = MEM32(esp + 0x8);
        uint32_t ret = MEM32(esp);
        uint32_t child = MEM32(ecx + 0x14);
        esp += 16;
        PUSH32(esp, arg2);
        PUSH32(esp, arg1);
        ecx = child;
        PUSH32(esp, ret);
        sub_00082BD0();
        return;
    }
    uint32_t base = MEM32(esp + 4);
    uint32_t dst_va = MEM32(esp + 0xC);
    uint32_t src_va = base + (uint32_t)((int32_t)idx * 0x50);
#if SC2_HAS_NEON
    vst1q_f32((float *)&MEM8(dst_va), vld1q_f32((const float *)&MEM8(src_va)));
#else
    MEM32(dst_va) = MEM32(src_va);
    MEM32(dst_va + 4) = MEM32(src_va + 4);
    MEM32(dst_va + 8) = MEM32(src_va + 8);
    MEM32(dst_va + 12) = MEM32(src_va + 12);
#endif
    eax = 1u;
    esp += 16;
}

static inline void fast_sub_00082DD0(void)
{
    uint32_t self_va = ecx;
    uint32_t arg0 = MEM32(esp + 4);
    uint32_t arg1 = MEM32(esp + 8);
    esp += 12;

    float tmp[4];
    int8_t idx = (int8_t)MEM8(self_va + 0x18);
    if (idx >= 0) {
        const float *s = (const float *)&MEM8(arg0 + (uint32_t)((int32_t)idx * 0x50));
        tmp[0] = s[0]; tmp[1] = s[1]; tmp[2] = s[2]; tmp[3] = s[3];
    } else {
        uint32_t saved_esp = esp;
        esp -= 16;
        uint32_t tmp_va = esp;
        PUSH32(esp, tmp_va);
        PUSH32(esp, arg1);
        ecx = MEM32(self_va + 0x14);
        PUSH32(esp, 0x00082DF0u);
        sub_00082BD0();
        const float *s = (const float *)&MEM8(tmp_va);
        tmp[0] = s[0]; tmp[1] = s[1]; tmp[2] = s[2]; tmp[3] = s[3];
        esp = saved_esp;
    }

    float *self_v = (float *)&MEM8(self_va);
#if SC2_HAS_NEON
    float32x4_t sv = vld1q_f32(self_v);
    float32x4_t tv = vld1q_f32(tmp);
    float32x4_t diff = vsubq_f32(sv, tv);
    float32x4_t sq = vmulq_f32(diff, diff);
    float len_sq = vgetq_lane_f32(sq, 0) + vgetq_lane_f32(sq, 1) + vgetq_lane_f32(sq, 2);
    if (isfinite(len_sq) && len_sq > 0.0f) {
        float inv = 1.0f / sqrtf(len_sq);
        float nx = vgetq_lane_f32(diff, 0) * inv;
        if (isfinite(nx)) {
            diff = vsetq_lane_f32(nx, diff, 0);
            diff = vsetq_lane_f32(vgetq_lane_f32(diff, 1) * inv, diff, 1);
            diff = vsetq_lane_f32(vgetq_lane_f32(diff, 2) * inv, diff, 2);
        } else {
            diff = vsetq_lane_f32(0.0f, diff, 0);
            diff = vsetq_lane_f32(0.0f, diff, 1);
            diff = vsetq_lane_f32(0.0f, diff, 2);
        }
    } else {
        diff = vsetq_lane_f32(0.0f, diff, 0);
        diff = vsetq_lane_f32(0.0f, diff, 1);
        diff = vsetq_lane_f32(0.0f, diff, 2);
    }
    float scale = MEMF(self_va + 0x10);
    float32x4_t out = vfmaq_n_f32(tv, diff, scale);
    vst1q_f32(self_v, out);
#else
    float dx = self_v[0] - tmp[0];
    float dy = self_v[1] - tmp[1];
    float dz = self_v[2] - tmp[2];
    float dw = self_v[3] - tmp[3];
    float len_sq = dx * dx + dy * dy + dz * dz;
    if (isfinite(len_sq) && len_sq > 0.0f) {
        float inv = 1.0f / sqrtf(len_sq);
        float nx = dx * inv;
        if (isfinite(nx)) {
            dx = nx;
            dy *= inv;
            dz *= inv;
        } else {
            dx = dy = dz = 0.0f;
        }
    } else {
        dx = dy = dz = 0.0f;
    }
    float scale = MEMF(self_va + 0x10);
    self_v[0] = tmp[0] + dx * scale;
    self_v[1] = tmp[1] + dy * scale;
    self_v[2] = tmp[2] + dz * scale;
    self_v[3] = tmp[3] + dw * scale;
#endif
    eax = self_va;
    g_eflags &= ~1u;
}


static inline void fast_sub_00012600(void)
{
    const float *a = (const float *)&MEM8(ecx);
    const float *b = (const float *)&MEM8(MEM32(esp + 4));
#if SC2_HAS_NEON
    float32x4_t p = vmulq_f32(vld1q_f32(a), vld1q_f32(b));
    float d = vgetq_lane_f32(p, 0) + vgetq_lane_f32(p, 1) + vgetq_lane_f32(p, 2);
#else
    float d = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
#endif
    g_fp_top = (g_fp_top + 7u) & 7u;
    g_fp_stack[g_fp_top] = (double)d;
    g_eflags &= ~1u;
    esp += 8;
}

static inline void fast_sub_00012640(void)
{
    const uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
    const float ax = a[0], ay = a[1], az = a[2], aw = a[3];
    const float bx = b[0], by = b[1], bz = b[2];
    dst[0] = ay * bz - az * by;
    dst[1] = az * bx - ax * bz;
    dst[2] = ax * by - ay * bx;
    dst[3] = aw;
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

static inline void fast_sub_000127F0(void)
{
    const uint32_t dst_va = ecx;
    const float *m = (const float *)&MEM8(MEM32(esp + 4));
    const float *v = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
    const float vx = v[0], vy = v[1], vz = v[2], vw = v[3];
#if SC2_HAS_NEON
    float32x4_t acc = vmulq_n_f32(vld1q_f32(m + 0), vx);
    acc = vfmaq_n_f32(acc, vld1q_f32(m + 4), vy);
    acc = vfmaq_n_f32(acc, vld1q_f32(m + 8), vz);
    acc = vsetq_lane_f32(vw, acc, 3);
    vst1q_f32(dst, acc);
#else
    dst[0] = m[0] * vx + m[4] * vy + m[8] * vz;
    dst[1] = m[1] * vx + m[5] * vy + m[9] * vz;
    dst[2] = m[2] * vx + m[6] * vy + m[10] * vz;
    dst[3] = vw;
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

static inline void fast_sub_00012950(void)
{
    const uint32_t dst_va = ecx;
    const float *v = (const float *)&MEM8(MEM32(esp + 4));
    float *dst = (float *)&MEM8(dst_va);
    const float x = v[0], y = v[1], z = v[2], w = v[3];
    const float len_sq = x * x + y * y + z * z;
    if (isfinite(len_sq) && len_sq > 0.0f) {
        const float inv = 1.0f / sqrtf(len_sq);
        const float nx = x * inv;
        if (isfinite(nx)) {
            dst[0] = nx;
            dst[1] = y * inv;
            dst[2] = z * inv;
            dst[3] = w;
        } else {
            dst[0] = 0.0f; dst[1] = 0.0f; dst[2] = 0.0f; dst[3] = w;
        }
    } else {
        dst[0] = 0.0f; dst[1] = 0.0f; dst[2] = 0.0f; dst[3] = w;
    }
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 8;
}

static inline void fast_sub_000823E0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_000823E0: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x18));
    esp = esp - 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = MEM32(ebp + 0xC);
    PUSH32(esp, esi);
    esi = MEM32(ebp + 8);
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    ecx = esp + 0x18;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000823FCu); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_000823FC: ;
    ecx = esp + 0x10;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082405u); fast_sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00082405: ;
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    edi = MEM32(ebp + 0x10);
    ecx = esp + 0x10;
    PUSH32(esp, ecx);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082418u); fast_sub_00012600(); /* call 0x00012600 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00082418: ;
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0xC)); /* fsub dword ptr [esp + 0xc] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 0x14)); /* fadd dword ptr [ebp + 0x14] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_NZ(_fa, _fb)) goto loc_00082458; /* jne: not equal / not zero */

loc_00082430: ;
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */
    SET_LO8(eax, 1);
    fp_st1() = RECOMP_FP_PC(fp_top() - fp_st1()); fp_pop(); /* fsubrp st(1) */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi)); /* fmul dword ptr [edi] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi)); /* fadd dword ptr [esi] */
    MEMF(esi) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi + 4)); /* fmul dword ptr [edi + 4] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 4)); /* fadd dword ptr [esi + 4] */
    MEMF(esi + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi + 8)); /* fmul dword ptr [edi + 8] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 8)); /* fadd dword ptr [esi + 8] */
    MEMF(esi + 8) = (float)fp_top(); fp_pop(); /* fstp */
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00082458: ;
    fp_pop(); /* fstp st(0) */
    POP32(esp, edi);
    fp_pop(); /* fstp st(0) */
    _cf = 0; /* xor clears CF */
    SET_LO8(eax, 0); /* xor self */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fas = (int32_t)(int8_t)(_fa); /* xor result */
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

static inline void fast_sub_00082470(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_00082470: ;
    fp_push(MEMF(esp + 4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_NZ(_fa, _fb)) goto loc_00082492; /* jne: not equal / not zero */

loc_00082481: ;
    fp_push(MEMF(esp + 8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000824B4; /* jnp: not parity */

loc_00082492: ;
    fp_push(MEMF(esp + 4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000824B7; /* jp: parity */

loc_000824A3: ;
    fp_push(MEMF(esp + 8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_NZ(_fa, _fb)) goto loc_000824B7; /* jne: not equal / not zero */

loc_000824B4: ;
    SET_LO8(eax, 1);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    esp += 4; return; /* ret */

loc_000824B7: ;
    _cf = 0; /* xor clears CF */
    SET_LO8(eax, 0); /* xor self */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fas = (int32_t)(int8_t)(_fa); /* xor result */
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

static inline void fast_sub_000824C0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_000824C0: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xDA254061u; /* flag setter tag */
    _fb = (uint32_t)(0x88) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x88));
    esp = esp - 0x88;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x1F693F8Cu; /* flag setter tag */
    PUSH32(esp, esi);
    esi = ecx;
    _cf = 0; /* xor clears CF */
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x71650E52u; /* flag setter tag */
    PUSH32(esp, edi);
    edi = MEM32(ebp + 8);
    MEM32(esi + 0x24) = ecx;
    MEM32(esi + 0x28) = ecx;
    MEM32(esi) = 0x17CF28;
    SET_LO16(eax, ZX8(MEM8(edi + 0x3E)));
    MEM16(esi + 0x20) = LO16(eax);
    SET_LO16(eax, ZX8(MEM8(edi + 0x3D)));
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(0xFF) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 0xFF (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x6CE999B8u; /* flag setter tag */
    MEM16(esi + 0x22) = LO16(eax);
    if (CMP_GE(_fas, _fbs)) goto loc_00082512; /* jge: greater or equal (signed >=) */

loc_000824F9: ;
    edx = ZX8(MEM8(edi + 0x3D));
    eax = MEM32(ebp + 0x10);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(edx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    edx = (uint32_t)(edx) << ((6) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x7E0ED333u; /* flag setter tag */
    SET_LO16(edx, ZX8(MEM8(edx + eax + 0x3D)));
    MEM16(esi + 0xD0) = LO16(edx);
    goto loc_0008251B;

loc_00082512: ;
    MEM16(esi + 0xD0) = 0xFF;

loc_0008251B: ;
    eax = MEM32(edi + 0x10);
    MEM32(esi + 0x10) = eax;
    edx = MEM32(edi + 0x14);
    MEM32(esi + 0x14) = edx;
    eax = MEM32(edi + 0x18);
    MEM32(esi + 0x18) = eax;
    eax = MEM32(ebp + 0xC);
    edx = 0x3F800000;
    MEM32(esi + 0xD4) = eax;
    MEM32(esi + 0x1C) = edx;
    SET_LO16(eax, ZX8(MEM8(edi + 0x3F)));
    MEM16(esi + 0xD2) = LO16(eax);
    eax = SX16(LO16(eax));
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(eax) < (uint32_t)(0xC));
    eax = eax - 0xC;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0xACE76FAAu; /* flag setter tag */
    if ((_fa == 0)) goto loc_0008259D; /* je: equal / zero */

loc_00082552: ;
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    if ((_fa != 0)) goto loc_0008261C; /* jne: not equal / not zero */

loc_00082559: ;
    MEM32(esi + 0xB8) = ecx;
    MEM32(esi + 0xB4) = ecx;
    MEM32(esi + 0xB0) = ecx;
    MEM32(esi + 0xBC) = edx;
    edx = MEM32(edi);
    ecx = esi + 0xC0;
    MEM32(ecx) = edx;
    eax = MEM32(edi + 4);
    MEM32(esi + 0xC4) = eax;
    edx = MEM32(edi + 8);
    PUSH32(esp, ecx);
    MEM32(esi + 0xC8) = edx;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082593u); fast_sub_00012950(); /* call 0x00012950 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082593: ;
    eax = esi;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 16; return; /* ret 12 */

loc_0008259D: ;
    eax = MEM32(edi + 0x28);
    ecx = MEM32(edi + 0x24);
    edx = MEM32(edi + 0x20);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = esp + 0x1C;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000825B2u); sub_00011C90(); /* call 0x00011C90 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000825B2: ;
    eax = MEM32(edi + 8);
    ecx = MEM32(edi + 4);
    edx = MEM32(edi);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = esp + 0x5C;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000825C6u); sub_00011C90(); /* call 0x00011C90 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000825C6: ;
    eax = esp + 0x50;
    PUSH32(esp, eax);
    ecx = esp + 0x14;
    PUSH32(esp, ecx);
    ecx = esp + 0x18;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000825D9u); sub_00011390(); /* call 0x00011390 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000825D9: ;
    edx = esp + 0x10;
    PUSH32(esp, edx);
    ecx = esi + 0xB0;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000825E9u); sub_000120E0(); /* call 0x000120E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000825E9: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi + 0x30)); /* fdiv dword ptr [edi + 0x30] */
    fp_top() = fabs(fp_top()); /* fabs */
    MEMF(esi + 0xC0) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi + 0x34)); /* fdiv dword ptr [edi + 0x34] */
    fp_top() = fabs(fp_top()); /* fabs */
    MEMF(esi + 0xC4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi + 0x38)); /* fdiv dword ptr [edi + 0x38] */
    fp_top() = fabs(fp_top()); /* fabs */
    MEMF(esi + 0xC8) = (float)fp_top(); fp_pop(); /* fstp */

loc_0008261C: ;
    POP32(esp, edi);
    eax = esi;
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : _cf)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 16; return; /* ret 12 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

static inline void fast_sub_00082630(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00082630: ;
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = ecx;
    eax = edi + 0xB0;
    esi = edi + 0x30;
    PUSH32(esp, eax);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082645u); sub_00011710(); /* call 0x00011710 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082645: ;
    ecx = MEM32(edi + 0x10);
    MEM32(esi + 0x30) = ecx;
    edx = MEM32(edi + 0x14);
    MEM32(esi + 0x34) = edx;
    eax = MEM32(edi + 0x18);
    MEM32(esi + 0x38) = eax;
    MEM32(esi + 0x3C) = 0x3F800000;
    SET_LO16(eax, MEM16(edi + 0x22));
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(0xFF) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 0xFF (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x6CE999B8u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0008267D; /* je: equal / zero */

loc_00082668: ;
    edx = MEM32(esp + 0xC);
    ecx = SX16(LO16(eax));
    if (((6) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((6) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x29DB4FF6u; /* flag setter tag */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(edx)) >> 32) & 1);
    ecx = ecx + edx;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD1D888B0u; /* flag setter tag */
    PUSH32(esp, esi);
    PUSH32(esp, ecx);
    ecx = esi;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008267Du); sub_00011250(); /* call 0x00011250 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008267D: ;
    PUSH32(esp, esi);
    ecx = edi + 0x70;
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : CMP_B(_fa, _fb))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082686u); sub_000111A0(); /* call 0x000111A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082686: ;
    POP32(esp, edi);
    POP32(esp, esi);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 8; return; /* ret 4 */

}

static inline void fast_sub_00082690(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_00082690: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xDA254061u; /* flag setter tag */
    _fb = (uint32_t)(0xD4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0xD4));
    esp = esp - 0xD4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x60EE5A65u; /* flag setter tag */
    eax = MEM32(ebp + 8);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(ebp + 0x10);
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    ecx = esp + 0x38;
    _cf = 0; /* xor clears CF */
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x61F5A291u; /* flag setter tag */
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000826B2u); sub_00012860(); /* call 0x00012860 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000826B2: ;
    ecx = MEM32(ebp + 0xC);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    ecx = esp + 0x48;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000826C0u); sub_00012860(); /* call 0x00012860 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000826C0: ;
    ecx = esp + 0x30;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000826C9u); fast_sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000826C9: ;
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = esp + 0x40;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000826D6u); fast_sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000826D6: ;
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176190)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176190] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000826EC; /* jp: parity */

loc_000826E7: ;
    ebx = 1;

loc_000826EC: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176190)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176190] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000826FC; /* jp: parity */

loc_000826F9: ;
    _cf = 0; /* logical op clears CF */
    ebx = ebx | 2;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0x396D938Cu; /* flag setter tag */

loc_000826FC: ;
    edx = esp + 0x30;
    PUSH32(esp, edx);
    eax = esp + 0x44;
    PUSH32(esp, eax);
    ecx = esp + 0x58;
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x396D938Cu ? (0) : CMP_B(_fa & _fb, 0))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008270Fu); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008270F: ;
    ecx = esp + 0x50;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082718u); fast_sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082718: ;
    MEMF(esp + 0xC) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_00082768; /* jnp: not parity */

loc_00082729: ;
    ecx = esp + 0x30;
    PUSH32(esp, ecx);
    ecx = esp + 0x54;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082737u); fast_sub_00012600(); /* call 0x00012600 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082737: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + 0xC)); /* fdiv dword ptr [esp + 0xc] */
    { double _t = fp_st1(); fp_push(_t); } /* fld st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x176190)); /* fsub dword ptr [0x176190] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0xC)); /* fmul dword ptr [esp + 0xc] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00082771; /* jp: parity */

loc_00082762: ;
    fp_pop(); /* fstp st(0) */
    fp_pop(); /* fstp st(0) */
    fp_pop(); /* fstp st(0) */

loc_00082768: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00082771: ;
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */
    edx = MEM32(ebp + 0x14);
    eax = MEM32(ebp + 0x18);
    esi = MEM32(ebp + 0x1C);
    _cf = 0; /* logical op clears CF */
    ebx = ebx | 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0xD0373272u; /* flag setter tag */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x063F0C18u; /* flag setter tag */
    { double _t = g_fp_stack[(g_fp_top + 2) & 7]; fp_push(_t); } /* fld st(2) */
    fp_top() = -fp_top(); /* fchs */
    fp_top() = RECOMP_FP_PC(fp_top() - fp_st1()); /* fsub st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    MEMF(edx) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() - g_fp_stack[(g_fp_top + 2) & 7]); /* fsub st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    fp_pop(); /* fstp st(0) */
    if (CMP_EQ(_fa, _fb)) goto loc_00082A06; /* je: equal / zero */

loc_0008279B: ;
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    ecx = esp + 0x68;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000827A6u); sub_00012860(); /* call 0x00012860 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000827A6: ;
    ecx = esp + 0x30;
    PUSH32(esp, ecx);
    edx = esp + 0x64;
    PUSH32(esp, edx);
    ecx = esp + 0x78;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000827B9u); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000827B9: ;
    eax = esp + 0x50;
    PUSH32(esp, eax);
    ecx = esp + 0x74;
    PUSH32(esp, ecx);
    ecx = esp + 0x88;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000827CFu); fast_sub_00012640(); /* call 0x00012640 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000827CF: ;
    ecx = esp + 0x80;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000827DBu); fast_sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000827DB: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1761a4] */
    ecx = esp + 0x80;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0008280A; /* jp: parity */

loc_000827EF: ;
    edx = esp + 0x50;
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, edx);
    eax = esp + 0x44;
    PUSH32(esp, eax);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082800u); fast_sub_00012640(); /* call 0x00012640 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082800: ;
    PUSH32(esp, eax);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082808u); fast_sub_00012950(); /* call 0x00012950 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082808: ;
    goto loc_00082823;

loc_0008280A: ;
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */
    PUSH32(esp, ecx);
    PUSH32(esp, ecx);
    ecx = esp + 0x88;
    fp_top() = RECOMP_FP_PC(MEMF(0x176190) / fp_top()); /* fdivr dword ptr [0x176190] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082823u); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082823: ;
    edx = esp + 0x80;
    PUSH32(esp, edx);
    eax = esp + 0x54;
    PUSH32(esp, eax);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082837u); fast_sub_00012640(); /* call 0x00012640 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082837: ;
    PUSH32(esp, eax);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008283Fu); fast_sub_00012950(); /* call 0x00012950 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008283F: ;
    PUSH32(esp, esi);
    ecx = esp + 0x84;
    PUSH32(esp, ecx);
    ecx = esp + 0x98;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082854u); fast_sub_00012640(); /* call 0x00012640 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082854: ;
    edx = MEM32(esp + 0x90);
    eax = MEM32(esp + 0x94);
    ecx = MEM32(esp + 0x98);
    MEM32(esp + 0xA0) = edx;
    edx = MEM32(esp + 0x9C);
    MEM32(esp + 0xB0) = eax;
    eax = MEM32(esp + 0x80);
    MEM32(esp + 0xC0) = ecx;
    ecx = MEM32(esp + 0x84);
    MEM32(esp + 0xD0) = edx;
    edx = MEM32(esp + 0x88);
    MEM32(esp + 0xA4) = eax;
    eax = MEM32(esp + 0x8C);
    MEM32(esp + 0xB4) = ecx;
    ecx = MEM32(esi);
    MEM32(esp + 0xC4) = edx;
    edx = MEM32(esi + 4);
    MEM32(esp + 0xD4) = eax;
    eax = MEM32(esi + 8);
    MEM32(esp + 0xA8) = ecx;
    ecx = MEM32(esi + 0xC);
    MEM32(esp + 0xB8) = edx;
    edx = esp + 0x40;
    MEM32(esp + 0xC8) = eax;
    PUSH32(esp, edx);
    eax = esp + 0xA4;
    MEM32(esp + 0xDC) = ecx;
    PUSH32(esp, eax);
    ecx = esp + 0x18;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082901u); fast_sub_000127F0(); /* call 0x000127F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082901: ;
    ecx = esp + 0x60;
    PUSH32(esp, ecx);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008290Du); fast_sub_00012600(); /* call 0x00012600 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008290D: ;
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0xC);
    ecx = MEM32(esp + 0x18);
    edx = MEM32(esp + 0x14);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    MEM32(esi) = 0;
    MEM32(esi + 4) = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008292Du); fast_sub_00082470(); /* call 0x00082470 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008292D: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0008293C; /* je: equal / zero */

loc_00082934: ;
    fp_push(MEMF(0x1761A0)); /* fld float */
    goto loc_00082940;

loc_0008293C: ;
    fp_push(MEMF(esp + 0x18)); /* fld float */

loc_00082940: ;
    MEMF(esi + 8) = (float)fp_top(); /* fst */
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A8)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a8] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_000829F3; /* jne: not equal / not zero */

loc_00082958: ;
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    PUSH32(esp, esi);
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    PUSH32(esp, ecx);
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    ecx = esi;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x1C)); /* fmul dword ptr [esp + 0x1c] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x42694A75u; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00082985; /* jne: not equal / not zero */

loc_0008297B: ;
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */
    fp_top() = RECOMP_FP_PC(MEMF(0x176190) / fp_top()); /* fdivr dword ptr [0x176190] */
    goto loc_00082997;

loc_00082985: ;
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x18)); /* fmul dword ptr [esp + 0x18] */
    fp_top() = RECOMP_FP_PC(MEMF(0x176190) - fp_top()); /* fsubr dword ptr [0x176190] */
    fp_st1() = RECOMP_FP_PC(fp_top() / fp_st1()); fp_pop(); /* fdivrp st(1) */
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */

loc_00082997: ;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008299Fu); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008299F: ;
    fp_push(MEMF(esi + 4)); /* fld float */
    edx = esp + 0xA0;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x14)); /* fsub dword ptr [esp + 0x14] */
    PUSH32(esp, edx);
    ecx = edx;
    MEMF(esi + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x1C)); /* fsub dword ptr [esp + 0x1c] */
    MEMF(esi + 8) = (float)fp_top(); fp_pop(); /* fstp */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000829C2u); sub_00011130(); /* call 0x00011130 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000829C2: ;
    PUSH32(esp, esi);
    eax = esp + 0xA4;
    PUSH32(esp, eax);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000829D2u); fast_sub_000127F0(); /* call 0x000127F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000829D2: ;
    fp_push(MEMF(esi)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi)); /* fdiv dword ptr [edi] */
    eax = ebx;
    MEMF(esi) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi + 4)); /* fdiv dword ptr [edi + 4] */
    MEMF(esi + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi + 8)); /* fdiv dword ptr [edi + 8] */
    MEMF(esi + 8) = (float)fp_top(); fp_pop(); /* fstp */
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_000829F3: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    fp_pop(); /* fstp st(0) */
    MEM32(esi) = eax;
    MEM32(esi + 4) = eax;
    MEM32(esi + 8) = eax;
    MEM32(esi + 0xC) = 0x3F800000;

loc_00082A06: ;
    POP32(esp, edi);
    POP32(esp, esi);
    eax = ebx;
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x063F0C18u ? (CMP_B(_fa, _fb)) : 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

static inline void fast_sub_00082A10(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_00082A10: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xDA254061u; /* flag setter tag */
    _fb = (uint32_t)(0x54) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x54));
    esp = esp - 0x54;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0xFF7B62C0u; /* flag setter tag */
    eax = MEM32(ebp + 0x1C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    ebx = ecx;
    if (CMP_NE(_fa, _fb)) goto loc_00082A2B; /* jne: not equal / not zero */

loc_00082A25: ;
    eax = ebx + 0x30;
    MEM32(ebp + 0x1C) = eax;

loc_00082A2B: ;
    edi = MEM32(ebp + 0x20);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00082A35; /* jne: not equal / not zero */

loc_00082A32: ;
    edi = ebx + 0x70;

loc_00082A35: ;
    _fa = (uint32_t)(MEM16(ebx + 0xD2)) & 0xFFFFu; _fb = (uint32_t)(0xC) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(ebx + 0xD2), 0xC (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x5E72863Cu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00082AAE; /* jne: not equal / not zero */

loc_00082A3F: ;
    ecx = MEM32(ebp + 8);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    ecx = esp + 0x48;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082A4Du); fast_sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082A4D: ;
    edx = MEM32(ebp + 0xC);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    ecx = esp + 0x58;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082A5Bu); fast_sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082A5B: ;
    esi = MEM32(ebp + 0x18);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x063F0C18u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00082A6B; /* je: equal / zero */

loc_00082A62: ;
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    ecx = esi;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082A6Bu); fast_sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082A6B: ;
    eax = MEM32(ebp + 0x14);
    ecx = MEM32(ebp + 0x10);
    PUSH32(esp, esi);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    _fb = (uint32_t)(0xC0) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0xC0)) >> 32) & 1);
    ebx = ebx + 0xC0;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x34283E20u; /* flag setter tag */
    PUSH32(esp, ebx);
    edx = esp + 0x60;
    PUSH32(esp, edx);
    eax = esp + 0x54;
    PUSH32(esp, eax);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082A8Au); fast_sub_00082690(); /* call 0x00082690 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082A8A: ;
    edi = eax;
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x18)) >> 32) & 1);
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBD67F1D5u; /* flag setter tag */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00082AA3; /* je: equal / zero */

loc_00082A93: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x063F0C18u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00082AA3; /* je: equal / zero */

loc_00082A97: ;
    ecx = MEM32(ebp + 0x1C);
    PUSH32(esp, esi);
    PUSH32(esp, ecx);
    ecx = esi;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082AA3u); fast_sub_000127F0(); /* call 0x000127F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082AA3: ;
    eax = edi;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x8C6ED7C1u ? (CMP_B(_fa, _fb)) : (_fk == 0x063F0C18u ? (CMP_B(_fa, _fb)) : (g_eflags & 1u) /* eflags */))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 32; return; /* ret 28 */

loc_00082AAE: ;
    edx = MEM32(ebp + 0xC);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    ecx = esp + 0x28;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082ABCu); fast_sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082ABC: ;
    esi = ebx + 0xC0;
    PUSH32(esp, esi);
    ecx = esp + 0x24;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082ACCu); fast_sub_00012600(); /* call 0x00012600 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082ACC: ;
    MEMF(esp + 0xC) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00082BBA; /* jp: parity */

loc_00082AE1: ;
    ebx = MEM32(ebp + 0x18);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00082B9A; /* je: equal / zero */

loc_00082AEC: ;
    fp_push(MEMF(esp + 0xC)); /* fld float */
    PUSH32(esp, esi);
    PUSH32(esp, ecx);
    fp_top() = -fp_top(); /* fchs */
    ecx = esp + 0x48;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B00u); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B00: ;
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B0Du); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B0D: ;
    edx = MEM32(ebp + 8);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    ecx = esp + 0x38;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B1Bu); fast_sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B1B: ;
    eax = esp + 0x30;
    PUSH32(esp, eax);
    ecx = esp + 0x44;
    PUSH32(esp, ecx);
    ecx = esp + 0x18;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B2Eu); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B2E: ;
    PUSH32(esp, esi);
    edx = esp + 0x14;
    PUSH32(esp, edx);
    ecx = esp + 0x18;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B3Du); fast_sub_00012640(); /* call 0x00012640 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B3D: ;
    eax = esp + 0x10;
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    ecx = esp + 0x18;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B4Cu); fast_sub_00012640(); /* call 0x00012640 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B4C: ;
    PUSH32(esp, eax);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B54u); fast_sub_00012950(); /* call 0x00012950 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B54: ;
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    ecx = esp + 0x34;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B62u); fast_sub_00012BC0(); /* call 0x00012BC0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B62: ;
    PUSH32(esp, ecx);
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    edx = esp + 0x14;
    PUSH32(esp, edx);
    eax = esp + 0x38;
    PUSH32(esp, eax);
    ecx = esp + 0x4C;
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B7Au); fast_sub_000823E0(); /* call 0x000823E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B7A: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    edx = esp + 0x20;
    PUSH32(esp, edx);
    eax = esp + 0x44;
    PUSH32(esp, eax);
    ecx = ebx;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B8Eu); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B8E: ;
    ecx = MEM32(ebp + 0x1C);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00082B9Au); fast_sub_000127F0(); /* call 0x000127F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00082B9A: ;
    edx = MEM32(ebp + 0x14);
    eax = MEM32(ebp + 0x10);
    MEM32(edx) = 0x3F000000;
    MEM32(eax) = 0x3F000000;
    eax = 7;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x711257E0u ? (CMP_B(_fa, _fb)) : (g_eflags & 1u) /* eflags */)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 32; return; /* ret 28 */

loc_00082BBA: ;
    POP32(esp, edi);
    POP32(esp, esi);
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 32; return; /* ret 28 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

void sc2_fast_sub_000843D0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_000843D0: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xDA254061u; /* flag setter tag */
    eax = 0x60B4;
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000843E0u); fast_alloca_probe(); /* call 0x00145C40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000843E0: ;
    eax = MEM32(0x3706E4);
    PUSH32(esp, ebx);
    MEM32(esp + 0x60B4) = eax;
    eax = MEM32(0x3CA0F0);
    _cf = 0; /* xor clears CF */
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x61F5A291u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x9A93F82Au; /* flag setter tag */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    MEM32(esp + 0x38) = ecx;
    if (CMP_NE(_fa, _fb)) goto loc_00084E7C; /* jne: not equal / not zero */

loc_00084402: ;
    eax = (uint32_t)(int32_t)SMEM16(ecx + 0x48);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x9A93F82Au; /* flag setter tag */
    edi = MEM32(ecx + 0x44);
    MEM32(esp + 0x90) = ebx;
    MEM32(esp + 0x94) = ebx;
    esi = esp + 0x20F0;
    if (CMP_LE(_fas, _fbs)) goto loc_0008447B; /* jle: less or equal (signed <=) */

loc_00084422: ;
    MEM32(esp + 0x14) = eax;

loc_00084426: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00084451; /* jne: not equal / not zero */

loc_0008442A: ;
    SET_LO8(eax, MEM8(edi + 0x50));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00084451; /* je: equal / zero */

loc_00084431: ;
    PUSH32(esp, edi);
    ecx = esi;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084439u); sub_00082C50(); /* call 0x00082C50 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084439: ;
    eax = MEM32(ebp + 8);
    PUSH32(esp, eax);
    ecx = esp + 0x20F4;
    PUSH32(esp, ecx);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008444Cu); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008444C: ;
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x50)) >> 32) & 1);
    esi = esi + 0x50;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x70EB5E23u; /* flag setter tag */
    goto loc_00084461;

loc_00084451: ;
    ecx = esi;
    PUSH32(esp, edi);
    ebx = 1;
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x50)) >> 32) & 1);
    esi = esi + 0x50;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x70EB5E23u; /* flag setter tag */
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084461u); sub_00082C50(); /* call 0x00082C50 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084461: ;
    eax = MEM32(esp + 0x14);
    edi = MEM32(edi + 0x24);
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x14) = eax;
    if ((_fa != 0)) goto loc_00084426; /* jne: not equal / not zero */

loc_0008446F: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00084D32; /* jne: not equal / not zero */

loc_00084477: ;
    ecx = MEM32(esp + 0x38);

loc_0008447B: ;
    eax = (uint32_t)(int32_t)SMEM16(ecx + 0x48);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    edi = esp + 0x20F0;
    if (CMP_LE(_fas, _fbs)) goto loc_00084585; /* jle: less or equal (signed <=) */

loc_0008448E: ;
    MEM32(esp + 0x18) = eax;

loc_00084492: ;
    eax = edi + 0x30;
    ebx = eax + 0x10;
    MEM32(esp + 0x20) = eax;
    eax = ZX8(MEM8(eax + -22));
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_00084573; /* jle: less or equal (signed <=) */

loc_000844A8: ;
    MEM32(esp + 0x1C) = eax;
    /* nop */

loc_000844B0: ;
    eax = ZX8(MEM8(ebx));
    edx = MEM32(esp + 0x20);
    esi = eax + eax * 4;
    eax = MEM32(edx);
    if (((4) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((4) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((4) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x2653F05Au; /* flag setter tag */
    esi = esp + esi + 0x20F0;
    PUSH32(esp, esi);
    ecx = edi;
    MEM32(esp + 0x14) = eax;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000844D2u); fast_sub_00012BC0(); /* call 0x00012BC0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000844D2: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x10)); /* fmul dword ptr [esp + 0x10] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp st(1) */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00084556; /* jp: parity */

loc_000844E3: ;
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    ecx = esp + 0x48;
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000844F4u); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000844F4: ;
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x10)); /* fsub dword ptr [esp + 0x10] */
    ecx = esp + 0x40;
    PUSH32(esp, ecx);
    PUSH32(esp, ecx);
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + 0x1C)); /* fdiv dword ptr [esp + 0x1c] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1761AC)); /* fmul dword ptr [0x1761ac] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084514u); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084514: ;
    edx = esp + 0x40;
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084521u); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084521: ;
    eax = esp + 0x40;
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008452Eu); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008452E: ;
    ecx = MEM32(ebp + 8);
    PUSH32(esp, ecx);
    edx = esp + 0x20F4;
    PUSH32(esp, edx);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084541u); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084541: ;
    eax = MEM32(ebp + 8);
    PUSH32(esp, eax);
    ecx = esp + 0x20F4;
    PUSH32(esp, ecx);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084554u); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084554: ;
    goto loc_00084558;

loc_00084556: ;
    fp_pop(); /* fstp st(0) */

loc_00084558: ;
    ecx = MEM32(esp + 0x20);
    eax = MEM32(esp + 0x1C);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(4)) >> 32) & 1);
    ecx = ecx + 4;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xAD912013u; /* flag setter tag */
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x39B6C9A7u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x20) = ecx;
    MEM32(esp + 0x1C) = eax;
    if ((_fa != 0)) goto loc_000844B0; /* jne: not equal / not zero */

loc_00084573: ;
    eax = MEM32(esp + 0x18);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x50)) >> 32) & 1);
    edi = edi + 0x50;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x8F6E6588u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x18) = eax;
    if ((_fa != 0)) goto loc_00084492; /* jne: not equal / not zero */

loc_00084585: ;
    edx = MEM32(esp + 0x38);
    eax = (uint32_t)(int32_t)SMEM16(edx + 0x48);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    edi = esp + 0x20F0;
    MEM32(esp + 0x28) = edi;
    if (CMP_LE(_fas, _fbs)) goto loc_00084938; /* jle: less or equal (signed <=) */

loc_000845A0: ;
    MEM32(esp + 0x24) = eax;

loc_000845A4: ;
    ebx = MEM32(ebp + 8);
    eax = MEM32(edi + 0x1C);
    ecx = esp + 0x40;
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    edx = esp + 0x20F8;
    PUSH32(esp, edx);
    ecx = edi;
    MEM32(esp + 0x40) = eax;
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : _cf)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000845C3u); fast_sub_00082D80(); /* call 0x00082D80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000845C3: ;
    SET_LO8(eax, MEM8(edi + 0x48));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    ecx = esp + 0x20F0;
    if (CMP_EQ(_fa, _fb)) goto loc_00084615; /* je: equal / zero */

loc_000845D1: ;
    eax = ZX8(MEM8(edi + 0x40));
    esi = eax + eax * 4;
    eax = esp + 0xA0;
    PUSH32(esp, eax);
    if (((4) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((4) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((4) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x2653F05Au; /* flag setter tag */
    PUSH32(esp, ebx);
    esi = esp + esi + 0x20F8;
    PUSH32(esp, ecx);
    ecx = esi;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000845F3u); fast_sub_00082D80(); /* call 0x00082D80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000845F3: ;
    edx = esp + 0xA0;
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084603u); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084603: ;
    ecx = esp + 0x40;
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084610u); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084610: ;
    goto loc_00084922;

loc_00084615: ;
    PUSH32(esp, ebx);
    edx = edi + 0x20;
    eax = edi + 0x44;
    PUSH32(esp, ecx);
    ecx = edi;
    MEM32(esp + 0x20) = edx;
    MEM32(esp + 0x24) = eax;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008462Cu); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008462C: ;
    eax = ZX8(MEM8(edi + 0x19));
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_00084922; /* jle: less or equal (signed <=) */

loc_00084638: ;
    MEM32(esp + 0x14) = eax;
    /* nop */

loc_00084640: ;
    eax = MEM32(esp + 0x1C);
    ecx = ZX8(MEM8(eax));
    edx = MEM32(esp + 0x18);
    ebx = MEM32(edx);
    edx = ecx;
    if (((7) & 31u)) _cf = (int)(((uint32_t)(edx) >> ((32 - (((7) & 31u))) & 31u)) & 1u);
    edx = (uint32_t)(edx) << ((7) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xE4BAE159u; /* flag setter tag */
    eax = esp + edx + 0xF0;
    MEM32(esp + 0x20) = eax;
    eax = 1;
    _cf = 0; /* xor clears CF */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x0296481Bu; /* flag setter tag */
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084669u); fast_allshl(); /* call 0x00145D40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084669: ;
    esi = MEM32(esp + 0x90);
    ecx = eax;
    _cf = 0; /* logical op clears CF */
    ecx = ecx & esi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xE75D52DFu; /* flag setter tag */
    esi = edx;
    MEM32(esp + 0x64) = edx;
    _cf = 0; /* logical op clears CF */
    esi = esi & MEM32(esp + 0x94);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xD9176213u; /* flag setter tag */
    _cf = 0; /* logical op clears CF */
    ecx = ecx | esi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0x77B20B66u; /* flag setter tag */
    if ((_fa != 0)) goto loc_000846CA; /* jne: not equal / not zero */

loc_00084685: ;
    edx = MEM32(esp + 0x20);
    esi = ebx + 0x30;
    ecx = 0x10;
    edi = edx;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    esi = ebx + 0x70;
    edi = edx + 0x40;
    ecx = 0x10;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    ecx = MEM32(esp + 0x90);
    edi = MEM32(esp + 0x28);
    _cf = 0; /* logical op clears CF */
    ecx = ecx | eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0xFBFE86F8u; /* flag setter tag */
    eax = MEM32(esp + 0x94);
    MEM32(esp + 0x90) = ecx;
    ecx = MEM32(esp + 0x64);
    _cf = 0; /* logical op clears CF */
    eax = eax | ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0xDADFDF7Eu; /* flag setter tag */
    MEM32(esp + 0x94) = eax;

loc_000846CA: ;
    edx = MEM32(esp + 0x34);
    esi = MEM32(esp + 0x20);
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    ecx = esp + 0x58;
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000846DDu); fast_sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000846DD: ;
    PUSH32(esp, edi);
    eax = esp + 0x54;
    PUSH32(esp, eax);
    ecx = esp + 0x88;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000846EFu); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000846EF: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F000000);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000846FCu); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000846FC: ;
    ecx = esp + 0x40;
    PUSH32(esp, ecx);
    edx = esp + 0x84;
    PUSH32(esp, edx);
    ecx = esp + 0x88;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084715u); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084715: ;
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0008471Cu); fast_sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0008471C: ;
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */
    eax = esp + 0x80;
    PUSH32(esp, eax);
    fp_top() = RECOMP_FP_PC(MEMF(edi + 0x10) / fp_top()); /* fdivr dword ptr [edi + 0x10] */
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x30);
    PUSH32(esp, ecx);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084739u); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084739: ;
    edx = esp + 0x40;
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084746u); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084746: ;
    eax = MEM32(esp + 0x50);
    ecx = MEM32(esp + 0x54);
    edx = MEM32(esp + 0x58);
    MEM32(esp + 0x70) = eax;
    eax = MEM32(esp + 0x5C);
    MEM32(esp + 0x7C) = eax;
    eax = esi + 0x40;
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    MEM32(esp + 0x7C) = ecx;
    MEM32(esp + 0x80) = edx;
    ecx = esp + 0x78;
    PUSH32(esp, ecx);
    edx = esp + 0x48;
    PUSH32(esp, edx);
    eax = esp + 0x20;
    PUSH32(esp, eax);
    ecx = esp + 0x94;
    PUSH32(esp, ecx);
    edx = esp + 0x58;
    PUSH32(esp, edx);
    ecx = ebx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084791u); fast_sub_00082A10(); /* call 0x00082A10 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084791: ;
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xFB437D9Eu; /* flag setter tag */
    MEM32(esp + 0x3C) = eax;
    if (TEST_Z(_fa, _fb)) goto loc_0008481A; /* je: equal / zero */

loc_0008479D: ;
    SET_LO8(eax, MEM8(edi + 0x18));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_0008481A; /* jl: less (signed <) */

loc_000847A4: ;
    ecx = SX8(LO8(eax));
    ecx = ecx + ecx * 4;
    if (((4) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((4) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((4) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xC7C22D63u; /* flag setter tag */
    SET_LO8(eax, MEM8(esp + ecx + 0x2109));
    ecx = esp + ecx + 0x20F0;
    MEM8(esp + 0x33) = LO8(eax);
    eax = ZX8(LO8(eax));
    _cf = 0; /* xor clears CF */
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x9F87D19Fu; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    edx = ecx + 0x20;
    MEM32(esp + 0x2C) = eax;
    if (CMP_LE(_fas, _fbs)) goto loc_000847E0; /* jle: less or equal (signed <=) */

loc_000847CF: ;
    /* nop */

loc_000847D0: ;
    _fa = (uint32_t)(MEM32(edx)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(edx), ebx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x21BC5B9Cu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000847E0; /* je: equal / zero */

loc_000847D4: ;
    eax = ZX8(MEM8(ecx + 0x19));
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x0AB4FBC0u; /* flag setter tag */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(4)) >> 32) & 1);
    edx = edx + 4;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xFA44BCD6u; /* flag setter tag */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, eax (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x66524C06u; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_000847D0; /* jl: less (signed <) */

loc_000847E0: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(MEM32(esp + 0x2C)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, MEM32(esp + 0x2C) (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x584F9A7Du; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_0008481A; /* jl: less (signed <) */

loc_000847E6: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x017CCF73u; /* flag setter tag */
    if (CMP_GE(_fas, _fbs)) goto loc_0008481A; /* jge: greater or equal (signed >=) */

loc_000847EB: ;
    SET_LO8(eax, MEM8(esp + 0x33));
    SET_LO8(eax, LO8(eax) + 1);
    _fa = (uint32_t)(LO8(eax)) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fb = (_fa == 0x80u); /* inc result/SF/OF; CF unchanged */
    _fk = 0xA41938D9u; /* flag setter tag */
    MEM8(ecx + 0x19) = LO8(eax);
    SET_LO16(edx, ZX8(LO8(eax)));
    eax = MEM32(ecx + 0x14);
    MEM16(eax + 0x5A) = LO16(edx);
    eax = MEM32(ecx + 0x1C);
    edx = esi + esi * 2;
    if (((5) & 31u)) _cf = (int)(((uint32_t)(edx) >> ((32 - (((5) & 31u))) & 31u)) & 1u);
    edx = (uint32_t)(edx) << ((5) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x0AA383CCu; /* flag setter tag */
    MEM32(edx + eax + 0x50) = ebx;
    MEM32(ecx + esi * 4 + 0x20) = ebx;
    SET_LO8(edx, MEM8(ebx + 0xD4));
    MEM8(esi + ecx + 0x44) = LO8(edx);

loc_0008481A: ;
    eax = MEM32(esp + 0x3C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00084869; /* je: equal / zero */

loc_00084822: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00084869; /* jne: not equal / not zero */

loc_00084833: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x17CAF0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x17caf0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00084869; /* jp: parity */

loc_00084844: ;
    eax = esp + 0x70;
    PUSH32(esp, eax);
    ecx = esp + 0x84;
    PUSH32(esp, ecx);
    ecx = edi;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084858u); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084858: ;
    edx = MEM32(ebp + 8);
    PUSH32(esp, edx);
    eax = esp + 0x20F4;
    PUSH32(esp, eax);
    goto loc_000848ED;

loc_00084869: ;
    ecx = MEM32(esp + 0x50);
    eax = MEM32(esp + 0x58);
    edx = MEM32(esp + 0x54);
    MEM32(esp + 0x70) = ecx;
    ecx = MEM32(esp + 0x5C);
    MEM32(esp + 0x7C) = ecx;
    ecx = MEM32(esp + 0x20);
    MEM32(esp + 0x78) = eax;
    eax = ecx + 0x40;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    MEM32(esp + 0x7C) = edx;
    edx = esp + 0x78;
    PUSH32(esp, edx);
    eax = esp + 0x48;
    PUSH32(esp, eax);
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    edx = esp + 0x58;
    PUSH32(esp, edx);
    ecx = ebx;
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : (_fk == 0x452A891Eu ? (CMP_B(_fa & _fb, 0)) : CMP_B(_fa & _fb, 0)))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000848AEu); fast_sub_00082A10(); /* call 0x00082A10 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000848AE: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000848F4; /* je: equal / zero */

loc_000848B2: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_000848F4; /* jne: not equal / not zero */

loc_000848C3: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x17CAF0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x17caf0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000848F4; /* jp: parity */

loc_000848D4: ;
    eax = esp + 0x70;
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    ecx = edi;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000848E1u); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000848E1: ;
    ecx = MEM32(ebp + 8);
    PUSH32(esp, ecx);
    edx = esp + 0x20F4;
    PUSH32(esp, edx);

loc_000848ED: ;
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000848F4u); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000848F4: ;
    esi = MEM32(esp + 0x34);
    edx = MEM32(esp + 0x18);
    ecx = MEM32(esp + 0x1C);
    eax = MEM32(esp + 0x14);
    _fb = (uint32_t)(0x60) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x60)) >> 32) & 1);
    esi = esi + 0x60;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x344CC3A9u; /* flag setter tag */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(4)) >> 32) & 1);
    edx = edx + 4;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xFA44BCD6u; /* flag setter tag */
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x53735CF0u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x34) = esi;
    MEM32(esp + 0x18) = edx;
    MEM32(esp + 0x1C) = ecx;
    MEM32(esp + 0x14) = eax;
    if ((_fa != 0)) goto loc_00084640; /* jne: not equal / not zero */

loc_00084922: ;
    eax = MEM32(esp + 0x24);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x50)) >> 32) & 1);
    edi = edi + 0x50;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x8F6E6588u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x28) = edi;
    MEM32(esp + 0x24) = eax;
    if ((_fa != 0)) goto loc_000845A4; /* jne: not equal / not zero */

loc_00084938: ;
    eax = MEM32(esp + 0x38);
    eax = (uint32_t)(int32_t)SMEM16(eax + 0x48);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    edi = esp + 0x20F0;
    if (CMP_LE(_fas, _fbs)) goto loc_00084A47; /* jle: less or equal (signed <=) */

loc_0008494F: ;
    MEM32(esp + 0x14) = eax;

loc_00084953: ;
    eax = edi + 0x30;
    ebx = eax + 0x10;
    MEM32(esp + 0x18) = eax;
    eax = ZX8(MEM8(eax + -22));
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_00084A35; /* jle: less or equal (signed <=) */

loc_00084969: ;
    MEM32(esp + 0x28) = eax;
    /* nop */

loc_00084970: ;
    eax = ZX8(MEM8(ebx));
    ecx = MEM32(esp + 0x18);
    edx = MEM32(ecx);
    esi = eax + eax * 4;
    if (((4) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((4) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((4) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x2653F05Au; /* flag setter tag */
    esi = esp + esi + 0x20F0;
    PUSH32(esp, esi);
    ecx = edi;
    MEM32(esp + 0x20) = edx;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084992u); fast_sub_00012BC0(); /* call 0x00012BC0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084992: ;
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x1C)); /* fmul dword ptr [esp + 0x1c] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp st(1) */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00084A18; /* jp: parity */

loc_000849A3: ;
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    ecx = esp + 0x48;
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000849B4u); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000849B4: ;
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x1C)); /* fsub dword ptr [esp + 0x1c] */
    eax = esp + 0x40;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + 0x2C)); /* fdiv dword ptr [esp + 0x2c] */
    ecx = eax;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1761AC)); /* fmul dword ptr [0x1761ac] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000849D6u); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000849D6: ;
    ecx = esp + 0x40;
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000849E3u); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000849E3: ;
    edx = esp + 0x40;
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000849F0u); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000849F0: ;
    eax = MEM32(ebp + 8);
    PUSH32(esp, eax);
    ecx = esp + 0x20F4;
    PUSH32(esp, ecx);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084A03u); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084A03: ;
    edx = MEM32(ebp + 8);
    PUSH32(esp, edx);
    eax = esp + 0x20F4;
    PUSH32(esp, eax);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084A16u); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084A16: ;
    goto loc_00084A1A;

loc_00084A18: ;
    fp_pop(); /* fstp st(0) */

loc_00084A1A: ;
    ecx = MEM32(esp + 0x18);
    eax = MEM32(esp + 0x28);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(4)) >> 32) & 1);
    ecx = ecx + 4;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xAD912013u; /* flag setter tag */
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x39B6C9A7u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x18) = ecx;
    MEM32(esp + 0x28) = eax;
    if ((_fa != 0)) goto loc_00084970; /* jne: not equal / not zero */

loc_00084A35: ;
    eax = MEM32(esp + 0x14);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x50)) >> 32) & 1);
    edi = edi + 0x50;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x8F6E6588u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x14) = eax;
    if ((_fa != 0)) goto loc_00084953; /* jne: not equal / not zero */

loc_00084A47: ;
    ecx = MEM32(esp + 0x38);
    eax = (uint32_t)(int32_t)SMEM16(ecx + 0x48);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    edi = esp + 0x20F0;
    if (CMP_LE(_fas, _fbs)) goto loc_00084D32; /* jle: less or equal (signed <=) */

loc_00084A5E: ;
    MEM32(esp + 0x2C) = eax;

loc_00084A62: ;
    ecx = MEM32(edi + 0x1C);
    eax = edi + 0x20;
    edx = eax + 0x24;
    MEM32(esp + 0x20) = eax;
    eax = ZX8(MEM8(eax + -5));
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    MEM32(esp + 0x14) = edx;
    if (CMP_LE(_fas, _fbs)) goto loc_00084D20; /* jle: less or equal (signed <=) */

loc_00084A7F: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x10)) >> 32) & 1);
    ecx = ecx + 0x10;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x04162A1Bu; /* flag setter tag */
    MEM32(esp + 0x34) = ecx;
    MEM32(esp + 0x28) = eax;
    /* nop */

loc_00084A90: ;
    edx = MEM32(esp + 0x14);
    eax = MEM32(esp + 0x20);
    ebx = ZX8(MEM8(edx));
    ecx = MEM32(eax);
    SET_LO8(eax, MEM8(edi + 0x1A));
    if (((7) & 31u)) _cf = (int)(((uint32_t)(ebx) >> ((32 - (((7) & 31u))) & 31u)) & 1u);
    ebx = (uint32_t)(ebx) << ((7) & 31u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x25D3FB5Du; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    MEM32(esp + 0x24) = ecx;
    ebx = esp + ebx + 0xF0;
    MEM32(esp + 0x1C) = 0;
    if (CMP_BE(_fa, _fb)) goto loc_00084CF2; /* jbe: below or equal (unsigned <=) */

loc_00084ABE: ;
    eax = MEM32(esp + 0x34);
    MEM32(esp + 0x18) = eax;
    goto loc_00084AD0;

loc_00084AD0: ;
    ecx = MEM32(esp + 0x1C);
    eax = ZX8(MEM8(edi + ecx + 0x40));
    esi = eax + eax * 4;
    eax = MEM32(ebp + 8);
    edx = esp + 0x90;
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = esp + 0x20F8;
    PUSH32(esp, ecx);
    if (((4) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((4) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((4) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x2653F05Au; /* flag setter tag */
    ecx = edi;
    esi = esp + esi + 0x20FC;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B01u); fast_sub_00082D80(); /* call 0x00082D80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B01: ;
    eax = MEM32(ebp + 8);
    edx = esp + 0xA0;
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = esp + 0x20F8;
    PUSH32(esp, ecx);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B1Cu); fast_sub_00082D80(); /* call 0x00082D80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B1C: ;
    edx = esp + 0xA0;
    PUSH32(esp, edx);
    eax = esp + 0x94;
    PUSH32(esp, eax);
    ecx = esp + 0x78;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B35u); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B35: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F000000);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B42u); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B42: ;
    ecx = MEM32(esp + 0x18);
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    ecx = esp + 0x58;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B51u); fast_sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B51: ;
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    ecx = esp + 0x48;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B5Cu); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B5C: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F000000);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B69u); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B69: ;
    edx = esp + 0x40;
    PUSH32(esp, edx);
    eax = esp + 0x54;
    PUSH32(esp, eax);
    ecx = esp + 0x88;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B7Fu); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B7F: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F000000);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084B8Cu); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084B8C: ;
    eax = MEM32(esp + 0x58);
    edx = MEM32(esp + 0x54);
    ecx = MEM32(esp + 0x50);
    MEM32(esp + 0x68) = eax;
    eax = ebx + 0x40;
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    MEM32(esp + 0x68) = ecx;
    ecx = MEM32(esp + 0x64);
    MEM32(esp + 0x6C) = edx;
    edx = esp + 0x68;
    PUSH32(esp, edx);
    MEM32(esp + 0x78) = ecx;
    eax = esp + 0x48;
    PUSH32(esp, eax);
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    ecx = MEM32(esp + 0x38);
    edx = esp + 0x94;
    PUSH32(esp, edx);
    eax = esp + 0x88;
    PUSH32(esp, eax);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084BD9u); fast_sub_00082A10(); /* call 0x00082A10 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084BD9: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00084C22; /* je: equal / zero */

loc_00084BDD: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00084C22; /* jne: not equal / not zero */

loc_00084BEE: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176190)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176190] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00084C22; /* jp: parity */

loc_00084BFF: ;
    ecx = esp + 0x80;
    PUSH32(esp, ecx);
    edx = esp + 0x64;
    PUSH32(esp, edx);
    ecx = edx;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084C13u); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084C13: ;
    ecx = esp + 0x40;
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084C20u); fast_sub_000125A0(); /* call 0x000125A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084C20: ;
    goto loc_00084C92;

loc_00084C22: ;
    eax = MEM32(esp + 0x54);
    edx = MEM32(esp + 0x50);
    ecx = MEM32(esp + 0x58);
    MEM32(esp + 0x64) = eax;
    eax = ebx + 0x40;
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    MEM32(esp + 0x68) = edx;
    edx = MEM32(esp + 0x64);
    MEM32(esp + 0x70) = ecx;
    eax = esp + 0x68;
    PUSH32(esp, eax);
    ecx = esp + 0x48;
    PUSH32(esp, ecx);
    MEM32(esp + 0x7C) = edx;
    edx = esp + 0x20;
    PUSH32(esp, edx);
    eax = esp + 0x54;
    PUSH32(esp, eax);
    ecx = esp + 0x88;
    PUSH32(esp, ecx);
    ecx = MEM32(esp + 0x40);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : (_fk == 0x540E384Au ? (CMP_B(_fa & _fb, 0)) : CMP_B(_fa & _fb, 0)))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084C6Cu); fast_sub_00082A10(); /* call 0x00082A10 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084C6C: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00084CD2; /* je: equal / zero */

loc_00084C70: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00084CD2; /* jne: not equal / not zero */

loc_00084C81: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176190)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176190] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00084CD2; /* jp: parity */

loc_00084C92: ;
    edx = esp + 0x60;
    PUSH32(esp, edx);
    ecx = edi;
    PUSH32(esp, edi);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : CMP_B(_fa & _fb, 0))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084C9Fu); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084C9F: ;
    eax = MEM32(ebp + 8);
    ecx = esp + 0x20F0;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084CB2u); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084CB2: ;
    edx = esp + 0x60;
    PUSH32(esp, edx);
    ecx = esi;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084CBFu); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084CBF: ;
    eax = MEM32(ebp + 8);
    ecx = esp + 0x20F0;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084CD2u); fast_sub_00082DD0(); /* call 0x00082DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084CD2: ;
    edx = MEM32(esp + 0x18);
    eax = MEM32(esp + 0x1C);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(0x10)) >> 32) & 1);
    edx = edx + 0x10;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xEF4FC9B8u; /* flag setter tag */
    MEM32(esp + 0x18) = edx;
    edx = ZX8(MEM8(edi + 0x1A));
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x86F8765Eu; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x3F7F8199u; /* flag setter tag */
    MEM32(esp + 0x1C) = eax;
    if (CMP_L(_fas, _fbs)) goto loc_00084AD0; /* jl: less (signed <) */

loc_00084CF2: ;
    esi = MEM32(esp + 0x34);
    edx = MEM32(esp + 0x20);
    ecx = MEM32(esp + 0x14);
    eax = MEM32(esp + 0x28);
    _fb = (uint32_t)(0x60) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x60)) >> 32) & 1);
    esi = esi + 0x60;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x344CC3A9u; /* flag setter tag */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(4)) >> 32) & 1);
    edx = edx + 4;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xFA44BCD6u; /* flag setter tag */
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x53735CF0u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x34) = esi;
    MEM32(esp + 0x20) = edx;
    MEM32(esp + 0x14) = ecx;
    MEM32(esp + 0x28) = eax;
    if ((_fa != 0)) goto loc_00084A90; /* jne: not equal / not zero */

loc_00084D20: ;
    eax = MEM32(esp + 0x2C);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x50)) >> 32) & 1);
    edi = edi + 0x50;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x8F6E6588u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x2C) = eax;
    if ((_fa != 0)) goto loc_00084A62; /* jne: not equal / not zero */

loc_00084D32: ;
    eax = MEM32(esp + 0x38);
    ebx = (uint32_t)(int32_t)SMEM16(eax + 0x48);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_00084D91; /* jle: less or equal (signed <=) */

loc_00084D3E: ;
    edi = esp + 0x2104;

loc_00084D45: ;
    ecx = MEM32(edi);
    esi = edi + -20;
    _fb = (uint32_t)(0x70) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x70)) >> 32) & 1);
    ecx = ecx + 0x70;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x8DAC98E1u; /* flag setter tag */
    edx = esi;
    eax = MEM32(edx);
    MEM32(ecx) = eax;
    eax = MEM32(edx + 4);
    MEM32(ecx + 4) = eax;
    eax = MEM32(edx + 8);
    MEM32(ecx + 8) = eax;
    edx = MEM32(edx + 0xC);
    MEM32(ecx + 0xC) = edx;
    ecx = MEM32(edi);
    edx = MEM32(ebp + 8);
    eax = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; { uint32_t _icall_target = MEM32(eax + 8); PUSH32(esp, 0x00084D70u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00084D6Du); } /* indirect call */
    }
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084D70: ;
    eax = MEM32(edi);
    _fb = (uint32_t)(0x70) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x70)) >> 32) & 1);
    eax = eax + 0x70;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xFAA933ECu; /* flag setter tag */
    ecx = MEM32(eax);
    MEM32(esi) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esi + 4) = edx;
    ecx = MEM32(eax + 8);
    MEM32(esi + 8) = ecx;
    edx = MEM32(eax + 0xC);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x50)) >> 32) & 1);
    edi = edi + 0x50;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x8F6E6588u; /* flag setter tag */
    ebx--;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0xF6219CEAu; /* flag setter tag */
    MEM32(esi + 0xC) = edx;
    if ((_fa != 0)) goto loc_00084D45; /* jne: not equal / not zero */

loc_00084D91: ;
    eax = MEM32(esp + 0x38);
    ecx = (uint32_t)(int32_t)SMEM16(eax + 0x48);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xE7AED121u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_00084E7C; /* jle: less or equal (signed <=) */

loc_00084DA1: ;
    eax = esp + 0x2109;
    MEM32(esp + 0x10) = eax;
    MEM32(esp + 0x28) = ecx;
    goto loc_00084DB6;

loc_00084DB2: ;
    eax = MEM32(esp + 0x10);

loc_00084DB6: ;
    ecx = MEM32(eax + 3);
    eax = ZX8(MEM8(eax));
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_00084E62; /* jle: less or equal (signed <=) */

loc_00084DC4: ;
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x50)) >> 32) & 1);
    ecx = ecx + 0x50;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x2F0F47F6u; /* flag setter tag */
    MEM32(esp + 0x14) = ecx;
    MEM32(esp + 0x24) = eax;
    /* nop */

loc_00084DD0: ;
    ecx = MEM32(esp + 0x14);
    esi = MEM32(ecx);
    eax = MEM32(esp + 0x10);
    ebx = ecx + -64;
    _fb = (uint32_t)(0x70) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x70)) >> 32) & 1);
    esi = esi + 0x70;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7B4A5AA3u; /* flag setter tag */
    ecx = 0x10;
    edi = esp + 0xB0;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    edi = ZX8(MEM8(eax + 1));
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    edx = eax + 0x27;
    MEM32(esp + 0x2C) = edx;
    if (CMP_LE(_fas, _fbs)) goto loc_00084E48; /* jle: less or equal (signed <=) */

loc_00084DFD: ;
    esi = edx;
    /* nop */

loc_00084E00: ;
    eax = ZX8(MEM8(esi));
    ecx = MEM32(esp + 0x10);
    eax = eax + eax * 4;
    if (((4) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((4) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((4) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x31CA26A0u; /* flag setter tag */
    eax = esp + eax + 0x20F0;
    _fb = (uint32_t)(0xFFFFFFE7u) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0xFFFFFFE7u)) >> 32) & 1);
    ecx = ecx + 0xFFFFFFE7u;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x29C365B5u; /* flag setter tag */
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = ebx;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084E20u); fast_sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084E20: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F000000);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084E2Du); fast_sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084E2D: ;
    PUSH32(esp, eax);
    edx = esp + 0xB4;
    PUSH32(esp, edx);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084E3Du); fast_sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084E3D: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0x10)) >> 32) & 1);
    ebx = ebx + 0x10;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xE8E364BBu; /* flag setter tag */
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x0AB4FBC0u; /* flag setter tag */
    edi--;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x10E6085Cu; /* flag setter tag */
    if ((_fa != 0)) goto loc_00084E00; /* jne: not equal / not zero */

loc_00084E44: ;
    MEM32(esp + 0x2C) = esi;

loc_00084E48: ;
    ecx = MEM32(esp + 0x14);
    eax = MEM32(esp + 0x24);
    _fb = (uint32_t)(0x60) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x60)) >> 32) & 1);
    ecx = ecx + 0x60;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x6BA8DA7Cu; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x14) = ecx;
    MEM32(esp + 0x24) = eax;
    if ((_fa != 0)) goto loc_00084DD0; /* jne: not equal / not zero */

loc_00084E62: ;
    ecx = MEM32(esp + 0x10);
    eax = MEM32(esp + 0x28);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x50)) >> 32) & 1);
    ecx = ecx + 0x50;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x2F0F47F6u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x10) = ecx;
    MEM32(esp + 0x28) = eax;
    if ((_fa != 0)) goto loc_00084DB2; /* jne: not equal / not zero */

loc_00084E7C: ;
    ecx = MEM32(esp + 0x60BC);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x9A93F82Au ? (CMP_B(_fa, _fb)) : (_fk == 0xE7AED121u ? (CMP_B(_fa, _fb)) : _cf))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00084E88u); esp += 4; /* call 0x001456E4 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00084E88: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}
