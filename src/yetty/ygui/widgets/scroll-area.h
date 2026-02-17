#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <algorithm>

namespace yetty::ygui {

class ScrollArea : public Widget {
public:
    float scrollX = 0, scrollY = 0;
    float contentW = 0, contentH = 0;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, bgColor, 4);

        float scrollbarW = 12, scrollbarH = 12;
        float viewW = w - scrollbarW;
        float viewH = h - scrollbarH;
        float cW = contentW > 0 ? contentW : viewW;
        float cH = contentH > 0 ? contentH : viewH;
        float maxScrollX = std::max(0.0f, cW - viewW);
        float maxScrollY = std::max(0.0f, cH - viewH);

        // Vertical scrollbar
        if (cH > viewH) {
            float trackX = x + w - scrollbarW;
            float trackH = h - scrollbarH;
            ctx.box(trackX, y, scrollbarW, trackH, 0xFF222233, scrollbarW / 2);
            float thumbH = std::max(20.0f, trackH * viewH / cH);
            float thumbRange = trackH - thumbH;
            float thumbY = y + (maxScrollY > 0 ? (scrollY / maxScrollY) * thumbRange : 0);
            ctx.box(trackX + 2, thumbY, scrollbarW - 4, thumbH, 0xFF444455, (scrollbarW - 4) / 2);
        }

        // Horizontal scrollbar
        if (cW > viewW) {
            float trackY = y + h - scrollbarH;
            float trackW = w - scrollbarW;
            ctx.box(x, trackY, trackW, scrollbarH, 0xFF222233, scrollbarH / 2);
            float thumbW = std::max(20.0f, trackW * viewW / cW);
            float thumbRange = trackW - thumbW;
            float thumbX = x + (maxScrollX > 0 ? (scrollX / maxScrollX) * thumbRange : 0);
            ctx.box(thumbX, trackY + 2, thumbW, scrollbarH - 4, 0xFF444455, (scrollbarH - 4) / 2);
        }

        // Corner fill
        if (cW > viewW && cH > viewH) {
            ctx.box(x + w - scrollbarW, y + h - scrollbarH, scrollbarW, scrollbarH, 0xFF1A1A2E, 0);
        }
    }

    void renderAll(RenderContext& ctx) override {
        effectiveX = x + ctx.offsetX();
        effectiveY = y + ctx.offsetY();
        wasRendered = true;
        render(ctx);
        ctx.pushOffset(x - scrollX, y - scrollY);
        for (auto& child : children)
            child->renderAll(ctx);
        ctx.popOffset();
    }

    std::optional<WidgetEvent> onScroll(float dx, float dy) override {
        float scrollbarW = 12, scrollbarH = 12;
        float viewW = w - scrollbarW;
        float viewH = h - scrollbarH;
        float cW = contentW > 0 ? contentW : viewW;
        float cH = contentH > 0 ? contentH : viewH;
        float maxScrollX = std::max(0.0f, cW - viewW);
        float maxScrollY = std::max(0.0f, cH - viewH);

        scrollY = std::clamp(scrollY - dy * 20.0f, 0.0f, maxScrollY);
        scrollX = std::clamp(scrollX - dx * 20.0f, 0.0f, maxScrollX);

        if (onChange.empty()) return {};
        return WidgetEvent{id, "scroll", "scrollY", std::to_string(scrollY)};
    }
};

} // namespace yetty::ygui
