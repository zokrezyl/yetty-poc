#pragma once

#include "../ygui-widgets.h"

namespace yetty::ygui {

class HBox : public Widget {
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

        // Pass 1: sum fixed widths and stretch weights
        float fixedSum = 0;
        float stretchTotal = 0;
        for (auto& child : children) {
            if (child->stretch > 0)
                stretchTotal += child->stretch;
            else
                fixedSum += child->w;
        }
        float gaps = spacing * (children.size() - 1);
        float remaining = w - fixedSum - gaps;
        if (remaining < 0) remaining = 0;

        // Pass 2: position and render children
        float cursor = 0;
        RenderContext::OffsetGuard guard(ctx, x, y);
        for (auto& child : children) {
            float childW;
            if (child->stretch > 0 && stretchTotal > 0)
                childW = remaining * (child->stretch / stretchTotal);
            else
                childW = child->w;

            child->x = cursor;
            child->y = 0;
            child->w = childW;
            child->h = h;
            child->renderAll(ctx);
            cursor += childW + spacing;
        }
    }
};

} // namespace yetty::ygui
