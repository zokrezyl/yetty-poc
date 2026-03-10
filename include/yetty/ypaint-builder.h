#pragma once

#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <yetty/card-manager.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include <yetty/msdf-cdb-provider.h>
#include <yetty/font-manager.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <memory>

namespace yetty::ypaint {

// Forward declarations
class YPaintBuffer;

//=============================================================================
// YPaintGlyph - Positioned glyph for GPU rendering (20 bytes)
//
// Layout (5 u32s):
//   u32 0: x (f32)           - position in scene coordinates
//   u32 1: y (f32)
//   u32 2: width (f16) | height (f16)  - glyph size as half-floats
//   u32 3: glyphIndex (u16) | layer (u8) | flags (u8)
//   u32 4: color (u32)       - RGBA packed
//=============================================================================
struct YPaintGlyph {
    float x, y;              // Position in scene coordinates
    uint32_t widthHeight;    // width (f16 low) | height (f16 high)
    uint32_t glyphLayerFlags; // glyphIndex (u16 low) | layer (u8) | flags (u8)
    uint32_t color;          // RGBA packed color

    // Helpers for setting packed fields
    void setSize(float w, float h) {
        auto toF16 = [](float f) -> uint16_t {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            uint16_t sign = (b >> 16) & 0x8000;
            int32_t  exp  = ((b >> 23) & 0xFF) - 127 + 15;
            uint16_t mant = (b >> 13) & 0x3FF;
            if (exp <= 0) { exp = 0; mant = 0; }
            else if (exp >= 31) { exp = 31; mant = 0; }
            return sign | (static_cast<uint16_t>(exp) << 10) | mant;
        };
        widthHeight = static_cast<uint32_t>(toF16(w)) |
                      (static_cast<uint32_t>(toF16(h)) << 16);
    }
    void setGlyphLayerFlags(uint16_t glyphIdx, uint8_t layer, uint8_t flags) {
        glyphLayerFlags = static_cast<uint32_t>(glyphIdx) |
                          (static_cast<uint32_t>(layer) << 16) |
                          (static_cast<uint32_t>(flags) << 24);
    }
    float width() const {
        // Decode f16 from lower 16 bits
        uint16_t h16 = static_cast<uint16_t>(widthHeight & 0xFFFF);
        uint32_t sign = (h16 & 0x8000u) << 16;
        uint32_t exp  = (h16 >> 10) & 0x1F;
        uint32_t mant = h16 & 0x3FF;
        uint32_t f32bits;
        if (exp == 0) f32bits = sign; // zero/denorm->0
        else if (exp == 31) f32bits = sign | 0x7F800000; // inf
        else f32bits = sign | ((exp + 112) << 23) | (mant << 13);
        float r; std::memcpy(&r, &f32bits, 4); return r;
    }
    float height() const {
        uint16_t h16 = static_cast<uint16_t>((widthHeight >> 16) & 0xFFFF);
        uint32_t sign = (h16 & 0x8000u) << 16;
        uint32_t exp  = (h16 >> 10) & 0x1F;
        uint32_t mant = h16 & 0x3FF;
        uint32_t f32bits;
        if (exp == 0) f32bits = sign;
        else if (exp == 31) f32bits = sign | 0x7F800000;
        else f32bits = sign | ((exp + 112) << 23) | (mant << 13);
        float r; std::memcpy(&r, &f32bits, 4); return r;
    }
};
static_assert(sizeof(YPaintGlyph) == 20, "YPaintGlyph must be 20 bytes");

//=============================================================================
// YPaintMetadata - GPU metadata layout (64 bytes, matches shader)
//=============================================================================
struct YPaintMetadata {
    uint32_t primitiveOffset;   // 0
    uint32_t primitiveCount;    // 4
    uint32_t gridOffset;        // 8
    uint32_t gridWidth;         // 12
    uint32_t gridHeight;        // 16
    uint32_t cellSizeXY;        // 20: cellSizeX (f16 low) | cellSizeY (f16 high)
    uint32_t glyphOffset;       // 24
    uint32_t glyphCount;        // 28
    uint32_t sceneMinX;         // 32 (f32 stored as bits)
    uint32_t sceneMinY;         // 36 (f32 stored as bits)
    uint32_t sceneMaxX;         // 40 (f32 stored as bits)
    uint32_t sceneMaxY;         // 44 (f32 stored as bits)
    uint32_t widthCells;        // 48
    uint32_t heightCells;       // 52
    uint32_t flags;             // 56
    uint32_t bgColor;           // 60

    // Helper to pack two f32 cell sizes into f16 pair
    static uint32_t packCellSize(float x, float y) {
        auto toF16 = [](float f) -> uint16_t {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            uint16_t sign = (b >> 16) & 0x8000;
            int32_t  exp  = ((b >> 23) & 0xFF) - 127 + 15;
            uint16_t mant = (b >> 13) & 0x3FF;
            if (exp <= 0) { exp = 0; mant = 0; }
            else if (exp >= 31) { exp = 31; mant = 0; }
            return sign | (static_cast<uint16_t>(exp) << 10) | mant;
        };
        return static_cast<uint32_t>(toF16(x)) |
               (static_cast<uint32_t>(toF16(y)) << 16);
    }
};
static_assert(sizeof(YPaintMetadata) == 64, "YPaintMetadata must be 64 bytes");

//=============================================================================
// YPaintBuilder - AABB computation and spatial grid builder for scrolling SDF
//
// Builds a spatial hash grid for GPU culling from YPaintBuffer primitives.
// Uses a two-level architecture:
//   Level 1: Internal incremental structure (std::vector<std::vector<uint32_t>>)
//   Level 2: Packed GPU format (cache, rebuilt when dirty)
//
// Usage:
//   auto builder = YPaintBuilder::create(fontMgr, alloc, cardMgr, slot);
//   builder->addYpaintBuffer(buffer1);  // Incrementally adds primitives
//   builder->addYpaintBuffer(buffer2);  // Can add multiple buffers
//=============================================================================
class YPaintBuilder : public base::Object,
                      public base::ObjectFactory<YPaintBuilder> {
public:
    using Ptr = std::shared_ptr<YPaintBuilder>;

    // Default grid parameters
    static constexpr uint32_t DEFAULT_MAX_PRIMS_PER_CELL = 16;
    static constexpr float DEFAULT_CELL_SIZE = 0.0f;  // 0 = auto-compute
    static constexpr uint32_t GLYPH_BIT = 0x80000000u;

    // Flags
    static constexpr uint32_t FLAG_SHOW_BOUNDS = 1;
    static constexpr uint32_t FLAG_SHOW_GRID = 2;
    static constexpr uint32_t FLAG_SHOW_EVAL_COUNT = 4;
    static constexpr uint32_t FLAG_HAS_3D = 8;
    static constexpr uint32_t FLAG_UNIFORM_SCALE = 16;
    static constexpr uint32_t FLAG_CUSTOM_ATLAS = 32;

    //=========================================================================
    // Factory methods
    //=========================================================================

    // Full factory — with GPU card manager for buffer lifecycle.
    static Result<Ptr> createImpl(FontManager::Ptr fontManager,
                                  GpuAllocator::Ptr allocator,
                                  CardManager::Ptr cardMgr,
                                  uint32_t metaSlotIndex,
                                  bool scrollingMode = false);

    // Lightweight factory — no GPU card manager (for offline/file writers).
    static Result<Ptr> createImpl(FontManager::Ptr fontManager,
                                  GpuAllocator::Ptr allocator,
                                  bool scrollingMode = false);

    ~YPaintBuilder() override = default;
    const char* typeName() const override { return "YPaintBuilder"; }

    //=========================================================================
    // Atlas access
    //=========================================================================
    virtual MsdfAtlas::Ptr atlas() const = 0;
    virtual MsdfAtlas::Ptr customAtlas() const = 0;
    virtual bool hasCustomAtlas() const = 0;
    virtual MsMsdfFont::Ptr font() const = 0;
    virtual FontManager::Ptr fontManager() const = 0;

    //=========================================================================
    // State management
    //=========================================================================
    virtual uint32_t primitiveCount() const = 0;
    virtual uint32_t glyphCount() const = 0;

    virtual void setBgColor(uint32_t color) = 0;
    virtual uint32_t bgColor() const = 0;

    virtual void setSceneBounds(float minX, float minY,
                                float maxX, float maxY) = 0;
    virtual bool hasExplicitBounds() const = 0;

    virtual void setGridCellSize(float sizeX, float sizeY) = 0;
    virtual float gridCellSizeX() const = 0;
    virtual float gridCellSizeY() const = 0;

    virtual void setFlags(uint32_t flags) = 0;
    virtual void addFlags(uint32_t flags) = 0;
    virtual uint32_t flags() const = 0;

    virtual void setMaxPrimsPerCell(uint32_t max) = 0;
    virtual uint32_t maxPrimsPerCell() const = 0;

    //=========================================================================
    // Staging data access (for GPU upload by card)
    //=========================================================================
    virtual const std::vector<uint32_t>& gridStaging() const = 0;
    virtual void clearGridStaging() = 0;
    virtual const std::vector<YPaintGlyph>& glyphs() const = 0;
    virtual std::vector<YPaintGlyph>& glyphsMut() = 0;

    /// Build primitive staging data for GPU upload. Call after addYpaintBuffer().
    /// Returns (offset_table, primitive_data) packed as u32 words.
    virtual void buildPrimStaging(std::vector<uint32_t>& out) const = 0;

    /// Returns true if there is content to render.
    virtual bool hasContent() const = 0;

    //=========================================================================
    // Text selection
    //=========================================================================

    // Per-glyph flag bits (in flags byte = bits 24-31 of glyphLayerFlags)
    static constexpr uint32_t GLYPH_FLAG_CUSTOM_ATLAS = 1;
    static constexpr uint32_t GLYPH_FLAG_SELECTED = 2;

    // Build sorted reading order (by Y then X). Call once after content is finalized.
    virtual void buildGlyphSortedOrder() = 0;

    // Find glyph nearest to scene position. Returns index in sorted order, or -1.
    virtual int32_t findNearestGlyphSorted(float sceneX, float sceneY) = 0;

    // Set selection range (sorted-order indices). Sets GLYPH_FLAG_SELECTED on glyphs.
    // Pass (-1, -1) to clear.
    virtual void setSelectionRange(int32_t startSorted, int32_t endSorted) = 0;

    // Extract selected text as UTF-8 using atlas reverse lookup.
    virtual std::string getSelectedText() = 0;

    //=========================================================================
    // Text measurement & font metrics
    //=========================================================================

    /// Measure text width in scene units. fontId -1 = default font.
    virtual float measureTextWidth(const std::string& text,
                                   float fontSize, int fontId = -1) = 0;

    /// Font ascent (positive, above baseline) in scene units.
    virtual float fontAscent(float fontSize, int fontId = -1) = 0;

    /// Font descent (positive, below baseline) in scene units.
    virtual float fontDescent(float fontSize, int fontId = -1) = 0;

    //=========================================================================
    // Buffer management
    //=========================================================================

    /// Add a YPaintBuffer to this builder. Primitives are copied to an internal
    /// buffer, AABBs are computed, and the spatial grid is updated incrementally.
    /// Scene bounds are extended to include the new primitives (if not explicit).
    /// Can be called multiple times to add multiple buffers.
    /// Returns the number of lines scrolled (0 if no scrolling occurred).
    virtual Result<uint32_t> addYpaintBuffer(std::shared_ptr<YPaintBuffer> buffer) = 0;

    /// Clear all primitives, glyphs, and grid data. Resets scene bounds.
    /// Call before addYpaintBuffer() for replace (vs. incremental) semantics.
    virtual void clear() = 0;

    //=========================================================================
    // Scrolling mode (terminal-style) - set at construction
    //=========================================================================

    /// Whether scrolling mode is enabled (set at construction).
    /// When enabled:
    /// - Primitives are positioned at cursor position (grid cells)
    /// - If content overflows scene, existing content scrolls up
    /// - Cursor advances after each addYpaintBuffer()
    virtual bool scrollingMode() const = 0;

    /// Set cursor position in grid cells (column, row). Used in scrolling mode.
    virtual void setCursorPosition(uint16_t col, uint16_t row) = 0;
    virtual uint16_t cursorCol() const = 0;
    virtual uint16_t cursorRow() const = 0;

    /// Get scene height in grid lines (sceneHeight / cellSizeY)
    virtual uint32_t sceneHeightInLines() const = 0;

    /// Scroll N lines in scrolling mode - removes top N lines.
    /// Call this when terminal scrolls to keep ypaint in sync.
    virtual void scrollLines(uint16_t numLines) = 0;

    //=========================================================================
    // Grid / Scene bounds
    //=========================================================================

    // Scene bounds (set explicitly or extended by addYpaintBuffer())
    virtual float sceneMinX() const = 0;
    virtual float sceneMinY() const = 0;
    virtual float sceneMaxX() const = 0;
    virtual float sceneMaxY() const = 0;
    virtual float cellSizeX() const = 0;
    virtual float cellSizeY() const = 0;
    virtual uint32_t gridWidth() const = 0;
    virtual uint32_t gridHeight() const = 0;

    //=========================================================================
    // GPU buffer lifecycle
    //
    // Cards forward their Card:: overrides to these methods.
    // Call addYpaintBuffer() before declareBufferNeeds().
    //
    // Flow:
    //   renderToStaging: card populates buffer, calls addYpaintBuffer()
    //   declareBufferNeeds: builder reserves space for prims + derived
    //   allocateBuffers: builder gets handles, writes prim/grid/glyph data
    //   allocateTextures: builder allocates custom atlas texture (if any)
    //   writeTextures: builder writes atlas pixel data
    //   finalize: builder re-writes dirty regions, returns metadata
    //   suspend/dispose: builder releases handles
    //=========================================================================

    /// Reserve buffer space for prims + derived (grid + glyphs + atlas).
    /// Call after addYpaintBuffer().
    virtual Result<void> declareBufferNeeds() = 0;

    /// Allocate handles and write prim/grid/glyph data into them.
    /// Call after commitReservations().
    virtual Result<void> allocateBuffers() = 0;

    /// Allocate custom atlas texture handle (if hasCustomAtlas).
    virtual Result<void> allocateTextures() = 0;

    /// Write atlas pixel data into allocated texture.
    virtual Result<void> writeTextures() = 0;

    /// Finalize: re-write dirty buffers and metadata.
    /// Called every frame from Card::finalize().
    virtual Result<void> writeBuffers() = 0;

    /// Whether buffer sizes changed and realloc is needed.
    virtual bool needsBufferRealloc() const = 0;

    /// Whether texture sizes changed and realloc is needed.
    virtual bool needsTextureRealloc() const = 0;

    /// Set card viewport dimensions (marks metadata dirty).
    virtual void setViewport(uint32_t widthCells, uint32_t heightCells) = 0;

    /// Set view zoom/pan (marks metadata dirty).
    virtual void setView(float zoom, float panX, float panY) = 0;

protected:
    YPaintBuilder() = default;
};

} // namespace yetty::ypaint
