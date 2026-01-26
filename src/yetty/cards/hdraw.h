#pragma once

#include <yetty/card.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
#include <vector>
#include <string>

namespace yetty::card {

//=============================================================================
// HDraw - Card for rendering SDF primitives with UNIFORM GRID SPATIAL HASHING
//
// Uses shader glyph U+100003 (card base + offset 0x0003)
//
// =============================================================================
// Architecture:
// =============================================================================
// 1. Input: SDF primitives (circle, box, bezier, etc.)
// 2. CPU: Build uniform grid - O(N) to assign primitives to cells
// 3. Fragment shader: O(1) grid lookup - pixel -> cell -> primitive list
//
// This is MUCH faster than BVH for dense/uniform scenes.
//
// =============================================================================
// Grid layout:
// =============================================================================
// Scene is divided into gridWidth x gridHeight cells.
// Each cell stores: [primCount][primIdx0][primIdx1]...[primIdxN]
// Cell lookup: cellIndex = (y / cellSize) * gridWidth + (x / cellSize)
//
// =============================================================================
// Metadata layout (64 bytes):
//   offset 0:  primitiveOffset (u32)  - offset in cardStorage (float index)
//   offset 4:  primitiveCount (u32)   - number of primitives
//   offset 8:  gridOffset (u32)       - offset of grid data in cardStorage
//   offset 12: gridWidth (u32)        - number of cells horizontally
//   offset 16: gridHeight (u32)       - number of cells vertically
//   offset 20: cellSize (f32)         - size of each cell in scene units
//   offset 24: maxPrimsPerCell (u32)  - max primitives stored per cell
//   offset 28: _reserved (u32)
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
// We forward declare here to avoid full include
struct SDFPrimitive;  // Defined in ydraw.h

//=============================================================================
// Text structures for MSDF text rendering
//=============================================================================

// Positioned glyph for GPU rendering (32 bytes)
// Stored in cardStorage, rendered by shader
struct HDrawGlyph {
    float x, y;              // Position in scene coordinates
    float width, height;     // Glyph size in scene coordinates
    uint32_t glyphIndex;     // Index in glyphMetadata buffer
    uint32_t color;          // RGBA packed color
    uint32_t layer;          // Draw order
    uint32_t _pad;           // Padding for alignment
};
static_assert(sizeof(HDrawGlyph) == 32, "HDrawGlyph must be 32 bytes");

//=============================================================================
// HDraw class
//=============================================================================

class HDraw : public Card,
              public base::ObjectFactory<HDraw> {
public:
    using Ptr = std::shared_ptr<HDraw>;

    // Shader glyph codepoint
    static constexpr uint32_t SHADER_GLYPH = 0x100003;  // Card base + 0x0003

    // Default grid parameters
    static constexpr uint32_t DEFAULT_MAX_PRIMS_PER_CELL = 16;
    static constexpr float DEFAULT_CELL_SIZE = 0.0f;  // 0 = auto-compute optimal size

    // Flags
    static constexpr uint32_t FLAG_SHOW_BOUNDS = 1;
    static constexpr uint32_t FLAG_SHOW_GRID = 2;
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

    virtual ~HDraw() = default;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> init() override = 0;
    void dispose() override = 0;
    void update(float time) override = 0;
    const char* typeName() const override { return "hdraw"; }

    // Override for 64-byte metadata slots (shader uses slotIndex * 16)
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    //=========================================================================
    // HDraw-specific API
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
    // Text is laid out horizontally starting at (x, y)
    // fontSize is in scene units
    virtual uint32_t addText(float x, float y, const std::string& text,
                             float fontSize, uint32_t color,
                             uint32_t layer = 0) = 0;

    // Clear all primitives and text
    virtual void clear() = 0;

    // Get counts
    virtual uint32_t primitiveCount() const = 0;
    virtual uint32_t glyphCount() const = 0;

    // Mark data as dirty (triggers grid rebuild on next update)
    virtual void markDirty() = 0;

    // Set background color
    virtual void setBgColor(uint32_t color) = 0;
    virtual uint32_t bgColor() const = 0;

    // Set scene bounds (if not set, computed automatically)
    virtual void setSceneBounds(float minX, float minY, float maxX, float maxY) = 0;
    virtual bool hasExplicitBounds() const = 0;

    // Set grid cell size (smaller = finer grid, more cells, more memory)
    virtual void setCellSize(float size) = 0;
    virtual float cellSize() const = 0;

protected:
    HDraw(CardBufferManager::Ptr mgr, const GPUContext& gpu,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}
};

} // namespace yetty::card
