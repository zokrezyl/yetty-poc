#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"

namespace yetty::ygui {

class Checkbox : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float boxSize = 16;
        ctx.box(x, y + t.padSmall, boxSize, boxSize, bgColor, t.radiusSmall);
        if (isChecked())
            ctx.box(x + 3, y + t.padSmall + 3, boxSize - 6, boxSize - 6, accentColor, t.radiusSmall);
        ctx.text(label, x + boxSize + t.padLarge, y + t.padSmall, fgColor);
        if (isHover())
            ctx.boxOutline(x, y + t.padSmall, boxSize, boxSize, accentColor, t.radiusSmall);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        flags ^= WIDGET_CHECKED;
        if (onChange.empty()) return {};
        return WidgetEvent{id, "change", "checked", isChecked() ? "true" : "false"};
    }
};

} // namespace yetty::ygui
