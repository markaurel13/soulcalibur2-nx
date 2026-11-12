#include "d3d_capture_packet.h"

#include <cassert>
#include <cstddef>
#include <cstring>
#include <new>
#include <stdexcept>

namespace {

using Draw = RecompD3dPresenterDrawCommand;
constexpr const void *Draw::*pointer_fields[] = {
    &Draw::vertex_bytes, &Draw::index_bytes, &Draw::texture_bytes,
    &Draw::palette_bytes, &Draw::alpha_mask_bytes,
    &Draw::alpha_mask_palette, &Draw::reflection_bytes, &Draw::reflection_stage2_bytes,
};
constexpr uint64_t max_span_bytes = 64u * 1024u * 1024u;
constexpr size_t null_offset = SIZE_MAX;

void copyRange(Draw &to, const Draw &from, size_t begin, size_t end)
{
    std::memcpy(reinterpret_cast<char *>(&to) + begin, reinterpret_cast<const char *>(&from) + begin, end - begin);
}

void copyCommand(RecompD3dPresenterCommand &to, const RecompD3dPresenterCommand &from)
{
    if ((from.type != RECOMP_D3D_PRESENTER_COMMAND_DRAW &&
         from.type != RECOMP_D3D_PRESENTER_COMMAND_DRAW_COVERAGE) || from.data.draw.program_count != 0u) {
        to = from;
        return;
    }
    using Lighting = RecompD3dDirectionalLighting;
    const Draw &draw = from.data.draw;
    Draw &out = to.data.draw;
    to.type = from.type;
    constexpr size_t lighting = offsetof(Draw, directional);
    copyRange(out, draw, 0u, offsetof(Draw, program));
    copyRange(out, draw, offsetof(Draw, program_alpha_mask), lighting + (draw.directional.enabled
        ? sizeof(Lighting) : offsetof(Lighting, normal_transforms)));
    if (!draw.directional.enabled) std::memcpy(out.directional.stage, draw.directional.stage, sizeof draw.directional.stage);
    copyRange(out, draw, lighting + sizeof(Lighting), offsetof(Draw, reflection_world_view));
    if (draw.has_reflection || draw.texgen_reflection || draw.texture_transform_uv || draw.texgen_projected)
        copyRange(out, draw, offsetof(Draw, reflection_world_view), offsetof(Draw, reflection_stage2));
    copyRange(out, draw, offsetof(Draw, reflection_stage2), sizeof(Draw));
}

} // namespace

D3dCapturePacket::D3dCapturePacket()
{
    // Pre-reserve buffers to eliminate reallocations during frame submission
    entries_.reserve(1024);
    commands_.reserve(1024);
    payload_.reserve(1024 * 1024); // 8 MB preallocated
}

RecompD3dPresenterError D3dCapturePacket::add(
    const RecompD3dPresenterCommand &command)
{
    if (sealed_ || static_cast<unsigned>(command.type) >
        RECOMP_D3D_PRESENTER_COMMAND_COPY_RECT) {
        return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    }

    uint64_t sizes[8]{};
    if (command.type == RECOMP_D3D_PRESENTER_COMMAND_DRAW ||
        command.type == RECOMP_D3D_PRESENTER_COMMAND_DRAW_COVERAGE) {
        const auto &draw = command.data.draw;
        sizes[0] = uint64_t(draw.vertex_count) * draw.vertex_stride;
        sizes[1] = uint64_t(draw.index_count) * 2u;
        if (command.type == RECOMP_D3D_PRESENTER_COMMAND_DRAW) {
            sizes[2] = draw.texture_byte_count;
            sizes[3] = draw.palette_byte_count;
            sizes[4] = draw.alpha_mask_byte_count;
            sizes[5] = draw.alpha_mask_palette_byte_count;
            sizes[6] = draw.reflection_byte_count;
            sizes[7] = draw.reflection_stage2_byte_count;
        }
        for (uint64_t size : sizes) {
            if (size > max_span_bytes) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
        }
    }

    const size_t old_payload_size = payload_.size();
    const size_t old_command_count = commands_.size();
    const size_t old_entry_count = entries_.size();
    try {
        CapturedCommand &captured = commands_.emplace_back();
        copyCommand(captured.value, command);
        if (command.type == RECOMP_D3D_PRESENTER_COMMAND_DRAW ||
            command.type == RECOMP_D3D_PRESENTER_COMMAND_DRAW_COVERAGE) {
            for (size_t i = 0u; i < 8u; ++i) {
                const void *source = command.data.draw.*pointer_fields[i];
                captured.value.data.draw.*pointer_fields[i] = nullptr;
                captured.offsets[i] = null_offset;
                if (source == nullptr || (command.type == RECOMP_D3D_PRESENTER_COMMAND_DRAW_COVERAGE && i >= 2u)) continue;

                const size_t size = static_cast<size_t>(sizes[i]);
                if (size == 0u) continue;

                // High-performance direct linear copy: eliminate hash table lookups and memcmp
                const size_t offset = payload_.size();
                const size_t words = (size + 7u) / 8u;
                if (words > payload_.max_size() - offset) throw std::length_error("capture payload");
                payload_.resize(offset + words);
                payload_[offset + words - 1u] = 0u;
                std::memcpy(payload_.data() + offset, source, size);
                captured.offsets[i] = offset;
            }
        }
        entries_.push_back({{COMMAND, 0u, 0u}, old_command_count});
        return RECOMP_D3D_PRESENTER_OK;
    } catch (const std::bad_alloc &) {
    } catch (const std::length_error &) {
    }

    entries_.resize(old_entry_count);
    commands_.resize(old_command_count);
    payload_.resize(old_payload_size);
    return RECOMP_D3D_PRESENTER_OUT_OF_MEMORY;
}

RecompD3dPresenterError D3dCapturePacket::addRecord(
    Kind kind, uint32_t base, uint32_t size)
{
    if (sealed_) return RECOMP_D3D_PRESENTER_INVALID_ARGUMENT;
    try {
        entries_.push_back({{kind, base, size}, 0u});
        return RECOMP_D3D_PRESENTER_OK;
    } catch (const std::bad_alloc &) {
    } catch (const std::length_error &) {
    }
    return RECOMP_D3D_PRESENTER_OUT_OF_MEMORY;
}

RecompD3dPresenterError D3dCapturePacket::addRelease(uint32_t base, uint32_t size)
{
    return addRecord(RELEASE, base, size);
}

RecompD3dPresenterError D3dCapturePacket::addInvalidate(uint32_t base, uint32_t size)
{
    return addRecord(INVALIDATE, base, size);
}

RecompD3dPresenterError D3dCapturePacket::addReport()
{
    return addRecord(REPORT, 0u, 0u);
}

void D3dCapturePacket::seal()
{
    if (sealed_) return;
    for (auto &captured : commands_) {
        if (captured.value.type != RECOMP_D3D_PRESENTER_COMMAND_DRAW &&
            captured.value.type != RECOMP_D3D_PRESENTER_COMMAND_DRAW_COVERAGE) continue;
        for (size_t i = 0u; i < 8u; ++i) {
            captured.value.data.draw.*pointer_fields[i] = captured.offsets[i] == null_offset
                ? nullptr : payload_.data() + captured.offsets[i];
        }
    }
    sealed_ = true;
}

size_t D3dCapturePacket::count() const
{
    return entries_.size();
}

const D3dCapturePacket::Record &D3dCapturePacket::record(size_t index) const
{
    return entries_[index].record;
}

const RecompD3dPresenterCommand &D3dCapturePacket::command(size_t index) const
{
    assert(sealed_ && entries_[index].record.kind == COMMAND);
    return commands_[entries_[index].command_index].value;
}

void D3dCapturePacket::clear()
{
    entries_.clear();
    commands_.clear();
    payload_.clear();
    sealed_ = false;
}

uint64_t D3dCapturePacket::bytes() const
{
    return uint64_t(commands_.size()) * sizeof(RecompD3dPresenterCommand) +
        uint64_t(entries_.size()) * sizeof(Record) + uint64_t(payload_.size()) * sizeof(uint64_t);
}
