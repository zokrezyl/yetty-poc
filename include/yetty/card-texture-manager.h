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
struct TextureHandle {
    uint32_t id;

    bool isValid() const { return id != 0; }
    static TextureHandle invalid() { return {0}; }
};

// Atlas position (returned by getAtlasPosition after createAtlas)
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
 * Protocol:
 *   1. Cards call allocate(w, h) to declare their texture size → TextureHandle
 *   2. gpu-screen calls createAtlas() when texture cards enter/exit (Loop 3)
 *   3. Cards call write(handle, pixels) to push pixel data into the atlas
 *   4. CardManager calls uploadAtlas(queue) during flush to upload to GPU
 */
class CardTextureManager {
public:
    using Config = CardTextureConfig;
    using Ptr = std::shared_ptr<CardTextureManager>;

    static Result<Ptr> create(GPUContext* gpuContext, Config config = {}) noexcept;

    virtual ~CardTextureManager() = default;

    // =========================================================================
    // Card API — called by individual cards
    // =========================================================================

    // Allocate a texture handle with declared size (RGBA8, width * height * 4 bytes)
    virtual Result<TextureHandle> allocate(uint32_t width, uint32_t height) = 0;

    // Deallocate a texture handle
    virtual Result<void> deallocate(TextureHandle handle) = 0;

    // Write pixels into the atlas region for this handle (RGBA8, width * height * 4 bytes)
    virtual Result<void> write(TextureHandle handle, const uint8_t* pixels) = 0;

    // Get the atlas position assigned by createAtlas (for card metadata)
    virtual AtlasPosition getAtlasPosition(TextureHandle handle) const = 0;

    // =========================================================================
    // gpu-screen API — called by gpu-screen / CardManager
    // =========================================================================

    // Pack atlas layout from all allocated handles. Called by gpu-screen in Loop 3
    // when a texture card enters or exits.
    virtual Result<void> createAtlas() = 0;

    // Upload dirty atlas regions to GPU. Called by CardManager::flush().
    virtual Result<void> uploadAtlas(WGPUQueue queue) = 0;

    // =========================================================================
    // Accessors
    // =========================================================================
    virtual WGPUTexture atlasTexture() const = 0;
    virtual WGPUTextureView atlasTextureView() const = 0;
    virtual WGPUSampler atlasSampler() const = 0;
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
