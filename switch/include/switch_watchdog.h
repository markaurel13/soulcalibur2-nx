#pragma once
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void Switch_StartWatchdog(void);
void Switch_StopWatchdog(void);
void Switch_NotifyActivity(uint32_t frame);

FILE* Switch_GetMainLogFile(void);
void Switch_GetPresenterThreadState(int *pending, int *front, int *open, int *status);
void Switch_GetDeko3DState(int *current_slot, int *cur_cmd_slot, uint32_t *frame_counter);

#ifdef __cplusplus
}
#endif
