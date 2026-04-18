#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"

namespace yetty::ygui {

class Button : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, isPressed() ? accentColor : bgColor, t.radiusMedium);
        ctx.text(label, x + t.padLarge, y + t.padMedium, fgColor);
        if (isHover())
            ctx.boxOutline(x, y, w, h, accentColor, t.radiusMedium);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        if (!onClick.empty())
            return WidgetEvent{id, "press", "", ""};
        return {};
    }

    std::optional<WidgetEvent> onRelease(float localX, float localY) override {
        // localX/Y are relative to this widget's effectiveX/Y
        if (localX < 0 || localX >= w || localY < 0 || localY >= h || onClick.empty())
            return {};
        return WidgetEvent{id, "click", "", ""};
    }
};

} // namespace yetty::ygui
