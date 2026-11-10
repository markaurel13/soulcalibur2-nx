#include "d3d_presenter_backend.h"
#include <switch.h>
#include <deko3d.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define NUM_FRAMEBUFFERS 2
#define CMD_BUF_SIZE (2 * 1024 * 1024)

struct RecompD3dPresenter {
    DkDevice device;
    DkQueue queue;
    DkSwapchain swapchain;

    DkMemBlock block_cmd;
    DkCmdBuf cmdbuf;

    DkImage framebuffers[NUM_FRAMEBUFFERS];
    DkImage const* framebuffer_ptrs[NUM_FRAMEBUFFERS];
    DkImageView framebuffer_views[NUM_FRAMEBUFFERS];
    DkImageView const* target_ptrs[NUM_FRAMEBUFFERS];

    uint32_t width;
    uint32_t height;
    bool immediate_present;
};

RecompD3dPresenterError d3d_backend_create(
    const RecompD3dPresenterConfig *config, RecompD3dPresenter **out_presenter) {
    if (!out_presenter) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;

    printf("[Deko3D] Initializing hardware device and graphics queue...\n");

    RecompD3dPresenter *p = (RecompD3dPresenter *)calloc(1, sizeof(RecompD3dPresenter));
    if (!p) return RECOMP_D3D_PRESENTER_OUT_OF_MEMORY;

    p->width = config && config->width ? config->width : 1280;
    p->height = config && config->height ? config->height : 720;
    p->immediate_present = false;

    // 1. Create Device
    DkDeviceMaker devMaker;
    dkDeviceMakerDefaults(&devMaker);
    p->device = dkDeviceCreate(&devMaker);

    // 2. Create Graphics Queue
    DkQueueMaker queueMaker;
    dkQueueMakerDefaults(&queueMaker, p->device);
    queueMaker.flags = DkQueueFlags_Graphics;
    p->queue = dkQueueCreate(&queueMaker);

    // 3. Command Buffer Memory Block & CmdBuf
    DkMemBlockMaker blockMaker;
    dkMemBlockMakerDefaults(&blockMaker, p->device, CMD_BUF_SIZE);
    blockMaker.flags = DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached;
    p->block_cmd = dkMemBlockCreate(&blockMaker);

    DkCmdBufMaker cmdBufMaker;
    dkCmdBufMakerDefaults(&cmdBufMaker, p->device);
    p->cmdbuf = dkCmdBufCreate(&cmdBufMaker);
    dkCmdBufAddMemory(p->cmdbuf, p->block_cmd, 0, CMD_BUF_SIZE);

    // 4. Swapchain (double-buffered)
    for (int i = 0; i < NUM_FRAMEBUFFERS; i++) {
        p->framebuffer_ptrs[i] = &p->framebuffers[i];
    }

    DkSwapchainMaker swapMaker;
    dkSwapchainMakerDefaults(&swapMaker, p->device, nwindowGetDefault(), p->framebuffer_ptrs, NUM_FRAMEBUFFERS);
    p->swapchain = dkSwapchainCreate(&swapMaker);

    for (int i = 0; i < NUM_FRAMEBUFFERS; i++) {
        dkImageViewDefaults(&p->framebuffer_views[i], &p->framebuffers[i]);
        p->target_ptrs[i] = &p->framebuffer_views[i];
    }

    printf("[Deko3D] Renderer ready: %ux%u (Double Buffered)\n", p->width, p->height);

    *out_presenter = p;
    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError d3d_backend_submit(
    RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *command) {
    if (!presenter || !command) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;

    if (command->type == RECOMP_D3D_PRESENTER_COMMAND_PRESENT) {
        int slot = dkQueueAcquireImage(presenter->queue, presenter->swapchain);
        if (slot >= 0) {
            // Bind backbuffer attachment
            dkCmdBufBindRenderTargets(presenter->cmdbuf, &presenter->target_ptrs[slot], 1, nullptr);

            // Finish and submit recorded commands to Maxwell GPU
            DkCmdList cmdList = dkCmdBufFinishList(presenter->cmdbuf);
            dkQueueSubmitCommands(presenter->queue, cmdList);

            // Present to display
            dkQueuePresentImage(presenter->queue, presenter->swapchain, slot);

            // Reset command buffer for next frame
            dkCmdBufClear(presenter->cmdbuf);
            dkCmdBufAddMemory(presenter->cmdbuf, presenter->block_cmd, 0, CMD_BUF_SIZE);
        }
    }

    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError d3d_backend_draw_coverage(
    RecompD3dPresenter *presenter, const RecompD3dPresenterDrawCommand *draw, bool *covered) {
    (void)presenter;
    (void)draw;
    if (covered) *covered = true;
    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError d3d_backend_release_memory(
    RecompD3dPresenter *presenter, uint32_t base, uint32_t size, bool targets) {
    (void)presenter;
    (void)base;
    (void)size;
    (void)targets;
    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError d3d_backend_destroy(RecompD3dPresenter **out_presenter) {
    if (!out_presenter || !*out_presenter) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    RecompD3dPresenter *p = *out_presenter;

    dkQueueWaitIdle(p->queue);
    dkSwapchainDestroy(p->swapchain);
    dkCmdBufDestroy(p->cmdbuf);
    dkMemBlockDestroy(p->block_cmd);
    dkQueueDestroy(p->queue);
    dkDeviceDestroy(p->device);

    free(p);
    *out_presenter = nullptr;
    return RECOMP_D3D_PRESENTER_OK;
}

void d3d_backend_set_immediate_present(bool enabled) {
    (void)enabled;
}

void d3d_backend_report_draw_textures() {
}

RecompD3dPresenterError d3d_backend_read_frame(RecompD3dPresenter *presenter,
    uint32_t *pixels, uint32_t capacity, uint32_t *width, uint32_t *height) {
    (void)presenter;
    (void)pixels;
    (void)capacity;
    if (width) *width = 0;
    if (height) *height = 0;
    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError d3d_backend_show_image(RecompD3dPresenter *presenter,
    const uint32_t *pixels, uint32_t width, uint32_t height) {
    (void)presenter;
    (void)pixels;
    (void)width;
    (void)height;
    return RECOMP_D3D_PRESENTER_OK;
}

D3dBackendVisibilityPoll d3d_backend_poll_visibility(
    RecompD3dPresenter *presenter, uint32_t *index, uint32_t *pixels) {
    (void)presenter;
    if (index) *index = 0;
    if (pixels) *pixels = 0;
    return D3D_BACKEND_VISIBILITY_IDLE;
}
