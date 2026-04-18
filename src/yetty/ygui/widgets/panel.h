#pragma once

#include "../ygui-widgets.h"
#include <algorithm>

namespace yetty::ygui {

/// Panel — root window widget with built-in scrolling.
/// Children whose y < headerH render fixed (no scroll).
/// Children whose y >= headerH are clipped and scrolled.
class Panel : public Widget {
public:
    float cornerRadius = 8;
    float scrollX = 0, scrollY = 0;
    float contentW = 0, contentH = 0;
    float headerH = 0;  // fixed header height (children with y < headerH don't scroll)
    bool _draggingScrollbar = false;
    float _dragStartY = 0;
    float _dragStartScrollY = 0;
    bool _draggingWindow = false;
    float _dragAbsX = 0, _dragAbsY = 0;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, bgColor, cornerRadius);

        float scrollableH = h - headerH;
        float sbW = t.scrollbarSize;
        if (contentH > scrollableH && scrollableH > 0) {
            float maxSY = contentH - scrollableH;
            float trackX = x + w - sbW;
            float trackY = y + headerH;
            float trackH = scrollableH;
            ctx.box(trackX, trackY, sbW, trackH, t.bgSecondary, sbW / 2);
            float thumbH = std::max(20.0f, trackH * scrollableH / contentH);
            float thumbRange = trackH - thumbH;
            float thumbY = trackY + (maxSY > 0 ? (scrollY / maxSY) * thumbRange : 0);
            ctx.box(trackX + t.padSmall, thumbY, sbW - t.padMedium, thumbH,
                    t.thumbNormal, (sbW - t.padMedium) / 2);
        }
    }

    void renderAll(RenderContext& ctx) override {
        effectiveX = x + ctx.offsetX();
        effectiveY = y + ctx.offsetY();
        wasRendered = true;
        render(ctx);

        auto& t = ctx.theme();
        float scrollableH = h - headerH;
        float sbW = (contentH > scrollableH && scrollableH > 0) ? t.scrollbarSize : 0;

        // Render fixed header children (y < headerH)
        {
            RenderContext::ClipGuard clip(ctx, x, y, w, headerH > 0 ? headerH : h);
            RenderContext::OffsetGuard guard(ctx, x, y);
            for (auto& child : children) {
                if (child->y < headerH)
                    child->renderAll(ctx);
            }
        }

        // Render scrollable content children (y >= headerH)
        {
            RenderContext::ClipGuard clip(ctx, x, y + headerH, w - sbW, scrollableH);
            RenderContext::OffsetGuard guard(ctx, x - scrollX, y - scrollY);
            for (auto& child : children) {
                if (child->y >= headerH)
                    child->renderAll(ctx);
            }
        }
    }

    std::optional<WidgetEvent> onScroll(float dx, float dy) override {
        float scrollableH = h - headerH;
        if (contentH <= scrollableH && contentW <= w) return {};
        auto& t = defaultTheme();
        float maxScrollY = std::max(0.0f, contentH - scrollableH);
        float maxScrollX = std::max(0.0f, contentW - w);
        scrollY = std::clamp(scrollY - dy * t.scrollSpeed, 0.0f, maxScrollY);
        scrollX = std::clamp(scrollX - dx * t.scrollSpeed, 0.0f, maxScrollX);
        return WidgetEvent{id, "scroll", "", ""};
    }

    std::optional<WidgetEvent> onPress(float px, float py) override {
        float scrollableH = h - headerH;
        auto& t = defaultTheme();
        float sbW = t.scrollbarSize;
        // Scrollbar click
        if (contentH > scrollableH && scrollableH > 0 && px >= w - sbW && py >= headerH) {
            _draggingScrollbar = true;
            float maxSY = contentH - scrollableH;
            float thumbH = std::max(20.0f, scrollableH * scrollableH / contentH);
            float thumbRange = scrollableH - thumbH;
            float clickInTrack = py - headerH;
            float ratio = std::clamp((clickInTrack - thumbH / 2) / thumbRange, 0.0f, 1.0f);
            scrollY = ratio * maxSY;
            _dragStartY = py;
            _dragStartScrollY = scrollY;
            return WidgetEvent{id, "scroll", "", ""};
        }
        // Header drag → move window
        if (headerH > 0 && py < headerH) {
            _draggingWindow = true;
            _dragAbsX = effectiveX + px;
            _dragAbsY = effectiveY + py;
            return WidgetEvent{id, "move", "", ""};
        }
        return {};
    }

    std::optional<WidgetEvent> onDrag(float px, float py) override {
        if (_draggingScrollbar) {
            float scrollableH = h - headerH;
            float maxSY = std::max(0.0f, contentH - scrollableH);
            float thumbH = std::max(20.0f, scrollableH * scrollableH / contentH);
            float thumbRange = scrollableH - thumbH;
            if (thumbRange > 0) {
                float dy = py - _dragStartY;
                scrollY = std::clamp(_dragStartScrollY + dy * maxSY / thumbRange, 0.0f, maxSY);
            }
            return WidgetEvent{id, "scroll", "", ""};
        }
        if (_draggingWindow) {
            float absX = effectiveX + px;
            float absY = effectiveY + py;
            x += absX - _dragAbsX;
            y += absY - _dragAbsY;
            _dragAbsX = absX;
            _dragAbsY = absY;
            return WidgetEvent{id, "move", "", ""};
        }
        return {};
    }

    std::optional<WidgetEvent> onRelease(float px, float py) override {
        _draggingScrollbar = false;
        _draggingWindow = false;
        return {};
    }
};

} // namespace yetty::ygui
