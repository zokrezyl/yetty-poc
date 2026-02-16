#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/yetty-context.h>
#include <string>

namespace yetty::card {

//=============================================================================
// YHtml - HTML/CSS rendering card
//
// Parses HTML+CSS via litehtml, renders boxes/borders as SDF primitives
// and text as MSDF glyphs via YDrawBuilder.
// Implementation (GPU buffers, litehtml) is in YHtmlImpl.
//=============================================================================
class YHtml : public Card,
              public base::ObjectFactory<YHtml> {
public:
    using Ptr = std::shared_ptr<YHtml>;

    static constexpr uint32_t SHADER_GLYPH = 0x100003;

    static Result<Ptr> createImpl(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    ~YHtml() override = default;
    const char* typeName() const override { return "yhtml"; }
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
    YHtml(CardManager::Ptr mgr, const GPUContext& gpu,
          int32_t x, int32_t y, uint32_t widthCells, uint32_t heightCells)
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

} // namespace yetty::card
