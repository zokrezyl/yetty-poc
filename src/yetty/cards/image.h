#pragma once

#include <yetty/card.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <vector>

namespace yetty::card {

//=============================================================================
// Image - Card for rendering images via shader glyph + texture atlas
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

class Image : public Card,
              public base::ObjectFactory<Image> {
public:
    using Ptr = std::shared_ptr<Image>;

    // Shader glyph codepoint for images
    static constexpr uint32_t SHADER_GLYPH = 0x100000;  // Card base + 0x0000

    // Flags
    static constexpr uint32_t FLAG_BILINEAR = 1;
    static constexpr uint32_t FLAG_PRESERVE_ASPECT = 2;

    //=========================================================================
    // Factory method
    //=========================================================================
    static Result<CardPtr> create(
        CardBufferManager::Ptr mgr,
        const GPUContext& gpu,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    // ObjectFactory createImpl
    static Result<Ptr> createImpl(
        ContextType& ctx,
        CardBufferManager::Ptr mgr,
        const GPUContext& gpu,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    virtual ~Image() = default;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> init() override = 0;
    void dispose() override = 0;
    void update(float time) override = 0;
    const char* typeName() const override { return "image"; }

    //=========================================================================
    // Image-specific API
    //=========================================================================

    // Set image data (copies and uploads to GPU buffer)
    // Data should be RGBA8 pixels (4 bytes per pixel)
    virtual Result<void> setImageData(const uint8_t* data, uint32_t width, uint32_t height) = 0;

    // Set zoom level (1.0 = fit to cell area)
    virtual void setZoom(float zoom) = 0;
    virtual float zoom() const = 0;

    // Set pan center (0.5, 0.5 = centered)
    virtual void setCenter(float x, float y) = 0;
    virtual float centerX() const = 0;
    virtual float centerY() const = 0;

    // Set flags
    virtual void setBilinearFilter(bool enabled) = 0;
    virtual void setPreserveAspect(bool enabled) = 0;

    // Get image dimensions
    virtual uint32_t imageWidth() const = 0;
    virtual uint32_t imageHeight() const = 0;

    // Get atlas position (set by compute shader preparation)
    virtual uint32_t atlasX() const = 0;
    virtual uint32_t atlasY() const = 0;
    virtual void setAtlasPosition(uint32_t x, uint32_t y) = 0;

protected:
    Image(CardBufferManager::Ptr mgr, const GPUContext& gpu,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}
};

} // namespace yetty::card
