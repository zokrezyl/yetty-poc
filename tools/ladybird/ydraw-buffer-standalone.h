#pragma once

// Standalone YDraw buffer for OSC serialization
// This is a minimal version that matches yetty's YDrawBuffer format

#include <cstdint>
#include <vector>
#include <string>
#include <span>

namespace ydraw {

// Primitive types matching yetty's YDraw
enum class PrimType : uint8_t {
    Box = 0,
    RoundedBox = 1,
    Circle = 2,
    Segment = 3,
    Triangle = 4,
    Arc = 5,
    Bezier = 6,
    Text = 7,
};

// Packed primitive data (12 floats per primitive = 48 bytes)
struct alignas(16) Primitive {
    float data[12];
};

// Text entry for text primitives
struct TextEntry {
    std::string text;
    float x, y;
    float fontSize;
    uint32_t color;  // RGBA packed
};

// Color helpers
inline uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    return (uint32_t(a) << 24) | (uint32_t(b) << 16) | (uint32_t(g) << 8) | r;
}

// Standalone buffer for YDraw primitives
class Buffer {
public:
    Buffer() = default;

    void clear() {
        _primitives.clear();
        _texts.clear();
        _width = 0;
        _height = 0;
    }

    void setSize(float width, float height) {
        _width = width;
        _height = height;
    }

    float width() const { return _width; }
    float height() const { return _height; }

    // Add a filled box
    void addBox(float x, float y, float w, float h, uint32_t color);

    // Add a rounded box
    void addRoundedBox(float x, float y, float w, float h, float radius, uint32_t color);

    // Add a circle
    void addCircle(float cx, float cy, float radius, uint32_t color);

    // Add a line segment
    void addSegment(float x1, float y1, float x2, float y2, float thickness, uint32_t color);

    // Add text
    void addText(const std::string& text, float x, float y, float fontSize, uint32_t color);

    // Serialize to binary format for OSC transfer
    std::vector<uint8_t> serialize() const;

    // Get primitive count
    size_t primitiveCount() const { return _primitives.size(); }
    size_t textCount() const { return _texts.size(); }

private:
    std::vector<Primitive> _primitives;
    std::vector<TextEntry> _texts;
    float _width = 0;
    float _height = 0;
};

// Output OSC 666666 sequence to stdout
// cellWidth/cellHeight are terminal cell dimensions for the card
void outputOSC(const Buffer& buffer, int cardId = 0, int cellWidth = 80, int cellHeight = 24);

} // namespace ydraw
