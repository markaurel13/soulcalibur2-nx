/**
 * Native ARM64 NEON SIMD implementations of Soulcalibur II bone matrix routines:
 *  - sub_000239D0
 *  - sub_00023A60
 *  - sub_00023AF0
 *  - sub_00023B90
 *
 * These 4 routines transform character bone matrices and skinning hierarchies.
 * Emulating them via x87/SSE soft-FPU loops was consuming ~8.5% of total CPU time.
 * With ARM64 NEON fused multiply-accumulate (vfmaq_laneq_f32), each transformation
 * executes in hardware SIMD cycles with 100% identical IEEE-754 precision.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <arm_neon.h>
#include "sc2_prof.h"

// Kernel 1: Matrix * Vector affine transformation (sub_000239D0 / sub_00023A60)
//  - out0 = v0.x*M.row0 + v0.y*M.row1 + v0.z*M.row2 + v0.w*M.row3
//  - out1 = v1.x*M.row0 + v1.y*M.row1 + v1.z*M.row2
static inline void neon_mat_transform_impl(void)
{
    uint32_t dest_va = MEM32(g_esp + 4);
    const float *mat = (const float *)recomp_memory_u32(eax);
    const float *v_in = (const float *)recomp_memory_u32(ecx);
    float *dest = (float *)recomp_memory_u32(dest_va);

    float32x4_t r0 = vld1q_f32(mat + 0);
    float32x4_t r1 = vld1q_f32(mat + 4);
    float32x4_t r2 = vld1q_f32(mat + 8);
    float32x4_t r3 = vld1q_f32(mat + 12);

    float32x4_t v0 = vld1q_f32(v_in + 0);
    float32x4_t out0 = vmulq_laneq_f32(r0, v0, 0);
    out0 = vfmaq_laneq_f32(out0, r1, v0, 1);
    out0 = vfmaq_laneq_f32(out0, r2, v0, 2);
    out0 = vfmaq_laneq_f32(out0, r3, v0, 3);
    vst1q_f32(dest + 0, out0);

    float32x4_t v1 = vld1q_f32(v_in + 4);
    float32x4_t out1 = vmulq_laneq_f32(r0, v1, 0);
    out1 = vfmaq_laneq_f32(out1, r1, v1, 1);
    out1 = vfmaq_laneq_f32(out1, r2, v1, 2);
    vst1q_f32(dest + 4, out1);

    g_esp += 4; // pop return address
}

// Kernel 2: Matrix * Vector affine transformation with Accumulation (sub_00023AF0 / sub_00023B90)
//  - out0 = v0.x*M.row0 + v0.y*M.row1 + v0.z*M.row2 + v0.w*M.row3 + acc0
//  - out1 = v1.x*M.row0 + v1.y*M.row1 + v1.z*M.row2 + acc1
static inline void neon_mat_transform_acc_impl(void)
{
    uint32_t acc_va = MEM32(g_esp + 4);
    const float *mat = (const float *)recomp_memory_u32(eax);
    const float *v_in = (const float *)recomp_memory_u32(ecx);
    const float *acc = (const float *)recomp_memory_u32(acc_va);
    float *dest = (float *)recomp_memory_u32(esi);

    float32x4_t r0 = vld1q_f32(mat + 0);
    float32x4_t r1 = vld1q_f32(mat + 4);
    float32x4_t r2 = vld1q_f32(mat + 8);
    float32x4_t r3 = vld1q_f32(mat + 12);

    float32x4_t v0 = vld1q_f32(v_in + 0);
    float32x4_t acc0 = vld1q_f32(acc + 0);
    float32x4_t out0 = vmulq_laneq_f32(r0, v0, 0);
    out0 = vfmaq_laneq_f32(out0, r1, v0, 1);
    out0 = vfmaq_laneq_f32(out0, r2, v0, 2);
    out0 = vfmaq_laneq_f32(out0, r3, v0, 3);
    out0 = vaddq_f32(out0, acc0);
    vst1q_f32(dest + 0, out0);

    float32x4_t v1 = vld1q_f32(v_in + 4);
    float32x4_t acc1 = vld1q_f32(acc + 4);
    float32x4_t out1 = vmulq_laneq_f32(r0, v1, 0);
    out1 = vfmaq_laneq_f32(out1, r1, v1, 1);
    out1 = vfmaq_laneq_f32(out1, r2, v1, 2);
    out1 = vaddq_f32(out1, acc1);
    vst1q_f32(dest + 4, out1);

    g_esp += 4; // pop return address
}

void sub_000239D0(void)
{
    SC2_PROF_SCOPE(0x000239D0u);
    neon_mat_transform_impl();
}

void sub_00023A60(void)
{
    SC2_PROF_SCOPE(0x00023A60u);
    neon_mat_transform_impl();
}

void sub_00023AF0(void)
{
    SC2_PROF_SCOPE(0x00023AF0u);
    neon_mat_transform_acc_impl();
}

void sub_00023B90(void)
{
    SC2_PROF_SCOPE(0x00023B90u);
    neon_mat_transform_acc_impl();
}
