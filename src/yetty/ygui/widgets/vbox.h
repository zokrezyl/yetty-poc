#pragma once

#include "../ygui-widgets.h"

namespace yetty::ygui {

class VBox : public Widget {
public:
    float spacing = 0;

    void render(RenderContext& ctx) override {
        if (bgColor) ctx.box(x, y, w, h, bgColor, 0);
    }

    void renderAll(RenderContext& ctx) override {
        effectiveX = x + ctx.offsetX();
        effectiveY = y + ctx.offsetY();
        wasRendered = true;
        render(ctx);

        if (children.empty()) return;

        // Pass 1: sum fixed heights and stretch weights
        float fixedSum = 0;
        float stretchTotal = 0;
        for (auto& child : children) {
            if (child->stretch > 0)
                stretchTotal += child->stretch;
            else
                fixedSum += child->h;
        }
        float gaps = spacing * (children.size() - 1);
        float remaining = h - fixedSum - gaps;
        if (remaining < 0) remaining = 0;

        // Pass 2: position and render children
        float cursor = 0;
        RenderContext::OffsetGuard guard(ctx, x, y);
        for (auto& child : children) {
            float childH;
            if (child->stretch > 0 && stretchTotal > 0)
                childH = remaining * (child->stretch / stretchTotal);
            else
                childH = child->h;

            child->x = 0;
            child->y = cursor;
            child->w = w;
            child->h = childH;
            child->renderAll(ctx);
            cursor += childH + spacing;
        }
    }
};

} // namespace yetty::ygui
