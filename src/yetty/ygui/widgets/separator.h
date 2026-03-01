#pragma once

#include "../ygui-widgets.h"

namespace yetty::ygui {

class Separator : public Widget {
public:
    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float lineY = y + h / 2;
        ctx.box(x, lineY - t.separatorSize, w, t.separatorSize * 2, t.border, 0);
    }
};

} // namespace yetty::ygui
