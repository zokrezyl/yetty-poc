#include "yhtml.h"
#include "html-container.h"
#include "html-renderer.h"
#include "http-fetcher.h"
#include <yetty/ydraw-builder.h>
#include "../../ydraw/ydraw-buffer.h"
#include "../../ydraw/ydraw-types.gen.h"
using yetty::YDrawBuffer;
#include <yetty/base/event-loop.h>
#include <yetty/msdf-glyph-data.h>
#include <yetty/card-texture-manager.h>
#include <ytrace/ytrace.hpp>
#include <litehtml.h>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstring>

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

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
// YHtmlImpl - full implementation with builder, GPU buffers, litehtml
//=============================================================================

class YHtmlImpl : public YHtml {
public:
    YHtmlImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : YHtml(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _screenId(ctx.screenId)
        , _argsStr(args)
        , _payloadStr(payload)
        , _fontManager(ctx.fontManager)
        , _gpuAllocator(ctx.gpuAllocator)
    {
        _shaderGlyph = SHADER_GLYPH;
        _buffer = *YDrawBuffer::create();
        // Default viewWidth from card pixel dimensions
        _viewWidth = static_cast<float>(_widthCells * _cellWidth);
    }

    ~YHtmlImpl() override { dispose(); }

    //=========================================================================
    // Card lifecycle
    //=========================================================================

    bool needsBuffer() const override { return true; }

    bool needsTexture() const override {
        return _builder && _builder->hasCustomAtlas();
    }

    bool needsBufferRealloc() override {
        return _builder && _builder->needsBufferRealloc();
    }

    bool needsTextureRealloc() override {
        return _builder && _builder->needsTextureRealloc();
    }

    void renderToStaging(float time) override {
        (void)time;
        if (!_builder) return;

        // First render: now that setCellSize has been called, we know the actual card width
        if (!_htmlContent.empty() && !_document) {
            if (auto res = renderHtmlInternal(); !res) {
                ywarn("YHtmlImpl::renderToStaging: render failed: {}", error_msg(res));
            } else {
                yinfo("YHtmlImpl::renderToStaging: {} prims, {} glyphs",
                      _builder->primitiveCount(), _builder->glyphCount());
                _dirty = true;
            }
        }

        // Process deferred navigation (avoid re-entrancy in litehtml callbacks)
        if (!_pendingNavigateUrl.empty()) {
            std::string url = std::move(_pendingNavigateUrl);
            _pendingNavigateUrl.clear();
            navigateTo(url);
            return;  // navigation supersedes redraw
        }

        // Process deferred redraw (from mouse events etc.)
        if (_needsRedraw && _document) {
            _needsRedraw = false;
            _buffer->clear();
            int viewW = static_cast<int>(_viewWidth);
            int docHeight = _document->height();
            litehtml::position clip(0, 0, viewW, docHeight);
            _document->draw(0, 0, 0, &clip);
            _dirty = true;
        }

        // Calculate grid when dirty (like ypdf does)
        if (_dirty) {
            _builder->calculate();
            _dirty = false;
        }
    }

    uint32_t metadataSlotIndex() const override {
        return _metaHandle.offset / 64;
    }

    Result<void> dispose() override {
        deregisterFromEvents();
        _derivedStorage = StorageHandle::invalid();
        _primStorage = StorageHandle::invalid();
        _primCount = 0;
        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("YHtmlImpl::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {
        _derivedStorage = StorageHandle::invalid();
        _primStorage = StorageHandle::invalid();
        _primCount = 0;
    }

    void declareBufferNeeds() override {
        if (!_builder) return;
        if (auto res = _builder->declareBufferNeeds(); !res) {
            yerror("YHtmlImpl::declareBufferNeeds: {}", error_msg(res));
        }
    }

    Result<void> allocateBuffers() override {
        if (!_builder) return Ok();
        return _builder->allocateBuffers();
    }

    Result<void> allocateTextures() override {
        if (!_builder) return Ok();
        return _builder->allocateTextures();
    }

    Result<void> writeTextures() override {
        if (!_builder) return Ok();
        return _builder->writeTextures();
    }

    Result<void> finalize() override {
        if (!_builder) return Ok();
        return _builder->writeBuffers();
    }

    //=========================================================================
    // Events - zoom/pan
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                _focused = true;
            } else if (_focused) {
                _focused = false;
            }
            // Don't consume — GPUScreen needs SetFocus to track _focusedCardId
            return Ok(false);
        }

        // Mouse events → litehtml
        if ((event.type == base::Event::Type::CardMouseDown ||
             event.type == base::Event::Type::CardMouseUp ||
             event.type == base::Event::Type::CardMouseMove) &&
            event.cardMouse.targetId == id() && _document) {
            int docX, docY;
            cardPixelToDocCoords(event.cardMouse.x, event.cardMouse.y, docX, docY);

            litehtml::position::vector redrawBoxes;
            if (event.type == base::Event::Type::CardMouseMove) {
                _document->on_mouse_over(docX, docY, docX, docY, redrawBoxes);
            } else if (event.type == base::Event::Type::CardMouseDown && event.cardMouse.button == 0) {
                _document->on_lbutton_down(docX, docY, docX, docY, redrawBoxes);
            } else if (event.type == base::Event::Type::CardMouseUp && event.cardMouse.button == 0) {
                _document->on_lbutton_up(docX, docY, docX, docY, redrawBoxes);
            }

            if (!redrawBoxes.empty()) {
                yinfo("YHtmlImpl: mouse event at doc({},{}) triggered redraw ({} boxes)",
                      docX, docY, redrawBoxes.size());
                redrawHtml();
            }
            return Ok(true);
        }

        // Keyboard events
        if (event.type == base::Event::Type::CardKeyDown &&
            event.cardKey.targetId == id()) {
            return handleCardKey(event.cardKey.key, event.cardKey.mods);
        }
        if (event.type == base::Event::Type::CardChar &&
            event.cardChar.targetId == id()) {
            return handleCardChar(event.cardChar.codepoint, event.cardChar.mods);
        }

        if (event.type == base::Event::Type::CardScroll &&
            event.cardScroll.targetId == id()) {
            float sceneW = _builder->sceneMaxX() - _builder->sceneMinX();
            float sceneH = _builder->sceneMaxY() - _builder->sceneMinY();

            if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
                float zoomDelta = event.cardScroll.dy * 0.1f;
                float newZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 20.0f);
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

        return Ok(false);
    }

    //=========================================================================
    // Init
    //=========================================================================

    Result<void> init() {
        auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
        if (!metaResult) {
            return Err<void>("YHtmlImpl::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        // Create builder with buffer
        auto builderRes = YDrawBuilder::create(
            _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
        if (!builderRes) {
            return Err<void>("YHtmlImpl::init: failed to create builder", builderRes);
        }
        _builder = *builderRes;

        if (auto res = registerForEvents(); !res) {
            ywarn("YHtmlImpl::init: event registration failed: {}", error_msg(res));
        }

        // White background for HTML
        _buffer->setBgColor(0xFFFFFFFF);

        // Enable uniform scaling (same as ypdf)
        _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);
        _builder->setViewport(_widthCells, _heightCells);
        _builder->setView(_viewZoom, _viewPanX, _viewPanY);
        _dirty = true;

        // Create HTTP fetcher
        auto fetcherResult = HttpFetcher::create();
        if (!fetcherResult) {
            return Err<void>("YHtmlImpl: failed to create HttpFetcher");
        }
        _fetcher = std::move(*fetcherResult);

        parseArgs(_argsStr);

        // Load HTML content (render deferred to renderToStaging when cell size is known)
        if (auto res = loadContent(); !res) {
            ywarn("YHtmlImpl::init: failed to load content: {}", error_msg(res));
        }

        _metadataDirty = true;
        return Ok();
    }

private:
    //=========================================================================
    // GPU helpers
    //=========================================================================

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("YHtmlImpl::uploadMetadata: invalid handle");
        }

        // Pack zoom as f16 in upper 16 bits of flags
        uint32_t zoomBits;
        {
            uint32_t f32bits;
            std::memcpy(&f32bits, &_viewZoom, sizeof(float));
            uint32_t sign = (f32bits >> 16) & 0x8000;
            int32_t  exp  = ((f32bits >> 23) & 0xFF) - 127 + 15;
            uint32_t mant = (f32bits >> 13) & 0x3FF;
            if (exp <= 0) { exp = 0; mant = 0; }
            else if (exp >= 31) { exp = 31; mant = 0; }
            zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
        }

        float sceneMinX = _builder->sceneMinX();
        float sceneMinY = _builder->sceneMinY();
        float sceneMaxX = _builder->sceneMaxX();
        float sceneMaxY = _builder->sceneMaxY();
        float contentW = sceneMaxX - sceneMinX;
        float contentH = sceneMaxY - sceneMinY;

        int16_t panXi16 = static_cast<int16_t>(std::clamp(
            _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
        int16_t panYi16 = static_cast<int16_t>(std::clamp(
            _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

        float cellSize = _builder->cellSize();

        YDrawMetadata meta = {};
        meta.primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
        meta.primitiveCount = _primCount;
        meta.gridOffset = _gridOffset;
        meta.gridWidth = _gridWidth;
        meta.gridHeight = _gridHeight;
        std::memcpy(&meta.cellSize, &cellSize, sizeof(float));
        meta.glyphOffset = _glyphOffset;
        meta.glyphCount = static_cast<uint32_t>(_builder->glyphs().size());
        std::memcpy(&meta.sceneMinX, &sceneMinX, sizeof(float));
        std::memcpy(&meta.sceneMinY, &sceneMinY, sizeof(float));
        std::memcpy(&meta.sceneMaxX, &sceneMaxX, sizeof(float));
        std::memcpy(&meta.sceneMaxY, &sceneMaxY, sizeof(float));
        meta.widthCells  = (_widthCells & 0xFFFF) |
                           (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
        meta.heightCells = (_heightCells & 0xFFFF) |
                           (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
        meta.flags = (_builder->flags() & 0xFFFF) | (zoomBits << 16);
        meta.bgColor = _builder->bgColor();

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("YHtmlImpl::uploadMetadata: write failed");
        }
        return Ok();
    }

    //=========================================================================
    // Events
    //=========================================================================

    Result<void> registerForEvents() {
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
        if (auto res = loop->registerListener(base::Event::Type::CardChar, self, 1000); !res) return res;
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        if (weak_from_this().expired()) return Ok();
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return Ok();
        (*loopResult)->deregisterListener(sharedAs<base::EventListener>());
        return Ok();
    }

    //=========================================================================
    // Input helpers
    //=========================================================================

    void setCellSize(float cellWidth, float cellHeight) override {
        _cellWidth = cellWidth;
        _cellHeight = cellHeight;
        // Update view width to match card pixel width
        _viewWidth = static_cast<float>(_widthCells) * cellWidth;
        // Tell builder about viewport and view
        if (_builder) {
            _builder->setViewport(_widthCells, _heightCells);
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
        }
    }

    void cardPixelToDocCoords(float cardX, float cardY, int& docX, int& docY) {
        // Card pixel dimensions from actual cell size
        float cardPixelW = static_cast<float>(_widthCells * _cellWidth);
        float cardPixelH = static_cast<float>(_heightCells * _cellHeight);

        float contentMinX = _builder->sceneMinX();
        float contentMinY = _builder->sceneMinY();
        float contentW = _builder->sceneMaxX() - contentMinX;
        float contentH = _builder->sceneMaxY() - contentMinY;

        // Match shader's view computation exactly
        float centerX = contentMinX + contentW * 0.5f;
        float centerY = contentMinY + contentH * 0.5f;
        float viewHalfW = contentW * 0.5f / _viewZoom;
        float viewHalfH = contentH * 0.5f / _viewZoom;
        float viewMinX = centerX - viewHalfW + _viewPanX;
        float viewMinY = centerY - viewHalfH + _viewPanY;
        float viewW = contentW / _viewZoom;
        float viewH = contentH / _viewZoom;

        // Match shader's uniform scaling (aspect ratio correction)
        float cardAspect = cardPixelW / std::max(cardPixelH, 1.0f);
        float viewAspect = viewW / std::max(viewH, 1e-6f);

        float uvX = cardX / cardPixelW;
        float uvY = cardY / cardPixelH;

        float sceneX, sceneY;
        if (viewAspect < cardAspect) {
            // Tall view: fits height, horizontal padding
            float visibleW = viewH * cardAspect;
            float offsetX = (visibleW - viewW) * 0.5f;
            sceneX = viewMinX - offsetX + uvX * visibleW;
            sceneY = viewMinY + uvY * viewH;
        } else {
            // Wide view: fits width, vertical padding
            float visibleH = viewW / cardAspect;
            float offsetY = (visibleH - viewH) * 0.5f;
            sceneX = viewMinX + uvX * viewW;
            sceneY = viewMinY - offsetY + uvY * visibleH;
        }

        docX = static_cast<int>(sceneX);
        docY = static_cast<int>(sceneY);
    }

    void redrawHtml() {
        // Don't modify builder during event handling — defer to renderToStaging
        _needsRedraw = true;
    }

    void navigateTo(const std::string& url) {
        if (!_fetcher) return;

        // Resolve relative URLs against current base
        std::string resolvedUrl = _fetcher->resolveUrl(url);
        yinfo("YHtmlImpl::navigateTo: {} -> {}", url, resolvedUrl);

        // Fetch the new page
        _fetcher->setBaseUrl(resolvedUrl);
        auto body = _fetcher->fetch(resolvedUrl);
        if (!body) {
            ywarn("YHtmlImpl::navigateTo: failed to fetch {}", resolvedUrl);
            return;
        }

        // Reset view state
        _viewZoom = 1.0f;
        _viewPanX = 0.0f;
        _viewPanY = 0.0f;
        _builder->setView(_viewZoom, _viewPanX, _viewPanY);

        // Destroy old document before old container (document holds raw ptr to container)
        _document.reset();
        _container.reset();

        // Re-render with new content
        _htmlContent = std::move(*body);
        _buffer->clear();

        if (auto res = renderHtmlInternal(); !res) {
            ywarn("YHtmlImpl::navigateTo: render failed: {}", error_msg(res));
            return;
        }

        _dirty = true;
        _metadataDirty = true;
        _needsBufferRealloc = true;
        _needsTextureRealloc = true;
        yinfo("YHtmlImpl::navigateTo: rendered {} prims, {} glyphs",
              _builder->primitiveCount(), _builder->glyphCount());
    }

    Result<bool> handleCardKey(int key, int mods) {
        constexpr int GLFW_KEY_UP = 265;
        constexpr int GLFW_KEY_DOWN = 264;
        constexpr int GLFW_KEY_LEFT = 263;
        constexpr int GLFW_KEY_RIGHT = 262;

        float sceneW = _builder->sceneMaxX() - _builder->sceneMinX();
        float sceneH = _builder->sceneMaxY() - _builder->sceneMinY();
        float scrollStep = 0.05f;

        if (key == GLFW_KEY_UP) {
            _viewPanY -= scrollStep * sceneH / _viewZoom;
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            return Ok(true);
        } else if (key == GLFW_KEY_DOWN) {
            _viewPanY += scrollStep * sceneH / _viewZoom;
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            return Ok(true);
        } else if (key == GLFW_KEY_LEFT) {
            _viewPanX -= scrollStep * sceneW / _viewZoom;
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            return Ok(true);
        } else if (key == GLFW_KEY_RIGHT) {
            _viewPanX += scrollStep * sceneW / _viewZoom;
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            return Ok(true);
        }
        return Ok(false);
    }

    Result<bool> handleCardChar(uint32_t /*codepoint*/, int /*mods*/) {
        // Future: text input for form fields
        return Ok(false);
    }

    //=========================================================================
    // Args parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        if (args.empty()) return;

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "-i" || token == "--input") {
                std::string val;
                if (iss >> val) _inputSource = val;
            } else if (token == "--font-size") {
                float val;
                if (iss >> val) _fontSize = val;
            } else if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.size() > 2 &&
                        (colorStr.substr(0, 2) == "0x" ||
                         colorStr.substr(0, 2) == "0X")) {
                        colorStr = colorStr.substr(2);
                    }
                    _buffer->setBgColor(static_cast<uint32_t>(
                        std::stoul(colorStr, nullptr, 16)));
                }
            }
        }
    }

    //=========================================================================
    // Content loading
    //=========================================================================

    Result<void> loadContent() {
        if (_inputSource == "-" || _inputSource.empty()) {
            // "-" means payload, empty also falls back to payload
            if (_payloadStr.empty()) {
                return Err<void>("YHtmlImpl::loadContent: no payload");
            }
            yinfo("YHtmlImpl::loadContent: using payload ({} bytes)", _payloadStr.size());
            _htmlContent = _payloadStr;
        } else if (HttpFetcher::isUrl(_inputSource)) {
            _fetcher->setBaseUrl(_inputSource);
            yinfo("YHtmlImpl::loadContent: fetching URL: {}", _inputSource);
            auto body = _fetcher->fetch(_inputSource);
            if (!body) {
                return Err<void>("YHtmlImpl::loadContent: failed to fetch URL");
            }
            _htmlContent = std::move(*body);
        } else {
            yinfo("YHtmlImpl::loadContent: reading file: {}", _inputSource);
            std::ifstream file(_inputSource);
            if (!file) {
                return Err<void>("YHtmlImpl::loadContent: failed to open file");
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            _htmlContent = buffer.str();
        }

        if (_htmlContent.empty()) {
            return Err<void>("YHtmlImpl::loadContent: empty content");
        }

        yinfo("YHtmlImpl::loadContent: {} bytes", _htmlContent.size());
        return Ok();
    }

    //=========================================================================
    // HTML rendering via litehtml
    //=========================================================================

    Result<void> renderHtmlInternal() {
        if (_htmlContent.empty()) {
            return Err<void>("YHtmlImpl::renderHtml: no content loaded");
        }

        auto result = renderHtmlToBuffer(
            _htmlContent, _buffer, _fontSize, _viewWidth, _fetcher.get());

        if (!result.document) {
            return Err<void>("YHtmlImpl::renderHtml: render failed");
        }

        _container = result.container;
        _document = result.document;

        // Wire up navigation callback for link clicks (deferred to avoid re-entrancy)
        _container->setNavigateCallback([this](const std::string& url) {
            _pendingNavigateUrl = url;
        });

        // Wire up cursor callback for hover feedback
        _container->setCursorCallback([this](const std::string& cursor) {
            auto loop = base::EventLoop::instance();
            if (!loop) return;
            int shape = 0;
            if (cursor == "pointer") {
                shape = 0x00036004;  // GLFW_POINTING_HAND_CURSOR
            }
            (*loop)->dispatch(base::Event::setCursorEvent(shape));
        });

        yinfo("YHtmlImpl::renderHtml: {}x{} prims={} glyphs={}",
              result.documentWidth, result.documentHeight,
              _builder->primitiveCount(), _builder->glyphCount());

        return Ok();
    }

    //=========================================================================
    // Members
    //=========================================================================

    // Builder + Buffer
    YDrawBuilder::Ptr _builder;
    YDrawBuffer::Ptr _buffer;
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;

    // GPU state
    StorageHandle _primStorage = StorageHandle::invalid();
    uint32_t _primCount = 0;
    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t _gridOffset = 0;
    uint32_t _glyphOffset = 0;
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
    TextureHandle _atlasTextureHandle = TextureHandle::invalid();
    bool _dirty = true;
    bool _metadataDirty = true;
    bool _needsBufferRealloc = false;
    bool _needsTextureRealloc = false;

    // View zoom/pan
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    bool _focused = false;
    float _cellWidth = 9.0f;
    float _cellHeight = 20.0f;

    // HTTP client
    std::shared_ptr<HttpFetcher> _fetcher;

    // HTML state
    std::string _htmlContent;
    std::string _inputSource;
    std::string _pendingNavigateUrl;
    bool _needsRedraw = false;
    std::shared_ptr<HtmlContainer> _container;
    std::shared_ptr<litehtml::document> _document;
    float _fontSize = 16.0f;
    float _viewWidth = 720.0f;
};

//=============================================================================
// Factory
//=============================================================================

Result<YHtml::Ptr> YHtml::createImpl(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("YHtml::create: pos=({},{}) size={}x{} payload_len={}",
          x, y, widthCells, heightCells, payload.size());

    auto card = std::make_shared<YHtmlImpl>(ctx, x, y, widthCells, heightCells,
                                             args, payload);

    if (auto res = card->init(); !res) {
        yerror("YHtml::create: init FAILED: {}", error_msg(res));
        return Err<Ptr>("YHtml::create: init failed");
    }

    yinfo("YHtml::create: SUCCESS");
    return Ok(std::move(card));
}

} // namespace yetty::card
