#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initializes 4-core detection, strips Core 3 from the main thread if granted,
// and ensures the Sphaira forwarder main.npdm is installed on SD when launched
// from a Sphaira forwarder.
void SwitchCore_Init(const char *argv0);

// Returns 3 when Core 3 is granted and active, or -1 in standard 3-core mode.
int SwitchCore_GetSystemCore(void);

// Returns the raw process InfoType_CoreMask (0x7 or 0xF).
unsigned SwitchCore_GetProcessCoreMask(void);

// Moves the calling thread to Core 3 (if available) and optionally sets its
// Horizon priority (pass -1 to leave priority unchanged).
bool SwitchCore_MoveCurrentThreadToSystemCore(int priority);

#ifdef __cplusplus
}
#endif
