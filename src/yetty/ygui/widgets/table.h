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
        if (!label.empty())
            ctx.text(label, x, y, fgColor);
    }
};

class Table : public Widget {
public:
    std::vector<float> columnWidths;
    float rowHeight = 0;       // 0 = use theme
    uint32_t headerColor = 0;  // 0 = use theme

    void render(RenderContext& ctx) override {
        auto& t = ctx.theme();
        float rh = rowHeight > 0 ? rowHeight : t.rowHeight;
        uint32_t hdrColor = headerColor ? headerColor : t.bgHeader;

        ctx.box(x, y, w, h, bgColor, t.radiusSmall);
        ctx.boxOutline(x, y, w, h, t.borderLight, t.radiusSmall);

        float cy = y;
        for (auto& child : children) {
            float childH = child->h > 0 ? child->h : rh;
            bool isHeader = dynamic_cast<TableHeader*>(child.get()) != nullptr;

            if (isHeader)
                ctx.box(x, cy, w, childH, hdrColor, 0);
            else if (child->isHover())
                ctx.box(x, cy, w, childH, t.bgHover, 0);

            uint32_t borderColor = isHeader ? t.borderLight : t.border;
            ctx.box(x, cy + childH - t.separatorSize, w, t.separatorSize, borderColor, 0);

            // Render cells
            float cx = x;
            size_t colIdx = 0;
            for (auto& cell : child->children) {
                float colW = (colIdx < columnWidths.size()) ? columnWidths[colIdx] : 80;
                if (colIdx > 0)
                    ctx.box(cx, cy, t.separatorSize, childH, borderColor, 0);
                auto* tc = dynamic_cast<TableCell*>(cell.get());
                std::string cellText = (tc && !tc->label.empty()) ? tc->label : cell->id;
                if (!cellText.empty()) {
                    uint32_t textColor = isHeader ? t.textPrimary : fgColor;
                    ctx.text(cellText, cx + t.padLarge - 2, cy + t.padMedium, textColor);
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
