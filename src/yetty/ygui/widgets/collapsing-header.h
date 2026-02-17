#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>

namespace yetty::ygui {

class CollapsingHeader : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, bgColor, 4);

        float arrowSize = 8;
        float arrowX = x + 10;
        float arrowY = y + h / 2;
        if (isOpen()) {
            ctx.triangle(arrowX, arrowY - arrowSize / 3,
                         arrowX + arrowSize, arrowY - arrowSize / 3,
                         arrowX + arrowSize / 2, arrowY + arrowSize / 3, fgColor);
        } else {
            ctx.triangle(arrowX, arrowY - arrowSize / 2,
                         arrowX, arrowY + arrowSize / 2,
                         arrowX + arrowSize * 0.7f, arrowY, fgColor);
        }

        ctx.text(label, x + 26, y + 4, fgColor);
        if (isHover())
            ctx.boxOutline(x, y, w, h, accentColor, 4);
    }

    void renderAll(RenderContext& ctx) override {
        effectiveX = x + ctx.offsetX();
        effectiveY = y + ctx.offsetY();
        wasRendered = true;
        render(ctx);
        if (isOpen()) {
            ctx.pushOffset(x, y + h);
            float yAccum = 0;
            for (auto& child : children) {
                ctx.pushOffset(0, yAccum);
                child->renderAll(ctx);
                ctx.popOffset();
                yAccum += child->h;
            }
            ctx.popOffset();
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        flags ^= WIDGET_OPEN;
        if (onClick.empty()) return {};
        return WidgetEvent{id, "click", "open", isOpen() ? "true" : "false"};
    }
};

} // namespace yetty::ygui
