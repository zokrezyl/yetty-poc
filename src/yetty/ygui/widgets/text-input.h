#pragma once

#include "../ygui-widgets.h"
#include "../ygui-io.h"
#include <string>

namespace yetty::ygui {

class TextInput : public Widget {
public:
    std::string text;
    uint32_t cursorPos = 0;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        ctx.box(x, y, w, h, bgColor, t.radiusMedium);
        ctx.text(text, x + t.padLarge, y + t.padMedium, fgColor);
        if (isFocused()) {
            float charW = ctx.measureText("M", t.fontSize);
            float cursorX = x + t.padLarge + cursorPos * charW;
            ctx.box(cursorX, y + t.padMedium, 2, h - t.padLarge, fgColor, 0);
            ctx.boxOutline(x, y, w, h, accentColor, t.radiusMedium);
        }
    }

    std::optional<WidgetEvent> onKey(uint32_t key, int mods) override {
        if (key == 8) { // Backspace
            if (cursorPos > 0 && !text.empty()) {
                text.erase(cursorPos - 1, 1);
                cursorPos--;
            }
        } else if (key == 127) { // Delete
            if (cursorPos < text.size()) {
                text.erase(cursorPos, 1);
            }
        } else if (key >= 32 && key < 127) {
            text.insert(cursorPos, 1, static_cast<char>(key));
            cursorPos++;
        }
        if (onChange.empty()) return {};
        return WidgetEvent{id, "change", "text", text};
    }
};

} // namespace yetty::ygui
