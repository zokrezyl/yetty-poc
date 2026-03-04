#include "yvideo-decoder.h"
#include <dav1d/dav1d.h>
#include <wels/codec_api.h>
#define MINIMP4_IMPLEMENTATION
#include <minimp4.h>
#include <ytrace/ytrace.hpp>
#include <cstring>

namespace yetty::yvideo {

//=============================================================================
// AV1 Decoder (dav1d)
//=============================================================================

class AV1Decoder : public Decoder {
public:
    AV1Decoder() = default;

    ~AV1Decoder() override {
        if (_currentPic.data[0]) {
            dav1d_picture_unref(&_currentPic);
        }
        if (_ctx) {
            dav1d_close(&_ctx);
        }
    }

    Result<void> init() {
        Dav1dSettings settings;
        dav1d_default_settings(&settings);

        settings.n_threads = 4;
        settings.max_frame_delay = 1;

        int ret = dav1d_open(&_ctx, &settings);
        if (ret < 0) {
            return Err<void>("dav1d_open failed: " + std::to_string(ret));
        }

        yinfo("AV1Decoder: initialized dav1d with {} threads", settings.n_threads);
        return Ok();
    }

    VideoCodec codec() const override { return VideoCodec::AV1; }

    Result<void> feed(std::span<const uint8_t> data) override {
        if (!_ctx) {
            return Err<void>("Decoder not initialized");
        }

        Dav1dData dav1dData;
        uint8_t* buf = dav1d_data_create(&dav1dData, data.size());
        if (!buf) {
            return Err<void>("dav1d_data_create failed");
        }

        std::memcpy(buf, data.data(), data.size());

        int ret = dav1d_send_data(_ctx, &dav1dData);
        if (ret < 0 && ret != DAV1D_ERR(EAGAIN)) {
            dav1d_data_unref(&dav1dData);
            return Err<void>("dav1d_send_data failed: " + std::to_string(ret));
        }

        return Ok();
    }

    Result<std::optional<YUVFrame>> getFrame() override {
        if (!_ctx) {
            return Err<std::optional<YUVFrame>>("Decoder not initialized");
        }

        if (_currentPic.data[0]) {
            dav1d_picture_unref(&_currentPic);
            _currentPic = {};
        }

        int ret = dav1d_get_picture(_ctx, &_currentPic);
        if (ret == DAV1D_ERR(EAGAIN)) {
            return Ok(std::nullopt);
        }
        if (ret < 0) {
            return Err<std::optional<YUVFrame>>("dav1d_get_picture failed: " + std::to_string(ret));
        }

        if (_currentPic.p.layout != DAV1D_PIXEL_LAYOUT_I420) {
            dav1d_picture_unref(&_currentPic);
            _currentPic = {};
            return Err<std::optional<YUVFrame>>("Unsupported pixel layout (expected YUV420)");
        }

        YUVFrame frame;
        frame.yPlane = static_cast<const uint8_t*>(_currentPic.data[0]);
        frame.yStride = static_cast<uint32_t>(_currentPic.stride[0]);
        frame.uPlane = static_cast<const uint8_t*>(_currentPic.data[1]);
        frame.uStride = static_cast<uint32_t>(_currentPic.stride[1]);
        frame.vPlane = static_cast<const uint8_t*>(_currentPic.data[2]);
        frame.vStride = static_cast<uint32_t>(_currentPic.stride[1]);
        frame.width = static_cast<uint32_t>(_currentPic.p.w);
        frame.height = static_cast<uint32_t>(_currentPic.p.h);

        switch (_currentPic.seq_hdr->mtrx) {
            case DAV1D_MC_BT601:
                frame.colorMatrix = YUVFrame::ColorMatrix::BT601;
                break;
            case DAV1D_MC_BT2020_NCL:
            case DAV1D_MC_BT2020_CL:
                frame.colorMatrix = YUVFrame::ColorMatrix::BT2020;
                break;
            default:
                frame.colorMatrix = YUVFrame::ColorMatrix::BT709;
                break;
        }

        return Ok(std::make_optional(frame));
    }

    void releaseFrame() override {
        if (_currentPic.data[0]) {
            dav1d_picture_unref(&_currentPic);
            _currentPic = {};
        }
    }

    Result<void> flush() override {
        if (!_ctx) return Err<void>("Decoder not initialized");
        dav1d_send_data(_ctx, nullptr);
        return Ok();
    }

    void reset() override {
        releaseFrame();
        if (_ctx) dav1d_flush(_ctx);
    }

private:
    Dav1dContext* _ctx = nullptr;
    Dav1dPicture _currentPic = {};
};

//=============================================================================
// H.264 Decoder (openh264)
//=============================================================================

class H264Decoder : public Decoder {
public:
    H264Decoder() = default;

    ~H264Decoder() override {
        if (_decoder) {
            _decoder->Uninitialize();
            WelsDestroyDecoder(_decoder);
        }
    }

    Result<void> init() {
        if (WelsCreateDecoder(&_decoder) != 0 || !_decoder) {
            return Err<void>("WelsCreateDecoder failed");
        }

        SDecodingParam param = {};
        param.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
        param.bParseOnly = false;

        if (_decoder->Initialize(&param) != 0) {
            WelsDestroyDecoder(_decoder);
            _decoder = nullptr;
            return Err<void>("H264 decoder Initialize failed");
        }

        yinfo("H264Decoder: initialized openh264");
        return Ok();
    }

    VideoCodec codec() const override { return VideoCodec::H264; }

    Result<void> feed(std::span<const uint8_t> data) override {
        if (!_decoder) return Err<void>("Decoder not initialized");

        _pendingData.assign(data.begin(), data.end());
        return Ok();
    }

    Result<std::optional<YUVFrame>> getFrame() override {
        if (!_decoder) return Err<std::optional<YUVFrame>>("Decoder not initialized");

        if (_pendingData.empty()) {
            return Ok(std::nullopt);
        }

        uint8_t* yuvData[3] = {nullptr, nullptr, nullptr};
        SBufferInfo bufInfo = {};

        DECODING_STATE state = _decoder->DecodeFrameNoDelay(
            _pendingData.data(),
            static_cast<int>(_pendingData.size()),
            yuvData,
            &bufInfo
        );

        _pendingData.clear();

        if (state != dsErrorFree || bufInfo.iBufferStatus != 1) {
            return Ok(std::nullopt);
        }

        // Copy YUV data to internal buffer (openh264 may reuse buffers)
        int width = bufInfo.UsrData.sSystemBuffer.iWidth;
        int height = bufInfo.UsrData.sSystemBuffer.iHeight;
        int yStride = bufInfo.UsrData.sSystemBuffer.iStride[0];
        int uvStride = bufInfo.UsrData.sSystemBuffer.iStride[1];

        size_t ySize = static_cast<size_t>(yStride * height);
        size_t uvSize = static_cast<size_t>(uvStride * (height / 2));

        _yBuffer.resize(ySize);
        _uBuffer.resize(uvSize);
        _vBuffer.resize(uvSize);

        std::memcpy(_yBuffer.data(), yuvData[0], ySize);
        std::memcpy(_uBuffer.data(), yuvData[1], uvSize);
        std::memcpy(_vBuffer.data(), yuvData[2], uvSize);

        _currentFrame.yPlane = _yBuffer.data();
        _currentFrame.yStride = static_cast<uint32_t>(yStride);
        _currentFrame.uPlane = _uBuffer.data();
        _currentFrame.uStride = static_cast<uint32_t>(uvStride);
        _currentFrame.vPlane = _vBuffer.data();
        _currentFrame.vStride = static_cast<uint32_t>(uvStride);
        _currentFrame.width = static_cast<uint32_t>(width);
        _currentFrame.height = static_cast<uint32_t>(height);
        _currentFrame.colorMatrix = YUVFrame::ColorMatrix::BT709;

        return Ok(std::make_optional(_currentFrame));
    }

    void releaseFrame() override {
        _currentFrame = {};
    }

    Result<void> flush() override {
        return Ok();
    }

    void reset() override {
        _pendingData.clear();
        _currentFrame = {};
    }

private:
    ISVCDecoder* _decoder = nullptr;
    std::vector<uint8_t> _pendingData;
    std::vector<uint8_t> _yBuffer;
    std::vector<uint8_t> _uBuffer;
    std::vector<uint8_t> _vBuffer;
    YUVFrame _currentFrame;
};

//=============================================================================
// Decoder factory functions
//=============================================================================

Result<Decoder::Ptr> Decoder::createAV1() {
    auto impl = std::make_shared<AV1Decoder>();
    if (auto res = impl->init(); !res) {
        return Err<Ptr>("Failed to initialize AV1 decoder", res);
    }
    return Ok<Ptr>(impl);
}

Result<Decoder::Ptr> Decoder::createH264() {
    auto impl = std::make_shared<H264Decoder>();
    if (auto res = impl->init(); !res) {
        return Err<Ptr>("Failed to initialize H264 decoder", res);
    }
    return Ok<Ptr>(impl);
}

Result<Decoder::Ptr> Decoder::create(VideoCodec codec) {
    switch (codec) {
        case VideoCodec::AV1: return createAV1();
        case VideoCodec::H264: return createH264();
        default: return Err<Ptr>("Unsupported codec");
    }
}

//=============================================================================
// VideoSource - MP4 demuxer + decoder
//=============================================================================

class VideoSourceImpl : public VideoSource {
public:
    VideoSourceImpl() = default;
    ~VideoSourceImpl() override = default;

    Result<void> open(std::span<const uint8_t> data) override {
        _data.assign(data.begin(), data.end());
        _offset = 0;

        // Detect format by magic bytes
        if (data.size() >= 8) {
            // MP4: check for 'ftyp' box
            if (data[4] == 'f' && data[5] == 't' && data[6] == 'y' && data[7] == 'p') {
                return openMP4();
            }
            // AV1 OBU: check OBU header (type in bits 3-6)
            uint8_t obuType = (data[0] >> 3) & 0x0F;
            if (obuType >= 1 && obuType <= 8) {
                _codec = VideoCodec::AV1;
                _isRawBitstream = true;
                auto decoderRes = Decoder::createAV1();
                if (!decoderRes) return Err<void>("Failed to create AV1 decoder", decoderRes);
                _decoder = *decoderRes;
                yinfo("VideoSource: detected raw AV1 OBU stream");
                return Ok();
            }
        }

        return Err<void>("Unknown video format");
    }

    uint32_t width() const override { return _width; }
    uint32_t height() const override { return _height; }
    float frameRate() const override { return _frameRate; }
    uint32_t frameCount() const override { return static_cast<uint32_t>(_samples.size()); }
    VideoCodec codec() const override { return _codec; }

    Result<std::optional<YUVFrame>> nextFrame() override {
        if (_isRawBitstream) {
            // Feed all data at once for raw bitstream
            if (!_fedAllData) {
                if (auto res = _decoder->feed(std::span<const uint8_t>(_data)); !res) {
                    return Err<std::optional<YUVFrame>>("Failed to feed data", res);
                }
                _fedAllData = true;
            }
            return _decoder->getFrame();
        }

        // MP4: feed next sample
        if (_currentSample >= _samples.size()) {
            return Ok(std::nullopt);
        }

        auto& sample = _samples[_currentSample++];
        if (sample.offset + sample.size > _data.size()) {
            return Err<std::optional<YUVFrame>>("Sample extends beyond data");
        }

        std::span<const uint8_t> sampleData(_data.data() + sample.offset, sample.size);
        if (auto res = _decoder->feed(sampleData); !res) {
            return Err<std::optional<YUVFrame>>("Failed to feed sample", res);
        }

        return _decoder->getFrame();
    }

    Result<void> seekFrame(uint32_t frameIndex) override {
        if (frameIndex < _samples.size()) {
            _currentSample = frameIndex;
            _decoder->reset();
            return Ok();
        }
        return Err<void>("Frame index out of range");
    }

    void reset() override {
        _currentSample = 0;
        _fedAllData = false;
        if (_decoder) _decoder->reset();
    }

    void releaseFrame() override {
        if (_decoder) _decoder->releaseFrame();
    }

private:
    struct Sample {
        size_t offset;
        size_t size;
    };

    Result<void> openMP4() {
        MP4D_demux_t mp4 = {};

        int res = MP4D_open(&mp4, [](int64_t offset, void* buffer, size_t size, void* user) -> int {
            auto* self = static_cast<VideoSourceImpl*>(user);
            if (offset < 0 || static_cast<size_t>(offset) + size > self->_data.size()) {
                return 0;
            }
            std::memcpy(buffer, self->_data.data() + offset, size);
            return static_cast<int>(size);
        }, this, static_cast<int64_t>(_data.size()));

        if (res == 0) {
            return Err<void>("MP4D_open failed");
        }

        // Find video track
        int videoTrack = -1;
        for (unsigned i = 0; i < mp4.track_count; ++i) {
            if (mp4.track[i].handler_type == MP4D_HANDLER_TYPE_VIDE) {
                videoTrack = static_cast<int>(i);
                break;
            }
        }

        if (videoTrack < 0) {
            MP4D_close(&mp4);
            return Err<void>("No video track found in MP4");
        }

        auto& track = mp4.track[videoTrack];
        _width = track.SampleDescription.video.width;
        _height = track.SampleDescription.video.height;

        // Detect codec from object type
        if (track.object_type_indication == 0x21) {
            _codec = VideoCodec::H264;
        } else if (track.object_type_indication == 0xA5) {
            _codec = VideoCodec::AV1;
        } else {
            // Try to detect from codec string
            _codec = VideoCodec::H264;  // Default to H264 for MP4
        }

        // Calculate frame rate
        if (track.timescale > 0 && track.sample_count > 0) {
            _frameRate = static_cast<float>(track.timescale) /
                        static_cast<float>(track.duration_hi * 0x100000000ULL + track.duration_lo) *
                        static_cast<float>(track.sample_count);
        }

        // Extract sample offsets
        _samples.reserve(track.sample_count);
        for (unsigned i = 0; i < track.sample_count; ++i) {
            unsigned frameBytes;
            unsigned timestamp;
            unsigned duration;
            MP4D_file_offset_t ofs = MP4D_frame_offset(&mp4, videoTrack, i, &frameBytes, &timestamp, &duration);
            if (ofs != 0) {
                _samples.push_back({static_cast<size_t>(ofs), frameBytes});
            }
        }

        MP4D_close(&mp4);

        yinfo("VideoSource: opened MP4 {}x{} @ {:.1f}fps, {} frames, codec={}",
              _width, _height, _frameRate, _samples.size(),
              _codec == VideoCodec::H264 ? "H264" : "AV1");

        // Create decoder
        auto decoderRes = Decoder::create(_codec);
        if (!decoderRes) {
            return Err<void>("Failed to create decoder", decoderRes);
        }
        _decoder = *decoderRes;

        return Ok();
    }

    std::vector<uint8_t> _data;
    size_t _offset = 0;
    uint32_t _width = 0;
    uint32_t _height = 0;
    float _frameRate = 30.0f;
    VideoCodec _codec = VideoCodec::Unknown;
    bool _isRawBitstream = false;
    bool _fedAllData = false;

    std::vector<Sample> _samples;
    size_t _currentSample = 0;
    Decoder::Ptr _decoder;
};

Result<VideoSource::Ptr> VideoSource::create() {
    return Ok<Ptr>(std::make_shared<VideoSourceImpl>());
}

} // namespace yetty::yvideo
