--[[
YGui Lua Bindings

LuaJIT FFI bindings for the YGui-C widget library.
Requires LuaJIT with FFI support.
]]

local ffi = require("ffi")

-- C declarations
ffi.cdef[[
    typedef struct ygui_engine ygui_engine_t;
    typedef struct ygui_widget ygui_widget_t;
    typedef struct ygui_theme ygui_theme_t;
    typedef struct ydraw_buffer ydraw_buffer_t;

    typedef enum {
        YGUI_WIDGET_BUTTON,
        YGUI_WIDGET_LABEL,
        YGUI_WIDGET_SLIDER,
        YGUI_WIDGET_CHECKBOX,
        YGUI_WIDGET_TEXTINPUT,
        YGUI_WIDGET_PANEL,
        YGUI_WIDGET_HBOX,
        YGUI_WIDGET_VBOX,
        YGUI_WIDGET_DROPDOWN,
        YGUI_WIDGET_LISTBOX,
        YGUI_WIDGET_TABLE,
        YGUI_WIDGET_TABBAR,
        YGUI_WIDGET_COLORPICKER,
        YGUI_WIDGET_SCROLLAREA,
        YGUI_WIDGET_PROGRESS,
        YGUI_WIDGET_SEPARATOR,
        YGUI_WIDGET_CUSTOM,
    } ygui_widget_type_t;

    typedef enum {
        YGUI_EVENT_NONE = 0,
        YGUI_EVENT_CLICK,
        YGUI_EVENT_PRESS,
        YGUI_EVENT_RELEASE,
        YGUI_EVENT_CHANGE,
        YGUI_EVENT_SCROLL,
        YGUI_EVENT_FOCUS,
        YGUI_EVENT_BLUR,
        YGUI_EVENT_KEY,
        YGUI_EVENT_TEXT,
    } ygui_event_type_t;

    typedef struct {
        const char* widget_id;
        int type;
        union {
            float float_value;
            int int_value;
            int bool_value;
            const char* string_value;
            struct { float r, g, b, a; } color;
            struct { float x, y; } scroll;
            struct { uint32_t key; int mods; } key;
        } data;
    } ygui_event_t;

    typedef void (*ygui_event_callback_t)(const ygui_event_t* event, void* userdata);

    // Engine
    ygui_engine_t* ygui_engine_create(ydraw_buffer_t* buffer);
    void ygui_engine_destroy(ygui_engine_t* engine);
    void ygui_engine_set_size(ygui_engine_t* engine, float width, float height);
    void ygui_engine_set_event_callback(ygui_engine_t* engine,
                                         ygui_event_callback_t callback,
                                         void* userdata);
    int ygui_engine_is_dirty(const ygui_engine_t* engine);
    void ygui_engine_mark_dirty(ygui_engine_t* engine);
    void ygui_engine_rebuild(ygui_engine_t* engine);
    void ygui_engine_clear(ygui_engine_t* engine);
    void ygui_engine_mouse_move(ygui_engine_t* engine, float x, float y);
    void ygui_engine_mouse_down(ygui_engine_t* engine, float x, float y, int button);
    void ygui_engine_mouse_up(ygui_engine_t* engine, float x, float y, int button);
    void ygui_engine_mouse_scroll(ygui_engine_t* engine, float x, float y, float dx, float dy);
    ygui_widget_t* ygui_engine_find(ygui_engine_t* engine, const char* id);

    // Widgets
    ygui_widget_t* ygui_button(ygui_engine_t* engine, const char* id,
                               float x, float y, float w, float h, const char* label);
    ygui_widget_t* ygui_label(ygui_engine_t* engine, const char* id,
                              float x, float y, const char* text);
    ygui_widget_t* ygui_slider(ygui_engine_t* engine, const char* id,
                               float x, float y, float w, float h,
                               float min_val, float max_val, float value);
    ygui_widget_t* ygui_checkbox(ygui_engine_t* engine, const char* id,
                                 float x, float y, float w, float h,
                                 const char* label, int checked);
    ygui_widget_t* ygui_panel(ygui_engine_t* engine, const char* id,
                              float x, float y, float w, float h);
    ygui_widget_t* ygui_progress(ygui_engine_t* engine, const char* id,
                                 float x, float y, float w, float h, float value);

    // Widget hierarchy
    void ygui_widget_add_child(ygui_widget_t* parent, ygui_widget_t* child);
    void ygui_widget_remove(ygui_widget_t* widget);

    // Widget properties
    void ygui_widget_set_position(ygui_widget_t* widget, float x, float y);
    void ygui_widget_set_size(ygui_widget_t* widget, float w, float h);
    void ygui_widget_set_visible(ygui_widget_t* widget, int visible);
    void ygui_widget_set_enabled(ygui_widget_t* widget, int enabled);
    void ygui_widget_set_bg_color(ygui_widget_t* widget, uint32_t color);

    // Widget-specific
    void ygui_button_set_label(ygui_widget_t* widget, const char* label);
    void ygui_label_set_text(ygui_widget_t* widget, const char* text);
    void ygui_slider_set_value(ygui_widget_t* widget, float value);
    float ygui_slider_get_value(const ygui_widget_t* widget);
    void ygui_checkbox_set_checked(ygui_widget_t* widget, int checked);
    int ygui_checkbox_get_checked(const ygui_widget_t* widget);
    void ygui_progress_set_value(ygui_widget_t* widget, float value);
    void ygui_panel_set_content_size(ygui_widget_t* widget, float w, float h);

    // Version
    const char* ygui_version(void);
]]

-- Load the library
local lib = ffi.load("ygui")

-- Module table
local ygui = {}

-- Event types
ygui.EventType = {
    NONE = 0,
    CLICK = 1,
    PRESS = 2,
    RELEASE = 3,
    CHANGE = 4,
    SCROLL = 5,
    FOCUS = 6,
    BLUR = 7,
    KEY = 8,
    TEXT = 9,
}

-- Widget class
local Widget = {}
Widget.__index = Widget

function Widget:new(handle)
    local w = setmetatable({}, Widget)
    w.handle = handle
    return w
end

function Widget:setPosition(x, y)
    lib.ygui_widget_set_position(self.handle, x, y)
end

function Widget:setSize(w, h)
    lib.ygui_widget_set_size(self.handle, w, h)
end

function Widget:setVisible(visible)
    lib.ygui_widget_set_visible(self.handle, visible and 1 or 0)
end

function Widget:setEnabled(enabled)
    lib.ygui_widget_set_enabled(self.handle, enabled and 1 or 0)
end

function Widget:setBgColor(color)
    lib.ygui_widget_set_bg_color(self.handle, color)
end

function Widget:addChild(child)
    lib.ygui_widget_add_child(self.handle, child.handle)
end

function Widget:remove()
    lib.ygui_widget_remove(self.handle)
    self.handle = nil
end

-- Button class
local Button = setmetatable({}, {__index = Widget})
Button.__index = Button

function Button:new(handle)
    local b = Widget:new(handle)
    setmetatable(b, Button)
    return b
end

function Button:setLabel(label)
    lib.ygui_button_set_label(self.handle, label)
end

-- Label class
local Label = setmetatable({}, {__index = Widget})
Label.__index = Label

function Label:new(handle)
    local l = Widget:new(handle)
    setmetatable(l, Label)
    return l
end

function Label:setText(text)
    lib.ygui_label_set_text(self.handle, text)
end

-- Slider class
local Slider = setmetatable({}, {__index = Widget})
Slider.__index = Slider

function Slider:new(handle)
    local s = Widget:new(handle)
    setmetatable(s, Slider)
    return s
end

function Slider:setValue(value)
    lib.ygui_slider_set_value(self.handle, value)
end

function Slider:getValue()
    return lib.ygui_slider_get_value(self.handle)
end

-- Checkbox class
local Checkbox = setmetatable({}, {__index = Widget})
Checkbox.__index = Checkbox

function Checkbox:new(handle)
    local c = Widget:new(handle)
    setmetatable(c, Checkbox)
    return c
end

function Checkbox:setChecked(checked)
    lib.ygui_checkbox_set_checked(self.handle, checked and 1 or 0)
end

function Checkbox:isChecked()
    return lib.ygui_checkbox_get_checked(self.handle) ~= 0
end

-- Panel class
local Panel = setmetatable({}, {__index = Widget})
Panel.__index = Panel

function Panel:new(handle)
    local p = Widget:new(handle)
    setmetatable(p, Panel)
    return p
end

function Panel:setContentSize(w, h)
    lib.ygui_panel_set_content_size(self.handle, w, h)
end

-- Progress class
local Progress = setmetatable({}, {__index = Widget})
Progress.__index = Progress

function Progress:new(handle)
    local p = Widget:new(handle)
    setmetatable(p, Progress)
    return p
end

function Progress:setValue(value)
    lib.ygui_progress_set_value(self.handle, value)
end

-- Engine class
local Engine = {}
Engine.__index = Engine

function ygui.Engine(width, height)
    width = width or 800
    height = height or 600

    local handle = lib.ygui_engine_create(nil)
    if handle == nil then
        return nil, "Failed to create engine"
    end

    lib.ygui_engine_set_size(handle, width, height)

    local e = setmetatable({}, Engine)
    e.handle = handle
    e.callback = nil
    e.callbackRef = nil
    e.widgets = {}

    return e
end

function Engine:destroy()
    if self.handle ~= nil then
        lib.ygui_engine_destroy(self.handle)
        self.handle = nil
    end
end

function Engine:setSize(width, height)
    lib.ygui_engine_set_size(self.handle, width, height)
end

function Engine:onEvent(callback)
    self.callback = callback

    -- Create C callback wrapper
    local function cCallback(event, userdata)
        if self.callback then
            local e = {
                widgetId = event.widget_id ~= nil and ffi.string(event.widget_id) or "",
                type = event.type,
                floatValue = event.data.float_value,
            }
            self.callback(e)
        end
    end

    self.callbackRef = ffi.cast("ygui_event_callback_t", cCallback)
    lib.ygui_engine_set_event_callback(self.handle, self.callbackRef, nil)
end

function Engine:isDirty()
    return lib.ygui_engine_is_dirty(self.handle) ~= 0
end

function Engine:markDirty()
    lib.ygui_engine_mark_dirty(self.handle)
end

function Engine:rebuild()
    lib.ygui_engine_rebuild(self.handle)
end

function Engine:clear()
    lib.ygui_engine_clear(self.handle)
    self.widgets = {}
end

function Engine:mouseMove(x, y)
    lib.ygui_engine_mouse_move(self.handle, x, y)
end

function Engine:mouseDown(x, y, button)
    button = button or 0
    lib.ygui_engine_mouse_down(self.handle, x, y, button)
end

function Engine:mouseUp(x, y, button)
    button = button or 0
    lib.ygui_engine_mouse_up(self.handle, x, y, button)
end

function Engine:mouseScroll(x, y, dx, dy)
    lib.ygui_engine_mouse_scroll(self.handle, x, y, dx, dy)
end

function Engine:find(id)
    local handle = lib.ygui_engine_find(self.handle, id)
    if handle ~= nil then
        return self.widgets[id] or Widget:new(handle)
    end
    return nil
end

-- Widget creation
function Engine:button(id, x, y, w, h, label)
    local handle = lib.ygui_button(self.handle, id, x, y, w, h, label)
    if handle == nil then return nil end
    local btn = Button:new(handle)
    self.widgets[id] = btn
    return btn
end

function Engine:label(id, x, y, text)
    local handle = lib.ygui_label(self.handle, id, x, y, text)
    if handle == nil then return nil end
    local lbl = Label:new(handle)
    self.widgets[id] = lbl
    return lbl
end

function Engine:slider(id, x, y, w, h, minVal, maxVal, value)
    minVal = minVal or 0
    maxVal = maxVal or 100
    value = value or 50
    local handle = lib.ygui_slider(self.handle, id, x, y, w, h, minVal, maxVal, value)
    if handle == nil then return nil end
    local sld = Slider:new(handle)
    self.widgets[id] = sld
    return sld
end

function Engine:checkbox(id, x, y, w, h, label, checked)
    checked = checked and 1 or 0
    local handle = lib.ygui_checkbox(self.handle, id, x, y, w, h, label, checked)
    if handle == nil then return nil end
    local chk = Checkbox:new(handle)
    self.widgets[id] = chk
    return chk
end

function Engine:panel(id, x, y, w, h)
    local handle = lib.ygui_panel(self.handle, id, x, y, w, h)
    if handle == nil then return nil end
    local pnl = Panel:new(handle)
    self.widgets[id] = pnl
    return pnl
end

function Engine:progress(id, x, y, w, h, value)
    value = value or 0
    local handle = lib.ygui_progress(self.handle, id, x, y, w, h, value)
    if handle == nil then return nil end
    local prg = Progress:new(handle)
    self.widgets[id] = prg
    return prg
end

-- Version
function ygui.version()
    return ffi.string(lib.ygui_version())
end

-- Export classes
ygui.Widget = Widget
ygui.Button = Button
ygui.Label = Label
ygui.Slider = Slider
ygui.Checkbox = Checkbox
ygui.Panel = Panel
ygui.Progress = Progress

return ygui
