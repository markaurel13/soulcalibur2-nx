#include "d3d_capture_packet.h"
#include "d3d_presenter_backend.h"
#include "switch_d3d_worker.h"

#include <switch.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <new>
#include <system_error>
#include <thread>
#include <unordered_map>

namespace {

struct PresenterThread {
    std::thread::id owner_thread = std::this_thread::get_id();
    std::thread::id d3d_worker_thread_id;
    std::thread worker;
    std::mutex mutex;
    std::mutex submit_mutex;
    std::condition_variable changed;
    std::condition_variable wake_cv;

    D3dCapturePacket packets[3];
    unsigned open = 0;
    unsigned front = 0;
    unsigned pending = 0; // Number of packets queued or being executed by worker

    bool started = false;
    bool shutdown = false;
    RecompD3dPresenterError created = RECOMP_D3D_PRESENTER_OK;
    std::atomic<RecompD3dPresenterError> status{RECOMP_D3D_PRESENTER_OK};
    RecompD3dPresenterError destroyed = RECOMP_D3D_PRESENTER_OK;

    RecompD3dPresenter *backend_ptr = nullptr;
    unsigned draw_declines = 0;
    bool covered = true;

    // Guest visibility indices
    struct Visibility { unsigned pending = 0; uint32_t pixels = 0; };
    std::unordered_map<uint32_t, Visibility> visibility;

    ~PresenterThread() = default;
};

PresenterThread *active_thread = nullptr;

RecompD3dPresenterError validate(RecompD3dPresenter *presenter)
{
    if (presenter == nullptr ||
        presenter != reinterpret_cast<RecompD3dPresenter *>(active_thread)) {
        return RECOMP_D3D_PRESENTER_NOT_INITIALIZED;
    }
    const auto tid = std::this_thread::get_id();
    return (tid == active_thread->owner_thread || tid == active_thread->d3d_worker_thread_id)
        ? RECOMP_D3D_PRESENTER_OK : RECOMP_D3D_PRESENTER_WRONG_THREAD;
}

inline RecompD3dPresenterError status(PresenterThread &thread)
{
    return thread.status.load(std::memory_order_relaxed);
}

void fail(PresenterThread &thread, RecompD3dPresenterError error)
{
    if (error == RECOMP_D3D_PRESENTER_OK) return;
    RecompD3dPresenterError expected = RECOMP_D3D_PRESENTER_OK;
    thread.status.compare_exchange_strong(expected, error, std::memory_order_release);
    thread.changed.notify_all();
}

void execute(PresenterThread &thread, RecompD3dPresenter *backend,
    const D3dCapturePacket &packet)
{
    for (size_t i = 0; i < packet.count(); ++i) {
        if (status(thread) != RECOMP_D3D_PRESENTER_OK) break;
        const auto &record = packet.record(i);
        RecompD3dPresenterError error = RECOMP_D3D_PRESENTER_OK;
        bool draw = false;
        try {
            switch (record.kind) {
            case D3dCapturePacket::COMMAND: {
                const auto &command = packet.command(i);
                draw = command.type == RECOMP_D3D_PRESENTER_COMMAND_DRAW;
                if (command.type == RECOMP_D3D_PRESENTER_COMMAND_DRAW_COVERAGE) {
                    bool covered = true;
                    error = d3d_backend_draw_coverage(backend, &command.data.draw, &covered);
                    std::lock_guard<std::mutex> lock(thread.mutex);
                    thread.covered = covered;
                } else {
                    error = d3d_backend_submit(backend, &command);
                }
                break;
            }
            case D3dCapturePacket::RELEASE:
            case D3dCapturePacket::INVALIDATE:
                error = d3d_backend_release_memory(backend, record.base, record.size,
                    record.kind == D3dCapturePacket::RELEASE);
                break;
            case D3dCapturePacket::REPORT:
                d3d_backend_report_draw_textures();
                break;
            }
        } catch (const std::bad_alloc &) {
            error = RECOMP_D3D_PRESENTER_OUT_OF_MEMORY;
        } catch (...) {
            error = RECOMP_D3D_PRESENTER_HOST_FAILURE;
        }
        if (draw && error != RECOMP_D3D_PRESENTER_OK &&
            error != RECOMP_D3D_PRESENTER_CLOSED) {
            const unsigned declines = ++thread.draw_declines;
            if ((declines & (declines - 1u)) == 0u) {
                const auto &d = packet.command(i).data.draw;
                std::fprintf(stderr,
                    "recomp d3d presenter: draw declines=%u error=%d fvf=0x%08X"
                    " program=%u prim=%u target=0x%08X fmt=0x%02X %ux%u\n",
                    declines, static_cast<int>(error),
                    d.fvf, d.program_count, d.primitive_type, d.target.color.data,
                    d.target.color.format_byte, d.target.color.width,
                    d.target.color.height);
            }
        } else if (error != RECOMP_D3D_PRESENTER_OK &&
                   status(thread) == RECOMP_D3D_PRESENTER_OK) {
            std::fprintf(stderr,
                "recomp d3d presenter: thread failed error=%d kind=%d command=%d\n",
                static_cast<int>(error), static_cast<int>(record.kind),
                record.kind == D3dCapturePacket::COMMAND
                    ? static_cast<int>(packet.command(i).type) : -1);
            fail(thread, error);
        }
    }
}

bool resolveVisibility(PresenterThread &thread, RecompD3dPresenter *backend)
{
    for (;;) {
        uint32_t index = 0u, pixels = 0u;
        const auto poll = d3d_backend_poll_visibility(backend, &index, &pixels);
        if (poll == D3D_BACKEND_VISIBILITY_FAILED) {
            fail(thread, RECOMP_D3D_PRESENTER_HOST_FAILURE);
            return false;
        }
        if (poll != D3D_BACKEND_VISIBILITY_READY) return poll == D3D_BACKEND_VISIBILITY_PENDING;
        std::lock_guard<std::mutex> lock(thread.mutex);
        const auto found = thread.visibility.find(index);
        if (found != thread.visibility.end() && found->second.pending != 0u) {
            found->second.pixels = pixels;
            --found->second.pending;
        }
    }
}

void run(PresenterThread &thread, RecompD3dPresenterConfig config)
{
#if defined(__SWITCH__)
    // Pin worker thread to Core 1 and assign higher priority (0x2B vs default 0x2C)
    svcSetThreadPriority(CUR_THREAD_HANDLE, 0x2B);
    svcSetThreadCoreMask(CUR_THREAD_HANDLE, 1, 1ULL << 1);
#endif

    RecompD3dPresenter *backend = nullptr;
    RecompD3dPresenterError created = RECOMP_D3D_PRESENTER_OK;
    try {
        created = d3d_backend_create(&config, &backend);
    } catch (const std::bad_alloc &) {
        created = RECOMP_D3D_PRESENTER_OUT_OF_MEMORY;
    } catch (...) {
        created = RECOMP_D3D_PRESENTER_HOST_FAILURE;
    }

    {
        std::lock_guard<std::mutex> lock(thread.mutex);
        thread.backend_ptr = backend;
        thread.created = created;
        thread.status = created;
        thread.started = true;
    }
    thread.changed.notify_all();
    if (created != RECOMP_D3D_PRESENTER_OK) return;

    for (;;) {
        D3dCapturePacket *packet = nullptr;
        {
            std::unique_lock<std::mutex> lock(thread.mutex);
            thread.wake_cv.wait(lock, [&] {
                return thread.pending != 0 || thread.shutdown;
            });
            if (thread.pending != 0) {
                packet = &thread.packets[thread.front];
            } else if (thread.shutdown) {
                break;
            }
        }

        if (packet != nullptr) {
            execute(thread, backend, *packet);
            packet->clear();
            resolveVisibility(thread, backend);

            {
                std::lock_guard<std::mutex> lock(thread.mutex);
                thread.front = (thread.front + 1) % 3;
                --thread.pending;
            }
            thread.changed.notify_all();
        }
    }

    thread.destroyed = d3d_backend_destroy(&backend);
    std::fprintf(stderr, "recomp d3d presenter thread: worker stopped, draw declines=%u\n",
        thread.draw_declines);
}

RecompD3dPresenterError publish(PresenterThread &thread, bool destroying)
{
    std::unique_lock<std::mutex> lock(thread.mutex);
    thread.changed.wait(lock, [&] {
        return thread.pending < 2 ||
            (!destroying && thread.status != RECOMP_D3D_PRESENTER_OK);
    });
    if (!destroying && thread.status != RECOMP_D3D_PRESENTER_OK) return thread.status;
    thread.packets[thread.open].seal();
    thread.open = (thread.open + 1) % 3;
    ++thread.pending;
    lock.unlock();
    thread.wake_cv.notify_one();
    return RECOMP_D3D_PRESENTER_OK;
}

} // namespace

RecompD3dPresenterError recomp_d3d_presenter_create(
    const RecompD3dPresenterConfig *config, RecompD3dPresenter **presenter)
{
    if (config == nullptr || presenter == nullptr || config->width == 0 ||
        config->height == 0 ||
        config->color_format != RECOMP_D3D_PRESENTER_COLOR_FORMAT_BGRA8_UNORM ||
        config->depth_format != RECOMP_D3D_PRESENTER_DEPTH_FORMAT_D24S8) {
        return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    }
    if (*presenter != nullptr || active_thread != nullptr) {
        return RECOMP_D3D_PRESENTER_ALREADY_INITIALIZED;
    }
    std::unique_ptr<PresenterThread> thread;
    try {
        thread = std::make_unique<PresenterThread>();
        thread->worker = std::thread(run, std::ref(*thread), *config);
    } catch (const std::bad_alloc &) {
        return RECOMP_D3D_PRESENTER_OUT_OF_MEMORY;
    } catch (const std::system_error &) {
        return RECOMP_D3D_PRESENTER_HOST_FAILURE;
    }
    std::unique_lock<std::mutex> lock(thread->mutex);
    thread->changed.wait(lock, [&] { return thread->started; });
    const auto error = thread->created;
    lock.unlock();
    if (error != RECOMP_D3D_PRESENTER_OK) {
        thread->worker.join();
        return error;
    }
    active_thread = thread.release();
    *presenter = reinterpret_cast<RecompD3dPresenter *>(active_thread);
    Switch_D3dWorkerInit();
    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError Switch_PresenterSubmitDirect(
    RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *command)
{
    auto error = validate(presenter);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    auto &thread = *active_thread;
    error = status(thread);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    if (command == nullptr) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;

    if (command->type == RECOMP_D3D_PRESENTER_COMMAND_VISIBILITY_END) {
        try {
            std::lock_guard<std::mutex> lock(thread.mutex);
            ++thread.visibility[command->data.visibility_index].pending;
        } catch (const std::bad_alloc &) {
            return RECOMP_D3D_PRESENTER_OUT_OF_MEMORY;
        }
        std::lock_guard<std::mutex> submit_lock(thread.submit_mutex);
        error = thread.packets[thread.open].add(*command);
        if (error != RECOMP_D3D_PRESENTER_OK) {
            std::lock_guard<std::mutex> lock(thread.mutex);
            --thread.visibility[command->data.visibility_index].pending;
        }
        return error;
    }

    if (command->type == RECOMP_D3D_PRESENTER_COMMAND_PRESENT) {
        std::lock_guard<std::mutex> submit_lock(thread.submit_mutex);
        error = thread.packets[thread.open].add(*command);
        if (error != RECOMP_D3D_PRESENTER_OK) return error;
        return publish(thread, false);
    }

    std::lock_guard<std::mutex> submit_lock(thread.submit_mutex);
    error = thread.packets[thread.open].add(*command);
    return error;
}

RecompD3dPresenterError Switch_PresenterReleaseMemoryDirect(
    RecompD3dPresenter *presenter, uint32_t base, uint32_t size, bool is_release)
{
    auto error = validate(presenter);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    auto &thread = *active_thread;
    error = status(thread);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    std::lock_guard<std::mutex> submit_lock(thread.submit_mutex);
    return is_release
        ? thread.packets[thread.open].addRelease(base, size)
        : thread.packets[thread.open].addInvalidate(base, size);
}

RecompD3dPresenterError recomp_d3d_presenter_submit(
    RecompD3dPresenter *presenter, const RecompD3dPresenterCommand *command)
{
    if (Switch_D3dWorkerIsActive() && command != nullptr) {
        if (Switch_D3dWorkerEnqueueDirect(presenter, command)) {
            if (command->type == RECOMP_D3D_PRESENTER_COMMAND_PRESENT) {
                Switch_D3dWorkerFlush();
            }
            return RECOMP_D3D_PRESENTER_OK;
        }
        Switch_D3dWorkerFlush();
    }
    return Switch_PresenterSubmitDirect(presenter, command);
}

RecompD3dPresenterError recomp_d3d_presenter_release_memory(
    RecompD3dPresenter *presenter, uint32_t base, uint32_t size)
{
    if (Switch_D3dWorkerIsActive()) {
        if (Switch_D3dWorkerEnqueueReleaseMemory(presenter, base, size, true)) {
            return RECOMP_D3D_PRESENTER_OK;
        }
        Switch_D3dWorkerFlush();
    }
    return Switch_PresenterReleaseMemoryDirect(presenter, base, size, true);
}

RecompD3dPresenterError recomp_d3d_presenter_invalidate_textures(
    RecompD3dPresenter *presenter, uint32_t base, uint32_t size)
{
    if (Switch_D3dWorkerIsActive()) {
        if (Switch_D3dWorkerEnqueueReleaseMemory(presenter, base, size, false)) {
            return RECOMP_D3D_PRESENTER_OK;
        }
        Switch_D3dWorkerFlush();
    }
    return Switch_PresenterReleaseMemoryDirect(presenter, base, size, false);
}

RecompD3dPresenterError recomp_d3d_presenter_flush(RecompD3dPresenter *presenter, bool wait)
{
    auto error = validate(presenter);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    Switch_D3dWorkerFlush();
    auto &thread = *active_thread;
    error = status(thread);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    if (thread.packets[thread.open].count() != 0u) {
        error = publish(thread, false);
        if (error != RECOMP_D3D_PRESENTER_OK) return error;
    }
    if (!wait) return RECOMP_D3D_PRESENTER_OK;
    std::unique_lock<std::mutex> lock(thread.mutex);
    thread.changed.wait(lock, [&] {
        return thread.pending == 0u || thread.status != RECOMP_D3D_PRESENTER_OK;
    });
    return thread.status;
}

RecompD3dPresenterError recomp_d3d_presenter_draw_coverage(
    RecompD3dPresenter *presenter, const RecompD3dPresenterDrawCommand *draw, bool *covered)
{
    auto error = validate(presenter);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    if (draw == nullptr || covered == nullptr) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    *covered = true;
    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError recomp_d3d_presenter_visibility_result(
    RecompD3dPresenter *presenter, uint32_t index, bool *complete, uint32_t *pixels)
{
    const auto error = validate(presenter);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    if (complete == nullptr || pixels == nullptr) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    Switch_D3dWorkerFlush();
    auto &thread = *active_thread;
    std::lock_guard<std::mutex> lock(thread.mutex);
    if (thread.status != RECOMP_D3D_PRESENTER_OK) return thread.status;
    const auto found = thread.visibility.find(index);
    if (found == thread.visibility.end()) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    *complete = found->second.pending == 0u;
    *pixels = found->second.pixels;
    return RECOMP_D3D_PRESENTER_OK;
}

RecompD3dPresenterError recomp_d3d_presenter_read_frame(RecompD3dPresenter *presenter,
    uint32_t *pixels, uint32_t capacity, uint32_t *width, uint32_t *height)
{
    auto error = validate(presenter);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    error = recomp_d3d_presenter_flush(presenter, true);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    return d3d_backend_read_frame(active_thread->backend_ptr, pixels, capacity, width, height);
}

RecompD3dPresenterError recomp_d3d_presenter_show_image(RecompD3dPresenter *presenter,
    const uint32_t *pixels, uint32_t width, uint32_t height)
{
    auto error = validate(presenter);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    error = recomp_d3d_presenter_flush(presenter, true);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    return d3d_backend_show_image(active_thread->backend_ptr, pixels, width, height);
}

RecompD3dPresenterError recomp_d3d_presenter_destroy(RecompD3dPresenter **presenter)
{
    if (presenter == nullptr) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    const auto error = validate(*presenter);
    if (error != RECOMP_D3D_PRESENTER_OK) return error;
    Switch_D3dWorkerShutdown();
    auto &thread = *active_thread;
    if (thread.packets[thread.open].count() != 0) publish(thread, true);
    {
        std::lock_guard<std::mutex> lock(thread.mutex);
        thread.shutdown = true;
    }
    thread.wake_cv.notify_one();
    thread.worker.join();
    const auto cur_status = thread.status.load();
    const auto pending = cur_status == RECOMP_D3D_PRESENTER_CLOSED
        ? RECOMP_D3D_PRESENTER_OK : cur_status;
    const auto destroyed = thread.destroyed != RECOMP_D3D_PRESENTER_OK
        ? thread.destroyed : pending;
    delete active_thread;
    active_thread = nullptr;
    *presenter = nullptr;
    return destroyed;
}

void recomp_d3d_presenter_set_immediate_present(bool enabled)
{
    d3d_backend_set_immediate_present(enabled);
}

void recomp_d3d_presenter_report_draw_textures()
{
    if (active_thread == nullptr || std::this_thread::get_id() != active_thread->owner_thread) return;
    auto &thread = *active_thread;
    if (status(thread) == RECOMP_D3D_PRESENTER_OK) {
        fail(thread, thread.packets[thread.open].addReport());
    }
}

extern "C" void Switch_GetPresenterThreadState(int *pending, int *front, int *open, int *status) {
    if (active_thread) {
        if (pending) *pending = (int)active_thread->pending;
        if (front) *front = (int)active_thread->front;
        if (open) *open = (int)active_thread->open;
        if (status) *status = (int)active_thread->status;
    } else {
        if (pending) *pending = -1;
        if (front) *front = -1;
        if (open) *open = -1;
        if (status) *status = -1;
    }
}

extern "C" void *Switch_PresenterAllocPayload(size_t size) {
    if (__builtin_expect(!active_thread, 0)) return nullptr;
    std::lock_guard<std::mutex> lock(active_thread->submit_mutex);
    return active_thread->packets[active_thread->open].allocPayload(size);
}

extern "C" bool Switch_PresenterContainsPayload(const void *ptr) {
    if (__builtin_expect(!active_thread, 0)) return false;
    std::lock_guard<std::mutex> lock(active_thread->submit_mutex);
    return active_thread->packets[active_thread->open].containsPayload(ptr);
}

extern "C" void Switch_RegisterD3dWorkerThread(void) {
    if (active_thread) {
        active_thread->d3d_worker_thread_id = std::this_thread::get_id();
    }
}
