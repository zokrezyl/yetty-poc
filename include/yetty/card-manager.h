#pragma once

#include <yetty/result.hpp>
#include <yetty/gpu-context.h>
#include <yetty/gpu-allocator.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/card-texture-manager.h>
#include <webgpu/webgpu.h>
#include <memory>
#include <optional>

namespace yetty {

// Metadata config (moved from CardBufferConfig)
struct MetadataConfig {
    uint32_t pool32Count = 0;
    uint32_t pool64Count = 1024;
    uint32_t pool128Count = 16;
    uint32_t pool256Count = 8;
};

/**
 * CardManager - Coordinates CardBufferManager and CardTextureManager.
 *
 * Owns the metadata GPU buffer (binding 1) and shared bind group.
 * CardBufferManager owns the linear storage buffer (binding 2).
 * CardTextureManager owns the atlas texture (binding 3,4) and texture buffer (binding 5).
 */
class CardManager {
public:
    using Ptr = std::shared_ptr<CardManager>;

    struct Config {
        MetadataConfig metadata;
        CardTextureConfig texture;
    };

    static Result<Ptr> create(GPUContext* gpuContext,
                              GpuAllocator::Ptr allocator,
                              WGPUBuffer uniformBuffer,
                              uint32_t uniformSize,
                              Config config = {}) noexcept;

    virtual ~CardManager() = default;

    // =========================================================================
    // Metadata operations (owned by CardManager)
    // =========================================================================
    virtual Result<MetadataHandle> allocateMetadata(uint32_t size) = 0;
    virtual Result<void> deallocateMetadata(MetadataHandle handle) = 0;
    virtual Result<void> writeMetadata(MetadataHandle handle, const void* data, uint32_t size) = 0;
    virtual Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset, const void* data, uint32_t size) = 0;

    // =========================================================================
    // Manager accessors
    // =========================================================================
    virtual CardBufferManager::Ptr bufferManager() const = 0;
    virtual CardTextureManager::Ptr textureManager() const = 0;

    // =========================================================================
    // GPU resources
    // =========================================================================
    virtual WGPUBuffer metadataBuffer() const = 0;
    virtual WGPUBindGroupLayout sharedBindGroupLayout() const = 0;
    virtual WGPUBindGroup sharedBindGroup() const = 0;

    // Recreate bind group (call after atlas initialization or buffer reallocation)
    virtual Result<void> updateBindGroup() = 0;

    // Force bind group recreation on next updateBindGroup() call
    virtual void invalidateBindGroup() = 0;

    // =========================================================================
    // GPU upload coordination
    // =========================================================================

    // Flush all dirty data to GPU (metadata + buffer + atlas)
    virtual Result<void> flush(WGPUQueue queue) = 0;

    // =========================================================================
    // Named card registry (for RPC streaming)
    // =========================================================================
    virtual void registerNamedCard(const std::string& name, uint32_t slotIndex) = 0;
    virtual void unregisterNamedCard(const std::string& name) = 0;
    virtual std::optional<uint32_t> getSlotIndexByName(const std::string& name) const = 0;
    virtual std::string getNameBySlotIndex(uint32_t slotIndex) const = 0;  // returns "" if not found

protected:
    CardManager() = default;
};

}  // namespace yetty
