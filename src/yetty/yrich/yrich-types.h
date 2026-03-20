#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace yetty::yrich {

//=============================================================================
// ElementId — unique identifier for elements within a document
//=============================================================================
using ElementId = uint64_t;
constexpr ElementId INVALID_ELEMENT_ID = 0;

//=============================================================================
// Rect — axis-aligned rectangle
//=============================================================================
struct Rect {
    float x = 0, y = 0, w = 0, h = 0;

    bool contains(float px, float py) const {
        return px >= x && px < x + w && py >= y && py < y + h;
    }

    bool intersects(const Rect& other) const {
        return x < other.x + other.w && x + w > other.x &&
               y < other.y + other.h && y + h > other.y;
    }

    Rect unite(const Rect& other) const {
        float nx = std::min(x, other.x);
        float ny = std::min(y, other.y);
        float nx2 = std::max(x + w, other.x + other.w);
        float ny2 = std::max(y + h, other.y + other.h);
        return {nx, ny, nx2 - nx, ny2 - ny};
    }
};

//=============================================================================
// Color — RGBA color (stored as packed uint32_t internally)
//=============================================================================
struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 255;

    constexpr Color() = default;
    constexpr Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255)
        : r(r_), g(g_), b(b_), a(a_) {}

    static constexpr Color fromPacked(uint32_t packed) {
        return Color{
            static_cast<uint8_t>(packed & 0xFF),
            static_cast<uint8_t>((packed >> 8) & 0xFF),
            static_cast<uint8_t>((packed >> 16) & 0xFF),
            static_cast<uint8_t>((packed >> 24) & 0xFF)
        };
    }

    constexpr uint32_t toPacked() const {
        return r | (g << 8) | (b << 16) | (a << 24);
    }

    static constexpr Color white() { return {255, 255, 255, 255}; }
    static constexpr Color black() { return {0, 0, 0, 255}; }
    static constexpr Color transparent() { return {0, 0, 0, 0}; }
};

//=============================================================================
// TextFormat — formatting flags (can be combined)
//=============================================================================
enum class TextFormat : uint32_t {
    None       = 0,
    Bold       = 1 << 0,
    Italic     = 1 << 1,
    Underline  = 1 << 2,
    Strike     = 1 << 3,
    Subscript  = 1 << 4,
    Superscript= 1 << 5,
};

inline TextFormat operator|(TextFormat a, TextFormat b) {
    return static_cast<TextFormat>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
inline TextFormat operator&(TextFormat a, TextFormat b) {
    return static_cast<TextFormat>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
inline TextFormat operator~(TextFormat a) {
    return static_cast<TextFormat>(~static_cast<uint32_t>(a));
}
inline bool hasFormat(TextFormat flags, TextFormat flag) {
    return (static_cast<uint32_t>(flags) & static_cast<uint32_t>(flag)) != 0;
}

//=============================================================================
// TextStyle — text formatting attributes
//=============================================================================
struct TextStyle {
    float fontSize = 14.0f;
    Color color = Color::black();
    Color bgColor = Color::transparent();
    TextFormat format = TextFormat::None;
    int fontId = 0;

    bool bold() const { return hasFormat(format, TextFormat::Bold); }
    bool italic() const { return hasFormat(format, TextFormat::Italic); }
    bool underline() const { return hasFormat(format, TextFormat::Underline); }
    bool strike() const { return hasFormat(format, TextFormat::Strike); }

    void setBold(bool v) {
        if (v) format = format | TextFormat::Bold;
        else format = format & ~TextFormat::Bold;
    }
    void setItalic(bool v) {
        if (v) format = format | TextFormat::Italic;
        else format = format & ~TextFormat::Italic;
    }
    void setUnderline(bool v) {
        if (v) format = format | TextFormat::Underline;
        else format = format & ~TextFormat::Underline;
    }
};

//=============================================================================
// BorderStyle — border attributes for cells/shapes
//=============================================================================
struct BorderStyle {
    float width = 1.0f;
    Color color = Color::black();
    enum Style { None, Solid, Dashed, Dotted } style = Solid;
};

//=============================================================================
// CellAddress — spreadsheet cell reference (row, col)
//=============================================================================
struct CellAddress {
    int32_t row = 0;
    int32_t col = 0;

    bool operator==(const CellAddress& other) const {
        return row == other.row && col == other.col;
    }
    bool operator<(const CellAddress& other) const {
        return row < other.row || (row == other.row && col < other.col);
    }
};

//=============================================================================
// CellRange — rectangular range of cells (A1:C10)
//=============================================================================
struct CellRange {
    CellAddress start;
    CellAddress end;

    bool contains(const CellAddress& addr) const {
        return addr.row >= start.row && addr.row <= end.row &&
               addr.col >= start.col && addr.col <= end.col;
    }

    int32_t rowCount() const { return end.row - start.row + 1; }
    int32_t colCount() const { return end.col - start.col + 1; }
};

//=============================================================================
// InputModifiers — keyboard modifiers for input events
//=============================================================================
struct InputModifiers {
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    bool meta = false;
};

//=============================================================================
// MouseButton — mouse button identifiers
//=============================================================================
enum class MouseButton { Left = 0, Right = 1, Middle = 2 };

//=============================================================================
// Key — common key codes
//=============================================================================
enum class Key {
    Unknown = 0,
    Enter, Tab, Backspace, Delete, Escape,
    Left, Right, Up, Down,
    Home, End, PageUp, PageDown,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
};

} // namespace yetty::yrich
