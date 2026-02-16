#include "ypdf.h"
#include "../../ypdf/pdf-renderer.h"
#include "../../ydraw/ydraw-buffer.h"
#include <yetty/ydraw-builder.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

extern "C" {
#include <pdfio.h>
}

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
constexpr int GLFW_KEY_C       = 67;

// Resolve a font family to TTF path via fontconfig
std::string resolveFontPath(const char* family, int weight = 400, bool italic = false) {
    std::string result;
#ifdef YETTY_USE_FONTCONFIG
    FcConfig* config = FcInitLoadConfigAndFonts();
    if (!config) return result;

    FcPattern* pattern = FcPatternCreate();
    if (!pattern) {
        FcConfigDestroy(config);
        return result;
    }

    FcPatternAddString(pattern, FC_FAMILY, reinterpret_cast<const FcChar8*>(family));

    int fcWeight = FC_WEIGHT_REGULAR;
    if (weight <= 300) fcWeight = FC_WEIGHT_LIGHT;
    else if (weight <= 400) fcWeight = FC_WEIGHT_REGULAR;
    else if (weight <= 500) fcWeight = FC_WEIGHT_MEDIUM;
    else if (weight <= 700) fcWeight = FC_WEIGHT_BOLD;
    else fcWeight = FC_WEIGHT_BLACK;
    FcPatternAddInteger(pattern, FC_WEIGHT, fcWeight);
    FcPatternAddInteger(pattern, FC_SLANT, italic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN);

    FcConfigSubstitute(config, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    FcResult fcResult;
    FcPattern* match = FcFontMatch(config, pattern, &fcResult);
    FcPatternDestroy(pattern);

    if (match && fcResult == FcResultMatch) {
        FcChar8* fontPath = nullptr;
        if (FcPatternGetString(match, FC_FILE, 0, &fontPath) == FcResultMatch) {
            result = reinterpret_cast<const char*>(fontPath);
        }
        FcPatternDestroy(match);
    }
    FcConfigDestroy(config);
#else
    (void)family; (void)weight; (void)italic;
#endif
    return result;
}
}

namespace yetty::card {

//=============================================================================
// Constructor / Destructor
//=============================================================================

YPdf::YPdf(const YettyContext& ctx,
           int32_t x, int32_t y,
           uint32_t widthCells, uint32_t heightCells,
           const std::string& args, const std::string& payload)
    : _argsStr(args)
    , _payloadStr(payload)
    , _fontManager(ctx.fontManager)
    , _gpuAllocator(ctx.gpuAllocator)
    , _cardMgr(ctx.cardManager)
    , _gpu(ctx.gpu)
    , _x(x), _y(y)
    , _widthCells(widthCells), _heightCells(heightCells)
{
    _shaderGlyph = SHADER_GLYPH;
    _buffer = *YDrawBuffer::create();
}

YPdf::~YPdf() {
    dispose();
    if (_pdfFile) {
        pdfioFileClose(_pdfFile);
        _pdfFile = nullptr;
    }
    // Clean up temp file if created
    if (!_tempPdfPath.empty() && std::filesystem::exists(_tempPdfPath)) {
        std::error_code ec;
        std::filesystem::remove(_tempPdfPath, ec);
    }
}

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> YPdf::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("YPdf::create: pos=({},{}) size={}x{} payload_len={}",
          x, y, widthCells, heightCells, payload.size());

    auto card = std::make_shared<YPdf>(ctx, x, y, widthCells, heightCells,
                                       args, payload);

    if (auto res = card->init(); !res) {
        yerror("YPdf::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("YPdf::create: init failed");
    }

    yinfo("YPdf::create: SUCCESS, shaderGlyph={:#x} pages={} glyphs={}",
          card->shaderGlyph(), card->_pageCount,
          card->_builder ? card->_builder->glyphCount() : 0);
    return Ok<CardPtr>(card);
}

//=============================================================================
// Initialization
//=============================================================================

Result<void> YPdf::init() {
    ydebug("YPdf::init: START");

    // Allocate metadata slot
    auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
    if (!metaResult) {
        return Err<void>("YPdf::init: failed to allocate metadata");
    }
    _metaHandle = *metaResult;

    // Create builder with buffer
    auto builderRes = YDrawBuilder::create(
        _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
    if (!builderRes) {
        return Err<void>("YPdf::init: failed to create builder", builderRes);
    }
    _builder = *builderRes;

    _builder->setViewport(_widthCells, _heightCells);
    _builder->setView(_viewZoom, _viewPanX, _viewPanY);
    _dirty = true;

    if (auto res = registerForEvents(); !res) {
        ywarn("YPdf::init: event registration failed: {}", error_msg(res));
    }

    parseArgs(_argsStr);

    if (auto res = loadPdf(); !res) {
        return res;
    }

    if (auto res = renderAllPages(); !res) {
        ywarn("YPdf::init: failed to render pages: {}", error_msg(res));
    }

    _dirty = true;
    return Ok();
}

//=============================================================================
// Args parsing
//=============================================================================

void YPdf::parseArgs(const std::string& args) {
    if (args.empty()) return;

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--input" || token == "-i") {
            std::string val;
            if (iss >> val) {
                _inputSource = val;
            }
        } else if (token == "--page" || token == "-p") {
            int val;
            if (iss >> val) {
                _currentPage = std::max(0, val);
            }
        } else if (token == "--font-size") {
            float val;
            if (iss >> val) {
                _fontSize = val;
            }
        } else if (token == "--color") {
            std::string colorStr;
            if (iss >> colorStr) {
                if (colorStr.size() > 2 &&
                    (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X")) {
                    colorStr = colorStr.substr(2);
                }
                _textColor = static_cast<uint32_t>(
                    std::stoul(colorStr, nullptr, 16));
            }
        } else if (token == "--bg-color") {
            std::string colorStr;
            if (iss >> colorStr) {
                if (colorStr.size() > 2 &&
                    (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X")) {
                    colorStr = colorStr.substr(2);
                }
                _buffer->setBgColor(static_cast<uint32_t>(
                    std::stoul(colorStr, nullptr, 16)));
            }
        }
    }
}

//=============================================================================
// PDF loading
//=============================================================================

static bool pdfioErrorHandler(pdfio_file_t* /*pdf*/, const char* message,
                               void* /*data*/) {
    ywarn("pdfio: {}", message);
    return true;  // Continue on error
}

Result<void> YPdf::loadPdf() {
    std::string pdfPath;

    // Determine PDF source based on -i/--input argument
    if (_inputSource == "-" || _inputSource.empty()) {
        // Read from payload (base64-decoded data passed via OSC)
        if (_payloadStr.empty()) {
            return Err<void>("YPdf::loadPdf: empty payload (use -i - to read from payload)");
        }

        // pdfio requires a file path, so write payload to a temp file
        _tempPdfPath = std::filesystem::temp_directory_path() /
                       ("ypdf_" + std::to_string(id()) + ".pdf");
        std::ofstream out(_tempPdfPath, std::ios::binary);
        if (!out) {
            return Err<void>("YPdf::loadPdf: failed to create temp file: " + _tempPdfPath.string());
        }
        out.write(_payloadStr.data(), static_cast<std::streamsize>(_payloadStr.size()));
        out.close();

        pdfPath = _tempPdfPath.string();
        yinfo("YPdf::loadPdf: wrote {} bytes to temp file '{}'", _payloadStr.size(), pdfPath);
    } else {
        // Read from file path specified via -i <path>
        if (!std::filesystem::exists(_inputSource)) {
            return Err<void>("YPdf::loadPdf: file not found: " + _inputSource);
        }
        pdfPath = _inputSource;
        yinfo("YPdf::loadPdf: loading from file '{}'", pdfPath);
    }

    _pdfFile = pdfioFileOpen(pdfPath.c_str(), /*password_cb=*/nullptr,
                              /*password_data=*/nullptr,
                              pdfioErrorHandler, /*error_data=*/nullptr);

    if (!_pdfFile) {
        return Err<void>("YPdf::loadPdf: failed to open: " + pdfPath);
    }

    _pageCount = static_cast<int>(pdfioFileGetNumPages(_pdfFile));
    if (_pageCount == 0) {
        pdfioFileClose(_pdfFile);
        _pdfFile = nullptr;
        return Err<void>("YPdf::loadPdf: PDF has no pages");
    }

    _currentPage = std::clamp(_currentPage, 0, _pageCount - 1);

    yinfo("YPdf::loadPdf: opened '{}' with {} pages", pdfPath, _pageCount);
    return Ok();
}

//=============================================================================
// Page rendering
//=============================================================================

Result<void> YPdf::renderAllPages() {
    if (!_pdfFile || !_builder) {
        return Err<void>("YPdf::renderAllPages: no document or builder");
    }

    // Render PDF into buffer (fonts, text, geometry, scene metadata)
    auto result = renderPdfToBuffer(_pdfFile, _buffer.get());

    if (result.pageCount == 0) {
        return Err<void>("YPdf::renderAllPages: no valid pages");
    }

    yinfo("YPdf::renderAllPages: {} pages, totalHeight={:.1f}",
          result.pageCount, result.totalHeight);

    return Ok();
}

//=============================================================================
// GPU helpers
//=============================================================================

void YPdf::cardPixelToScene(float cardX, float cardY, float& sceneX, float& sceneY) {
    float cardPixelW = static_cast<float>(_widthCells * _cellWidth);
    float cardPixelH = static_cast<float>(_heightCells * _cellHeight);
    if (cardPixelW <= 0 || cardPixelH <= 0) { sceneX = sceneY = 0; return; }

    float contentW = _builder->sceneMaxX() - _builder->sceneMinX();
    float contentH = _builder->sceneMaxY() - _builder->sceneMinY();
    float centerX = _builder->sceneMinX() + contentW * 0.5f;
    float centerY = _builder->sceneMinY() + contentH * 0.5f;
    float viewHalfW = contentW * 0.5f / _viewZoom;
    float viewHalfH = contentH * 0.5f / _viewZoom;
    float viewMinX = centerX - viewHalfW + _viewPanX;
    float viewMinY = centerY - viewHalfH + _viewPanY;
    float viewW = contentW / _viewZoom;
    float viewH = contentH / _viewZoom;

    float cardAspect = cardPixelW / std::max(cardPixelH, 1.0f);
    float viewAspect = viewW / std::max(viewH, 1e-6f);
    float uvX = cardX / cardPixelW;
    float uvY = cardY / cardPixelH;

    if (viewAspect < cardAspect) {
        float visibleW = viewH * cardAspect;
        float offsetX = (visibleW - viewW) * 0.5f;
        sceneX = viewMinX - offsetX + uvX * visibleW;
        sceneY = viewMinY + uvY * viewH;
    } else {
        float visibleH = viewW / cardAspect;
        float offsetY = (visibleH - viewH) * 0.5f;
        sceneX = viewMinX + uvX * viewW;
        sceneY = viewMinY - offsetY + uvY * visibleH;
    }
}

//=============================================================================
// Card lifecycle — forwards to YDrawBuilder
//=============================================================================

void YPdf::renderToStaging(float /*time*/) {
    if (_builder && _dirty) {
        _builder->calculate();
        _dirty = false;
    }
}

bool YPdf::needsBufferRealloc() {
    return _builder && _builder->needsBufferRealloc();
}

bool YPdf::needsTextureRealloc() {
    return _builder && _builder->needsTextureRealloc();
}

void YPdf::declareBufferNeeds() {
    if (!_builder) return;
    if (auto res = _builder->declareBufferNeeds(); !res) {
        yerror("YPdf::declareBufferNeeds: {}", error_msg(res));
    }
}

Result<void> YPdf::allocateBuffers() {
    if (!_builder) return Ok();
    return _builder->allocateBuffers();
}

Result<void> YPdf::allocateTextures() {
    if (!_builder) return Ok();
    return _builder->allocateTextures();
}

Result<void> YPdf::writeTextures() {
    if (!_builder) return Ok();
    return _builder->writeTextures();
}

Result<void> YPdf::finalize() {
    if (!_builder) return Ok();
    return _builder->writeBuffers();
}

Result<void> YPdf::dispose() {
    deregisterFromEvents();
    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YPdf::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }
    return Ok();
}

void YPdf::suspend() {
}

//=============================================================================
// Event handling - zoom/pan + text selection
//=============================================================================

Result<void> YPdf::registerForEvents() {
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) return Err<void>("no EventLoop", loopResult);
    auto loop = *loopResult;
    auto self = sharedAs<base::EventListener>();
    if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardMouseDown, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardMouseUp, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardMouseMove, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardKeyDown, self, 1000); !res) return res;
    return Ok();
}

Result<void> YPdf::deregisterFromEvents() {
    if (weak_from_this().expired()) return Ok();
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) return Ok();
    (*loopResult)->deregisterListener(sharedAs<base::EventListener>());
    return Ok();
}

Result<bool> YPdf::onEvent(const base::Event& event) {
    if (event.type == base::Event::Type::SetFocus) {
        if (event.setFocus.objectId == id()) {
            _focused = true;
            return Ok(true);
        } else if (_focused) {
            _focused = false;
        }
        return Ok(false);
    }

    if (event.type == base::Event::Type::CardScroll &&
        event.cardScroll.targetId == id()) {
        float sceneW = _builder->sceneMaxX() - _builder->sceneMinX();
        float sceneH = _builder->sceneMaxY() - _builder->sceneMinY();

        if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
            float zoomDelta = event.cardScroll.dy * 0.1f * _viewZoom;
            float newZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 1000.0f);
            if (newZoom != _viewZoom) {
                _viewZoom = newZoom;
                _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            }
            return Ok(true);
        } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
            _viewPanX += event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            return Ok(true);
        } else {
            _viewPanY += event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            return Ok(true);
        }
    }

    // Mouse selection
    if (event.type == base::Event::Type::CardMouseDown &&
        event.cardMouse.targetId == id() && event.cardMouse.button == 0) {
        if (!_builder || _builder->glyphCount() == 0) return Ok(false);
        if (!_sortedOrderBuilt) {
            _builder->buildGlyphSortedOrder();
            _sortedOrderBuilt = true;
        }
        float sx, sy;
        cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
        int32_t idx = _builder->findNearestGlyphSorted(sx, sy);
        _selStartSorted = idx;
        _selEndSorted = idx;
        _selecting = true;
        _builder->setSelectionRange(_selStartSorted, _selEndSorted);
        // glyphs marked dirty by setSelectionRange, written in finalize→writeBuffers
        return Ok(true);
    }

    if (event.type == base::Event::Type::CardMouseMove &&
        event.cardMouse.targetId == id() && _selecting) {
        float sx, sy;
        cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
        int32_t idx = _builder->findNearestGlyphSorted(sx, sy);
        if (idx != _selEndSorted) {
            _selEndSorted = idx;
            _builder->setSelectionRange(_selStartSorted, _selEndSorted);
            // glyphs marked dirty by setSelectionRange, written in finalize→writeBuffers
        }
        return Ok(true);
    }

    if (event.type == base::Event::Type::CardMouseUp &&
        event.cardMouse.targetId == id() && event.cardMouse.button == 0) {
        if (_selecting) {
            float sx, sy;
            cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
            _selEndSorted = _builder->findNearestGlyphSorted(sx, sy);
            _selecting = false;
            _builder->setSelectionRange(_selStartSorted, _selEndSorted);
            // glyphs marked dirty by setSelectionRange, written in finalize→writeBuffers
        }
        return Ok(true);
    }

    // Ctrl+C: copy selected text
    if (event.type == base::Event::Type::CardKeyDown &&
        event.cardKey.targetId == id() &&
        event.cardKey.key == GLFW_KEY_C &&
        (event.cardKey.mods & GLFW_MOD_CONTROL)) {
        std::string text = _builder->getSelectedText();
        if (!text.empty()) {
            auto loop = *base::EventLoop::instance();
            loop->dispatch(base::Event::copyEvent(
                std::make_shared<std::string>(text)));
            yinfo("YPdf: copied {} bytes to clipboard", text.size());
        }
        return Ok(true);
    }

    return Ok(false);
}

} // namespace yetty::card
