#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>

namespace yetty::ygui {

class CollapsingHeader : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, bgColor, t.radiusMedium);

        float arrowSize = t.padLarge;
        float arrowX = x + t.padLarge + 2;
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

        ctx.text(label, x + arrowSize + t.padLarge * 2 + 2, y + t.padMedium, fgColor);
        if (isHover())
            ctx.boxOutline(x, y, w, h, accentColor, t.radiusMedium);
    }

    void renderAll(RenderContext& ctx) override {
        effectiveX = x + ctx.offsetX();
        effectiveY = y + ctx.offsetY();
        wasRendered = true;
        render(ctx);
        if (isOpen()) {
            RenderContext::OffsetGuard outerGuard(ctx, x, y + h);
            float yAccum = 0;
            for (auto& child : children) {
                RenderContext::OffsetGuard innerGuard(ctx, 0, yAccum);
                child->renderAll(ctx);
                yAccum += child->h;
            }
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        flags ^= WIDGET_OPEN;
        if (onClick.empty()) return {};
        return WidgetEvent{id, "click", "open", isOpen() ? "true" : "false"};
    }
};

} // namespace yetty::ygui
