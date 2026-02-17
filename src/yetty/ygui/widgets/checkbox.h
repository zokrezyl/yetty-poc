#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"

namespace yetty::ygui {

class Checkbox : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        float boxSize = 16;
        ctx.box(x, y + 2, boxSize, boxSize, bgColor, 2);
        if (isChecked())
            ctx.box(x + 3, y + 5, boxSize - 6, boxSize - 6, accentColor, 2);
        ctx.text(label, x + boxSize + 8, y + 2, fgColor);
        if (isHover())
            ctx.boxOutline(x, y + 2, boxSize, boxSize, accentColor, 2);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        flags ^= WIDGET_CHECKED;
        if (onChange.empty()) return {};
        return WidgetEvent{id, "change", "checked", isChecked() ? "true" : "false"};
    }
};

} // namespace yetty::ygui
