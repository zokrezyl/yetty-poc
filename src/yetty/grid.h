#pragma once

#error "DEPRECATED: grid.h is obsolete. Use GPUScreen Cell struct."

#include <vector>
#include <cstdint>

namespace yetty {

// Special glyph index values (legacy 16-bit range, kept for compatibility)
constexpr uint32_t GLYPH_PLUGIN = 0xFFFF;     // Cell is occupied by a decorator plugin
constexpr uint32_t GLYPH_WIDE_CONT = 0xFFFE;  // Wide char continuation (look at previous cell)

// Legacy custom glyph range (conflicts with Nerd Fonts, prefer SHADER_GLYPH range)
constexpr uint32_t GLYPH_CUSTOM_START = 0xF000;
constexpr uint32_t GLYPH_CUSTOM_END = 0xFFFD;

// =============================================================================
// Font type encoding for cell attributes (bits 5-7 of style byte)
// =============================================================================
constexpr uint8_t FONT_TYPE_MSDF = 0;    // Default text rendering
constexpr uint8_t FONT_TYPE_BITMAP = 1;  // Bitmap fonts (emoji, color fonts)
constexpr uint8_t FONT_TYPE_SHADER = 2;  // Single-cell shader glyphs (spinner, etc.)
constexpr uint8_t FONT_TYPE_CARD = 3;    // Multi-cell card glyphs (image card, etc.)
// 4-7 = reserved for future font types

// =============================================================================
// UTF Codepoint ranges for different glyph types
// =============================================================================

// Card glyph range - multi-cell widgets rendered by shader
// Uses Unicode Plane 16 Private Use Area-B (U+100000 - U+100FFF)
constexpr uint32_t CARD_GLYPH_BASE = 0x100000;   // 1048576 decimal (U+100000)
constexpr uint32_t CARD_GLYPH_END = 0x100FFF;    // 1052671 decimal (U+100FFF)

// Shader glyph range - single-cell procedural glyphs
// Uses Unicode Plane 16 Private Use Area-B (U+101000 - U+10FFFD)
constexpr uint32_t SHADER_GLYPH_BASE = 0x101000;  // 1052672 decimal (U+101000)
constexpr uint32_t SHADER_GLYPH_END = 0x10FFFD;   // 1114109 decimal (U+10FFFD)

// Legacy aliases for backwards compatibility
constexpr uint32_t SHADER_GLYPH_START = CARD_GLYPH_BASE;

// =============================================================================
// Helper functions for glyph type detection
// =============================================================================

// Check if codepoint is a card glyph (multi-cell widget)
inline bool isCardGlyph(uint32_t codepoint) {
    return codepoint >= CARD_GLYPH_BASE && codepoint <= CARD_GLYPH_END;
}

// Check if codepoint is a shader glyph (single-cell procedural)
inline bool isShaderGlyph(uint32_t codepoint) {
    return codepoint >= SHADER_GLYPH_BASE && codepoint <= SHADER_GLYPH_END;
}

// Check if codepoint is any procedural glyph (card or shader)
inline bool isProceduralGlyph(uint32_t codepoint) {
    return codepoint >= CARD_GLYPH_BASE && codepoint <= SHADER_GLYPH_END;
}

// Get offset within card glyph range (for dispatch index)
inline uint32_t getCardGlyphOffset(uint32_t codepoint) {
    return codepoint - CARD_GLYPH_BASE;
}

// Get offset within shader glyph range (for dispatch index)
inline uint32_t getShaderGlyphOffset(uint32_t codepoint) {
    return codepoint - SHADER_GLYPH_BASE;
}

// Legacy helper - get shader ID from glyph index
inline uint32_t getShaderGlyphId(uint32_t glyphIndex) {
    return glyphIndex - CARD_GLYPH_BASE;
}

// Legacy helper (kept for compatibility)
inline bool isCustomGlyph(uint32_t glyphIndex) {
    return (glyphIndex >= GLYPH_CUSTOM_START && glyphIndex <= GLYPH_CUSTOM_END) ||
           isShaderGlyph(glyphIndex);
}

// Cell attributes packed into a single byte for GPU upload
// Bit layout: [fontType(3)][strikethrough][underline_type(2)][italic][bold]
// This matches what the shader expects in cellAttrsTexture (R8Uint)
//
// fontType (bits 5-7):
//   0 = MSDF (default text)
//   1 = Bitmap (emoji, color fonts)
//   2 = Shader glyph (single-cell animated)
//   3 = Card glyph (multi-cell widgets)
//   4-7 = reserved
struct CellAttrs {
    uint8_t _bold : 1;           // Bit 0: bold
    uint8_t _italic : 1;         // Bit 1: italic
    uint8_t _underline : 2;      // Bits 2-3: underline type (0=none, 1=single, 2=double, 3=curly)
    uint8_t _strikethrough : 1;  // Bit 4: strikethrough
    uint8_t _fontType : 3;       // Bits 5-7: font type (MSDF=0, Bitmap=1, Shader=2, Card=3)

    uint8_t pack() const {
        return static_cast<uint8_t>(
            (_bold & 0x1) |
            ((_italic & 0x1) << 1) |
            ((_underline & 0x3) << 2) |
            ((_strikethrough & 0x1) << 4) |
            ((_fontType & 0x7) << 5)
        );
    }

    static CellAttrs unpack(uint8_t packed) {
        CellAttrs attrs;
        attrs._bold = packed & 0x1;
        attrs._italic = (packed >> 1) & 0x1;
        attrs._underline = (packed >> 2) & 0x3;
        attrs._strikethrough = (packed >> 4) & 0x1;
        attrs._fontType = (packed >> 5) & 0x7;
        return attrs;
    }

    // Convenience: check if this is a bitmap/emoji font type
    bool isEmoji() const { return _fontType == FONT_TYPE_BITMAP; }

    // Convenience: set font type from legacy emoji flag
    void setEmoji(bool emoji) {
        _fontType = emoji ? FONT_TYPE_BITMAP : FONT_TYPE_MSDF;
    }
};

class Grid {
public:
    Grid(uint32_t cols = 80, uint32_t rows = 24);
    virtual ~Grid() = default;

    void resize(uint32_t cols, uint32_t rows);
    void clear();

    // Set cell data (without attributes - legacy)
    void setCell(uint32_t col, uint32_t row, uint16_t glyphIndex,
                 uint8_t fgR, uint8_t fgG, uint8_t fgB,
                 uint8_t bgR, uint8_t bgG, uint8_t bgB);

    // Set cell data with attributes
    void setCell(uint32_t col, uint32_t row, uint16_t glyphIndex,
                 uint8_t fgR, uint8_t fgG, uint8_t fgB,
                 uint8_t bgR, uint8_t bgG, uint8_t bgB,
                 CellAttrs attrs);

    void setGlyph(uint32_t col, uint32_t row, uint16_t glyphIndex);
    void setFgColor(uint32_t col, uint32_t row, uint8_t r, uint8_t g, uint8_t b);
    void setBgColor(uint32_t col, uint32_t row, uint8_t r, uint8_t g, uint8_t b);
    void setAttrs(uint32_t col, uint32_t row, CellAttrs attrs);

    // Get cell data
    uint16_t getGlyph(uint32_t col, uint32_t row) const;
    void getFgColor(uint32_t col, uint32_t row, uint8_t& r, uint8_t& g, uint8_t& b) const;

    // Widget ID tracking (separate from glyph data)
    void setWidgetId(uint32_t col, uint32_t row, uint16_t widgetId);
    uint16_t getWidgetId(uint32_t col, uint32_t row) const;
    void clearWidgetId(uint32_t col, uint32_t row);

    // Write ASCII string (helper) - needs Font to convert codepoints to glyph indices
    // If font is nullptr, uses codepoint directly as index (for testing only)
    void writeString(uint32_t col, uint32_t row, const char* str,
                     uint8_t fgR, uint8_t fgG, uint8_t fgB,
                     class Font* font);

    void scrollUp();

    virtual uint32_t getCols() const { return cols_; }
    virtual uint32_t getRows() const { return rows_; }

    // Direct access to texture data for GPU upload
    virtual const uint16_t* getGlyphData() const { return glyphIndices_.data(); }
    virtual const uint8_t* getFgColorData() const { return fgColors_.data(); }
    virtual const uint8_t* getBgColorData() const { return bgColors_.data(); }
    virtual const uint8_t* getAttrsData() const { return attrs_.data(); }

    // Data sizes in bytes
    size_t getGlyphDataSize() const { return glyphIndices_.size() * sizeof(uint16_t); }
    size_t getFgColorDataSize() const { return fgColors_.size(); }
    size_t getBgColorDataSize() const { return bgColors_.size(); }
    size_t getAttrsDataSize() const { return attrs_.size(); }

    // Mark as dirty when content changes
    bool isDirty() const { return _dirty; }
    void clearDirty() { _dirty = false; }

private:
    uint32_t cols_;
    uint32_t rows_;

    // Texture data - flat arrays for GPU upload
    std::vector<uint16_t> glyphIndices_;  // 1 x uint16 per cell
    std::vector<uint8_t> fgColors_;       // RGBA per cell (4 bytes)
    std::vector<uint8_t> bgColors_;       // RGBA per cell (4 bytes)
    std::vector<uint8_t> attrs_;          // 1 byte per cell (packed CellAttrs)

    // Widget tracking (0 = no widget, non-zero = widget ID)
    std::vector<uint16_t> _widgetIds;

    bool _dirty = true;

    size_t cellIndex(uint32_t col, uint32_t row) const {
        return row * cols_ + col;
    }
};

} // namespace yetty
