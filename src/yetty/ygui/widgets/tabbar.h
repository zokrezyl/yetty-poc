#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>

namespace yetty::ygui {

class TabItem : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        if (isOpen()) {
            ctx.box(x, y, w, h, bgColor, ctx.theme().radiusMedium);
        }
    }
};

class TabBar : public Widget {
public:
    int activeTab = 0;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, bgColor, t.radiusMedium);

        float tabX = x + t.padMedium;
        int tabIdx = 0;
        for (auto& child : children) {
            bool isActive = (tabIdx == activeTab);
            float tabW = child->w > 0 ? child->w : 80;
            float tabH = h - t.padMedium;

            if (isActive)
                ctx.box(tabX, y + t.padSmall, tabW, tabH, accentColor, t.radiusMedium);
            else if (child->isHover())
                ctx.box(tabX, y + t.padSmall, tabW, tabH, t.bgHover, t.radiusMedium);

            auto* tabItem = dynamic_cast<TabItem*>(child.get());
            std::string label;
            if (tabItem) label = tabItem->label;
            ctx.text(label, tabX + t.padLarge, y + t.padLarge - 2,
                     isActive ? t.textPrimary : t.textMuted);

            tabX += tabW + t.padMedium;
            tabIdx++;
        }
    }

    // Children (TabItems) are rendered inline by render() — don't recurse
    void renderAll(RenderContext& ctx) override {
        render(ctx);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        auto& t = defaultTheme();
        float tabX = t.padMedium;
        int tabIdx = 0;
        for (auto& child : children) {
            float tabW = child->w > 0 ? child->w : 80;
            if (localX >= tabX && localX < tabX + tabW) {
                activeTab = tabIdx;
                if (!onChange.empty())
                    return WidgetEvent{id, "change", "selected", std::to_string(activeTab)};
                return {};
            }
            tabX += tabW + t.padMedium;
            tabIdx++;
        }
        return {};
    }
};

} // namespace yetty::ygui
