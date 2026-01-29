#pragma once

#include <yetty/card.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <vector>
#include <string>

namespace yetty::card {

//=============================================================================
// YDraw - Card for rendering SDF primitives + MSDF text via BVH acceleration
//
// Uses shader glyph U+100002 (card base + offset 0x0002)
//
// =============================================================================
// Architecture:
// =============================================================================
// 1. Input: SDF primitives (circle, box, bezier, etc.) + text spans
// 2. Compute shader: BVH construction + text layout (glyph positioning)
// 3. Fragment shader: BVH traversal + SDF eval + MSDF text sampling
//
// Layer ordering is preserved - lower layers render first (can be overdrawn)
//
// =============================================================================
// SDF Primitive Types:
// =============================================================================
//   0 = Circle:     params[0-1]=center, params[2]=radius
//   1 = Box:        params[0-1]=center, params[2-3]=halfSize
//   2 = Segment:    params[0-1]=p0, params[2-3]=p1
//   3 = Triangle:   params[0-5]=p0,p1,p2
//   4 = Bezier2:    params[0-5]=p0,p1,p2 (quadratic)
//   5 = Bezier3:    params[0-7]=p0,p1,p2,p3 (cubic)
//   6 = Ellipse:    params[0-1]=center, params[2-3]=radii
//   7 = Arc:        params[0-1]=center, params[2-3]=sc, params[4-5]=ra,rb
//  64 = TextGlyph:  params[0-1]=position, params[2]=glyphIndex, params[3]=fontSize
//
// =============================================================================
// Metadata layout (64 bytes):
//   offset 0:  primitiveOffset (u32)  - offset in cardStorage (float index)
//   offset 4:  primitiveCount (u32)   - number of primitives
//   offset 8:  textSpanOffset (u32)   - offset in cardStorage (float index)
//   offset 12: textSpanCount (u32)    - number of text spans
//   offset 16: bvhOffset (u32)        - offset of BVH nodes in cardStorage
//   offset 20: bvhNodeCount (u32)     - number of BVH nodes
//   offset 24: glyphOffset (u32)      - offset of positioned glyphs
//   offset 28: glyphCount (u32)       - number of positioned glyphs
//   offset 32: sceneMinX (f32)        - scene bounds for Morton codes
//   offset 36: sceneMinY (f32)
//   offset 40: sceneMaxX (f32)
//   offset 44: sceneMaxY (f32)
//   offset 48: widthCells (u32)       - widget width in cells
//   offset 52: heightCells (u32)      - widget height in cells
//   offset 56: flags (u32)            - rendering flags
//   offset 60: bgColor (u32)          - background color (packed RGBA)
//=============================================================================

//=============================================================================
// Data structures for SDF primitives and text
//=============================================================================

// SDF primitive types
enum class SDFType : uint32_t {
    Circle = 0,
    Box = 1,
    Segment = 2,
    Triangle = 3,
    Bezier2 = 4,    // Quadratic bezier
    Bezier3 = 5,    // Cubic bezier
    Ellipse = 6,
    Arc = 7,
    RoundedBox = 8, // Box with per-corner radii
    Rhombus = 9,
    Pentagon = 10,
    Hexagon = 11,
    Star = 12,
    Pie = 13,
    Ring = 14,
    Heart = 15,
    Cross = 16,
    RoundedX = 17,
    Capsule = 18,
    Moon = 19,
    Egg = 20,
    // Text glyph (handled specially)
    TextGlyph = 64
};

// SDF primitive (96 bytes, 24 floats - GPU aligned)
struct SDFPrimitive {
    uint32_t type;           // SDFType
    uint32_t layer;          // Draw order (lower = earlier)
    float params[12];        // Type-specific parameters
    uint32_t fillColor;      // RGBA packed
    uint32_t strokeColor;    // RGBA packed
    float strokeWidth;
    float round;             // Corner rounding
    // AABB (computed or set)
    float aabbMinX, aabbMinY;
    float aabbMaxX, aabbMaxY;
    uint32_t _pad[2];        // Padding to 96 bytes (multiple of 16 for GPU)
};
static_assert(sizeof(SDFPrimitive) == 96, "SDFPrimitive must be 96 bytes");

// Text span (input format, 64 bytes)
struct TextSpan {
    uint32_t layer;          // Draw order
    float x, y;              // Position (line start)
    float fontSize;
    uint32_t color;          // RGBA packed
    uint32_t textOffset;     // Offset into text buffer (bytes)
    uint32_t textLength;     // Length in bytes (UTF-8)
    uint32_t fontId;         // Font family index (0 = default)
    uint32_t _pad[8];        // Padding to 64 bytes
};
static_assert(sizeof(TextSpan) == 64, "TextSpan must be 64 bytes");

// Positioned glyph (output from compute shader, 32 bytes)
struct PositionedGlyph {
    float x, y;              // Screen position
    float width, height;     // Glyph size
    uint32_t glyphIndex;     // Index in MSDF atlas
    uint32_t color;          // RGBA packed
    uint32_t layer;          // For sorting
    uint32_t _pad;
};
static_assert(sizeof(PositionedGlyph) == 32, "PositionedGlyph must be 32 bytes");

// BVH node (32 bytes)
struct BVHNode {
    float aabbMinX, aabbMinY;
    float aabbMaxX, aabbMaxY;
    uint32_t leftChild;      // Child index or primitive index if leaf
    uint32_t rightChild;     // 0xFFFFFFFF if leaf
    uint32_t primIndex;      // First primitive in leaf
    uint32_t primCount;      // Number of primitives (0 for internal nodes)
};
static_assert(sizeof(BVHNode) == 32, "BVHNode must be 32 bytes");

//=============================================================================
// YDraw class
//=============================================================================

class YDraw : public Card,
              public base::ObjectFactory<YDraw> {
public:
    using Ptr = std::shared_ptr<YDraw>;

    // Shader glyph codepoint
    static constexpr uint32_t SHADER_GLYPH = 0x100002;  // Card base + 0x0002

    // Flags
    static constexpr uint32_t FLAG_SHOW_BOUNDS = 1;     // Debug: show primitive bounds
    static constexpr uint32_t FLAG_SHOW_BVH = 2;        // Debug: show BVH structure

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

    virtual ~YDraw() = default;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> dispose() override = 0;
    Result<void> update(float time) override = 0;
    const char* typeName() const override { return "ydraw"; }

    // Override for 64-byte metadata slots (shader uses slotIndex * 16)
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    //=========================================================================
    // YDraw-specific API
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

    // Add text span
    virtual uint32_t addTextSpan(float x, float y, const std::string& text,
                                 float fontSize, uint32_t color,
                                 uint32_t fontId = 0, uint32_t layer = 0) = 0;

    // Clear all primitives and text
    virtual void clear() = 0;

    // Get primitive/text counts
    virtual uint32_t primitiveCount() const = 0;
    virtual uint32_t textSpanCount() const = 0;

    // Mark data as dirty (triggers BVH rebuild on next update)
    virtual void markDirty() = 0;

    // Set background color
    virtual void setBgColor(uint32_t color) = 0;
    virtual uint32_t bgColor() const = 0;

    // Set scene bounds (for Morton code normalization)
    // If not set, computed automatically from primitives
    virtual void setSceneBounds(float minX, float minY, float maxX, float maxY) = 0;
    virtual bool hasExplicitBounds() const = 0;

protected:
    YDraw(CardBufferManager::Ptr mgr, const GPUContext& gpu,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}
};

} // namespace yetty::card
