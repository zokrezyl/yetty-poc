#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/yetty-context.h>
#include <string>
#include <memory>

namespace yetty::card {

//=============================================================================
// YThorVG - Vector graphics card using ThorVG RenderMethod interface
//
// Unlike the original ThorVG card which traverses the scene tree via Accessor,
// YThorVG implements ThorVG's RenderMethod interface directly. ThorVG calls
// our prepare()/renderShape() methods with pre-processed, world-space data.
//
// Benefits:
// - No manual transform accumulation (ThorVG passes world transforms)
// - No manual opacity accumulation (ThorVG passes combined opacity)
// - Text renders as bezier paths through standard renderShape()
// - Correct render order managed by ThorVG
//=============================================================================
class YThorVG : public Card,
                public base::ObjectFactory<YThorVG> {
public:
    using Ptr = std::shared_ptr<YThorVG>;

    // Use YDraw shader glyph for SDF rendering
    static constexpr uint32_t SHADER_GLYPH = 0x100003;

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    static Result<Ptr> createImpl(
        ContextType& ctx,
        const YettyContext& yettyCtx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    ~YThorVG() override = default;
    const char* typeName() const override { return "ythorvg"; }
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
    void setScreenOrigin(float sx, float sy) override {
        _screenOriginX = sx; _screenOriginY = sy;
    }

protected:
    YThorVG(GpuMemoryManager::Ptr mgr, const GPUContext& gpu,
            int32_t x, int32_t y, uint32_t widthCells, uint32_t heightCells)
        : _cardMgr(std::move(mgr)), _gpu(gpu)
        , _x(x), _y(y), _widthCells(widthCells), _heightCells(heightCells) {}

    GpuMemoryManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    uint32_t _shaderGlyph = 0;
    int32_t _x, _y;
    uint32_t _widthCells, _heightCells;
    float _screenOriginX = 0.0f, _screenOriginY = 0.0f;
    std::string _name;
};

} // namespace yetty::card
