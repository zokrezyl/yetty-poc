#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace yetty::ygui {

//=============================================================================
// Widget types
//=============================================================================
enum class WidgetType : uint8_t {
    Label,
    Button,
    Checkbox,
    Slider,
    Dropdown,
    TextInput,
    Progress,
    Panel,
    VBox,
    HBox,
    ChoiceBox,        // Radio button group (shows all options)
    Separator,        // Horizontal line
    ColorPicker,      // RGBA color selector
    Selectable,       // Clickable/toggleable row
    CollapsingHeader, // Expandable section header
    VScrollbar,       // Vertical scrollbar
    HScrollbar,       // Horizontal scrollbar
    ScrollArea,       // Scrollable container with children
    Table,            // Table container
    TableRow,         // Table row
    TableCell,        // Table cell
    TableHeader,      // Table header row
    TabBar,           // Tab container (horizontal tabs)
    TabItem,          // Individual tab
    Popup,            // Modal popup/dialog
    ListBox,          // Scrollable list of selectable items
    Tooltip,          // Hover tooltip
};

//=============================================================================
// Widget state flags
//=============================================================================
enum WidgetFlags : uint32_t {
    WIDGET_HOVER    = 1 << 0,
    WIDGET_PRESSED  = 1 << 1,
    WIDGET_FOCUSED  = 1 << 2,
    WIDGET_DISABLED = 1 << 3,
    WIDGET_CHECKED  = 1 << 4,
    WIDGET_OPEN     = 1 << 5,
};

//=============================================================================
// Widget base
//=============================================================================
struct Widget {
    std::string id;
    WidgetType type = WidgetType::Label;
    float x = 0, y = 0, w = 100, h = 24;
    uint32_t flags = 0;
    std::string label;

    // Type-specific data
    float value = 0;                    // Slider value, progress
    float minValue = 0, maxValue = 1;   // Slider range
    std::vector<std::string> options;   // Dropdown/ChoiceBox options
    int selectedIndex = 0;              // Dropdown/ChoiceBox selection
    std::string text;                   // TextInput content
    uint32_t cursorPos = 0;             // TextInput cursor
    uint32_t colorValue = 0xFFFFFFFF;   // ColorPicker ABGR value

    // ScrollArea properties
    float scrollX = 0, scrollY = 0;     // Current scroll offset
    float contentW = 0, contentH = 0;   // Content size (for scroll range)

    // Table properties
    std::vector<float> columnWidths;    // Column widths for Table
    float rowHeight = 24;               // Row height for Table/ListBox

    // Tab properties
    int activeTab = 0;                  // Active tab index for TabBar

    // Tooltip
    std::string tooltip;                // Tooltip text (shown on hover)

    // Popup properties
    bool modal = false;                 // Modal popup blocks input

    // Styling
    uint32_t bgColor = 0xFF2A2A3E;
    uint32_t fgColor = 0xFFFFFFFF;
    uint32_t accentColor = 0xFF4488FF;
    uint32_t headerColor = 0xFF3A3A4E;

    // Children (for layout widgets)
    std::vector<std::shared_ptr<Widget>> children;

    // Events (callbacks stored as event names to emit via OSC)
    std::string onClick;
    std::string onChange;

    bool contains(float px, float py) const {
        return px >= x && px < x + w && py >= y && py < y + h;
    }

    bool isHover() const { return flags & WIDGET_HOVER; }
    bool isPressed() const { return flags & WIDGET_PRESSED; }
    bool isFocused() const { return flags & WIDGET_FOCUSED; }
    bool isDisabled() const { return flags & WIDGET_DISABLED; }
    bool isChecked() const { return flags & WIDGET_CHECKED; }
    bool isOpen() const { return flags & WIDGET_OPEN; }
};

using WidgetPtr = std::shared_ptr<Widget>;

} // namespace yetty::ygui
