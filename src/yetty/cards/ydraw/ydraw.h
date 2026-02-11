#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/yetty-context.h>
#include <string>

namespace yetty::card {

//=============================================================================
// YDraw - SDF card with YAML/binary payload parsing
//
// Uses YDrawBuilder for content generation.
// Implementation (GPU buffers, parsing, animation) is in YDrawImpl.
//=============================================================================
class YDraw : public Card,
              public base::ObjectFactory<YDraw> {
public:
    using Ptr = std::shared_ptr<YDraw>;

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

    ~YDraw() override = default;
    const char* typeName() const override { return "ydraw"; }
    bool needsBuffer() const override { return true; }

protected:
    using Card::Card;
};

} // namespace yetty::card
