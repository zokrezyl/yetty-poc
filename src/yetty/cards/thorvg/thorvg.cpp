#include "thorvg.h"
#include <yetty/yetty-context.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <thorvg.h>
#include <yaml-cpp/yaml.h>

#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>

// GLFW modifier constants
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

namespace yetty::card {

//=============================================================================
// ThorVG engine reference counting - shared across all ThorVG card instances
//=============================================================================

static int s_thorvgRefCount = 0;

static Result<void> thorvgEngineRef() {
    if (s_thorvgRefCount == 0) {
        auto result = tvg::Initializer::init(0);
        if (result != tvg::Result::Success) {
            return Err<void>("ThorVG::init: tvg::Initializer::init failed");
        }
        uint32_t major, minor, micro;
        const char* version = tvg::Initializer::version(&major, &minor, &micro);
        yinfo("ThorVG engine initialized: {}", version ? version : "unknown");
    }
    ++s_thorvgRefCount;
    return Ok();
}

static void thorvgEngineUnref() {
    if (--s_thorvgRefCount == 0) {
        tvg::Initializer::term();
        yinfo("ThorVG engine terminated");
    }
}

//=============================================================================
// YAML to SVG conversion
//=============================================================================

static Result<std::string> yamlToSvg(const std::string& yamlContent) {
    try {
        YAML::Node root = YAML::Load(yamlContent);

        int width = 800;
        int height = 600;
        std::string bgColor = "none";

        if (root["canvas"]) {
            if (root["canvas"]["width"]) width = root["canvas"]["width"].as<int>();
            if (root["canvas"]["height"]) height = root["canvas"]["height"].as<int>();
            if (root["canvas"]["background"]) bgColor = root["canvas"]["background"].as<std::string>();
        }

        std::ostringstream svg;
        svg << R"(<?xml version="1.0" encoding="UTF-8"?>)" << "\n";
        svg << R"(<svg xmlns="http://www.w3.org/2000/svg" width=")" << width
            << R"(" height=")" << height << R"(" viewBox="0 0 )" << width << " " << height << R"(">)" << "\n";

        if (bgColor != "none") {
            svg << R"(  <rect width="100%" height="100%" fill=")" << bgColor << R"("/>)" << "\n";
        }

        YAML::Node shapes;
        if (root["body"]) {
            shapes = root["body"];
        } else if (root["shapes"]) {
            shapes = root["shapes"];
        } else if (root.IsSequence()) {
            shapes = root;
        }

        if (shapes && shapes.IsSequence()) {
            for (const auto& shape : shapes) {
                if (shape["circle"]) {
                    auto c = shape["circle"];
                    float cx = c["cx"] ? c["cx"].as<float>() : (c["x"] ? c["x"].as<float>() : 0);
                    float cy = c["cy"] ? c["cy"].as<float>() : (c["y"] ? c["y"].as<float>() : 0);
                    float r = c["r"] ? c["r"].as<float>() : (c["radius"] ? c["radius"].as<float>() : 10);
                    std::string fill = c["fill"] ? c["fill"].as<std::string>() : "#000000";
                    std::string stroke = c["stroke"] ? c["stroke"].as<std::string>() : "none";
                    float strokeWidth = c["stroke-width"] ? c["stroke-width"].as<float>() : 1;
                    float opacity = c["opacity"] ? c["opacity"].as<float>() : 1.0f;

                    svg << R"(  <circle cx=")" << cx << R"(" cy=")" << cy
                        << R"(" r=")" << r << R"(" fill=")" << fill
                        << R"(" stroke=")" << stroke << R"(" stroke-width=")" << strokeWidth
                        << R"(" opacity=")" << opacity << R"("/>)" << "\n";
                }
                else if (shape["rect"]) {
                    auto c = shape["rect"];
                    float x = c["x"] ? c["x"].as<float>() : 0;
                    float y = c["y"] ? c["y"].as<float>() : 0;
                    float w = c["width"] ? c["width"].as<float>() : (c["w"] ? c["w"].as<float>() : 50);
                    float h = c["height"] ? c["height"].as<float>() : (c["h"] ? c["h"].as<float>() : 50);
                    float rx = c["rx"] ? c["rx"].as<float>() : (c["round"] ? c["round"].as<float>() : 0);
                    float ry = c["ry"] ? c["ry"].as<float>() : rx;
                    std::string fill = c["fill"] ? c["fill"].as<std::string>() : "#000000";
                    std::string stroke = c["stroke"] ? c["stroke"].as<std::string>() : "none";
                    float strokeWidth = c["stroke-width"] ? c["stroke-width"].as<float>() : 1;
                    float opacity = c["opacity"] ? c["opacity"].as<float>() : 1.0f;

                    svg << R"(  <rect x=")" << x << R"(" y=")" << y
                        << R"(" width=")" << w << R"(" height=")" << h
                        << R"(" rx=")" << rx << R"(" ry=")" << ry
                        << R"(" fill=")" << fill << R"(" stroke=")" << stroke
                        << R"(" stroke-width=")" << strokeWidth
                        << R"(" opacity=")" << opacity << R"("/>)" << "\n";
                }
                else if (shape["line"]) {
                    auto c = shape["line"];
                    float x1 = c["x1"] ? c["x1"].as<float>() : 0;
                    float y1 = c["y1"] ? c["y1"].as<float>() : 0;
                    float x2 = c["x2"] ? c["x2"].as<float>() : 100;
                    float y2 = c["y2"] ? c["y2"].as<float>() : 100;
                    std::string stroke = c["stroke"] ? c["stroke"].as<std::string>() : "#000000";
                    float strokeWidth = c["stroke-width"] ? c["stroke-width"].as<float>() : 1;
                    float opacity = c["opacity"] ? c["opacity"].as<float>() : 1.0f;

                    svg << R"(  <line x1=")" << x1 << R"(" y1=")" << y1
                        << R"(" x2=")" << x2 << R"(" y2=")" << y2
                        << R"(" stroke=")" << stroke << R"(" stroke-width=")" << strokeWidth
                        << R"(" opacity=")" << opacity << R"("/>)" << "\n";
                }
                else if (shape["ellipse"]) {
                    auto c = shape["ellipse"];
                    float cx = c["cx"] ? c["cx"].as<float>() : (c["x"] ? c["x"].as<float>() : 0);
                    float cy = c["cy"] ? c["cy"].as<float>() : (c["y"] ? c["y"].as<float>() : 0);
                    float rx = c["rx"] ? c["rx"].as<float>() : 20;
                    float ry = c["ry"] ? c["ry"].as<float>() : 10;
                    std::string fill = c["fill"] ? c["fill"].as<std::string>() : "#000000";
                    std::string stroke = c["stroke"] ? c["stroke"].as<std::string>() : "none";
                    float strokeWidth = c["stroke-width"] ? c["stroke-width"].as<float>() : 1;
                    float opacity = c["opacity"] ? c["opacity"].as<float>() : 1.0f;

                    svg << R"(  <ellipse cx=")" << cx << R"(" cy=")" << cy
                        << R"(" rx=")" << rx << R"(" ry=")" << ry
                        << R"(" fill=")" << fill << R"(" stroke=")" << stroke
                        << R"(" stroke-width=")" << strokeWidth
                        << R"(" opacity=")" << opacity << R"("/>)" << "\n";
                }
                else if (shape["polygon"]) {
                    auto c = shape["polygon"];
                    std::string points;
                    if (c["points"]) {
                        if (c["points"].IsSequence()) {
                            for (size_t i = 0; i < c["points"].size(); i += 2) {
                                if (i > 0) points += " ";
                                points += std::to_string(c["points"][i].as<float>()) + "," +
                                         std::to_string(c["points"][i+1].as<float>());
                            }
                        } else {
                            points = c["points"].as<std::string>();
                        }
                    }
                    std::string fill = c["fill"] ? c["fill"].as<std::string>() : "#000000";
                    std::string stroke = c["stroke"] ? c["stroke"].as<std::string>() : "none";
                    float strokeWidth = c["stroke-width"] ? c["stroke-width"].as<float>() : 1;
                    float opacity = c["opacity"] ? c["opacity"].as<float>() : 1.0f;

                    svg << R"(  <polygon points=")" << points
                        << R"(" fill=")" << fill << R"(" stroke=")" << stroke
                        << R"(" stroke-width=")" << strokeWidth
                        << R"(" opacity=")" << opacity << R"("/>)" << "\n";
                }
                else if (shape["path"]) {
                    auto c = shape["path"];
                    std::string d = c["d"] ? c["d"].as<std::string>() : "";
                    std::string fill = c["fill"] ? c["fill"].as<std::string>() : "none";
                    std::string stroke = c["stroke"] ? c["stroke"].as<std::string>() : "#000000";
                    float strokeWidth = c["stroke-width"] ? c["stroke-width"].as<float>() : 1;
                    float opacity = c["opacity"] ? c["opacity"].as<float>() : 1.0f;

                    svg << R"(  <path d=")" << d
                        << R"(" fill=")" << fill << R"(" stroke=")" << stroke
                        << R"(" stroke-width=")" << strokeWidth
                        << R"(" opacity=")" << opacity << R"("/>)" << "\n";
                }
                else if (shape["text"]) {
                    auto c = shape["text"];
                    float x = c["x"] ? c["x"].as<float>() : 0;
                    float y = c["y"] ? c["y"].as<float>() : 0;
                    std::string content = c["content"] ? c["content"].as<std::string>() : "";
                    std::string fill = c["fill"] ? c["fill"].as<std::string>() : "#000000";
                    float fontSize = c["font-size"] ? c["font-size"].as<float>() : 16;
                    std::string fontFamily = c["font-family"] ? c["font-family"].as<std::string>() : "sans-serif";
                    std::string anchor = c["text-anchor"] ? c["text-anchor"].as<std::string>() : "start";
                    float opacity = c["opacity"] ? c["opacity"].as<float>() : 1.0f;

                    svg << R"(  <text x=")" << x << R"(" y=")" << y
                        << R"(" fill=")" << fill << R"(" font-size=")" << fontSize
                        << R"(" font-family=")" << fontFamily << R"(" text-anchor=")" << anchor
                        << R"(" opacity=")" << opacity << R"(">)" << content << R"(</text>)" << "\n";
                }
                else if (shape["group"] || shape["g"]) {
                    auto c = shape["group"] ? shape["group"] : shape["g"];
                    std::string transform = c["transform"] ? c["transform"].as<std::string>() : "";
                    float opacity = c["opacity"] ? c["opacity"].as<float>() : 1.0f;

                    svg << R"(  <g)";
                    if (!transform.empty()) svg << R"( transform=")" << transform << R"(")";
                    if (opacity < 1.0f) svg << R"( opacity=")" << opacity << R"(")";
                    svg << R"(>)" << "\n";
                    svg << R"(  </g>)" << "\n";
                }
            }
        }

        svg << R"(</svg>)" << "\n";
        return Ok(svg.str());

    } catch (const YAML::Exception& e) {
        return Err<std::string>(std::string("YAML parse error: ") + e.what());
    } catch (const std::exception& e) {
        return Err<std::string>(std::string("Error converting YAML to SVG: ") + e.what());
    }
}

//=============================================================================
// ThorVGImpl - Implementation of ThorVG card
//=============================================================================

class ThorVGImpl : public ThorVG {
public:
    ThorVGImpl(const YettyContext& ctx,
               int32_t x, int32_t y,
               uint32_t widthCells, uint32_t heightCells,
               const std::string& args, const std::string& payload)
        : ThorVG(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _ctx(ctx)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
        ydebug("ThorVGImpl::ctor: x={} y={} w={} h={} args='{}' payload_size={}",
               x, y, widthCells, heightCells, args, payload.size());
    }

    ~ThorVGImpl() override {
        dispose();
    }

    const char* typeName() const override { return "thorvg"; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    void setCellSize(uint32_t cellWidth, uint32_t cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            recalculateDimensions();
            _needsRerender = true;
        }
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() {
        ydebug("ThorVG::init: BEGIN payload_size={}", _payloadStr.size());

        // Initialize ThorVG engine (ref-counted)
        ydebug("ThorVG::init: initializing engine (refcount={})", s_thorvgRefCount);
        if (auto res = thorvgEngineRef(); !res) {
            return Err<void>("ThorVG::init: engine init failed", res);
        }
        _engineInitialized = true;
        ydebug("ThorVG::init: engine initialized OK (refcount={})", s_thorvgRefCount);

        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("ThorVG::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;
        ydebug("ThorVG::init: allocated metadata at offset={} size={}", _metaHandle.offset, _metaHandle.size);

        // Parse args
        parseArgs(_argsStr);

        // Load content from payload
        if (!_payloadStr.empty()) {
            ydebug("ThorVG::init: loading content from payload ({} bytes)", _payloadStr.size());
            if (auto res = loadContent(); !res) {
                yerror("ThorVG::init: failed to load content: {}", error_msg(res));
                return Err<void>("ThorVG::init: failed to load content", res);
            }
            ydebug("ThorVG::init: content loaded OK, animation={}", (void*)_animation.get());
        } else {
            ydebug("ThorVG::init: WARNING - payload is empty!");
        }

        // Initial render will happen via allocateTextures() in the lifecycle.
        // setCellSize() sets _needsRerender=true, allocateTextures() picks it up.
        _needsRerender = true;
        _metadataDirty = true;

        // Register for events
        if (auto res = registerForEvents(); !res) {
            return Err<void>("ThorVG::init: failed to register for events", res);
        }

        ydebug("ThorVG::init: DONE - shaderGlyph=0x{:X} metaSlot={}", _shaderGlyph, metadataSlotIndex());
        return Ok();
    }

    Result<void> dispose() override {
        if (auto res = deregisterFromEvents(); !res) {
            // Not fatal
        }

        // Release ThorVG objects before engine shutdown
        _canvas.reset();
        _animation.reset();
        _picture = nullptr;

        if (_engineInitialized) {
            thorvgEngineUnref();
            _engineInitialized = false;
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        // Free render buffer
        _renderBuffer.clear();
        _renderBuffer.shrink_to_fit();

        return Ok();
    }

    void suspend() override {
        _renderBuffer.clear();
        _needsRerender = true;
        yinfo("ThorVG::suspend: deallocated texture handle");
    }

    Result<void> allocateTextures() override {
        _textureHandle = TextureHandle::invalid();
        if (_animation) {
            if (_needsRerender || _renderBuffer.empty()) {
                recalculateDimensions();
                if (auto res = renderFrame(); !res) {
                    return Err<void>("ThorVG::allocateTextures: failed to render", res);
                }
                _needsRerender = false;
            } else {
                // Re-allocate handle for existing rendered pixels
                auto allocResult = _cardMgr->textureManager()->allocate(_renderWidth, _renderHeight);
                if (!allocResult) {
                    return Err<void>("ThorVG::allocateTextures: failed to allocate texture handle", allocResult);
                }
                _textureHandle = *allocResult;
            }
            _metadataDirty = true;
        }
        return Ok();
    }

    Result<void> writeTextures() override {
        if (_textureHandle.isValid() && !_renderBuffer.empty()) {
            if (auto res = _cardMgr->textureManager()->write(_textureHandle, _renderBuffer.data()); !res) {
                return Err<void>("ThorVG::writeTextures: write failed", res);
            }
        }
        return Ok();
    }

    Result<void> render(float time) override {
        // Advance animation
        if (_isAnimated && _playing && _animation && _duration > 0) {
            _accumulatedTime += 0.016;  // ~60fps

            float fps = _totalFrames / _duration;
            float targetFrame = static_cast<float>(_accumulatedTime * fps);

            if (targetFrame >= _totalFrames) {
                if (_loop) {
                    _accumulatedTime = std::fmod(_accumulatedTime, static_cast<double>(_duration));
                    targetFrame = std::fmod(targetFrame, _totalFrames);
                } else {
                    targetFrame = _totalFrames - 1;
                    _playing = false;
                }
            }

            if (std::abs(targetFrame - _currentFrame) >= 0.5f) {
                _currentFrame = targetFrame;
                _needsRerender = true;
            }
        }

        // Re-render if content changed (animation frame update)
        if (_needsRerender && _animation) {
            if (auto res = renderFrame(); !res) {
                return Err<void>("ThorVG::update: render failed", res);
            }
            // Write updated pixels to atlas (atlas exists by render time)
            if (_textureHandle.isValid() && !_renderBuffer.empty()) {
                _cardMgr->textureManager()->write(_textureHandle, _renderBuffer.data());
            }
            _needsRerender = false;
        }

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("ThorVG::update: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // EventListener interface
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                if (!_focused) {
                    _focused = true;
                    ydebug("ThorVG::onEvent: focused (id={})", id());
                }
                return Ok(true);
            } else if (_focused) {
                _focused = false;
                ydebug("ThorVG::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        if (_focused && event.type == base::Event::Type::Scroll) {
            if (event.scroll.mods & GLFW_MOD_CONTROL) {
                float zoomDelta = event.scroll.dy * 0.1f;
                float newZoom = std::clamp(_contentZoom + zoomDelta, 0.1f, 10.0f);
                if (newZoom != _contentZoom) {
                    _contentZoom = newZoom;
                    _metadataDirty = true;
                }
                return Ok(true);
            } else if (event.scroll.mods & GLFW_MOD_SHIFT) {
                float dx = event.scroll.dy * 0.05f / _contentZoom;
                _centerX = std::clamp(_centerX + dx, 0.0f, 1.0f);
                _metadataDirty = true;
                return Ok(true);
            } else {
                float dy = event.scroll.dy * 0.05f / _contentZoom;
                _centerY = std::clamp(_centerY + dy, 0.0f, 1.0f);
                _metadataDirty = true;
                return Ok(true);
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
            return Err<void>("ThorVG::registerForEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
            return Err<void>("ThorVG::registerForEvents: failed to register SetFocus", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::Scroll, self, 1000); !res) {
            return Err<void>("ThorVG::registerForEvents: failed to register Scroll", res);
        }

        yinfo("ThorVG card {} registered for events (priority 1000)", id());
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        if (weak_from_this().expired()) {
            return Ok();
        }

        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("ThorVG::deregisterFromEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;

        if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
            return Err<void>("ThorVG::deregisterFromEvents: failed to deregister", res);
        }
        return Ok();
    }

    //=========================================================================
    // Dimensions
    //=========================================================================

    void recalculateDimensions() {
        if (_contentWidth == 0 || _contentHeight == 0) {
            _renderWidth = 0;
            _renderHeight = 0;
            return;
        }

        uint32_t targetWidth = _widthCells * _cellWidth;
        uint32_t targetHeight = _heightCells * _cellHeight;

        if (targetWidth == 0 || targetHeight == 0) {
            _renderWidth = _contentWidth;
            _renderHeight = _contentHeight;
            return;
        }

        // Fit content to target while preserving aspect ratio
        float contentAspect = static_cast<float>(_contentWidth) / static_cast<float>(_contentHeight);
        float targetAspect = static_cast<float>(targetWidth) / static_cast<float>(targetHeight);

        if (contentAspect > targetAspect) {
            _renderWidth = targetWidth;
            _renderHeight = static_cast<uint32_t>(targetWidth / contentAspect);
        } else {
            _renderHeight = targetHeight;
            _renderWidth = static_cast<uint32_t>(targetHeight * contentAspect);
        }

        _renderWidth = std::max(_renderWidth, 1u);
        _renderHeight = std::max(_renderHeight, 1u);

        yinfo("ThorVG::recalculateDimensions: content={}x{} target={}x{} render={}x{}",
              _contentWidth, _contentHeight, targetWidth, targetHeight, _renderWidth, _renderHeight);

        _metadataDirty = true;
    }

    //=========================================================================
    // Content loading
    //=========================================================================

    Result<void> loadContent() {
        if (_payloadStr.empty()) {
            return Err<void>("ThorVG::loadContent: empty payload");
        }

        ydebug("ThorVG::loadContent: BEGIN payload_size={}", _payloadStr.size());

        // Show first 100 chars of payload for debugging
        std::string preview = _payloadStr.substr(0, std::min<size_t>(_payloadStr.size(), 100));
        ydebug("ThorVG::loadContent: payload preview='{}'", preview);

        std::string content = _payloadStr;
        std::string mimeType;

        // Check for type prefix (format: "type\n<content>")
        size_t newlinePos = _payloadStr.find('\n');
        if (newlinePos != std::string::npos && newlinePos < 20) {
            std::string prefix = _payloadStr.substr(0, newlinePos);
            ydebug("ThorVG::loadContent: found prefix='{}' at pos {}", prefix, newlinePos);
            if (prefix == "svg" || prefix == "lottie" || prefix == "yaml") {
                mimeType = prefix;
                content = _payloadStr.substr(newlinePos + 1);
            }
        }

        // Auto-detect from content
        if (mimeType.empty()) {
            size_t start = content.find_first_not_of(" \t\n\r");
            if (start != std::string::npos && content[start] == '{') {
                mimeType = "lottie";
            } else if (content.find("<svg") != std::string::npos ||
                       (content.find("<?xml") != std::string::npos && content.find("<svg") != std::string::npos)) {
                mimeType = "svg";
            }
            ydebug("ThorVG::loadContent: auto-detected mimeType='{}'", mimeType);
        }

        ydebug("ThorVG::loadContent: mimeType='{}', content size={}", mimeType, content.size());

        // Convert YAML to SVG
        if (mimeType == "yaml") {
            ydebug("ThorVG::loadContent: converting YAML to SVG");
            auto result = yamlToSvg(content);
            if (!result) {
                return Err<void>("ThorVG: failed to convert YAML to SVG", result);
            }
            content = *result;
            mimeType = "svg";
            ydebug("ThorVG::loadContent: YAML->SVG OK, svg size={}", content.size());
        }

        // Default dimensions
        _contentWidth = 256;
        _contentHeight = 256;

        // Use Animation wrapper for both animated and static content
        // (Picture has protected destructor, Animation manages its lifetime)
        ydebug("ThorVG::loadContent: creating Animation wrapper");
        _animation.reset(tvg::Animation::gen());
        if (!_animation) {
            return Err<void>("ThorVG: failed to create Animation");
        }

        tvg::Picture* picture = _animation->picture();
        if (!picture) {
            _animation.reset();
            return Err<void>("ThorVG: failed to get picture from Animation");
        }
        ydebug("ThorVG::loadContent: Animation created, picture={}", (void*)picture);

        if (mimeType == "lottie" || mimeType == "lottie+json") {
            ydebug("ThorVG::loadContent: loading as Lottie, data size={}", content.size());
            auto result = picture->load(content.c_str(), static_cast<uint32_t>(content.size()),
                                        "lottie", nullptr, true);
            ydebug("ThorVG::loadContent: Lottie load result={}", static_cast<int>(result));
            if (result != tvg::Result::Success) {
                _animation.reset();
                return Err<void>("ThorVG: failed to load Lottie animation");
            }
            _isAnimated = true;
            _totalFrames = _animation->totalFrame();
            _duration = _animation->duration();
            _currentFrame = 0.0f;
            ydebug("ThorVG::loadContent: Lottie loaded - {} frames, {}s duration", _totalFrames, _duration);
        } else {
            // Static content (SVG etc.) - let ThorVG auto-detect
            ydebug("ThorVG::loadContent: loading as static (mime=nullptr), data size={}", content.size());
            auto result = picture->load(content.c_str(), static_cast<uint32_t>(content.size()),
                                        nullptr, nullptr, true);
            ydebug("ThorVG::loadContent: static load result={}", static_cast<int>(result));
            if (result != tvg::Result::Success) {
                _animation.reset();
                return Err<void>("ThorVG: failed to load content (result=" +
                                 std::to_string(static_cast<int>(result)) + ")");
            }
            _isAnimated = false;
        }

        // Get content dimensions
        float w = 0, h = 0;
        picture->size(&w, &h);
        ydebug("ThorVG::loadContent: picture dimensions={}x{}", w, h);
        if (w > 0 && h > 0) {
            _contentWidth = static_cast<uint32_t>(w);
            _contentHeight = static_cast<uint32_t>(h);
        }

        _picture = picture;

        // Free payload string now that content is loaded
        _payloadStr.clear();
        _payloadStr.shrink_to_fit();

        ydebug("ThorVG::loadContent: DONE - {}x{} animated={}", _contentWidth, _contentHeight, _isAnimated);
        return Ok();
    }

    //=========================================================================
    // Software rendering via SwCanvas -> textureData buffer
    //=========================================================================

    Result<void> renderFrame() {
        ydebug("ThorVG::renderFrame: BEGIN anim={} pic={} render={}x{}",
               (void*)_animation.get(), (void*)_picture, _renderWidth, _renderHeight);

        if (!_animation || !_picture || _renderWidth == 0 || _renderHeight == 0) {
            return Err<void>("ThorVG::renderFrame: not ready");
        }

        uint32_t dataSize = _renderWidth * _renderHeight * 4;
        ydebug("ThorVG::renderFrame: need {} bytes for {}x{} RGBA", dataSize, _renderWidth, _renderHeight);

        // Allocate/resize local render buffer
        _renderBuffer.resize(dataSize);

        // Create SwCanvas targeting our local buffer
        ydebug("ThorVG::renderFrame: creating SwCanvas");
        _canvas.reset(tvg::SwCanvas::gen());
        if (!_canvas) {
            return Err<void>("ThorVG::renderFrame: failed to create SwCanvas");
        }

        ydebug("ThorVG::renderFrame: setting target buffer={} stride={} w={} h={}",
               (void*)_renderBuffer.data(), _renderWidth, _renderWidth, _renderHeight);
        auto targetResult = _canvas->target(
            reinterpret_cast<uint32_t*>(_renderBuffer.data()),
            _renderWidth, _renderWidth, _renderHeight,
            tvg::ColorSpace::ABGR8888S);
        if (targetResult != tvg::Result::Success) {
            ydebug("ThorVG::renderFrame: target() failed result={}", static_cast<int>(targetResult));
            _canvas.reset();
            return Err<void>("ThorVG::renderFrame: failed to set canvas target");
        }
        ydebug("ThorVG::renderFrame: canvas target set OK");

        // Scale picture to fit render dimensions
        _picture->size(static_cast<float>(_renderWidth), static_cast<float>(_renderHeight));
        ydebug("ThorVG::renderFrame: picture resized to {}x{}", _renderWidth, _renderHeight);

        // Set animation frame if animated
        if (_isAnimated && _animation) {
            _animation->frame(_currentFrame);
            ydebug("ThorVG::renderFrame: set animation frame={}", _currentFrame);
        }

        // Push picture to canvas, render, sync
        ydebug("ThorVG::renderFrame: push()");
        auto pushResult = _canvas->push(_picture);
        if (pushResult != tvg::Result::Success) {
            ydebug("ThorVG::renderFrame: push() failed result={}", static_cast<int>(pushResult));
            _canvas.reset();
            return Err<void>("ThorVG::renderFrame: canvas push failed");
        }

        ydebug("ThorVG::renderFrame: update()");
        auto updateResult = _canvas->update();
        if (updateResult != tvg::Result::Success) {
            ydebug("ThorVG::renderFrame: update() failed result={}", static_cast<int>(updateResult));
            _canvas.reset();
            return Err<void>("ThorVG::renderFrame: canvas update failed");
        }

        ydebug("ThorVG::renderFrame: draw(true)");
        auto drawResult = _canvas->draw(true);
        if (drawResult != tvg::Result::Success) {
            ydebug("ThorVG::renderFrame: draw() failed result={}", static_cast<int>(drawResult));
            _canvas.reset();
            return Err<void>("ThorVG::renderFrame: canvas draw failed");
        }

        ydebug("ThorVG::renderFrame: sync()");
        auto syncResult = _canvas->sync();
        if (syncResult != tvg::Result::Success) {
            ydebug("ThorVG::renderFrame: sync() failed result={}", static_cast<int>(syncResult));
            _canvas.reset();
            return Err<void>("ThorVG::renderFrame: canvas sync failed");
        }

        // Check first few pixels to see if anything was rendered
        uint32_t* pixels = reinterpret_cast<uint32_t*>(_renderBuffer.data());
        ydebug("ThorVG::renderFrame: first 4 pixels: 0x{:08X} 0x{:08X} 0x{:08X} 0x{:08X}",
               pixels[0], pixels[1], pixels[2], pixels[3]);

        // Reset canvas (picture lifetime managed by _animation)
        _canvas.reset();

        // Allocate texture handle if needed
        if (!_textureHandle.isValid()) {
            auto allocResult = _cardMgr->textureManager()->allocate(_renderWidth, _renderHeight);
            if (!allocResult) {
                return Err<void>("ThorVG::renderFrame: failed to allocate texture handle", allocResult);
            }
            _textureHandle = *allocResult;
        }
        _metadataDirty = true;

        ydebug("ThorVG::renderFrame: DONE - handle id={}", _textureHandle.id);
        return Ok();
    }

    //=========================================================================
    // Args parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        yinfo("ThorVG::parseArgs: args='{}'", args);

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--zoom" || token == "-z") {
                float val;
                if (iss >> val) {
                    _contentZoom = std::clamp(val, 0.1f, 10.0f);
                }
            } else if (token == "--center" || token == "-c") {
                float x, y;
                if (iss >> x >> y) {
                    _centerX = std::clamp(x, 0.0f, 1.0f);
                    _centerY = std::clamp(y, 0.0f, 1.0f);
                }
            } else if (token == "--no-loop") {
                _loop = false;
            } else if (token == "--paused") {
                _playing = false;
            }
        }
    }

    //=========================================================================
    // Metadata upload
    //=========================================================================

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("ThorVG::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};
        meta.textureDataOffset = 0;  // No longer used
        meta.textureWidth = _renderWidth;
        meta.textureHeight = _renderHeight;

        if (_textureHandle.isValid()) {
            auto pos = _cardMgr->textureManager()->getAtlasPosition(_textureHandle);
            meta.atlasX = pos.x;
            meta.atlasY = pos.y;
        } else {
            meta.atlasX = 0;
            meta.atlasY = 0;
        }
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.zoom = _contentZoom;
        meta.centerX = _centerX;
        meta.centerY = _centerY;
        meta.flags = 0;
        meta.bgColor = 0xFF000000;
        meta.scaledWidth = _renderWidth;
        meta.scaledHeight = _renderHeight;

        ydebug("ThorVG::uploadMetadata: metaOffset={} texDataOffset={} texSize={}x{} "
               "cells={}x{} zoom={} center=({},{}) scaledSize={}x{}",
               _metaHandle.offset, meta.textureDataOffset,
               meta.textureWidth, meta.textureHeight,
               meta.widthCells, meta.heightCells,
               meta.zoom, meta.centerX, meta.centerY,
               meta.scaledWidth, meta.scaledHeight);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("ThorVG::uploadMetadata: write failed", res);
        }

        return Ok();
    }

    //=========================================================================
    // Data structures
    //=========================================================================

    // Metadata structure (64 bytes, matches Texture card / shader layout)
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
    std::string _payloadStr;

    // ThorVG objects
    std::unique_ptr<tvg::SwCanvas> _canvas;
    std::unique_ptr<tvg::Animation> _animation;
    tvg::Picture* _picture = nullptr;  // Owned by animation

    // Content dimensions (original from SVG/Lottie)
    uint32_t _contentWidth = 0;
    uint32_t _contentHeight = 0;

    // Render dimensions (fitted to cell grid)
    uint32_t _renderWidth = 0;
    uint32_t _renderHeight = 0;

    // Animation state
    bool _isAnimated = false;
    float _totalFrames = 0.0f;
    float _currentFrame = 0.0f;
    float _duration = 0.0f;
    bool _playing = true;
    bool _loop = true;
    double _accumulatedTime = 0.0;

    // Content zoom and pan
    float _contentZoom = 1.0f;
    float _centerX = 0.5f;
    float _centerY = 0.5f;

    // Cell dimensions
    uint32_t _cellWidth = 10;
    uint32_t _cellHeight = 20;

    // Render buffer (local CPU pixels)
    std::vector<uint8_t> _renderBuffer;

    // Texture handle for atlas packing
    TextureHandle _textureHandle = TextureHandle::invalid();

    // State
    bool _focused = false;
    bool _needsRerender = false;
    bool _metadataDirty = true;
    bool _engineInitialized = false;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<ThorVG::Ptr> ThorVG::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;

    ydebug("ThorVG::createImpl: x={} y={} w={} h={} args='{}' payload_size={} cardMgr={}",
           x, y, widthCells, heightCells, args, payload.size(), (void*)yettyCtx.cardManager->bufferManager().get());

    if (!yettyCtx.cardManager) {
        return Err<Ptr>("ThorVG::createImpl: null CardBufferManager");
    }
    if (false) { // cardManager always valid
        return Err<Ptr>("ThorVG::createImpl: null CardTextureManager");
    }

    auto card = std::make_shared<ThorVGImpl>(
        yettyCtx, x, y, widthCells, heightCells, args, payload);

    ydebug("ThorVG::createImpl: calling init()");
    if (auto res = card->init(); !res) {
        yerror("ThorVG::createImpl: init failed: {}", error_msg(res));
        return Err<Ptr>("ThorVG::createImpl: init failed", res);
    }

    ydebug("ThorVG::createImpl: SUCCESS glyph=0x{:X} metaOffset={}", card->shaderGlyph(), card->metadataOffset());
    return Ok<Ptr>(card);
}

} // namespace yetty::card
