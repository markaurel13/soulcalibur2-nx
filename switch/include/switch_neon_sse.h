#ifndef SWITCH_NEON_SSE_H
#define SWITCH_NEON_SSE_H

#include <stdint.h>
#include <string.h>
#include <math.h>

#if defined(__aarch64__)
#include <arm_neon.h>

// Vector arithmetic
static inline RecompXmm XMM_ADD(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_f32(r.f, vaddq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm XMM_SUB(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_f32(r.f, vsubq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm XMM_MUL(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_f32(r.f, vmulq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm XMM_DIV(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_f32(r.f, vdivq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

// Min/Max with exact SSE semantics
static inline RecompXmm XMM_MIN(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    float32x4_t va = vld1q_f32(a.f);
    float32x4_t vb = vld1q_f32(b.f);
    uint32x4_t lt = vcltq_f32(va, vb);
    vst1q_f32(r.f, vbslq_f32(lt, va, vb));
    return r;
}

static inline RecompXmm XMM_MAX(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    float32x4_t va = vld1q_f32(a.f);
    float32x4_t vb = vld1q_f32(b.f);
    uint32x4_t gt = vcgtq_f32(va, vb);
    vst1q_f32(r.f, vbslq_f32(gt, va, vb));
    return r;
}

// Bitwise logic
static inline RecompXmm XMM_AND(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vandq_u32(vld1q_u32(a.u), vld1q_u32(b.u)));
    return r;
}

static inline RecompXmm XMM_OR(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vorrq_u32(vld1q_u32(a.u), vld1q_u32(b.u)));
    return r;
}

static inline RecompXmm XMM_XOR(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, veorq_u32(vld1q_u32(a.u), vld1q_u32(b.u)));
    return r;
}

static inline RecompXmm XMM_ANDN(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vbicq_u32(vld1q_u32(b.u), vld1q_u32(a.u)));
    return r;
}

// Comparisons
static inline RecompXmm XMM_CMP_EQ(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vceqq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm XMM_CMP_LT(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vcltq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm XMM_CMP_LE(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vcleq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm XMM_CMP_NEQ(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vmvnq_u32(vceqq_f32(vld1q_f32(a.f), vld1q_f32(b.f))));
    return r;
}

static inline int recomp_cmp_pred(float a, float b, int p) {
    switch (p & 7) {
    case 0:  return a == b;
    case 1:  return a < b;
    case 2:  return a <= b;
    case 3:  return a != a || b != b;
    case 4:  return !(a == b);
    case 5:  return !(a < b);
    case 6:  return !(a <= b);
    default: return !(a != a || b != b);
    }
}

static inline RecompXmm XMM_CMP_PRED(RecompXmm a, RecompXmm b, int p) {
    RecompXmm r;
    for (int i = 0; i < 4; ++i)
        r.u[i] = recomp_cmp_pred(a.f[i], b.f[i], p) ? 0xFFFFFFFFu : 0u;
    return r;
}

// Hardware Vector Roots and Reciprocals
static inline RecompXmm XMM_SQRT(RecompXmm a, RecompXmm b) {
    (void)a;
    RecompXmm r;
    vst1q_f32(r.f, vsqrtq_f32(vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm XMM_RSQRT(RecompXmm a, RecompXmm b) {
    (void)a;
    RecompXmm r;
    vst1q_f32(r.f, vrsqrteq_f32(vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm XMM_RCP(RecompXmm a, RecompXmm b) {
    (void)a;
    RecompXmm r;
    vst1q_f32(r.f, vrecpeq_f32(vld1q_f32(b.f)));
    return r;
}

// Unpack & Moves
static inline RecompXmm XMM_UNPACK_LOW(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vzip1q_u32(vld1q_u32(a.u), vld1q_u32(b.u)));
    return r;
}

static inline RecompXmm XMM_UNPACK_HIGH(RecompXmm a, RecompXmm b) {
    RecompXmm r;
    vst1q_u32(r.u, vzip2q_u32(vld1q_u32(a.u), vld1q_u32(b.u)));
    return r;
}

static inline RecompXmm XMM_MOVE_LOW_TO_HIGH(RecompXmm a, RecompXmm b) {
    RecompXmm r = a;
    r.u[2] = b.u[0];
    r.u[3] = b.u[1];
    return r;
}

static inline RecompXmm XMM_MOVE_HIGH_TO_LOW(RecompXmm a, RecompXmm b) {
    RecompXmm r = a;
    r.u[0] = b.u[2];
    r.u[1] = b.u[3];
    return r;
}

static inline RecompXmm XMM_SHUFFLE(RecompXmm a, RecompXmm b, uint32_t imm) {
    RecompXmm r;
    r.u[0] = a.u[(imm >> 0) & 3u];
    r.u[1] = a.u[(imm >> 2) & 3u];
    r.u[2] = b.u[(imm >> 4) & 3u];
    r.u[3] = b.u[(imm >> 6) & 3u];
    return r;
}

static inline uint32_t XMM_MOVEMASK(RecompXmm a) {
    return ((a.u[0] >> 31) & 1u) | (((a.u[1] >> 31) & 1u) << 1)
         | (((a.u[2] >> 31) & 1u) << 2) | (((a.u[3] >> 31) & 1u) << 3);
}

#endif // __aarch64__
#endif // SWITCH_NEON_SSE_H
