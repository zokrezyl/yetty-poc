#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
#include <string>

namespace yetty::card {

// Import SDFPrimitive from hdraw.h (same format)
struct SDFPrimitive;  // Defined in hdraw.h

// Positioned glyph for GPU rendering (32 bytes)
struct JDrawGlyph {
    float x, y;              // Position in scene coordinates
    float width, height;     // Glyph size in scene coordinates
    uint32_t glyphIndex;     // Index in glyphMetadata buffer
    uint32_t color;          // RGBA packed color
    uint32_t layer;          // Draw order
    uint32_t _pad;           // Padding for alignment
};
static_assert(sizeof(JDrawGlyph) == 32, "JDrawGlyph must be 32 bytes");

//=============================================================================
// JDraw - Card for rendering SDF primitives with GPU compute shader tile culling
//
// Uses shader glyph U+100005 (card base + offset 0x0005)
//=============================================================================

class JDraw : public Card,
              public base::ObjectFactory<JDraw> {
public:
    using Ptr = std::shared_ptr<JDraw>;

    // Shader glyph codepoint
    static constexpr uint32_t SHADER_GLYPH = 0x100005;  // Card base + 0x0005

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

    virtual ~JDraw() = default;

    //=========================================================================
    // Card interface
    //=========================================================================
    const char* typeName() const override { return "jdraw"; }
    bool needsBuffer() const override { return true; }

    // Card accessors
    uint32_t metadataOffset() const override { return _metaHandle.offset; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    uint32_t shaderGlyph() const override { return _shaderGlyph; }
    int32_t x() const override { return _x; }
    int32_t y() const override { return _y; }
    void setPosition(int32_t x, int32_t y) override { _x = x; _y = y; }
    uint32_t widthCells() const override { return _widthCells; }
    uint32_t heightCells() const override { return _heightCells; }
    const std::string& name() const override { return _name; }
    void setName(const std::string& n) override { _name = n; }
    bool hasName() const override { return !_name.empty(); }
    void setScreenOrigin(float sx, float sy) override { _screenOriginX = sx; _screenOriginY = sy; }

protected:
    JDraw(CardManager::Ptr mgr, const GPUContext& gpu,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells)
        : _cardMgr(std::move(mgr)), _gpu(gpu)
        , _x(x), _y(y), _widthCells(widthCells), _heightCells(heightCells)
    {}

    CardManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    uint32_t _shaderGlyph = 0;
    int32_t _x, _y;
    uint32_t _widthCells, _heightCells;
    float _screenOriginX = 0.0f, _screenOriginY = 0.0f;
    std::string _name;
};

} // namespace yetty::card
