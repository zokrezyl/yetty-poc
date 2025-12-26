#pragma once

#include <webgpu/webgpu.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <glm/glm.hpp>

namespace yetty {

// Glyph metrics for CPU-side lookups
struct GlyphMetrics {
    // UV coordinates in atlas (normalized 0-1)
    glm::vec2 _uvMin;
    glm::vec2 _uvMax;

    // Glyph metrics in pixels
    glm::vec2 _size;           // Width and height of glyph
    glm::vec2 _bearing;        // Offset from baseline to left/top of glyph
    float _advance;            // Horizontal advance to next character
};

// GPU-friendly glyph metadata for SSBO (40 bytes, 8-byte aligned)
struct GlyphMetadataGPU {
    float _uvMinX, _uvMinY;     // 8 bytes
    float _uvMaxX, _uvMaxY;     // 8 bytes
    float _sizeX, _sizeY;       // 8 bytes
    float _bearingX, _bearingY; // 8 bytes
    float _advance;             // 4 bytes
    float _pad;                 // 4 bytes (alignment)
};

class Font {
public:
    Font();
    ~Font();

#if !YETTY_USE_PREBUILT_ATLAS
    // Generate MSDF atlas from a TTF font file (native only)
    bool generate(const std::string& fontPath, float fontSize, uint32_t atlasSize = 2048);

    // Save atlas to PNG and metrics to JSON
    bool saveAtlas(const std::string& atlasPath, const std::string& metricsPath) const;
#endif

    // Load pre-generated atlas and metrics (works on all platforms)
    bool loadAtlas(const std::string& atlasPath, const std::string& metricsPath);

    // Create WebGPU texture from atlas
    bool createTexture(WGPUDevice device, WGPUQueue queue);

    // Create glyph metadata SSBO buffer
    bool createGlyphMetadataBuffer(WGPUDevice device);

    // Get glyph index for a codepoint (for Grid)
    // On native builds, tries to load missing glyphs from fallback fonts
    uint16_t getGlyphIndex(uint32_t codepoint);

    // Get glyph metrics for a codepoint (for CPU-side calculations)
    const GlyphMetrics* getGlyph(uint32_t codepoint) const;

    // Atlas properties
    uint32_t getAtlasWidth() const { return _atlasWidth; }
    uint32_t getAtlasHeight() const { return _atlasHeight; }
    float getFontSize() const { return _fontSize; }
    float getLineHeight() const { return _lineHeight; }

    WGPUTexture getTexture() const { return _texture; }
    WGPUTextureView getTextureView() const { return _textureView; }
    WGPUSampler getSampler() const { return _sampler; }

    // Glyph metadata buffer for shader access
    WGPUBuffer getGlyphMetadataBuffer() const { return _glyphMetadataBuffer; }
    uint32_t getGlyphCount() const { return static_cast<uint32_t>(_glyphMetadata.size()); }

    // Get pixel range for MSDF shader
    float getPixelRange() const { return _pixelRange; }

#if !YETTY_USE_PREBUILT_ATLAS
    // Request loading of a missing glyph (will use fontconfig for fallback)
    // Returns true if glyph was loaded, false if not available
    bool loadMissingGlyph(uint32_t codepoint);

    // Check if there are pending glyphs that need GPU upload
    bool hasPendingGlyphs() const { return !_pendingGlyphs.empty(); }

    // Upload pending glyphs to GPU (call after loadMissingGlyph)
    bool uploadPendingGlyphs(WGPUDevice device, WGPUQueue queue);
#endif

private:
    // Build the codepoint→index mapping and GPU metadata array
    void buildGlyphIndexMap();

#if !YETTY_USE_PREBUILT_ATLAS
    // Find font files that contain the given codepoint using fontconfig
    // Returns multiple candidates in priority order
    std::vector<std::string> findFontsForCodepoint(uint32_t codepoint);

    // Load a single glyph from a font file into the atlas
    bool loadGlyphFromFont(const std::string& fontPath, uint32_t codepoint);

    // Track which fallback fonts are already loaded
    std::unordered_map<std::string, void*> _fallbackFonts;  // path → FontHandle*

    // Pending glyphs that need to be uploaded to GPU
    std::set<uint32_t> _pendingGlyphs;

    // Track failed codepoints to avoid repeated lookups
    std::set<uint32_t> _failedCodepoints;

    // Shelf packer state for dynamic glyph addition
    int _shelfX = 0;
    int _shelfY = 0;
    int _shelfHeight = 0;
    int _atlasPadding = 2;

    // FreeType handle (kept alive for fallback loading)
    void* _freetypeHandle = nullptr;
    void* _primaryFont = nullptr;
#endif

    std::unordered_map<uint32_t, GlyphMetrics> _glyphs;        // codepoint → metrics
    std::unordered_map<uint32_t, uint16_t> _codepointToIndex;  // codepoint → glyph index
    std::vector<GlyphMetadataGPU> _glyphMetadata;              // GPU metadata array
    std::vector<uint8_t> _atlasData;

    uint32_t _atlasWidth = 0;
    uint32_t _atlasHeight = 0;
    float _fontSize = 0.0f;
    float _lineHeight = 0.0f;
    float _pixelRange = 4.0f;  // MSDF pixel range (higher = better AA quality)

    WGPUTexture _texture = nullptr;
    WGPUTextureView _textureView = nullptr;
    WGPUSampler _sampler = nullptr;
    WGPUBuffer _glyphMetadataBuffer = nullptr;
};

} // namespace yetty
