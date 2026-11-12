#pragma once

#ifdef __cplusplus
extern "C" {
#endif

static inline int backtrace(void **buffer, int size) {
    (void)buffer;
    (void)size;
    return 0;
}

static inline char **backtrace_symbols(void *const *buffer, int size) {
    (void)buffer;
    (void)size;
    return (char **)0;
}

#ifdef __cplusplus
}
#endif
