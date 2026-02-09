#pragma once

#include <yetty/card.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include <yetty/msdf-cdb-provider.h>
#include <yetty/font-manager.h>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>

namespace yetty {

// Forward declare SDFPrimitive (defined in cards/hdraw/hdraw.h)
namespace card { struct SDFPrimitive; }

// Forward declare Animation
namespace animation { class Animation; }

//=============================================================================
// YDrawGlyph - Positioned glyph for GPU rendering (32 bytes)
//=============================================================================
struct YDrawGlyph {
    float x, y;              // Position in scene coordinates
    float width, height;     // Glyph size in scene coordinates
    uint32_t glyphIndex;     // Index in glyphMetadata buffer
    uint32_t color;          // RGBA packed color
    uint32_t layer;          // Draw order
    uint32_t _pad;           // Padding for alignment
};
static_assert(sizeof(YDrawGlyph) == 32, "YDrawGlyph must be 32 bytes");

//=============================================================================
// YDrawBase - Rendering base class for SDF + MSDF text cards
//
// Provides:
//   - Spatial hash grid with O(1) lookup for SDF primitives and text glyphs
//   - GPU buffer management (primitives, grid, glyphs, metadata)
//   - Card lifecycle integration (declare/allocate/render/dispose/suspend)
//
// Subclasses provide:
//   - typeName() (e.g. "ydraw", "markdown")
//   - Input parsing (YAML, binary, markdown, etc.)
//=============================================================================
class YDrawBase : public Card {
public:
    // Shader glyph codepoint
    static constexpr uint32_t SHADER_GLYPH = 0x100003;  // Card base + 0x0003

    // Default grid parameters
    static constexpr uint32_t DEFAULT_MAX_PRIMS_PER_CELL = 16;
    static constexpr float DEFAULT_CELL_SIZE = 0.0f;  // 0 = auto-compute

    // Flags
    static constexpr uint32_t FLAG_SHOW_BOUNDS = 1;
    static constexpr uint32_t FLAG_SHOW_GRID = 2;
    static constexpr uint32_t FLAG_SHOW_EVAL_COUNT = 4;
    static constexpr uint32_t FLAG_HAS_3D = 8;

    virtual ~YDrawBase();

    //=========================================================================
    // Card interface overrides
    //=========================================================================
    bool needsBuffer() const override { return true; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    Result<void> dispose() override;
    void suspend() override;
    void declareBufferNeeds() override;
    Result<void> allocateBuffers() override;
    Result<void> render(float time) override;

    //=========================================================================
    // EventListener - zoom/pan via mouse wheel
    //=========================================================================
    Result<bool> onEvent(const base::Event& event) override;

    //=========================================================================
    // Public API - SDF primitives
    //=========================================================================
    uint32_t addPrimitive(const card::SDFPrimitive& prim);
    uint32_t addCircle(float cx, float cy, float radius,
                       uint32_t fillColor, uint32_t strokeColor = 0,
                       float strokeWidth = 0, uint32_t layer = 0);
    uint32_t addBox(float cx, float cy, float halfW, float halfH,
                    uint32_t fillColor, uint32_t strokeColor = 0,
                    float strokeWidth = 0, float round = 0,
                    uint32_t layer = 0);
    uint32_t addSegment(float x0, float y0, float x1, float y1,
                        uint32_t strokeColor, float strokeWidth = 1,
                        uint32_t layer = 0);
    uint32_t addBezier2(float x0, float y0, float x1, float y1,
                        float x2, float y2,
                        uint32_t strokeColor, float strokeWidth = 1,
                        uint32_t layer = 0);

    //=========================================================================
    // Public API - MSDF text
    //=========================================================================
    uint32_t addText(float x, float y, const std::string& text,
                     float fontSize, uint32_t color,
                     uint32_t layer = 0, int fontId = 0);

    //=========================================================================
    // Public API - Font registration
    //=========================================================================
    int addFont(const std::string& ttfPath);
    int registerFont(const std::string& cdbPath,
                     const std::string& ttfPath = "",
                     MsdfCdbProvider::Ptr provider = nullptr);

    //=========================================================================
    // Public API - Atlas access
    //=========================================================================
    MsdfAtlas::Ptr atlas() const { return _atlas; }

    //=========================================================================
    // Public API - state management
    //=========================================================================
    void clear();
    uint32_t primitiveCount() const;
    uint32_t glyphCount() const;
    void markDirty();

    void setBgColor(uint32_t color);
    uint32_t bgColor() const;

    void setSceneBounds(float minX, float minY, float maxX, float maxY);
    bool hasExplicitBounds() const;

    void setGridCellSize(float size);
    float gridCellSize() const;

    void setFlags(uint32_t flags);
    void addFlags(uint32_t flags);
    uint32_t flags() const;

    void setMaxPrimsPerCell(uint32_t max);

    //=========================================================================
    // Public API - Animation
    //=========================================================================
    animation::Animation* animation();   // create on first access
    void startAnimation();               // snapshot base prims, play
    void stopAnimation();                // restore base prims, stop

protected:
    // Constructor for subclasses
    YDrawBase(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells);

    // Allocates metadata slot - subclasses call from their init()
    Result<void> initBase();

    // Init-parsing mode: when true, addPrimitive routes to CPU staging
    void setInitParsing(bool v);
    bool isInitParsing() const;

    // Total primitives including staging (for use during init parsing)
    uint32_t totalPendingPrimitives() const;

    // Access to font and font manager for subclasses
    MsMsdfFont::Ptr font() const { return _font; }
    FontManager::Ptr fontManager() const { return _fontManager; }

    // Direct access to primitive buffer for procedural cards
    card::SDFPrimitive* primitivePtr() { return _primitives; }
    const card::SDFPrimitive* primitivePtr() const { return _primitives; }

    // Recompute AABB for a primitive
    static void recomputeAABB(card::SDFPrimitive& prim);

    // Mark primitive buffer as dirty (after direct modifications via primitivePtr)
    void markPrimBufferDirty();

private:
    // Metadata structure (matches shader layout - 64 bytes)
    struct Metadata {
        uint32_t primitiveOffset;   // 0
        uint32_t primitiveCount;    // 4
        uint32_t gridOffset;        // 8
        uint32_t gridWidth;         // 12
        uint32_t gridHeight;        // 16
        uint32_t cellSize;          // 20 (f32 stored as bits)
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
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    static constexpr uint32_t GLYPH_BIT = 0x80000000u;

    Result<void> ensurePrimCapacity(uint32_t required);
    void calculate();
    void computeSceneBounds();
    void buildGrid();
    Result<void> rebuildAndUpload();
    Result<void> uploadMetadata();
    Result<void> registerForEvents();
    Result<void> deregisterFromEvents();

    // Primitive data - direct buffer storage
    card::SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;
    StorageHandle _primStorage = StorageHandle::invalid();
    std::vector<card::SDFPrimitive> _primStaging;

    // Grid & derived storage
    uint32_t* _grid = nullptr;
    uint32_t _gridSize = 0;
    StorageHandle _derivedStorage = StorageHandle::invalid();
    std::vector<uint32_t> _gridStaging;  // Pre-built grid from declareBufferNeeds

    // Text glyphs
    std::vector<YDrawGlyph> _glyphs;

    // Grid dimensions (computed in rebuildAndUpload)
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;

    // Scene bounds
    float _sceneMinX = 0, _sceneMinY = 0;
    float _sceneMaxX = 100, _sceneMaxY = 100;
    bool _hasExplicitBounds = false;

    // Grid parameters
    float _cellSize = DEFAULT_CELL_SIZE;
    uint32_t _maxPrimsPerCell = DEFAULT_MAX_PRIMS_PER_CELL;

    // GPU offsets (in float units)
    uint32_t _primitiveOffset = 0;
    uint32_t _gridOffset = 0;
    uint32_t _glyphOffset = 0;

    // State
    uint32_t _bgColor = 0xFF2E1A1A;
    uint32_t _flags = 0;
    bool _dirty = true;
    bool _metadataDirty = true;
    bool _initParsing = false;

    // Font for text rendering
    FontManager::Ptr _fontManager;
    MsMsdfFont::Ptr _font;
    MsdfAtlas::Ptr _atlas;  // shared with _font
    std::unordered_map<std::string, int> _fontIdCache;  // cdbPath → fontId

    // View zoom/pan (applied to metadata scene bounds, not to grid)
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;   // pan offset in scene units
    float _viewPanY = 0.0f;
    bool _focused = false;

    // Animation
    std::unique_ptr<animation::Animation> _animation;
    std::vector<card::SDFPrimitive> _basePrimitives;  // snapshot at animation start
    float _lastRenderTime = -1.0f;
};

} // namespace yetty
