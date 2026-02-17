#pragma once

#include "../ygui-widgets.h"
#include <string>
#include <vector>

namespace yetty::ygui {

class TableHeader : public Widget {
public:
    bool isTableHeader = true;
    void render(RenderContext& ctx) override {
        // Rendered inline by Table parent
    }
};

class TableRow : public Widget {
public:
    std::optional<WidgetEvent> onPress(float localX, float localY) override {
        flags ^= WIDGET_CHECKED;
        if (onClick.empty()) return {};
        return WidgetEvent{id, "click", "checked", isChecked() ? "true" : "false"};
    }
};

class TableCell : public Widget {
public:
    std::string label;

    void render(RenderContext& ctx) override {
        // Standalone use
        if (!label.empty())
            ctx.text(label, x, y, fgColor);
    }
};

class Table : public Widget {
public:
    std::vector<float> columnWidths;
    float rowHeight = 24;
    uint32_t headerColor = 0xFF3A3A4E;

    void render(RenderContext& ctx) override {
        ctx.box(x, y, w, h, bgColor, 2);
        ctx.boxOutline(x, y, w, h, 0xFF555566, 2);

        float cy = y;
        for (auto& child : children) {
            float childH = child->h > 0 ? child->h : rowHeight;
            bool isHeader = dynamic_cast<TableHeader*>(child.get()) != nullptr;

            if (isHeader)
                ctx.box(x, cy, w, childH, headerColor, 0);
            else if (child->isHover())
                ctx.box(x, cy, w, childH, 0xFF333344, 0);

            uint32_t borderColor = isHeader ? 0xFF555566 : 0xFF444455;
            ctx.box(x, cy + childH - 1, w, 1, borderColor, 0);

            // Render cells
            float cx = x;
            size_t colIdx = 0;
            for (auto& cell : child->children) {
                float colW = (colIdx < columnWidths.size()) ? columnWidths[colIdx] : 80;
                if (colIdx > 0)
                    ctx.box(cx, cy, 1, childH, borderColor, 0);
                auto* tc = dynamic_cast<TableCell*>(cell.get());
                std::string cellText = (tc && !tc->label.empty()) ? tc->label : cell->id;
                if (!cellText.empty()) {
                    uint32_t textColor = isHeader ? 0xFFFFFFFF : fgColor;
                    ctx.text(cellText, cx + 6, cy + 4, textColor);
                }
                cx += colW;
                colIdx++;
            }
            cy += childH;
        }
    }

    // Children are rendered inline by render() — don't recurse
    void renderAll(RenderContext& ctx) override {
        render(ctx);
    }
};

} // namespace yetty::ygui
