#pragma once

#include "../ygui-widgets.h"

namespace yetty::ygui {

class Progress : public Widget {
public:
    float value = 0;
    float minValue = 0;
    float maxValue = 1;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, bgColor, t.radiusMedium);
        float pct = (value - minValue) / (maxValue - minValue);
        float pad = t.padSmall;
        ctx.box(x + pad, y + pad, (w - pad * 2) * pct, h - pad * 2, accentColor, t.radiusSmall);
    }
};

} // namespace yetty::ygui
