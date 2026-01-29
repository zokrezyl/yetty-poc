#pragma once

#include <yetty/result.hpp>
#include <yetty/gpu-context.h>
#include <webgpu/webgpu.h>
#include <cstdint>
#include <memory>

namespace yetty {

// Constants for image atlas
constexpr uint32_t IMAGE_ATLAS_SIZE = 2048;       // 2048x2048 = 16MB RGBA8
constexpr uint32_t MAX_CARD_SLOTS = 16384;        // Max unique card slots to track

// Handle types for type safety
struct MetadataHandle {
    uint32_t offset;
    uint32_t size;

    bool isValid() const { return size > 0; }
    static MetadataHandle invalid() { return {0, 0}; }
};

struct StorageHandle {
    uint8_t* data;      // Raw pointer to CPU buffer - write directly here
    uint32_t offset;    // Byte offset (for metadata references)
    uint32_t size;      // Allocation size in bytes

    bool isValid() const { return data != nullptr && size > 0; }
    static StorageHandle invalid() { return {nullptr, 0, 0}; }
};

// Alias for texture data (same structure as StorageHandle)
using TextureDataHandle = StorageHandle;

// Configuration for CardBufferManager
struct CardBufferConfig {
    uint32_t metadataPool32Count = 0;    // No 32-byte slots needed currently
    uint32_t metadataPool64Count = 256;  // 64-byte metadata slots (image/texture cards)
    uint32_t metadataPool128Count = 16;
    uint32_t metadataPool256Count = 8;
    uint32_t storageCapacity = 1 * 1024 * 1024;        // 1MB for plot data (array<f32>)
    uint32_t textureDataCapacity = 64 * 1024 * 1024;   // 64MB for scaled image pixels
};

/**
 * CardBufferManager manages GPU buffers for card metadata, storage, and images.
 *
 * Stored in YettyContext for access by CardFactory and cards.
 */
class CardBufferManager {
public:
    using Config = CardBufferConfig;
    using Ptr = std::shared_ptr<CardBufferManager>;

    // gpuContext: CardBufferManager will update sharedBindGroup/Layout when resources change
    // uniformBuffer: shared uniform buffer for bind group creation
    static Result<Ptr> create(GPUContext* gpuContext, WGPUBuffer uniformBuffer, uint32_t uniformSize, Config config = {}) noexcept;

    virtual ~CardBufferManager() = default;

    // Metadata operations
    virtual Result<MetadataHandle> allocateMetadata(uint32_t size) = 0;
    virtual Result<void> deallocateMetadata(MetadataHandle handle) = 0;
    virtual Result<void> writeMetadata(MetadataHandle handle, const void* data, uint32_t size) = 0;
    virtual Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset, const void* data, uint32_t size) = 0;

    // Storage operations (for float data like plot points)
    virtual Result<StorageHandle> allocateStorage(uint32_t size) = 0;
    virtual Result<void> deallocateStorage(StorageHandle handle) = 0;
    virtual Result<void> writeStorage(StorageHandle handle, const void* data, uint32_t size) = 0;
    virtual Result<void> writeStorageAt(StorageHandle handle, uint32_t offset, const void* data, uint32_t size) = 0;

    // Allocate storage and write offset to metadata field
    virtual Result<StorageHandle> allocateStorageAndLink(MetadataHandle metaHandle,
                                                          uint32_t metaFieldOffset,
                                                          uint32_t storageSize) = 0;

    // Texture data operations (for scaled RGBA8 pixel data)
    // Cards allocate space, write directly to handle.data, then call markTextureDataDirty
    virtual Result<TextureDataHandle> allocateTextureData(uint32_t size) = 0;
    virtual Result<void> deallocateTextureData(TextureDataHandle handle) = 0;
    virtual void markTextureDataDirty(TextureDataHandle handle) = 0;

    // GPU upload
    virtual Result<void> flush(WGPUQueue queue) = 0;

    // Image Atlas
    virtual Result<void> initAtlas() = 0;

    // Accessors
    virtual WGPUBuffer metadataBuffer() const = 0;
    virtual WGPUBuffer storageBuffer() const = 0;
    virtual WGPUBuffer textureDataBuffer() const = 0;

    // High water marks for upload optimization
    virtual uint32_t metadataHighWaterMark() const = 0;
    virtual uint32_t storageHighWaterMark() const = 0;
    virtual uint32_t textureDataHighWaterMark() const = 0;

    // Atlas accessors
    virtual WGPUTexture atlasTexture() const = 0;
    virtual WGPUTextureView atlasTextureView() const = 0;
    virtual WGPUSampler atlasSampler() const = 0;
    virtual bool atlasInitialized() const = 0;

    // Accessors for bind group (managed internally, auto-recreated on resource changes)
    virtual WGPUBindGroupLayout sharedBindGroupLayout() const = 0;
    virtual WGPUBindGroup sharedBindGroup() const = 0;

    struct Stats {
        uint32_t metadataUsed;
        uint32_t metadataCapacity;
        uint32_t storageUsed;
        uint32_t storageCapacity;
        uint32_t textureDataUsed;
        uint32_t textureDataCapacity;
        uint32_t pendingMetadataUploads;
        uint32_t pendingStorageUploads;
    };
    virtual Stats getStats() const = 0;

protected:
    CardBufferManager() = default;
};

}  // namespace yetty
