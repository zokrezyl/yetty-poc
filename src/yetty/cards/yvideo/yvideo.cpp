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

        // Create video source
        auto sourceResult = yvideo::VideoSource::create();
        if (!sourceResult) {
            return Err<void>("YVideo::init: failed to create video source", sourceResult);
        }
        _videoSource = *sourceResult;

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
        // Stop playback timer
        stopPlayback();
        
        // Destroy timer
        if (_timerId >= 0) {
            auto loopResult = base::EventLoop::instance();
            if (loopResult) {
                (*loopResult)->destroyTimer(_timerId);
            }
            _timerId = -1;
        }
        
        // Deregister from events
        if (auto res = deregisterFromEvents(); !res) {
            return Err<void>("YVideo::dispose: failed to deregister", res);
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        // Clear decoder
        _videoSource.reset();

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

        // Write Y plane (grayscale expanded to RGBA, scaled to fit texture)
        std::vector<uint8_t> yRgba(_scaledYWidth * _scaledYHeight * 4);
        
        // Simple nearest-neighbor scaling from _frameWidth x _frameHeight to _scaledYWidth x _scaledYHeight
        for (uint32_t y = 0; y < _scaledYHeight; ++y) {
            uint32_t srcY = y * _frameHeight / _scaledYHeight;
            for (uint32_t x = 0; x < _scaledYWidth; ++x) {
                uint32_t srcX = x * _frameWidth / _scaledYWidth;
                size_t srcIdx = srcY * _frameWidth + srcX;
                size_t dstIdx = (y * _scaledYWidth + x) * 4;
                uint8_t val = (srcIdx < _yPlane.size()) ? _yPlane[srcIdx] : 0;
                yRgba[dstIdx + 0] = val;
                yRgba[dstIdx + 1] = val;
                yRgba[dstIdx + 2] = val;
                yRgba[dstIdx + 3] = 255;
            }
        }
        if (auto res = _cardMgr->textureManager()->write(_yTextureHandle, yRgba.data()); !res) {
            return Err<void>("YVideo::writeTextures: Y write failed", res);
        }

        // Write UV planes (U and V side by side, scaled)
        uint32_t scaledUVWidth = _scaledYWidth / 2;
        uint32_t scaledUVHeight = _scaledYHeight / 2;
        uint32_t srcUVWidth = _frameWidth / 2;
        uint32_t srcUVHeight = _frameHeight / 2;
        
        std::vector<uint8_t> uvRgba(_scaledYWidth * scaledUVHeight * 4);

        for (uint32_t y = 0; y < scaledUVHeight; ++y) {
            uint32_t srcY = y * srcUVHeight / scaledUVHeight;
            for (uint32_t x = 0; x < scaledUVWidth; ++x) {
                uint32_t srcX = x * srcUVWidth / scaledUVWidth;
                size_t srcIdx = srcY * srcUVWidth + srcX;
                
                // U plane on the left
                size_t dstIdxU = (y * _scaledYWidth + x) * 4;
                uint8_t uVal = (srcIdx < _uPlane.size()) ? _uPlane[srcIdx] : 128;
                uvRgba[dstIdxU + 0] = uVal;
                uvRgba[dstIdxU + 1] = uVal;
                uvRgba[dstIdxU + 2] = uVal;
                uvRgba[dstIdxU + 3] = 255;

                // V plane on the right
                size_t dstIdxV = (y * _scaledYWidth + scaledUVWidth + x) * 4;
                uint8_t vVal = (srcIdx < _vPlane.size()) ? _vPlane[srcIdx] : 128;
                uvRgba[dstIdxV + 0] = vVal;
                uvRgba[dstIdxV + 1] = vVal;
                uvRgba[dstIdxV + 2] = vVal;
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
        // Handle timer events for frame advancement
        if (event.type == base::Event::Type::Timer) {
            if (event.timer.timerId == _timerId) {
                advanceFrame();
                return Ok(true);
            }
            return Ok(false);
        }
        
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
        // Payload is video data (MP4, AV1 OBU, etc.)
        auto data = std::span<const uint8_t>(
            reinterpret_cast<const uint8_t*>(payload.data()),
            payload.size()
        );

        // Open video source (auto-detects format)
        if (auto res = _videoSource->open(data); !res) {
            return Err<void>("YVideo::decodePayload: open failed", res);
        }

        // Get first frame - may need multiple calls due to B-frames
        std::optional<yvideo::YUVFrame> frameOpt;
        for (int attempt = 0; attempt < 30; ++attempt) {  // Try up to 30 samples
            auto frameResult = _videoSource->nextFrame();
            if (!frameResult) {
                return Err<void>("YVideo::decodePayload: nextFrame failed", frameResult);
            }
            frameOpt = *frameResult;
            if (frameOpt) {
                yinfo("YVideo::decodePayload: got frame after {} sample(s)", attempt + 1);
                break;
            }
        }
        
        if (!frameOpt) {
            // No frame ready yet
            ywarn("YVideo::decodePayload: no frame after 30 samples");
            return Ok();
        }

        auto& frame = *frameOpt;
        if (!frame.isValid()) {
            _videoSource->releaseFrame();
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

        _videoSource->releaseFrame();
        _metadataDirty = true;

        yinfo("YVideo::decodePayload: decoded frame {}x{}", _frameWidth, _frameHeight);
        
        // Start playback timer
        _frameRate = _videoSource->frameRate();
        if (auto res = startPlayback(); !res) {
            ywarn("YVideo::decodePayload: failed to start playback: {}", error_msg(res));
        }
        
        return Ok();
    }
    
    void advanceFrame() {
        if (!_videoSource) return;
        
        auto frameResult = _videoSource->nextFrame();
        if (!frameResult) {
            ywarn("YVideo::advanceFrame: nextFrame failed: {}", error_msg(frameResult));
            stopPlayback();
            return;
        }
        
        auto frameOpt = *frameResult;
        if (!frameOpt) {
            // End of video - loop back to start
            if (auto res = _videoSource->seekFrame(0); res) {
                ytrace("YVideo::advanceFrame: looping to start");
                // Try to get first frame after seek
                frameResult = _videoSource->nextFrame();
                if (frameResult) {
                    frameOpt = *frameResult;
                }
            }
            if (!frameOpt) {
                ytrace("YVideo::advanceFrame: no more frames, stopping");
                stopPlayback();
                return;
            }
        }
        
        auto& frame = *frameOpt;
        if (!frame.isValid()) {
            _videoSource->releaseFrame();
            return;
        }
        
        // Copy new frame data
        copyYUVPlanes(frame);
        _videoSource->releaseFrame();
        
        // Write updated texture data (marks textures as dirty for upload)
        if (_yTextureHandle.isValid() && !_yPlane.empty()) {
            writeTexturesToManager();
        }
        
        _metadataDirty = true;
        
        // Request screen update
        requestScreenUpdate();
    }
    
    void writeTexturesToManager() {
        if (!_cardMgr || !_cardMgr->textureManager()) return;
        
        // Write Y plane (grayscale expanded to RGBA, scaled)
        std::vector<uint8_t> yRgba(_scaledYWidth * _scaledYHeight * 4);
        
        for (uint32_t y = 0; y < _scaledYHeight; ++y) {
            uint32_t srcY = y * _frameHeight / _scaledYHeight;
            for (uint32_t x = 0; x < _scaledYWidth; ++x) {
                uint32_t srcX = x * _frameWidth / _scaledYWidth;
                size_t srcIdx = srcY * _frameWidth + srcX;
                size_t dstIdx = (y * _scaledYWidth + x) * 4;
                uint8_t val = (srcIdx < _yPlane.size()) ? _yPlane[srcIdx] : 0;
                yRgba[dstIdx + 0] = val;
                yRgba[dstIdx + 1] = val;
                yRgba[dstIdx + 2] = val;
                yRgba[dstIdx + 3] = 255;
            }
        }
        _cardMgr->textureManager()->write(_yTextureHandle, yRgba.data());
        
        // Write UV planes (scaled)
        uint32_t scaledUVWidth = _scaledYWidth / 2;
        uint32_t scaledUVHeight = _scaledYHeight / 2;
        uint32_t srcUVWidth = _frameWidth / 2;
        uint32_t srcUVHeight = _frameHeight / 2;
        
        std::vector<uint8_t> uvRgba(_scaledYWidth * scaledUVHeight * 4);
        
        for (uint32_t y = 0; y < scaledUVHeight; ++y) {
            uint32_t srcY = y * srcUVHeight / scaledUVHeight;
            for (uint32_t x = 0; x < scaledUVWidth; ++x) {
                uint32_t srcX = x * srcUVWidth / scaledUVWidth;
                size_t srcIdx = srcY * srcUVWidth + srcX;
                
                size_t dstIdxU = (y * _scaledYWidth + x) * 4;
                uint8_t uVal = (srcIdx < _uPlane.size()) ? _uPlane[srcIdx] : 128;
                uvRgba[dstIdxU + 0] = uVal;
                uvRgba[dstIdxU + 1] = uVal;
                uvRgba[dstIdxU + 2] = uVal;
                uvRgba[dstIdxU + 3] = 255;
                
                size_t dstIdxV = (y * _scaledYWidth + scaledUVWidth + x) * 4;
                uint8_t vVal = (srcIdx < _vPlane.size()) ? _vPlane[srcIdx] : 128;
                uvRgba[dstIdxV + 0] = vVal;
                uvRgba[dstIdxV + 1] = vVal;
                uvRgba[dstIdxV + 2] = vVal;
                uvRgba[dstIdxV + 3] = 255;
            }
        }
        _cardMgr->textureManager()->write(_uvTextureHandle, uvRgba.data());
    }
    
    Result<void> startPlayback() {
        if (_playing) return Ok();
        
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("YVideo::startPlayback: no EventLoop");
        }
        auto loop = *loopResult;
        
        // Create timer if needed
        if (_timerId < 0) {
            auto timerResult = loop->createTimer();
            if (!timerResult) {
                return Err<void>("YVideo::startPlayback: createTimer failed", timerResult);
            }
            _timerId = *timerResult;
            
            // Register this card as timer listener
            if (auto res = loop->registerTimerListener(_timerId, sharedAs<base::EventListener>()); !res) {
                return Err<void>("YVideo::startPlayback: registerTimerListener failed", res);
            }
        }
        
        // Configure and start timer (frame interval in ms)
        int intervalMs = static_cast<int>(1000.0f / _frameRate);
        if (intervalMs < 1) intervalMs = 1;
        
        if (auto res = loop->configTimer(_timerId, intervalMs); !res) {
            return Err<void>("YVideo::startPlayback: configTimer failed", res);
        }
        if (auto res = loop->startTimer(_timerId); !res) {
            return Err<void>("YVideo::startPlayback: startTimer failed", res);
        }
        
        _playing = true;
        yinfo("YVideo::startPlayback: started at {}fps ({}ms interval)", _frameRate, intervalMs);
        return Ok();
    }
    
    void stopPlayback() {
        if (!_playing) return;
        
        auto loopResult = base::EventLoop::instance();
        if (loopResult) {
            auto loop = *loopResult;
            if (_timerId >= 0) {
                loop->stopTimer(_timerId);
            }
        }
        
        _playing = false;
        ytrace("YVideo::stopPlayback: stopped");
    }
    
    void requestScreenUpdate() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        
        auto loop = *loopResult;
        loop->dispatch(base::Event::screenUpdateEvent());
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

    // Video source (demuxer + decoder)
    yvideo::VideoSource::Ptr _videoSource;

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
    
    // Playback timer
    base::TimerId _timerId = -1;
    float _frameRate = 30.0f;
    bool _playing = false;
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
