#pragma once

#include <vector>
#include <cstdint>

namespace yetty {

// Special glyph index values (legacy 16-bit range, kept for compatibility)
constexpr uint32_t GLYPH_PLUGIN = 0xFFFF;     // Cell is occupied by a decorator plugin
constexpr uint32_t GLYPH_WIDE_CONT = 0xFFFE;  // Wide char continuation (look at previous cell)

// Legacy custom glyph range (conflicts with Nerd Fonts, prefer SHADER_GLYPH range)
constexpr uint32_t GLYPH_CUSTOM_START = 0xF000;
constexpr uint32_t GLYPH_CUSTOM_END = 0xFFFD;

// Shader glyph range - procedural glyphs rendered directly in terminal shader
// Uses Unicode Plane 16 Private Use Area-B (U+100000 - U+10FFFD)
// Codepoint IS the glyph index directly - no bit manipulation needed
// Files: src/yetty/shaders/1048577-spinner.wgsl -> codepoint 1048577 (U+100001)
constexpr uint32_t SHADER_GLYPH_START = 0x100000;  // 1048576 decimal (U+100000)
constexpr uint32_t SHADER_GLYPH_END = 0x10FFFD;    // 1114109 decimal (U+10FFFD)

// Helper to check if a glyph index is a shader glyph
inline bool isShaderGlyph(uint32_t glyphIndex) {
    return glyphIndex >= SHADER_GLYPH_START && glyphIndex <= SHADER_GLYPH_END;
}

// Get shader ID from glyph index (1048577 -> 1)
inline uint32_t getShaderGlyphId(uint32_t glyphIndex) {
    return glyphIndex - SHADER_GLYPH_START;
}

// Legacy helper (kept for compatibility)
inline bool isCustomGlyph(uint32_t glyphIndex) {
    return (glyphIndex >= GLYPH_CUSTOM_START && glyphIndex <= GLYPH_CUSTOM_END) ||
           isShaderGlyph(glyphIndex);
}

// Cell attributes packed into a single byte for GPU upload
// Bit layout: [reserved(2)][emoji][strikethrough][underline_type(2)][italic][bold]
// This matches what the shader expects in cellAttrsTexture (R8Uint)
struct CellAttrs {
    uint8_t _bold : 1;           // Bit 0: bold
    uint8_t _italic : 1;         // Bit 1: italic
    uint8_t _underline : 2;      // Bits 2-3: underline type (0=none, 1=single, 2=double, 3=curly)
    uint8_t _strikethrough : 1;  // Bit 4: strikethrough
    uint8_t _emoji : 1;          // Bit 5: emoji (render from color emoji atlas instead of MSDF)
    uint8_t _reserved : 2;       // Bits 6-7: reserved

    uint8_t pack() const {
        return static_cast<uint8_t>(
            (_bold & 0x1) |
            ((_italic & 0x1) << 1) |
            ((_underline & 0x3) << 2) |
            ((_strikethrough & 0x1) << 4) |
            ((_emoji & 0x1) << 5)
        );
    }

    static CellAttrs unpack(uint8_t packed) {
        CellAttrs attrs;
        attrs._bold = packed & 0x1;
        attrs._italic = (packed >> 1) & 0x1;
        attrs._underline = (packed >> 2) & 0x3;
        attrs._strikethrough = (packed >> 4) & 0x1;
        attrs._emoji = (packed >> 5) & 0x1;
        attrs._reserved = 0;
        return attrs;
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
