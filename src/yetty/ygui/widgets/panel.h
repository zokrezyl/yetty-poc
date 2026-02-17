#pragma once

#include "../ygui-widgets.h"

namespace yetty::ygui {

class Panel : public Widget {
public:
    float cornerRadius = 8;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, bgColor, cornerRadius);
    }

    void renderAll(RenderContext& ctx) override {
        effectiveX = x + ctx.offsetX();
        effectiveY = y + ctx.offsetY();
        wasRendered = true;
        render(ctx);
        ctx.pushOffset(x, y);
        for (auto& child : children)
            child->renderAll(ctx);
        ctx.popOffset();
    }
};

} // namespace yetty::ygui
