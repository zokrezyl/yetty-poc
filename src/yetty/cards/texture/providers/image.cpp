#include "image.h"
#include <yetty/yetty-context.h>
#include <ytrace/ytrace.hpp>

#include <stb_image.h>

#include <cstring>

namespace yetty::card {

//=============================================================================
// ImageProviderImpl
//=============================================================================

class ImageProviderImpl : public ImageProvider {
public:
    ImageProviderImpl(
        const YettyContext& ctx,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload)
        : _ctx(ctx)
        , _widthCells(widthCells)
        , _heightCells(heightCells)
        , _args(args)
        , _payload(payload)
    {}

    ~ImageProviderImpl() override {
        dispose();
    }

    //=========================================================================
    // Lifecycle
    //=========================================================================

    Result<void> init() override {
        yinfo("ImageProvider::init: payload size={}", _payload.size());

        if (_payload.empty()) {
            return Err<void>("ImageProvider::init: empty payload");
        }

        // Load image using stb_image
        int width, height, channels;
        uint8_t* pixels = stbi_load_from_memory(
            reinterpret_cast<const uint8_t*>(_payload.data()),
            static_cast<int>(_payload.size()),
            &width, &height, &channels, 4);  // Force RGBA

        if (!pixels) {
            return Err<void>(std::string("ImageProvider::init: stbi_load failed: ") +
                             stbi_failure_reason());
        }

        yinfo("ImageProvider::init: loaded image {}x{} ({} channels -> 4)",
              width, height, channels);

        // Calculate target dimensions based on card size (using default cell size 10x20)
        constexpr uint32_t cellWidth = 10;
        constexpr uint32_t cellHeight = 20;
        uint32_t targetWidth = _widthCells * cellWidth;
        uint32_t targetHeight = _heightCells * cellHeight;

        // Calculate scaled dimensions preserving aspect ratio
        float imageAspect = static_cast<float>(width) / static_cast<float>(height);
        float targetAspect = static_cast<float>(targetWidth) / static_cast<float>(targetHeight);

        if (imageAspect > targetAspect) {
            // Image is wider - fit to width
            _textureWidth = targetWidth;
            _textureHeight = static_cast<uint32_t>(targetWidth / imageAspect);
        } else {
            // Image is taller - fit to height
            _textureHeight = targetHeight;
            _textureWidth = static_cast<uint32_t>(targetHeight * imageAspect);
        }
        _textureWidth = std::max(_textureWidth, 1u);
        _textureHeight = std::max(_textureHeight, 1u);

        yinfo("ImageProvider::init: scaling {}x{} -> {}x{} (target {}x{})",
              width, height, _textureWidth, _textureHeight, targetWidth, targetHeight);

        // Scale image using bilinear interpolation
        size_t dataSize = _textureWidth * _textureHeight * 4;
        _pixelData.resize(dataSize);
        scaleImageBilinear(pixels, width, height, _pixelData.data(), _textureWidth, _textureHeight);

        stbi_image_free(pixels);

        // Clear payload to free memory (we have the scaled pixels now)
        _payload.clear();
        _payload.shrink_to_fit();

        _dirty = true;
        return Ok();
    }

    // Bilinear scaling helper
    void scaleImageBilinear(const uint8_t* src, uint32_t srcW, uint32_t srcH,
                            uint8_t* dst, uint32_t dstW, uint32_t dstH) {
        for (uint32_t y = 0; y < dstH; ++y) {
            for (uint32_t x = 0; x < dstW; ++x) {
                // Map destination pixel to source coordinates
                float srcX = (x + 0.5f) * srcW / dstW - 0.5f;
                float srcY = (y + 0.5f) * srcH / dstH - 0.5f;

                // Clamp to valid range
                srcX = std::max(0.0f, std::min(srcX, static_cast<float>(srcW - 1)));
                srcY = std::max(0.0f, std::min(srcY, static_cast<float>(srcH - 1)));

                // Get integer and fractional parts
                uint32_t x0 = static_cast<uint32_t>(srcX);
                uint32_t y0 = static_cast<uint32_t>(srcY);
                uint32_t x1 = std::min(x0 + 1, srcW - 1);
                uint32_t y1 = std::min(y0 + 1, srcH - 1);
                float fx = srcX - x0;
                float fy = srcY - y0;

                // Sample 4 pixels
                const uint8_t* p00 = src + (y0 * srcW + x0) * 4;
                const uint8_t* p10 = src + (y0 * srcW + x1) * 4;
                const uint8_t* p01 = src + (y1 * srcW + x0) * 4;
                const uint8_t* p11 = src + (y1 * srcW + x1) * 4;

                // Bilinear interpolation for each channel
                uint8_t* out = dst + (y * dstW + x) * 4;
                for (int c = 0; c < 4; ++c) {
                    float top = p00[c] * (1 - fx) + p10[c] * fx;
                    float bot = p01[c] * (1 - fx) + p11[c] * fx;
                    out[c] = static_cast<uint8_t>(top * (1 - fy) + bot * fy);
                }
            }
        }
    }

    void dispose() override {
        _pixelData.clear();
        _pixelData.shrink_to_fit();
        _textureWidth = 0;
        _textureHeight = 0;
    }

    //=========================================================================
    // Rendering
    //=========================================================================

    Result<void> render(
        std::vector<uint8_t>& buffer,
        uint32_t& outWidth,
        uint32_t& outHeight) override
    {
        if (_pixelData.empty()) {
            return Err<void>("ImageProvider::render: no pixel data");
        }

        // For static images, just copy the decoded data
        buffer = _pixelData;
        outWidth = _textureWidth;
        outHeight = _textureHeight;

        yinfo("ImageProvider::render: {}x{} ({} bytes)",
              outWidth, outHeight, buffer.size());

        return Ok();
    }

    bool isDirty() const override {
        return _dirty;
    }

    void markClean() override {
        _dirty = false;
    }

    void update(float time) override {
        (void)time;
        // Static images don't update
    }

    //=========================================================================
    // Properties
    //=========================================================================

    uint32_t textureWidth() const override { return _textureWidth; }
    uint32_t textureHeight() const override { return _textureHeight; }

private:
    const YettyContext& _ctx;
    uint32_t _widthCells;
    uint32_t _heightCells;
    std::string _args;
    std::string _payload;

    std::vector<uint8_t> _pixelData;
    uint32_t _textureWidth = 0;
    uint32_t _textureHeight = 0;
    bool _dirty = false;
};

//=============================================================================
// Factory
//=============================================================================

Result<ImageProvider::Ptr> ImageProvider::create(
    const YettyContext& ctx,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("ImageProvider::create: widthCells={} heightCells={} payload_len={}",
          widthCells, heightCells, payload.size());

    auto provider = std::make_shared<ImageProviderImpl>(
        ctx, widthCells, heightCells, args, payload);

    if (auto res = provider->init(); !res) {
        yerror("ImageProvider::create: init failed: {}", error_msg(res));
        return Err<Ptr>("ImageProvider::create: init failed", res);
    }

    return Ok<Ptr>(provider);
}

} // namespace yetty::card
