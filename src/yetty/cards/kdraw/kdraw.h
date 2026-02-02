#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
#include <vector>
#include <string>

namespace yetty::card {

//=============================================================================
// KDraw - Card for rendering SDF primitives with COMPUTE SHADER TILE CULLING
//
// Uses shader glyph U+100004 (card base + offset 0x0004)
//
// =============================================================================
// Architecture:
// =============================================================================
// 1. Input: SDF primitives (circle, box, bezier, etc.)
// 2. Compute shader: For each 8x8 pixel tile, build compact primitive list
// 3. Fragment shader: Read tile's compact list, evaluate only those primitives
//
// This approach:
// - Uses screen-space tiles (always 8x8 pixels regardless of zoom)
// - Leverages GPU shared memory for tile-local primitive lists
// - All 64 threads in a tile share the same primitive list (no divergence)
// - Much better cache coherency than scene-space grids
//
// =============================================================================
// Buffer layout:
// =============================================================================
// 1. Primitives buffer: SDFPrimitive array
// 2. Tile lists buffer: Per-tile compact primitive indices
//    - Organized as: [tile0_count, tile0_idx0, tile0_idx1, ..., tile1_count, ...]
//    - Each tile has MAX_PRIMS_PER_TILE + 1 slots (count + indices)
//
// =============================================================================
// Metadata layout (64 bytes):
//   offset 0:  primitiveOffset (u32)  - offset in cardStorage (float index)
//   offset 4:  primitiveCount (u32)   - number of primitives
//   offset 8:  tileListOffset (u32)   - offset of tile lists in cardStorage
//   offset 12: tileCountX (u32)       - number of tiles horizontally
//   offset 16: tileCountY (u32)       - number of tiles vertically
//   offset 20: tileSize (u32)         - tile size in pixels (8)
//   offset 24: glyphOffset (u32)      - offset of glyph data
//   offset 28: glyphCount (u32)       - number of glyphs
//   offset 32: sceneMinX (f32)
//   offset 36: sceneMinY (f32)
//   offset 40: sceneMaxX (f32)
//   offset 44: sceneMaxY (f32)
//   offset 48: widthCells (u32)       - widget width in terminal cells
//   offset 52: heightCells (u32)      - widget height in terminal cells
//   offset 56: flags (u32)
//   offset 60: bgColor (u32)
//=============================================================================

// Import SDFPrimitive from ydraw.h (same format)
struct SDFPrimitive;  // Defined in ydraw.h

// Positioned glyph for GPU rendering (32 bytes) - same as HDraw
struct KDrawGlyph {
    float x, y;              // Position in scene coordinates
    float width, height;     // Glyph size in scene coordinates
    uint32_t glyphIndex;     // Index in glyphMetadata buffer
    uint32_t color;          // RGBA packed color
    uint32_t layer;          // Draw order
    uint32_t _pad;           // Padding for alignment
};
static_assert(sizeof(KDrawGlyph) == 32, "KDrawGlyph must be 32 bytes");

//=============================================================================
// KDraw class
//=============================================================================

class KDraw : public Card,
              public base::ObjectFactory<KDraw> {
public:
    using Ptr = std::shared_ptr<KDraw>;

    // Shader glyph codepoint
    static constexpr uint32_t SHADER_GLYPH = 0x100004;  // Card base + 0x0004

    // Tile parameters
    static constexpr uint32_t TILE_SIZE = 8;  // 8x8 pixels per tile
    static constexpr uint32_t MAX_PRIMS_PER_TILE = 16;  // Max primitives per tile

    // Flags
    static constexpr uint32_t FLAG_SHOW_BOUNDS = 1;
    static constexpr uint32_t FLAG_SHOW_TILES = 2;
    static constexpr uint32_t FLAG_SHOW_EVAL_COUNT = 4;

    //=========================================================================
    // Factory method
    //=========================================================================
    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    // ObjectFactory createImpl
    static Result<Ptr> createImpl(
        ContextType& ctx,
        const YettyContext& yettyCtx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    virtual ~KDraw() = default;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> dispose() override = 0;
    Result<void> render(float time) override = 0;
    const char* typeName() const override { return "kdraw"; }
    bool needsBuffer() const override { return true; }

    // Override for 64-byte metadata slots
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    //=========================================================================
    // KDraw-specific API
    //=========================================================================

    // Add SDF primitive (returns primitive index)
    virtual uint32_t addPrimitive(const SDFPrimitive& prim) = 0;

    // Convenience methods for common primitives
    virtual uint32_t addCircle(float cx, float cy, float radius,
                               uint32_t fillColor, uint32_t strokeColor = 0,
                               float strokeWidth = 0, uint32_t layer = 0) = 0;

    virtual uint32_t addBox(float cx, float cy, float halfW, float halfH,
                            uint32_t fillColor, uint32_t strokeColor = 0,
                            float strokeWidth = 0, float round = 0,
                            uint32_t layer = 0) = 0;

    virtual uint32_t addSegment(float x0, float y0, float x1, float y1,
                                uint32_t strokeColor, float strokeWidth = 1,
                                uint32_t layer = 0) = 0;

    virtual uint32_t addBezier2(float x0, float y0, float x1, float y1,
                                float x2, float y2,
                                uint32_t strokeColor, float strokeWidth = 1,
                                uint32_t layer = 0) = 0;

    // Add text at position (returns number of glyphs added)
    virtual uint32_t addText(float x, float y, const std::string& text,
                             float fontSize, uint32_t color,
                             uint32_t layer = 0) = 0;

    // Clear all primitives and text
    virtual void clear() = 0;

    // Get counts
    virtual uint32_t primitiveCount() const = 0;
    virtual uint32_t glyphCount() const = 0;

    // Mark data as dirty (triggers tile rebuild on next update)
    virtual void markDirty() = 0;

    // Set background color
    virtual void setBgColor(uint32_t color) = 0;
    virtual uint32_t bgColor() const = 0;

    // Set scene bounds (if not set, computed automatically)
    virtual void setSceneBounds(float minX, float minY, float maxX, float maxY) = 0;
    virtual bool hasExplicitBounds() const = 0;

    // Run compute shader to build tile lists (called automatically in update)
    virtual void runTileCulling() = 0;

protected:
    KDraw(CardManager::Ptr mgr, const GPUContext& gpu,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}
};

} // namespace yetty::card
