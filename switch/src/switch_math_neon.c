/**
 * Native ARM64 NEON implementations of hot Soulcalibur II math routines:
 *   - sub_00011000: deg-to-rad sincos
 *   - sub_00011250: 4x4 row-major matrix multiply (dst = B * A)
 *   - sub_00011390: 3x3 upper-left matrix multiply preserving A[0..2].w and A[3]
 *   - sub_000123C0: quaternion SLERP + normalize
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include "sc2_fast_mem.h"
#include "sc2_prof.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

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

void sub_00012950(void)
{
    uint32_t dst_va = ecx;
    uint32_t src_va = MEM32(esp + 4);
    const float *src = (const float *)&MEM8(src_va);
    float *dst = (float *)&MEM8(dst_va);
    const float x = src[0], y = src[1], z = src[2], w = src[3];
    const float len_sq = x * x + y * y + z * z;
    if (len_sq > 0.0f) {
        const float inv = 1.0f / sqrtf(len_sq);
        const float nx = x * inv, ny = y * inv, nz = z * inv;
        if (isfinite(nx)) {
            dst[0] = nx;
            dst[1] = ny;
            dst[2] = nz;
            dst[3] = w;
            eax = dst_va;
            g_eflags &= ~1u;
            esp += 8;
            return;
        }
    }
    dst[0] = 0.0f;
    dst[1] = 0.0f;
    dst[2] = 0.0f;
    dst[3] = w;
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 8;
}

void sub_00145A64(void)
{
    const double v = g_fp_stack[g_fp_top];
    g_fp_top = (g_fp_top + 1u) & 7u;
    const int64_t r = (int64_t)v;
    eax = (uint32_t)(uint64_t)r;
    edx = (uint32_t)((uint64_t)r >> 32);
    esp += 4;
}

void sub_00079AE0(void)
{
    const uint32_t bs = ecx;
    const int32_t nbits = (int32_t)MEM32(esp + 4);
    if (nbits <= 0) {
        eax = 0;
        g_eflags &= ~1u;
        esp += 8;
        return;
    }
    uint32_t bit_pos = MEM32(bs);
    uint32_t byte_ptr = MEM32(bs + 4);
    if (nbits <= 24) {
        uint32_t w;
        __builtin_memcpy(&w, &MEM8(byte_ptr), 4);
        w = __builtin_bswap32(w);
        const uint32_t val = (w << bit_pos) >> (32u - (uint32_t)nbits);
        const uint32_t total = bit_pos + (uint32_t)nbits;
        MEM32(bs) = total & 7u;
        MEM32(bs + 4) = byte_ptr + (total >> 3);
        eax = val;
        g_eflags &= ~1u;
        esp += 8;
        return;
    }
    uint32_t val = 0;
    int32_t rem = nbits;
    while (rem > 0) {
        const uint32_t avail = 8u - bit_pos;
        const uint8_t cur = MEM8(byte_ptr);
        if ((int32_t)avail >= rem) {
            val = (val << rem) | ((cur >> (avail - (uint32_t)rem)) & ((1u << rem) - 1u));
            bit_pos += (uint32_t)rem;
            if (bit_pos >= 8u) { byte_ptr++; bit_pos = 0; }
            rem = 0;
        } else {
            val = (val << avail) | (cur & ((1u << avail) - 1u));
            rem -= (int32_t)avail;
            byte_ptr++;
            bit_pos = 0;
        }
    }
    MEM32(bs) = bit_pos;
    MEM32(bs + 4) = byte_ptr;
    eax = val;
    g_eflags &= ~1u;
    esp += 8;
}

void sub_00079A30(void)
{
    const uint32_t bs = ecx;
    const int32_t nbits = (int32_t)MEM32(esp + 4);
    if (nbits <= 0) {
        eax = 0;
        g_eflags &= ~1u;
        esp += 8;
        return;
    }
    uint32_t bit_pos = MEM32(bs);
    uint32_t byte_ptr = MEM32(bs + 4);
    if (nbits <= 24) {
        uint32_t w;
        __builtin_memcpy(&w, &MEM8(byte_ptr), 4);
        w = __builtin_bswap32(w);
        const int32_t sval = (int32_t)(w << bit_pos) >> (32 - nbits);
        const uint32_t total = bit_pos + (uint32_t)nbits;
        MEM32(bs) = total & 7u;
        MEM32(bs + 4) = byte_ptr + (total >> 3);
        eax = (uint32_t)sval;
        g_eflags &= ~1u;
        esp += 8;
        return;
    }
    uint32_t val = 0;
    int32_t rem = nbits;
    while (rem > 0) {
        const uint32_t avail = 8u - bit_pos;
        const uint8_t cur = MEM8(byte_ptr);
        if ((int32_t)avail >= rem) {
            val = (val << rem) | ((cur >> (avail - (uint32_t)rem)) & ((1u << rem) - 1u));
            bit_pos += (uint32_t)rem;
            if (bit_pos >= 8u) { byte_ptr++; bit_pos = 0; }
            rem = 0;
        } else {
            val = (val << avail) | (cur & ((1u << avail) - 1u));
            rem -= (int32_t)avail;
            byte_ptr++;
            bit_pos = 0;
        }
    }
    MEM32(bs) = bit_pos;
    MEM32(bs + 4) = byte_ptr;
    eax = (nbits < 32) ? (uint32_t)((int32_t)(val << (32 - nbits)) >> (32 - nbits)) : val;
    g_eflags &= ~1u;
    esp += 8;
}

void sub_00079CD0(void)
{
    const uint32_t tbl = ecx;
    const uint32_t bs = MEM32(esp + 4);
    const uint32_t max_bits = MEM8(tbl + 4u);
    const uint32_t bit_pos = MEM32(bs);
    const uint32_t byte_ptr = MEM32(bs + 4);
    uint32_t code = 0;
    if (max_bits > 0u) {
        uint32_t w;
        __builtin_memcpy(&w, &MEM8(byte_ptr), 4);
        w = __builtin_bswap32(w);
        code = (w << bit_pos) >> (32u - max_bits);
    }
    const uint32_t count = MEM8(tbl + 5u);
    uint32_t p = tbl + count * 4u + 2u;
    const uint32_t end = tbl + 6u;
    while (p > end && (uint32_t)MEM16(p) > code) {
        p -= 4u;
    }
    const uint32_t sym_len = MEM8(p + 2u);
    const uint32_t total = bit_pos + sym_len;
    MEM32(bs) = total & 7u;
    MEM32(bs + 4) = byte_ptr + (total >> 3);
    eax = MEM8(p + 3u);
    g_eflags &= ~1u;
    esp += 8;
}

void sub_000EB2A0(void)
{
    const uint32_t p_va = MEM32(esp + 4);
    const uint32_t tri_va = MEM32(esp + 8);
    const float px = MEMF(p_va);
    const float pz = MEMF(p_va + 8u);
    const float x0 = MEMF(tri_va + 0x00u), z0 = MEMF(tri_va + 0x08u);
    const float x1 = MEMF(tri_va + 0x10u), z1 = MEMF(tri_va + 0x18u);
    const float x2 = MEMF(tri_va + 0x20u), z2 = MEMF(tri_va + 0x28u);
    const int b0 = ((x1 - x0) * (pz - z0)) > ((px - x0) * (z1 - z0));
    const int b1 = ((x2 - x1) * (pz - z1)) > ((px - x1) * (z2 - z1));
    if (b0 != b1) {
        SET_LO8(eax, 0);
        g_eflags &= ~1u;
        esp += 12;
        return;
    }
    const int b2 = ((x0 - x2) * (pz - z2)) > ((px - x2) * (z0 - z2));
    SET_LO8(eax, (b1 == b2) ? 1 : 0);
    g_eflags &= ~1u;
    esp += 12;
}

void __wrap_sub_00012860(void)
{
    const uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#ifdef __aarch64__
    vst1q_f32(dst, vmulq_f32(vld1q_f32(a), vld1q_f32(b)));
#else
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
    dst[2] = a[2] * b[2];
    dst[3] = a[3] * b[3];
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

void __wrap_sub_00012570(void)
{
    const uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
    const float w = dst[3];
#ifdef __aarch64__
    vst1q_f32(dst, vaddq_f32(vld1q_f32(a), vld1q_f32(b)));
#else
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
#endif
    dst[3] = w;
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 12;
}

void __wrap_sub_00012640(void)
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

void __wrap_sub_000126A0(void)
{
    const uint32_t dst_va = ecx;
    const float s = MEMF(esp + 4);
    const float *a = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#ifdef __aarch64__
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

void __wrap_sub_00012790(void)
{
    const uint32_t dst_va = ecx;
    const float *m = (const float *)&MEM8(MEM32(esp + 4));
    const float *v = (const float *)&MEM8(MEM32(esp + 8));
    float *dst = (float *)&MEM8(dst_va);
#ifdef __aarch64__
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

void __wrap_sub_00012950(void)
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
            dst[0] = 0.0f;
            dst[1] = 0.0f;
            dst[2] = 0.0f;
            dst[3] = w;
        }
    } else {
        dst[0] = 0.0f;
        dst[1] = 0.0f;
        dst[2] = 0.0f;
        dst[3] = w;
    }
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 8;
}

void __wrap_sub_000123C0(void)
{
    const uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    const float t = MEMF(esp + 0xC);
    float *dst = (float *)&MEM8(dst_va);

    if (t <= 1e-4f) {
        sc2_quat_normalize(dst, a);
        eax = dst_va;
        g_eflags &= ~1u;
        esp += 16;
        return;
    }
    if (t == 1.0f) {
        sc2_quat_normalize(dst, b);
        eax = dst_va;
        g_eflags &= ~1u;
        esp += 16;
        return;
    }

#ifdef __aarch64__
    float32x4_t qa = vld1q_f32(a);
    float32x4_t qb = vld1q_f32(b);
    float dot = vaddvq_f32(vmulq_f32(qa, qb));
    if (dot < 0.0f) {
        qb = vnegq_f32(qb);
        dot = -dot;
    }
    const float sin_theta = sqrtf(fabsf(1.0f - dot * dot));
    float w0, w1;
    if (sin_theta > 1e-4f) {
        const float angle = atan2f(sin_theta, dot);
        const float inv_sin = 1.0f / sin_theta;
        w0 = sinf((1.0f - t) * angle) * inv_sin;
        w1 = sinf(t * angle) * inv_sin;
    } else {
        w0 = 1.0f - t;
        w1 = t;
    }
    float32x4_t q = vfmaq_n_f32(vmulq_n_f32(qa, w0), qb, w1);
    float len_sq = vaddvq_f32(vmulq_f32(q, q));
    if (len_sq > 0.0f) {
        q = vmulq_n_f32(q, 1.0f / sqrtf(len_sq));
    }
    vst1q_f32(dst, q);
#else
    float dot = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
    float sign = 1.0f;
    if (dot < 0.0f) {
        sign = -1.0f;
        dot = -dot;
    }
    const float sin_theta = sqrtf(fabsf(1.0f - dot * dot));
    float w0, w1;
    if (sin_theta > 1e-4f) {
        const float angle = atan2f(sin_theta, dot);
        const float inv_sin = 1.0f / sin_theta;
        w0 = sinf((1.0f - t) * angle) * inv_sin;
        w1 = sinf(t * angle) * inv_sin * sign;
    } else {
        w0 = 1.0f - t;
        w1 = t * sign;
    }
    float tmp[4] = {
        a[0] * w0 + b[0] * w1,
        a[1] * w0 + b[1] * w1,
        a[2] * w0 + b[2] * w1,
        a[3] * w0 + b[3] * w1
    };
    sc2_quat_normalize(dst, tmp);
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 16;
}

void __wrap_sub_000125A0(void)
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

void __wrap_sub_00012B00(void)
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

void __wrap_sub_00012BC0(void)
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

void __wrap_sub_000129F0(void)
{
    const uint32_t dst_va = ecx;
    const float *a = (const float *)&MEM8(MEM32(esp + 4));
    const float *b = (const float *)&MEM8(MEM32(esp + 8));
    const float t = MEMF(esp + 0xC);
    const float omt = MEMF(0x176190) - t;
    float *dst = (float *)&MEM8(dst_va);
#if SC2_HAS_NEON
    float32x4_t va = vld1q_f32(a);
    float32x4_t vb = vld1q_f32(b);
    vst1q_f32(dst, vfmaq_n_f32(vmulq_n_f32(va, omt), vb, t));
#else
    dst[0] = a[0] * omt + b[0] * t;
    dst[1] = a[1] * omt + b[1] * t;
    dst[2] = a[2] * omt + b[2] * t;
    dst[3] = a[3] * omt + b[3] * t;
#endif
    eax = dst_va;
    g_eflags &= ~1u;
    esp += 16;
}

void __wrap_sub_00011B60(void)
{
    const uint32_t src_va = MEM32(esp + 4);
    const float sx = MEMF(esp + 8);
    const float sy = MEMF(esp + 0xC);
    const float sz = MEMF(esp + 0x10);
    const float *src = (const float *)&MEM8(src_va);
    float *dst = (float *)&MEM8(ecx);
#if SC2_HAS_NEON
    float32x4_t r0 = vld1q_f32(src + 0);
    float32x4_t r1 = vld1q_f32(src + 4);
    float32x4_t r2 = vld1q_f32(src + 8);
    float32x4_t r3 = vld1q_f32(src + 12);
    r0 = vsetq_lane_f32(vgetq_lane_f32(r0, 3), vmulq_n_f32(r0, sx), 3);
    r1 = vsetq_lane_f32(vgetq_lane_f32(r1, 3), vmulq_n_f32(r1, sy), 3);
    r2 = vsetq_lane_f32(vgetq_lane_f32(r2, 3), vmulq_n_f32(r2, sz), 3);
    vst1q_f32(dst + 0, r0);
    vst1q_f32(dst + 4, r1);
    vst1q_f32(dst + 8, r2);
    vst1q_f32(dst + 12, r3);
#else
    dst[0] = src[0] * sx; dst[1] = src[1] * sx; dst[2] = src[2] * sx; dst[3] = src[3];
    dst[4] = src[4] * sy; dst[5] = src[5] * sy; dst[6] = src[6] * sy; dst[7] = src[7];
    dst[8] = src[8] * sz; dst[9] = src[9] * sz; dst[10] = src[10] * sz; dst[11] = src[11];
    dst[12] = src[12]; dst[13] = src[13]; dst[14] = src[14]; dst[15] = src[15];
#endif
    eax = src_va;
    g_eflags &= ~1u;
    esp += 20;
}
