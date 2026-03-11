#pragma once

#include <yetty/result.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace yetty::yvideo {

/**
 * YUV frame data from video decoder.
 * Contains pointers to Y, U, V planes in YUV420 format.
 */
struct YUVFrame {
    // Y plane (full resolution)
    const uint8_t* yPlane = nullptr;
    uint32_t yStride = 0;

    // U plane (half resolution in both dimensions)
    const uint8_t* uPlane = nullptr;
    uint32_t uStride = 0;

    // V plane (half resolution in both dimensions)
    const uint8_t* vPlane = nullptr;
    uint32_t vStride = 0;

    // Frame dimensions (Y plane size)
    uint32_t width = 0;
    uint32_t height = 0;

    // Color matrix type for YUV→RGB conversion
    enum class ColorMatrix {
        BT601,   // SD content (ITU-R BT.601)
        BT709,   // HD content (ITU-R BT.709)
        BT2020,  // UHD/HDR content
    };
    ColorMatrix colorMatrix = ColorMatrix::BT709;

    bool isValid() const {
        return yPlane && uPlane && vPlane && width > 0 && height > 0;
    }
};

/**
 * Supported video codecs
 */
enum class VideoCodec {
    Unknown,
    H264,    // openh264
    AV1,     // dav1d
    VP9,     // future: libvpx
};

/**
 * Decoder - Base class for video decoders
 *
 * Implementations: AV1Decoder (dav1d), H264Decoder (openh264)
 */
class Decoder {
public:
    using Ptr = std::shared_ptr<Decoder>;

    virtual ~Decoder() = default;

    /**
     * Feed compressed data to the decoder.
     * May not produce a frame immediately due to frame reordering.
     */
    virtual Result<void> feed(std::span<const uint8_t> data) = 0;

    /**
     * Get the next decoded frame if available.
     */
    virtual Result<std::optional<YUVFrame>> getFrame() = 0;

    /**
     * Release the current frame back to the decoder.
     */
    virtual void releaseFrame() = 0;

    /**
     * Flush the decoder - retrieve all remaining frames.
     */
    virtual Result<void> flush() = 0;

    /**
     * Reset decoder state for new stream.
     */
    virtual void reset() = 0;

    /**
     * Get the codec type this decoder handles.
     */
    virtual VideoCodec codec() const = 0;

    /**
     * Create AV1 decoder (dav1d).
     */
    static Result<Ptr> createAV1();

    /**
     * Create H.264 decoder (openh264).
     */
    static Result<Ptr> createH264();

    /**
     * Create decoder for specified codec.
     */
    static Result<Ptr> create(VideoCodec codec);
};

/**
 * VideoSource - Demuxer + decoder for video files
 *
 * Handles container formats (MP4, WebM, OBU) and routes to appropriate decoder.
 * Provides frame-by-frame playback interface.
 */
class VideoSource {
public:
    using Ptr = std::shared_ptr<VideoSource>;

    virtual ~VideoSource() = default;

    /**
     * Open a video file or memory buffer.
     */
    virtual Result<void> open(std::span<const uint8_t> data) = 0;

    /**
     * Get video dimensions.
     */
    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;

    /**
     * Get frame rate (fps).
     */
    virtual float frameRate() const = 0;

    /**
     * Get total frame count (0 if unknown/streaming).
     */
    virtual uint32_t frameCount() const = 0;

    /**
     * Get the detected codec.
     */
    virtual VideoCodec codec() const = 0;

    /**
     * Decode and return the next frame.
     */
    virtual Result<std::optional<YUVFrame>> nextFrame() = 0;

    /**
     * Seek to a specific frame (if supported).
     */
    virtual Result<void> seekFrame(uint32_t frameIndex) = 0;

    /**
     * Reset to beginning.
     */
    virtual void reset() = 0;

    /**
     * Release current frame resources.
     */
    virtual void releaseFrame() = 0;

    /**
     * Create a VideoSource that auto-detects format.
     * Supports: MP4 (H.264), raw AV1 OBU
     */
    static Result<Ptr> create();
};

} // namespace yetty::yvideo
