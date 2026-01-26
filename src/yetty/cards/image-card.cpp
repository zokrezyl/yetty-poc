#include "image-card.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>

// For image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace yetty {

ImageCard::ImageCard(CardBufferManager* mgr, int32_t x, int32_t y,
                     uint32_t widthCells, uint32_t heightCells,
                     const std::string& args, const std::string& payload)
    : Card(mgr, x, y, widthCells, heightCells)
    , argsStr_(args)
    , payloadStr_(payload)
{
    shaderGlyph_ = SHADER_GLYPH;
}

ImageCard::~ImageCard() {
    dispose();
}

Result<CardPtr> ImageCard::create(
    CardBufferManager* mgr,
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

    auto card = std::unique_ptr<ImageCard>(
        new ImageCard(mgr, x, y, widthCells, heightCells, args, payload));

    yinfo("ImageCard::create: calling init()...");
    if (auto res = card->init(); !res) {
        yerror("ImageCard::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("ImageCard::create: init failed");
    }

    yinfo("ImageCard::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok(CardPtr(card.release()));
}

Result<void> ImageCard::init() {
    // Allocate metadata slot
    auto metaResult = cardMgr_->allocateMetadata(sizeof(Metadata));
    if (!metaResult) {
        return Err<void>("ImageCard::init: failed to allocate metadata");
    }
    metaHandle_ = *metaResult;

    yinfo("ImageCard::init: allocated metadata at offset {}", metaHandle_.offset);

    // Parse args
    parseArgs(argsStr_);

    // Decode and load image from payload if provided
    if (!payloadStr_.empty()) {
        if (auto res = decodePayload(payloadStr_); !res) {
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

void ImageCard::dispose() {
    if (imageDataHandle_.isValid() && cardMgr_) {
        cardMgr_->deallocateImageData(imageDataHandle_);
        imageDataHandle_ = ImageDataHandle::invalid();
    }

    if (metaHandle_.isValid() && cardMgr_) {
        cardMgr_->deallocateMetadata(metaHandle_);
        metaHandle_ = MetadataHandle::invalid();
    }
}

void ImageCard::update(float time) {
    (void)time;

    if (metadataDirty_) {
        uploadMetadata();
        metadataDirty_ = false;
    }
}

void ImageCard::parseArgs(const std::string& args) {
    yinfo("ImageCard::parseArgs: args='{}'", args);

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--zoom" || token == "-z") {
            float val;
            if (iss >> val) {
                zoom_ = std::max(0.1f, val);
            }
        } else if (token == "--center" || token == "-c") {
            float x, y;
            if (iss >> x >> y) {
                centerX_ = std::clamp(x, 0.0f, 1.0f);
                centerY_ = std::clamp(y, 0.0f, 1.0f);
            }
        } else if (token == "--bilinear") {
            flags_ |= FLAG_BILINEAR;
        } else if (token == "--no-bilinear" || token == "--nearest") {
            flags_ &= ~FLAG_BILINEAR;
        } else if (token == "--preserve-aspect") {
            flags_ |= FLAG_PRESERVE_ASPECT;
        } else if (token == "--stretch") {
            flags_ &= ~FLAG_PRESERVE_ASPECT;
        } else if (token == "--bg-color") {
            std::string colorStr;
            if (iss >> colorStr) {
                if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                    colorStr = colorStr.substr(2);
                }
                bgColor_ = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
            }
        }
    }

    metadataDirty_ = true;
}

Result<void> ImageCard::decodePayload(const std::string& payload) {
    yinfo("ImageCard::decodePayload: payload length={}", payload.size());

    // The OSC parser already decoded the base94 payload to raw bytes.
    // The payload string contains the raw image file data (JPEG/PNG/etc).
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

Result<void> ImageCard::setImageData(const uint8_t* data, uint32_t width, uint32_t height) {
    yinfo("ImageCard::setImageData: {}x{}", width, height);

    if (!data || width == 0 || height == 0) {
        return Err<void>("ImageCard::setImageData: invalid data or dimensions");
    }

    imageWidth_ = width;
    imageHeight_ = height;

    // Deallocate old image data if exists
    if (imageDataHandle_.isValid()) {
        cardMgr_->deallocateImageData(imageDataHandle_);
        imageDataHandle_ = ImageDataHandle::invalid();
    }

    // Calculate storage size (RGBA8 = 4 bytes per pixel)
    uint32_t dataSize = width * height * 4;
    yinfo("ImageCard::setImageData: allocating {} bytes for {}x{} RGBA image",
          dataSize, width, height);

    // Allocate from image data buffer
    auto allocResult = cardMgr_->allocateImageData(dataSize);
    if (!allocResult) {
        yerror("ImageCard::setImageData: failed to allocate image data");
        return Err<void>("ImageCard::setImageData: failed to allocate image data");
    }
    imageDataHandle_ = *allocResult;

    yinfo("ImageCard::setImageData: image data allocated at byte offset {}",
          imageDataHandle_.offset);

    // Write image data to buffer
    if (auto res = cardMgr_->writeImageData(imageDataHandle_, data, dataSize); !res) {
        yerror("ImageCard::setImageData: failed to write image data");
        return Err<void>("ImageCard::setImageData: failed to write image data");
    }

    yinfo("ImageCard::setImageData: {} bytes written to image data buffer", dataSize);

    metadataDirty_ = true;
    return Ok();
}

void ImageCard::setZoom(float zoom) {
    zoom_ = std::max(0.1f, zoom);
    metadataDirty_ = true;
}

void ImageCard::setCenter(float x, float y) {
    centerX_ = std::clamp(x, 0.0f, 1.0f);
    centerY_ = std::clamp(y, 0.0f, 1.0f);
    metadataDirty_ = true;
}

void ImageCard::setBilinearFilter(bool enabled) {
    if (enabled) flags_ |= FLAG_BILINEAR;
    else flags_ &= ~FLAG_BILINEAR;
    metadataDirty_ = true;
}

void ImageCard::setPreserveAspect(bool enabled) {
    if (enabled) flags_ |= FLAG_PRESERVE_ASPECT;
    else flags_ &= ~FLAG_PRESERVE_ASPECT;
    metadataDirty_ = true;
}

void ImageCard::setAtlasPosition(uint32_t x, uint32_t y) {
    atlasX_ = x;
    atlasY_ = y;
    metadataDirty_ = true;
}

Result<void> ImageCard::uploadMetadata() {
    if (!metaHandle_.isValid()) {
        return Err<void>("ImageCard::uploadMetadata: invalid metadata handle");
    }

    Metadata meta = {};  // Zero-initialize including reserved
    meta.imageDataOffset = imageDataHandle_.isValid() ? imageDataHandle_.offset : 0;
    meta.imageWidth = imageWidth_;
    meta.imageHeight = imageHeight_;
    meta.atlasX = atlasX_;
    meta.atlasY = atlasY_;
    meta.widthCells = widthCells_;
    meta.heightCells = heightCells_;
    meta.zoom = zoom_;
    meta.centerX = centerX_;
    meta.centerY = centerY_;
    meta.flags = flags_;
    meta.bgColor = bgColor_;

    yinfo("ImageCard::uploadMetadata: metaOffset={} imageDataOffset={} "
          "imageSize={}x{} atlasPos=({},{}) widgetSize={}x{} zoom={} center=({},{}) flags={:#x}",
          metaHandle_.offset, meta.imageDataOffset,
          meta.imageWidth, meta.imageHeight,
          meta.atlasX, meta.atlasY,
          meta.widthCells, meta.heightCells,
          meta.zoom, meta.centerX, meta.centerY, meta.flags);

    if (auto res = cardMgr_->writeMetadata(metaHandle_, &meta, sizeof(meta)); !res) {
        return Err<void>("ImageCard::uploadMetadata: write failed");
    }

    return Ok();
}

void registerImageCard(CardFactory& factory) {
    factory.registerCard("image", ImageCard::create);
}

} // namespace yetty
