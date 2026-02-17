#pragma once

#include "ygui-widgets.h"
#include "ygui-io.h"
#include <cstdint>
#include <string>
#include <vector>

namespace yetty {
class YDrawBuffer;
}

namespace yetty::ygui {

//=============================================================================
// SpatialGrid — O(1) widget lookup by screen position
//
// Updated immediately when widgets are added/removed/moved.
//=============================================================================
class SpatialGrid {
public:
    SpatialGrid() = default;

    // Resize the grid to cover the given pixel area
    void resize(float width, float height, float cellSize = 32.0f);

    // Insert a widget (uses its x, y, w, h). Also inserts all descendants.
    void insert(const WidgetPtr& widget);

    // Remove a widget and all its descendants from the grid.
    void remove(const WidgetPtr& widget);

    // Insert rendered widgets only (checks wasRendered flag). Recurses into children.
    void insertRendered(const WidgetPtr& widget);

    // Clear all entries.
    void clear();

    // Find the topmost widget at the given point (last inserted = on top).
    WidgetPtr queryAt(float px, float py) const;

private:
    void insertSingle(const WidgetPtr& widget);
    void removeSingle(const WidgetPtr& widget);

    float _cellSize = 32.0f;
    int _cols = 0, _rows = 0;
    float _width = 0, _height = 0;

    // Each cell holds widgets that overlap it, in insertion order (last = top)
    std::vector<std::vector<WidgetPtr>> _cells;
};

//=============================================================================
// YGuiEngine — standalone widget engine
//
// - Widgets are added programmatically via addWidget()
// - Spatial grid updates instantly on add/remove
// - rebuild() renders all widgets to the YDrawBuffer
// - Input events use the grid for O(1) widget lookup
// - No dependency on builder, card, EventLoop, GPU, or YAML
//=============================================================================
class YGuiEngine {
public:
    YGuiEngine(YDrawBuffer* buffer, YGuiIO* io = nullptr);

    // Scene setup
    void setPixelSize(float width, float height);
    void setCellSize(float cellWidth, float cellHeight);
    void setBgColor(uint32_t color) { _bgColor = color; }

    // Widget management — grid updates instantly
    void addWidget(WidgetPtr widget);
    void removeWidget(const WidgetPtr& widget);
    void clearWidgets();

    // Widget lookup
    WidgetPtr widgetAt(float px, float py) const;
    WidgetPtr findWidgetById(const std::string& id) const;

    // Input (coordinates in card pixel space — engine converts to widget space)
    void handleMouseMove(float px, float py);
    void handleMouseDown(float px, float py, int button);
    void handleMouseUp(float px, float py, int button);
    void handleMouseScroll(float px, float py, float dx, float dy);
    void handleKeyDown(uint32_t key, int mods);

    // Render all widgets to buffer. Call when dirty.
    void rebuild();

    // State
    bool isDirty() const { return _dirty; }
    void markDirty() { _dirty = true; }

private:
    // Coordinate conversion (card pixels → widget space, assuming 10x20 nominal)
    float toWidgetX(float px) const { return px * 10.0f / _cellWidthF; }
    float toWidgetY(float py) const { return py * 20.0f / _cellHeightF; }

    void emitEvent(const WidgetEvent& event);

    YDrawBuffer* _buffer;
    YGuiIO* _io;
    SpatialGrid _grid;

    std::vector<WidgetPtr> _widgets;  // top-level widgets

    float _pixelWidth = 0;
    float _pixelHeight = 0;
    float _cellWidthF = 10.0f;
    float _cellHeightF = 20.0f;
    uint32_t _bgColor = 0xFF1A1A2E;

    WidgetPtr _hovered;   // currently hovered widget
    WidgetPtr _pressed;   // currently pressed widget (tracks drag)
    WidgetPtr _focused;   // keyboard focus

    bool _dirty = true;
};

} // namespace yetty::ygui
