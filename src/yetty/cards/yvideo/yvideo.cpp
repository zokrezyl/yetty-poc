#include "yvideo.h"
#include <yetty/yvideo/yvideo-decoder.h>
#include <yetty/yetty-context.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <yetty/wgpu-compat.h>

namespace yetty::card {

//=============================================================================
// YVideoImpl - Implementation of YVideo card
//=============================================================================

class YVideoImpl : public YVideo {
public:
    YVideoImpl(const YettyContext& ctx,
               int32_t x, int32_t y,
               uint32_t widthCells, uint32_t heightCells,
               const std::string& args, const std::string& payload)
        : YVideo(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _ctx(ctx)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~YVideoImpl() override {
        dispose();
    }

    const char* typeName() const override { return "yvideo"; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    void setCellSize(float cellWidth, float cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            recalculateScaledDimensions();
            _needsRealloc = true;
        }
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("YVideo::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("YVideo::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Create decoder
        auto decoderResult = yvideo::Decoder::create();
        if (!decoderResult) {
            return Err<void>("YVideo::init: failed to create decoder", decoderResult);
        }
        _decoder = *decoderResult;

        // Register for events
        if (auto res = registerForEvents(); !res) {
            return Err<void>("YVideo::init: failed to register for events", res);
        }

        // If payload contains initial data, decode it
        if (!_payloadStr.empty()) {
            if (auto res = decodePayload(_payloadStr); !res) {
                ywarn("YVideo::init: failed to decode initial payload: {}", error_msg(res));
            }
        }

        _metadataDirty = true;
        return Ok();
    }

    Result<void> dispose() override {
        // Deregister from events
        if (auto res = deregisterFromEvents(); !res) {
            return Err<void>("YVideo::dispose: failed to deregister", res);
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        // Clear decoder
        _decoder.reset();

        // Clear frame data
        _yPlane.clear();
        _uPlane.clear();
        _vPlane.clear();

        return Ok();
    }

    void suspend() override {
        _yTextureHandle = TextureHandle::invalid();
        _uvTextureHandle = TextureHandle::invalid();
        _needsRealloc = true;
        yinfo("YVideo::suspend: released texture handles");
    }

    bool needsTextureRealloc() override {
        return _needsRealloc;
    }

    Result<void> allocateTextures() override {
        _yTextureHandle = TextureHandle::invalid();
        _uvTextureHandle = TextureHandle::invalid();

        if (_frameWidth == 0 || _frameHeight == 0) {
            return Ok();
        }

        // Calculate scaled dimensions
        recalculateScaledDimensions();

        // Allocate Y plane texture (grayscale stored as RGBA)
        auto yResult = _cardMgr->textureManager()->allocate(_scaledYWidth, _scaledYHeight);
        if (!yResult) {
            return Err<void>("YVideo::allocateTextures: failed to allocate Y texture", yResult);
        }
        _yTextureHandle = *yResult;

        // Allocate UV planes texture (U and V side by side, each half resolution)
        // Layout: [U plane (w/2 x h/2)] [V plane (w/2 x h/2)]
        uint32_t uvWidth = _scaledYWidth;  // U and V side by side
        uint32_t uvHeight = _scaledYHeight / 2;
        auto uvResult = _cardMgr->textureManager()->allocate(uvWidth, uvHeight);
        if (!uvResult) {
            return Err<void>("YVideo::allocateTextures: failed to allocate UV texture", uvResult);
        }
        _uvTextureHandle = *uvResult;

        _needsRealloc = false;
        _metadataDirty = true;

        yinfo("YVideo::allocateTextures: Y={}x{} UV={}x{}",
              _scaledYWidth, _scaledYHeight, uvWidth, uvHeight);

        return Ok();
    }

    Result<void> writeTextures() override {
        if (!_yTextureHandle.isValid() || _yPlane.empty()) {
            return Ok();
        }

        // Write Y plane (grayscale expanded to RGBA)
        std::vector<uint8_t> yRgba(_scaledYWidth * _scaledYHeight * 4);
        for (size_t i = 0; i < _yPlane.size(); ++i) {
            yRgba[i * 4 + 0] = _yPlane[i];
            yRgba[i * 4 + 1] = _yPlane[i];
            yRgba[i * 4 + 2] = _yPlane[i];
            yRgba[i * 4 + 3] = 255;
        }
        if (auto res = _cardMgr->textureManager()->write(_yTextureHandle, yRgba.data()); !res) {
            return Err<void>("YVideo::writeTextures: Y write failed", res);
        }

        // Write UV planes (U and V side by side)
        uint32_t uvWidth = _scaledYWidth / 2;
        uint32_t uvHeight = _scaledYHeight / 2;
        std::vector<uint8_t> uvRgba(_scaledYWidth * uvHeight * 4);

        for (uint32_t y = 0; y < uvHeight; ++y) {
            for (uint32_t x = 0; x < uvWidth; ++x) {
                size_t srcIdx = y * uvWidth + x;
                // U plane on the left
                size_t dstIdxU = (y * _scaledYWidth + x) * 4;
                uvRgba[dstIdxU + 0] = _uPlane[srcIdx];
                uvRgba[dstIdxU + 1] = _uPlane[srcIdx];
                uvRgba[dstIdxU + 2] = _uPlane[srcIdx];
                uvRgba[dstIdxU + 3] = 255;

                // V plane on the right
                size_t dstIdxV = (y * _scaledYWidth + uvWidth + x) * 4;
                uvRgba[dstIdxV + 0] = _vPlane[srcIdx];
                uvRgba[dstIdxV + 1] = _vPlane[srcIdx];
                uvRgba[dstIdxV + 2] = _vPlane[srcIdx];
                uvRgba[dstIdxV + 3] = 255;
            }
        }

        if (auto res = _cardMgr->textureManager()->write(_uvTextureHandle, uvRgba.data()); !res) {
            return Err<void>("YVideo::writeTextures: UV write failed", res);
        }

        return Ok();
    }

    void renderToStaging(float time) override {
        (void)time;
        // Video decoding happens in update()
    }

    Result<void> finalize() override {
        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("YVideo::finalize: metadata upload failed", res);
            }
            _metadataDirty = false;
        }
        return Ok();
    }

    //=========================================================================
    // Update API - receive new video data via OSC
    //=========================================================================

    Result<void> update(const std::string& args, const std::string& payload) override {
        (void)args;

        if (payload.empty()) {
            return Ok();
        }

        return decodePayload(payload);
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
                    ydebug("YVideo::onEvent: focused (id={})", id());
                }
                return Ok(true);
            } else if (_focused) {
                _focused = false;
                ydebug("YVideo::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        // Handle scroll events (only when focused)
        if (_focused && event.type == base::Event::Type::Scroll) {
            constexpr int GLFW_MOD_CONTROL = 0x0002;
            if (event.scroll.mods & GLFW_MOD_CONTROL) {
                // Ctrl+Scroll: zoom
                float zoomDelta = event.scroll.dy * 0.1f;
                float newZoom = std::clamp(_zoom + zoomDelta, 0.1f, 10.0f);
                if (newZoom != _zoom) {
                    _zoom = newZoom;
                    _metadataDirty = true;
                }
                return Ok(true);
            }
        }

        return Ok(false);
    }

private:
    //=========================================================================
    // Internal methods
    //=========================================================================

    void parseArgs(const std::string& args) {
        // Parse key=value pairs separated by semicolons
        // Supported: zoom=1.0; centerX=0.5; centerY=0.5
        (void)args;
        _zoom = 1.0f;
        _centerX = 0.5f;
        _centerY = 0.5f;
    }

    Result<void> decodePayload(const std::string& payload) {
        // Payload is raw AV1 OBU data (base64 decoded by OSC layer)
        auto data = std::span<const uint8_t>(
            reinterpret_cast<const uint8_t*>(payload.data()),
            payload.size()
        );

        if (auto res = _decoder->feed(data); !res) {
            return Err<void>("YVideo::decodePayload: feed failed", res);
        }

        // Try to get decoded frame
        auto frameResult = _decoder->getFrame();
        if (!frameResult) {
            return Err<void>("YVideo::decodePayload: getFrame failed", frameResult);
        }

        auto frameOpt = *frameResult;
        if (!frameOpt) {
            // No frame ready yet (buffered)
            return Ok();
        }

        auto& frame = *frameOpt;
        if (!frame.isValid()) {
            _decoder->releaseFrame();
            return Err<void>("YVideo::decodePayload: invalid frame");
        }

        // Store frame dimensions
        bool sizeChanged = (_frameWidth != frame.width || _frameHeight != frame.height);
        _frameWidth = frame.width;
        _frameHeight = frame.height;
        _colorMatrix = frame.colorMatrix;

        if (sizeChanged) {
            _needsRealloc = true;
            recalculateScaledDimensions();
        }

        // Copy YUV planes
        copyYUVPlanes(frame);

        _decoder->releaseFrame();
        _metadataDirty = true;

        yinfo("YVideo::decodePayload: decoded frame {}x{}", _frameWidth, _frameHeight);
        return Ok();
    }

    void copyYUVPlanes(const yvideo::YUVFrame& frame) {
        // Copy Y plane (full resolution)
        uint32_t ySize = frame.width * frame.height;
        _yPlane.resize(ySize);
        for (uint32_t row = 0; row < frame.height; ++row) {
            std::memcpy(_yPlane.data() + row * frame.width,
                        frame.yPlane + row * frame.yStride,
                        frame.width);
        }

        // Copy U/V planes (half resolution)
        uint32_t uvWidth = frame.width / 2;
        uint32_t uvHeight = frame.height / 2;
        uint32_t uvSize = uvWidth * uvHeight;

        _uPlane.resize(uvSize);
        _vPlane.resize(uvSize);

        for (uint32_t row = 0; row < uvHeight; ++row) {
            std::memcpy(_uPlane.data() + row * uvWidth,
                        frame.uPlane + row * frame.uStride,
                        uvWidth);
            std::memcpy(_vPlane.data() + row * uvWidth,
                        frame.vPlane + row * frame.vStride,
                        uvWidth);
        }
    }

    void recalculateScaledDimensions() {
        if (_frameWidth == 0 || _frameHeight == 0) {
            _scaledYWidth = 0;
            _scaledYHeight = 0;
            return;
        }

        uint32_t targetWidth = static_cast<uint32_t>(_widthCells * _cellWidth);
        uint32_t targetHeight = static_cast<uint32_t>(_heightCells * _cellHeight);

        if (targetWidth == 0 || targetHeight == 0) {
            _scaledYWidth = _frameWidth;
            _scaledYHeight = _frameHeight;
            return;
        }

        float frameAspect = static_cast<float>(_frameWidth) / static_cast<float>(_frameHeight);
        float targetAspect = static_cast<float>(targetWidth) / static_cast<float>(targetHeight);

        if (frameAspect > targetAspect) {
            _scaledYWidth = targetWidth;
            _scaledYHeight = static_cast<uint32_t>(targetWidth / frameAspect);
        } else {
            _scaledYHeight = targetHeight;
            _scaledYWidth = static_cast<uint32_t>(targetHeight * frameAspect);
        }

        // Ensure even dimensions for YUV420
        _scaledYWidth = (_scaledYWidth + 1) & ~1u;
        _scaledYHeight = (_scaledYHeight + 1) & ~1u;

        _scaledYWidth = std::max(_scaledYWidth, 2u);
        _scaledYHeight = std::max(_scaledYHeight, 2u);
    }

    Result<void> uploadMetadata() {
        Metadata meta = {};
        meta.reserved = 0;
        meta.frameWidth = _frameWidth;
        meta.frameHeight = _frameHeight;

        if (_yTextureHandle.isValid()) {
            auto yPos = _cardMgr->textureManager()->getAtlasPosition(_yTextureHandle);
            meta.yAtlasX = yPos.x;
            meta.yAtlasY = yPos.y;
        }

        if (_uvTextureHandle.isValid()) {
            auto uvPos = _cardMgr->textureManager()->getAtlasPosition(_uvTextureHandle);
            meta.uvAtlasX = uvPos.x;
            meta.uvAtlasY = uvPos.y;
        }

        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.zoom = _zoom;
        meta.centerX = _centerX;
        meta.centerY = _centerY;
        meta.colorMatrix = static_cast<uint32_t>(_colorMatrix);
        meta.bgColor = 0xFF000000;  // Black background
        meta.scaledYWidth = _scaledYWidth;
        meta.scaledYHeight = _scaledYHeight;

        _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta));
        return Ok();
    }

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("YVideo::registerForEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
            return Err<void>("YVideo::registerForEvents: failed to register SetFocus", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::Scroll, self, 1000); !res) {
            return Err<void>("YVideo::registerForEvents: failed to register Scroll", res);
        }

        yinfo("YVideo card {} registered for events", id());
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        if (weak_from_this().expired()) {
            return Ok();
        }

        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("YVideo::deregisterFromEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;

        if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
            return Err<void>("YVideo::deregisterFromEvents: failed to deregister", res);
        }
        return Ok();
    }

    //=========================================================================
    // Metadata layout (must match shader)
    //=========================================================================
    struct Metadata {
        uint32_t reserved;
        uint32_t frameWidth;
        uint32_t frameHeight;
        uint32_t yAtlasX;
        uint32_t yAtlasY;
        uint32_t uvAtlasX;
        uint32_t uvAtlasY;
        uint32_t widthCells;
        uint32_t heightCells;
        float zoom;
        float centerX;
        float centerY;
        uint32_t colorMatrix;
        uint32_t bgColor;
        uint32_t scaledYWidth;
        uint32_t scaledYHeight;
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    //=========================================================================
    // Member variables
    //=========================================================================
    const YettyContext& _ctx;
    std::string _argsStr;
    std::string _payloadStr;

    // Decoder
    yvideo::Decoder::Ptr _decoder;

    // Frame data
    uint32_t _frameWidth = 0;
    uint32_t _frameHeight = 0;
    yvideo::YUVFrame::ColorMatrix _colorMatrix = yvideo::YUVFrame::ColorMatrix::BT709;

    // YUV planes (CPU copy)
    std::vector<uint8_t> _yPlane;
    std::vector<uint8_t> _uPlane;
    std::vector<uint8_t> _vPlane;

    // Texture handles
    TextureHandle _yTextureHandle = TextureHandle::invalid();
    TextureHandle _uvTextureHandle = TextureHandle::invalid();

    // Scaling
    float _cellWidth = 1.0f;
    float _cellHeight = 1.0f;
    uint32_t _scaledYWidth = 0;
    uint32_t _scaledYHeight = 0;

    // View state
    float _zoom = 1.0f;
    float _centerX = 0.5f;
    float _centerY = 0.5f;

    // Flags
    bool _needsRealloc = false;
    bool _metadataDirty = true;
    bool _focused = false;
};

//=============================================================================
// Factory
//=============================================================================

Result<YVideo::Ptr> YVideo::createImpl(ContextType& ctx, const YettyContext& yettyCtx,
                                       int32_t x, int32_t y, uint32_t widthCells,
                                       uint32_t heightCells, const std::string& args,
                                       const std::string& payload) noexcept {
    (void)ctx;  // ObjectFactory context marker

    auto card = std::make_shared<YVideoImpl>(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (auto res = card->init(); !res) {
        return Err<Ptr>("YVideo::createImpl: init failed", res);
    }
    return Ok<Ptr>(card);
}

} // namespace yetty::card
