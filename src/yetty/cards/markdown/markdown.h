#pragma once

#include <yetty/card.h>
#include <yetty/yetty-context.h>
#include <string>
#include <vector>

namespace yetty::card {

//=============================================================================
// ParsedSpan - a run of styled text from markdown parsing
//=============================================================================
struct ParsedSpan {
    std::string text;
    enum Style { Regular, Bold, Italic, BoldItalic } style = Regular;
    uint8_t headerLevel = 0;  // 0=normal, 1-6=header
    bool isCode = false;
    bool isBullet = false;
};

//=============================================================================
// ParsedLine - a line of text with styled spans from markdown parsing
//=============================================================================
struct ParsedLine {
    std::vector<ParsedSpan> spans;
    float indent = 0.0f;
    float scale = 1.0f;  // For headers
};

//=============================================================================
// Markdown - SDF card that renders markdown content via YDrawBuilder
//
// Parses markdown text and generates addText() / addBox() calls for
// headers, bold, italic, code, bullet lists.
// Implementation (GPU buffers, parsing) is in MarkdownImpl.
//=============================================================================
class Markdown : public Card {
public:
    using Ptr = std::shared_ptr<Markdown>;

    static constexpr uint32_t SHADER_GLYPH = 0x100003;

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    ~Markdown() override = default;
    const char* typeName() const override { return "markdown"; }
    bool needsBuffer() const override { return true; }

protected:
    using Card::Card;
};

} // namespace yetty::card
