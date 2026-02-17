#pragma once

#include "../ygui-widgets.h"

namespace yetty::ygui {

class Progress : public Widget {
public:
    float value = 0;
    float minValue = 0;
    float maxValue = 1;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, bgColor, 4);
        float pct = (value - minValue) / (maxValue - minValue);
        ctx.box(x + 2, y + 2, (w - 4) * pct, h - 4, accentColor, 2);
    }
};

} // namespace yetty::ygui
