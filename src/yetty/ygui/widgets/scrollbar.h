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
        float trackW = w > 0 ? w : 12;
        ctx.box(x, y, trackW, h, 0xFF222233, trackW / 2);

        float thumbH = std::max(20.0f, h * 0.2f);
        float trackRange = h - thumbH;
        float thumbY = y + value * trackRange;
        uint32_t thumbColor = isPressed() ? accentColor
                            : (isHover() ? 0xFF555566 : 0xFF444455);
        ctx.box(x + 2, thumbY, trackW - 4, thumbH, thumbColor, (trackW - 4) / 2);
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
        float trackH = h > 0 ? h : 12;
        ctx.box(x, y, w, trackH, 0xFF222233, trackH / 2);

        float thumbW = std::max(20.0f, w * 0.2f);
        float trackRange = w - thumbW;
        float thumbX = x + value * trackRange;
        uint32_t thumbColor = isPressed() ? accentColor
                            : (isHover() ? 0xFF555566 : 0xFF444455);
        ctx.box(thumbX, y + 2, thumbW, trackH - 4, thumbColor, (trackH - 4) / 2);
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
