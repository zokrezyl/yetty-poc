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

protected:
    using Card::Card;
};

} // namespace yetty::card
