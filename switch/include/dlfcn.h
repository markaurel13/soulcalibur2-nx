#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *dli_fname;
    void       *dli_fbase;
    const char *dli_sname;
    void       *dli_saddr;
} Dl_info;

static inline int dladdr(const void *addr, Dl_info *info) {
    if (info) {
        info->dli_fname = "";
        info->dli_fbase = (void *)0;
        info->dli_sname = "";
        info->dli_saddr = (void *)0;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
