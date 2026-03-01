#pragma once

#include "../ygui-widgets.h"
#include <yetty/base/event-loop.h>
#include <algorithm>

namespace yetty::ygui {

class Label : public Widget {
public:
    std::string label;
    bool selectable = false;

    // Selection state
    int _selStart = -1;   // char index in label (-1 = no selection)
    int _selEnd = -1;
    bool _selecting = false;

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float fontSize = t.fontSize;
        float lineH = fontSize * 1.4f;
        float charW = fontSize * 0.6f;
        float curY = y;

        int selMin = std::min(_selStart, _selEnd);
        int selMax = std::max(_selStart, _selEnd);
        bool hasSel = (_selStart >= 0 && _selEnd >= 0 && selMin != selMax);

        // Split on newlines and render each line
        size_t start = 0;
        while (start <= label.size()) {
            size_t end = label.find('\n', start);
            if (end == std::string::npos) end = label.size();
            int lineStart = (int)start;
            int lineEnd = (int)end;

            // Draw selection highlight behind text
            if (hasSel && lineEnd > selMin && lineStart < selMax) {
                int hlStart = std::max(lineStart, selMin) - lineStart;
                int hlEnd = std::min(lineEnd, selMax) - lineStart;
                float hlX = x + hlStart * charW;
                float hlW = (hlEnd - hlStart) * charW;
                ctx.box(hlX, curY, hlW, lineH, t.selectionBg, 0);
            }

            if (end > start) {
                std::string line = label.substr(start, end - start);
                ctx.text(line, x, curY, fgColor, fontSize);
            }
            curY += lineH;
            start = end + 1;
        }
    }

    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        if (!selectable) return {};
        int idx = charIndexAt(localX, localY);
        _selStart = _selEnd = idx;
        _selecting = true;
        return WidgetEvent{id, "select", "", ""};
    }

    std::optional<WidgetEvent> onDrag(float localX, float localY) override {
        if (!selectable || !_selecting) return {};
        _selEnd = charIndexAt(localX, localY);
        return WidgetEvent{id, "select", "", ""};
    }

    std::optional<WidgetEvent> onRelease(float localX, float localY) override {
        if (!selectable || !_selecting) return {};
        _selecting = false;
        _selEnd = charIndexAt(localX, localY);

        int selMin = std::min(_selStart, _selEnd);
        int selMax = std::max(_selStart, _selEnd);
        if (selMin != selMax && selMin >= 0 && selMax <= (int)label.size()) {
            auto text = std::make_shared<std::string>(label.substr(selMin, selMax - selMin));
            if (auto loop = base::EventLoop::instance(); loop) {
                (*loop)->dispatch(base::Event::copyEvent(std::move(text)));
            }
        }
        return WidgetEvent{id, "select", "", ""};
    }

private:
    int charIndexAt(float localX, float localY) const {
        auto& t = defaultTheme();
        float fontSize = t.fontSize;
        float lineH = fontSize * 1.4f;
        float charW = fontSize * 0.6f;

        int lineIdx = std::max(0, (int)(localY / lineH));

        // Walk newlines to find the target line
        int curLine = 0;
        size_t start = 0;
        while (start <= label.size()) {
            size_t end = label.find('\n', start);
            if (end == std::string::npos) end = label.size();

            if (curLine == lineIdx) {
                int lineLen = (int)(end - start);
                int charIdx = std::clamp((int)(localX / charW), 0, lineLen);
                return (int)start + charIdx;
            }
            curLine++;
            start = end + 1;
        }
        return (int)label.size();
    }
};

} // namespace yetty::ygui
