// DisplayList to YDraw converter implementation

#include "displaylist-to-ydraw.h"
#include <algorithm>
#include <cmath>

namespace displaylist {

DisplayListToYDraw::DisplayListToYDraw(ydraw::Buffer& buffer)
    : _buffer(buffer)
{
}

void DisplayListToYDraw::execute(const DisplayList& list) {
    _buffer.setSize(static_cast<float>(list.width), static_cast<float>(list.height));
    _state = RenderState{};
    _state.clip_rect = {0, 0, list.width, list.height};
    _stateStack.clear();

    for (const auto& cmd : list.commands) {
        executeCommand(cmd);
    }
}

void DisplayListToYDraw::executeCommand(const Command& cmd) {
    std::visit([this](auto&& c) {
        using T = std::decay_t<decltype(c)>;
        if constexpr (std::is_same_v<T, FillRect>) handleFillRect(c);
        else if constexpr (std::is_same_v<T, FillRectWithRoundedCorners>) handleFillRectWithRoundedCorners(c);
        else if constexpr (std::is_same_v<T, DrawGlyphRun>) handleDrawGlyphRun(c);
        else if constexpr (std::is_same_v<T, DrawLine>) handleDrawLine(c);
        else if constexpr (std::is_same_v<T, FillEllipse>) handleFillEllipse(c);
        else if constexpr (std::is_same_v<T, DrawEllipse>) handleDrawEllipse(c);
        else if constexpr (std::is_same_v<T, FillPath>) handleFillPath(c);
        else if constexpr (std::is_same_v<T, StrokePath>) handleStrokePath(c);
        else if constexpr (std::is_same_v<T, PaintLinearGradient>) handlePaintLinearGradient(c);
        else if constexpr (std::is_same_v<T, PaintRadialGradient>) handlePaintRadialGradient(c);
        else if constexpr (std::is_same_v<T, DrawScaledBitmap>) handleDrawScaledBitmap(c);
        else if constexpr (std::is_same_v<T, PaintOuterBoxShadow>) handlePaintOuterBoxShadow(c);
        else if constexpr (std::is_same_v<T, PaintInnerBoxShadow>) handlePaintInnerBoxShadow(c);
        else if constexpr (std::is_same_v<T, Save>) handleSave(c);
        else if constexpr (std::is_same_v<T, Restore>) handleRestore(c);
        else if constexpr (std::is_same_v<T, Translate>) handleTranslate(c);
        else if constexpr (std::is_same_v<T, AddClipRect>) handleAddClipRect(c);
        else if constexpr (std::is_same_v<T, AddRoundedRectClip>) handleAddRoundedRectClip(c);
    }, cmd);
}

bool DisplayListToYDraw::isVisible(const Rect& r) const {
    Rect tr = {r.x + _state.translate_x, r.y + _state.translate_y, r.width, r.height};
    return !(tr.x >= _state.clip_rect.x + _state.clip_rect.width ||
             tr.x + tr.width <= _state.clip_rect.x ||
             tr.y >= _state.clip_rect.y + _state.clip_rect.height ||
             tr.y + tr.height <= _state.clip_rect.y);
}

Rect DisplayListToYDraw::clip(const Rect& r) const {
    int x1 = std::max(r.x + _state.translate_x, _state.clip_rect.x);
    int y1 = std::max(r.y + _state.translate_y, _state.clip_rect.y);
    int x2 = std::min(r.x + r.width + _state.translate_x, _state.clip_rect.x + _state.clip_rect.width);
    int y2 = std::min(r.y + r.height + _state.translate_y, _state.clip_rect.y + _state.clip_rect.height);
    return {x1, y1, std::max(0, x2 - x1), std::max(0, y2 - y1)};
}

void DisplayListToYDraw::handleFillRect(const FillRect& cmd) {
    if (!isVisible(cmd.rect)) return;
    Rect r = clip(cmd.rect);
    if (r.width > 0 && r.height > 0) {
        _buffer.addBox(
            static_cast<float>(r.x),
            static_cast<float>(r.y),
            static_cast<float>(r.width),
            static_cast<float>(r.height),
            cmd.color.toPackedRGBA()
        );
    }
}

void DisplayListToYDraw::handleFillRectWithRoundedCorners(const FillRectWithRoundedCorners& cmd) {
    if (!isVisible(cmd.rect)) return;
    Point p = transform(cmd.rect.x, cmd.rect.y);
    // Use average of corner radii
    float avgRadius = (cmd.radii.top_left + cmd.radii.top_right +
                       cmd.radii.bottom_left + cmd.radii.bottom_right) / 4.0f;
    _buffer.addRoundedBox(
        static_cast<float>(p.x),
        static_cast<float>(p.y),
        static_cast<float>(cmd.rect.width),
        static_cast<float>(cmd.rect.height),
        avgRadius,
        cmd.color.toPackedRGBA()
    );
}

void DisplayListToYDraw::handleDrawGlyphRun(const DrawGlyphRun& cmd) {
    if (!isVisible(cmd.rect)) return;
    Point p = transform(cmd.rect.x + static_cast<int>(cmd.translation.x),
                        cmd.rect.y + static_cast<int>(cmd.translation.y));
    _buffer.addText(
        cmd.text,
        static_cast<float>(p.x),
        static_cast<float>(p.y),
        cmd.font_size,
        cmd.color.toPackedRGBA()
    );
}

void DisplayListToYDraw::handleDrawLine(const DrawLine& cmd) {
    Point from = transform(cmd.from);
    Point to = transform(cmd.to);
    _buffer.addSegment(
        static_cast<float>(from.x),
        static_cast<float>(from.y),
        static_cast<float>(to.x),
        static_cast<float>(to.y),
        static_cast<float>(cmd.thickness),
        cmd.color.toPackedRGBA()
    );
}

void DisplayListToYDraw::handleFillEllipse(const FillEllipse& cmd) {
    if (!isVisible(cmd.rect)) return;
    Point center = transform(cmd.rect.x + cmd.rect.width / 2,
                             cmd.rect.y + cmd.rect.height / 2);
    // Use average of width and height for radius
    float radius = static_cast<float>(cmd.rect.width + cmd.rect.height) / 4.0f;
    _buffer.addCircle(
        static_cast<float>(center.x),
        static_cast<float>(center.y),
        radius,
        cmd.color.toPackedRGBA()
    );
}

void DisplayListToYDraw::handleDrawEllipse(const DrawEllipse& cmd) {
    // Draw ellipse as 4 arcs (simplified)
    // For now just draw a circle outline using segments
    if (!isVisible(cmd.rect)) return;
    Point center = transform(cmd.rect.x + cmd.rect.width / 2,
                             cmd.rect.y + cmd.rect.height / 2);
    float rx = cmd.rect.width / 2.0f;
    float ry = cmd.rect.height / 2.0f;
    float r = (rx + ry) / 2.0f;

    // Approximate circle with segments
    constexpr int SEGMENTS = 32;
    for (int i = 0; i < SEGMENTS; ++i) {
        float a1 = 2 * M_PI * i / SEGMENTS;
        float a2 = 2 * M_PI * (i + 1) / SEGMENTS;
        _buffer.addSegment(
            center.x + r * std::cos(a1), center.y + r * std::sin(a1),
            center.x + r * std::cos(a2), center.y + r * std::sin(a2),
            static_cast<float>(cmd.thickness),
            cmd.color.toPackedRGBA()
        );
    }
}

void DisplayListToYDraw::handleFillPath(const FillPath& cmd) {
    // For now, just fill the bounding rect
    // TODO: Parse SVG path data and decompose
    if (!isVisible(cmd.bounding_rect)) return;
    Point p = transform(cmd.bounding_rect.x, cmd.bounding_rect.y);
    _buffer.addBox(
        static_cast<float>(p.x),
        static_cast<float>(p.y),
        static_cast<float>(cmd.bounding_rect.width),
        static_cast<float>(cmd.bounding_rect.height),
        cmd.color.toPackedRGBA()
    );
}

void DisplayListToYDraw::handleStrokePath(const StrokePath& cmd) {
    // For now, just stroke the bounding rect outline
    // TODO: Parse SVG path data and decompose
    if (!isVisible(cmd.bounding_rect)) return;
    Rect r = cmd.bounding_rect;
    Point p1 = transform(r.x, r.y);
    Point p2 = transform(r.x + r.width, r.y);
    Point p3 = transform(r.x + r.width, r.y + r.height);
    Point p4 = transform(r.x, r.y + r.height);

    uint32_t color = cmd.color.toPackedRGBA();
    _buffer.addSegment(p1.x, p1.y, p2.x, p2.y, cmd.thickness, color);
    _buffer.addSegment(p2.x, p2.y, p3.x, p3.y, cmd.thickness, color);
    _buffer.addSegment(p3.x, p3.y, p4.x, p4.y, cmd.thickness, color);
    _buffer.addSegment(p4.x, p4.y, p1.x, p1.y, cmd.thickness, color);
}

void DisplayListToYDraw::handlePaintLinearGradient(const PaintLinearGradient& cmd) {
    // Simplified: use average color
    if (!isVisible(cmd.rect)) return;
    Point p = transform(cmd.rect.x, cmd.rect.y);
    uint8_t r = (cmd.start_color.r + cmd.end_color.r) / 2;
    uint8_t g = (cmd.start_color.g + cmd.end_color.g) / 2;
    uint8_t b = (cmd.start_color.b + cmd.end_color.b) / 2;
    uint8_t a = (cmd.start_color.a + cmd.end_color.a) / 2;
    _buffer.addBox(
        static_cast<float>(p.x),
        static_cast<float>(p.y),
        static_cast<float>(cmd.rect.width),
        static_cast<float>(cmd.rect.height),
        ydraw::packColor(r, g, b, a)
    );
}

void DisplayListToYDraw::handlePaintRadialGradient(const PaintRadialGradient& cmd) {
    // Simplified: use center color
    if (!isVisible(cmd.rect)) return;
    Point center = transform(cmd.center.x, cmd.center.y);
    float radius = static_cast<float>(std::min(cmd.rect.width, cmd.rect.height)) / 2.0f;
    _buffer.addCircle(
        static_cast<float>(center.x),
        static_cast<float>(center.y),
        radius,
        cmd.center_color.toPackedRGBA()
    );
}

void DisplayListToYDraw::handleDrawScaledBitmap(const DrawScaledBitmap& cmd) {
    // Draw placeholder rectangle for bitmap
    if (!isVisible(cmd.dst_rect)) return;
    Point p = transform(cmd.dst_rect.x, cmd.dst_rect.y);
    _buffer.addBox(
        static_cast<float>(p.x),
        static_cast<float>(p.y),
        static_cast<float>(cmd.dst_rect.width),
        static_cast<float>(cmd.dst_rect.height),
        cmd.placeholder_color.toPackedRGBA()
    );
}

void DisplayListToYDraw::handlePaintOuterBoxShadow(const PaintOuterBoxShadow& cmd) {
    // Draw larger semi-transparent box behind the element
    Point p = transform(cmd.rect.x - cmd.blur_radius + cmd.offset_x,
                        cmd.rect.y - cmd.blur_radius + cmd.offset_y);
    int expandedWidth = cmd.rect.width + 2 * cmd.blur_radius;
    int expandedHeight = cmd.rect.height + 2 * cmd.blur_radius;

    // Semi-transparent shadow
    Color shadow = cmd.color;
    shadow.a = shadow.a / 2;  // More transparent

    float avgRadius = (cmd.radii.top_left + cmd.radii.top_right +
                       cmd.radii.bottom_left + cmd.radii.bottom_right) / 4.0f + cmd.blur_radius;
    _buffer.addRoundedBox(
        static_cast<float>(p.x),
        static_cast<float>(p.y),
        static_cast<float>(expandedWidth),
        static_cast<float>(expandedHeight),
        avgRadius,
        shadow.toPackedRGBA()
    );
}

void DisplayListToYDraw::handlePaintInnerBoxShadow(const PaintInnerBoxShadow& cmd) {
    // For inner shadow, draw a slightly smaller semi-transparent box
    // This is a simplification
    Point p = transform(cmd.rect.x + cmd.blur_radius,
                        cmd.rect.y + cmd.blur_radius);
    int shrunkWidth = std::max(0, cmd.rect.width - 2 * cmd.blur_radius);
    int shrunkHeight = std::max(0, cmd.rect.height - 2 * cmd.blur_radius);

    if (shrunkWidth > 0 && shrunkHeight > 0) {
        Color shadow = cmd.color;
        shadow.a = shadow.a / 3;
        _buffer.addBox(
            static_cast<float>(p.x),
            static_cast<float>(p.y),
            static_cast<float>(shrunkWidth),
            static_cast<float>(shrunkHeight),
            shadow.toPackedRGBA()
        );
    }
}

void DisplayListToYDraw::handleSave(const Save&) {
    _stateStack.push_back(_state);
}

void DisplayListToYDraw::handleRestore(const Restore&) {
    if (!_stateStack.empty()) {
        _state = _stateStack.back();
        _stateStack.pop_back();
    }
}

void DisplayListToYDraw::handleTranslate(const Translate& cmd) {
    _state.translate_x += cmd.delta.x;
    _state.translate_y += cmd.delta.y;
}

void DisplayListToYDraw::handleAddClipRect(const AddClipRect& cmd) {
    Rect r = {cmd.rect.x + _state.translate_x, cmd.rect.y + _state.translate_y,
              cmd.rect.width, cmd.rect.height};

    // Intersect with existing clip
    int x1 = std::max(_state.clip_rect.x, r.x);
    int y1 = std::max(_state.clip_rect.y, r.y);
    int x2 = std::min(_state.clip_rect.x + _state.clip_rect.width, r.x + r.width);
    int y2 = std::min(_state.clip_rect.y + _state.clip_rect.height, r.y + r.height);
    _state.clip_rect = {x1, y1, std::max(0, x2 - x1), std::max(0, y2 - y1)};
}

void DisplayListToYDraw::handleAddRoundedRectClip(const AddRoundedRectClip& cmd) {
    // Treat as regular clip rect for simplicity
    handleAddClipRect(AddClipRect{cmd.rect});
}

} // namespace displaylist
