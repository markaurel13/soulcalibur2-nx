#ifndef SWITCH_D3D_CAPTURE_PACKET_H
#define SWITCH_D3D_CAPTURE_PACKET_H

#include "d3d_presenter.h"

#include <cstddef>
#include <cstdlib>
#include <new>
#include <utility>
#include <vector>

class D3dCapturePacket {
public:
    enum Kind : uint32_t { COMMAND, RELEASE, REPORT, INVALIDATE };
    struct Record { Kind kind; uint32_t base; uint32_t size; };

    D3dCapturePacket();
    ~D3dCapturePacket() = default;
    D3dCapturePacket(const D3dCapturePacket &) = delete;
    D3dCapturePacket &operator=(const D3dCapturePacket &) = delete;
    D3dCapturePacket(D3dCapturePacket &&) noexcept = default;
    D3dCapturePacket &operator=(D3dCapturePacket &&) noexcept = default;

    RecompD3dPresenterError add(const RecompD3dPresenterCommand &command);
    RecompD3dPresenterError addRelease(uint32_t base, uint32_t size);
    RecompD3dPresenterError addInvalidate(uint32_t base, uint32_t size);
    RecompD3dPresenterError addReport();
    void *allocPayload(size_t bytes);
    bool containsPayload(const void *ptr) const;
    void seal();
    size_t count() const;
    const Record &record(size_t index) const;
    const RecompD3dPresenterCommand &command(size_t index) const;
    void clear();
    uint64_t bytes() const;

private:
    struct PayloadArena {
        uint8_t *data_ = nullptr;
        size_t size_ = 0;
        size_t capacity_ = 0;

        PayloadArena() {
            capacity_ = 10 * 1024 * 1024; // 10 MB pre-allocated
            data_ = static_cast<uint8_t *>(std::malloc(capacity_));
        }
        ~PayloadArena() {
            std::free(data_);
        }
        PayloadArena(const PayloadArena &) = delete;
        PayloadArena &operator=(const PayloadArena &) = delete;
        PayloadArena(PayloadArena &&other) noexcept
            : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        PayloadArena &operator=(PayloadArena &&other) noexcept {
            if (this != &other) {
                std::free(data_);
                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;
                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }
            return *this;
        }

        void clear() noexcept {
            size_ = 0;
        }

        void truncate(size_t old_size) noexcept {
            if (old_size < size_) size_ = old_size;
        }

        uint8_t *data() noexcept { return data_; }
        const uint8_t *data() const noexcept { return data_; }
        size_t size() const noexcept { return size_; }
        size_t capacity() const noexcept { return capacity_; }

        size_t alloc(size_t bytes) {
            const size_t offset = (size_ + 7u) & ~size_t(7u);
            const size_t needed = offset + bytes;
            if (__builtin_expect(needed > capacity_, 0)) {
                size_t new_cap = capacity_ * 2;
                if (new_cap < needed) new_cap = needed + (2 * 1024 * 1024);
                uint8_t *new_data = static_cast<uint8_t *>(std::realloc(data_, new_cap));
                if (!new_data) throw std::bad_alloc();
                data_ = new_data;
                capacity_ = new_cap;
            }
            size_ = needed;
            return offset;
        }
    };

    struct Entry { Record record; size_t command_index; };
    struct CapturedCommand {
        CapturedCommand() {}
        alignas(8) RecompD3dPresenterCommand value;
        size_t offsets[2];
    };

    RecompD3dPresenterError addRecord(Kind kind, uint32_t base, uint32_t size);

    std::vector<Entry> entries_;
    std::vector<CapturedCommand> commands_;
    PayloadArena payload_;
    bool sealed_ = false;
};

#endif // SWITCH_D3D_CAPTURE_PACKET_H
