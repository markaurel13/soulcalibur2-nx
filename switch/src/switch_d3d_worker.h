#ifndef SWITCH_D3D_WORKER_H
#define SWITCH_D3D_WORKER_H

#include "d3d_presenter.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SwitchGatherPartDesc {
    const uint8_t *source;
    uint32_t step;
    uint32_t size;
    uint32_t offset;
} SwitchGatherPartDesc;

typedef struct SwitchGatherDrawJob {
    RecompD3dPresenterCommand cmd;
    uint32_t num_parts;
    SwitchGatherPartDesc parts[8];
    uint32_t vertex_count;
    uint32_t vertex_stride;
    const uint16_t *indices;
    uint32_t index_count;
    uint32_t smallest;
    uint16_t *owned_sequential;
} SwitchGatherDrawJob;

typedef struct SwitchPushShadowJob {
    uint32_t push_offset;
    uint32_t push_bytes;
    uint32_t fvf;
    RecompD3dPresenterTarget target;
    RecompD3dBlendState blend;
    RecompD3dDepthState depth;
    RecompD3dCullMode cull_mode;
    int32_t depth_bias;
    float slope_scaled_depth_bias;
    uint32_t texture_factor;
    float transform[16];
    bool has_transform;
    float viewport[6];
    float screen_space_offset[2];
    int32_t scissor[4];
} SwitchPushShadowJob;

void Switch_D3dWorkerInit(void);
void Switch_D3dWorkerShutdown(void);
bool Switch_D3dWorkerIsActive(void);

bool Switch_D3dWorkerEnqueueDirect(RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *cmd);
bool Switch_D3dWorkerEnqueueGatherDraw(RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *cmd, const SwitchGatherDrawJob *job);
uint8_t *Switch_D3dWorkerAllocPushBytes(uint32_t bytes, uint32_t *out_offset);
bool Switch_D3dWorkerEnqueuePushShadow(RecompD3dPresenter *presenter, const SwitchPushShadowJob *job);
bool Switch_D3dWorkerEnqueueReleaseMemory(RecompD3dPresenter *presenter, uint32_t base, uint32_t size, bool is_release);
void Switch_D3dWorkerFlush(void);
void Switch_D3dWorkerEndFrame(void);
void Switch_TakeWorkerProf(uint32_t *direct_cnt, uint32_t *gather_cnt, uint32_t *push_cnt,
    uint64_t *exec_ticks, uint64_t *flush_ticks, uint64_t *enqueue_ticks,
    uint32_t *wakeups, uint32_t *full_spins);

/* Direct presenter entrypoints (bypassing worker queue, called by worker itself) */
RecompD3dPresenterError Switch_PresenterSubmitDirect(RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *command);
RecompD3dPresenterError Switch_PresenterReleaseMemoryDirect(RecompD3dPresenter *presenter, uint32_t base, uint32_t size, bool is_release);
void *Switch_PresenterAllocPayload(size_t size);

#ifdef __cplusplus
}
#endif

#endif
