#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <optional>

namespace yetty {

/**
 * YEcho Parser - parses text with embedded glyphs and styled blocks.
 *
 * Grammar:
 *   @name           -> shader glyph by name
 *   {attrs: content} -> styled block
 *   \{ \} \@ \\     -> escaped literals
 *
 * Attributes (key=value, separated by whitespace):
 *   color=#RRGGBB   -> text color (hex)
 *   bg=#RRGGBB      -> background color (hex)
 *   style=bold      -> text style (bold, italic, underline, bold|italic)
 *   font-size=N     -> font size in pixels (triggers ytext card)
 *
 * Example:
 *   "Loading @spinner {color=#ff0000: Error!}"
 *   "{style=bold|italic: emphasized} normal"
 *   "{font-size=32: Big text}"  // Requires ytext card
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
        StyledBlock // {attrs: content}
    };
    
    Type type = Type::Text;
    std::string text;           // For Text: the text content
    std::string glyphName;      // For Glyph: the glyph name
    std::vector<YEchoAttribute> attrs;  // For StyledBlock: attributes
    std::vector<YEchoSpan> children;    // For StyledBlock: nested content
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
     * Parse input text into spans.
     * Returns root span containing all parsed content.
     */
    YEchoSpan parse(const std::string& input);
    
    /**
     * Convert parsed spans to terminal output.
     * 
     * - Glyphs: UTF-8 with embedded codepoints
     * - Styled blocks without font-size: ANSI escape sequences
     * - Styled blocks with font-size: requires ytext (use needsYtext() to check)
     */
    std::string toTerminal(const YEchoSpan& root) const;
    
    /**
     * Check if parsed content requires ytext card (has font-size attributes).
     */
    bool needsYtext(const YEchoSpan& root) const;
    
    /**
     * Get parse errors (if any).
     */
    const std::vector<std::string>& errors() const { return errors_; }

private:
    const GlyphRegistry* registry_;
    std::vector<std::string> errors_;
    
    void parseInto(const std::string& input, size_t& pos, std::vector<YEchoSpan>& spans, char endChar = '\0');
    bool parseGlyph(const std::string& input, size_t& pos, YEchoSpan& span);
    bool parseStyledBlock(const std::string& input, size_t& pos, YEchoSpan& span);
    bool parseAttributes(const std::string& input, size_t& pos, std::vector<YEchoAttribute>& attrs);
    
    void appendUtf8(std::string& out, uint32_t codepoint) const;
};

} // namespace yetty
