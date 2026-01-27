#include "texture.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>

// For image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace yetty::card {

//=============================================================================
// TextureImpl - Implementation of Texture
//=============================================================================

class TextureImpl : public Texture {
public:
    TextureImpl(CardBufferManager::Ptr mgr, const GPUContext& gpu,
                int32_t x, int32_t y,
                uint32_t widthCells, uint32_t heightCells,
                const std::string& args, const std::string& payload)
        : Texture(std::move(mgr), gpu, x, y, widthCells, heightCells)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~TextureImpl() override {
        dispose();
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() override {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("Texture::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("Texture::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Decode and load texture from payload if provided
        if (!_payloadStr.empty()) {
            if (auto res = decodePayload(_payloadStr); !res) {
                yerror("Texture::init: failed to decode payload: {}", error_msg(res));
                return Err<void>("Texture::init: failed to decode payload");
            }
        }

        // Upload initial metadata
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("Texture::init: failed to upload metadata");
        }

        return Ok();
    }

    void dispose() override {
        if (_textureDataHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateImageData(_textureDataHandle);
            _textureDataHandle = ImageDataHandle::invalid();
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
    // Texture-specific API
    //=========================================================================

    Result<void> setTextureData(const uint8_t* data, uint32_t width, uint32_t height) override {
        yinfo("Texture::setTextureData: {}x{}", width, height);

        if (!data || width == 0 || height == 0) {
            return Err<void>("Texture::setTextureData: invalid data or dimensions");
        }

        _textureWidth = width;
        _textureHeight = height;

        // Deallocate old texture data if exists
        if (_textureDataHandle.isValid()) {
            _cardMgr->deallocateImageData(_textureDataHandle);
            _textureDataHandle = ImageDataHandle::invalid();
        }

        // Calculate storage size (RGBA8 = 4 bytes per pixel)
        uint32_t dataSize = width * height * 4;
        yinfo("Texture::setTextureData: allocating {} bytes for {}x{} RGBA texture",
              dataSize, width, height);

        // Allocate from texture data buffer
        auto allocResult = _cardMgr->allocateImageData(dataSize);
        if (!allocResult) {
            yerror("Texture::setTextureData: failed to allocate texture data");
            return Err<void>("Texture::setTextureData: failed to allocate texture data");
        }
        _textureDataHandle = *allocResult;

        yinfo("Texture::setTextureData: texture data allocated at byte offset {}",
              _textureDataHandle.offset);

        // Write texture data to buffer
        if (auto res = _cardMgr->writeImageData(_textureDataHandle, data, dataSize); !res) {
            yerror("Texture::setTextureData: failed to write texture data");
            return Err<void>("Texture::setTextureData: failed to write texture data");
        }

        yinfo("Texture::setTextureData: {} bytes written to texture data buffer", dataSize);

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

    uint32_t textureWidth() const override { return _textureWidth; }
    uint32_t textureHeight() const override { return _textureHeight; }

    uint32_t atlasX() const override { return _atlasX; }
    uint32_t atlasY() const override { return _atlasY; }

    void setAtlasPosition(uint32_t x, uint32_t y) override {
        _atlasX = x;
        _atlasY = y;
        _metadataDirty = true;
    }

private:
    void parseArgs(const std::string& args) {
        yinfo("Texture::parseArgs: args='{}'", args);

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
        yinfo("Texture::decodePayload: payload length={}", payload.size());

        if (payload.empty()) {
            return Err<void>("Texture::decodePayload: empty payload");
        }

        // Convert string to byte vector (string contains raw binary data)
        std::vector<uint8_t> textureFileData(payload.begin(), payload.end());
        yinfo("Texture::decodePayload: texture file data {} bytes", textureFileData.size());

        // Load texture using stb_image
        int width, height, channels;
        uint8_t* pixels = stbi_load_from_memory(
            textureFileData.data(),
            static_cast<int>(textureFileData.size()),
            &width, &height, &channels, 4);  // Force RGBA

        if (!pixels) {
            return Err<void>(std::string("Texture::decodePayload: stbi_load failed: ") +
                             stbi_failure_reason());
        }

        yinfo("Texture::decodePayload: loaded texture {}x{} ({} channels -> 4)",
              width, height, channels);

        // Set texture data (this will upload to GPU)
        auto result = setTextureData(pixels, static_cast<uint32_t>(width),
                                     static_cast<uint32_t>(height));

        stbi_image_free(pixels);

        return result;
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("Texture::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};  // Zero-initialize including reserved
        meta.textureDataOffset = _textureDataHandle.isValid() ? _textureDataHandle.offset : 0;
        meta.textureWidth = _textureWidth;
        meta.textureHeight = _textureHeight;
        meta.atlasX = _atlasX;
        meta.atlasY = _atlasY;
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.zoom = _zoom;
        meta.centerX = _centerX;
        meta.centerY = _centerY;
        meta.flags = _flags;
        meta.bgColor = _bgColor;

        yinfo("Texture::uploadMetadata: metaOffset={} textureDataOffset={} "
              "textureSize={}x{} atlasPos=({},{}) widgetSize={}x{} zoom={} center=({},{}) flags={:#x}",
              _metaHandle.offset, meta.textureDataOffset,
              meta.textureWidth, meta.textureHeight,
              meta.atlasX, meta.atlasY,
              meta.widthCells, meta.heightCells,
              meta.zoom, meta.centerX, meta.centerY, meta.flags);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("Texture::uploadMetadata: write failed");
        }

        return Ok();
    }

    // Metadata structure (matches shader layout)
    struct Metadata {
        uint32_t textureDataOffset;  // Byte offset in cardImageData buffer
        uint32_t textureWidth;       // Texture width in pixels
        uint32_t textureHeight;      // Texture height in pixels
        uint32_t atlasX;             // Atlas X position (set during render prep)
        uint32_t atlasY;             // Atlas Y position (set during render prep)
        uint32_t widthCells;         // Widget width in cells
        uint32_t heightCells;        // Widget height in cells
        float zoom;                  // Zoom level
        float centerX;               // Pan center X
        float centerY;               // Pan center Y
        uint32_t flags;              // Rendering flags
        uint32_t bgColor;            // Background color
        uint32_t _reserved[4];       // Padding to 64 bytes
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    uint32_t _textureWidth = 0;
    uint32_t _textureHeight = 0;
    float _zoom = 1.0f;
    float _centerX = 0.5f;
    float _centerY = 0.5f;
    uint32_t _flags = FLAG_BILINEAR | FLAG_PRESERVE_ASPECT;
    uint32_t _bgColor = 0xFF000000;  // Black background

    uint32_t _atlasX = 0;
    uint32_t _atlasY = 0;

    ImageDataHandle _textureDataHandle = ImageDataHandle::invalid();
    bool _metadataDirty = true;

    std::string _argsStr;
    std::string _payloadStr;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> Texture::create(
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("Texture::create: ENTERED pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!mgr) {
        yerror("Texture::create: null CardBufferManager!");
        return Err<CardPtr>("Texture::create: null CardBufferManager");
    }

    auto card = std::make_shared<TextureImpl>(
        std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);

    yinfo("Texture::create: calling init()...");
    if (auto res = card->init(); !res) {
        yerror("Texture::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("Texture::create: init failed");
    }

    yinfo("Texture::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<Texture::Ptr> Texture::createImpl(
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
        return Err<Ptr>("Failed to create Texture", result);
    }
    // Dynamic cast from CardPtr to Texture::Ptr
    auto textureCard = std::dynamic_pointer_cast<Texture>(*result);
    if (!textureCard) {
        return Err<Ptr>("Created card is not a Texture");
    }
    return Ok(textureCard);
}

} // namespace yetty::card
