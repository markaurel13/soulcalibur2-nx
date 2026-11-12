#pragma once
#include <sys/types.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline ssize_t getxattr(const char *path, const char *name, void *value, size_t size, ...) {
    (void)path; (void)name; (void)value; (void)size;
    errno = ENOTSUP;
    return -1;
}

static inline int setxattr(const char *path, const char *name, const void *value, size_t size, ...) {
    (void)path; (void)name; (void)value; (void)size;
    errno = ENOTSUP;
    return -1;
}

static inline int fsetxattr(int fd, const char *name, const void *value, size_t size, ...) {
    (void)fd; (void)name; (void)value; (void)size;
    errno = ENOTSUP;
    return -1;
}

static inline ssize_t fgetxattr(int fd, const char *name, void *value, size_t size, ...) {
    (void)fd; (void)name; (void)value; (void)size;
    errno = ENOTSUP;
    return -1;
}

#ifdef __cplusplus
}
#endif
