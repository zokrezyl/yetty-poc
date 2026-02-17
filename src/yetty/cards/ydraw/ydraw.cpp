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

            // Auto-compute zoom/pan to fit one page height after scene bounds are known
            if (_fitPageHeight > 0.0f) {
                float sceneMinY = _builder->sceneMinY();
                float sceneMaxY = _builder->sceneMaxY();
                float sceneH = sceneMaxY - sceneMinY;
                if (sceneH > _fitPageHeight) {
                    _viewZoom = sceneH / _fitPageHeight;
                    float centerY = (sceneMinY + sceneMaxY) * 0.5f;
                    _viewPanY = _fitPageHeight * 0.5f - centerY;
                    _builder->setView(_viewZoom, _viewPanX, _viewPanY);
                }
                _fitPageHeight = 0.0f;
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
            } else if (token == "--fit-page-height") {
                std::string s;
                if (iss >> s) _fitPageHeight = std::stof(s);
            }
        }
    }

    Result<void> parsePayload(const std::string& payload) {
        if (payload.empty()) return Ok();
        bool isYaml = false;
        if (payload.size() >= 4) {
            char first = payload[0];
            if (first == '#' || first == '-' || first == ' ' || first == '\n' ||
                first == '\t' || (first >= 'a' && first <= 'z') ||
                (first >= 'A' && first <= 'Z')) {
                isYaml = true;
            }
        }
        if (isYaml) {
            auto res = parseYDrawYAML(_buffer, payload);
            if (!res) return Err<void>(res.error().message());
            if (*res) _animation = *res;
            return Ok();
        }
        return parseBinary(payload);
    }

    Result<void> parseBinary(const std::string& payload) {
        const size_t HEADER_SIZE = 16;

        if (payload.size() < HEADER_SIZE) return Ok();

        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());
        uint32_t version, primCount, bgColorVal, flagsVal;
        std::memcpy(&version, data + 0, 4);
        std::memcpy(&primCount, data + 4, 4);
        std::memcpy(&bgColorVal, data + 8, 4);
        std::memcpy(&flagsVal, data + 12, 4);

        if (version != 3) return Ok();

        _buffer->setBgColor(bgColorVal);
        _buffer->addFlags(flagsVal);

        // Read totalCompactWords + compact prim data
        if (payload.size() < HEADER_SIZE + 4) return Ok();
        uint32_t totalCompactWords;
        std::memcpy(&totalCompactWords, data + HEADER_SIZE, 4);

        size_t primDataStart = HEADER_SIZE + 4;
        size_t primDataBytes = static_cast<size_t>(totalCompactWords) * sizeof(float);
        if (payload.size() < primDataStart + primDataBytes) return Ok();

        // Build a serialized buffer blob: [primCount:u32][totalWords:u32][compact_data]
        size_t blobSize = 8 + primDataBytes;
        std::vector<uint8_t> blob(blobSize);
        std::memcpy(blob.data(), &primCount, 4);
        std::memcpy(blob.data() + 4, &totalCompactWords, 4);
        std::memcpy(blob.data() + 8, data + primDataStart, primDataBytes);
        if (auto res = _buffer->deserialize(blob.data(), blob.size()); !res) {
            return res;
        }

        // Font + text sections follow the prim data
        size_t textSectionOffset = primDataStart + primDataBytes;
        if (auto res = parseFontAndText(data, payload.size(), textSectionOffset); !res) {
            ywarn("parseBinary: text section failed: {}", error_msg(res));
        }

        return Ok();
    }

    Result<void> parseFontAndText(const uint8_t* data, size_t totalSize, size_t offset) {
        auto read32 = [&](uint32_t& val) -> bool {
            if (offset + 4 > totalSize) return false;
            std::memcpy(&val, data + offset, 4);
            offset += 4;
            return true;
        };
        auto readF32 = [&](float& val) -> bool {
            if (offset + 4 > totalSize) return false;
            std::memcpy(&val, data + offset, 4);
            offset += 4;
            return true;
        };

        // --- Font Section ---
        uint32_t fontCount = 0;
        if (!read32(fontCount)) return Ok();

        std::vector<int> fontIdMap(fontCount, -1);

        for (uint32_t i = 0; i < fontCount; i++) {
            uint32_t fontDataSize = 0;
            if (!read32(fontDataSize)) return Ok();

            uint32_t paddedSize = (fontDataSize + 3) & ~3u;
            if (offset + paddedSize > totalSize) return Ok();

            int bufFontId = _buffer->addFontBlob(
                data + offset, fontDataSize,
                "font_" + std::to_string(i));
            fontIdMap[i] = bufFontId;
            yinfo("parseFontAndText: registered font {} from {} bytes -> bufFontId={}",
                  i, fontDataSize, bufFontId);

            offset += paddedSize;
        }

        // --- Text Section ---
        uint32_t defaultSpanCount = 0, customSpanCount = 0, textDataSize = 0;
        if (!read32(defaultSpanCount)) return Ok();
        if (!read32(customSpanCount)) return Ok();
        if (!read32(textDataSize)) return Ok();

        // Remember where span arrays and text data start
        size_t defaultSpansStart = offset;
        size_t customSpansStart = defaultSpansStart + defaultSpanCount * 32;
        size_t textDataStart = customSpansStart + customSpanCount * 32;
        uint32_t paddedTextSize = (textDataSize + 3) & ~3u;

        if (textDataStart + paddedTextSize > totalSize) {
            ywarn("parseFontAndText: text section truncated");
            return Ok();
        }

        const char* textBlob = reinterpret_cast<const char*>(data + textDataStart);

        // Parse default text spans (use default MsMsdfFont)
        offset = defaultSpansStart;
        for (uint32_t i = 0; i < defaultSpanCount; i++) {
            float x, y, fontSize;
            uint32_t color, layer, textOffset, textLength, pad;
            if (!readF32(x) || !readF32(y) || !readF32(fontSize) ||
                !read32(color) || !read32(layer) ||
                !read32(textOffset) || !read32(textLength) || !read32(pad))
                break;

            if (textOffset + textLength > textDataSize) continue;
            std::string text(textBlob + textOffset, textLength);
            _buffer->addText(x, y, text, fontSize, color, layer, -1);
        }

        // Parse custom text spans (use uploaded fonts)
        offset = customSpansStart;
        for (uint32_t i = 0; i < customSpanCount; i++) {
            float x, y, fontSize;
            uint32_t color, fontIndex, layer, textOffset, textLength;
            if (!readF32(x) || !readF32(y) || !readF32(fontSize) ||
                !read32(color) || !read32(fontIndex) || !read32(layer) ||
                !read32(textOffset) || !read32(textLength))
                break;

            if (textOffset + textLength > textDataSize) continue;
            std::string text(textBlob + textOffset, textLength);

            int fid = (fontIndex < fontCount && fontIdMap[fontIndex] >= 0)
                      ? fontIdMap[fontIndex] : -1;
            _buffer->addText(x, y, text, fontSize, color, layer, fid);
        }

        // --- Rotated Text Spans (optional, after text blob) ---
        offset = textDataStart + paddedTextSize;
        uint32_t rotatedSpanCount = 0;
        if (read32(rotatedSpanCount) && rotatedSpanCount > 0) {
            for (uint32_t i = 0; i < rotatedSpanCount; i++) {
                float rx, ry, rFontSize, rRotation;
                uint32_t rColor, rFontIndex, rTextOffset, rTextLength;
                if (!readF32(rx) || !readF32(ry) || !readF32(rFontSize) ||
                    !readF32(rRotation) || !read32(rColor) || !read32(rFontIndex) ||
                    !read32(rTextOffset) || !read32(rTextLength))
                    break;

                if (rTextOffset + rTextLength > textDataSize) continue;
                std::string text(textBlob + rTextOffset, rTextLength);

                int fid = (rFontIndex < fontCount && fontIdMap[rFontIndex] >= 0)
                          ? fontIdMap[rFontIndex] : -1;
                _buffer->addRotatedText(rx, ry, text, rFontSize,
                                        rColor, rRotation, fid);
            }
        }

        yinfo("parseFontAndText: {} fonts, {} default spans, {} custom spans, "
              "{} rotated spans, {} text bytes",
              fontCount, defaultSpanCount, customSpanCount,
              rotatedSpanCount, textDataSize);

        return Ok();
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
    float _fitPageHeight = 0.0f; // if set, compute zoom/pan to fit one page
    float _viewPanY = 0.0f;
    bool _focused = false;

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
