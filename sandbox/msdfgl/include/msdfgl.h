/**
 * WebGPU implementation for multi-channel signed distance field generator
 * -----------------------------------------------------------------------
 *
 * msdfgl-webgpu       Port to WebGPU
 * msdfgl              Henrik Nyman,    (c) 2019 -
 * msdfgen             Viktor Chlumsky, (c) 2014 - 2019
 *
 * The technique used to generate multi-channel distance fields in this code has
 * been developed by Viktor Chlumsky in 2014 for his master's thesis, "Shape
 * Decomposition for Multi-Channel Distance Fields". It provides improved
 * quality of sharp corners in glyphs and other 2D shapes in comparison to
 * monochrome distance fields. To reconstruct an image of the shape, apply the
 * median of three operation on the triplet of sampled distance field values.
 *
 * This WebGPU port provides a version of that algorithm that runs on the GPU
 * using WebGPU, and a higher-level API handling font atlases and textures.
 */

#ifndef MSDFGL_WEBGPU_H
#define MSDFGL_WEBGPU_H

#include <webgpu/webgpu.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#define MSDFGL_VERSION "1.0.0-webgpu"

namespace msdfgl {

// Forward declarations
class Context;
class Font;
class Atlas;

/**
 * Serialized format of a glyph for rendering.
 */
struct Glyph {
    float x;           // X coordinate in projection coordinates
    float y;           // Y coordinate in projection coordinates
    uint32_t color;    // Color in 0xRRGGBBAA format
    int32_t key;       // Unicode code point or atlas index
    float size;        // Font size for rendering
    float offset;      // Y offset (for subscripts/superscripts)
    float skew;        // Lean amount (positive = right, negative = left)
    float strength;    // Boldness (0.5 = normal, lower = thinner, higher = thicker)
};

/**
 * Index entry stored in the atlas for each glyph.
 */
struct IndexEntry {
    float offsetX;
    float offsetY;
    float sizeX;
    float sizeY;
    float bearingX;
    float bearingY;
    float glyphWidth;
    float glyphHeight;
};

/**
 * Printf-style rendering flags.
 */
enum class PrintfFlags : uint32_t {
    None = 0x00,
    Kerning = 0x01,    // Use FreeType kerning if available
    Vertical = 0x04,   // Draw text vertically
    Utf8 = 0x08,       // Parse text as UTF-8
};

inline PrintfFlags operator|(PrintfFlags a, PrintfFlags b) {
    return static_cast<PrintfFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline bool operator&(PrintfFlags a, PrintfFlags b) {
    return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
}

/**
 * Character map item storing glyph metadata.
 */
struct MapItem {
    uint32_t code;
    int32_t index;
    float advance[2];
};

/**
 * Atlas configuration.
 */
struct AtlasConfig {
    int textureWidth = 0;   // 0 = use max texture size
    int padding = 2;        // Pixels between glyphs
};

/**
 * Font configuration.
 */
struct FontConfig {
    float range = 4.0f;     // MSDF range in pixels
    float scale = 1.0f;     // Scale factor for glyph generation
};

/**
 * Context configuration.
 */
struct ContextConfig {
    float dpiX = 72.0f;
    float dpiY = 72.0f;
};

/**
 * Callback for handling missing glyphs.
 * Return true to retry lookup after callback (e.g., if glyph was generated).
 */
using MissingGlyphCallback = std::function<bool(Font& font, int32_t codepoint)>;

/**
 * Atlas - manages the texture atlas for storing MSDF glyph bitmaps.
 */
class Atlas {
public:
    Atlas(WGPUDevice device, int textureWidth, int padding);
    ~Atlas();

    // Non-copyable
    Atlas(const Atlas&) = delete;
    Atlas& operator=(const Atlas&) = delete;

    // Move semantics
    Atlas(Atlas&& other) noexcept;
    Atlas& operator=(Atlas&& other) noexcept;

    // Getters
    WGPUTexture getAtlasTexture() const { return _atlasTexture; }
    WGPUTextureView getAtlasTextureView() const { return _atlasTextureView; }
    WGPUBuffer getIndexBuffer() const { return _indexBuffer; }
    int getTextureWidth() const { return _textureWidth; }
    int getTextureHeight() const { return _textureHeight; }
    size_t getGlyphCount() const { return _nglyphs; }
    int getPadding() const { return _padding; }

    // Atlas management
    bool allocateGlyph(float width, float height, float& outX, float& outY);
    bool resizeTexture(int newHeight);
    void updateIndexBuffer(const std::vector<IndexEntry>& entries, size_t startIndex);

private:
    friend class Context;
    friend class Font;

    WGPUDevice _device = nullptr;
    WGPUTexture _atlasTexture = nullptr;
    WGPUTextureView _atlasTextureView = nullptr;
    WGPUBuffer _indexBuffer = nullptr;

    int _textureWidth = 0;
    int _textureHeight = 0;
    size_t _nglyphs = 0;
    size_t _nallocated = 0;

    size_t _offsetX = 1;
    size_t _offsetY = 1;
    size_t _yIncrement = 0;

    int _padding = 2;
    int _refcount = 0;
    bool _implicit = false;

    void cleanup();
};

/**
 * Font - manages a loaded font and its glyphs.
 */
class Font {
public:
    Font(Context& ctx, FT_Face face, const FontConfig& config, std::shared_ptr<Atlas> atlas);
    ~Font();

    // Non-copyable
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;

    // Glyph generation
    int generateGlyph(int32_t codepoint);
    int generateGlyphs(int32_t start, int32_t end);
    int generateGlyphList(const std::vector<int32_t>& codepoints);
    int generateAscii() { return generateGlyphs(0, 127); }
    int generateAsciiExt() { return generateGlyphs(0, 255); }

    // Glyph lookup
    MapItem* getGlyph(int32_t codepoint);
    MapItem* getOrAddGlyph(int32_t codepoint);

    // Getters
    FT_Face getFace() const { return _face; }
    float getScale() const { return _scale; }
    float getRange() const { return _range; }
    float getVerticalAdvance(float size) const;
    std::shared_ptr<Atlas> getAtlas() const { return _atlas; }
    Context& getContext() { return _context; }

private:
    friend class Context;

    Context& _context;
    FT_Face _face = nullptr;
    float _scale = 1.0f;
    float _range = 4.0f;
    float _verticalAdvance = 0.0f;

    std::shared_ptr<Atlas> _atlas;

    // Character index map
    std::array<MapItem, 256> _linearMap;
    std::vector<MapItem> _dynamicMap;

    int _directLookupUpperLimit = 0;

    // GPU buffers for glyph generation
    WGPUBuffer _metaInputBuffer = nullptr;
    WGPUBuffer _pointInputBuffer = nullptr;

    void initMap();
    MapItem* mapGet(uint32_t code);
    MapItem* mapInsert(uint32_t code);
    void cleanup();
};

/**
 * Context - main MSDFGL context managing shaders and rendering.
 */
class Context {
public:
    explicit Context(WGPUDevice device, const ContextConfig& config = {});
    ~Context();

    // Non-copyable
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    // Font loading
    std::unique_ptr<Font> loadFont(const std::string& fontPath, const FontConfig& config = {},
                                    std::shared_ptr<Atlas> atlas = nullptr);
    std::unique_ptr<Font> loadFontMem(const void* data, size_t size, const FontConfig& config = {},
                                       std::shared_ptr<Atlas> atlas = nullptr);

    // Atlas creation
    std::shared_ptr<Atlas> createAtlas(const AtlasConfig& config = {});

    // Glyph generation (renders MSDF to atlas)
    void generateGlyphs(Font& font, const std::vector<int32_t>& codepoints);

    // Text rendering
    void render(Font& font, const std::vector<Glyph>& glyphs, const float projection[16],
                WGPUTextureView targetView);

    // Printf-style rendering
    float printf(float x, float y, Font& font, float size, uint32_t color,
                 const float projection[16], WGPUTextureView targetView,
                 PrintfFlags flags, const char* fmt, ...);

    // Geometry calculation (without rendering)
    void geometry(float& x, float& y, Font& font, float size, PrintfFlags flags,
                  const char* fmt, ...);

    // Configuration
    void setDpi(float horizontal, float vertical);
    void setMissingGlyphCallback(MissingGlyphCallback callback);

    // Getters
    WGPUDevice getDevice() const { return _device; }
    FT_Library getFreeType() const { return _ftLibrary; }
    int getMaxTextureSize() const { return _maxTextureSize; }
    float getDpiX() const { return _dpi[0]; }
    float getDpiY() const { return _dpi[1]; }

private:
    friend class Font;
    friend class Atlas;

    WGPUDevice _device = nullptr;
    FT_Library _ftLibrary = nullptr;

    // MSDF generation pipeline
    WGPUShaderModule _genShaderModule = nullptr;
    WGPURenderPipeline _genPipeline = nullptr;
    WGPUBindGroupLayout _genBindGroupLayout = nullptr;

    // Font rendering pipeline
    WGPUShaderModule _renderShaderModule = nullptr;
    WGPURenderPipeline _renderPipeline = nullptr;
    WGPUBindGroupLayout _renderBindGroupLayout = nullptr;
    WGPUSampler _atlasSampler = nullptr;

    // Uniform buffers
    WGPUBuffer _genUniformBuffer = nullptr;
    WGPUBuffer _renderUniformBuffer = nullptr;

    float _dpi[2] = {72.0f, 72.0f};
    int _maxTextureSize = 8192;

    MissingGlyphCallback _missingGlyphCallback;

    bool initShaders();
    bool initPipelines();
    void cleanup();

    // Internal glyph generation
    int generateGlyphsInternal(Font& font, int32_t start, int32_t end,
                               bool useRange, const int32_t* keys, int nkeys);
};

// Utility functions
void ortho(float left, float right, float bottom, float top,
           float nearVal, float farVal, float dest[16]);

uint32_t parseUtf8(const uint8_t* buf, size_t& len);

} // namespace msdfgl

#endif // MSDFGL_WEBGPU_H
