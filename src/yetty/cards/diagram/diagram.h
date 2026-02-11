#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/yetty-context.h>
#include <string>

namespace yetty::card {

//=============================================================================
// Diagram - Card for rendering Mermaid/Graphviz diagrams
//
// Uses YDrawBuilder for rendering.
// Supports:
//   - Mermaid flowchart syntax (graph TD, graph LR, etc.)
//   - Graphviz DOT syntax (digraph, graph)
//=============================================================================
class Diagram : public Card,
                public base::ObjectFactory<Diagram> {
public:
    using Ptr = std::shared_ptr<Diagram>;

    static constexpr uint32_t SHADER_GLYPH = 0x100003;  // Same as YDraw

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

    ~Diagram() override = default;
    const char* typeName() const override { return "diagram"; }
    bool needsBuffer() const override { return true; }

protected:
    using Card::Card;
};

} // namespace yetty::card
