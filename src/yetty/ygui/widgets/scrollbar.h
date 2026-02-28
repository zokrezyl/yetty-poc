#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <algorithm>
#include <string>

namespace yetty::ygui {

class VScrollbar : public Widget {
public:
    float value = 0; // 0..1

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float trackW = w > 0 ? w : t.scrollbarSize;
        ctx.box(x, y, trackW, h, t.bgSecondary, trackW / 2);

        float thumbH = std::max(20.0f, h * 0.2f);
        float trackRange = h - thumbH;
        float thumbY = y + value * trackRange;
        uint32_t thumbColor = isPressed() ? accentColor
                            : (isHover() ? t.thumbHover : t.thumbNormal);
        ctx.box(x + t.padSmall, thumbY, trackW - t.padMedium, thumbH,
                thumbColor, (trackW - t.padMedium) / 2);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        updateValue(localY);
        if (onChange.empty()) return {};
        return WidgetEvent{id, "change", "value", std::to_string(value)};
    }

    std::optional<WidgetEvent> onDrag(float localX, float localY) override {
        updateValue(localY);
        if (onChange.empty()) return {};
        return WidgetEvent{id, "change", "value", std::to_string(value)};
    }

private:
    void updateValue(float localY) {
        float thumbH = std::max(20.0f, h * 0.2f);
        float trackRange = h - thumbH;
        if (trackRange <= 0) return;
        float pct = (localY - thumbH / 2) / trackRange;
        value = std::clamp(pct, 0.0f, 1.0f);
    }
};

class HScrollbar : public Widget {
public:
    float value = 0; // 0..1

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float trackH = h > 0 ? h : t.scrollbarSize;
        ctx.box(x, y, w, trackH, t.bgSecondary, trackH / 2);

        float thumbW = std::max(20.0f, w * 0.2f);
        float trackRange = w - thumbW;
        float thumbX = x + value * trackRange;
        uint32_t thumbColor = isPressed() ? accentColor
                            : (isHover() ? t.thumbHover : t.thumbNormal);
        ctx.box(thumbX, y + t.padSmall, thumbW, trackH - t.padMedium,
                thumbColor, (trackH - t.padMedium) / 2);
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        updateValue(localX);
        if (onChange.empty()) return {};
        return WidgetEvent{id, "change", "value", std::to_string(value)};
    }

    std::optional<WidgetEvent> onDrag(float localX, float localY) override {
        updateValue(localX);
        if (onChange.empty()) return {};
        return WidgetEvent{id, "change", "value", std::to_string(value)};
    }

private:
    void updateValue(float localX) {
        float thumbW = std::max(20.0f, w * 0.2f);
        float trackRange = w - thumbW;
        if (trackRange <= 0) return;
        float pct = (localX - thumbW / 2) / trackRange;
        value = std::clamp(pct, 0.0f, 1.0f);
    }
};

} // namespace yetty::ygui
