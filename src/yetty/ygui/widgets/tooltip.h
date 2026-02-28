#pragma once

#include "../ygui-widgets.h"
#include <string>

namespace yetty::ygui {

class Tooltip : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        if (label.empty()) return;
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, t.tooltipBg, t.radiusMedium);
        ctx.boxOutline(x, y, w, h, t.borderMuted, t.radiusMedium);
        ctx.text(label, x + t.padLarge - 2, y + t.padMedium, fgColor);
    }
};

} // namespace yetty::ygui
