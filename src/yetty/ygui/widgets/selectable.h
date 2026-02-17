#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>

namespace yetty::ygui {

class Selectable : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        if (isChecked())
            ctx.box(x, y, w, h, accentColor, 2);
        else if (isHover())
            ctx.box(x, y, w, h, 0xFF333344, 2);
        ctx.text(label, x + 8, y + 4, fgColor);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        flags ^= WIDGET_CHECKED;
        if (onClick.empty()) return {};
        return WidgetEvent{id, "click", "checked", isChecked() ? "true" : "false"};
    }
};

} // namespace yetty::ygui
