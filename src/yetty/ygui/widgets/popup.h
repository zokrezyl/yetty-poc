#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>

namespace yetty::ygui {

class Popup : public Widget {
public:
    std::string label;
    bool modal = false;
    uint32_t headerColor = 0;  // 0 = use theme
    float sceneWidth = 0;
    float sceneHeight = 0;

    void render(RenderContext& ctx) override {
        if (!isOpen()) return;
        auto& t = ctx.theme();

        if (modal)
            ctx.box(0, 0, sceneWidth, sceneHeight, t.overlayModal, 0);

        ctx.box(x + t.padMedium, y + t.padMedium, w, h, t.shadow, t.radiusLarge); // Shadow
        ctx.box(x, y, w, h, bgColor, t.radiusLarge);
        ctx.boxOutline(x, y, w, h, accentColor, t.radiusLarge);

        if (!label.empty()) {
            uint32_t hdrColor = headerColor ? headerColor : t.bgHeader;
            float hdrH = t.rowHeight + t.padMedium;
            ctx.box(x, y, w, hdrH, hdrColor, t.radiusLarge);
            ctx.text(label, x + t.padLarge, y + t.padLarge - 2, fgColor);
        }
    }

    void renderAll(RenderContext& ctx) override {
        render(ctx);
        if (isOpen()) {
            for (auto& child : children)
                child->renderAll(ctx);
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        flags ^= WIDGET_OPEN;
        if (onClick.empty()) return {};
        return WidgetEvent{id, "click", "open", isOpen() ? "true" : "false"};
    }
};

} // namespace yetty::ygui
