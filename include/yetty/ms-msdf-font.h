#pragma once

#include <yetty/yetty-font.h>
#include <yetty/font.h>  // For GlyphMetadataGPU
#include <yetty/msdf-glyph-data.h>
#include <webgpu/webgpu.h>

#include <string>
#include <unordered_map>
#include <memory>

extern "C" {
struct cdb;
}

namespace yetty {

class MsMsdfFont : public YettyFont {
public:
    // Factory method - calls constructor then init()
    static Result<std::shared_ptr<MsMsdfFont>> create(const std::string& cdbBasePath);

    ~MsMsdfFont() override;

    // YettyFont interface
    uint16_t getGlyphIndex(uint32_t codepoint) override;
    uint16_t getGlyphIndex(uint32_t codepoint, Style style) override;
    uint16_t getGlyphIndex(uint32_t codepoint, bool bold, bool italic) override;

    void uploadToGpu() override;
    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }

    uint32_t getAtlasWidth() const override { return _atlasWidth; }
    uint32_t getAtlasHeight() const override { return _atlasHeight; }
    const std::vector<uint8_t>& getAtlasData() const override { return _atlasData; }

    // WebGPU resource creation
    Result<void> createTexture(WGPUDevice device, WGPUQueue queue);
    Result<void> createGlyphMetadataBuffer(WGPUDevice device);

    // WebGPU resource accessors (compatible with Font interface)
    WGPUTexture getTexture() const { return _texture; }
    WGPUTextureView getTextureView() const { return _textureView; }
    WGPUSampler getSampler() const { return _sampler; }
    WGPUBuffer getGlyphMetadataBuffer() const { return _glyphMetadataBuffer; }

    // Font metrics (compatible with Font interface)
    float getFontSize() const { return _fontSize; }
    float getLineHeight() const { return _lineHeight; }
    float getPixelRange() const { return _pixelRange; }
    uint32_t getGlyphCount() const { return static_cast<uint32_t>(_glyphMetadata.size()); }
    uint32_t getBufferGlyphCount() const { return _bufferGlyphCount; }
    uint32_t getResourceVersion() const { return _resourceVersion; }

    // Additional accessors
    const std::vector<GlyphMetadataGPU>& getGlyphMetadata() const { return _glyphMetadata; }

    // Upload pending changes to GPU
    Result<void> uploadPendingGlyphs(WGPUDevice device, WGPUQueue queue);
    bool hasPendingGlyphs() const { return _dirty; }

private:
    explicit MsMsdfFont(const std::string& cdbBasePath);
    Result<void> init() override;

    // CDB file handle for a variant
    struct CdbFile {
        int fd = -1;
        void* mapped = nullptr;
        size_t size = 0;
        struct cdb* cdb = nullptr;
    };

    // One CDB per style variant
    CdbFile _cdbFiles[4];  // Regular, Bold, Italic, BoldItalic

    std::string _cdbBasePath;

    // Runtime atlas (built on demand from CDB)
    std::vector<uint8_t> _atlasData;
    uint32_t _atlasWidth = 2048;
    uint32_t _atlasHeight = 512;

    // Per-style glyph tracking: codepoint -> glyph index
    std::unordered_map<uint32_t, uint16_t> _codepointToIndex[4];

    // GPU metadata for all loaded glyphs
    std::vector<GlyphMetadataGPU> _glyphMetadata;

    bool _dirty = false;

    // Font metrics
    float _fontSize = 32.0f;
    float _lineHeight = 40.0f;
    float _pixelRange = 4.0f;

    // WebGPU resources
    WGPUTexture _texture = nullptr;
    WGPUTextureView _textureView = nullptr;
    WGPUSampler _sampler = nullptr;
    WGPUBuffer _glyphMetadataBuffer = nullptr;
    uint32_t _bufferGlyphCount = 0;
    uint32_t _resourceVersion = 0;

    // Shelf packer state for atlas
    uint32_t _shelfX = 0;
    uint32_t _shelfY = 0;
    uint32_t _shelfHeight = 0;
    static constexpr uint32_t ATLAS_PADDING = 2;

    // Load glyph from CDB into atlas
    // Returns glyph index, or 0 (placeholder) if not found
    uint16_t loadGlyphFromCdb(uint32_t codepoint, Style style);

    // Initialize with basic Latin (ASCII 32-126)
    void initBasicLatin();

    // Open CDB file for a variant
    Result<void> openCdb(Style style);

    // Close CDB file
    void closeCdb(Style style);

    // Helper to get style index
    static int styleIndex(Style style) { return static_cast<int>(style); }
    static int styleIndex(bool bold, bool italic) {
        return (bold ? 1 : 0) | (italic ? 2 : 0);
    }
};

} // namespace yetty
