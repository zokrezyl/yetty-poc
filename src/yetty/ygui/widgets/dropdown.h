#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>
#include <vector>

namespace yetty::ygui {

class Dropdown : public Widget {
public:
    std::vector<std::string> options;
    int selectedIndex = 0;
    int hoverOptionIdx = -1;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, bgColor, 4);

        std::string text = (selectedIndex >= 0 && selectedIndex < (int)options.size())
            ? options[selectedIndex] : "";
        ctx.text(text, x + 8, y + 4, fgColor);

        // Chevron
        float arrowSize = 8;
        float arrowX = x + w - 16 - arrowSize / 2;
        float arrowY = y + h / 2;
        if (isOpen()) {
            ctx.triangle(arrowX, arrowY + arrowSize / 3,
                         arrowX + arrowSize, arrowY + arrowSize / 3,
                         arrowX + arrowSize / 2, arrowY - arrowSize / 3, fgColor);
        } else {
            ctx.triangle(arrowX, arrowY - arrowSize / 3,
                         arrowX + arrowSize, arrowY - arrowSize / 3,
                         arrowX + arrowSize / 2, arrowY + arrowSize / 3, fgColor);
        }
        if (isHover())
            ctx.boxOutline(x, y, w, h, accentColor, 4);

        // Render dropdown options when open
        if (isOpen()) {
            float optH = 24;
            float listH = (float)options.size() * optH;
            float optY = y + h;
            ctx.box(x, optY, w, listH, 0xFF1E1E2E, 4);

            for (size_t i = 0; i < options.size(); i++) {
                float oy = optY + i * optH;
                bool isSelected = ((int)i == selectedIndex);
                bool isHovered = ((int)i == hoverOptionIdx);

                if (isHovered)
                    ctx.box(x + 2, oy + 2, w - 4, optH - 4, accentColor, 2);
                else if (isSelected)
                    ctx.box(x + 2, oy + 2, w - 4, optH - 4, 0xFF2A2A3E, 2);

                ctx.text(options[i], x + 8, oy + 4, fgColor);
                if (isSelected)
                    ctx.text("\xE2\x9C\x93", x + w - 20, oy + 4, accentColor);
            }
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        if (isOpen()) {
            // Check if clicking on an option
            float optH = 24;
            float optY = h; // options start below the button
            if (localY >= optY) {
                int idx = (int)((localY - optY) / optH);
                if (idx >= 0 && idx < (int)options.size()) {
                    selectedIndex = idx;
                    flags &= ~WIDGET_OPEN;
                    if (!onChange.empty())
                        return WidgetEvent{id, "change", "selected", std::to_string(selectedIndex)};
                    return {};
                }
            }
            // Clicked on button itself or outside options — close
            flags &= ~WIDGET_OPEN;
        } else {
            flags |= WIDGET_OPEN;
            hoverOptionIdx = -1;
        }
        return {};
    }
};

} // namespace yetty::ygui
