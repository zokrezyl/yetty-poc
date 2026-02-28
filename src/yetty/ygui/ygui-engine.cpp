#include "ygui-engine.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include <algorithm>
#include <cmath>

namespace yetty::ygui {

//=============================================================================
// RenderContext — maps high-level draw calls to YDrawBuffer primitives
//=============================================================================

void RenderContext::box(float x, float y, float w, float h, uint32_t color, float radius) {
    float ox = x + _offsetX, oy = y + _offsetY;
    if (isClipped(ox, oy, w, h)) return;
    // Clamp to clip rect if active
    if (hasClip()) {
        auto& c = currentClip();
        float x1 = std::max(ox, c.x);
        float y1 = std::max(oy, c.y);
        float x2 = std::min(ox + w, c.x + c.w);
        float y2 = std::min(oy + h, c.y + c.h);
        float cw = x2 - x1, ch = y2 - y1;
        _buffer->addRoundedBox(_buffer->primCount(),
            x1 + cw / 2, y1 + ch / 2, cw / 2, ch / 2,
            radius, radius, radius, radius,
            color, 0, 0.0f, 0.0f);
        return;
    }
    _buffer->addRoundedBox(_buffer->primCount(),
        ox + w / 2, oy + h / 2, w / 2, h / 2,
        radius, radius, radius, radius,
        color, 0, 0.0f, 0.0f);
}

void RenderContext::boxOutline(float x, float y, float w, float h, uint32_t color,
                               float radius, float strokeWidth) {
    float ox = x + _offsetX, oy = y + _offsetY;
    if (isClipped(ox, oy, w, h)) return;
    _buffer->addRoundedBox(_buffer->primCount(),
        ox + w / 2, oy + h / 2, w / 2, h / 2,
        radius, radius, radius, radius,
        0, color, strokeWidth, 0.0f);
}

void RenderContext::text(const std::string& text, float x, float y, uint32_t color,
                         float fontSize) {
    if (text.empty()) return;
    float ox = x + _offsetX, oy = y + _offsetY;
    if (hasClip()) {
        auto& c = currentClip();
        if (oy < c.y || oy + fontSize > c.y + c.h || ox > c.x + c.w) return;
    }
    _buffer->addText(ox, oy + fontSize * 0.8f, text, fontSize, color, 0, -1);
}

void RenderContext::triangle(float x0, float y0, float x1, float y1,
                             float x2, float y2, uint32_t color) {
    float ax = x0 + _offsetX, ay = y0 + _offsetY;
    float bx = x1 + _offsetX, by = y1 + _offsetY;
    float cx = x2 + _offsetX, cy = y2 + _offsetY;
    if (hasClip()) {
        float minX = std::min({ax, bx, cx}), maxX = std::max({ax, bx, cx});
        float minY = std::min({ay, by, cy}), maxY = std::max({ay, by, cy});
        if (isClipped(minX, minY, maxX - minX, maxY - minY)) return;
    }
    _buffer->addTriangle(_buffer->primCount(), ax, ay, bx, by, cx, cy,
                         color, 0, 0.0f, 0.0f);
}

void RenderContext::circle(float cx, float cy, float radius, uint32_t color) {
    float ox = cx + _offsetX, oy = cy + _offsetY;
    if (isClipped(ox - radius, oy - radius, radius * 2, radius * 2)) return;
    _buffer->addCircle(_buffer->primCount(), ox, oy, radius,
                       color, 0, 0.0f, 0.0f);
}

void RenderContext::circleOutline(float cx, float cy, float radius, uint32_t color,
                                  float strokeWidth) {
    float ox = cx + _offsetX, oy = cy + _offsetY;
    if (isClipped(ox - radius, oy - radius, radius * 2, radius * 2)) return;
    _buffer->addCircle(_buffer->primCount(), ox, oy, radius,
                       0, color, strokeWidth, 0.0f);
}

void RenderContext::roundedX(float cx, float cy, float size, float armWidth,
                             uint32_t color, float rounding) {
    float ox = cx + _offsetX, oy = cy + _offsetY;
    if (hasClip()) {
        if (isClipped(ox - size, oy - size, size * 2, size * 2)) return;
    }
    _buffer->addRoundedX(_buffer->primCount(), ox, oy, size, armWidth,
                         color, 0, 0.0f, rounding);
}

void RenderContext::colorWheel(float cx, float cy, float outerR, float innerR,
                               float hue, float sat, float val, float indicatorSize) {
    float ox = cx + _offsetX, oy = cy + _offsetY;
    if (isClipped(ox - outerR, oy - outerR, outerR * 2, outerR * 2)) return;
    _buffer->addColorWheel(_buffer->primCount(), ox, oy, outerR, innerR,
                           hue, sat, val, indicatorSize, 0, 0, 0.0f, 0.0f);
}

//=============================================================================
// SpatialGrid
//=============================================================================

void SpatialGrid::resize(float width, float height, float cellSize) {
    _cellSize = cellSize;
    _width = width;
    _height = height;
    _cols = std::max(1, (int)std::ceil(width / cellSize));
    _rows = std::max(1, (int)std::ceil(height / cellSize));
    _cells.clear();
    _cells.resize(_cols * _rows);
}

void SpatialGrid::insertSingle(const WidgetPtr& widget) {
    if (_cols == 0 || _rows == 0) return;
    int minC = std::max(0, (int)(widget->effectiveX / _cellSize));
    int minR = std::max(0, (int)(widget->effectiveY / _cellSize));
    int maxC = std::min(_cols - 1, (int)((widget->effectiveX + widget->w) / _cellSize));
    int maxR = std::min(_rows - 1, (int)((widget->effectiveY + widget->h) / _cellSize));

    for (int r = minR; r <= maxR; r++) {
        for (int c = minC; c <= maxC; c++) {
            _cells[r * _cols + c].push_back(widget);
        }
    }
}

void SpatialGrid::removeSingle(const WidgetPtr& widget) {
    if (_cols == 0 || _rows == 0) return;
    int minC = std::max(0, (int)(widget->effectiveX / _cellSize));
    int minR = std::max(0, (int)(widget->effectiveY / _cellSize));
    int maxC = std::min(_cols - 1, (int)((widget->effectiveX + widget->w) / _cellSize));
    int maxR = std::min(_rows - 1, (int)((widget->effectiveY + widget->h) / _cellSize));

    for (int r = minR; r <= maxR; r++) {
        for (int c = minC; c <= maxC; c++) {
            auto& cell = _cells[r * _cols + c];
            cell.erase(std::remove(cell.begin(), cell.end(), widget), cell.end());
        }
    }
}

void SpatialGrid::insert(const WidgetPtr& widget) {
    insertSingle(widget);
    for (auto& child : widget->children)
        insert(child);
}

void SpatialGrid::insertRendered(const WidgetPtr& widget) {
    if (widget->wasRendered)
        insertSingle(widget);
    for (auto& child : widget->children)
        insertRendered(child);
}

void SpatialGrid::remove(const WidgetPtr& widget) {
    for (auto& child : widget->children)
        remove(child);
    removeSingle(widget);
}

void SpatialGrid::clear() {
    for (auto& cell : _cells)
        cell.clear();
}

WidgetPtr SpatialGrid::queryAt(float px, float py) const {
    if (_cols == 0 || _rows == 0) return nullptr;
    int c = std::clamp((int)(px / _cellSize), 0, _cols - 1);
    int r = std::clamp((int)(py / _cellSize), 0, _rows - 1);
    const auto& cell = _cells[r * _cols + c];

    // Last inserted = on top, walk backwards
    for (int i = (int)cell.size() - 1; i >= 0; i--) {
        if (cell[i]->contains(px, py))
            return cell[i];
    }
    return nullptr;
}

//=============================================================================
// YGuiEngine
//=============================================================================

YGuiEngine::YGuiEngine(YDrawBuffer* buffer, YGuiIO* io)
    : _buffer(buffer), _io(io) {}

void YGuiEngine::setPixelSize(float width, float height) {
    _pixelWidth = width;
    _pixelHeight = height;
    _grid.resize(width, height);
    _dirty = true;
}

void YGuiEngine::setCellSize(float cellWidth, float cellHeight) {
    _cellWidthF = cellWidth;
    _cellHeightF = cellHeight;
    _dirty = true;
}

static void initEffectivePositions(const WidgetPtr& w) {
    w->effectiveX = w->x;
    w->effectiveY = w->y;
    for (auto& c : w->children)
        initEffectivePositions(c);
}

void YGuiEngine::addWidget(WidgetPtr widget) {
    initEffectivePositions(widget);
    _widgets.push_back(widget);
    _grid.insert(widget);
    _dirty = true;
}

void YGuiEngine::removeWidget(const WidgetPtr& widget) {
    _grid.remove(widget);
    _widgets.erase(std::remove(_widgets.begin(), _widgets.end(), widget), _widgets.end());
    _dirty = true;
}

void YGuiEngine::clearWidgets() {
    _grid.clear();
    _widgets.clear();
    _hovered = nullptr;
    _pressed = nullptr;
    _focused = nullptr;
    _dirty = true;
}

void YGuiEngine::clearInteractionState() {
    if (_pressed) {
        _pressed->onRelease(0, 0);  // let widget clean up drag state
        _pressed->flags &= ~WIDGET_PRESSED;
        _pressed = nullptr;
    }
    if (_hovered) {
        _hovered->onHoverLeave();
        _hovered = nullptr;
    }
    _dirty = true;
}

WidgetPtr YGuiEngine::widgetAt(float px, float py) const {
    return _grid.queryAt(px, py);
}

WidgetPtr YGuiEngine::findWidgetById(const std::string& id) const {
    for (auto& w : _widgets) {
        if (w->id == id) return w;
        // Search children recursively
        std::function<WidgetPtr(const WidgetPtr&)> search = [&](const WidgetPtr& widget) -> WidgetPtr {
            for (auto& child : widget->children) {
                if (child->id == id) return child;
                auto found = search(child);
                if (found) return found;
            }
            return nullptr;
        };
        auto found = search(w);
        if (found) return found;
    }
    return nullptr;
}

void YGuiEngine::emitEvent(const WidgetEvent& event) {
    if (_io && !event.eventType.empty())
        _io->emitEvent(event);
}

//=============================================================================
// Input handling — coordinates arrive in card pixel space
//=============================================================================

void YGuiEngine::handleMouseMove(float px, float py) {
    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    // Update hover via spatial grid
    auto hit = _grid.queryAt(wx, wy);
    if (hit != _hovered) {
        if (_hovered) _hovered->onHoverLeave();
        _hovered = hit;
        if (_hovered) _hovered->onHoverEnter();
        _dirty = true;
    }

    // Track drag on pressed widget
    if (_pressed) {
        auto event = _pressed->onDrag(wx - _pressed->effectiveX, wy - _pressed->effectiveY);
        if (event) emitEvent(*event);
        _dirty = true;
    }
}

void YGuiEngine::handleMouseDown(float px, float py, int button) {
    if (button != 0) return;

    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    auto hit = _grid.queryAt(wx, wy);
    if (!hit) return;

    // Update focus
    if (_focused != hit) {
        if (_focused) _focused->flags &= ~WIDGET_FOCUSED;
        _focused = hit;
        _focused->flags |= WIDGET_FOCUSED;
    }

    hit->flags |= WIDGET_PRESSED;
    _pressed = hit;

    auto event = hit->onPress(wx - hit->effectiveX, wy - hit->effectiveY);
    if (event) {
        emitEvent(*event);
        _dirty = true;
        return;
    }

    // Bubble: if child didn't handle press, try top-level widgets (e.g. Panel header drag)
    for (auto& root : _widgets) {
        if (root.get() != hit.get() && root->contains(wx, wy)) {
            auto ev = root->onPress(wx - root->effectiveX, wy - root->effectiveY);
            if (ev) {
                hit->flags &= ~WIDGET_PRESSED;
                _pressed = root;
                root->flags |= WIDGET_PRESSED;
                emitEvent(*ev);
                _dirty = true;
                return;
            }
        }
    }

    _dirty = true;
}

void YGuiEngine::handleMouseUp(float px, float py, int button) {
    if (button != 0) return;

    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    if (_pressed) {
        auto event = _pressed->onRelease(wx - _pressed->effectiveX, wy - _pressed->effectiveY);
        if (event) emitEvent(*event);
        _pressed->flags &= ~WIDGET_PRESSED;
        _pressed = nullptr;
        _dirty = true;
    }
}

// Find the deepest ancestor chain from root widgets to the hit widget
static WidgetPtr findScrollableAncestor(const WidgetPtr& root, const WidgetPtr& target) {
    if (root == target) return root;
    for (auto& child : root->children) {
        auto found = findScrollableAncestor(child, target);
        if (found) return found;
    }
    return nullptr;
}

void YGuiEngine::handleMouseScroll(float px, float py, float dx, float dy) {
    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    auto hit = _grid.queryAt(wx, wy);
    if (!hit) return;

    // Try the hit widget first
    auto event = hit->onScroll(dx, dy);
    if (event) {
        emitEvent(*event);
        _dirty = true;
        return;
    }

    // Bubble: try each top-level widget that contains the hit as a descendant
    for (auto& root : _widgets) {
        if (root->contains(wx, wy)) {
            auto ev = root->onScroll(dx, dy);
            if (ev) {
                emitEvent(*ev);
                _dirty = true;
                return;
            }
        }
    }
}

void YGuiEngine::handleKeyDown(uint32_t key, int mods) {
    if (!_focused) return;
    auto event = _focused->onKey(key, mods);
    if (event) emitEvent(*event);
    _dirty = true;
}

//=============================================================================
// Rendering
//=============================================================================

static void clearRenderState(const WidgetPtr& w) {
    w->wasRendered = false;
    for (auto& c : w->children)
        clearRenderState(c);
}

void YGuiEngine::rebuild() {
    if (!_buffer) return;

    _buffer->clear();
    _buffer->setSceneBounds(0, 0, _pixelWidth, _pixelHeight);
    _buffer->setBgColor(_bgColor);

    // Clear render state before rendering
    for (auto& w : _widgets)
        clearRenderState(w);

    RenderContext ctx(_buffer, _measureFn);
    for (auto& w : _widgets)
        w->renderAll(ctx);

    // Rebuild grid with effective positions computed by renderAll
    _grid.clear();
    for (auto& w : _widgets)
        _grid.insertRendered(w);

    _dirty = false;
}

} // namespace yetty::ygui
