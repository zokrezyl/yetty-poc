//! YGui Rust Bindings
//!
//! Safe Rust wrappers for the YGui-C widget library.

use std::ffi::{c_void, CStr, CString};
use std::os::raw::{c_char, c_float, c_int};
use std::ptr;

// FFI bindings
mod ffi {
    use std::ffi::c_void;
    use std::os::raw::{c_char, c_float, c_int};

    pub type YguiEngine = c_void;
    pub type YguiWidget = c_void;
    pub type YguiTheme = c_void;
    pub type YdrawBuffer = c_void;

    #[repr(C)]
    #[derive(Debug, Copy, Clone)]
    pub enum YguiWidgetType {
        Button = 0,
        Label = 1,
        Slider = 2,
        Checkbox = 3,
        TextInput = 4,
        Panel = 5,
        HBox = 6,
        VBox = 7,
        Dropdown = 8,
        ListBox = 9,
        Table = 10,
        TabBar = 11,
        ColorPicker = 12,
        ScrollArea = 13,
        Progress = 14,
        Separator = 15,
        Custom = 16,
    }

    #[repr(C)]
    #[derive(Debug, Copy, Clone, PartialEq)]
    pub enum YguiEventType {
        None = 0,
        Click = 1,
        Press = 2,
        Release = 3,
        Change = 4,
        Scroll = 5,
        Focus = 6,
        Blur = 7,
        Key = 8,
        Text = 9,
    }

    #[repr(C)]
    #[derive(Copy, Clone)]
    pub union YguiEventData {
        pub float_value: c_float,
        pub int_value: i32,
        pub bool_value: c_int,
        pub string_value: *const c_char,
    }

    #[repr(C)]
    pub struct YguiEvent {
        pub widget_id: *const c_char,
        pub event_type: c_int,
        pub data: YguiEventData,
    }

    pub type YguiEventCallback = extern "C" fn(*const YguiEvent, *mut c_void);

    extern "C" {
        // Engine
        pub fn ygui_engine_create(buffer: *mut YdrawBuffer) -> *mut YguiEngine;
        pub fn ygui_engine_destroy(engine: *mut YguiEngine);
        pub fn ygui_engine_set_size(engine: *mut YguiEngine, width: c_float, height: c_float);
        pub fn ygui_engine_set_event_callback(
            engine: *mut YguiEngine,
            callback: Option<YguiEventCallback>,
            userdata: *mut c_void,
        );
        pub fn ygui_engine_is_dirty(engine: *const YguiEngine) -> c_int;
        pub fn ygui_engine_mark_dirty(engine: *mut YguiEngine);
        pub fn ygui_engine_rebuild(engine: *mut YguiEngine);
        pub fn ygui_engine_clear(engine: *mut YguiEngine);
        pub fn ygui_engine_mouse_move(engine: *mut YguiEngine, x: c_float, y: c_float);
        pub fn ygui_engine_mouse_down(engine: *mut YguiEngine, x: c_float, y: c_float, button: c_int);
        pub fn ygui_engine_mouse_up(engine: *mut YguiEngine, x: c_float, y: c_float, button: c_int);
        pub fn ygui_engine_mouse_scroll(
            engine: *mut YguiEngine,
            x: c_float,
            y: c_float,
            dx: c_float,
            dy: c_float,
        );
        pub fn ygui_engine_find(engine: *mut YguiEngine, id: *const c_char) -> *mut YguiWidget;

        // Widgets
        pub fn ygui_button(
            engine: *mut YguiEngine,
            id: *const c_char,
            x: c_float,
            y: c_float,
            w: c_float,
            h: c_float,
            label: *const c_char,
        ) -> *mut YguiWidget;

        pub fn ygui_label(
            engine: *mut YguiEngine,
            id: *const c_char,
            x: c_float,
            y: c_float,
            text: *const c_char,
        ) -> *mut YguiWidget;

        pub fn ygui_slider(
            engine: *mut YguiEngine,
            id: *const c_char,
            x: c_float,
            y: c_float,
            w: c_float,
            h: c_float,
            min_val: c_float,
            max_val: c_float,
            value: c_float,
        ) -> *mut YguiWidget;

        pub fn ygui_checkbox(
            engine: *mut YguiEngine,
            id: *const c_char,
            x: c_float,
            y: c_float,
            w: c_float,
            h: c_float,
            label: *const c_char,
            checked: c_int,
        ) -> *mut YguiWidget;

        pub fn ygui_panel(
            engine: *mut YguiEngine,
            id: *const c_char,
            x: c_float,
            y: c_float,
            w: c_float,
            h: c_float,
        ) -> *mut YguiWidget;

        pub fn ygui_progress(
            engine: *mut YguiEngine,
            id: *const c_char,
            x: c_float,
            y: c_float,
            w: c_float,
            h: c_float,
            value: c_float,
        ) -> *mut YguiWidget;

        // Widget hierarchy
        pub fn ygui_widget_add_child(parent: *mut YguiWidget, child: *mut YguiWidget);
        pub fn ygui_widget_remove(widget: *mut YguiWidget);

        // Widget properties
        pub fn ygui_widget_set_position(widget: *mut YguiWidget, x: c_float, y: c_float);
        pub fn ygui_widget_set_size(widget: *mut YguiWidget, w: c_float, h: c_float);
        pub fn ygui_widget_set_visible(widget: *mut YguiWidget, visible: c_int);
        pub fn ygui_widget_set_enabled(widget: *mut YguiWidget, enabled: c_int);
        pub fn ygui_widget_set_bg_color(widget: *mut YguiWidget, color: u32);

        // Widget-specific
        pub fn ygui_button_set_label(widget: *mut YguiWidget, label: *const c_char);
        pub fn ygui_label_set_text(widget: *mut YguiWidget, text: *const c_char);
        pub fn ygui_slider_set_value(widget: *mut YguiWidget, value: c_float);
        pub fn ygui_slider_get_value(widget: *const YguiWidget) -> c_float;
        pub fn ygui_checkbox_set_checked(widget: *mut YguiWidget, checked: c_int);
        pub fn ygui_checkbox_get_checked(widget: *const YguiWidget) -> c_int;
        pub fn ygui_progress_set_value(widget: *mut YguiWidget, value: c_float);
        pub fn ygui_panel_set_content_size(widget: *mut YguiWidget, w: c_float, h: c_float);

        // Version
        pub fn ygui_version() -> *const c_char;
    }
}

/// Event types
#[derive(Debug, Clone, PartialEq)]
pub enum EventType {
    None,
    Click,
    Press,
    Release,
    Change,
    Scroll,
    Focus,
    Blur,
    Key,
    Text,
}

impl From<c_int> for EventType {
    fn from(v: c_int) -> Self {
        match v {
            1 => EventType::Click,
            2 => EventType::Press,
            3 => EventType::Release,
            4 => EventType::Change,
            5 => EventType::Scroll,
            6 => EventType::Focus,
            7 => EventType::Blur,
            8 => EventType::Key,
            9 => EventType::Text,
            _ => EventType::None,
        }
    }
}

/// Widget event
#[derive(Debug, Clone)]
pub struct Event {
    pub widget_id: String,
    pub event_type: EventType,
    pub float_value: Option<f32>,
}

/// Widget handle
pub struct Widget {
    handle: *mut ffi::YguiWidget,
}

impl Widget {
    fn new(handle: *mut ffi::YguiWidget) -> Option<Self> {
        if handle.is_null() {
            None
        } else {
            Some(Widget { handle })
        }
    }

    pub fn set_position(&self, x: f32, y: f32) {
        unsafe { ffi::ygui_widget_set_position(self.handle, x, y) }
    }

    pub fn set_size(&self, w: f32, h: f32) {
        unsafe { ffi::ygui_widget_set_size(self.handle, w, h) }
    }

    pub fn set_visible(&self, visible: bool) {
        unsafe { ffi::ygui_widget_set_visible(self.handle, if visible { 1 } else { 0 }) }
    }

    pub fn set_enabled(&self, enabled: bool) {
        unsafe { ffi::ygui_widget_set_enabled(self.handle, if enabled { 1 } else { 0 }) }
    }

    pub fn set_bg_color(&self, color: u32) {
        unsafe { ffi::ygui_widget_set_bg_color(self.handle, color) }
    }

    pub fn add_child(&self, child: &Widget) {
        unsafe { ffi::ygui_widget_add_child(self.handle, child.handle) }
    }
}

/// Button widget
pub struct Button(Widget);

impl Button {
    pub fn set_label(&self, label: &str) {
        let c_label = CString::new(label).unwrap();
        unsafe { ffi::ygui_button_set_label(self.0.handle, c_label.as_ptr()) }
    }
}

impl std::ops::Deref for Button {
    type Target = Widget;
    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

/// Label widget
pub struct Label(Widget);

impl Label {
    pub fn set_text(&self, text: &str) {
        let c_text = CString::new(text).unwrap();
        unsafe { ffi::ygui_label_set_text(self.0.handle, c_text.as_ptr()) }
    }
}

impl std::ops::Deref for Label {
    type Target = Widget;
    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

/// Slider widget
pub struct Slider(Widget);

impl Slider {
    pub fn set_value(&self, value: f32) {
        unsafe { ffi::ygui_slider_set_value(self.0.handle, value) }
    }

    pub fn get_value(&self) -> f32 {
        unsafe { ffi::ygui_slider_get_value(self.0.handle) }
    }
}

impl std::ops::Deref for Slider {
    type Target = Widget;
    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

/// Checkbox widget
pub struct Checkbox(Widget);

impl Checkbox {
    pub fn set_checked(&self, checked: bool) {
        unsafe { ffi::ygui_checkbox_set_checked(self.0.handle, if checked { 1 } else { 0 }) }
    }

    pub fn is_checked(&self) -> bool {
        unsafe { ffi::ygui_checkbox_get_checked(self.0.handle) != 0 }
    }
}

impl std::ops::Deref for Checkbox {
    type Target = Widget;
    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

/// Panel widget
pub struct Panel(Widget);

impl Panel {
    pub fn set_content_size(&self, w: f32, h: f32) {
        unsafe { ffi::ygui_panel_set_content_size(self.0.handle, w, h) }
    }
}

impl std::ops::Deref for Panel {
    type Target = Widget;
    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

/// Progress widget
pub struct Progress(Widget);

impl Progress {
    pub fn set_value(&self, value: f32) {
        unsafe { ffi::ygui_progress_set_value(self.0.handle, value) }
    }
}

impl std::ops::Deref for Progress {
    type Target = Widget;
    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

/// Event callback type
type EventCallback = Box<dyn Fn(Event)>;

/// YGui engine
pub struct Engine {
    handle: *mut ffi::YguiEngine,
    _callback: Option<Box<EventCallback>>,
}

impl Engine {
    /// Create a new engine
    pub fn new(width: f32, height: f32) -> Option<Self> {
        let handle = unsafe { ffi::ygui_engine_create(ptr::null_mut()) };
        if handle.is_null() {
            return None;
        }

        unsafe { ffi::ygui_engine_set_size(handle, width, height) };

        Some(Engine {
            handle,
            _callback: None,
        })
    }

    /// Set engine size
    pub fn set_size(&self, width: f32, height: f32) {
        unsafe { ffi::ygui_engine_set_size(self.handle, width, height) }
    }

    /// Check if engine needs rebuild
    pub fn is_dirty(&self) -> bool {
        unsafe { ffi::ygui_engine_is_dirty(self.handle) != 0 }
    }

    /// Mark engine as dirty
    pub fn mark_dirty(&self) {
        unsafe { ffi::ygui_engine_mark_dirty(self.handle) }
    }

    /// Rebuild all widgets
    pub fn rebuild(&self) {
        unsafe { ffi::ygui_engine_rebuild(self.handle) }
    }

    /// Clear all widgets
    pub fn clear(&self) {
        unsafe { ffi::ygui_engine_clear(self.handle) }
    }

    /// Handle mouse move
    pub fn mouse_move(&self, x: f32, y: f32) {
        unsafe { ffi::ygui_engine_mouse_move(self.handle, x, y) }
    }

    /// Handle mouse down
    pub fn mouse_down(&self, x: f32, y: f32, button: i32) {
        unsafe { ffi::ygui_engine_mouse_down(self.handle, x, y, button) }
    }

    /// Handle mouse up
    pub fn mouse_up(&self, x: f32, y: f32, button: i32) {
        unsafe { ffi::ygui_engine_mouse_up(self.handle, x, y, button) }
    }

    /// Handle mouse scroll
    pub fn mouse_scroll(&self, x: f32, y: f32, dx: f32, dy: f32) {
        unsafe { ffi::ygui_engine_mouse_scroll(self.handle, x, y, dx, dy) }
    }

    /// Create a button
    pub fn button(&self, id: &str, x: f32, y: f32, w: f32, h: f32, label: &str) -> Option<Button> {
        let c_id = CString::new(id).ok()?;
        let c_label = CString::new(label).ok()?;
        let handle = unsafe {
            ffi::ygui_button(self.handle, c_id.as_ptr(), x, y, w, h, c_label.as_ptr())
        };
        Widget::new(handle).map(Button)
    }

    /// Create a label
    pub fn label(&self, id: &str, x: f32, y: f32, text: &str) -> Option<Label> {
        let c_id = CString::new(id).ok()?;
        let c_text = CString::new(text).ok()?;
        let handle = unsafe {
            ffi::ygui_label(self.handle, c_id.as_ptr(), x, y, c_text.as_ptr())
        };
        Widget::new(handle).map(Label)
    }

    /// Create a slider
    pub fn slider(
        &self,
        id: &str,
        x: f32,
        y: f32,
        w: f32,
        h: f32,
        min: f32,
        max: f32,
        value: f32,
    ) -> Option<Slider> {
        let c_id = CString::new(id).ok()?;
        let handle = unsafe {
            ffi::ygui_slider(self.handle, c_id.as_ptr(), x, y, w, h, min, max, value)
        };
        Widget::new(handle).map(Slider)
    }

    /// Create a checkbox
    pub fn checkbox(
        &self,
        id: &str,
        x: f32,
        y: f32,
        w: f32,
        h: f32,
        label: &str,
        checked: bool,
    ) -> Option<Checkbox> {
        let c_id = CString::new(id).ok()?;
        let c_label = CString::new(label).ok()?;
        let handle = unsafe {
            ffi::ygui_checkbox(
                self.handle,
                c_id.as_ptr(),
                x,
                y,
                w,
                h,
                c_label.as_ptr(),
                if checked { 1 } else { 0 },
            )
        };
        Widget::new(handle).map(Checkbox)
    }

    /// Create a panel
    pub fn panel(&self, id: &str, x: f32, y: f32, w: f32, h: f32) -> Option<Panel> {
        let c_id = CString::new(id).ok()?;
        let handle = unsafe { ffi::ygui_panel(self.handle, c_id.as_ptr(), x, y, w, h) };
        Widget::new(handle).map(Panel)
    }

    /// Create a progress bar
    pub fn progress(&self, id: &str, x: f32, y: f32, w: f32, h: f32, value: f32) -> Option<Progress> {
        let c_id = CString::new(id).ok()?;
        let handle = unsafe { ffi::ygui_progress(self.handle, c_id.as_ptr(), x, y, w, h, value) };
        Widget::new(handle).map(Progress)
    }
}

impl Drop for Engine {
    fn drop(&mut self) {
        if !self.handle.is_null() {
            unsafe { ffi::ygui_engine_destroy(self.handle) }
        }
    }
}

/// Get YGui version
pub fn version() -> String {
    unsafe {
        let ptr = ffi::ygui_version();
        if ptr.is_null() {
            return String::new();
        }
        CStr::from_ptr(ptr).to_string_lossy().into_owned()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_version() {
        // This will fail without the library, but shows the API
        // let v = version();
        // assert!(!v.is_empty());
    }
}
