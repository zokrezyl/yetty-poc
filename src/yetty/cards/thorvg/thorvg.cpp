#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "thorvg.h"
#include "../../thorvg/thorvg-renderer.h"
#include "../../ydraw/ydraw-buffer.h"
#include <yetty/ydraw-builder.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <cmath>

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
}

namespace yetty::card {

//=============================================================================
// ThorVGImpl - full implementation with renderer, builder, GPU buffers
//=============================================================================

class ThorVGImpl : public ThorVG {
public:
    ThorVGImpl(const YettyContext& ctx,
               int32_t x, int32_t y,
               uint32_t widthCells, uint32_t heightCells,
               const std::string& args, const std::string& payload)
        : ThorVG(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _screenId(ctx.screenId)
        , _argsStr(args)
        , _payloadStr(payload)
        , _fontManager(ctx.fontManager)
        , _gpuAllocator(ctx.gpuAllocator)
    {
        _shaderGlyph = SHADER_GLYPH;
        _buffer = *YDrawBuffer::create();
    }

    ~ThorVGImpl() override { dispose(); }

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
                yerror("ThorVG::dispose: deallocateMetadata failed");
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {}

    void declareBufferNeeds() override {
        if (!_builder) return;
        if (auto res = _builder->declareBufferNeeds(); !res) {
            yerror("ThorVG::declareBufferNeeds: {}", error_msg(res));
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
        if (!_builder || !_renderer) return;

        // Animation update
        if (_renderer->isAnimated() && _playing) {
            float dt = (_lastRenderTime < 0.0f) ? 0.0f : (time - _lastRenderTime);
            _lastRenderTime = time;

            _animTime += dt;
            float duration = _renderer->duration();
            if (duration > 0) {
                if (_animTime >= duration) {
                    if (_loop) {
                        _animTime = std::fmod(_animTime, duration);
                    } else {
                        _animTime = duration;
                        _playing = false;
                    }
                }
                float frame = (_animTime / duration) * _renderer->totalFrames();
                _renderer->setFrame(frame);
                _dirty = true;
            }
        }

        // Re-render if dirty
        if (_dirty && _renderer) {
            if (auto res = _renderer->render(); !res) {
                yerror("ThorVG::renderToStaging: render failed");
            }
            _builder->calculate();

            // First-time zoom: fit content
            if (_needsInitialZoom && _cellWidth > 0 && _cellHeight > 0) {
                float contentW = _builder->sceneMaxX() - _builder->sceneMinX();
                float contentH = _builder->sceneMaxY() - _builder->sceneMinY();
                if (contentW > 0 && contentH > 0) {
                    float cardPixelW = static_cast<float>(_widthCells * _cellWidth);
                    float cardPixelH = static_cast<float>(_heightCells * _cellHeight);
                    float cardAspect = cardPixelW / std::max(cardPixelH, 1.0f);
                    _viewZoom = contentH * cardAspect / contentW;
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
    // Events
    //=========================================================================

    void setCellSize(float cellWidth, float cellHeight) override {
        _cellWidth = static_cast<uint32_t>(cellWidth);
        _cellHeight = static_cast<uint32_t>(cellHeight);
    }

    Result<bool> onEvent(const base::Event& event) override {
        if (!_builder) return Ok(false);

        if (event.type == base::Event::Type::SetFocus) {
            _focused = (event.setFocus.objectId == id());
            return Ok(false);
        }

        if (event.type == base::Event::Type::CardScroll &&
            event.cardScroll.targetId == id()) {
            float sceneW = _builder->sceneMaxX() - _builder->sceneMinX();
            float sceneH = _builder->sceneMaxY() - _builder->sceneMinY();

            if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
                float zoomDelta = event.cardScroll.dy * 0.1f * _viewZoom;
                _viewZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 100.0f);
            } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
                _viewPanX += event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
            } else {
                _viewPanY += event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
            }
            _builder->setView(_viewZoom, _viewPanX, _viewPanY);
            return Ok(true);
        }

        return Ok(false);
    }

    //=========================================================================
    // Update (OSC update command)
    //=========================================================================

    Result<void> update(const std::string& args, const std::string& payload) override {
        if (payload.empty()) return Ok();

        // Detect content type and reload
        const char* mimeType = nullptr;
        std::string content = payload;

        // Check for type prefix
        size_t newlinePos = payload.find('\n');
        if (newlinePos != std::string::npos && newlinePos < 20) {
            std::string prefix = payload.substr(0, newlinePos);
            if (prefix == "svg" || prefix == "lottie") {
                mimeType = prefix.c_str();
                content = payload.substr(newlinePos + 1);
            }
        }

        if (auto res = _renderer->load(content, mimeType); !res) {
            return Err<void>("ThorVG::update: load failed", res);
        }

        _needsInitialZoom = true;
        _animTime = 0.0f;
        _playing = _renderer->isAnimated();
        _dirty = true;

        return Ok();
    }

    //=========================================================================
    // Init
    //=========================================================================

    Result<void> init() {
        auto metaResult = _cardMgr->allocateMetadata(64);
        if (!metaResult) {
            return Err<void>("ThorVG::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        auto builderRes = YDrawBuilder::create(
            _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
        if (!builderRes) {
            return Err<void>("ThorVG::init: failed to create builder", builderRes);
        }
        _builder = *builderRes;
        _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);
        _builder->setViewport(_widthCells, _heightCells);

        // Create renderer
        auto rendererRes = yetty::thorvg::ThorVgRenderer::create(_buffer);
        if (!rendererRes) {
            return Err<void>("ThorVG::init: failed to create renderer", rendererRes);
        }
        _renderer = *rendererRes;

        // Parse args
        parseArgs(_argsStr);

        // Load payload
        if (!_payloadStr.empty()) {
            if (auto res = loadPayload(_payloadStr); !res) {
                ywarn("ThorVG::init: payload load failed: {}", error_msg(res));
            }
        }

        // Set scene bounds from content
        if (_renderer->contentWidth() > 0 && _renderer->contentHeight() > 0) {
            _builder->setSceneBounds(0, 0,
                _renderer->contentWidth(), _renderer->contentHeight());
        }

        // Render initial frame
        if (auto res = _renderer->render(); res) {
            _builder->calculate();
        }

        // Start animation if animated
        _playing = _renderer->isAnimated();
        _lastRenderTime = -1.0f;

        // Register for events
        if (auto res = registerForEvents(); !res) {
            ywarn("ThorVG::init: event registration failed");
        }

        yinfo("ThorVG::init: {}x{} prims={} animated={}",
              _renderer->contentWidth(), _renderer->contentHeight(),
              _buffer->primCount(), _renderer->isAnimated());

        return Ok();
    }

private:
    //=========================================================================
    // Helpers
    //=========================================================================

    void parseArgs(const std::string& args) {
        std::istringstream iss(args);
        std::string token;
        while (iss >> token) {
            if (token == "--no-loop") {
                _loop = false;
            } else if (token == "--paused") {
                _playing = false;
            } else if (token == "--zoom") {
                std::string s;
                if (iss >> s) _viewZoom = std::clamp(std::stof(s), 0.1f, 100.0f);
            }
        }
    }

    Result<void> loadPayload(const std::string& payload) {
        const char* mimeType = nullptr;
        std::string content = payload;

        // Check for type prefix (format: "type\n<content>")
        size_t newlinePos = payload.find('\n');
        if (newlinePos != std::string::npos && newlinePos < 20) {
            std::string prefix = payload.substr(0, newlinePos);
            if (prefix == "svg" || prefix == "lottie") {
                mimeType = prefix.c_str();
                content = payload.substr(newlinePos + 1);
            }
        }

        return _renderer->load(content, mimeType);
    }

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return Err<void>("no EventLoop", loopResult);
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();
        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) return res;
        return Ok();
    }

    void deregisterFromEvents() {
        if (weak_from_this().expired()) return;
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        (*loopResult)->deregisterListener(sharedAs<base::EventListener>());
    }

    //=========================================================================
    // Members
    //=========================================================================

    yetty::thorvg::ThorVgRenderer::Ptr _renderer;
    YDrawBuilder::Ptr _builder;
    YDrawBuffer::Ptr _buffer;
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;

    // View
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    bool _focused = false;
    bool _needsInitialZoom = true;

    // Cell dimensions
    uint32_t _cellWidth = 10;
    uint32_t _cellHeight = 20;

    // Animation
    bool _playing = false;
    bool _loop = true;
    float _animTime = 0.0f;
    float _lastRenderTime = -1.0f;

    bool _dirty = true;
};

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> ThorVG::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    if (!ctx.cardManager) {
        return Err<CardPtr>("ThorVG::create: null CardManager");
    }
    auto card = std::make_shared<ThorVGImpl>(ctx, x, y, widthCells, heightCells, args, payload);
    if (auto res = card->init(); !res) {
        return Err<CardPtr>("ThorVG::create: init failed", res);
    }
    return Ok<CardPtr>(card);
}

Result<ThorVG::Ptr> ThorVG::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;
    auto result = create(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (!result) return Err<Ptr>("Failed to create ThorVG", result);
    auto card = std::dynamic_pointer_cast<ThorVG>(*result);
    if (!card) return Err<Ptr>("Created card is not ThorVG");
    return Ok(card);
}

} // namespace yetty::card
