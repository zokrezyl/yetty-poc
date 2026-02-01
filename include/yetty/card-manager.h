#pragma once

#include <yetty/result.hpp>
#include <yetty/gpu-context.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/card-texture-manager.h>
#include <webgpu/webgpu.h>
#include <memory>

namespace yetty {

/**
 * CardManager - Coordinates CardBufferManager and CardTextureManager.
 *
 * Creates both managers (as shared_ptr), creates the combined bind group
 * that includes resources from both, and handles GPU upload coordination.
 */
class CardManager {
public:
    using Ptr = std::shared_ptr<CardManager>;

    struct Config {
        CardBufferConfig buffer;
        CardTextureConfig texture;
    };

    static Result<Ptr> create(GPUContext* gpuContext,
                              WGPUBuffer uniformBuffer,
                              uint32_t uniformSize,
                              Config config = {}) noexcept;

    virtual ~CardManager() = default;

    // =========================================================================
    // Manager accessors
    // =========================================================================
    virtual CardBufferManager::Ptr bufferManager() const = 0;
    virtual CardTextureManager::Ptr textureManager() const = 0;

    // =========================================================================
    // Bind group (combines resources from both managers)
    // =========================================================================
    virtual WGPUBindGroupLayout sharedBindGroupLayout() const = 0;
    virtual WGPUBindGroup sharedBindGroup() const = 0;

    // Recreate bind group (call after atlas initialization or buffer reallocation)
    virtual Result<void> updateBindGroup() = 0;

    // =========================================================================
    // GPU upload coordination
    // =========================================================================

    // Pack atlas and flush all dirty data to GPU
    virtual Result<void> flush(WGPUQueue queue) = 0;

protected:
    CardManager() = default;
};

}  // namespace yetty
