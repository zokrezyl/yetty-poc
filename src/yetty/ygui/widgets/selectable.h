#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>

namespace yetty::ygui {

class Selectable : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        if (isChecked())
            ctx.box(x, y, w, h, accentColor, t.radiusSmall);
        else if (isHover())
            ctx.box(x, y, w, h, t.bgHover, t.radiusSmall);
        ctx.text(label, x + t.padLarge, y + t.padMedium, fgColor);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        flags ^= WIDGET_CHECKED;
        if (onClick.empty()) return {};
        return WidgetEvent{id, "click", "checked", isChecked() ? "true" : "false"};
    }
};

} // namespace yetty::ygui
