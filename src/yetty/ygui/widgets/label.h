#pragma once

#include "../ygui-widgets.h"

namespace yetty::ygui {

class Label : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        ctx.text(label, x, y, fgColor);
    }
};

} // namespace yetty::ygui
