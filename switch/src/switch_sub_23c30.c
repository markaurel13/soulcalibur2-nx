/**
 * Native ARM64 NEON implementation of sub_00023C30 (bone skinning / hierarchy transform)
 * for Nintendo Switch.
 *
 * Replaces the generated x86-emulated loop (guest stack spills, per-vertex ABI calls,
 * and recomp_memory_u32 lookups) with direct guest-pointer NEON FMA math while
 * preserving the 1024-iteration safety guard and child-count clamp.
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

extern FILE *Switch_GetMainLogFile(void);

#if SC2_HAS_NEON
typedef struct {
    float32x4_t o0;
    float32x4_t o1;
} Sc2BonePair;

static inline Sc2BonePair bone_transform(const float *m, const float *in)
{
    float32x4_t m0 = vld1q_f32(m + 0);
    float32x4_t m1 = vld1q_f32(m + 4);
    float32x4_t m2 = vld1q_f32(m + 8);
    float32x4_t m3 = vld1q_f32(m + 12);
    float32x4_t v0 = vld1q_f32(in + 0);
    float32x4_t v1 = vld1q_f32(in + 4);

    float32x4_t o0 = vmulq_laneq_f32(m0, v0, 0);
    o0 = vfmaq_laneq_f32(o0, m1, v0, 1);
    o0 = vfmaq_laneq_f32(o0, m2, v0, 2);
    o0 = vfmaq_laneq_f32(o0, m3, v0, 3);

    float32x4_t o1 = vmulq_laneq_f32(m0, v1, 0);
    o1 = vfmaq_laneq_f32(o1, m1, v1, 1);
    o1 = vfmaq_laneq_f32(o1, m2, v1, 2);

    Sc2BonePair res = { o0, o1 };
    return res;
}

static inline Sc2BonePair bone_transform_acc(const float *m, const float *in, Sc2BonePair acc)
{
    float32x4_t m0 = vld1q_f32(m + 0);
    float32x4_t m1 = vld1q_f32(m + 4);
    float32x4_t m2 = vld1q_f32(m + 8);
    float32x4_t m3 = vld1q_f32(m + 12);
    float32x4_t v0 = vld1q_f32(in + 0);
    float32x4_t v1 = vld1q_f32(in + 4);

    float32x4_t o0 = vfmaq_laneq_f32(acc.o0, m0, v0, 0);
    o0 = vfmaq_laneq_f32(o0, m1, v0, 1);
    o0 = vfmaq_laneq_f32(o0, m2, v0, 2);
    o0 = vfmaq_laneq_f32(o0, m3, v0, 3);

    float32x4_t o1 = vfmaq_laneq_f32(acc.o1, m0, v1, 0);
    o1 = vfmaq_laneq_f32(o1, m1, v1, 1);
    o1 = vfmaq_laneq_f32(o1, m2, v1, 2);

    Sc2BonePair res = { o0, o1 };
    return res;
}

static inline void bone_store(float *out, Sc2BonePair val)
{
    vst1q_f32(out + 0, val.o0);
    vst1q_f32(out + 4, val.o1);
}
#else
typedef struct {
    float o0[4];
    float o1[4];
} Sc2BonePair;

static inline Sc2BonePair bone_transform(const float *m, const float *in)
{
    Sc2BonePair r;
    for (int i = 0; i < 4; i++) {
        r.o0[i] = in[0] * m[i] + in[1] * m[4 + i] + in[2] * m[8 + i] + in[3] * m[12 + i];
        r.o1[i] = in[4] * m[i] + in[5] * m[4 + i] + in[6] * m[8 + i];
    }
    return r;
}

static inline Sc2BonePair bone_transform_acc(const float *m, const float *in, Sc2BonePair acc)
{
    Sc2BonePair r;
    for (int i = 0; i < 4; i++) {
        r.o0[i] = acc.o0[i] + in[0] * m[i] + in[1] * m[4 + i] + in[2] * m[8 + i] + in[3] * m[12 + i];
        r.o1[i] = acc.o1[i] + in[4] * m[i] + in[5] * m[4 + i] + in[6] * m[8 + i];
    }
    return r;
}

static inline void bone_store(float *out, Sc2BonePair val)
{
    for (int i = 0; i < 4; i++) {
        out[i] = val.o0[i];
        out[4 + i] = val.o1[i];
    }
}
#endif

void sub_00023C30(void)
{
    SC2_PROF_SCOPE(0x00023C30u);

    uint32_t obj_va = MEM32(esp + 4);
    uint32_t frame_idx = MEM32(0x38C4B4);
    uint32_t bone_base_va = MEM32(0x38C668);
    float ref_val = MEMF(0x1761A0);

    uint32_t count1 = MEM32(obj_va + 0x00);
    uint32_t count2 = MEM32(obj_va + 0x04);
    uint32_t count3 = MEM32(obj_va + 0x08);
    uint32_t src_va = MEM32(obj_va + 0x10);
    uint32_t dst_va = MEM32(obj_va + 0x14 + frame_idx * 4u);

    const uint8_t *bones = (const uint8_t *)&MEM8(bone_base_va);
    const uint8_t *src_base = (const uint8_t *)&MEM8(src_va);
    uint8_t *dst_base = (uint8_t *)&MEM8(dst_va);

    const uint8_t *src = src_base;
    uint8_t *dst = dst_base;

    /* Pass 1: 1-bone rigid skinning (count1 vertices) */
    for (uint32_t i = 0; i < count1; i++) {
        uint32_t b0 = *(const uint32_t *)(src + 0x1C);
        if (b0 >= 4096u) break;
        const float *m0 = (const float *)(bones + (b0 << 6));
        Sc2BonePair res = bone_transform(m0, (const float *)src);
        bone_store((float *)dst, res);
        src += 0x20;
        dst += 0x20;
    }

    /* Pass 2: 2-bone weighted skinning (count2 vertices) */
    for (uint32_t i = 0; i < count2; i++) {
        uint32_t b0 = *(const uint32_t *)(src + 0x1C);
        uint32_t b1 = *(const uint32_t *)(src + 0x3C);
        if (b0 >= 4096u || b1 >= 4096u) break;
        const float *m0 = (const float *)(bones + (b0 << 6));
        const float *m1 = (const float *)(bones + (b1 << 6));
        Sc2BonePair acc = bone_transform(m0, (const float *)(src + 0x00));
        acc = bone_transform_acc(m1, (const float *)(src + 0x20), acc);
        bone_store((float *)dst, acc);
        src += 0x40;
        dst += 0x20;
    }

    /* Pass 3: 3-bone weighted skinning (count3 vertices) */
    src = src_base + (count1 + count2 * 2u) * 0x20u;
    dst = dst_base + (count1 + count2) * 0x20u;
    for (uint32_t i = 0; i < count3; i++) {
        uint32_t b0 = *(const uint32_t *)(src + 0x1C);
        uint32_t b1 = *(const uint32_t *)(src + 0x3C);
        uint32_t b2 = *(const uint32_t *)(src + 0x5C);
        if (b0 >= 4096u || b1 >= 4096u || b2 >= 4096u) break;
        const float *m0 = (const float *)(bones + (b0 << 6));
        const float *m1 = (const float *)(bones + (b1 << 6));
        const float *m2 = (const float *)(bones + (b2 << 6));
        Sc2BonePair acc = bone_transform(m0, (const float *)(src + 0x00));
        acc = bone_transform_acc(m1, (const float *)(src + 0x20), acc);
        acc = bone_transform_acc(m2, (const float *)(src + 0x40), acc);
        bone_store((float *)dst, acc);
        src += 0x60;
        dst += 0x20;
    }

    /* Pass 4: N-bone variable skinning chain */
    src = src_base + (count1 + count2 * 2u + count3 * 3u) * 0x20u;
    dst = dst_base + (count1 + count2 + count3) * 0x20u;
    int32_t child_cnt = 0;
    int outer_iters = 0;

    for (;;) {
        uint32_t f_0c_bits = *(const uint32_t *)(src + 0x0C);
        float f_0c = *(const float *)(src + 0x0C);
        /* -ffast-math strips isnan(), so check IEEE-754 NaN bits explicitly */
        if ((f_0c_bits & 0x7FFFFFFFu) > 0x7F800000u || f_0c < ref_val) {
            break;
        }

        uint32_t b0 = *(const uint32_t *)(src + 0x1C);
        uint32_t b1 = *(const uint32_t *)(src + 0x3C);
        uint32_t b2 = *(const uint32_t *)(src + 0x5C);
        if (b0 >= 4096u || b1 >= 4096u || b2 >= 4096u) {
            break;
        }
        const float *m0 = (const float *)(bones + (b0 << 6));
        const float *m1 = (const float *)(bones + (b1 << 6));
        const float *m2 = (const float *)(bones + (b2 << 6));

        Sc2BonePair acc = bone_transform(m0, (const float *)(src + 0x00));
        acc = bone_transform_acc(m1, (const float *)(src + 0x20), acc);
        acc = bone_transform_acc(m2, (const float *)(src + 0x40), acc);

        const uint8_t *cur = src + 0x60;
        if (child_cnt > 0) {
            if ((uint32_t)child_cnt > 32u) {
                FILE *f = Switch_GetMainLogFile();
                if (f) {
                    fprintf(f, "[sub_00023C30 Safe] Clamping bogus child count: 0x%x -> 0\n",
                            (uint32_t)child_cnt);
                    fflush(f);
                }
            } else {
                for (int32_t c = 0; c < child_cnt; c++) {
                    uint32_t bc = (uint32_t)(*(cur + 0x1C));
                    const float *mc = (const float *)(bones + (bc << 6));
                    acc = bone_transform_acc(mc, (const float *)cur, acc);
                    cur += 0x20;
                }
            }
        }

        uint32_t bf = (uint32_t)(*(cur + 0x1C));
        const float *mf = (const float *)(bones + (bf << 6));
        acc = bone_transform_acc(mf, (const float *)cur, acc);
        bone_store((float *)dst, acc);

        uint32_t last_meta = *(const uint32_t *)(cur + 0x1C);
        child_cnt += (int32_t)(last_meta >> 8);

        src = cur + 0x20;
        dst += 0x20;
        outer_iters++;
        if (outer_iters > 1024) {
            FILE *f = Switch_GetMainLogFile();
            if (f) {
                fprintf(f, "[sub_00023C30 Safe] Outer loop reached 1024 iters! Terminating bone hierarchy cleanly.\n");
                fflush(f);
            }
            break;
        }
    }

    esp += 4;
}
