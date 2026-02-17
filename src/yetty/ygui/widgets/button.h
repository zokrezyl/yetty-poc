#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"

namespace yetty::ygui {

class Button : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, isPressed() ? accentColor : bgColor, 4);
        ctx.text(label, x + 8, y + 4, fgColor);
        if (isHover())
            ctx.boxOutline(x, y, w, h, accentColor, 4);
    }

    std::optional<WidgetEvent> onRelease(float localX, float localY) override {
        if (!contains(localX + x, localY + y) || onClick.empty())
            return {};
        return WidgetEvent{id, "click", "", ""};
    }
};

} // namespace yetty::ygui
