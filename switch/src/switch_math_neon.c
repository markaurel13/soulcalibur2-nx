/**
 * Native ARM64 NEON SIMD implementations of hot guest math functions.
 * Replaces emulated x87 / SSE scalar emulation with single-cycle NEON vector instructions.
 * 100% IEEE-754 32-bit floating point compliant.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include "sc2_prof.h"
#include <arm_neon.h>
#include <math.h>

// sub_00011000: fsincos with scaling factor
void __wrap_sub_00011000(void)
{
    SC2_PROF_SCOPE(0x00011000u);
    float angle = MEMF(esp + 4) * MEMF(0x176188);
    uint32_t ptr_cos = MEM32(esp + 0xC);
    uint32_t ptr_sin = MEM32(esp + 8);
    MEMF(ptr_cos) = cosf(angle);
    MEMF(ptr_sin) = sinf(angle);
    esp += 4;
}

// sub_00011250: 4x4 matrix multiplication (ecx = a * b)
void __wrap_sub_00011250(void)
{
    SC2_PROF_SCOPE(0x00011250u);
    uint32_t a_addr = MEM32(esp + 4);
    uint32_t b_addr = MEM32(esp + 8);
    uint32_t out_addr = ecx;

    float32x4_t a0 = vld1q_f32((const float *)recomp_memory(a_addr, 16));
    float32x4_t a1 = vld1q_f32((const float *)recomp_memory(a_addr + 16, 16));
    float32x4_t a2 = vld1q_f32((const float *)recomp_memory(a_addr + 32, 16));
    float32x4_t a3 = vld1q_f32((const float *)recomp_memory(a_addr + 48, 16));

    for (int i = 0; i < 4; ++i) {
        float32x4_t b_row = vld1q_f32((const float *)recomp_memory(b_addr + i * 16, 16));
        float32x4_t r = vmulq_laneq_f32(a0, b_row, 0);
        r = vmlaq_laneq_f32(r, a1, b_row, 1);
        r = vmlaq_laneq_f32(r, a2, b_row, 2);
        r = vmlaq_laneq_f32(r, a3, b_row, 3);
        vst1q_f32((float *)recomp_memory(out_addr + i * 16, 16), r);
    }
    esp += 12; // ret 8 + 4
}

// sub_000126A0: vec4 scale (eax = edx * ecx_val)
void __wrap_sub_000126A0(void)
{
    SC2_PROF_SCOPE(0x000126A0u);
    uint32_t src_addr = MEM32(esp + 8);
    float scale = MEMF(esp + 4);
    uint32_t dst_addr = ecx;

    float32x4_t v = vld1q_f32((const float *)recomp_memory(src_addr, 16));
    float32x4_t scaled = vmulq_n_f32(v, scale);
    vst1q_f32((float *)recomp_memory(dst_addr, 16), scaled);
    eax = dst_addr;
    esp += 12; // ret 8 + 4
}

// sub_00145A64: _ftol2 (float/double on top of x87 stack to int32 in eax, edx)
void __wrap_sub_00145A64(void)
{
    SC2_PROF_SCOPE(0x00145A64u);
    double val = g_fp_stack[g_fp_top];
    g_fp_top = (g_fp_top + 1u) & 7u; // fp_pop
    int64_t i64 = (int64_t)val;
    eax = (uint32_t)i64;
    edx = (uint32_t)(i64 >> 32);
    esp += 4;
}
