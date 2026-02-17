#pragma once

#include "ygui-io.h"
#include <cstdint>
#include <cmath>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace yetty {
class YDrawBuffer;
}

namespace yetty::ygui {

//=============================================================================
// Widget state flags
//=============================================================================
enum WidgetFlags : uint32_t {
    WIDGET_HOVER    = 1 << 0,
    WIDGET_PRESSED  = 1 << 1,
    WIDGET_FOCUSED  = 1 << 2,
    WIDGET_DISABLED = 1 << 3,
    WIDGET_CHECKED  = 1 << 4,
    WIDGET_OPEN     = 1 << 5,
};

//=============================================================================
// RenderContext — convenience wrapper around YDrawBuffer with offset stack
//=============================================================================
class RenderContext {
public:
    explicit RenderContext(YDrawBuffer* buffer) : _buffer(buffer) {}

    void box(float x, float y, float w, float h, uint32_t color, float radius = 0);
    void boxOutline(float x, float y, float w, float h, uint32_t color,
                    float radius = 0, float strokeWidth = 2.0f);
    void text(const std::string& text, float x, float y, uint32_t color,
              float fontSize = 14.0f);
    void triangle(float x0, float y0, float x1, float y1,
                  float x2, float y2, uint32_t color);
    void circle(float cx, float cy, float radius, uint32_t color);
    void circleOutline(float cx, float cy, float radius, uint32_t color,
                       float strokeWidth = 1.5f);
    void colorWheel(float cx, float cy, float outerR, float innerR,
                    float hue, float sat, float val, float indicatorSize);

    // Offset stack for layout widgets (ScrollArea, CollapsingHeader, etc.)
    void pushOffset(float dx, float dy) {
        _offsetStack.push_back({_offsetX, _offsetY});
        _offsetX += dx;
        _offsetY += dy;
    }
    void popOffset() {
        if (!_offsetStack.empty()) {
            _offsetX = _offsetStack.back().first;
            _offsetY = _offsetStack.back().second;
            _offsetStack.pop_back();
        }
    }
    float offsetX() const { return _offsetX; }
    float offsetY() const { return _offsetY; }

    YDrawBuffer* buffer() const { return _buffer; }

private:
    YDrawBuffer* _buffer;
    float _offsetX = 0, _offsetY = 0;
    std::vector<std::pair<float, float>> _offsetStack;
};

//=============================================================================
// Widget — polymorphic base class
//
// Subclass to create custom widget types. The engine calls renderAll() which
// by default calls render() then recurses into children. Layout widgets
// override renderAll() to position children or apply offsets.
//=============================================================================
class Widget : public std::enable_shared_from_this<Widget> {
public:
    using Ptr = std::shared_ptr<Widget>;

    virtual ~Widget() = default;

    // Identity
    std::string id;

    // Geometry (local coordinates — relative to parent container)
    float x = 0, y = 0, w = 100, h = 24;

    // Effective screen position (computed during renderAll from offsets)
    float effectiveX = 0, effectiveY = 0;
    bool wasRendered = false;

    // State
    uint32_t flags = 0;

    bool isHover() const    { return flags & WIDGET_HOVER; }
    bool isPressed() const  { return flags & WIDGET_PRESSED; }
    bool isFocused() const  { return flags & WIDGET_FOCUSED; }
    bool isDisabled() const { return flags & WIDGET_DISABLED; }
    bool isChecked() const  { return flags & WIDGET_CHECKED; }
    bool isOpen() const     { return flags & WIDGET_OPEN; }

    bool contains(float px, float py) const {
        return px >= effectiveX && px < effectiveX + w
            && py >= effectiveY && py < effectiveY + h;
    }

    // Children
    std::vector<Ptr> children;

    // Styling
    uint32_t bgColor = 0xFF2A2A3E;
    uint32_t fgColor = 0xFFFFFFFF;
    uint32_t accentColor = 0xFF4488FF;

    // Event names (emitted via IO when triggered)
    std::string onClick;
    std::string onChange;

    //=========================================================================
    // Virtual interface — override in subclasses
    //=========================================================================

    // Render this widget into the buffer
    virtual void render(RenderContext& ctx) {}

    // Render self + children. Override for layout widgets that need to
    // reposition children or apply scroll offsets.
    virtual void renderAll(RenderContext& ctx) {
        effectiveX = x + ctx.offsetX();
        effectiveY = y + ctx.offsetY();
        wasRendered = true;
        render(ctx);
        for (auto& child : children)
            child->renderAll(ctx);
    }

    // Input handlers — return an event to emit, or nullopt
    virtual std::optional<WidgetEvent> onPress(float localX, float localY) { return {}; }
    virtual std::optional<WidgetEvent> onRelease(float localX, float localY) { return {}; }
    virtual std::optional<WidgetEvent> onDrag(float localX, float localY) { return {}; }
    virtual std::optional<WidgetEvent> onScroll(float dx, float dy) { return {}; }
    virtual std::optional<WidgetEvent> onKey(uint32_t key, int mods) { return {}; }

    // Hover (no events, just state changes)
    virtual void onHoverEnter() { flags |= WIDGET_HOVER; }
    virtual void onHoverLeave() { flags &= ~WIDGET_HOVER; }
};

using WidgetPtr = Widget::Ptr;

} // namespace yetty::ygui
