#include "pdf.h"
#include <yetty/yetty-context.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#include <fpdfview.h>
#include <fpdf_edit.h>

#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <mutex>

// GLFW modifier constants
constexpr int GLFW_MOD_CONTROL = 0x0002;

namespace yetty::card {

//=============================================================================
// PDFium library init (once per process)
//=============================================================================
static std::once_flag s_pdfiumInitFlag;

static void ensurePdfiumInit() {
    std::call_once(s_pdfiumInitFlag, []() {
        FPDF_LIBRARY_CONFIG config = {};
        config.version = 2;
        FPDF_InitLibraryWithConfig(&config);
        yinfo("PDFium library initialized");
    });
}

//=============================================================================
// PdfImpl
//=============================================================================

class PdfImpl : public Pdf {
public:
    PdfImpl(const YettyContext& ctx,
            int32_t x, int32_t y,
            uint32_t widthCells, uint32_t heightCells,
            const std::string& args, const std::string& payload)
        : Pdf(ctx.cardBufferManager, ctx.gpu, x, y, widthCells, heightCells)
        , _ctx(ctx)
        , _argsStr(args)
        , _pdfData(payload.begin(), payload.end())
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~PdfImpl() override {
        dispose();
    }

    const char* typeName() const override { return "pdf"; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    void setCellSize(uint32_t cellWidth, uint32_t cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            _needsRender = true;
            _metadataDirty = true;
        }
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() {
        ensurePdfiumInit();

        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("Pdf::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("Pdf::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Load PDF document from memory
        _doc = FPDF_LoadMemDocument(_pdfData.data(), static_cast<int>(_pdfData.size()), nullptr);
        if (!_doc) {
            unsigned long err = FPDF_GetLastError();
            return Err<void>("Pdf::init: FPDF_LoadMemDocument failed (error " + std::to_string(err) + ")");
        }

        _pageCount = FPDF_GetPageCount(_doc);
        yinfo("Pdf::init: loaded PDF with {} pages", _pageCount);

        if (_pageCount == 0) {
            return Err<void>("Pdf::init: PDF has no pages");
        }

        // Render first page directly at card pixel size
        if (auto res = renderCurrentPage(); !res) {
            return Err<void>("Pdf::init: failed to render first page", res);
        }

        // Upload rendered pixels to GPU
        if (!_pagePixels.empty()) {
            if (auto res = uploadPixelsToGPU(); !res) {
                return Err<void>("Pdf::init: failed to upload pixels", res);
            }
        }

        // Upload initial metadata
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("Pdf::init: failed to upload metadata", res);
        }

        // Register for events
        if (auto res = registerForEvents(); !res) {
            return Err<void>("Pdf::init: failed to register for events", res);
        }

        return Ok();
    }

    Result<void> dispose() override {
        // Deregister from events
        deregisterFromEvents();

        // Release buffer allocations
        if (_textureDataHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateTextureData(_textureDataHandle);
            _textureDataHandle = TextureDataHandle::invalid();
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        // Close PDF document
        if (_doc) {
            FPDF_CloseDocument(_doc);
            _doc = nullptr;
        }

        // Clear CPU memory
        _pagePixels.clear();
        _pagePixels.shrink_to_fit();
        _pdfData.clear();
        _pdfData.shrink_to_fit();

        return Ok();
    }

    Result<void> update(float time) override {
        (void)time;

        if (_needsRender) {
            if (auto res = renderCurrentPage(); !res) {
                return Err<void>("Pdf::update: render failed", res);
            }
            _needsRender = false;
            _needsUpload = true;
        }

        if (_needsUpload && !_pagePixels.empty()) {
            if (auto res = uploadPixelsToGPU(); !res) {
                return Err<void>("Pdf::update: upload failed", res);
            }
            _needsUpload = false;
        }

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("Pdf::update: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // EventListener interface
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        // Handle SetFocus events
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                if (!_focused) {
                    _focused = true;
                    ydebug("Pdf::onEvent: focused (id={})", id());
                }
                return Ok(true);
            } else if (_focused) {
                _focused = false;
                ydebug("Pdf::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        if (!_focused) {
            return Ok(false);
        }

        // Handle scroll events when focused
        if (event.type == base::Event::Type::Scroll) {
            if (event.scroll.mods & GLFW_MOD_CONTROL) {
                // Ctrl+Scroll: zoom
                float zoomDelta = event.scroll.dy * 0.1f;
                float newZoom = std::clamp(_contentZoom + zoomDelta, 0.1f, 10.0f);
                if (newZoom != _contentZoom) {
                    _contentZoom = newZoom;
                    _needsRender = true;
                    _metadataDirty = true;
                    yinfo("Pdf::onEvent: content zoom={:.2f}", _contentZoom);
                }
                return Ok(true);
            } else {
                // Plain scroll: page navigation
                if (event.scroll.dy > 0 && _currentPage > 0) {
                    _currentPage--;
                    _needsRender = true;
                    _metadataDirty = true;
                    yinfo("Pdf::onEvent: prev page -> {}", _currentPage);
                    return Ok(true);
                } else if (event.scroll.dy < 0 && _currentPage < _pageCount - 1) {
                    _currentPage++;
                    _needsRender = true;
                    _metadataDirty = true;
                    yinfo("Pdf::onEvent: next page -> {}", _currentPage);
                    return Ok(true);
                }
            }
        }

        return Ok(false);
    }

private:
    //=========================================================================
    // Event registration
    //=========================================================================

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("Pdf::registerForEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
            return Err<void>("Pdf::registerForEvents: failed to register SetFocus", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::Scroll, self, 1000); !res) {
            return Err<void>("Pdf::registerForEvents: failed to register Scroll", res);
        }

        yinfo("Pdf card {} registered for events (priority 1000)", id());
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("Pdf::deregisterFromEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;

        if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
            return Err<void>("Pdf::deregisterFromEvents: failed to deregister", res);
        }
        return Ok();
    }

    //=========================================================================
    // PDF rendering — renders directly at card pixel size
    //
    // The bitmap is always card-sized (widthCells * cellWidth × heightCells * cellHeight).
    // At zoom=1 the whole page fits. At zoom>1 we tell PDFium to render the page
    // at (baseSize * zoom) pixels but offset so that the viewport around
    // (_centerX, _centerY) lands in the card-sized bitmap. PDFium clips
    // automatically and — being a vector renderer — produces full-resolution
    // output for the visible rectangle.
    //=========================================================================

    Result<void> renderCurrentPage() {
        if (!_doc) {
            return Err<void>("Pdf::renderCurrentPage: no document");
        }

        FPDF_PAGE page = FPDF_LoadPage(_doc, _currentPage);
        if (!page) {
            return Err<void>("Pdf::renderCurrentPage: failed to load page " + std::to_string(_currentPage));
        }

        // Card pixel dimensions (fixed output size)
        uint32_t cardW = _widthCells * _cellWidth;
        uint32_t cardH = _heightCells * _cellHeight;
        if (cardW == 0) cardW = 800;
        if (cardH == 0) cardH = 600;

        _renderWidth = cardW;
        _renderHeight = cardH;

        // Page aspect ratio
        double pageWidthPt = FPDF_GetPageWidth(page);
        double pageHeightPt = FPDF_GetPageHeight(page);
        double pageAspect = pageWidthPt / pageHeightPt;
        double cardAspect = static_cast<double>(cardW) / static_cast<double>(cardH);

        // At zoom=1 the whole page fits in the card (preserving aspect ratio)
        double baseW, baseH;
        if (pageAspect > cardAspect) {
            baseW = cardW;
            baseH = cardW / pageAspect;
        } else {
            baseH = cardH;
            baseW = cardH * pageAspect;
        }

        // At current zoom the page occupies this many pixels
        int sizeX = static_cast<int>(baseW * _contentZoom);
        int sizeY = static_cast<int>(baseH * _contentZoom);

        // Position so that (_centerX, _centerY) of the page is at the
        // center of the card bitmap. PDFium clips anything outside.
        int startX = static_cast<int>(cardW / 2.0 - _centerX * sizeX);
        int startY = static_cast<int>(cardH / 2.0 - _centerY * sizeY);

        yinfo("Pdf::renderCurrentPage: page {} bitmap={}x{} pageSize={}x{} "
              "start=({},{}) zoom={:.2f} center=({:.2f},{:.2f})",
              _currentPage, cardW, cardH, sizeX, sizeY,
              startX, startY, _contentZoom, _centerX, _centerY);

        // Create card-sized bitmap (BGRA)
        FPDF_BITMAP bitmap = FPDFBitmap_Create(cardW, cardH, 0);
        if (!bitmap) {
            FPDF_ClosePage(page);
            return Err<void>("Pdf::renderCurrentPage: FPDFBitmap_Create failed");
        }

        // White background
        FPDFBitmap_FillRect(bitmap, 0, 0, cardW, cardH, 0xFFFFFFFF);

        // Render — PDFium scales the page to sizeX×sizeY and places it at
        // (startX, startY) on the bitmap. Only the visible part is rasterized.
        FPDF_RenderPageBitmap(bitmap, page, startX, startY, sizeX, sizeY, 0,
                              FPDF_ANNOT | FPDF_PRINTING);

        // Convert BGRA → RGBA
        uint8_t* bitmapData = static_cast<uint8_t*>(FPDFBitmap_GetBuffer(bitmap));
        int stride = FPDFBitmap_GetStride(bitmap);
        size_t pixelCount = static_cast<size_t>(cardW) * cardH;

        _pagePixels.resize(pixelCount * 4);

        for (uint32_t y = 0; y < cardH; ++y) {
            const uint8_t* srcRow = bitmapData + y * stride;
            uint8_t* dstRow = _pagePixels.data() + y * cardW * 4;
            for (uint32_t x = 0; x < cardW; ++x) {
                dstRow[x * 4 + 0] = srcRow[x * 4 + 2]; // R ← B
                dstRow[x * 4 + 1] = srcRow[x * 4 + 1]; // G
                dstRow[x * 4 + 2] = srcRow[x * 4 + 0]; // B ← R
                dstRow[x * 4 + 3] = srcRow[x * 4 + 3]; // A
            }
        }

        FPDFBitmap_Destroy(bitmap);
        FPDF_ClosePage(page);

        _metadataDirty = true;

        yinfo("Pdf::renderCurrentPage: rendered {} bytes ({}x{})",
              _pagePixels.size(), cardW, cardH);
        return Ok();
    }

    //=========================================================================
    // Upload card-sized pixel buffer directly to GPU textureData
    //=========================================================================

    Result<void> uploadPixelsToGPU() {
        if (_pagePixels.empty() || _renderWidth == 0 || _renderHeight == 0) {
            return Err<void>("Pdf::uploadPixelsToGPU: no data");
        }

        uint32_t dataSize = _renderWidth * _renderHeight * 4;

        // (Re)allocate textureData slot if size changed
        if (!_textureDataHandle.isValid() || _textureDataHandle.size != dataSize) {
            if (_textureDataHandle.isValid()) {
                _cardMgr->deallocateTextureData(_textureDataHandle);
                _textureDataHandle = TextureDataHandle::invalid();
            }

            auto allocResult = _cardMgr->allocateTextureData(dataSize);
            if (!allocResult) {
                return Err<void>("Pdf::uploadPixelsToGPU: failed to allocate textureData", allocResult);
            }
            _textureDataHandle = *allocResult;

            yinfo("Pdf::uploadPixelsToGPU: allocated {} bytes at offset {}",
                  dataSize, _textureDataHandle.offset);
        }

        // Copy rendered pixels into the CardBufferManager's CPU-side textureData buffer.
        // flush() will upload dirty regions to GPU, and the atlas compute shader
        // packs them into the atlas texture for the fragment shader to sample.
        std::memcpy(_textureDataHandle.data, _pagePixels.data(), dataSize);
        _cardMgr->markTextureDataDirty(_textureDataHandle);

        yinfo("Pdf::uploadPixelsToGPU: copied {}x{} ({} bytes) to textureData CPU buffer",
              _renderWidth, _renderHeight, dataSize);
        return Ok();
    }

    //=========================================================================
    // Args parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        yinfo("Pdf::parseArgs: args='{}'", args);

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--zoom" || token == "-z") {
                float val;
                if (iss >> val) {
                    _contentZoom = std::clamp(val, 0.1f, 10.0f);
                }
            } else if (token == "--page" || token == "-p") {
                int val;
                if (iss >> val) {
                    _currentPage = std::max(0, val);
                }
            }
        }
    }

    //=========================================================================
    // Metadata upload
    //=========================================================================

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("Pdf::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};
        meta.textureDataOffset = _textureDataHandle.isValid() ? _textureDataHandle.offset : 0;
        meta.textureWidth = _renderWidth;
        meta.textureHeight = _renderHeight;
        meta.atlasX = 0;
        meta.atlasY = 0;
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.zoom = 1.0f;
        meta.centerX = 0.5f;
        meta.centerY = 0.5f;
        meta.flags = 0;
        meta.bgColor = 0xFFFFFFFF;
        meta.scaledWidth = _renderWidth;
        meta.scaledHeight = _renderHeight;

        yinfo("Pdf::uploadMetadata: offset={} size={}x{} page={}/{} zoom={:.2f}",
              _metaHandle.offset, _renderWidth, _renderHeight,
              _currentPage + 1, _pageCount, _contentZoom);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("Pdf::uploadMetadata: write failed", res);
        }

        return Ok();
    }

    //=========================================================================
    // Data structures
    //=========================================================================

    // Metadata structure (64 bytes, matches Image/Texture card shader layout)
    struct Metadata {
        uint32_t textureDataOffset;
        uint32_t textureWidth;
        uint32_t textureHeight;
        uint32_t atlasX;
        uint32_t atlasY;
        uint32_t widthCells;
        uint32_t heightCells;
        float zoom;
        float centerX;
        float centerY;
        uint32_t flags;
        uint32_t bgColor;
        uint32_t scaledWidth;
        uint32_t scaledHeight;
        uint32_t _reserved[2];
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    //=========================================================================
    // Member variables
    //=========================================================================

    const YettyContext& _ctx;
    std::string _argsStr;

    // PDF document (PDFium)
    std::vector<uint8_t> _pdfData;         // Keep payload alive for PDFium
    FPDF_DOCUMENT _doc = nullptr;
    int _currentPage = 0;
    int _pageCount = 0;

    // Rendered page pixels (CPU memory, RGBA, card-sized)
    std::vector<uint8_t> _pagePixels;
    uint32_t _renderWidth = 0;
    uint32_t _renderHeight = 0;

    // Content zoom & pan
    float _contentZoom = 1.0f;
    float _centerX = 0.5f;   // 0..1 in page space
    float _centerY = 0.5f;

    // Cell dimensions
    uint32_t _cellWidth = 10;
    uint32_t _cellHeight = 20;

    // Buffer handle for GPU textureData
    TextureDataHandle _textureDataHandle = TextureDataHandle::invalid();

    // State
    bool _focused = false;
    bool _needsRender = false;
    bool _needsUpload = false;
    bool _metadataDirty = true;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<Pdf::Ptr> Pdf::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;

    if (!yettyCtx.cardBufferManager) {
        return Err<Ptr>("Pdf::createImpl: null CardBufferManager");
    }

    auto card = std::make_shared<PdfImpl>(
        yettyCtx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        return Err<Ptr>("Pdf::createImpl: init failed", res);
    }

    return Ok<Ptr>(card);
}

} // namespace yetty::card
