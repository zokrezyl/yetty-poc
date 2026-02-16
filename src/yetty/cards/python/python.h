#pragma once

#include <cstdint>
#include <string>
#include <yetty/base/factory.h>
#include <yetty/card.h>
#include <yetty/yetty-context.h>

namespace yetty::card {

class PythonCard : public Card, public base::ObjectFactory<PythonCard> {
public:
    using Ptr = std::shared_ptr<PythonCard>;

    // Reuse the texture card shader glyph — renders pixels just like Image/ThorVG
    static constexpr uint32_t SHADER_GLYPH = 0x100000;

    static Result<Ptr> createImpl(ContextType& ctx, const YettyContext& yettyCtx,
                                  int32_t x, int32_t y, uint32_t widthCells,
                                  uint32_t heightCells, const std::string& args,
                                  const std::string& payload) noexcept;

    bool needsTexture() const override { return true; }

    virtual ~PythonCard() = default;

    // Card accessors
    uint32_t metadataOffset() const override { return _metaHandle.offset; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    uint32_t shaderGlyph() const override { return _shaderGlyph; }
    int32_t x() const override { return _x; }
    int32_t y() const override { return _y; }
    void setPosition(int32_t x, int32_t y) override { _x = x; _y = y; }
    uint32_t widthCells() const override { return _widthCells; }
    uint32_t heightCells() const override { return _heightCells; }
    const char* typeName() const override { return "python"; }
    const std::string& name() const override { return _name; }
    void setName(const std::string& n) override { _name = n; }
    bool hasName() const override { return !_name.empty(); }
    void setScreenOrigin(float sx, float sy) override { _screenOriginX = sx; _screenOriginY = sy; }

protected:
    PythonCard(CardManager::Ptr mgr, const GPUContext& gpu, int32_t x, int32_t y,
               uint32_t widthCells, uint32_t heightCells)
        : _cardMgr(std::move(mgr)), _gpu(gpu)
        , _x(x), _y(y), _widthCells(widthCells), _heightCells(heightCells) {}

    CardManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    uint32_t _shaderGlyph = 0;
    int32_t _x, _y;
    uint32_t _widthCells, _heightCells;
    float _screenOriginX = 0.0f, _screenOriginY = 0.0f;
    std::string _name;
};

}  // namespace yetty::card
