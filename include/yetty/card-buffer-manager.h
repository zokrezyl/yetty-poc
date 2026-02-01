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

struct BufferHandle {
    uint8_t* data;      // Raw pointer to CPU buffer - write directly here
    uint32_t offset;    // Byte offset (for metadata references)
    uint32_t size;      // Allocation size in bytes

    bool isValid() const { return size > 0; }
    static BufferHandle invalid() { return {nullptr, 0, 0}; }
};

// Opaque handle for texture atlas regions
// Cards get this from allocateTextureHandle() and link their pixel buffer to it.
// The card never sees atlas positions — the manager assigns them during packAtlas().
struct TextureHandle {
    uint32_t id;

    bool isValid() const { return id != 0; }
    static TextureHandle invalid() { return {0}; }
};

// Atlas position (returned by getAtlasPosition after packAtlas)
struct AtlasPosition {
    uint32_t x;
    uint32_t y;
};

// Configuration for CardBufferManager
struct CardBufferConfig {
    uint32_t metadataPool32Count = 0;    // No 32-byte slots needed currently
    uint32_t metadataPool64Count = 1024; // 64-byte metadata slots (image/texture cards)
    uint32_t metadataPool128Count = 16;
    uint32_t metadataPool256Count = 8;
    uint32_t bufferCapacity = 1 * 1024 * 1024;  // 1MB for data buffers (array<f32>)
};

/**
 * CardBufferManager manages GPU buffers for card metadata, data buffers, and the texture atlas.
 *
 * Two types of GPU data:
 *   - Buffer: linear data (floats, structs) for plot/draw cards
 *   - Texture: RGBA8 pixel data packed into a GPU atlas texture
 *
 * Texture cards use an opaque TextureHandle. They write pixels to their own CPU buffer,
 * link it to the handle, and the manager packs everything into the atlas on packAtlas().
 */
class CardBufferManager {
public:
    using Config = CardBufferConfig;
    using Ptr = std::shared_ptr<CardBufferManager>;

    static Result<Ptr> create(GPUContext* gpuContext, WGPUBuffer uniformBuffer, uint32_t uniformSize, Config config = {}) noexcept;

    virtual ~CardBufferManager() = default;

    // =========================================================================
    // Metadata operations
    // =========================================================================
    virtual Result<MetadataHandle> allocateMetadata(uint32_t size) = 0;
    virtual Result<void> deallocateMetadata(MetadataHandle handle) = 0;
    virtual Result<void> writeMetadata(MetadataHandle handle, const void* data, uint32_t size) = 0;
    virtual Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset, const void* data, uint32_t size) = 0;

    // =========================================================================
    // Buffer operations (linear data: floats, structs for plot/draw cards)
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
    // Texture atlas operations (RGBA8 pixel data)
    // =========================================================================

    // Allocate an opaque texture handle (no atlas position assigned yet)
    virtual Result<TextureHandle> allocateTextureHandle() = 0;

    // Link a card's CPU pixel buffer to its handle (call after rendering pixels)
    virtual Result<void> linkTextureData(TextureHandle handle,
                                          const uint8_t* pixels,
                                          uint32_t width, uint32_t height) = 0;

    // Deallocate a texture handle (frees atlas tracking, not the card's pixel buffer)
    virtual Result<void> deallocateTextureHandle(TextureHandle handle) = 0;

    // Invalidate all texture handles (atlas will be fully repacked)
    virtual void invalidateAllTextureHandles() = 0;

    // Pack atlas: compute optimal layout for all active handles.
    // Must be called after all cards have linked their pixel data.
    virtual Result<void> packAtlas(WGPUQueue queue) = 0;

    // Write one handle's pixels to its assigned atlas region.
    // Must be called after packAtlas().
    virtual Result<void> writeToAtlas(TextureHandle handle, WGPUQueue queue) = 0;

    // Write all packed handles' pixels to the atlas.
    // Convenience method that iterates all active handles.
    virtual Result<void> writeAllToAtlas(WGPUQueue queue) = 0;

    // Get the atlas position assigned by packAtlas (for writing to card metadata)
    virtual AtlasPosition getAtlasPosition(TextureHandle handle) const = 0;

    // =========================================================================
    // GPU upload
    // =========================================================================
    virtual Result<void> flush(WGPUQueue queue) = 0;

    // Image Atlas
    virtual Result<void> initAtlas() = 0;

    // =========================================================================
    // Accessors
    // =========================================================================
    virtual WGPUBuffer metadataBuffer() const = 0;
    virtual WGPUBuffer bufferGpu() const = 0;

    // High water marks for upload optimization
    virtual uint32_t metadataHighWaterMark() const = 0;
    virtual uint32_t bufferHighWaterMark() const = 0;

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
        uint32_t bufferUsed;
        uint32_t bufferCapacity;
        uint32_t atlasTextureCount;
        uint32_t atlasWidth;
        uint32_t atlasHeight;
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
