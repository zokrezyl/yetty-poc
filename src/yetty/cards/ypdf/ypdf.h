#pragma once

#include <yetty/card.h>
#include <yetty/gpu-context.h>
#include <yetty/gpu-allocator.h>
#include <yetty/yetty-context.h>
#include <yetty/font-manager.h>
#include <yetty/ydraw-builder.h>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <memory>
#include <map>
#include <filesystem>

// Forward declarations for pdfio (pdfio uses _pdfio_*_s naming)
struct _pdfio_file_s;
typedef struct _pdfio_file_s pdfio_file_t;
struct _pdfio_obj_s;
typedef struct _pdfio_obj_s pdfio_obj_t;

namespace yetty::card {

//=============================================================================
// YPdf - PDF rendering card using YDrawBuilder
//
// Parses PDF content streams, extracts text using CDB glyph advances
// for correct inter-character spacing, and renders via YDrawBuilder's
// spatial hash grid + ydraw shader (0x100003).
//=============================================================================
class YPdf : public yetty::Card {
public:
    using Ptr = std::shared_ptr<YPdf>;

    // Use YDraw shader (0x100000 base + 0x0003)
    static constexpr uint32_t SHADER_GLYPH = 0x100003;

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
    bool needsTexture() const override { return _builder && _builder->hasCustomAtlas(); }

    // Card accessors
    uint32_t metadataOffset() const override { return _metaHandle.offset; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    uint32_t shaderGlyph() const override { return _shaderGlyph; }
    int32_t x() const override { return _x; }
    int32_t y() const override { return _y; }
    void setPosition(int32_t x, int32_t y) override { _x = x; _y = y; }
    uint32_t widthCells() const override { return _widthCells; }
    uint32_t heightCells() const override { return _heightCells; }
    const std::string& name() const override { return _name; }
    void setName(const std::string& n) override { _name = n; }
    bool hasName() const override { return !_name.empty(); }
    void setScreenOrigin(float sx, float sy) override { _screenOriginX = sx; _screenOriginY = sy; }

    void declareBufferNeeds() override;
    Result<void> allocateBuffers() override;
    Result<void> allocateTextures() override;
    Result<void> writeTextures() override;
    Result<void> finalize() override;
    Result<void> dispose() override;
    void suspend() override;
    void setCellSize(float cellWidth, float cellHeight) override {
        _cellWidth = cellWidth;
        _cellHeight = cellHeight;
    }

    //=========================================================================
    // EventListener - zoom/pan + text selection
    //=========================================================================
    Result<bool> onEvent(const base::Event& event) override;

    // Constructor is public for make_shared, use create() factory instead
    YPdf(const YettyContext& ctx,
         int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells,
         const std::string& args, const std::string& payload);

private:
    //=========================================================================
    // Initialization
    //=========================================================================
    Result<void> init();
    void parseArgs(const std::string& args);
    Result<void> loadPdf();

    //=========================================================================
    // Page rendering (delegates to shared renderPdfToBuffer)
    //=========================================================================
    Result<void> renderAllPages();

    //=========================================================================
    // GPU helpers
    //=========================================================================
    uint32_t computeDerivedSize() const;
    Result<void> uploadMetadata();
    void uploadGlyphData();
    void cardPixelToScene(float cardX, float cardY, float& sceneX, float& sceneY);

    //=========================================================================
    // Event handling helpers
    //=========================================================================
    Result<void> registerForEvents();
    Result<void> deregisterFromEvents();

    //=========================================================================
    // State
    //=========================================================================

    YDrawBuilder::Ptr _builder;
    std::shared_ptr<YDrawBuffer> _buffer;

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

    // Font management
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;

    // Atlas texture handle for cardTextureManager
    TextureHandle _atlasTextureHandle = TextureHandle::invalid();

    // Primitive storage (SDF segments/boxes for table borders/fills)
    StorageHandle _primStorage = StorageHandle::invalid();
    uint32_t _primitiveOffset = 0;
    uint32_t _primCount = 0;

    // Derived storage
    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t _gridOffset = 0;
    uint32_t _glyphOffset = 0;
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;

    // View zoom/pan
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    bool _focused = false;

    // Text selection
    bool _selecting = false;
    bool _sortedOrderBuilt = false;
    int32_t _selStartSorted = -1;
    int32_t _selEndSorted = -1;
    uint32_t _cellWidth = 0;
    uint32_t _cellHeight = 0;

    // Dirty flags
    bool _dirty = true;
    bool _metadataDirty = true;

    // Common card state (was in Card base class)
    CardManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    uint32_t _shaderGlyph = 0;
    int32_t _x = 0, _y = 0;
    uint32_t _widthCells = 0, _heightCells = 0;
    float _screenOriginX = 0.0f, _screenOriginY = 0.0f;
    std::string _name;
};

} // namespace yetty::card
