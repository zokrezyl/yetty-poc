// JSON-based DisplayList format for IPC between LibWeb and YDraw
// Matches Ladybird's DisplayListCommand structure
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <variant>
#include <optional>

namespace displaylist {

// Color as RGBA
struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 255;

    uint32_t toPackedRGBA() const {
        return (uint32_t(a) << 24) | (uint32_t(b) << 16) | (uint32_t(g) << 8) | r;
    }
};

struct Point {
    int x = 0, y = 0;
};

struct FloatPoint {
    float x = 0, y = 0;
};

struct Rect {
    int x = 0, y = 0, width = 0, height = 0;
};

struct CornerRadii {
    float top_left = 0, top_right = 0, bottom_left = 0, bottom_right = 0;
};

// Display list commands - matching LibWeb's DisplayListCommand.h

struct DrawGlyphRun {
    static constexpr const char* type = "DrawGlyphRun";
    Rect rect;
    FloatPoint translation;
    Color color;
    std::string text;  // Extracted text (glyphs converted to UTF-8)
    float font_size = 16.0f;
};

struct FillRect {
    static constexpr const char* type = "FillRect";
    Rect rect;
    Color color;
};

struct FillRectWithRoundedCorners {
    static constexpr const char* type = "FillRectWithRoundedCorners";
    Rect rect;
    Color color;
    CornerRadii radii;
};

struct DrawLine {
    static constexpr const char* type = "DrawLine";
    Color color;
    Point from;
    Point to;
    int thickness = 1;
};

struct FillEllipse {
    static constexpr const char* type = "FillEllipse";
    Rect rect;
    Color color;
};

struct DrawEllipse {
    static constexpr const char* type = "DrawEllipse";
    Rect rect;
    Color color;
    int thickness = 1;
};

struct FillPath {
    static constexpr const char* type = "FillPath";
    Rect bounding_rect;
    Color color;
    // Path segments serialized as SVG path data
    std::string path_data;
};

struct StrokePath {
    static constexpr const char* type = "StrokePath";
    Rect bounding_rect;
    Color color;
    float thickness = 1.0f;
    std::string path_data;
};

struct PaintLinearGradient {
    static constexpr const char* type = "PaintLinearGradient";
    Rect rect;
    // Simplified: just use start/end colors
    Color start_color;
    Color end_color;
};

struct PaintRadialGradient {
    static constexpr const char* type = "PaintRadialGradient";
    Rect rect;
    Point center;
    Color center_color;
    Color edge_color;
};

struct DrawScaledBitmap {
    static constexpr const char* type = "DrawScaledBitmap";
    Rect dst_rect;
    // We can't transfer actual bitmaps, so just draw a placeholder
    Color placeholder_color{200, 200, 200, 255};
};

struct PaintOuterBoxShadow {
    static constexpr const char* type = "PaintOuterBoxShadow";
    Rect rect;
    Color color;
    int blur_radius = 0;
    int offset_x = 0, offset_y = 0;
    CornerRadii radii;
};

struct PaintInnerBoxShadow {
    static constexpr const char* type = "PaintInnerBoxShadow";
    Rect rect;
    Color color;
    int blur_radius = 0;
};

struct Save {
    static constexpr const char* type = "Save";
};

struct Restore {
    static constexpr const char* type = "Restore";
};

struct Translate {
    static constexpr const char* type = "Translate";
    Point delta;
};

struct AddClipRect {
    static constexpr const char* type = "AddClipRect";
    Rect rect;
};

struct AddRoundedRectClip {
    static constexpr const char* type = "AddRoundedRectClip";
    Rect rect;
    CornerRadii radii;
};

// Command variant
using Command = std::variant<
    DrawGlyphRun,
    FillRect,
    FillRectWithRoundedCorners,
    DrawLine,
    FillEllipse,
    DrawEllipse,
    FillPath,
    StrokePath,
    PaintLinearGradient,
    PaintRadialGradient,
    DrawScaledBitmap,
    PaintOuterBoxShadow,
    PaintInnerBoxShadow,
    Save,
    Restore,
    Translate,
    AddClipRect,
    AddRoundedRectClip
>;

// DisplayList container
struct DisplayList {
    int width = 800;
    int height = 600;
    std::vector<Command> commands;
};

// Parse JSON to DisplayList
DisplayList parseJSON(const std::string& json);

// Serialize DisplayList to JSON (for testing)
std::string toJSON(const DisplayList& list);

} // namespace displaylist
