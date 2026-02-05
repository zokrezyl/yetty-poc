#pragma once

#include <yetty/font.h>
#include <yetty/msdf-glyph-data.h>
#include <webgpu/webgpu.h>

#include <string>
#include <unordered_map>
#include <memory>

extern "C" {
struct cdb;
}

namespace yetty {

class MsMsdfFont : public Font {
public:
    using Ptr = std::shared_ptr<MsMsdfFont>;

    // Factory method - calls constructor then init()
    static Result<Ptr> create(const std::string& cdbBasePath);

    ~MsMsdfFont() override;

    // Set fallback CDB for CJK characters (loaded lazily on demand)
    void setFallbackCdb(const std::string& cdbPath);

    // Font interface
    uint32_t getGlyphIndex(uint32_t codepoint) override;
    uint32_t getGlyphIndex(uint32_t codepoint, Style style) override;
    uint32_t getGlyphIndex(uint32_t codepoint, bool bold, bool italic) override;

    // Reverse lookup: glyph index -> Unicode codepoint (0 if unknown)
    uint32_t getCodepoint(uint32_t glyphIndex) const;

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

    // Fallback CDB for CJK characters (loaded lazily)
    CdbFile _fallbackCdb;
    std::string _fallbackCdbPath;
    std::unordered_map<uint32_t, uint32_t> _fallbackCodepointToIndex;

    std::string _cdbBasePath;

    // Runtime atlas (built on demand from CDB)
    std::vector<uint8_t> _atlasData;
    uint32_t _atlasWidth = 4096;
    uint32_t _atlasHeight = 4096;

    // Per-style glyph tracking: codepoint -> glyph index
    std::unordered_map<uint32_t, uint32_t> _codepointToIndex[4];

    // Reverse mapping: glyph index -> codepoint (shared across styles)
    std::unordered_map<uint32_t, uint32_t> _indexToCodepoint;

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
    uint32_t loadGlyphFromCdb(uint32_t codepoint, Style style);

    // Initialize with basic Latin (ASCII 32-126)
    void initBasicLatin();

    // Open CDB file for a variant
    Result<void> openCdb(Style style);

    // Close CDB file
    void closeCdb(Style style);

    // Fallback CDB support
    Result<void> openFallbackCdb();
    void closeFallbackCdb();
    uint32_t loadGlyphFromFallbackCdb(uint32_t codepoint);
    static bool isCJKCodepoint(uint32_t codepoint);

    // Helper to get style index
    static int styleIndex(Style style) { return static_cast<int>(style); }
    static int styleIndex(bool bold, bool italic) {
        return (bold ? 1 : 0) | (italic ? 2 : 0);
    }
};

} // namespace yetty
