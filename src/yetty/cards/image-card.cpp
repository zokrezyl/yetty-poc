#include "image-card.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>

// For image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace yetty {

//=============================================================================
// ImageCardImpl - Implementation of ImageCard
//=============================================================================

class ImageCardImpl : public ImageCard {
public:
    ImageCardImpl(CardBufferManager::Ptr mgr, const GPUContext& gpu,
                  int32_t x, int32_t y,
                  uint32_t widthCells, uint32_t heightCells,
                  const std::string& args, const std::string& payload)
        : ImageCard(std::move(mgr), gpu, x, y, widthCells, heightCells)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~ImageCardImpl() override {
        dispose();
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() override {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("ImageCard::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("ImageCard::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Decode and load image from payload if provided
        if (!_payloadStr.empty()) {
            if (auto res = decodePayload(_payloadStr); !res) {
                yerror("ImageCard::init: failed to decode payload: {}", error_msg(res));
                return Err<void>("ImageCard::init: failed to decode payload");
            }
        }

        // Upload initial metadata
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("ImageCard::init: failed to upload metadata");
        }

        return Ok();
    }

    void dispose() override {
        if (_imageDataHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateImageData(_imageDataHandle);
            _imageDataHandle = ImageDataHandle::invalid();
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }
    }

    void update(float time) override {
        (void)time;

        if (_metadataDirty) {
            uploadMetadata();
            _metadataDirty = false;
        }
    }

    //=========================================================================
    // Image-specific API
    //=========================================================================

    Result<void> setImageData(const uint8_t* data, uint32_t width, uint32_t height) override {
        yinfo("ImageCard::setImageData: {}x{}", width, height);

        if (!data || width == 0 || height == 0) {
            return Err<void>("ImageCard::setImageData: invalid data or dimensions");
        }

        _imageWidth = width;
        _imageHeight = height;

        // Deallocate old image data if exists
        if (_imageDataHandle.isValid()) {
            _cardMgr->deallocateImageData(_imageDataHandle);
            _imageDataHandle = ImageDataHandle::invalid();
        }

        // Calculate storage size (RGBA8 = 4 bytes per pixel)
        uint32_t dataSize = width * height * 4;
        yinfo("ImageCard::setImageData: allocating {} bytes for {}x{} RGBA image",
              dataSize, width, height);

        // Allocate from image data buffer
        auto allocResult = _cardMgr->allocateImageData(dataSize);
        if (!allocResult) {
            yerror("ImageCard::setImageData: failed to allocate image data");
            return Err<void>("ImageCard::setImageData: failed to allocate image data");
        }
        _imageDataHandle = *allocResult;

        yinfo("ImageCard::setImageData: image data allocated at byte offset {}",
              _imageDataHandle.offset);

        // Write image data to buffer
        if (auto res = _cardMgr->writeImageData(_imageDataHandle, data, dataSize); !res) {
            yerror("ImageCard::setImageData: failed to write image data");
            return Err<void>("ImageCard::setImageData: failed to write image data");
        }

        yinfo("ImageCard::setImageData: {} bytes written to image data buffer", dataSize);

        _metadataDirty = true;
        return Ok();
    }

    void setZoom(float zoom) override {
        _zoom = std::max(0.1f, zoom);
        _metadataDirty = true;
    }

    float zoom() const override { return _zoom; }

    void setCenter(float x, float y) override {
        _centerX = std::clamp(x, 0.0f, 1.0f);
        _centerY = std::clamp(y, 0.0f, 1.0f);
        _metadataDirty = true;
    }

    float centerX() const override { return _centerX; }
    float centerY() const override { return _centerY; }

    void setBilinearFilter(bool enabled) override {
        if (enabled) _flags |= FLAG_BILINEAR;
        else _flags &= ~FLAG_BILINEAR;
        _metadataDirty = true;
    }

    void setPreserveAspect(bool enabled) override {
        if (enabled) _flags |= FLAG_PRESERVE_ASPECT;
        else _flags &= ~FLAG_PRESERVE_ASPECT;
        _metadataDirty = true;
    }

    uint32_t imageWidth() const override { return _imageWidth; }
    uint32_t imageHeight() const override { return _imageHeight; }

    uint32_t atlasX() const override { return _atlasX; }
    uint32_t atlasY() const override { return _atlasY; }

    void setAtlasPosition(uint32_t x, uint32_t y) override {
        _atlasX = x;
        _atlasY = y;
        _metadataDirty = true;
    }

private:
    void parseArgs(const std::string& args) {
        yinfo("ImageCard::parseArgs: args='{}'", args);

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--zoom" || token == "-z") {
                float val;
                if (iss >> val) {
                    _zoom = std::max(0.1f, val);
                }
            } else if (token == "--center" || token == "-c") {
                float x, y;
                if (iss >> x >> y) {
                    _centerX = std::clamp(x, 0.0f, 1.0f);
                    _centerY = std::clamp(y, 0.0f, 1.0f);
                }
            } else if (token == "--bilinear") {
                _flags |= FLAG_BILINEAR;
            } else if (token == "--no-bilinear" || token == "--nearest") {
                _flags &= ~FLAG_BILINEAR;
            } else if (token == "--preserve-aspect") {
                _flags |= FLAG_PRESERVE_ASPECT;
            } else if (token == "--stretch") {
                _flags &= ~FLAG_PRESERVE_ASPECT;
            } else if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                        colorStr = colorStr.substr(2);
                    }
                    _bgColor = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
                }
            }
        }

        _metadataDirty = true;
    }

    Result<void> decodePayload(const std::string& payload) {
        yinfo("ImageCard::decodePayload: payload length={}", payload.size());

        if (payload.empty()) {
            return Err<void>("ImageCard::decodePayload: empty payload");
        }

        // Convert string to byte vector (string contains raw binary data)
        std::vector<uint8_t> imageFileData(payload.begin(), payload.end());
        yinfo("ImageCard::decodePayload: image file data {} bytes", imageFileData.size());

        // Load image using stb_image
        int width, height, channels;
        uint8_t* pixels = stbi_load_from_memory(
            imageFileData.data(),
            static_cast<int>(imageFileData.size()),
            &width, &height, &channels, 4);  // Force RGBA

        if (!pixels) {
            return Err<void>(std::string("ImageCard::decodePayload: stbi_load failed: ") +
                             stbi_failure_reason());
        }

        yinfo("ImageCard::decodePayload: loaded image {}x{} ({} channels -> 4)",
              width, height, channels);

        // Set image data (this will upload to GPU)
        auto result = setImageData(pixels, static_cast<uint32_t>(width),
                                    static_cast<uint32_t>(height));

        stbi_image_free(pixels);

        return result;
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("ImageCard::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};  // Zero-initialize including reserved
        meta.imageDataOffset = _imageDataHandle.isValid() ? _imageDataHandle.offset : 0;
        meta.imageWidth = _imageWidth;
        meta.imageHeight = _imageHeight;
        meta.atlasX = _atlasX;
        meta.atlasY = _atlasY;
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.zoom = _zoom;
        meta.centerX = _centerX;
        meta.centerY = _centerY;
        meta.flags = _flags;
        meta.bgColor = _bgColor;

        yinfo("ImageCard::uploadMetadata: metaOffset={} imageDataOffset={} "
              "imageSize={}x{} atlasPos=({},{}) widgetSize={}x{} zoom={} center=({},{}) flags={:#x}",
              _metaHandle.offset, meta.imageDataOffset,
              meta.imageWidth, meta.imageHeight,
              meta.atlasX, meta.atlasY,
              meta.widthCells, meta.heightCells,
              meta.zoom, meta.centerX, meta.centerY, meta.flags);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("ImageCard::uploadMetadata: write failed");
        }

        return Ok();
    }

    // Metadata structure (matches shader layout)
    struct Metadata {
        uint32_t imageDataOffset;  // Byte offset in cardImageData buffer
        uint32_t imageWidth;       // Image width in pixels
        uint32_t imageHeight;      // Image height in pixels
        uint32_t atlasX;           // Atlas X position (set during render prep)
        uint32_t atlasY;           // Atlas Y position (set during render prep)
        uint32_t widthCells;       // Widget width in cells
        uint32_t heightCells;      // Widget height in cells
        float zoom;                // Zoom level
        float centerX;             // Pan center X
        float centerY;             // Pan center Y
        uint32_t flags;            // Rendering flags
        uint32_t bgColor;          // Background color
        uint32_t _reserved[4];     // Padding to 64 bytes
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    uint32_t _imageWidth = 0;
    uint32_t _imageHeight = 0;
    float _zoom = 1.0f;
    float _centerX = 0.5f;
    float _centerY = 0.5f;
    uint32_t _flags = FLAG_BILINEAR | FLAG_PRESERVE_ASPECT;
    uint32_t _bgColor = 0xFF000000;  // Black background

    uint32_t _atlasX = 0;
    uint32_t _atlasY = 0;

    ImageDataHandle _imageDataHandle = ImageDataHandle::invalid();
    bool _metadataDirty = true;

    std::string _argsStr;
    std::string _payloadStr;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> ImageCard::create(
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("ImageCard::create: ENTERED pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!mgr) {
        yerror("ImageCard::create: null CardBufferManager!");
        return Err<CardPtr>("ImageCard::create: null CardBufferManager");
    }

    auto card = std::make_shared<ImageCardImpl>(
        std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);

    yinfo("ImageCard::create: calling init()...");
    if (auto res = card->init(); !res) {
        yerror("ImageCard::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("ImageCard::create: init failed");
    }

    yinfo("ImageCard::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<ImageCard::Ptr> ImageCard::createImpl(
    ContextType& ctx,
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx; // ObjectFactory context marker

    auto result = create(std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);
    if (!result) {
        return Err<Ptr>("Failed to create ImageCard", result);
    }
    // Dynamic cast from CardPtr to ImageCard::Ptr
    auto imageCard = std::dynamic_pointer_cast<ImageCard>(*result);
    if (!imageCard) {
        return Err<Ptr>("Created card is not an ImageCard");
    }
    return Ok(imageCard);
}

} // namespace yetty
