#pragma once

#include <yetty/result.hpp>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>

namespace yetty::yvideo {

/**
 * H.264 encoder configuration
 */
struct EncoderConfig {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t bitrate = 2000000;      // Target bitrate in bps (default 2 Mbps)
    float frameRate = 30.0f;         // Target frame rate
    uint32_t idrInterval = 60;       // Keyframe interval (frames between IDR frames)
    bool screenContent = true;       // Use screen content coding mode (optimal for terminal)
};

/**
 * Encoded frame output
 */
struct EncodedFrame {
    std::vector<uint8_t> data;       // H.264 NAL units (Annex B format with start codes)
    bool isIDR = false;              // True if this is a keyframe
    uint64_t timestamp = 0;          // Frame timestamp in microseconds
    uint32_t frameIndex = 0;         // Frame number
};

/**
 * Encoder - H.264 video encoder using openh264
 *
 * Usage:
 *   1. Create encoder with config
 *   2. For each frame: encode(yuvFrame) -> EncodedFrame
 *   3. Optionally call forceIDR() before encode to request keyframe
 */
class Encoder {
public:
    using Ptr = std::shared_ptr<Encoder>;

    virtual ~Encoder() = default;

    /**
     * Encode a YUV420 frame to H.264.
     * @param yPlane Y plane data (width * height bytes)
     * @param uPlane U plane data (width/2 * height/2 bytes)
     * @param vPlane V plane data (width/2 * height/2 bytes)
     * @param yStride Stride of Y plane in bytes
     * @param uvStride Stride of U/V planes in bytes
     * @return Encoded H.264 data
     */
    virtual Result<EncodedFrame> encode(
        const uint8_t* yPlane, const uint8_t* uPlane, const uint8_t* vPlane,
        uint32_t yStride, uint32_t uvStride) = 0;

    /**
     * Force next frame to be an IDR (keyframe).
     * Call before encode() to request a keyframe.
     */
    virtual void forceIDR() = 0;

    /**
     * Get encoder configuration.
     */
    virtual const EncoderConfig& config() const = 0;

    /**
     * Update bitrate dynamically.
     */
    virtual Result<void> setBitrate(uint32_t bitrate) = 0;

    /**
     * Create H.264 encoder (openh264).
     */
    static Result<Ptr> createH264(const EncoderConfig& config);
};

} // namespace yetty::yvideo
