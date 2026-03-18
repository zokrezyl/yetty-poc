#include "yvideo-encoder.h"
#include <wels/codec_api.h>
#include <ytrace/ytrace.hpp>
#include <cstring>

namespace yetty::yvideo {

class H264Encoder : public Encoder {
public:
    H264Encoder() = default;

    ~H264Encoder() override {
        if (_encoder) {
            _encoder->Uninitialize();
            WelsDestroySVCEncoder(_encoder);
        }
    }

    Result<void> init(const EncoderConfig& config) {
        _config = config;

        if (WelsCreateSVCEncoder(&_encoder) != 0 || !_encoder) {
            return Err<void>("WelsCreateSVCEncoder failed");
        }

        // Use SEncParamExt for more control
        SEncParamExt param;
        _encoder->GetDefaultParams(&param);

        // Basic parameters
        param.iPicWidth = config.width;
        param.iPicHeight = config.height;
        param.iTargetBitrate = config.bitrate;
        param.iMaxBitrate = config.bitrate * 2;
        param.fMaxFrameRate = config.frameRate;

        // Screen content coding mode - optimized for terminal/desktop capture
        // Uses intra-block copy and palette modes for sharp edges and text
        if (config.screenContent) {
            param.iUsageType = SCREEN_CONTENT_REAL_TIME;
        } else {
            param.iUsageType = CAMERA_VIDEO_REAL_TIME;
        }

        // Single layer (no SVC)
        param.iSpatialLayerNum = 1;
        param.iTemporalLayerNum = 1;

        // Spatial layer config
        param.sSpatialLayers[0].iVideoWidth = config.width;
        param.sSpatialLayers[0].iVideoHeight = config.height;
        param.sSpatialLayers[0].fFrameRate = config.frameRate;
        param.sSpatialLayers[0].iSpatialBitrate = config.bitrate;
        param.sSpatialLayers[0].iMaxSpatialBitrate = config.bitrate * 2;

        // Profile: Baseline for maximum compatibility
        param.sSpatialLayers[0].uiProfileIdc = PRO_BASELINE;
        param.sSpatialLayers[0].uiLevelIdc = LEVEL_3_1;

        // Rate control
        param.iRCMode = RC_BITRATE_MODE;
        param.iPaddingFlag = 0;

        // IDR interval (keyframe interval)
        param.uiIntraPeriod = config.idrInterval;

        // Enable frame skipping if needed to maintain bitrate
        param.bEnableFrameSkip = false;

        // Entropy coding: CAVLC for baseline (CABAC not supported in baseline)
        param.iEntropyCodingModeFlag = 0;

        // Multi-threading
        param.iMultipleThreadIdc = 1;  // Single thread for low latency

        // Enable long-term reference for better quality
        param.bEnableLongTermReference = false;

        // Slice mode: single slice per frame
        param.sSpatialLayers[0].sSliceArgument.uiSliceMode = SM_SINGLE_SLICE;

        // Initialize encoder
        int ret = _encoder->InitializeExt(&param);
        if (ret != 0) {
            WelsDestroySVCEncoder(_encoder);
            _encoder = nullptr;
            return Err<void>("H264 encoder InitializeExt failed: " + std::to_string(ret));
        }

        // Set option to prepend SPS/PPS before each IDR
        int videoFormat = videoFormatI420;
        _encoder->SetOption(ENCODER_OPTION_DATAFORMAT, &videoFormat);

        int prependSpsPps = 1;
        _encoder->SetOption(ENCODER_OPTION_SPS_PPS_ID_STRATEGY, &prependSpsPps);

        yinfo("H264Encoder: initialized {}x{} @ {:.1f}fps, {}kbps, IDR every {} frames, screen={}",
              config.width, config.height, config.frameRate,
              config.bitrate / 1000, config.idrInterval, config.screenContent);

        return Ok();
    }

    Result<EncodedFrame> encode(
        const uint8_t* yPlane, const uint8_t* uPlane, const uint8_t* vPlane,
        uint32_t yStride, uint32_t uvStride) override
    {
        if (!_encoder) {
            return Err<EncodedFrame>("Encoder not initialized");
        }

        // Prepare source picture
        SSourcePicture srcPic = {};
        srcPic.iColorFormat = videoFormatI420;
        srcPic.iPicWidth = _config.width;
        srcPic.iPicHeight = _config.height;
        srcPic.iStride[0] = yStride;
        srcPic.iStride[1] = uvStride;
        srcPic.iStride[2] = uvStride;
        srcPic.pData[0] = const_cast<uint8_t*>(yPlane);
        srcPic.pData[1] = const_cast<uint8_t*>(uPlane);
        srcPic.pData[2] = const_cast<uint8_t*>(vPlane);
        srcPic.uiTimeStamp = _frameIndex * (1000000 / static_cast<uint64_t>(_config.frameRate));

        // Force IDR if requested
        if (_forceIDR) {
            _encoder->ForceIntraFrame(true);
            _forceIDR = false;
        }

        // Encode
        SFrameBSInfo info = {};
        int ret = _encoder->EncodeFrame(&srcPic, &info);
        if (ret != 0) {
            return Err<EncodedFrame>("EncodeFrame failed: " + std::to_string(ret));
        }

        EncodedFrame frame;
        frame.frameIndex = _frameIndex++;
        frame.timestamp = srcPic.uiTimeStamp;

        // Check if frame was produced
        if (info.eFrameType == videoFrameTypeSkip) {
            // Frame skipped due to rate control, return empty frame
            ytrace("H264Encoder: frame {} skipped", frame.frameIndex);
            return Ok(frame);
        }

        frame.isIDR = (info.eFrameType == videoFrameTypeIDR);

        // Collect all NAL units into output buffer
        for (int layer = 0; layer < info.iLayerNum; layer++) {
            const SLayerBSInfo& layerInfo = info.sLayerInfo[layer];
            int layerSize = 0;
            for (int nal = 0; nal < layerInfo.iNalCount; nal++) {
                layerSize += layerInfo.pNalLengthInByte[nal];
            }
            frame.data.insert(frame.data.end(),
                              layerInfo.pBsBuf,
                              layerInfo.pBsBuf + layerSize);
        }

        ytrace("H264Encoder: frame {} encoded, {} bytes, IDR={}",
               frame.frameIndex, frame.data.size(), frame.isIDR);

        return Ok(frame);
    }

    void forceIDR() override {
        _forceIDR = true;
    }

    const EncoderConfig& config() const override {
        return _config;
    }

    Result<void> setBitrate(uint32_t bitrate) override {
        if (!_encoder) {
            return Err<void>("Encoder not initialized");
        }

        SBitrateInfo bitrateInfo = {};
        bitrateInfo.iLayer = SPATIAL_LAYER_ALL;
        bitrateInfo.iBitrate = bitrate;
        int ret = _encoder->SetOption(ENCODER_OPTION_BITRATE, &bitrateInfo);
        if (ret != 0) {
            return Err<void>("Failed to set bitrate: " + std::to_string(ret));
        }

        _config.bitrate = bitrate;
        return Ok();
    }

private:
    ISVCEncoder* _encoder = nullptr;
    EncoderConfig _config;
    uint32_t _frameIndex = 0;
    bool _forceIDR = false;
};

Result<Encoder::Ptr> Encoder::createH264(const EncoderConfig& config) {
    auto encoder = std::make_shared<H264Encoder>();
    if (auto res = encoder->init(config); !res) {
        return Err<Ptr>("Failed to initialize H264 encoder", res);
    }
    return Ok<Ptr>(encoder);
}

} // namespace yetty::yvideo
