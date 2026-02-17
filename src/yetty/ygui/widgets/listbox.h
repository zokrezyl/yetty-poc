#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

namespace yetty::ygui {

class ListBox : public Widget {
public:
    std::vector<std::string> options;
    int selectedIndex = 0;
    int hoverIndex = -1;
    float scrollY = 0;
    float rowHeight = 24;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, bgColor, 4);
        ctx.boxOutline(x, y, w, h, 0xFF444455, 4);

        float itemH = rowHeight;
        float contentH = options.size() * itemH;
        bool needsScroll = contentH > h;
        float scrollbarW = needsScroll ? 10 : 0;
        float contentWidth = w - scrollbarW - 4;
        float maxScroll = std::max(0.0f, contentH - h);
        scrollY = std::clamp(scrollY, 0.0f, maxScroll);

        int startIdx = (int)(scrollY / itemH);
        int visibleCount = (int)(std::ceil(h / itemH)) + 1;
        int endIdx = std::min(startIdx + visibleCount, (int)options.size());

        for (int i = startIdx; i < endIdx; i++) {
            float itemY = y + i * itemH - scrollY;
            if (itemY + itemH <= y || itemY >= y + h) continue;

            bool isSelected = (i == selectedIndex);
            bool isHovered = (i == hoverIndex);

            if (isSelected)
                ctx.box(x + 2, itemY + 1, contentWidth, itemH - 2, accentColor, 2);
            else if (isHovered)
                ctx.box(x + 2, itemY + 1, contentWidth, itemH - 2, 0xFF333344, 2);

            ctx.text(options[i], x + 8, itemY + 4, fgColor);
        }

        if (needsScroll) {
            float trackH = h - 4;
            float thumbH = std::max(20.0f, trackH * h / contentH);
            float thumbY = y + 2 + (maxScroll > 0 ? (scrollY / maxScroll) * (trackH - thumbH) : 0);
            ctx.box(x + w - scrollbarW, y + 2, scrollbarW - 2, trackH, 0xFF222233, 4);
            ctx.box(x + w - scrollbarW + 1, thumbY, scrollbarW - 4, thumbH, 0xFF555566, 3);
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        float itemH = rowHeight;
        float localYScrolled = localY + scrollY;
        int idx = (int)(localYScrolled / itemH);
        if (idx >= 0 && idx < (int)options.size()) {
            selectedIndex = idx;
            if (!onChange.empty())
                return WidgetEvent{id, "change", "selected", std::to_string(selectedIndex)};
        }
        return {};
    }

    std::optional<WidgetEvent> onScroll(float dx, float dy) override {
        float contentH = options.size() * rowHeight;
        float maxScroll = std::max(0.0f, contentH - h);
        scrollY = std::clamp(scrollY - dy * rowHeight, 0.0f, maxScroll);
        return {};
    }
};

} // namespace yetty::ygui
