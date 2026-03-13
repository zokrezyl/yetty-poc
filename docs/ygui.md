# YGui - Retained-Mode Widget System

YGui is a retained-mode UI widget system built on top of YDraw. It provides interactive widgets (buttons, sliders, panels, etc.) rendered via GPU-accelerated SDF primitives.

## Architecture

```
┌─────────────────────────────────────┐
│  Application                        │
├─────────────────────────────────────┤
│  YGuiEngine                         │  Widget management, input routing
│    ├── SpatialGrid                  │  O(1) widget hit detection
│    └── Widget tree                  │  Polymorphic widget hierarchy
├─────────────────────────────────────┤
│  RenderContext                      │  Drawing API wrapper
├─────────────────────────────────────┤
│  YDrawBuffer                        │  SDF primitive storage
├─────────────────────────────────────┤
│  GPU (WebGPU/Dawn)                  │
└─────────────────────────────────────┘
```

## Core Components

### YGuiEngine

The main engine that manages widgets and routes input events.

```cpp
YGuiEngine engine(&buffer, &io);
engine.setPixelSize(800, 600);

// Add widgets
auto btn = std::make_shared<Button>();
btn->id = "my_button";
btn->label = "Click Me";
btn->x = 10; btn->y = 10;
btn->w = 100; btn->h = 30;
btn->onClick = "button_clicked";
engine.addWidget(btn);

// Render to buffer
engine.rebuild();

// Handle input
engine.handleMouseMove(px, py);
engine.handleMouseDown(px, py, 0);
engine.handleMouseUp(px, py, 0);
```

**Key methods:**
- `addWidget(widget)` - Add a widget to the engine
- `removeWidget(widget)` - Remove a widget
- `rebuild()` - Render all widgets to YDrawBuffer
- `widgetAt(px, py)` - O(1) lookup of widget at pixel position
- `handleMouseMove/Down/Up()` - Input event handlers
- `handleMouseScroll()` - Scroll event handler
- `handleKeyDown()` - Keyboard event handler

### SpatialGrid

Provides O(1) widget lookup by screen position. Automatically maintained when widgets are added/removed.

### Widget Base Class

All widgets inherit from `Widget`:

```cpp
class Widget {
    std::string id;
    float x, y, w, h;           // Local geometry
    float effectiveX, effectiveY; // Computed screen position
    uint32_t flags;             // HOVER, PRESSED, FOCUSED, etc.
    uint32_t bgColor, fgColor, accentColor;
    std::string onClick, onChange;
    std::vector<Ptr> children;

    virtual void render(RenderContext& ctx);
    virtual void renderAll(RenderContext& ctx);
    virtual std::optional<WidgetEvent> onPress(float x, float y);
    virtual std::optional<WidgetEvent> onRelease(float x, float y);
    virtual std::optional<WidgetEvent> onDrag(float x, float y);
    virtual std::optional<WidgetEvent> onScroll(float dx, float dy);
    virtual std::optional<WidgetEvent> onKey(uint32_t key, int mods);
};
```

### RenderContext

Convenience wrapper around YDrawBuffer with offset/clip stacks:

```cpp
void render(RenderContext& ctx) {
    ctx.box(x, y, w, h, bgColor, 4.0f);
    ctx.text("Hello", x + 10, y + 5, fgColor, 14.0f);
    ctx.circle(cx, cy, r, accentColor);
    ctx.boxOutline(x, y, w, h, borderColor, 4.0f, 2.0f);
}
```

**Drawing methods:**
- `box()` - Filled rounded rectangle
- `boxOutline()` - Rectangle outline
- `text()` - Text rendering
- `circle()` / `circleOutline()` - Circles
- `triangle()` - Filled triangle
- `colorWheel()` - HSV color picker wheel

**Layout helpers:**
- `pushOffset()` / `popOffset()` - Offset stack for nested layouts
- `pushClipRect()` / `popClipRect()` - Clip rect stack for scrolling
- `OffsetGuard` / `ClipGuard` - RAII guards

### Theme

Centralized style constants:

```cpp
struct Theme {
    float padSmall = 2.0f;
    float padMedium = 4.0f;
    float padLarge = 8.0f;
    float radiusSmall = 2.0f;
    float radiusMedium = 4.0f;
    float radiusLarge = 8.0f;
    float rowHeight = 24.0f;
    float scrollbarSize = 12.0f;
    float fontSize = 14.0f;

    uint32_t bgPrimary = 0xFF1A1A2E;
    uint32_t bgSurface = 0xFF2A2A3E;
    uint32_t textPrimary = 0xFFFFFFFF;
    // ... more colors
};
```

### WidgetEvent

Events emitted by widget interactions:

```cpp
struct WidgetEvent {
    std::string widgetId;   // Widget ID
    std::string eventType;  // "click", "change", "scroll"
    std::string key;        // "value", "checked", "text"
    std::string value;      // Serialized value
};
```

## Widgets

### Basic Widgets

| Widget | Description |
|--------|-------------|
| `Label` | Static text display |
| `Button` | Clickable button with label |
| `Checkbox` | Toggle checkbox |
| `Slider` | Value slider with min/max |
| `TextInput` | Text entry field |
| `Separator` | Visual divider |
| `Tooltip` | Hover tooltip |
| `Progress` | Progress bar |

### Selection Widgets

| Widget | Description |
|--------|-------------|
| `Dropdown` | Dropdown menu selector |
| `ChoiceBox` | Inline choice selection |
| `ListBox` | Scrollable list |
| `Selectable` | Selectable item in a list |

### Layout Widgets

| Widget | Description |
|--------|-------------|
| `Panel` | Root container with scrolling |
| `HBox` | Horizontal layout |
| `VBox` | Vertical layout |
| `ScrollArea` | Scrollable content area |
| `CollapsingHeader` | Collapsible section |

### Complex Widgets

| Widget | Description |
|--------|-------------|
| `Table` | Data table with header/rows |
| `TabBar` | Tabbed interface |
| `ColorPicker` | HSV color picker |
| `Popup` | Modal popup/dialog |

## Widget Examples

### Button

```cpp
auto btn = std::make_shared<Button>();
btn->id = "submit";
btn->label = "Submit";
btn->x = 10; btn->y = 10;
btn->w = 80; btn->h = 30;
btn->onClick = "submit_form";
engine.addWidget(btn);
```

### Slider

```cpp
auto slider = std::make_shared<Slider>();
slider->id = "volume";
slider->x = 10; slider->y = 50;
slider->w = 200; slider->h = 24;
slider->minValue = 0;
slider->maxValue = 100;
slider->value = 50;
slider->onChange = "volume_changed";
engine.addWidget(slider);
```

### Panel with Children

```cpp
auto panel = std::make_shared<Panel>();
panel->id = "main";
panel->x = 50; panel->y = 50;
panel->w = 300; panel->h = 400;
panel->headerH = 30;  // Fixed header
panel->contentH = 800; // Total content height

auto title = std::make_shared<Label>();
title->id = "title";
title->text = "Settings";
title->y = 5; // In header (y < headerH)
panel->children.push_back(title);

auto btn = std::make_shared<Button>();
btn->y = 50; // In scrollable area (y >= headerH)
panel->children.push_back(btn);

engine.addWidget(panel);
```

## Event Handling

Events are emitted via `YGuiIO`:

```cpp
class MyIO : public YGuiIO {
    void emitEvent(const WidgetEvent& e) override {
        if (e.widgetId == "volume" && e.eventType == "change") {
            float vol = std::stof(e.value);
            setVolume(vol);
        }
        if (e.widgetId == "submit" && e.eventType == "click") {
            submitForm();
        }
    }
};
```

## File Structure

```
src/yetty/ygui/
├── ygui-engine.h/cpp      # Core engine
├── ygui-widgets.h         # Widget base class, RenderContext
├── ygui-theme.h           # Theme constants
├── ygui-io.h              # Event interface
├── ygui-overlay.h/cpp     # Fullscreen overlay rendering
└── widgets/
    ├── button.h
    ├── slider.h
    ├── checkbox.h
    ├── panel.h
    ├── hbox.h / vbox.h
    ├── scroll-area.h
    ├── dropdown.h
    ├── listbox.h
    ├── table.h
    ├── tabbar.h
    ├── color-picker.h
    ├── text-input.h
    ├── collapsing-header.h
    ├── popup.h
    └── ...
```

## Relationship to YDraw

YGui uses YDraw for all rendering:

1. Widgets call `RenderContext` methods (box, text, circle, etc.)
2. RenderContext calls `YDrawBuffer::add*` methods
3. YDrawBuilder computes spatial grid and prepares GPU data
4. GPU shader evaluates SDFs for rendering

This means all YGui widgets are resolution-independent and GPU-accelerated.
