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

    bool contains(float px, float py) const override {
        if (Widget::contains(px, py)) return true;
        if (isOpen()) {
            float optH = defaultTheme().rowHeight;
            float listH = (float)options.size() * optH;
            return px >= effectiveX && px < effectiveX + w
                && py >= effectiveY + h && py < effectiveY + h + listH;
        }
        return false;
    }

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, bgColor, t.radiusMedium);

        std::string text = (selectedIndex >= 0 && selectedIndex < (int)options.size())
            ? options[selectedIndex] : "";
        ctx.text(text, x + t.padLarge, y + t.padMedium, fgColor);

        // Chevron
        float arrowSize = t.padLarge;
        float arrowX = x + w - t.padLarge * 2 - arrowSize / 2;
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
            ctx.boxOutline(x, y, w, h, accentColor, t.radiusMedium);

        // Render dropdown options when open
        if (isOpen()) {
            float optH = t.rowHeight;
            float listH = (float)options.size() * optH;
            float optY = y + h;
            ctx.box(x, optY, w, listH, t.bgDropdown, t.radiusMedium);

            for (size_t i = 0; i < options.size(); i++) {
                float oy = optY + i * optH;
                bool isSel = ((int)i == selectedIndex);
                bool isHov = ((int)i == hoverOptionIdx);

                if (isHov)
                    ctx.box(x + t.padSmall, oy + t.padSmall, w - t.padSmall * 2,
                            optH - t.padSmall * 2, accentColor, t.radiusSmall);
                else if (isSel)
                    ctx.box(x + t.padSmall, oy + t.padSmall, w - t.padSmall * 2,
                            optH - t.padSmall * 2, t.bgSurface, t.radiusSmall);

                ctx.text(options[i], x + t.padLarge, oy + t.padMedium, fgColor);
                if (isSel)
                    ctx.text("\xE2\x9C\x93", x + w - 20, oy + t.padMedium, accentColor);
            }
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        if (isOpen()) {
            float optH = defaultTheme().rowHeight;
            float optY = h;
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
            flags &= ~WIDGET_OPEN;
        } else {
            flags |= WIDGET_OPEN;
            hoverOptionIdx = -1;
        }
        return {};
    }
};

} // namespace yetty::ygui
