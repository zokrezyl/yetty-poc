#pragma once

#include <yetty/card.h>
#include <yetty/gpu-context.h>
#include <yetty/gpu-allocator.h>
#include <yetty/yetty-context.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include <yetty/msdf-cdb-provider.h>
#include <yetty/font-manager.h>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <filesystem>

// Forward declarations for pdfio (pdfio uses _pdfio_*_s naming)
struct _pdfio_file_s;
typedef struct _pdfio_file_s pdfio_file_t;
struct _pdfio_obj_s;
typedef struct _pdfio_obj_s pdfio_obj_t;

namespace yetty::card {

//=============================================================================
// PdfGlyph - Positioned glyph for GPU rendering (32 bytes)
//
// Independent definition, same binary layout as YDrawGlyph for shader compat.
//=============================================================================
struct PdfGlyph {
    float x, y;              // Position in scene coordinates
    float width, height;     // Glyph size in scene coordinates
    uint32_t glyphIndex;     // Index in glyphMetadata buffer
    uint32_t color;          // RGBA packed color
    uint32_t layer;          // Draw order
    uint32_t _pad;           // Padding for alignment
};
static_assert(sizeof(PdfGlyph) == 32, "PdfGlyph must be 32 bytes");

//=============================================================================
// YPdf - PDF rendering card with own independent rendering pipeline
//
// Parses PDF content streams, extracts text using CDB glyph advances
// for correct inter-character spacing, and renders via its own spatial
// hash grid + MSDF shader (0x100008).
//
// Does NOT use YDrawBase. Inherits directly from Card.
//=============================================================================
class YPdf : public yetty::Card {
public:
    using Ptr = std::shared_ptr<YPdf>;

    // Shader glyph codepoint (0x100000 base + 0x0008)
    static constexpr uint32_t SHADER_GLYPH = 0x100008;

    // Grid constants
    static constexpr uint32_t MAX_ENTRIES_PER_CELL = 128;
    static constexpr uint32_t GLYPH_BIT = 0x80000000u;

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    ~YPdf() override;

    //=========================================================================
    // Card interface overrides
    //=========================================================================
    const char* typeName() const override { return "ypdf"; }
    bool needsBuffer() const override { return true; }
    bool needsTexture() const override { return true; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    void declareBufferNeeds() override;
    Result<void> allocateBuffers() override;
    Result<void> allocateTextures() override;
    Result<void> writeTextures() override;
    Result<void> render(float time) override;
    Result<void> dispose() override;
    void suspend() override;

    //=========================================================================
    // EventListener - zoom/pan via mouse wheel
    //=========================================================================
    Result<bool> onEvent(const base::Event& event) override;

    // Constructor is public for make_shared, use create() factory instead
    YPdf(const YettyContext& ctx,
         int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells,
         const std::string& args, const std::string& payload);

private:
    // Metadata structure (matches shader layout - 64 bytes)
    struct Metadata {
        uint32_t atlasXW;           // 0  [15:0]=atlasX, [31:16]=msdfAtlasWidth
        uint32_t atlasYH;           // 4  [15:0]=atlasY, [31:16]=msdfAtlasHeight
        uint32_t gridOffset;        // 8
        uint32_t gridWidth;         // 12
        uint32_t gridHeight;        // 16
        uint32_t cellSize;          // 20 (f32 stored as bits)
        uint32_t glyphOffset;       // 24 - offset of PdfGlyph array
        uint32_t glyphCount;        // 28
        uint32_t sceneMinX;         // 32 (f32 stored as bits)
        uint32_t sceneMinY;         // 36 (f32 stored as bits)
        uint32_t sceneMaxX;         // 40 (f32 stored as bits)
        uint32_t sceneMaxY;         // 44 (f32 stored as bits)
        uint32_t widthCells;        // 48 [15:0]=widthCells, [31:16]=panX (i16)
        uint32_t heightCells;       // 52 [15:0]=heightCells, [31:16]=panY (i16)
        uint32_t flags;             // 56 [15:0]=flags, [31:16]=zoom (f16)
        uint32_t bgColor;           // 60
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    //=========================================================================
    // Initialization
    //=========================================================================
    Result<void> init();
    void parseArgs(const std::string& args);
    Result<void> loadPdf();

    //=========================================================================
    // Page rendering
    //=========================================================================
    Result<void> renderAllPages();
    Result<void> renderPage(int pageIndex, float yOffset);

    //=========================================================================
    // Glyph placement (CDB-based)
    //=========================================================================
    float placeGlyphs(const std::string& text,
                      float posX, float posY,
                      float effectiveSize,
                      const struct PdfTextState& textState);

    //=========================================================================
    // Font management
    //=========================================================================
    void extractFonts(pdfio_obj_t* pageObj);
    int addFont(const std::string& ttfPath);

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
    std::string _inputSource;  // "-" for payload, or file path
    std::filesystem::path _tempPdfPath;  // temp file when loading from payload

    // pdfio state
    pdfio_file_t* _pdfFile = nullptr;
    int _currentPage = 0;
    int _pageCount = 0;

    // Rendering parameters
    float _fontSize = 0.0f;  // 0 = use PDF's font sizes
    uint32_t _textColor = 0xFF000000;  // Black (ABGR packed)

    // Font / atlas (own instances, not shared with ydraw)
    GpuAllocator::Ptr _globalAllocator;
    FontManager::Ptr _fontManager;
    MsdfAtlas::Ptr _atlas;
    std::unordered_map<std::string, int> _pdfFontMap;   // PDF font name → fontId
    std::unordered_map<std::string, int> _fontIdCache;   // cdbPath → fontId
    std::string _rawFontCacheDir;

    // Atlas texture handle for cardTextureManager
    TextureHandle _atlasTextureHandle = TextureHandle::invalid();

    // Glyph data
    std::vector<PdfGlyph> _glyphs;

    // Grid and derived storage
    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
    float _cellSize = 0.0f;
    uint32_t _gridOffset = 0;
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
    uint32_t _bgColor = 0xFFFFFFFF;  // White background for PDF
    uint32_t _flags = 0;
};

} // namespace yetty::card
