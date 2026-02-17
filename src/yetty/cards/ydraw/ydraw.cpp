#include "ydraw.h"
#include <yetty/ydraw-builder.h>
#include "../../ydraw/animation.h"
#include "../../ydraw/yaml2ydraw.h"
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
constexpr int GLFW_KEY_C       = 67;
}

namespace yetty::card {

//=============================================================================
// YDrawImpl - full implementation with builder, GPU buffers, animation
//=============================================================================

class YDrawImpl : public YDraw {
public:
    YDrawImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : YDraw(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _screenId(ctx.screenId)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
        _buffer = *YDrawBuffer::create();
        _fontManager = ctx.fontManager;
        _gpuAllocator = ctx.gpuAllocator;
    }

    ~YDrawImpl() override { dispose(); }

    //=========================================================================
    // Card lifecycle
    //=========================================================================

    bool needsTexture() const override {
        return _builder && _builder->hasCustomAtlas();
    }

    uint32_t metadataSlotIndex() const override {
        return _metaHandle.offset / 64;
    }

    Result<void> dispose() override {
        deregisterFromEvents();
        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("YDrawImpl::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {
    }

    void declareBufferNeeds() override {
        if (!_builder) return;
        if (auto res = _builder->declareBufferNeeds(); !res) {
            yerror("YDrawImpl::declareBufferNeeds: {}", error_msg(res));
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

    bool needsBufferRealloc() override {
        return _builder && _builder->needsBufferRealloc();
    }

    bool needsTextureRealloc() override {
        return _builder && _builder->needsTextureRealloc();
    }

    void renderToStaging(float time) override {
        if (!_builder) return;

        // Auto-start animation
        if (_animation && _animation->hasProperties() && !_animation->isPlaying()
            && !_animationStarted && !_buffer->empty()) {
            startAnimation();
        }

        // Animation update — advance time, apply to buffer
        if (_animation && _animation->isPlaying() && !_buffer->empty()) {
            float dt = (_lastRenderTime < 0.0f) ? 0.0f : (time - _lastRenderTime);
            _lastRenderTime = time;
            if (_animation->advance(dt)) {
                _animation->apply();
                _dirty = true;
            }
        }

        // If dirty, recalculate grid
        if (_dirty && !_buffer->empty()) {
            _builder->calculate();

            // First-time zoom: fit content width to card width (like ypdf)
            if (_needsInitialZoom && _cellWidth > 0 && _cellHeight > 0) {
                float contentW = _builder->sceneMaxX() - _builder->sceneMinX();
                float contentH = _builder->sceneMaxY() - _builder->sceneMinY();
                if (contentW > 0 && contentH > 0) {
                    float cardAspect = static_cast<float>(_widthCells * _cellWidth) /
                                       std::max(static_cast<float>(_heightCells * _cellHeight), 1.0f);
                    // With UNIFORM_SCALE: zoom to fit content width to card width
                    _viewZoom = contentH * cardAspect / contentW;

                    // Pan to show top of document
                    float centerY = (_builder->sceneMinY() + _builder->sceneMaxY()) * 0.5f;
                    float viewHalfH = contentH * 0.5f / _viewZoom;
                    _viewPanY = _builder->sceneMinY() - centerY + viewHalfH;

                    _builder->setView(_viewZoom, _viewPanX, _viewPanY);
                    _needsInitialZoom = false;
                }
            }

            _dirty = false;
        }
    }

    Result<void> finalize() override {
        if (!_builder) return Ok();
        return _builder->writeBuffers();
    }

    //=========================================================================
    // Events - zoom/pan + text selection
    //=========================================================================

    void setCellSize(float cellWidth, float cellHeight) override {
        _cellWidth = cellWidth;
        _cellHeight = cellHeight;
    }

    void cardPixelToScene(float cardX, float cardY, float& sceneX, float& sceneY) {
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

    Result<bool> onEvent(const base::Event& event) override {
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
                _viewZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 1000.0f);
            } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
                _viewPanX += event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
            } else {
                _viewPanY += event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
            }
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            return Ok(true);
        }

        // Mouse selection
        if (event.type == base::Event::Type::CardMouseDown &&
            event.cardMouse.targetId == id() && event.cardMouse.button == 0) {
            if (_builder->glyphCount() == 0) return Ok(false);
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
                yinfo("YDraw: copied {} bytes to clipboard", text.size());
            }
            return Ok(true);
        }

        return Ok(false);
    }

    //=========================================================================
    // Init
    //=========================================================================

    Result<void> init() {
        auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
        if (!metaResult) {
            return Err<void>("YDrawImpl::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        auto builderRes = YDrawBuilder::create(
            _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
        if (!builderRes) {
            return Err<void>("YDrawImpl::init: failed to create builder", builderRes);
        }
        _builder = *builderRes;

        _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);

        if (auto res = registerForEvents(); !res) {
            ywarn("YDrawImpl::init: event registration failed: {}", error_msg(res));
        }

        parseArgs(_argsStr);

        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("YDrawImpl::init: payload parse failed: {}", error_msg(res));
            }
        }

        yinfo("YDrawImpl::init: {} prims, {} glyphs",
              _builder->primitiveCount(), _builder->glyphCount());

        _builder->setViewport(_widthCells, _heightCells);
        _builder->setView(_viewZoom, _viewPanX, _viewPanY);
        _dirty = true;
        return Ok();
    }

private:
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
    // Animation
    //=========================================================================

    animation::Animation* anim() {
        if (!_animation) {
            auto res = animation::Animation::create(_buffer);
            if (res) _animation = *res;
        }
        return _animation.get();
    }

    void startAnimation() {
        if (!_animation || !_animation->hasProperties()) return;
        if (_buffer->empty()) return;

        _animation->snapshotBase();
        _animationStarted = true;
        _lastRenderTime = -1.0f;

        if (!_builder->hasExplicitBounds()) {
            float minX = _builder->sceneMinX();
            float minY = _builder->sceneMinY();
            float maxX = _builder->sceneMaxX();
            float maxY = _builder->sceneMaxY();
            _animation->expandBounds(minX, minY, maxX, maxY);
            float padX = (maxX - minX) * 0.05f;
            float padY = (maxY - minY) * 0.05f;
            _builder->setSceneBounds(minX - padX, minY - padY,
                                     maxX + padX, maxY + padY);
        }
        _animation->play();
    }

    //=========================================================================
    // Parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        std::istringstream iss(args);
        std::string token;
        while (iss >> token) {
            if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X")
                        colorStr = colorStr.substr(2);
                    _buffer->setBgColor(static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16)));
                }
            } else if (token == "--cell-size") {
                std::string s; if (iss >> s) _builder->setGridCellSize(std::stof(s));
            } else if (token == "--max-prims-per-cell") {
                std::string s; if (iss >> s) _builder->setMaxPrimsPerCell(static_cast<uint32_t>(std::stoul(s)));
            } else if (token == "--show-bounds") {
                _buffer->addFlags(YDrawBuffer::FLAG_SHOW_BOUNDS);
            } else if (token == "--show-grid") {
                _buffer->addFlags(YDrawBuffer::FLAG_SHOW_GRID);
            } else if (token == "--show-eval-count") {
                _buffer->addFlags(YDrawBuffer::FLAG_SHOW_EVAL_COUNT);
            } else if (token == "--zoom") {
                std::string s;
                if (iss >> s) _viewZoom = std::clamp(std::stof(s), 0.1f, 1000.0f);
            } else if (token == "--pan-y") {
                std::string s;
                if (iss >> s) _viewPanY = std::stof(s);
            } else if (token == "--yaml") {
                _yamlMode = true;
            }
        }
    }

    Result<void> parsePayload(const std::string& payload) {
        if (payload.empty()) return Ok();
        if (_yamlMode) {
            auto res = parseYDrawYAML(_buffer, payload);
            if (!res) return Err<void>(res.error().message());
            if (*res) _animation = *res;
            return Ok();
        }
        return parseBinary(payload);
    }

    Result<void> parseBinary(const std::string& payload) {
        ydebug("parseBinary: payload size={}", payload.size());
        if (payload.empty()) return Ok();
        if (payload.size() >= 8) {
            ydebug("parseBinary: first 8 bytes: {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X}",
                   (uint8_t)payload[0], (uint8_t)payload[1], (uint8_t)payload[2], (uint8_t)payload[3],
                   (uint8_t)payload[4], (uint8_t)payload[5], (uint8_t)payload[6], (uint8_t)payload[7]);
        }
        auto res = _buffer->deserialize(
            reinterpret_cast<const uint8_t*>(payload.data()), payload.size());
        if (!res) {
            yerror("parseBinary: deserialize failed: {}", error_msg(res));
        }
        return res;
    }

    // Old parseYAML/parseYAMLPrimitive/parseColor removed — now in yaml2ydraw.cpp
    //=========================================================================
    // State
    //=========================================================================

    YDrawBuilder::Ptr _builder;
    YDrawBuffer::Ptr _buffer;
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;

    bool _dirty = true;

    // View
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    bool _focused = false;
    bool _needsInitialZoom = true;

    // Cell pixel size (from setCellSize)
    uint32_t _cellWidth = 0;
    uint32_t _cellHeight = 0;

    // Text selection
    bool _selecting = false;
    int32_t _selStartSorted = -1;
    int32_t _selEndSorted = -1;
    bool _sortedOrderBuilt = false;

    // Animation
    animation::Animation::Ptr _animation;
    bool _animationStarted = false;
    float _lastRenderTime = -1.0f;

    // Payload format
    bool _yamlMode = false;
};

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> YDraw::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    if (!ctx.cardManager) {
        return Err<CardPtr>("YDraw::create: null CardManager");
    }
    auto card = std::make_shared<YDrawImpl>(ctx, x, y, widthCells, heightCells, args, payload);
    if (auto res = card->init(); !res) {
        return Err<CardPtr>("YDraw::create: init failed");
    }
    return Ok<CardPtr>(card);
}

Result<YDraw::Ptr> YDraw::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;
    auto result = create(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (!result) return Err<Ptr>("Failed to create YDraw", result);
    auto ydraw = std::dynamic_pointer_cast<YDraw>(*result);
    if (!ydraw) return Err<Ptr>("Created card is not a YDraw");
    return Ok(ydraw);
}

} // namespace yetty::card
