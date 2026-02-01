#pragma once

#include <yetty/result.hpp>
#include <yetty/gpu-context.h>
#include <webgpu/webgpu.h>
#include <cstdint>
#include <memory>

namespace yetty {

// Constants for image atlas
constexpr uint32_t IMAGE_ATLAS_SIZE = 2048;       // 2048x2048 = 16MB RGBA8

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

// Configuration for CardTextureManager
struct CardTextureConfig {
    uint32_t initialAtlasSize = IMAGE_ATLAS_SIZE;  // 2048x2048
    uint32_t maxAtlasSize = 8192;                  // Max atlas dimension
};

/**
 * CardTextureManager manages the texture atlas for cards.
 *
 * Texture cards use an opaque TextureHandle. They write pixels to their own CPU buffer,
 * link it to the handle, and the manager packs everything into the atlas on packAtlas().
 */
class CardTextureManager {
public:
    using Config = CardTextureConfig;
    using Ptr = std::shared_ptr<CardTextureManager>;

    static Result<Ptr> create(GPUContext* gpuContext, Config config = {}) noexcept;

    virtual ~CardTextureManager() = default;

    // =========================================================================
    // Texture atlas operations
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
    // Atlas initialization
    // =========================================================================
    virtual Result<void> initAtlas() = 0;

    // =========================================================================
    // Accessors
    // =========================================================================
    virtual WGPUTexture atlasTexture() const = 0;
    virtual WGPUTextureView atlasTextureView() const = 0;
    virtual WGPUSampler atlasSampler() const = 0;
    virtual WGPUBuffer textureBuffer() const = 0;  // binding 5: raw pixel data
    virtual bool atlasInitialized() const = 0;

    // Current atlas size
    virtual uint32_t atlasSize() const = 0;

    struct Stats {
        uint32_t textureCount;
        uint32_t atlasWidth;
        uint32_t atlasHeight;
        uint32_t usedPixels;
    };
    virtual Stats getStats() const = 0;

protected:
    CardTextureManager() = default;
};

}  // namespace yetty
