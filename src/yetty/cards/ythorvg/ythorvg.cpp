#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "ythorvg.h"
#include "../../ythorvg/ydraw-render-method.h"
#include "../../ydraw/ydraw-buffer.h"
#include <yetty/ydraw-builder.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

// ThorVG internal headers
#include "tvgPaint.h"
#include "tvgRender.h"

#include <thorvg.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <mutex>

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
}

namespace yetty::card {

//=============================================================================
// ThorVG engine reference counting (shared with thorvg card)
//=============================================================================

static int s_thorvgRefCount = 0;
static std::mutex s_thorvgMutex;

static Result<void> ythorvgInit() {
    std::lock_guard<std::mutex> lock(s_thorvgMutex);
    if (s_thorvgRefCount == 0) {
        auto result = tvg::Initializer::init(0);
        if (result != tvg::Result::Success) {
            return Err<void>("YThorVG: tvg::Initializer::init failed");
        }
        uint32_t major, minor, micro;
        const char* version = tvg::Initializer::version(&major, &minor, &micro);
        yinfo("YThorVG engine initialized: {}", version ? version : "unknown");
    }
    ++s_thorvgRefCount;
    return Ok();
}

static void ythorvgTerm() {
    std::lock_guard<std::mutex> lock(s_thorvgMutex);
    if (--s_thorvgRefCount == 0) {
        tvg::Initializer::term();
        yinfo("YThorVG engine terminated");
    }
}

//=============================================================================
// YThorVGImpl - full implementation with RenderMethod
//=============================================================================

class YThorVGImpl : public YThorVG {
public:
    YThorVGImpl(const YettyContext& ctx,
                int32_t x, int32_t y,
                uint32_t widthCells, uint32_t heightCells,
                const std::string& args, const std::string& payload)
        : YThorVG(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _screenId(ctx.screenId)
        , _argsStr(args)
        , _payloadStr(payload)
        , _fontManager(ctx.fontManager)
        , _gpuAllocator(ctx.gpuAllocator)
    {
        _shaderGlyph = SHADER_GLYPH;
        _buffer = *YDrawBuffer::create();
    }

    ~YThorVGImpl() override { dispose(); }

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
                yerror("YThorVG::dispose: deallocateMetadata failed");
            }
            _metaHandle = MetadataHandle::invalid();
        }

        // Clean up ThorVG resources
        _animation.reset();
        _picture = nullptr;
        ythorvgTerm();

        return Ok();
    }

    void suspend() override {}

    void declareBufferNeeds() override {
        if (!_builder) return;
        if (auto res = _builder->declareBufferNeeds(); !res) {
            yerror("YThorVG::declareBufferNeeds: {}", error_msg(res));
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
        if (!_builder || !_picture || !_renderMethod) return;

        // Animation update
        if (_isAnimated && _playing) {
            float dt = (_lastRenderTime < 0.0f) ? 0.0f : (time - _lastRenderTime);
            _lastRenderTime = time;

            _animTime += dt;
            if (_duration > 0) {
                if (_animTime >= _duration) {
                    if (_loop) {
                        _animTime = std::fmod(_animTime, _duration);
                    } else {
                        _animTime = _duration;
                        _playing = false;
                    }
                }
                float frame = (_animTime / _duration) * _totalFrames;
                _animation->frame(frame);
                _dirty = true;
            }
        }

        // Re-render if dirty
        if (_dirty && _picture) {
            renderFrame();

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

        const char* mimeType = nullptr;
        std::string content = payload;

        size_t newlinePos = payload.find('\n');
        if (newlinePos != std::string::npos && newlinePos < 20) {
            std::string prefix = payload.substr(0, newlinePos);
            if (prefix == "svg" || prefix == "lottie") {
                mimeType = prefix.c_str();
                content = payload.substr(newlinePos + 1);
            }
        }

        if (auto res = loadContent(content, mimeType); !res) {
            return Err<void>("YThorVG::update: load failed", res);
        }

        _needsInitialZoom = true;
        _animTime = 0.0f;
        _playing = _isAnimated;
        _dirty = true;

        return Ok();
    }

    //=========================================================================
    // Init
    //=========================================================================

    Result<void> init() {
        // Initialize ThorVG
        if (auto res = ythorvgInit(); !res) {
            return Err<void>("YThorVG::init: failed to initialize ThorVG", res);
        }

        // Allocate metadata
        auto metaResult = _cardMgr->allocateMetadata(64);
        if (!metaResult) {
            return Err<void>("YThorVG::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        // Create YDraw builder
        auto builderRes = YDrawBuilder::create(
            _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
        if (!builderRes) {
            return Err<void>("YThorVG::init: failed to create builder", builderRes);
        }
        _builder = *builderRes;
        _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);
        _builder->setViewport(_widthCells, _heightCells);
        _builder->setView(_viewZoom, _viewPanX, _viewPanY);

        // Create our custom RenderMethod
        _renderMethod = std::make_unique<ythorvg::YDrawRenderMethod>(_buffer);
        _renderMethod->setTarget(_widthCells * 10, _heightCells * 20);  // approx pixel size

        // Parse args and load content
        parseArgs(_argsStr);

        if (auto res = loadContentFromArgs(); !res) {
            ywarn("YThorVG::init: content load failed: {}", error_msg(res));
        }

        // Set scene bounds
        if (_contentWidth > 0 && _contentHeight > 0) {
            _builder->setSceneBounds(0, 0, _contentWidth, _contentHeight);
        }

        // Render initial frame
        renderFrame();

        // Start animation if animated
        _playing = _isAnimated;
        _lastRenderTime = -1.0f;

        // Register for events
        if (auto res = registerForEvents(); !res) {
            ywarn("YThorVG::init: event registration failed");
        }

        yinfo("YThorVG::init: {}x{} prims={} animated={}",
              _contentWidth, _contentHeight, _buffer->primCount(), _isAnimated);

        return Ok();
    }

private:
    //=========================================================================
    // Render frame using RenderMethod
    //=========================================================================

    void renderFrame() {
        if (!_picture || !_renderMethod) return;

        // Use ThorVG's internal Paint interface to update and render
        auto* paintImpl = PAINT(_picture);
        if (!paintImpl) return;

        // preRender clears the buffer
        _renderMethod->preRender();

        // Update paint tree with our renderer
        tvg::Array<tvg::RenderData> clips;
        auto identity = tvg::Matrix{1, 0, 0, 0, 1, 0, 0, 0, 1};
        paintImpl->update(_renderMethod.get(), identity, clips, 255, tvg::RenderUpdateFlag::All);

        // Render paint tree
        paintImpl->render(_renderMethod.get());

        // postRender (we don't do much here, builder->calculate() is separate)
        _renderMethod->postRender();

        // Calculate grid
        _builder->calculate();
    }

    //=========================================================================
    // Helpers
    //=========================================================================

    void parseArgs(const std::string& args) {
        std::istringstream iss(args);
        std::string token;
        while (iss >> token) {
            if (token == "-i" || token == "--input") {
                std::string val;
                if (iss >> val) _inputSource = val;
            } else if (token == "--svg") {
                _contentType = "svg";
            } else if (token == "--lottie") {
                _contentType = "lottie";
            } else if (token == "--no-loop") {
                _loop = false;
            } else if (token == "--paused") {
                _playing = false;
            } else if (token == "--zoom") {
                std::string s;
                if (iss >> s) _viewZoom = std::clamp(std::stof(s), 0.1f, 100.0f);
            }
        }
    }

    Result<void> loadContentFromArgs() {
        std::string content;

        if (_inputSource == "-") {
            content = _payloadStr;
        } else if (!_inputSource.empty()) {
            std::ifstream file(_inputSource);
            if (file) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                content = buffer.str();
            } else {
                return Err<void>("YThorVG: failed to open file: " + _inputSource);
            }
        } else if (!_payloadStr.empty()) {
            content = _payloadStr;
        } else {
            return Err<void>("YThorVG: no input specified");
        }

        if (content.empty()) {
            return Err<void>("YThorVG: empty content");
        }

        const char* mimeType = nullptr;
        if (!_contentType.empty()) {
            mimeType = _contentType.c_str();
        }

        return loadContent(content, mimeType);
    }

    Result<void> loadContent(const std::string& content, const char* mimeType) {
        // Auto-detect content type
        std::string detectedType;
        if (!mimeType) {
            if (content.find("<svg") != std::string::npos ||
                content.find("<?xml") != std::string::npos) {
                detectedType = "svg";
            } else if (content.find("\"v\"") != std::string::npos &&
                       content.find("\"layers\"") != std::string::npos) {
                detectedType = "lottie";
            }
            mimeType = detectedType.empty() ? nullptr : detectedType.c_str();
        }

        // Create Animation wrapper
        _animation.reset(tvg::Animation::gen());
        if (!_animation) {
            return Err<void>("YThorVG: failed to create Animation");
        }

        _picture = _animation->picture();
        if (!_picture) {
            _animation.reset();
            return Err<void>("YThorVG: failed to get picture from Animation");
        }

        // Load content
        tvg::Result result;
        if (mimeType && std::string(mimeType) == "lottie") {
            result = _picture->load(content.c_str(), static_cast<uint32_t>(content.size()),
                                    "lottie", nullptr, true);
            if (result == tvg::Result::Success) {
                _totalFrames = _animation->totalFrame();
                _duration = _animation->duration();
                _isAnimated = _totalFrames > 1;
            }
        } else {
            result = _picture->load(content.c_str(), static_cast<uint32_t>(content.size()),
                                    mimeType, nullptr, true);
            _isAnimated = false;
        }

        if (result != tvg::Result::Success) {
            _animation.reset();
            _picture = nullptr;
            return Err<void>("YThorVG: failed to load content");
        }

        // Get dimensions
        float w = 0, h = 0;
        _picture->size(&w, &h);
        _contentWidth = w > 0 ? w : 256;
        _contentHeight = h > 0 ? h : 256;

        return Ok();
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

    std::unique_ptr<ythorvg::YDrawRenderMethod> _renderMethod;
    std::unique_ptr<tvg::Animation> _animation;
    tvg::Picture* _picture = nullptr;
    YDrawBuilder::Ptr _builder;
    YDrawBuffer::Ptr _buffer;
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;
    std::string _inputSource;
    std::string _contentType;

    // Content info
    float _contentWidth = 0;
    float _contentHeight = 0;
    bool _isAnimated = false;
    float _totalFrames = 0;
    float _duration = 0;

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

Result<CardPtr> YThorVG::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    if (!ctx.cardManager) {
        return Err<CardPtr>("YThorVG::create: null CardManager");
    }
    auto card = std::make_shared<YThorVGImpl>(ctx, x, y, widthCells, heightCells, args, payload);
    if (auto res = card->init(); !res) {
        return Err<CardPtr>("YThorVG::create: init failed", res);
    }
    return Ok<CardPtr>(card);
}

Result<YThorVG::Ptr> YThorVG::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;
    auto result = create(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (!result) return Err<Ptr>("Failed to create YThorVG", result);
    auto card = std::dynamic_pointer_cast<YThorVG>(*result);
    if (!card) return Err<Ptr>("Created card is not YThorVG");
    return Ok(card);
}

} // namespace yetty::card
