#pragma once

#include "../card.h"
#include "../card-factory.h"
#include <vector>

namespace yetty {

//=============================================================================
// ImageCard - Card for rendering images via shader glyph + texture atlas
//
// Uses shader glyph U+100000 (card base + offset 0x0000)
//
// =============================================================================
// ANSI-Compatible Cell Encoding (24-bit true-color)
// =============================================================================
// Cards are rendered by outputting ANSI escape sequences to vterm, allowing
// proper scrolling and scrollback buffer management. Each cell encodes:
//
//   fg (24 bits via ANSI RGB): Metadata SLOT INDEX (not byte offset!)
//     - slotIndex = byteOffset / 32 (metadata is 32-byte aligned)
//     - Shader converts: metaOffset_u32 = slotIndex * 8
//     - Addressable: 2^24 × 32 = 512MB metadata
//
//   bg (24 bits via ANSI RGB): Relative position within widget
//     - Encoding: (relRow << 12) | relCol
//     - 12 bits each → max 4096×4096 cell widgets
//
// =============================================================================
// Image Flow:
// =============================================================================
// 1. OSC command received with base94-encoded image payload (decoded by OSC parser)
// 2. ImageCard loads raw image file data (via stb_image) and stores RGBA in cardImageData buffer
// 3. Compute shader copies visible card images from buffer to atlas texture
// 4. Fragment shader samples from atlas using cell-relative UV coordinates
//
// The atlas is managed per-frame - only visible cards have their images in it.
// This allows unlimited images in scrollback without exhausting GPU memory.
//
// =============================================================================
// Metadata layout (64 bytes):
//   offset 0:  imageDataOffset (u32) - byte offset in cardImageData buffer
//   offset 4:  imageWidth (u32)      - image width in pixels
//   offset 8:  imageHeight (u32)     - image height in pixels
//   offset 12: atlasX (u32)          - X position in atlas (set by compute shader prep)
//   offset 16: atlasY (u32)          - Y position in atlas (set by compute shader prep)
//   offset 20: widthCells (u32)      - widget width in cells
//   offset 24: heightCells (u32)     - widget height in cells
//   offset 28: zoom (f32)            - zoom level (1.0 = fit, 2.0 = 200%)
//   offset 32: centerX (f32)         - pan center X in [0,1] UV space (0.5 = center)
//   offset 36: centerY (f32)         - pan center Y in [0,1] UV space (0.5 = center)
//   offset 40: flags (u32)           - bit 0: bilinear filter, bit 1: preserve aspect
//   offset 44: bgColor (u32)         - background color (packed RGBA)
//   offset 48-63: reserved (16 bytes)
//
// Note: Cell dimensions come from grid.cellSize uniform (not per-card metadata)
//=============================================================================

class ImageCard : public Card {
public:
    // Shader glyph codepoint for images
    static constexpr uint32_t SHADER_GLYPH = 0x100000;  // Card base + 0x0000

    // Flags
    static constexpr uint32_t FLAG_BILINEAR = 1;
    static constexpr uint32_t FLAG_PRESERVE_ASPECT = 2;

    //=========================================================================
    // Factory method
    //=========================================================================
    static Result<CardPtr> create(
        CardBufferManager* mgr,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload
    );

    ~ImageCard() override;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> init() override;
    void dispose() override;
    void update(float time) override;
    const char* typeName() const override { return "image"; }

    //=========================================================================
    // Image-specific API
    //=========================================================================

    // Set image data (copies and uploads to GPU buffer)
    // Data should be RGBA8 pixels (4 bytes per pixel)
    Result<void> setImageData(const uint8_t* data, uint32_t width, uint32_t height);

    // Set zoom level (1.0 = fit to cell area)
    void setZoom(float zoom);
    float zoom() const { return zoom_; }

    // Set pan center (0.5, 0.5 = centered)
    void setCenter(float x, float y);
    float centerX() const { return centerX_; }
    float centerY() const { return centerY_; }

    // Set flags
    void setBilinearFilter(bool enabled);
    void setPreserveAspect(bool enabled);

    // Get image dimensions
    uint32_t imageWidth() const { return imageWidth_; }
    uint32_t imageHeight() const { return imageHeight_; }

    // Get atlas position (set by compute shader preparation)
    uint32_t atlasX() const { return atlasX_; }
    uint32_t atlasY() const { return atlasY_; }
    void setAtlasPosition(uint32_t x, uint32_t y);

private:
    ImageCard(CardBufferManager* mgr, int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload);

    // Parse args string (e.g., "--zoom 2.0 --center 0.5 0.5")
    void parseArgs(const std::string& args);

    // Load image from payload (raw image file data, already base94-decoded by OSC parser)
    Result<void> decodePayload(const std::string& payload);

    // Upload metadata to GPU
    Result<void> uploadMetadata();

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

    uint32_t imageWidth_ = 0;
    uint32_t imageHeight_ = 0;
    float zoom_ = 1.0f;
    float centerX_ = 0.5f;
    float centerY_ = 0.5f;
    uint32_t flags_ = FLAG_BILINEAR | FLAG_PRESERVE_ASPECT;
    uint32_t bgColor_ = 0xFF000000;  // Black background

    // Atlas position (updated during render preparation)
    // Initialized to invalid (0xFFFFFFFF) - compute shader writes valid values
    // DEBUG: Set to 0 to test if fragment shader works without compute shader
    uint32_t atlasX_ = 0;
    uint32_t atlasY_ = 0;

    ImageDataHandle imageDataHandle_ = ImageDataHandle::invalid();
    bool metadataDirty_ = true;

    std::string argsStr_;
    std::string payloadStr_;
};

// Register ImageCard with CardFactory
void registerImageCard(CardFactory& factory);

} // namespace yetty
