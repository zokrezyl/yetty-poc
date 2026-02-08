#pragma once

#include <yetty/font.h>
#include <yetty/gpu-allocator.h>
#include <yetty/result.hpp>
#include <webgpu/webgpu.h>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

namespace yetty {

// Bitmap glyph metadata for GPU (matches shader struct)
struct BitmapGlyphMetadata {
    float uvMinX, uvMinY;  // UV coordinates in atlas
    float uvMaxX, uvMaxY;
    float width, height;   // Glyph size in pixels (at atlas resolution)
    float _pad1, _pad2;    // Padding for alignment
};

/**
 * BmFont manages a color (RGBA) texture atlas for bitmap font rendering.
 * This includes emojis and any other color bitmap glyphs.
 *
 * Implements the Font interface for unified font handling.
 */
class BmFont : public Font {
public:
    using Ptr = std::shared_ptr<BmFont>;

    // Factory method
    static Result<Ptr> create(WGPUDevice device, GpuAllocator::Ptr allocator,
                              const std::string& fontPath = "",
                              uint32_t glyphSize = 64) noexcept;

    ~BmFont() override;

    // Non-copyable
    BmFont(const BmFont&) = delete;
    BmFont& operator=(const BmFont&) = delete;

    // =========================================================================
    // Font interface implementation
    // =========================================================================

    uint32_t getGlyphIndex(uint32_t codepoint) override;
    uint32_t getGlyphIndex(uint32_t codepoint, Style style) override;
    uint32_t getGlyphIndex(uint32_t codepoint, bool bold, bool italic) override;

    void uploadToGpu() override;
    bool isDirty() const override { return _needsUpload; }
    void clearDirty() override { _needsUpload = false; }

    uint32_t getAtlasWidth() const override { return _atlasSize; }
    uint32_t getAtlasHeight() const override { return _atlasSize; }
    const std::vector<uint8_t>& getAtlasData() const override { return _atlasData; }

    // =========================================================================
    // Bitmap font specific methods
    // =========================================================================

    // Load common glyphs (emojis)
    Result<void> loadCommonGlyphs() noexcept;

    // Load a specific glyph (for lazy loading)
    // Returns glyph index, or -1 if not found
    Result<int> loadGlyph(uint32_t codepoint) noexcept;

    // Check if a glyph is loaded
    bool hasGlyph(uint32_t codepoint) const noexcept;

    // GPU resources
    WGPUTexture getTexture() const noexcept { return _texture; }
    WGPUTextureView getTextureView() const noexcept { return _textureView; }
    WGPUSampler getSampler() const noexcept { return _sampler; }
    WGPUBuffer getMetadataBuffer() const noexcept { return _metadataBuffer; }

    // Atlas info
    uint32_t getGlyphSize() const noexcept { return _glyphSize; }
    uint32_t getGlyphCount() const noexcept { return static_cast<uint32_t>(_glyphMetadata.size()); }

protected:
    Result<void> init() override;

private:
    BmFont(WGPUDevice device, GpuAllocator::Ptr allocator, const std::string& fontPath,
           uint32_t glyphSize) noexcept;

    Result<void> findFont() noexcept;
    Result<void> createGPUResources() noexcept;
    Result<void> renderGlyph(uint32_t codepoint, int atlasX, int atlasY) noexcept;

    WGPUDevice _device = nullptr;
    GpuAllocator::Ptr _allocator;

    // FreeType handles
    void* _ftLibrary = nullptr;
    void* _ftFace = nullptr;
    std::string _fontPath;

    // Atlas data
    uint32_t _glyphSize = 64;       // Size of each glyph cell
    uint32_t _atlasSize = 2048;     // Atlas texture size
    uint32_t _glyphsPerRow = 32;    // atlasSize / glyphSize
    std::vector<uint8_t> _atlasData; // RGBA pixel data

    // Metadata
    std::vector<BitmapGlyphMetadata> _glyphMetadata;
    std::unordered_map<uint32_t, int> _codepointToIndex; // codepoint -> metadata index

    // Shelf packer state
    int _nextGlyphX = 0;
    int _nextGlyphY = 0;

    // GPU resources
    WGPUTexture _texture = nullptr;
    WGPUTextureView _textureView = nullptr;
    WGPUSampler _sampler = nullptr;
    WGPUBuffer _metadataBuffer = nullptr;
    bool _gpuResourcesCreated = false;
    bool _needsUpload = false;
};

} // namespace yetty
