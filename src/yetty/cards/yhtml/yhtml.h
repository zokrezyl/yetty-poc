#pragma once

#include <yetty/card.h>
#include <yetty/gpu-context.h>
#include <yetty/gpu-allocator.h>
#include <yetty/yetty-context.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include <yetty/msdf-cdb-provider.h>
#include <yetty/font-manager.h>
#include <yetty/base/factory.h>
#include "../hdraw/hdraw.h"
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <unordered_map>

// Forward declare litehtml types
namespace litehtml {
class document;
class context;
}

namespace yetty::card {

class HtmlContainer;
class HttpFetcher;

//=============================================================================
// SDFPrimitive - SDF shape for GPU rendering (96 bytes)
//=============================================================================
struct HtmlSDFPrimitive {
    uint32_t type;           // SDFType
    uint32_t layer;          // Draw order
    float params[12];        // Type-specific parameters
    uint32_t fillColor;      // RGBA packed
    uint32_t strokeColor;    // RGBA packed
    float strokeWidth;
    float round;             // Corner rounding
    float aabbMinX, aabbMinY;
    float aabbMaxX, aabbMaxY;
    uint32_t _pad[2];        // Padding to 96 bytes
};
static_assert(sizeof(HtmlSDFPrimitive) == 96, "HtmlSDFPrimitive must be 96 bytes");

//=============================================================================
// HtmlGlyph - Positioned glyph for GPU rendering (32 bytes)
//=============================================================================
struct HtmlGlyph {
    float x, y;              // Position in scene coordinates
    float width, height;     // Glyph size in scene coordinates
    uint32_t glyphIndex;     // Index in glyphMetadata buffer
    uint32_t color;          // RGBA packed color
    uint32_t layer;          // Draw order
    uint32_t _pad;           // Padding for alignment
};
static_assert(sizeof(HtmlGlyph) == 32, "HtmlGlyph must be 32 bytes");

//=============================================================================
// YHtml - HTML/CSS rendering card with independent rendering pipeline
//
// Parses HTML+CSS via litehtml, renders boxes/borders as SDF primitives
// and text as MSDF glyphs. Uses ypdf-style independent pipeline —
// inherits from Card directly, NOT YDrawBase.
//
// Shader glyph: 0x100009
//=============================================================================
class YHtml : public yetty::Card,
              public base::ObjectFactory<YHtml> {
public:
    using Ptr = std::shared_ptr<YHtml>;

    // Shader glyph codepoint (0x100000 base + 0x0009)
    static constexpr uint32_t SHADER_GLYPH = 0x100009;

    // Grid constants
    static constexpr uint32_t MAX_ENTRIES_PER_CELL = 128;
    static constexpr uint32_t GLYPH_BIT = 0x80000000u;

    static Result<Ptr> createImpl(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    ~YHtml() override;

    //=========================================================================
    // Card interface overrides
    //=========================================================================
    const char* typeName() const override { return "yhtml"; }
    bool needsBuffer() const override { return true; }
    bool needsTexture() const override { return true; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    void declareBufferNeeds() override;
    Result<void> allocateBuffers() override;
    Result<void> allocateTextures() override;
    Result<void> render(float time) override;
    Result<void> dispose() override;
    void suspend() override;

    void setCellSize(uint32_t cellWidth, uint32_t cellHeight) override;

    //=========================================================================
    // EventListener - zoom/pan via scroll
    //=========================================================================
    Result<bool> onEvent(const base::Event& event) override;

    //=========================================================================
    // Public API for HtmlContainer
    //=========================================================================
    void addText(float x, float y, const char* text,
                 float fontSize, uint32_t color, uint32_t layer, int fontId = 0);

    uint32_t addBox(float cx, float cy, float halfW, float halfH,
                    uint32_t fillColor, uint32_t strokeColor,
                    float strokeWidth, float round, uint32_t layer);

    uint32_t addSegment(float x0, float y0, float x1, float y1,
                        uint32_t strokeColor, float strokeWidth,
                        uint32_t layer);

    uint32_t addCircle(float cx, float cy, float radius,
                       uint32_t fillColor, uint32_t strokeColor,
                       float strokeWidth, uint32_t layer);

    void setBgColor(uint32_t color);
    void setSceneBounds(float minX, float minY, float maxX, float maxY);

    MsdfAtlas::Ptr atlas() const { return _atlas; }
    int addFont(const std::string& ttfPath);

    void clear();
    void markDirty() { _dirty = true; _metadataDirty = true; }

    size_t primitiveCount() const { return _primitives.size(); }
    size_t glyphCount() const { return _glyphs.size(); }

    HttpFetcher& fetcher() { return *_fetcher; }
    const HttpFetcher& fetcher() const { return *_fetcher; }

    // Constructor is public for make_shared, use createImpl() factory instead
    YHtml(const YettyContext& ctx,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells,
          const std::string& args, const std::string& payload);

private:
    // Metadata structure (matches ypdf shader layout - 64 bytes)
    // Layout identical to ypdf for consistency, with primitiveCount in flags field
    struct Metadata {
        uint32_t atlasXW;           // 0  [15:0]=atlasX, [31:16]=msdfAtlasWidth
        uint32_t atlasYH;           // 4  [15:0]=atlasY, [31:16]=msdfAtlasHeight
        uint32_t gridOffset;        // 8
        uint32_t gridWidth;         // 12
        uint32_t gridHeight;        // 16
        uint32_t cellSize;          // 20 (f32 stored as bits)
        uint32_t glyphOffset;       // 24 - offset of HtmlGlyph array
        uint32_t glyphCount;        // 28
        uint32_t sceneMinX;         // 32 (f32 stored as bits)
        uint32_t sceneMinY;         // 36 (f32 stored as bits)
        uint32_t sceneMaxX;         // 40 (f32 stored as bits)
        uint32_t sceneMaxY;         // 44 (f32 stored as bits)
        uint32_t widthCells;        // 48 [15:0]=widthCells, [31:16]=panX (i16)
        uint32_t heightCells;       // 52 [15:0]=heightCells, [31:16]=panY (i16)
        uint32_t flags;             // 56 [15:0]=primitiveCount, [31:16]=zoom (f16)
        uint32_t bgColor;           // 60
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    //=========================================================================
    // Initialization
    //=========================================================================
    Result<void> init();
    void parseArgs(const std::string& args);
    Result<void> loadContent();
    Result<void> renderHtml();

    //=========================================================================
    // Primitive helpers
    //=========================================================================
    uint32_t addPrimitive(HtmlSDFPrimitive& prim);
    static void computeAABB(HtmlSDFPrimitive& prim);

    //=========================================================================
    // Grid building and GPU upload
    //=========================================================================
    void computeSceneBounds();
    void buildGrid();
    uint32_t computeDerivedSize() const;
    Result<void> rebuildAndUpload();
    Result<void> uploadMetadata();

    //=========================================================================
    // Event handling helpers
    //=========================================================================
    Result<void> registerForEvents();
    Result<void> deregisterFromEvents();

    //=========================================================================
    // State
    //=========================================================================

    // Args / payload
    std::string _argsStr;
    std::string _payloadStr;

    // HTTP client
    std::shared_ptr<HttpFetcher> _fetcher;

    // Loaded HTML content
    std::string _htmlContent;
    std::string _inputSource;  // URL or file path from --input arg

    // litehtml state
    std::shared_ptr<HtmlContainer> _container;
    std::shared_ptr<litehtml::document> _document;

    // Rendering parameters
    float _fontSize = 16.0f;
    float _viewWidth = 600.0f;  // default until setCellSize provides real value

    // Cell pixel dimensions (set by setCellSize callback)
    uint32_t _cellWidthPx = 0;
    uint32_t _cellHeightPx = 0;
    bool _needsHtmlRender = false;

    // Font / atlas (own instances - NO fontManager dependency)
    GpuAllocator::Ptr _globalAllocator;
    std::string _cacheDir;
    MsdfCdbProvider::Ptr _cdbProvider;
    MsdfAtlas::Ptr _atlas;
    std::unordered_map<std::string, int> _fontIdCache;   // cdbPath -> fontId

    // Atlas texture handle for cardTextureManager
    TextureHandle _atlasTextureHandle = TextureHandle::invalid();

    // Primitive and glyph data
    std::vector<HtmlSDFPrimitive> _primitives;
    std::vector<HtmlGlyph> _glyphs;

    // Grid and derived storage
    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
    float _cellSize = 0.0f;
    uint32_t _gridOffset = 0;
    uint32_t _primitiveOffset = 0;
    uint32_t _glyphOffset = 0;
    uint32_t _glyphMetaOffset = 0;

    // Scene bounds
    float _sceneMinX = 0, _sceneMinY = 0;
    float _sceneMaxX = 100, _sceneMaxY = 100;
    bool _hasExplicitBounds = false;

    // View zoom/pan
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    bool _focused = false;

    // Dirty flags
    bool _dirty = true;
    bool _metadataDirty = true;

    // Visual state
    uint32_t _bgColor = 0xFFFFFFFF;  // White background for HTML
    uint32_t _flags = 0;
};

} // namespace yetty::card
