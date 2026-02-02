#include "markdown.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <fstream>

namespace yetty::card {

//=============================================================================
// Constructor
//=============================================================================

Markdown::Markdown(const YettyContext& ctx,
                   int32_t x, int32_t y,
                   uint32_t widthCells, uint32_t heightCells,
                   const std::string& args, const std::string& payload)
    : YDrawBase(ctx, x, y, widthCells, heightCells)
    , _argsStr(args)
    , _payloadStr(payload)
{}

//=============================================================================
// Initialization
//=============================================================================

Result<void> Markdown::init() {
    if (auto res = initBase(); !res) {
        return res;
    }

    parseArgs(_argsStr);

    std::string content;

    // Check if payload is inline content or file path
    if (_payloadStr.substr(0, 7) == "inline:") {
        content = _payloadStr.substr(7);
    } else if (_payloadStr.size() > 0 && _payloadStr[0] != '#' &&
               _payloadStr[0] != '*' && _payloadStr[0] != '-' &&
               _payloadStr[0] != '\n' && _payloadStr.find('\n') == std::string::npos) {
        // Single line without markdown markers - try as file path
        std::ifstream file(_payloadStr);
        if (file) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            content = buffer.str();
        } else {
            // Not a file - treat as inline content
            content = _payloadStr;
        }
    } else {
        content = _payloadStr;
    }

    setInitParsing(true);
    parseMarkdown(content);
    generatePrimitives();
    setInitParsing(false);

    yinfo("Markdown::init: parsed {} lines, prims={} glyphs={}",
          _parsedLines.size(), primitiveCount(), glyphCount());

    markDirty();
    return Ok();
}

//=============================================================================
// Args parsing
//=============================================================================

void Markdown::parseArgs(const std::string& args) {
    if (args.empty()) return;

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token.substr(0, 12) == "--font-size=") {
            _fontSize = std::stof(token.substr(12));
        } else if (token.substr(0, 11) == "--bg-color=") {
            std::string hex = token.substr(11);
            if (hex[0] == '#') hex = hex.substr(1);
            uint32_t rgb = std::stoul(hex, nullptr, 16);
            if (hex.size() <= 6) {
                setBgColor(0xFF000000 | ((rgb & 0xFF) << 16) |
                           (rgb & 0xFF00) | ((rgb >> 16) & 0xFF));
            }
        } else if (token.substr(0, 15) == "--line-spacing=") {
            _lineSpacing = std::stof(token.substr(15));
        }
    }
}

//=============================================================================
// Markdown Parser (ported from plugins/markdown)
//=============================================================================

void Markdown::parseMarkdown(const std::string& content) {
    _parsedLines.clear();

    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line)) {
        ParsedLine textLine;

        // Check for headers
        int headerLevel = 0;
        size_t i = 0;
        while (i < line.size() && line[i] == '#') {
            headerLevel++;
            i++;
        }
        if (headerLevel > 0 && i < line.size() && line[i] == ' ') {
            line = line.substr(i + 1);
            textLine.scale = 1.0f + (0.15f * (7 - std::min(headerLevel, 6)));
        } else {
            headerLevel = 0;  // Not a valid header
        }

        // Check for bullet list
        bool isBullet = false;
        if (line.size() >= 2 && (line[0] == '-' || line[0] == '*') && line[1] == ' ') {
            isBullet = true;
            textLine.indent = 20.0f;
            line = line.substr(2);
        }

        // Parse inline styles
        size_t pos = 0;
        while (pos < line.size()) {
            ParsedSpan span;

            // Check for code
            if (line[pos] == '`') {
                size_t end = line.find('`', pos + 1);
                if (end != std::string::npos) {
                    span.text = line.substr(pos + 1, end - pos - 1);
                    span.isCode = true;
                    span.style = ParsedSpan::Regular;
                    textLine.spans.push_back(span);
                    pos = end + 1;
                    continue;
                }
            }

            // Check for bold+italic (***text***)
            if (pos + 2 < line.size() && line.substr(pos, 3) == "***") {
                size_t end = line.find("***", pos + 3);
                if (end != std::string::npos) {
                    span.text = line.substr(pos + 3, end - pos - 3);
                    span.style = ParsedSpan::BoldItalic;
                    textLine.spans.push_back(span);
                    pos = end + 3;
                    continue;
                }
            }

            // Check for bold (**text**)
            if (pos + 1 < line.size() && line.substr(pos, 2) == "**") {
                size_t end = line.find("**", pos + 2);
                if (end != std::string::npos) {
                    span.text = line.substr(pos + 2, end - pos - 2);
                    span.style = ParsedSpan::Bold;
                    textLine.spans.push_back(span);
                    pos = end + 2;
                    continue;
                }
            }

            // Check for italic (*text*)
            if (line[pos] == '*') {
                size_t end = line.find('*', pos + 1);
                if (end != std::string::npos) {
                    span.text = line.substr(pos + 1, end - pos - 1);
                    span.style = ParsedSpan::Italic;
                    textLine.spans.push_back(span);
                    pos = end + 1;
                    continue;
                }
            }

            // Regular text - find next special character
            size_t next = line.find_first_of("*`", pos);
            if (next == std::string::npos) next = line.size();
            if (next > pos) {
                span.text = line.substr(pos, next - pos);
                span.style = (headerLevel > 0) ? ParsedSpan::Bold : ParsedSpan::Regular;
                span.headerLevel = static_cast<uint8_t>(headerLevel);
                textLine.spans.push_back(span);
                pos = next;
            } else {
                // Unmatched special char - treat as literal
                span.text = std::string(1, line[pos]);
                span.style = ParsedSpan::Regular;
                textLine.spans.push_back(span);
                pos++;
            }
        }

        // Add bullet prefix
        if (isBullet && !textLine.spans.empty()) {
            ParsedSpan bullet;
            bullet.text = "\xE2\x80\xA2 ";  // Unicode bullet
            bullet.style = ParsedSpan::Regular;
            bullet.isBullet = true;
            textLine.spans.insert(textLine.spans.begin(), bullet);
        }

        // Empty lines become paragraph breaks
        if (textLine.spans.empty()) {
            ParsedSpan empty;
            empty.text = "";
            empty.style = ParsedSpan::Regular;
            textLine.spans.push_back(empty);
        }

        _parsedLines.push_back(textLine);
    }
}

//=============================================================================
// Generate SDF primitives from parsed markdown
//=============================================================================

void Markdown::generatePrimitives() {
    float cursorY = 2.0f;  // Start with some padding
    float lineHeight = _fontSize * _lineSpacing;

    for (const auto& line : _parsedLines) {
        float cursorX = 2.0f + line.indent;
        float scale = line.scale;
        float scaledSize = _fontSize * scale;
        float scaledLineHeight = lineHeight * scale;

        for (const auto& span : line.spans) {
            if (span.text.empty()) continue;

            // Choose color based on style
            uint32_t color;
            if (span.isCode) {
                color = _codeColor;
            } else if (span.headerLevel > 0) {
                color = _headerColor;
            } else if (span.style == ParsedSpan::Bold ||
                       span.style == ParsedSpan::BoldItalic) {
                color = _boldColor;
            } else {
                color = _textColor;
            }

            // Add code background box
            if (span.isCode) {
                float textWidth = span.text.size() * scaledSize * 0.6f;
                addBox(cursorX + textWidth * 0.5f,
                       cursorY + scaledSize * 0.4f,
                       textWidth * 0.5f + 1.0f,
                       scaledSize * 0.5f + 0.5f,
                       _codeBgColor, 0, 0, 1.0f, 0);
            }

            // Add text
            addText(cursorX, cursorY, span.text, scaledSize, color, 0);

            // Advance cursor
            cursorX += span.text.size() * scaledSize * 0.6f;
        }

        cursorY += scaledLineHeight;
    }

    // No explicit bounds — let YDrawBase auto-compute from content
    // and adjust aspect ratio to match the card.
}

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> Markdown::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("Markdown::create: pos=({},{}) size={}x{} payload_len={}",
          x, y, widthCells, heightCells, payload.size());

    if (!ctx.cardManager) {
        yerror("Markdown::create: null CardBufferManager!");
        return Err<CardPtr>("Markdown::create: null CardBufferManager");
    }

    auto card = std::make_shared<Markdown>(
        ctx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        yerror("Markdown::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("Markdown::create: init failed");
    }

    yinfo("Markdown::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

} // namespace yetty::card
