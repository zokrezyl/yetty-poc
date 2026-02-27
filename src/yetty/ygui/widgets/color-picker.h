#pragma once

// Prevent Windows min/max macros from conflicting with std::min/std::max
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <algorithm>
#include <cmath>
#include <string>

namespace yetty::ygui {

class ColorPicker : public Widget {
public:
    std::string label;
    uint32_t colorValue = 0xFFFFFFFF;

    void render(RenderContext& ctx) override {
        if (w >= 80 && h >= 80) {
            // Full color wheel
            uint8_t r = colorValue & 0xFF;
            uint8_t g = (colorValue >> 8) & 0xFF;
            uint8_t b = (colorValue >> 16) & 0xFF;
            float rf = r / 255.0f, gf = g / 255.0f, bf = b / 255.0f;
            float cmax = std::max({rf, gf, bf});
            float cmin = std::min({rf, gf, bf});
            float delta = cmax - cmin;
            float hue = 0, sat = 0, val = cmax;
            if (delta > 0) {
                sat = delta / cmax;
                if (cmax == rf)
                    hue = std::fmod((gf - bf) / delta + 6.0f, 6.0f) / 6.0f;
                else if (cmax == gf)
                    hue = ((bf - rf) / delta + 2.0f) / 6.0f;
                else
                    hue = ((rf - gf) / delta + 4.0f) / 6.0f;
            }
            float radius = std::min(w, h) / 2.0f - 4.0f;
            ctx.colorWheel(x + w / 2.0f, y + h / 2.0f,
                           radius, radius * 0.7f, hue, sat, val, 6.0f);
        } else {
            // Small preview box
            float boxSize = 20;
            ctx.box(x, y + 2, boxSize, boxSize, colorValue, 2);
            ctx.boxOutline(x, y + 2, boxSize, boxSize, 0xFF666666, 2);
            if (!label.empty())
                ctx.text(label, x + boxSize + 8, y + 4, fgColor);
            if (isHover())
                ctx.boxOutline(x, y + 2, boxSize, boxSize, accentColor, 2);
        }
    }
};

} // namespace yetty::ygui
