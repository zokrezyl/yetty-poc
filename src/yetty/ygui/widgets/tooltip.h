#pragma once

#include "../ygui-widgets.h"
#include <string>

namespace yetty::ygui {

class Tooltip : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        if (label.empty()) return;
        ctx.box(x, y, w, h, 0xF0222233, 4);
        ctx.boxOutline(x, y, w, h, 0xFF666677, 4);
        ctx.text(label, x + 6, y + 4, fgColor);
    }
};

} // namespace yetty::ygui
