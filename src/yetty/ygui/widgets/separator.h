#pragma once

#include "../ygui-widgets.h"

namespace yetty::ygui {

class Separator : public Widget {
public:
    void render(RenderContext& ctx) override {
        float lineY = y + h / 2;
        ctx.box(x, lineY - 1, w, 2, 0xFF444455, 0);
    }
};

} // namespace yetty::ygui
