#pragma once

#include <yetty/result.hpp>
#include <yetty/gpu-context.h>
#include <yetty/card-texture-manager.h>
#include <webgpu/webgpu.h>
#include <cstdint>
#include <memory>

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

// Configuration for CardBufferManager
struct CardBufferConfig {
    uint32_t metadataPool32Count = 0;    // No 32-byte slots needed currently
    uint32_t metadataPool64Count = 1024; // 64-byte metadata slots
    uint32_t metadataPool128Count = 16;
    uint32_t metadataPool256Count = 8;
    uint32_t bufferCapacity = 1 * 1024 * 1024;  // 1MB for buffer (array<f32>) - plot/draw
};

/**
 * CardBufferManager manages GPU buffers for card metadata and linear data buffers.
 *
 * Buffer cards (plot, ydraw, hdraw, kdraw) use this for float arrays and structs.
 * Texture cards use CardTextureManager separately for the atlas.
 */
class CardBufferManager {
public:
    using Config = CardBufferConfig;
    using Ptr = std::shared_ptr<CardBufferManager>;

    static Result<Ptr> create(GPUContext* gpuContext,
                              CardTextureManager::Ptr textureMgr,
                              WGPUBuffer uniformBuffer,
                              uint32_t uniformSize,
                              Config config = {}) noexcept;

    virtual ~CardBufferManager() = default;

    // =========================================================================
    // Metadata operations
    // =========================================================================
    virtual Result<MetadataHandle> allocateMetadata(uint32_t size) = 0;
    virtual Result<void> deallocateMetadata(MetadataHandle handle) = 0;
    virtual Result<void> writeMetadata(MetadataHandle handle, const void* data, uint32_t size) = 0;
    virtual Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset, const void* data, uint32_t size) = 0;

    // =========================================================================
    // Buffer operations (linear data: floats for plot/draw cards) - binding 2
    // =========================================================================
    virtual Result<BufferHandle> allocateBuffer(uint32_t size) = 0;
    virtual Result<void> deallocateBuffer(BufferHandle handle) = 0;
    virtual Result<void> writeBuffer(BufferHandle handle, const void* data, uint32_t size) = 0;
    virtual Result<void> writeBufferAt(BufferHandle handle, uint32_t offset, const void* data, uint32_t size) = 0;

    // Allocate buffer and write offset to metadata field
    virtual Result<BufferHandle> allocateBufferAndLink(MetadataHandle metaHandle,
                                                       uint32_t metaFieldOffset,
                                                       uint32_t bufferSize) = 0;

    // Mark buffer region as dirty (after direct writes to handle.data)
    virtual void markBufferDirty(BufferHandle handle) = 0;

    // =========================================================================
    // GPU upload
    // =========================================================================
    virtual Result<void> flush(WGPUQueue queue) = 0;

    // =========================================================================
    // Accessors
    // =========================================================================
    virtual WGPUBuffer metadataBuffer() const = 0;
    virtual WGPUBuffer bufferGpu() const = 0;

    // High water marks for upload optimization
    virtual uint32_t metadataHighWaterMark() const = 0;
    virtual uint32_t bufferHighWaterMark() const = 0;

    // Bind group dirty flag (set when GPU buffers are recreated)
    virtual bool bindGroupDirty() const = 0;
    virtual void clearBindGroupDirty() = 0;

    // Access to texture manager
    virtual CardTextureManager::Ptr textureManager() const = 0;

    struct Stats {
        uint32_t metadataUsed;
        uint32_t metadataCapacity;
        uint32_t bufferUsed;
        uint32_t bufferCapacity;
        uint32_t pendingMetadataUploads;
        uint32_t pendingBufferUploads;
    };
    virtual Stats getStats() const = 0;

protected:
    CardBufferManager() = default;
};

// Backwards compatibility aliases (TODO: remove after full migration)
using StorageHandle = BufferHandle;
using TextureDataHandle = BufferHandle;

}  // namespace yetty
