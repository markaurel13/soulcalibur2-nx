#pragma once
#include_next <sys/stat.h>

#ifndef UTIME_NOW
#define UTIME_NOW  ((1l << 30) - 1l)
#endif

#ifndef UTIME_OMIT
#define UTIME_OMIT ((1l << 30) - 2l)
#endif
