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
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, bgColor, t.radiusMedium);

        float sbW = t.scrollbarSize, sbH = t.scrollbarSize;
        float viewW = w - sbW;
        float viewH = h - sbH;
        float cW = contentW > 0 ? contentW : viewW;
        float cH = contentH > 0 ? contentH : viewH;
        float maxScrollX = std::max(0.0f, cW - viewW);
        float maxScrollY = std::max(0.0f, cH - viewH);

        // Vertical scrollbar
        if (cH > viewH) {
            float trackX = x + w - sbW;
            float trackH = h - sbH;
            ctx.box(trackX, y, sbW, trackH, t.bgSecondary, sbW / 2);
            float thumbH = std::max(20.0f, trackH * viewH / cH);
            float thumbRange = trackH - thumbH;
            float thumbY = y + (maxScrollY > 0 ? (scrollY / maxScrollY) * thumbRange : 0);
            ctx.box(trackX + t.padSmall, thumbY, sbW - t.padMedium, thumbH,
                    t.thumbNormal, (sbW - t.padMedium) / 2);
        }

        // Horizontal scrollbar
        if (cW > viewW) {
            float trackY = y + h - sbH;
            float trackW = w - sbW;
            ctx.box(x, trackY, trackW, sbH, t.bgSecondary, sbH / 2);
            float thumbW = std::max(20.0f, trackW * viewW / cW);
            float thumbRange = trackW - thumbW;
            float thumbX = x + (maxScrollX > 0 ? (scrollX / maxScrollX) * thumbRange : 0);
            ctx.box(thumbX, trackY + t.padSmall, thumbW, sbH - t.padMedium,
                    t.thumbNormal, (sbH - t.padMedium) / 2);
        }

        // Corner fill
        if (cW > viewW && cH > viewH) {
            ctx.box(x + w - sbW, y + h - sbH, sbW, sbH, t.bgPrimary, 0);
        }
    }

    void renderAll(RenderContext& ctx) override {
        effectiveX = x + ctx.offsetX();
        effectiveY = y + ctx.offsetY();
        wasRendered = true;
        render(ctx);
        auto& t = ctx.theme();
        float sbW = t.scrollbarSize, sbH = t.scrollbarSize;
        RenderContext::ClipGuard clip(ctx, x, y, w - sbW, h - sbH);
        RenderContext::OffsetGuard guard(ctx, x - scrollX, y - scrollY);
        for (auto& child : children)
            child->renderAll(ctx);
    }

    std::optional<WidgetEvent> onScroll(float dx, float dy) override {
        auto& t = defaultTheme();
        float sbW = t.scrollbarSize, sbH = t.scrollbarSize;
        float viewW = w - sbW;
        float viewH = h - sbH;
        float cW = contentW > 0 ? contentW : viewW;
        float cH = contentH > 0 ? contentH : viewH;
        float maxScrollX = std::max(0.0f, cW - viewW);
        float maxScrollY = std::max(0.0f, cH - viewH);

        scrollY = std::clamp(scrollY - dy * t.scrollSpeed, 0.0f, maxScrollY);
        scrollX = std::clamp(scrollX - dx * t.scrollSpeed, 0.0f, maxScrollX);

        if (onChange.empty()) return {};
        return WidgetEvent{id, "scroll", "scrollY", std::to_string(scrollY)};
    }
};

} // namespace yetty::ygui
