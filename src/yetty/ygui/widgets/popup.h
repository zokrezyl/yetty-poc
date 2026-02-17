#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>

namespace yetty::ygui {

class Popup : public Widget {
public:
    std::string label;
    bool modal = false;
    uint32_t headerColor = 0xFF3A3A4E;
    float sceneWidth = 0;  // set by engine for modal overlay
    float sceneHeight = 0;

    void render(RenderContext& ctx) override {
        if (!isOpen()) return;

        if (modal)
            ctx.box(0, 0, sceneWidth, sceneHeight, 0x80000000, 0);

        ctx.box(x + 4, y + 4, w, h, 0x40000000, 8); // Shadow
        ctx.box(x, y, w, h, bgColor, 8);
        ctx.boxOutline(x, y, w, h, accentColor, 8);

        if (!label.empty()) {
            ctx.box(x, y, w, 28, headerColor, 8);
            ctx.text(label, x + 8, y + 6, fgColor);
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
