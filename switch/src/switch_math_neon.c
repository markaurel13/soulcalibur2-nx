/**
 * Native ARM64 NEON implementations of hot Soulcalibur II math routines:
 *   - sub_00011000: deg-to-rad sincos
 *   - sub_00011250: 4x4 row-major matrix multiply (dst = B * A)
 *   - sub_00011390: 3x3 upper-left matrix multiply preserving A[0..2].w and A[3]
 *   - sub_000123C0: quaternion SLERP + normalize
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include "sc2_prof.h"
#include <math.h>
#include <stdint.h>

#if defined(__aarch64__) || defined(_M_ARM64)
#include <arm_neon.h>
#define SC2_HAS_NEON 1
#else
#define SC2_HAS_NEON 0
#endif

static inline void sc2_quat_normalize(float *dst, const float *src)
{
#if SC2_HAS_NEON
    float32x4_t q = vld1q_f32(src);
    float32x4_t sq = vmulq_f32(q, q);
    float len_sq = vaddvq_f32(sq);
    if (len_sq > 0.0f) {
        float inv_len = 1.0f / sqrtf(len_sq);
        q = vmulq_n_f32(q, inv_len);
    }
    vst1q_f32(dst, q);
#else
    float len_sq = src[0] * src[0] + src[1] * src[1] + src[2] * src[2] + src[3] * src[3];
    if (len_sq > 0.0f) {
        float inv_len = 1.0f / sqrtf(len_sq);
        for (int i = 0; i < 4; i++) dst[i] = src[i] * inv_len;
    } else {
        for (int i = 0; i < 4; i++) dst[i] = src[i];
    }
#endif
}

void sub_00011000(void)
{
    SC2_PROF_SCOPE(0x00011000u);
    float angle = MEMF(esp + 4);
    uint32_t out_sin_va = MEM32(esp + 8);
    uint32_t out_cos_va = MEM32(esp + 0xC);
    float rad = angle * MEMF(0x176188);

    float s = sinf(rad);
    float c = cosf(rad);
    MEMF(out_sin_va) = s;
    MEMF(out_cos_va) = c;

    eax = out_sin_va;
    ecx = out_cos_va;
    esp += 4;
}

void sub_00011250(void)
{
    SC2_PROF_SCOPE(0x00011250u);
    uint32_t dst_va = ecx;
    uint32_t a_va = MEM32(esp + 4);
    uint32_t b_va = MEM32(esp + 8);

    const float *a = (const float *)&MEM8(a_va);
    const float *b = (const float *)&MEM8(b_va);
    float *dst = (float *)&MEM8(dst_va);

#if SC2_HAS_NEON
    float32x4_t a0 = vld1q_f32(a + 0);
    float32x4_t a1 = vld1q_f32(a + 4);
    float32x4_t a2 = vld1q_f32(a + 8);
    float32x4_t a3 = vld1q_f32(a + 12);

    float32x4_t b0 = vld1q_f32(b + 0);
    float32x4_t b1 = vld1q_f32(b + 4);
    float32x4_t b2 = vld1q_f32(b + 8);
    float32x4_t b3 = vld1q_f32(b + 12);

    float32x4_t r0 = vmulq_laneq_f32(a0, b0, 0);
    r0 = vfmaq_laneq_f32(r0, a1, b0, 1);
    r0 = vfmaq_laneq_f32(r0, a2, b0, 2);
    r0 = vfmaq_laneq_f32(r0, a3, b0, 3);

    float32x4_t r1 = vmulq_laneq_f32(a0, b1, 0);
    r1 = vfmaq_laneq_f32(r1, a1, b1, 1);
    r1 = vfmaq_laneq_f32(r1, a2, b1, 2);
    r1 = vfmaq_laneq_f32(r1, a3, b1, 3);

    float32x4_t r2 = vmulq_laneq_f32(a0, b2, 0);
    r2 = vfmaq_laneq_f32(r2, a1, b2, 1);
    r2 = vfmaq_laneq_f32(r2, a2, b2, 2);
    r2 = vfmaq_laneq_f32(r2, a3, b2, 3);

    float32x4_t r3 = vmulq_laneq_f32(a0, b3, 0);
    r3 = vfmaq_laneq_f32(r3, a1, b3, 1);
    r3 = vfmaq_laneq_f32(r3, a2, b3, 2);
    r3 = vfmaq_laneq_f32(r3, a3, b3, 3);

    vst1q_f32(dst + 0, r0);
    vst1q_f32(dst + 4, r1);
    vst1q_f32(dst + 8, r2);
    vst1q_f32(dst + 12, r3);
#else
    float out[16];
    for (int i = 0; i < 4; i++) {
        const float *bi = b + i * 4;
        for (int j = 0; j < 4; j++) {
            out[i * 4 + j] = bi[0] * a[j] + bi[1] * a[4 + j] + bi[2] * a[8 + j] + bi[3] * a[12 + j];
        }
    }
    for (int i = 0; i < 16; i++) dst[i] = out[i];
#endif

    esp += 12;
}

void sub_00011390(void)
{
    SC2_PROF_SCOPE(0x00011390u);
    uint32_t dst_va = ecx;
    uint32_t a_va = MEM32(esp + 4);
    uint32_t b_va = MEM32(esp + 8);

    const float *a = (const float *)&MEM8(a_va);
    const float *b = (const float *)&MEM8(b_va);
    float *dst = (float *)&MEM8(dst_va);

#if SC2_HAS_NEON
    float32x4_t a0 = vld1q_f32(a + 0);
    float32x4_t a1 = vld1q_f32(a + 4);
    float32x4_t a2 = vld1q_f32(a + 8);
    float32x4_t a3 = vld1q_f32(a + 12);

    float32x4_t b0 = vld1q_f32(b + 0);
    float32x4_t b1 = vld1q_f32(b + 4);
    float32x4_t b2 = vld1q_f32(b + 8);

    float32x4_t r0 = vmulq_laneq_f32(a0, b0, 0);
    r0 = vfmaq_laneq_f32(r0, a1, b0, 1);
    r0 = vfmaq_laneq_f32(r0, a2, b0, 2);

    float32x4_t r1 = vmulq_laneq_f32(a0, b1, 0);
    r1 = vfmaq_laneq_f32(r1, a1, b1, 1);
    r1 = vfmaq_laneq_f32(r1, a2, b1, 2);

    float32x4_t r2 = vmulq_laneq_f32(a0, b2, 0);
    r2 = vfmaq_laneq_f32(r2, a1, b2, 1);
    r2 = vfmaq_laneq_f32(r2, a2, b2, 2);

    r0 = vsetq_lane_f32(vgetq_lane_f32(a0, 3), r0, 3);
    r1 = vsetq_lane_f32(vgetq_lane_f32(a1, 3), r1, 3);
    r2 = vsetq_lane_f32(vgetq_lane_f32(a2, 3), r2, 3);

    vst1q_f32(dst + 0, r0);
    vst1q_f32(dst + 4, r1);
    vst1q_f32(dst + 8, r2);
    vst1q_f32(dst + 12, a3);
#else
    float out[16];
    for (int i = 0; i < 3; i++) {
        const float *bi = b + i * 4;
        for (int j = 0; j < 3; j++) {
            out[i * 4 + j] = bi[0] * a[j] + bi[1] * a[4 + j] + bi[2] * a[8 + j];
        }
        out[i * 4 + 3] = a[i * 4 + 3];
    }
    for (int j = 0; j < 4; j++) out[12 + j] = a[12 + j];
    for (int i = 0; i < 16; i++) dst[i] = out[i];
#endif

    esp += 12;
}

void sub_000123C0(void)
{
    SC2_PROF_SCOPE(0x000123C0u);
    uint32_t dst_va = ecx;
    uint32_t q0_va = MEM32(esp + 4);
    uint32_t q1_va = MEM32(esp + 8);
    float t = MEMF(esp + 0xC);

    float eps = MEMF(0x1761A4);
    float one = MEMF(0x176190);

    const float *q0 = (const float *)&MEM8(q0_va);
    const float *q1 = (const float *)&MEM8(q1_va);
    float *dst = (float *)&MEM8(dst_va);

    if (t < eps) {
        sc2_quat_normalize(dst, q0);
        eax = dst_va;
        esp += 16;
        return;
    }

    if (!(t < one) && !isnan(t)) {
        sc2_quat_normalize(dst, q1);
        eax = dst_va;
        esp += 16;
        return;
    }

    float q1_tmp[4] = { q1[0], q1[1], q1[2], q1[3] };
    float dot = q0[0] * q1_tmp[0] + q0[1] * q1_tmp[1] + q0[2] * q1_tmp[2] + q0[3] * q1_tmp[3];
    if (dot < MEMF(0x1761A0)) {
        q1_tmp[0] = -q1_tmp[0];
        q1_tmp[1] = -q1_tmp[1];
        q1_tmp[2] = -q1_tmp[2];
        q1_tmp[3] = -q1_tmp[3];
        dot = -dot;
    }

    float sin_theta = sqrtf(fabsf(one - dot * dot));
    float s0, s1;
    if (sin_theta <= eps || isnan(sin_theta)) {
        s0 = one - t;
        s1 = t;
    } else {
        float theta = atan2f(sin_theta, dot) * MEMF(0x176194);
        float inv_sin = one / sin_theta;
        float deg_to_rad = MEMF(0x176188);
        s0 = sinf((one - t) * theta * deg_to_rad) * inv_sin;
        s1 = sinf(t * theta * deg_to_rad) * inv_sin;
    }

    float blended[4];
    for (int i = 0; i < 4; i++) {
        blended[i] = s0 * q0[i] + s1 * q1_tmp[i];
    }
    sc2_quat_normalize(dst, blended);

    eax = dst_va;
    esp += 16;
}

void sub_00012600(void)
{
    const float *a = (const float *)&MEM8(ecx);
    const float *b = (const float *)&MEM8(MEM32(esp + 4));
    float dot = (a[0] * b[0] + a[1] * b[1]) + a[2] * b[2];
    g_fp_top = (g_fp_top + 7u) & 7u;
    g_fp_stack[g_fp_top] = (double)dot;
    g_eflags &= ~1u;
    esp += 8;
}

void sub_00012640(void)
{
    uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
    float aw = a[3];
    float x = a[1] * b[2] - a[2] * b[1];
    float y = a[2] * b[0] - a[0] * b[2];
    float z = a[0] * b[1] - a[1] * b[0];
    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
    dst[3] = aw;
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

void sub_000126A0(void)
{
    uint32_t dst_va = ecx;
    float scale = MEMF(esp + 4);
    const float *src = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#ifdef __aarch64__
    vst1q_f32(dst, vmulq_n_f32(vld1q_f32(src), scale));
#else
    dst[0] = src[0] * scale;
    dst[1] = src[1] * scale;
    dst[2] = src[2] * scale;
    dst[3] = src[3] * scale;
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

void sub_00012720(void)
{
    uint32_t dst_va = ecx;
    const float *mat = (const float *)&MEM8(MEM32(esp + 4));
    const float *vec = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#ifdef __aarch64__
    float32x4_t v = vld1q_f32(vec);
    float32x4_t m0 = vld1q_f32(mat + 0);
    float32x4_t m1 = vld1q_f32(mat + 4);
    float32x4_t m2 = vld1q_f32(mat + 8);
    float32x4_t m3 = vld1q_f32(mat + 12);
    float32x4_t res = vmulq_laneq_f32(m0, v, 0);
    res = vfmaq_laneq_f32(res, m1, v, 1);
    res = vfmaq_laneq_f32(res, m2, v, 2);
    res = vfmaq_laneq_f32(res, m3, v, 3);
    vst1q_f32(dst, res);
#else
    float out[4];
    for (int i = 0; i < 4; i++) {
        out[i] = mat[i] * vec[0] + mat[4 + i] * vec[1] + mat[8 + i] * vec[2] + mat[12 + i] * vec[3];
    }
    dst[0] = out[0]; dst[1] = out[1]; dst[2] = out[2]; dst[3] = out[3];
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

void sub_00012550(void)
{
    uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#ifdef __aarch64__
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

void sub_00012790(void)
{
    uint32_t dst_va = ecx;
    const float *mat = (const float *)&MEM8(MEM32(esp + 4));
    const float *vec = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#ifdef __aarch64__
    float32x4_t v = vld1q_f32(vec);
    float32x4_t m0 = vld1q_f32(mat + 0);
    float32x4_t m1 = vld1q_f32(mat + 4);
    float32x4_t m2 = vld1q_f32(mat + 8);
    float32x4_t m3 = vld1q_f32(mat + 12);
    float32x4_t res = vfmaq_laneq_f32(m3, m0, v, 0);
    res = vfmaq_laneq_f32(res, m1, v, 1);
    res = vfmaq_laneq_f32(res, m2, v, 2);
    vst1q_f32(dst, res);
#else
    float out[4];
    for (int i = 0; i < 4; i++) {
        out[i] = mat[i] * vec[0] + mat[4 + i] * vec[1] + mat[8 + i] * vec[2] + mat[12 + i];
    }
    dst[0] = out[0]; dst[1] = out[1]; dst[2] = out[2]; dst[3] = out[3];
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

void sub_00011710(void)
{
    const float *q = (const float *)&MEM8(MEM32(esp + 4));
    float *dst = (float *)&MEM8(ecx);
    const float x = q[0], y = q[1], z = q[2], w = q[3];
    const float x2 = x + x, y2 = y + y, z2 = z + z;
    const float xx = x * x2, xy = x * y2, xz = x * z2;
    const float yy = y * y2, yz = y * z2, zz = z * z2;
    const float wx = w * x2, wy = w * y2, wz = w * z2;

    dst[0]  = 1.0f - (yy + zz);
    dst[1]  = xy + wz;
    dst[2]  = xz - wy;
    dst[3]  = 0.0f;

    dst[4]  = xy - wz;
    dst[5]  = 1.0f - (xx + zz);
    dst[6]  = yz + wx;
    dst[7]  = 0.0f;

    dst[8]  = xz + wy;
    dst[9]  = yz - wx;
    dst[10] = 1.0f - (xx + yy);
    dst[11] = 0.0f;

    dst[15] = 1.0f;
    g_eflags &= ~1u;
    esp += 8;
}
