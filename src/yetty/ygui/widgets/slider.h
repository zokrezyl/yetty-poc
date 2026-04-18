#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <algorithm>
#include <string>

namespace yetty::ygui {

class Slider : public Widget {
public:
    float value = 0;
    float minValue = 0;
    float maxValue = 1;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float trackH = t.padMedium;
        float trackY = y + (h - trackH) / 2;
        ctx.box(x, trackY, w, trackH, bgColor, t.radiusSmall);

        float pct = (value - minValue) / (maxValue - minValue);
        float fillW = pct * w;
        ctx.box(x, trackY, fillW, trackH, accentColor, t.radiusSmall);

        float handleW = t.scrollbarSize;
        float handleX = x + fillW - handleW / 2;
        ctx.box(handleX, y, handleW, h, accentColor, handleW / 2);
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

    std::optional<WidgetEvent> onScroll(float dx, float dy) override {
        float range = maxValue - minValue;
        float delta = dy * range * 0.05f;
        value = std::clamp(value + delta, minValue, maxValue);
        if (onChange.empty()) return {};
        return WidgetEvent{id, "change", "value", std::to_string(value)};
    }

private:
    void updateValue(float localX) {
        float pct = std::clamp(localX / w, 0.0f, 1.0f);
        value = minValue + pct * (maxValue - minValue);
    }
};

} // namespace yetty::ygui
