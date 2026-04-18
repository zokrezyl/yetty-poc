#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>
#include <vector>

namespace yetty::ygui {

class ChoiceBox : public Widget {
public:
    std::vector<std::string> options;
    int selectedIndex = 0;
    int hoverIndex = -1;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float optH = t.rowHeight;
        float cy = y;
        for (size_t i = 0; i < options.size(); i++) {
            bool isSelected = ((int)i == selectedIndex);
            bool isHovered = ((int)i == hoverIndex);
            float radioSize = 14;
            float centerX = x + radioSize / 2;
            float centerY = cy + t.padMedium + radioSize / 2;

            ctx.circleOutline(centerX, centerY, radioSize / 2,
                              isHovered ? accentColor : t.borderMuted);
            if (isSelected)
                ctx.circle(centerX, centerY, radioSize / 4, accentColor);
            if (isHovered && !isSelected)
                ctx.circle(centerX, centerY, radioSize / 6, t.thumbHover);

            ctx.text(options[i], x + radioSize + t.padLarge, cy + t.padMedium, fgColor);
            cy += optH;
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        float optH = defaultTheme().rowHeight;
        int idx = (int)(localY / optH);
        if (idx >= 0 && idx < (int)options.size()) {
            selectedIndex = idx;
            if (!onChange.empty())
                return WidgetEvent{id, "change", "selected", std::to_string(selectedIndex)};
        }
        return {};
    }
};

} // namespace yetty::ygui
