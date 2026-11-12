#pragma once
#include <stddef.h>
#include <stdlib.h>

#define PROT_NONE       0x0
#define PROT_READ       0x1
#define PROT_WRITE      0x2
#define PROT_EXEC       0x4

#define MAP_SHARED      0x01
#define MAP_PRIVATE     0x02
#define MAP_FIXED       0x10
#define MAP_ANON        0x20
#define MAP_ANONYMOUS   MAP_ANON
#define MAP_FAILED      ((void *)-1)

#ifdef __cplusplus
extern "C" {
#endif

static inline void *mmap(void *addr, size_t length, int prot, int flags, int fd, long offset) {
    (void)addr; (void)prot; (void)flags; (void)fd; (void)offset;
    size_t aligned_len = (length + 4095) & ~4095UL;
    void *ptr = aligned_alloc(4096, aligned_len);
    if (!ptr) return MAP_FAILED;
    return ptr;
}

static inline int munmap(void *addr, size_t length) {
    (void)length;
    free(addr);
    return 0;
}

static inline int mprotect(void *addr, size_t len, int prot) {
    (void)addr; (void)len; (void)prot;
    return 0;
}

#ifdef __cplusplus
}
#endif
