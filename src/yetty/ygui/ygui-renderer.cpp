#include "ygui-renderer.h"
#include <yetty/ydraw-writer.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cmath>

namespace yetty::ygui {

YGuiRenderer::YGuiRenderer(std::shared_ptr<YDrawWriter> writer)
    : _writer(std::move(writer)) {}

void YGuiRenderer::setSceneBounds(float width, float height) {
    _pixelWidth = width;
    _pixelHeight = height;
    if (_writer) {
        _writer->setSceneBounds(0, 0, width, height);
    }
}

void YGuiRenderer::setBgColor(uint32_t color) {
    if (_writer) {
        _writer->setBgColor(color);
    }
}

void YGuiRenderer::clear() {
    if (_writer) {
        _writer->clear();
    }
}

void YGuiRenderer::render(const std::vector<WidgetPtr>& widgets,
                          const YGuiRenderState& state) {
    if (!_writer) return;

    _writer->clear();
    _writer->setSceneBounds(0, 0, _pixelWidth, _pixelHeight);
    _writer->setBgColor(0xFF1A1A2E);

    float offsetY = 0;
    for (const auto& w : widgets) {
        renderWidget(w.get(), 0, offsetY, state);
    }

    // Render open dropdown overlay last (on top)
    if (state.openDropdown && state.openDropdown->isOpen()) {
        renderDropdownOptions(state.openDropdown, state);
    }
}

void YGuiRenderer::renderWidget(Widget* w, float offsetX, float offsetY,
                                 const YGuiRenderState& state) {
    float x = w->x + offsetX;
    float y = w->y + offsetY;

    switch (w->type) {
    case WidgetType::Label:
        addText(w->label, x, y, w->fgColor);
        break;

    case WidgetType::Button:
        addBox(x, y, w->w, w->h, w->isPressed() ? w->accentColor : w->bgColor, 4);
        addText(w->label, x + 8, y + 4, w->fgColor);
        if (w->isHover())
            addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
        break;

    case WidgetType::Checkbox: {
        float boxSize = 16;
        addBox(x, y + 2, boxSize, boxSize, w->bgColor, 2);
        if (w->isChecked()) {
            addBox(x + 3, y + 5, boxSize - 6, boxSize - 6, w->accentColor, 2);
        }
        addText(w->label, x + boxSize + 8, y + 2, w->fgColor);
        if (w->isHover())
            addBoxOutline(x, y + 2, boxSize, boxSize, w->accentColor, 2);
        break;
    }

    case WidgetType::Slider: {
        float trackH = 4;
        float trackY = y + (w->h - trackH) / 2;
        addBox(x, trackY, w->w, trackH, w->bgColor, 2);

        float pct = (w->value - w->minValue) / (w->maxValue - w->minValue);
        float fillW = pct * w->w;
        addBox(x, trackY, fillW, trackH, w->accentColor, 2);

        float handleW = 12;
        float handleX = x + fillW - handleW / 2;
        addBox(handleX, y, handleW, w->h, w->accentColor, 6);
        break;
    }

    case WidgetType::Dropdown: {
        addBox(x, y, w->w, w->h, w->bgColor, 4);
        std::string text = w->selectedIndex < static_cast<int>(w->options.size())
                               ? w->options[w->selectedIndex]
                               : "";
        addText(text, x + 8, y + 4, w->fgColor);
        float arrowSize = 8;
        float arrowX = x + w->w - 16 - arrowSize / 2;
        float arrowY = y + w->h / 2;
        if (w->isOpen()) {
            addTriangle(arrowX, arrowY + arrowSize / 3, arrowX + arrowSize,
                        arrowY + arrowSize / 3, arrowX + arrowSize / 2,
                        arrowY - arrowSize / 3, w->fgColor);
        } else {
            addTriangle(arrowX, arrowY - arrowSize / 3, arrowX + arrowSize,
                        arrowY - arrowSize / 3, arrowX + arrowSize / 2,
                        arrowY + arrowSize / 3, w->fgColor);
        }
        if (w->isHover())
            addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
        break;
    }

    case WidgetType::TextInput: {
        addBox(x, y, w->w, w->h, w->bgColor, 4);
        addText(w->text, x + 8, y + 4, w->fgColor);
        if (w->isFocused()) {
            float cursorX = x + 8 + w->cursorPos * 8;
            addBox(cursorX, y + 4, 2, w->h - 8, w->fgColor, 0);
        }
        if (w->isFocused())
            addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
        break;
    }

    case WidgetType::Progress: {
        addBox(x, y, w->w, w->h, w->bgColor, 4);
        float pct = (w->value - w->minValue) / (w->maxValue - w->minValue);
        addBox(x + 2, y + 2, (w->w - 4) * pct, w->h - 4, w->accentColor, 2);
        break;
    }

    case WidgetType::Panel:
        addBox(x, y, w->w, w->h, w->bgColor, 8);
        for (auto& child : w->children) {
            renderWidget(child.get(), x, y, state);
        }
        break;

    case WidgetType::VBox: {
        float cy = y;
        for (auto& child : w->children) {
            child->x = x;
            child->y = cy;
            renderWidget(child.get(), 0, 0, state);
            cy += child->h + 4;
        }
        break;
    }

    case WidgetType::HBox: {
        float cx = x;
        for (auto& child : w->children) {
            child->x = cx;
            child->y = y;
            renderWidget(child.get(), 0, 0, state);
            cx += child->w + 4;
        }
        break;
    }

    case WidgetType::ChoiceBox: {
        float optH = 24;
        float cy = y;
        for (size_t i = 0; i < w->options.size(); i++) {
            bool isSelected = (static_cast<int>(i) == w->selectedIndex);
            bool isHovered =
                (state.hoverChoiceWidget == w && static_cast<int>(i) == state.hoverChoiceIdx);
            float radioX = x;
            float radioY = cy + 4;
            float radioSize = 14;
            float centerX = radioX + radioSize / 2;
            float centerY = radioY + radioSize / 2;

            addCircleOutline(centerX, centerY, radioSize / 2,
                             isHovered ? w->accentColor : 0xFF666677);
            if (isSelected) {
                addCircle(centerX, centerY, radioSize / 4, w->accentColor);
            }
            if (isHovered && !isSelected) {
                addCircle(centerX, centerY, radioSize / 6, 0xFF555566);
            }
            addText(w->options[i], x + radioSize + 8, cy + 4, w->fgColor);
            cy += optH;
        }
        break;
    }

    case WidgetType::Separator: {
        float lineY = y + w->h / 2;
        addBox(x, lineY - 1, w->w, 2, 0xFF444455, 0);
        break;
    }

    case WidgetType::ColorPicker: {
        if (w->w >= 80 && w->h >= 80) {
            uint8_t r = w->colorValue & 0xFF;
            uint8_t g = (w->colorValue >> 8) & 0xFF;
            uint8_t b = (w->colorValue >> 16) & 0xFF;
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
            float radius = std::min(w->w, w->h) / 2.0f - 4.0f;
            float cx = x + w->w / 2.0f;
            float cy = y + w->h / 2.0f;
            _writer->addColorWheel(cx, cy, radius, radius * 0.7f, hue, sat, val, 6.0f);
        } else {
            float boxSize = 20;
            addBox(x, y + 2, boxSize, boxSize, w->colorValue, 2);
            addBoxOutline(x, y + 2, boxSize, boxSize, 0xFF666666, 2);
            if (!w->label.empty()) {
                addText(w->label, x + boxSize + 8, y + 4, w->fgColor);
            }
            if (w->isHover())
                addBoxOutline(x, y + 2, boxSize, boxSize, w->accentColor, 2);
        }
        break;
    }

    case WidgetType::Selectable: {
        if (w->isChecked()) {
            addBox(x, y, w->w, w->h, w->accentColor, 2);
        } else if (w->isHover()) {
            addBox(x, y, w->w, w->h, 0xFF333344, 2);
        }
        addText(w->label, x + 8, y + 4, w->fgColor);
        break;
    }

    case WidgetType::CollapsingHeader: {
        addBox(x, y, w->w, w->h, w->bgColor, 4);
        float arrowSize = 8;
        float arrowX = x + 10;
        float arrowY = y + w->h / 2;
        if (w->isOpen()) {
            addTriangle(arrowX, arrowY - arrowSize / 3, arrowX + arrowSize,
                        arrowY - arrowSize / 3, arrowX + arrowSize / 2,
                        arrowY + arrowSize / 3, w->fgColor);
        } else {
            addTriangle(arrowX, arrowY - arrowSize / 2, arrowX,
                        arrowY + arrowSize / 2, arrowX + arrowSize * 0.7f, arrowY,
                        w->fgColor);
        }
        addText(w->label, x + 26, y + 4, w->fgColor);
        if (w->isHover())
            addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);

        if (w->isOpen()) {
            float cy = y + w->h + 4;
            for (auto& child : w->children) {
                child->x = x + 16;
                child->y = cy;
                renderWidget(child.get(), 0, 0, state);
                cy += child->h + 4;
            }
        }
        break;
    }

    case WidgetType::VScrollbar: {
        float trackW = w->w > 0 ? w->w : 12;
        addBox(x, y, trackW, w->h, 0xFF222233, trackW / 2);
        float thumbH = std::max(20.0f, w->h * 0.2f);
        float trackRange = w->h - thumbH;
        float thumbY = y + w->value * trackRange;
        uint32_t thumbColor = w->isPressed()
                                  ? w->accentColor
                                  : (w->isHover() ? 0xFF555566 : 0xFF444455);
        addBox(x + 2, thumbY, trackW - 4, thumbH, thumbColor, (trackW - 4) / 2);
        break;
    }

    case WidgetType::HScrollbar: {
        float trackH = w->h > 0 ? w->h : 12;
        addBox(x, y, w->w, trackH, 0xFF222233, trackH / 2);
        float thumbW = std::max(20.0f, w->w * 0.2f);
        float trackRange = w->w - thumbW;
        float thumbX = x + w->value * trackRange;
        uint32_t thumbColor = w->isPressed()
                                  ? w->accentColor
                                  : (w->isHover() ? 0xFF555566 : 0xFF444455);
        addBox(thumbX, y + 2, thumbW, trackH - 4, thumbColor, (trackH - 4) / 2);
        break;
    }

    case WidgetType::ScrollArea: {
        addBox(x, y, w->w, w->h, w->bgColor, 4);

        float scrollbarW = 12;
        float scrollbarH = 12;
        float viewW = w->w - scrollbarW;
        float viewH = w->h - scrollbarH;
        float contentW = w->contentW > 0 ? w->contentW : viewW;
        float contentH = w->contentH > 0 ? w->contentH : viewH;
        float maxScrollX = std::max(0.0f, contentW - viewW);
        float maxScrollY = std::max(0.0f, contentH - viewH);

        for (auto& child : w->children) {
            float childX = x + child->x - w->scrollX;
            float childY = y + child->y - w->scrollY;
            if (childX + child->w > x && childX < x + viewW &&
                childY + child->h > y && childY < y + viewH) {
                renderWidget(child.get(), x - w->scrollX, y - w->scrollY, state);
            }
        }

        if (contentH > viewH) {
            float trackX = x + w->w - scrollbarW;
            float trackH = w->h - scrollbarH;
            addBox(trackX, y, scrollbarW, trackH, 0xFF222233, scrollbarW / 2);
            float thumbH = std::max(20.0f, trackH * viewH / contentH);
            float thumbRange = trackH - thumbH;
            float thumbY =
                y + (maxScrollY > 0 ? (w->scrollY / maxScrollY) * thumbRange : 0);
            addBox(trackX + 2, thumbY, scrollbarW - 4, thumbH, 0xFF444455,
                   (scrollbarW - 4) / 2);
        }

        if (contentW > viewW) {
            float trackY = y + w->h - scrollbarH;
            float trackW = w->w - scrollbarW;
            addBox(x, trackY, trackW, scrollbarH, 0xFF222233, scrollbarH / 2);
            float thumbW = std::max(20.0f, trackW * viewW / contentW);
            float thumbRange = trackW - thumbW;
            float thumbX =
                x + (maxScrollX > 0 ? (w->scrollX / maxScrollX) * thumbRange : 0);
            addBox(thumbX, trackY + 2, thumbW, scrollbarH - 4, 0xFF444455,
                   (scrollbarH - 4) / 2);
        }

        if (contentW > viewW && contentH > viewH) {
            addBox(x + w->w - scrollbarW, y + w->h - scrollbarH, scrollbarW,
                   scrollbarH, 0xFF1A1A2E, 0);
        }
        break;
    }

    case WidgetType::Table: {
        addBox(x, y, w->w, w->h, w->bgColor, 2);
        addBoxOutline(x, y, w->w, w->h, 0xFF555566, 2);

        float rowH = w->rowHeight > 0 ? w->rowHeight : 24;
        float cy = y;
        for (auto& child : w->children) {
            float childH = child->h > 0 ? child->h : rowH;
            bool isHeader = (child->type == WidgetType::TableHeader);

            if (isHeader) {
                addBox(x, cy, w->w, childH, w->headerColor, 0);
            } else if (child->isHover()) {
                addBox(x, cy, w->w, childH, 0xFF333344, 0);
            }

            uint32_t borderColor = isHeader ? 0xFF555566 : 0xFF444455;
            addBox(x, cy + childH - 1, w->w, 1, borderColor, 0);

            float cx = x;
            size_t colIdx = 0;
            for (auto& cell : child->children) {
                float colW =
                    (colIdx < w->columnWidths.size()) ? w->columnWidths[colIdx] : 80;
                if (colIdx > 0) {
                    addBox(cx, cy, 1, childH, borderColor, 0);
                }
                uint32_t textColor = isHeader ? 0xFFFFFFFF : w->fgColor;
                if (!cell->label.empty()) {
                    addText(cell->label, cx + 6, cy + 4, textColor);
                }
                cx += colW;
                colIdx++;
            }
            cy += childH;
        }
        break;
    }

    case WidgetType::TableHeader:
    case WidgetType::TableRow:
    case WidgetType::TableCell: {
        if (w->type == WidgetType::TableCell) {
            addText(w->label, x, y, w->fgColor);
        }
        break;
    }

    case WidgetType::TabBar: {
        addBox(x, y, w->w, w->h, w->bgColor, 4);
        float tabX = x + 4;
        int tabIdx = 0;
        for (auto& child : w->children) {
            bool isActive = (tabIdx == w->activeTab);
            float tabW = child->w > 0 ? child->w : 80;
            float tabH = w->h - 4;

            if (isActive) {
                addBox(tabX, y + 2, tabW, tabH, w->accentColor, 4);
            } else if (child->isHover()) {
                addBox(tabX, y + 2, tabW, tabH, 0xFF333344, 4);
            }
            addText(child->label, tabX + 8, y + 6,
                    isActive ? 0xFFFFFFFF : 0xFFAAAAAA);
            tabX += tabW + 4;
            tabIdx++;
        }
        break;
    }

    case WidgetType::TabItem: {
        if (w->isOpen()) {
            addBox(x, y, w->w, w->h, w->bgColor, 4);
            float cy = y + 4;
            for (auto& child : w->children) {
                renderWidget(child.get(), x + 4, cy, state);
                cy += child->h + 4;
            }
        }
        break;
    }

    case WidgetType::Popup: {
        if (w->isOpen()) {
            if (w->modal) {
                addBox(0, 0, state.pixelWidth, state.pixelHeight, 0x80000000, 0);
            }
            addBox(x + 4, y + 4, w->w, w->h, 0x40000000, 8);
            addBox(x, y, w->w, w->h, w->bgColor, 8);
            addBoxOutline(x, y, w->w, w->h, w->accentColor, 8);

            if (!w->label.empty()) {
                addBox(x, y, w->w, 28, w->headerColor, 8);
                addText(w->label, x + 8, y + 6, w->fgColor);
            }

            float contentY = y + (w->label.empty() ? 8 : 32);
            for (auto& child : w->children) {
                renderWidget(child.get(), x + 8, contentY, state);
                contentY += child->h + 4;
            }
        }
        break;
    }

    case WidgetType::ListBox: {
        addBox(x, y, w->w, w->h, w->bgColor, 4);
        addBoxOutline(x, y, w->w, w->h, 0xFF444455, 4);

        float itemH = w->rowHeight > 0 ? w->rowHeight : 24;
        float contentH = w->options.size() * itemH;
        bool needsScroll = contentH > w->h;
        float scrollbarW = needsScroll ? 10 : 0;
        float contentW = w->w - scrollbarW - 4;
        float maxScroll = std::max(0.0f, contentH - w->h);

        int startIdx = static_cast<int>(w->scrollY / itemH);
        int visibleCount = static_cast<int>(std::ceil(w->h / itemH)) + 1;
        int endIdx = std::min(startIdx + visibleCount,
                              static_cast<int>(w->options.size()));

        for (int i = startIdx; i < endIdx; i++) {
            float itemY = y + i * itemH - w->scrollY;
            if (itemY + itemH <= y || itemY >= y + w->h)
                continue;

            bool isSelected = (i == w->selectedIndex);
            bool isHovered = (state.hoverListWidget == w && i == state.hoverListIdx);

            if (isSelected) {
                addBox(x + 2, itemY + 1, contentW, itemH - 2, w->accentColor, 2);
            } else if (isHovered) {
                addBox(x + 2, itemY + 1, contentW, itemH - 2, 0xFF333344, 2);
            }
            addText(w->options[i], x + 8, itemY + 4, w->fgColor);
        }

        if (needsScroll) {
            float trackH = w->h - 4;
            float thumbH = std::max(20.0f, trackH * w->h / contentH);
            float thumbY =
                y + 2 +
                (maxScroll > 0 ? (w->scrollY / maxScroll) * (trackH - thumbH) : 0);
            addBox(x + w->w - scrollbarW, y + 2, scrollbarW - 2, trackH, 0xFF222233, 4);
            addBox(x + w->w - scrollbarW + 1, thumbY, scrollbarW - 4, thumbH,
                   0xFF555566, 3);
        }
        break;
    }

    case WidgetType::Tooltip: {
        if (!w->label.empty()) {
            addBox(x, y, w->w, w->h, 0xF0222233, 4);
            addBoxOutline(x, y, w->w, w->h, 0xFF666677, 4);
            addText(w->label, x + 6, y + 4, w->fgColor);
        }
        break;
    }
    }

    // Render tooltip on hover
    if (w->isHover() && !w->tooltip.empty()) {
        float tipX = x + w->w + 4;
        float tipY = y;
        float tipW = static_cast<float>(w->tooltip.length()) * 7 + 12;
        float tipH = 22;
        addBox(tipX, tipY, tipW, tipH, 0xF0222233, 4);
        addBoxOutline(tipX, tipY, tipW, tipH, 0xFF666677, 4);
        addText(w->tooltip, tipX + 6, tipY + 4, 0xFFFFFFFF);
    }
}

void YGuiRenderer::renderDropdownOptions(Widget* w, const YGuiRenderState& state) {
    float x = w->x;
    float y = w->y + w->h;
    float optH = 24;

    float listH = static_cast<float>(w->options.size()) * optH;
    addBox(x, y, w->w, listH, 0xFF1E1E2E, 4);

    for (size_t i = 0; i < w->options.size(); i++) {
        float optY = y + i * optH;
        bool isSelected = (static_cast<int>(i) == w->selectedIndex);
        bool isHovered = (static_cast<int>(i) == state.hoverOptionIdx);

        if (isHovered) {
            addBox(x + 2, optY + 2, w->w - 4, optH - 4, w->accentColor, 2);
        } else if (isSelected) {
            addBox(x + 2, optY + 2, w->w - 4, optH - 4, 0xFF2A2A3E, 2);
        }

        addText(w->options[i], x + 8, optY + 4, w->fgColor);

        if (isSelected) {
            addText("\xE2\x9C\x93", x + w->w - 20, optY + 4, w->accentColor);
        }
    }
}

//=============================================================================
// Primitive helpers - delegate to YDrawWriter
//=============================================================================

void YGuiRenderer::addBox(float x, float y, float w, float h, uint32_t color,
                          float radius) {
    if (!_writer) return;
    // YDrawWriter uses center coordinates
    _writer->addRoundedBox(x + w / 2, y + h / 2, w / 2, h / 2, radius, color, 0, 0);
}

void YGuiRenderer::addBoxOutline(float x, float y, float w, float h,
                                  uint32_t color, float radius) {
    if (!_writer) return;
    // YDrawWriter uses center coordinates, strokeWidth for outline
    _writer->addRoundedBox(x + w / 2, y + h / 2, w / 2, h / 2, radius, 0, color, 2);
}

void YGuiRenderer::addText(const std::string& text, float x, float y,
                            uint32_t color) {
    if (!_writer || text.empty()) return;
    float fontSize = 14.0f;
    _writer->addText(x, y + fontSize * 0.8f, text, fontSize, color, 0);
}

void YGuiRenderer::addTriangle(float x0, float y0, float x1, float y1,
                                float x2, float y2, uint32_t color) {
    if (!_writer) return;
    _writer->addTriangle(x0, y0, x1, y1, x2, y2, color, 0, 0);
}

void YGuiRenderer::addCircle(float cx, float cy, float radius, uint32_t color) {
    if (!_writer) return;
    _writer->addCircle(cx, cy, radius, color, 0, 0);
}

void YGuiRenderer::addCircleOutline(float cx, float cy, float radius,
                                     uint32_t color) {
    if (!_writer) return;
    _writer->addCircle(cx, cy, radius, 0, color, 1.5f);
}

} // namespace yetty::ygui
