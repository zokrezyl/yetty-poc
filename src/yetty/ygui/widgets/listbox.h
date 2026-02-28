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
    float rowHeight = 0;  // 0 = use theme

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float itemH = rowHeight > 0 ? rowHeight : t.rowHeight;
        ctx.box(x, y, w, h, bgColor, t.radiusMedium);
        ctx.boxOutline(x, y, w, h, t.border, t.radiusMedium);

        float contentH = options.size() * itemH;
        bool needsScroll = contentH > h;
        float scrollbarW = needsScroll ? 10 : 0;
        float contentWidth = w - scrollbarW - t.padMedium;
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
                ctx.box(x + t.padSmall, itemY + 1, contentWidth, itemH - t.padSmall,
                        accentColor, t.radiusSmall);
            else if (isHovered)
                ctx.box(x + t.padSmall, itemY + 1, contentWidth, itemH - t.padSmall,
                        t.bgHover, t.radiusSmall);

            ctx.text(options[i], x + t.padLarge, itemY + t.padMedium, fgColor);
        }

        if (needsScroll) {
            float trackH = h - t.padMedium;
            float thumbH = std::max(20.0f, trackH * h / contentH);
            float thumbY = y + t.padSmall + (maxScroll > 0 ? (scrollY / maxScroll) * (trackH - thumbH) : 0);
            ctx.box(x + w - scrollbarW, y + t.padSmall, scrollbarW - t.padSmall,
                    trackH, t.bgSecondary, t.radiusMedium);
            ctx.box(x + w - scrollbarW + 1, thumbY, scrollbarW - t.padMedium,
                    thumbH, t.thumbHover, 3);
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        float itemH = rowHeight > 0 ? rowHeight : defaultTheme().rowHeight;
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
        float itemH = rowHeight > 0 ? rowHeight : defaultTheme().rowHeight;
        float contentH = options.size() * itemH;
        float maxScroll = std::max(0.0f, contentH - h);
        scrollY = std::clamp(scrollY - dy * itemH, 0.0f, maxScroll);
        return {};
    }
};

} // namespace yetty::ygui
