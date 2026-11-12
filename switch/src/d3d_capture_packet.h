#ifndef SWITCH_D3D_CAPTURE_PACKET_H
#define SWITCH_D3D_CAPTURE_PACKET_H

#include "d3d_presenter.h"

#include <cstddef>
#include <new>
#include <utility>
#include <vector>

class D3dCapturePacket {
public:
    enum Kind : uint32_t { COMMAND, RELEASE, REPORT, INVALIDATE };
    struct Record { Kind kind; uint32_t base; uint32_t size; };

    D3dCapturePacket();
    D3dCapturePacket(const D3dCapturePacket &) = delete;
    D3dCapturePacket &operator=(const D3dCapturePacket &) = delete;
    D3dCapturePacket(D3dCapturePacket &&) noexcept = default;
    D3dCapturePacket &operator=(D3dCapturePacket &&) noexcept = default;

    RecompD3dPresenterError add(const RecompD3dPresenterCommand &command);
    RecompD3dPresenterError addRelease(uint32_t base, uint32_t size);
    RecompD3dPresenterError addInvalidate(uint32_t base, uint32_t size);
    RecompD3dPresenterError addReport();
    void seal();
    size_t count() const;
    const Record &record(size_t index) const;
    const RecompD3dPresenterCommand &command(size_t index) const;
    void clear();
    uint64_t bytes() const;

private:
    template <typename T>
    struct NoInitAllocator : std::allocator<T> {
        using std::allocator<T>::allocator;
        template <typename U>
        struct rebind { using other = NoInitAllocator<U>; };

        template <typename U>
        void construct(U *pointer) {
            ::new (static_cast<void *>(pointer)) U;
        }

        template <typename U, typename... Args>
        void construct(U *pointer, Args &&...args) {
            ::new (static_cast<void *>(pointer)) U(std::forward<Args>(args)...);
        }
    };

    struct Entry { Record record; size_t command_index; };
    struct CapturedCommand {
        CapturedCommand() {}
        alignas(8) RecompD3dPresenterCommand value;
        size_t offsets[8];
    };

    RecompD3dPresenterError addRecord(Kind kind, uint32_t base, uint32_t size);

    std::vector<Entry> entries_;
    std::vector<CapturedCommand> commands_;
    std::vector<uint64_t, NoInitAllocator<uint64_t>> payload_;
    bool sealed_ = false;
};

#endif // SWITCH_D3D_CAPTURE_PACKET_H
