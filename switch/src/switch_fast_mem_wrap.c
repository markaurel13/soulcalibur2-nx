// Fast guest-memory wrappers compiled with LTO so callers can inline them directly.
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#ifdef __aarch64__
#include <arm_neon.h>
#endif

static inline void sc2_fast_copy(void *dst, const void *src, size_t size) {
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    if (size == 4u) {
        uint32_t v;
        memcpy(&v, s, 4u);
        memcpy(d, &v, 4u);
        return;
    }
    if (size == 8u) {
        uint64_t v;
        memcpy(&v, s, 8u);
        memcpy(d, &v, 8u);
        return;
    }
    if (size == 16u) {
#ifdef __aarch64__
        vst1q_u8(d, vld1q_u8(s));
#else
        memcpy(d, s, 16u);
#endif
        return;
    }
    if (size == 64u) {
#ifdef __aarch64__
        uint8x16_t v0 = vld1q_u8(s + 0);
        uint8x16_t v1 = vld1q_u8(s + 16);
        uint8x16_t v2 = vld1q_u8(s + 32);
        uint8x16_t v3 = vld1q_u8(s + 48);
        vst1q_u8(d + 0, v0);
        vst1q_u8(d + 16, v1);
        vst1q_u8(d + 32, v2);
        vst1q_u8(d + 48, v3);
#else
        memcpy(d, s, 64u);
#endif
        return;
    }
    if (size <= 256u) {
#ifdef __aarch64__
        while (size >= 64u) {
            uint8x16_t v0 = vld1q_u8(s + 0);
            uint8x16_t v1 = vld1q_u8(s + 16);
            uint8x16_t v2 = vld1q_u8(s + 32);
            uint8x16_t v3 = vld1q_u8(s + 48);
            vst1q_u8(d + 0, v0);
            vst1q_u8(d + 16, v1);
            vst1q_u8(d + 32, v2);
            vst1q_u8(d + 48, v3);
            d += 64u;
            s += 64u;
            size -= 64u;
        }
        while (size >= 16u) {
            vst1q_u8(d, vld1q_u8(s));
            d += 16u;
            s += 16u;
            size -= 16u;
        }
#endif
        while (size >= 8u) {
            uint64_t v;
            memcpy(&v, s, 8u);
            memcpy(d, &v, 8u);
            d += 8u;
            s += 8u;
            size -= 8u;
        }
        if (size >= 4u) {
            uint32_t v;
            memcpy(&v, s, 4u);
            memcpy(d, &v, 4u);
            d += 4u;
            s += 4u;
            size -= 4u;
        }
        while (size--) *d++ = *s++;
        return;
    }
    memcpy(d, s, size);
}

extern uint8_t *recomp_fast_ram;
uint64_t g_sc2_mem_wrap_calls[4] = {0, 0, 0, 0};
uint64_t g_sc2_mem_miss_calls = 0;
uint32_t g_sc2_mem_sample_addr[4] = {0, 0, 0, 0};
uint32_t g_sc2_mem_sample_width[4] = {0, 0, 0, 0};
uint8_t *__real_recomp_memory(uint32_t guest_address, size_t width);
uint32_t *__real_recomp_memory_u32_checked(uint32_t guest_address);
void __real_recomp_guest_load(void *destination, uint32_t source, size_t size);
void __real_recomp_guest_store(uint32_t destination, const void *source, size_t size);
void __real_recomp_guest_memcpy(uint32_t destination, uint32_t source, size_t size);
void __real_recomp_guest_memset(uint32_t destination, int value, size_t size);

static inline bool sc2_is_guest_ram(uint32_t guest_address, size_t width) {
    if (guest_address >= 0xfd000000u && guest_address < 0xff000000u) {
        return false;
    }
    uint32_t offset = guest_address & 0x03ffffffu;
    return (offset + width) <= 0x04000000u;
}

static inline uint32_t sc2_normalize_guest_address(uint32_t guest_address) {
    if (guest_address >= 0xfd000000u && guest_address < 0xff000000u) {
        return guest_address;
    }
    return 0x80000000u | (guest_address & 0x03ffffffu);
}

__attribute__((used))
uint8_t *__wrap_recomp_memory(uint32_t guest_address, size_t width) {
    uint64_t idx = g_sc2_mem_wrap_calls[0]++;
    if (sc2_is_guest_ram(guest_address, width)) {
        if (idx < 4ULL || (idx & 4095ULL) == 0ULL) {
            uint32_t slot = (uint32_t)(idx & 3ULL);
            g_sc2_mem_sample_addr[slot] = guest_address;
            g_sc2_mem_sample_width[slot] = (uint32_t)width;
        }
        uint32_t offset = guest_address & 0x03ffffffu;
        if (recomp_fast_ram != NULL) {
            return recomp_fast_ram + offset;
        }
        guest_address = 0x80000000u | offset;
    } else {
        uint64_t m = g_sc2_mem_miss_calls++;
        uint32_t slot = (uint32_t)(m & 3ULL);
        g_sc2_mem_sample_addr[slot] = guest_address;
        g_sc2_mem_sample_width[slot] = (uint32_t)width;
    }
    return __real_recomp_memory(guest_address, width);
}

__attribute__((used))
uint32_t *__wrap_recomp_memory_u32_checked(uint32_t guest_address) {
    ++g_sc2_mem_wrap_calls[1];
    if (sc2_is_guest_ram(guest_address, 4u)) {
        uint32_t offset = guest_address & 0x03ffffffu;
        if (recomp_fast_ram != NULL) {
            return (uint32_t *)(void *)(recomp_fast_ram + offset);
        }
        guest_address = 0x80000000u | offset;
    }
    return __real_recomp_memory_u32_checked(guest_address);
}

__attribute__((used))
void __wrap_recomp_guest_load(void *destination, uint32_t source, size_t size) {
    ++g_sc2_mem_wrap_calls[2];
    if (size == 0) return;
    if (sc2_is_guest_ram(source, size)) {
        uint32_t offset = source & 0x03ffffffu;
        if (recomp_fast_ram != NULL) {
            sc2_fast_copy(destination, recomp_fast_ram + offset, size);
            return;
        }
        source = 0x80000000u | offset;
    }
    __real_recomp_guest_load(destination, source, size);
}

__attribute__((used))
void __wrap_recomp_guest_store(uint32_t destination, const void *source, size_t size) {
    ++g_sc2_mem_wrap_calls[3];
    if (size == 0) return;
    if (sc2_is_guest_ram(destination, size)) {
        uint32_t offset = destination & 0x03ffffffu;
        if (recomp_fast_ram != NULL) {
            sc2_fast_copy(recomp_fast_ram + offset, source, size);
            return;
        }
        destination = 0x80000000u | offset;
    }
    __real_recomp_guest_store(destination, source, size);
}

__attribute__((used))
void __wrap_recomp_guest_memcpy(uint32_t destination, uint32_t source, size_t size) {
    if (size == 0) return;
    if (recomp_fast_ram != NULL && sc2_is_guest_ram(destination, size) && sc2_is_guest_ram(source, size)) {
        uint8_t *d = recomp_fast_ram + (destination & 0x03ffffffu);
        const uint8_t *s = recomp_fast_ram + (source & 0x03ffffffu);
        if (d + size <= s || s + size <= d) {
            sc2_fast_copy(d, s, size);
        } else {
            memmove(d, s, size);
        }
        return;
    }
    destination = sc2_normalize_guest_address(destination);
    source = sc2_normalize_guest_address(source);
    __real_recomp_guest_memcpy(destination, source, size);
}

__attribute__((used))
void __wrap_recomp_guest_memset(uint32_t destination, int value, size_t size) {
    if (size == 0) return;
    if (recomp_fast_ram != NULL && sc2_is_guest_ram(destination, size)) {
        uint8_t *d = recomp_fast_ram + (destination & 0x03ffffffu);
        memset(d, value, size);
        return;
    }
    destination = sc2_normalize_guest_address(destination);
    __real_recomp_guest_memset(destination, value, size);
}
