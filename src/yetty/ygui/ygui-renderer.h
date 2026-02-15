#pragma once

#include "ygui-widgets.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace yetty {
class YDrawWriter;
}

namespace yetty::ygui {

//=============================================================================
// YGuiRenderState - tracks hover/selection state for rendering
//=============================================================================
struct YGuiRenderState {
    Widget* hoverChoiceWidget = nullptr;  // ChoiceBox being hovered
    Widget* hoverListWidget = nullptr;    // ListBox being hovered
    Widget* openDropdown = nullptr;       // Currently open dropdown
    int hoverChoiceIdx = -1;              // Index in ChoiceBox
    int hoverListIdx = -1;                // Index in ListBox
    int hoverOptionIdx = -1;              // Index in dropdown options
    float pixelWidth = 0;                 // Scene width
    float pixelHeight = 0;                // Scene height
};

//=============================================================================
// YGuiRenderer - renders widgets to a YDrawWriter
//
// Works with any YDrawWriter implementation:
//   - YDrawWriterInternal: for in-process GPU rendering
//   - YDrawWriterBinary: for generating binary ydraw streams
//=============================================================================
class YGuiRenderer {
public:
    YGuiRenderer(std::shared_ptr<YDrawWriter> writer);

    /// Render all widgets in the list.
    void render(const std::vector<WidgetPtr>& widgets,
                const YGuiRenderState& state);

    /// Render a single widget tree.
    void renderWidget(Widget* w, float offsetX, float offsetY,
                      const YGuiRenderState& state);

    /// Set scene bounds for rendering.
    void setSceneBounds(float width, float height);

    /// Set background color.
    void setBgColor(uint32_t color);

    /// Clear all primitives and reset builder.
    void clear();

private:
    // Primitive helpers - delegate to YDrawWriter
    void addBox(float x, float y, float w, float h, uint32_t color, float radius);
    void addBoxOutline(float x, float y, float w, float h, uint32_t color, float radius);
    void addText(const std::string& text, float x, float y, uint32_t color);
    void addTriangle(float x0, float y0, float x1, float y1, float x2, float y2,
                     uint32_t color);
    void addCircle(float cx, float cy, float radius, uint32_t color);
    void addCircleOutline(float cx, float cy, float radius, uint32_t color);

    // Widget-specific rendering
    void renderDropdownOptions(Widget* w, const YGuiRenderState& state);

    std::shared_ptr<YDrawWriter> _writer;
    float _pixelWidth = 0;
    float _pixelHeight = 0;
};

} // namespace yetty::ygui
