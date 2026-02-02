#pragma once

#include "../../ydraw/ydraw-base.h"
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
// Markdown - SDF card that renders markdown content using YDrawBase
//
// Parses markdown text and generates addText() / addBox() calls for
// headers, bold, italic, code, bullet lists.
//=============================================================================
class Markdown : public yetty::YDrawBase {
public:
    using Ptr = std::shared_ptr<Markdown>;

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    ~Markdown() override = default;

    const char* typeName() const override { return "markdown"; }

    // Constructor is public for make_shared, use create() factory instead
    Markdown(const YettyContext& ctx,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args, const std::string& payload);

private:
    Result<void> init();

    void parseArgs(const std::string& args);
    void parseMarkdown(const std::string& content);
    void generatePrimitives();

    std::string _argsStr;
    std::string _payloadStr;
    std::vector<ParsedLine> _parsedLines;

    float _fontSize = 14.0f;
    float _lineSpacing = 1.4f;
    uint32_t _textColor = 0xFFE6E6E6;      // Light gray (ABGR)
    uint32_t _boldColor = 0xFFFFFFFF;       // White
    uint32_t _codeColor = 0xFF66CC99;       // Green
    uint32_t _headerColor = 0xFFFFFFFF;     // White
    uint32_t _codeBgColor = 0xFF3D3D3D;     // Dark gray background
};

} // namespace yetty::card
