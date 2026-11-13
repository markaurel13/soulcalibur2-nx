#ifndef SC2_FAST_MEM_H
#define SC2_FAST_MEM_H

/*
 * Optimizaciones globales para recomp_0000..0007.c (Fase 1 + 1.5):
 * 1. Acceso directo a recomp_fast_ram para 0x00xxxxxx y alias cached 0x80xxxxxx
 *    mediante (addr & 0x7fffffffu) < 64 MB (1 sola mascara + 1 comparacion).
 * 2. XMM_MEM / XMM_STORE y aritmetica SSE inline con ARM64 NEON.
 * 3. recomp_guest_memcpy / recomp_guest_memset inline directo a recomp_fast_ram.
 * 4. RECOMP_FP_PC(x) directo a ((double)(float)(x)).
 */

#ifdef RECOMP_GENERATED_CODE

#include <arm_neon.h>

static inline uint8_t *sc2_mem_u8_ptr(uint32_t addr)
{
    const uint32_t norm = addr & 0x7fffffffu;
    if (__builtin_expect(norm < RECOMP_XBOX_RAM_SIZE, 1)) {
        return recomp_fast_ram + norm;
    }
    return recomp_memory(addr, 1u);
}

static inline uint16_t *sc2_mem_u16_ptr(uint32_t addr)
{
    const uint32_t norm = addr & 0x7fffffffu;
    if (__builtin_expect(norm <= (RECOMP_XBOX_RAM_SIZE - 2u), 1)) {
        return (uint16_t *)(void *)(recomp_fast_ram + norm);
    }
    return (uint16_t *)(void *)recomp_memory(addr, 2u);
}

static inline uint32_t *sc2_mem_u32_ptr(uint32_t addr)
{
    const uint32_t norm = addr & 0x7fffffffu;
    if (__builtin_expect(norm <= (RECOMP_XBOX_RAM_SIZE - 4u), 1)) {
        return (uint32_t *)(void *)(recomp_fast_ram + norm);
    }
    return recomp_memory_u32_checked(addr);
}

static inline uint64_t *sc2_mem_u64_ptr(uint32_t addr)
{
    const uint32_t norm = addr & 0x7fffffffu;
    if (__builtin_expect(norm <= (RECOMP_XBOX_RAM_SIZE - 8u), 1)) {
        return (uint64_t *)(void *)(recomp_fast_ram + norm);
    }
    return (uint64_t *)(void *)recomp_memory(addr, 8u);
}

#undef MEM8
#undef MEM16
#undef MEM32
#undef SMEM8
#undef SMEM16
#undef SMEM32
#undef SMEM64
#undef MEMF
#undef MEMD
#define MEM8(address)   (*sc2_mem_u8_ptr((uint32_t)(address)))
#define MEM16(address)  (*sc2_mem_u16_ptr((uint32_t)(address)))
#define MEM32(address)  (*sc2_mem_u32_ptr((uint32_t)(address)))
#define SMEM8(address)  (*(int8_t *)(void *)sc2_mem_u8_ptr((uint32_t)(address)))
#define SMEM16(address) (*(int16_t *)(void *)sc2_mem_u16_ptr((uint32_t)(address)))
#define SMEM32(address) (*(int32_t *)(void *)sc2_mem_u32_ptr((uint32_t)(address)))
#define SMEM64(address) (*(int64_t *)(void *)sc2_mem_u64_ptr((uint32_t)(address)))
#define MEMF(address)   (*(float *)(void *)sc2_mem_u32_ptr((uint32_t)(address)))
#define MEMD(address)   (*(double *)(void *)sc2_mem_u64_ptr((uint32_t)(address)))

static inline void sc2_guest_memcpy_fast(uint32_t dst, uint32_t src, size_t size)
{
    if (__builtin_expect(size == 0u, 0)) return;
    const uint32_t dnorm = dst & 0x7fffffffu;
    const uint32_t snorm = src & 0x7fffffffu;
    if (__builtin_expect(
            size <= RECOMP_XBOX_RAM_SIZE &&
            dnorm <= (RECOMP_XBOX_RAM_SIZE - size) &&
            snorm <= (RECOMP_XBOX_RAM_SIZE - size), 1)) {
        __builtin_memcpy(recomp_fast_ram + dnorm, recomp_fast_ram + snorm, size);
        return;
    }
    recomp_guest_memcpy(dst, src, size);
}

static inline void sc2_guest_memset_fast(uint32_t dst, int value, size_t size)
{
    if (__builtin_expect(size == 0u, 0)) return;
    const uint32_t dnorm = dst & 0x7fffffffu;
    if (__builtin_expect(
            size <= RECOMP_XBOX_RAM_SIZE &&
            dnorm <= (RECOMP_XBOX_RAM_SIZE - size), 1)) {
        __builtin_memset(recomp_fast_ram + dnorm, value, size);
        return;
    }
    recomp_guest_memset(dst, value, size);
}

#undef memcpy
#define memcpy(destination, source, size) \
    sc2_guest_memcpy_fast( \
        (uint32_t)(uintptr_t)(destination), \
        (uint32_t)(uintptr_t)(source), \
        (size_t)(size))

#undef memset
#define memset(destination, value, size) \
    sc2_guest_memset_fast( \
        (uint32_t)(uintptr_t)(destination), \
        (int)(value), \
        (size_t)(size))

#undef RECOMP_FP_PC
#define RECOMP_FP_PC(x) ((double)(float)(x))

static inline RecompXmm sc2_xmm_mem(uint32_t addr)
{
    RecompXmm r;
    const uint32_t norm = addr & 0x7fffffffu;
    if (__builtin_expect(norm <= (RECOMP_XBOX_RAM_SIZE - 16u), 1)) {
        vst1q_f32(r.f, vld1q_f32((const float *)(const void *)(recomp_fast_ram + norm)));
        return r;
    }
    recomp_guest_load(&r, addr, 16u);
    return r;
}

static inline void sc2_xmm_store(uint32_t addr, const RecompXmm *reg)
{
    const uint32_t norm = addr & 0x7fffffffu;
    if (__builtin_expect(norm <= (RECOMP_XBOX_RAM_SIZE - 16u), 1)) {
        vst1q_f32((float *)(void *)(recomp_fast_ram + norm), vld1q_f32(reg->f));
        return;
    }
    recomp_guest_store(addr, reg, 16u);
}

static inline void sc2_xmm_load_half(float *dst, uint32_t addr)
{
    const uint32_t norm = addr & 0x7fffffffu;
    if (__builtin_expect(norm <= (RECOMP_XBOX_RAM_SIZE - 8u), 1)) {
        *(uint64_t *)(void *)dst = *(const uint64_t *)(const void *)(recomp_fast_ram + norm);
        return;
    }
    recomp_guest_load(dst, addr, 8u);
}

static inline void sc2_xmm_store_half(uint32_t addr, const float *src)
{
    const uint32_t norm = addr & 0x7fffffffu;
    if (__builtin_expect(norm <= (RECOMP_XBOX_RAM_SIZE - 8u), 1)) {
        *(uint64_t *)(void *)(recomp_fast_ram + norm) = *(const uint64_t *)(const void *)src;
        return;
    }
    recomp_guest_store(addr, src, 8u);
}

#undef XMM_MEM
#undef XMM_STORE
#undef XMM_LOAD_LOW
#undef XMM_STORE_LOW
#undef XMM_LOAD_HIGH
#undef XMM_STORE_HIGH
#define XMM_MEM(address)             sc2_xmm_mem((uint32_t)(address))
#define XMM_STORE(address, reg)      sc2_xmm_store((uint32_t)(address), &(reg))
#define XMM_LOAD_LOW(reg, address)   sc2_xmm_load_half(&(reg).f[0], (uint32_t)(address))
#define XMM_STORE_LOW(address, reg)  sc2_xmm_store_half((uint32_t)(address), &(reg).f[0])
#define XMM_LOAD_HIGH(reg, address)  sc2_xmm_load_half(&(reg).f[2], (uint32_t)(address))
#define XMM_STORE_HIGH(address, reg) sc2_xmm_store_half((uint32_t)(address), &(reg).f[2])

static inline RecompXmm sc2_xmm_add(RecompXmm a, RecompXmm b)
{
    RecompXmm r;
    vst1q_f32(r.f, vaddq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm sc2_xmm_sub(RecompXmm a, RecompXmm b)
{
    RecompXmm r;
    vst1q_f32(r.f, vsubq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm sc2_xmm_mul(RecompXmm a, RecompXmm b)
{
    RecompXmm r;
    vst1q_f32(r.f, vmulq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm sc2_xmm_div(RecompXmm a, RecompXmm b)
{
    RecompXmm r;
    vst1q_f32(r.f, vdivq_f32(vld1q_f32(a.f), vld1q_f32(b.f)));
    return r;
}

static inline RecompXmm sc2_xmm_and(RecompXmm a, RecompXmm b)
{
    RecompXmm r;
    vst1q_u32(r.u, vandq_u32(vld1q_u32(a.u), vld1q_u32(b.u)));
    return r;
}

static inline RecompXmm sc2_xmm_andn(RecompXmm a, RecompXmm b)
{
    RecompXmm r;
    /* ~a & b = vbicq_u32(b, a) */
    vst1q_u32(r.u, vbicq_u32(vld1q_u32(b.u), vld1q_u32(a.u)));
    return r;
}

static inline RecompXmm sc2_xmm_or(RecompXmm a, RecompXmm b)
{
    RecompXmm r;
    vst1q_u32(r.u, vorrq_u32(vld1q_u32(a.u), vld1q_u32(b.u)));
    return r;
}

static inline RecompXmm sc2_xmm_xor(RecompXmm a, RecompXmm b)
{
    RecompXmm r;
    vst1q_u32(r.u, veorq_u32(vld1q_u32(a.u), vld1q_u32(b.u)));
    return r;
}

#define XMM_ADD  sc2_xmm_add
#define XMM_SUB  sc2_xmm_sub
#define XMM_MUL  sc2_xmm_mul
#define XMM_DIV  sc2_xmm_div
#define XMM_AND  sc2_xmm_and
#define XMM_ANDN sc2_xmm_andn
#define XMM_OR   sc2_xmm_or
#define XMM_XOR  sc2_xmm_xor

#endif /* RECOMP_GENERATED_CODE */

#endif /* SC2_FAST_MEM_H */
