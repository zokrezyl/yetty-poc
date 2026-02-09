#pragma once

#include <yetty/result.hpp>
#include <yetty/gpu-context.h>
#include <yetty/gpu-allocator.h>
#include <webgpu/webgpu.h>
#include <cstdint>
#include <memory>
#include <string>

namespace yetty {

constexpr uint32_t MAX_CARD_SLOTS = 16384;        // Max unique card slots to track

// Handle types for type safety
struct MetadataHandle {
    uint32_t offset;
    uint32_t size;

    bool isValid() const { return size > 0; }
    static MetadataHandle invalid() { return {0, 0}; }
};

struct BufferHandle {
    uint8_t* data;      // Raw pointer to CPU buffer - write directly here
    uint32_t offset;    // Byte offset (for metadata references)
    uint32_t size;      // Allocation size in bytes

    bool isValid() const { return size > 0; }
    static BufferHandle invalid() { return {nullptr, 0, 0}; }
};

/**
 * CardBufferManager manages the GPU linear storage buffer (binding 2).
 *
 * Buffer cards (plot, hdraw, ydraw, kdraw, jdraw) use this for float arrays.
 * Metadata is managed by CardManager, not here.
 *
 * Sub-allocations are tracked by (slotIndex, scope) for per-card visibility.
 */
class CardBufferManager {
public:
    using Ptr = std::shared_ptr<CardBufferManager>;

    static Result<Ptr> create(GPUContext* gpuContext,
                              GpuAllocator::Ptr allocator) noexcept;

    virtual ~CardBufferManager() = default;

    // =========================================================================
    // Reservation API (Loop 1: cards declare needs, then commit)
    // =========================================================================
    // Cards call reserve() during declareBufferNeeds phase.
    // gpu-screen calls commitReservations() after all declarations.
    // This ensures the buffer is large enough BEFORE any allocateBuffer() calls,
    // so allocateBuffer() never triggers a resize and .data pointers stay stable.
    virtual void reserve(uint32_t size) = 0;
    virtual Result<void> commitReservations() = 0;

    // =========================================================================
    // Buffer operations — keyed by (slotIndex, scope) for tracking
    // =========================================================================
    // slotIndex = card's metadataSlotIndex() (unique per card)
    // scope = logical name (e.g. "prims", "derived", "storage", "data")
    virtual Result<BufferHandle> allocateBuffer(uint32_t slotIndex,
                                                const std::string& scope,
                                                uint32_t size) = 0;

    virtual Result<void> writeBuffer(BufferHandle handle, const void* data, uint32_t size) = 0;
    virtual Result<void> writeBufferAt(BufferHandle handle, uint32_t offset, const void* data, uint32_t size) = 0;

    // Mark buffer region as dirty (after direct writes to handle.data)
    virtual void markBufferDirty(BufferHandle handle) = 0;

    // =========================================================================
    // GPU upload
    // =========================================================================
    virtual Result<void> flush(WGPUQueue queue) = 0;

    // =========================================================================
    // Accessors
    // =========================================================================
    virtual WGPUBuffer bufferGpu() const = 0;
    virtual uint32_t bufferHighWaterMark() const = 0;

    // Bind group dirty flag (set when GPU buffer is recreated)
    virtual bool bindGroupDirty() const = 0;
    virtual void clearBindGroupDirty() = 0;

    struct Stats {
        uint32_t bufferUsed;
        uint32_t bufferCapacity;
        uint32_t pendingBufferUploads;
    };
    virtual Stats getStats() const = 0;

    // Log per-card sub-allocation breakdown
    virtual void dumpSubAllocations() const = 0;

    // Return per-card sub-allocation breakdown as text
    virtual std::string dumpSubAllocationsToString() const = 0;

protected:
    CardBufferManager() = default;
};

// Backwards compatibility aliases
using StorageHandle = BufferHandle;

}  // namespace yetty
