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
        ctx.box(x, y, w, h, bgColor, 4);
        ctx.text(text, x + 8, y + 4, fgColor);
        if (isFocused()) {
            float cursorX = x + 8 + cursorPos * 8;
            ctx.box(cursorX, y + 4, 2, h - 8, fgColor, 0);
            ctx.boxOutline(x, y, w, h, accentColor, 4);
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
