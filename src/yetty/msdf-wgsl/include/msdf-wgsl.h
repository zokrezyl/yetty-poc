#pragma once

/**
 * MSDF-WGSL: Compute shader-based MSDF generation for WebGPU
 */

#include <webgpu/webgpu.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <vector>
#include <string>
#include <cstdint>

namespace msdf {

// Forward declarations
class Context;
class Font;
class Atlas;

// Glyph data for CDB export
struct GlyphData {
    uint32_t codepoint;
    uint16_t atlasX, atlasY;      // Position in atlas (pixels)
    uint16_t atlasW, atlasH;      // Size in atlas (pixels)
    int16_t bearingX, bearingY;   // Bearing in font units
    uint16_t glyphWidth, glyphHeight; // Glyph size in font units
    float advance;                // Horizontal advance
};

// Configuration for font loading
struct FontConfig {
    float scale = 2.0f;    // Scale factor for MSDF generation
    float range = 4.0f;    // Distance field range in pixels
};

// Atlas - stores generated MSDF textures
class Atlas {
public:
    Atlas(WGPUDevice device, int width);
    ~Atlas();

    Atlas(const Atlas&) = delete;
    Atlas& operator=(const Atlas&) = delete;
    Atlas(Atlas&& other) noexcept;
    Atlas& operator=(Atlas&& other) noexcept;

    // Allocate space for a glyph, returns position
    bool allocate(int width, int height, int& outX, int& outY);

    // Resize texture height (grows as needed)
    bool resize(int newHeight);

    // Getters
    WGPUTexture getTexture() const { return _texture; }
    WGPUTextureView getTextureView() const { return _textureView; }
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    size_t getGlyphCount() const { return _glyphCount; }

private:
    friend class Context;

    WGPUDevice _device = nullptr;
    WGPUTexture _texture = nullptr;
    WGPUTextureView _textureView = nullptr;

    int _width = 0;
    int _height = 0;
    int _rowHeight = 0;
    int _cursorX = 1;
    int _cursorY = 1;
    size_t _glyphCount = 0;

    void cleanup();
};

// Font - represents a loaded font with its atlas
class Font {
public:
    ~Font();

    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&& other) noexcept;
    Font& operator=(Font&& other) noexcept;

    // Get generated glyph data
    const std::vector<GlyphData>& getGlyphs() const { return _glyphs; }

    // Get atlas
    Atlas* getAtlas() { return _atlas.get(); }
    const Atlas* getAtlas() const { return _atlas.get(); }

    // Font properties
    float getScale() const { return _scale; }
    float getRange() const { return _range; }
    FT_Face getFace() const { return _face; }

    // Get all codepoints present in the font
    std::vector<uint32_t> getAllCodepoints() const;

private:
    friend class Context;

    Font(Context& ctx, FT_Face face, const FontConfig& config);

    Context& _context;
    FT_Face _face = nullptr;
    float _scale = 2.0f;
    float _range = 4.0f;

    std::unique_ptr<Atlas> _atlas;
    std::vector<GlyphData> _glyphs;

    // GPU buffers for glyph data
    WGPUBuffer _metadataBuffer = nullptr;
    WGPUBuffer _pointsBuffer = nullptr;

    void cleanup();
};

// Context - main entry point for MSDF generation
class Context {
public:
    Context(WGPUDevice device, WGPUInstance instance = nullptr);
    ~Context();

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    // Load a font
    std::unique_ptr<Font> loadFont(const std::string& path, const FontConfig& config = {});

    // Generate MSDF for a list of codepoints
    int generateGlyphs(Font& font, const std::vector<uint32_t>& codepoints);

    // Read atlas texture back to CPU (for CDB export)
    std::vector<uint8_t> readAtlasToRGBA8(const Atlas& atlas);

    // Get device
    WGPUDevice getDevice() const { return _device; }

private:
    WGPUDevice _device = nullptr;
    WGPUInstance _instance = nullptr;
    FT_Library _ftLibrary = nullptr;

    // Compute pipeline resources
    WGPUShaderModule _computeShader = nullptr;
    WGPUComputePipeline _computePipeline = nullptr;
    WGPUBindGroupLayout _bindGroupLayout = nullptr;

    bool initCompute();
    void cleanup();
};

// Utility: Get default charset for CDB generation
std::vector<uint32_t> getDefaultCharset(bool includeNerdFonts = true);

// Utility: Set shader directory path
void setShaderPath(const std::string& path);

} // namespace msdf
