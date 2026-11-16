#pragma once

#ifdef __cplusplus
extern "C" {
#endif

bool SwitchModMenu_IsOpen(void);
bool SwitchModMenu_GetUnlockAll(void);
int SwitchModMenu_GetCursor(void);
bool SwitchModMenu_WasApplied(void);
void SwitchModMenu_ResetPauseState(void);
bool SwitchModMenu_TickPause(void);
void SwitchModMenu_WaitReleaseOnExit(void);

#ifdef __cplusplus
}
#endif
