#include "ygui-parser.h"
#include <yaml-cpp/yaml.h>
#include <ytrace/ytrace.hpp>

namespace yetty::ygui {

WidgetPtr parseWidgetImpl(const YAML::Node& node);

std::vector<WidgetPtr> parseWidgets(const std::string& yaml) {
    std::vector<WidgetPtr> widgets;
    try {
        YAML::Node root = YAML::Load(yaml);
        if (root["widgets"]) {
            for (const auto& node : root["widgets"]) {
                auto w = parseWidgetImpl(node);
                if (w)
                    widgets.push_back(w);
            }
        }
    } catch (const std::exception& e) {
        yerror("ygui::parseWidgets: YAML parse error: {}", e.what());
    }
    return widgets;
}

WidgetPtr parseWidget(const void* nodePtr) {
    const auto& node = *static_cast<const YAML::Node*>(nodePtr);
    return parseWidgetImpl(node);
}

WidgetPtr parseWidgetImpl(const YAML::Node& node) {
    auto w = std::make_shared<Widget>();

    if (node["id"])
        w->id = node["id"].as<std::string>();

    if (node["type"]) {
        std::string t = node["type"].as<std::string>();
        if (t == "label")
            w->type = WidgetType::Label;
        else if (t == "button")
            w->type = WidgetType::Button;
        else if (t == "checkbox")
            w->type = WidgetType::Checkbox;
        else if (t == "slider")
            w->type = WidgetType::Slider;
        else if (t == "dropdown")
            w->type = WidgetType::Dropdown;
        else if (t == "textinput")
            w->type = WidgetType::TextInput;
        else if (t == "progress")
            w->type = WidgetType::Progress;
        else if (t == "panel")
            w->type = WidgetType::Panel;
        else if (t == "vbox")
            w->type = WidgetType::VBox;
        else if (t == "hbox")
            w->type = WidgetType::HBox;
        else if (t == "choicebox" || t == "choice" || t == "radio")
            w->type = WidgetType::ChoiceBox;
        else if (t == "separator" || t == "sep")
            w->type = WidgetType::Separator;
        else if (t == "colorpicker" || t == "color")
            w->type = WidgetType::ColorPicker;
        else if (t == "selectable")
            w->type = WidgetType::Selectable;
        else if (t == "collapsing" || t == "header")
            w->type = WidgetType::CollapsingHeader;
        else if (t == "vscrollbar" || t == "vscroll")
            w->type = WidgetType::VScrollbar;
        else if (t == "hscrollbar" || t == "hscroll")
            w->type = WidgetType::HScrollbar;
        else if (t == "scrollarea" || t == "scroll")
            w->type = WidgetType::ScrollArea;
        else if (t == "table")
            w->type = WidgetType::Table;
        else if (t == "row" || t == "tr")
            w->type = WidgetType::TableRow;
        else if (t == "cell" || t == "td")
            w->type = WidgetType::TableCell;
        else if (t == "header" || t == "th")
            w->type = WidgetType::TableHeader;
        else if (t == "tabbar" || t == "tabs")
            w->type = WidgetType::TabBar;
        else if (t == "tab" || t == "tabitem")
            w->type = WidgetType::TabItem;
        else if (t == "popup" || t == "dialog" || t == "modal")
            w->type = WidgetType::Popup;
        else if (t == "listbox" || t == "list")
            w->type = WidgetType::ListBox;
        else if (t == "tooltip")
            w->type = WidgetType::Tooltip;
    }

    if (node["x"])
        w->x = node["x"].as<float>();
    if (node["y"])
        w->y = node["y"].as<float>();
    if (node["w"] || node["width"])
        w->w = (node["w"] ? node["w"] : node["width"]).as<float>();
    if (node["h"] || node["height"])
        w->h = (node["h"] ? node["h"] : node["height"]).as<float>();
    if (node["label"])
        w->label = node["label"].as<std::string>();
    if (node["text"])
        w->text = node["text"].as<std::string>();
    if (node["value"])
        w->value = node["value"].as<float>();
    if (node["min"])
        w->minValue = node["min"].as<float>();
    if (node["max"])
        w->maxValue = node["max"].as<float>();
    if (node["checked"] && node["checked"].as<bool>())
        w->flags |= WIDGET_CHECKED;
    if (node["disabled"] && node["disabled"].as<bool>())
        w->flags |= WIDGET_DISABLED;

    if (node["options"]) {
        for (const auto& opt : node["options"]) {
            w->options.push_back(opt.as<std::string>());
        }
    }
    if (node["selected"])
        w->selectedIndex = node["selected"].as<int>();

    // ColorPicker color value (hex string like "0xFFRRGGBB" or "#RRGGBB")
    if (node["color"]) {
        std::string colorStr = node["color"].as<std::string>();
        if (colorStr.size() > 2 && colorStr[0] == '0' &&
            (colorStr[1] == 'x' || colorStr[1] == 'X')) {
            w->colorValue =
                static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
        } else if (colorStr.size() > 0 && colorStr[0] == '#') {
            // #RRGGBB -> 0xFFBBGGRR (ABGR)
            uint32_t rgb =
                static_cast<uint32_t>(std::stoul(colorStr.substr(1), nullptr, 16));
            uint8_t r = (rgb >> 16) & 0xFF;
            uint8_t g = (rgb >> 8) & 0xFF;
            uint8_t b = rgb & 0xFF;
            w->colorValue = 0xFF000000 | (b << 16) | (g << 8) | r;
        }
    }

    // CollapsingHeader initial state
    if (node["open"] && node["open"].as<bool>())
        w->flags |= WIDGET_OPEN;

    if (node["on_click"])
        w->onClick = node["on_click"].as<std::string>();
    if (node["on_change"])
        w->onChange = node["on_change"].as<std::string>();

    // Parse children
    if (node["children"]) {
        for (const auto& child : node["children"]) {
            auto cw = parseWidgetImpl(child);
            if (cw)
                w->children.push_back(cw);
        }
    }

    // Auto-calculate height for widgets with options
    if (w->type == WidgetType::ChoiceBox && !w->options.empty()) {
        w->h = static_cast<float>(w->options.size()) * 24.0f;
    }

    // ScrollArea content size
    if (node["content_w"])
        w->contentW = node["content_w"].as<float>();
    if (node["content_h"])
        w->contentH = node["content_h"].as<float>();

    // Table properties
    if (node["columns"] && node["columns"].IsSequence()) {
        for (const auto& col : node["columns"]) {
            w->columnWidths.push_back(col.as<float>());
        }
    }
    if (node["row_height"])
        w->rowHeight = node["row_height"].as<float>();

    // Tab properties
    if (node["active_tab"])
        w->activeTab = node["active_tab"].as<int>();
    if (node["active"])
        w->activeTab = node["active"].as<int>();

    // Tooltip
    if (node["tooltip"])
        w->tooltip = node["tooltip"].as<std::string>();

    // Popup properties
    if (node["modal"] && node["modal"].as<bool>())
        w->modal = true;

    // Header color
    if (node["header_color"]) {
        std::string colorStr = node["header_color"].as<std::string>();
        if (colorStr[0] == '#') {
            uint32_t rgb =
                static_cast<uint32_t>(std::stoul(colorStr.substr(1), nullptr, 16));
            uint8_t r = (rgb >> 16) & 0xFF;
            uint8_t g = (rgb >> 8) & 0xFF;
            uint8_t b = rgb & 0xFF;
            w->headerColor = 0xFF000000 | (b << 16) | (g << 8) | r;
        }
    }

    // Auto-calculate ListBox height
    if (w->type == WidgetType::ListBox && !w->options.empty() && w->h == 24) {
        w->h = std::min(static_cast<float>(w->options.size()), 5.0f) * w->rowHeight;
    }

    return w;
}

} // namespace yetty::ygui
