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
            ctx.box(x, y, w, h, bgColor, 4);
        }
    }
};

class TabBar : public Widget {
public:
    int activeTab = 0;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, bgColor, 4);

        float tabX = x + 4;
        int tabIdx = 0;
        for (auto& child : children) {
            bool isActive = (tabIdx == activeTab);
            float tabW = child->w > 0 ? child->w : 80;
            float tabH = h - 4;

            if (isActive)
                ctx.box(tabX, y + 2, tabW, tabH, accentColor, 4);
            else if (child->isHover())
                ctx.box(tabX, y + 2, tabW, tabH, 0xFF333344, 4);

            auto* tabItem = dynamic_cast<TabItem*>(child.get());
            std::string label;
            if (tabItem) label = tabItem->label;
            ctx.text(label, tabX + 8, y + 6, isActive ? 0xFFFFFFFF : 0xFFAAAAAA);

            tabX += tabW + 4;
            tabIdx++;
        }
    }

    // Children (TabItems) are rendered inline by render() — don't recurse
    void renderAll(RenderContext& ctx) override {
        render(ctx);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        float tabX = 4;
        int tabIdx = 0;
        for (auto& child : children) {
            float tabW = child->w > 0 ? child->w : 80;
            if (localX >= tabX && localX < tabX + tabW) {
                activeTab = tabIdx;
                if (!onChange.empty())
                    return WidgetEvent{id, "change", "selected", std::to_string(activeTab)};
                return {};
            }
            tabX += tabW + 4;
            tabIdx++;
        }
        return {};
    }
};

} // namespace yetty::ygui
