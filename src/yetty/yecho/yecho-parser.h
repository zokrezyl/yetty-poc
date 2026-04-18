#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <optional>

namespace yetty {

/**
 * YEcho Parser - parses text with embedded glyphs, styled blocks, and cards.
 *
 * Grammar:
 *   @name                              -> shader glyph by name
 *   {attrs: content}                   -> styled block (ANSI)
 *   {card=TYPE; attrs: content}        -> card block (OSC)
 *   \{ \} \@ \\                        -> escaped literals
 *
 * Styled Block Attributes (whitespace-separated):
 *   color=#RRGGBB   -> text color (hex)
 *   bg=#RRGGBB      -> background color (hex)
 *   style=bold      -> text style (bold, italic, underline, bold|italic)
 *
 * Card Block Syntax:
 *   {card=TYPE; attr1=val1; @attr2=val2: content}
 *   - First statement must be card=TYPE (plot, ytext, qr, etc.)
 *   - Statements separated by semicolons on BOTH sides of ':'
 *   - '@' prefix for special attributes (@view, @fg-color, etc.)
 *   - Content depends on card type (expression, text, URL, etc.)
 *
 * Examples:
 *   "Loading @spinner {color=#ff0000: Error!}"
 *   "{card=plot; w=80; h=20; @view=-3.14..3.14,-1..1: f1=sin(x); @f1.color=#00FF00}"
 *   "{card=ytext; w=40; h=10; @font-size=48: Hello World}"
 */

// Style flags (can be combined with |)
enum class YEchoStyle : uint32_t {
    None      = 0,
    Bold      = 1 << 0,
    Italic    = 1 << 1,
    Underline = 1 << 2,
};

inline YEchoStyle operator|(YEchoStyle a, YEchoStyle b) {
    return static_cast<YEchoStyle>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline bool hasStyle(YEchoStyle flags, YEchoStyle check) {
    return (static_cast<uint32_t>(flags) & static_cast<uint32_t>(check)) != 0;
}

struct YEchoAttribute {
    std::string key;
    std::string value;
    
    // Parsed value helpers
    bool isColor() const { return !value.empty() && value[0] == '#'; }
    bool isInteger() const;
    bool isStyle() const { return key == "style"; }
    
    std::optional<uint32_t> asColor() const;  // Returns 0xRRGGBB
    std::optional<int> asInteger() const;
    YEchoStyle asStyle() const;  // Parses "bold|italic" etc.
};

struct YEchoSpan {
    enum class Type {
        Text,       // Plain text
        Glyph,      // @name -> shader glyph
        Block       // {attrs: content} -> semicolon-separated statements
    };
    
    Type type = Type::Text;
    std::string text;           // For Text: the text content
    std::string glyphName;      // For Glyph: the glyph name
    std::vector<YEchoAttribute> attrs;  // For Block: left side statements (before ':')
    std::string content;        // For Block: right side content (after ':')
};

class GlyphRegistry {
public:
    /**
     * Create registry with built-in glyphs (from generated header).
     */
    GlyphRegistry();
    
    /**
     * Load additional glyph names from shader directory.
     * Parses filenames like "0x0000-spinner.wgsl" -> "spinner" at offset 0x0000
     */
    bool loadFromDirectory(const std::string& shaderDir);
    
    /**
     * Get codepoint for glyph name.
     * Returns 0 if not found.
     */
    uint32_t getCodepoint(const std::string& name) const;
    
    /**
     * Check if glyph name exists.
     */
    bool hasGlyph(const std::string& name) const;
    
    /**
     * Get all registered glyph names.
     */
    std::vector<std::string> getNames() const;
    
    /**
     * Register a glyph manually (for testing or custom glyphs).
     */
    void registerGlyph(const std::string& name, uint32_t codepoint);

private:
    static constexpr uint32_t SHADER_GLYPH_BASE = 0x101000;
    std::unordered_map<std::string, uint32_t> glyphs_;
};

class YEchoParser {
public:
    explicit YEchoParser(const GlyphRegistry* registry = nullptr);
    
    /**
     * Parse input text into vector of spans.
     */
    std::vector<YEchoSpan> parse(const std::string& input);
    
    /**
     * Convert parsed spans to terminal output.
     * 
     * - Glyphs: UTF-8 with embedded codepoints
     * - Blocks: Interpreted based on first attribute (card= triggers OSC)
     */
    std::string toTerminal(const std::vector<YEchoSpan>& spans) const;
    
    /**
     * Get parse errors (if any).
     */
    const std::vector<std::string>& errors() const { return errors_; }

private:
    const GlyphRegistry* registry_;
    mutable std::vector<std::string> errors_;
    
    void parseInto(const std::string& input, size_t& pos, std::vector<YEchoSpan>& spans, char endChar = '\0');
    bool parseGlyph(const std::string& input, size_t& pos, YEchoSpan& span);
    bool parseBlock(const std::string& input, size_t& pos, YEchoSpan& span);
    void parseStatements(const std::string& str, std::vector<YEchoAttribute>& attrs);
    
    std::string generateOSC(const YEchoSpan& span, const std::string& cardType) const;
    void appendUtf8(std::string& out, uint32_t codepoint) const;
};

} // namespace yetty
