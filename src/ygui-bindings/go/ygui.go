// Package ygui provides Go bindings for the YGui-C widget library.
package ygui

/*
#cgo LDFLAGS: -lygui
#include <stdlib.h>
#include "ygui.h"

// Event callback wrapper
extern void goEventCallback(ygui_event_t* event, void* userdata);
*/
import "C"
import (
	"sync"
	"unsafe"
)

// WidgetType represents widget types
type WidgetType int

const (
	WidgetButton WidgetType = iota
	WidgetLabel
	WidgetSlider
	WidgetCheckbox
	WidgetTextInput
	WidgetPanel
	WidgetHBox
	WidgetVBox
	WidgetDropdown
	WidgetListBox
	WidgetTable
	WidgetTabBar
	WidgetColorPicker
	WidgetScrollArea
	WidgetProgress
	WidgetSeparator
	WidgetCustom
)

// EventType represents event types
type EventType int

const (
	EventNone EventType = iota
	EventClick
	EventPress
	EventRelease
	EventChange
	EventScroll
	EventFocus
	EventBlur
	EventKey
	EventText
)

// Event represents a widget event
type Event struct {
	WidgetID   string
	Type       EventType
	FloatValue float32
	IntValue   int32
	BoolValue  bool
}

// EventCallback is the callback function type
type EventCallback func(Event)

// Widget represents a widget handle
type Widget struct {
	handle *C.ygui_widget_t
}

// SetPosition sets widget position
func (w *Widget) SetPosition(x, y float32) {
	C.ygui_widget_set_position(w.handle, C.float(x), C.float(y))
}

// SetSize sets widget size
func (w *Widget) SetSize(width, height float32) {
	C.ygui_widget_set_size(w.handle, C.float(width), C.float(height))
}

// SetVisible sets widget visibility
func (w *Widget) SetVisible(visible bool) {
	v := C.int(0)
	if visible {
		v = 1
	}
	C.ygui_widget_set_visible(w.handle, v)
}

// SetEnabled sets widget enabled state
func (w *Widget) SetEnabled(enabled bool) {
	e := C.int(0)
	if enabled {
		e = 1
	}
	C.ygui_widget_set_enabled(w.handle, e)
}

// SetBgColor sets background color
func (w *Widget) SetBgColor(color uint32) {
	C.ygui_widget_set_bg_color(w.handle, C.uint32_t(color))
}

// AddChild adds a child widget
func (w *Widget) AddChild(child *Widget) {
	C.ygui_widget_add_child(w.handle, child.handle)
}

// Remove removes the widget
func (w *Widget) Remove() {
	C.ygui_widget_remove(w.handle)
	w.handle = nil
}

// Button is a button widget
type Button struct {
	Widget
}

// SetLabel sets button label
func (b *Button) SetLabel(label string) {
	cLabel := C.CString(label)
	defer C.free(unsafe.Pointer(cLabel))
	C.ygui_button_set_label(b.handle, cLabel)
}

// Label is a label widget
type Label struct {
	Widget
}

// SetText sets label text
func (l *Label) SetText(text string) {
	cText := C.CString(text)
	defer C.free(unsafe.Pointer(cText))
	C.ygui_label_set_text(l.handle, cText)
}

// Slider is a slider widget
type Slider struct {
	Widget
}

// SetValue sets slider value
func (s *Slider) SetValue(value float32) {
	C.ygui_slider_set_value(s.handle, C.float(value))
}

// Value returns slider value
func (s *Slider) Value() float32 {
	return float32(C.ygui_slider_get_value(s.handle))
}

// Checkbox is a checkbox widget
type Checkbox struct {
	Widget
}

// SetChecked sets checkbox state
func (c *Checkbox) SetChecked(checked bool) {
	v := C.int(0)
	if checked {
		v = 1
	}
	C.ygui_checkbox_set_checked(c.handle, v)
}

// Checked returns checkbox state
func (c *Checkbox) Checked() bool {
	return C.ygui_checkbox_get_checked(c.handle) != 0
}

// Panel is a panel widget
type Panel struct {
	Widget
}

// SetContentSize sets panel content size
func (p *Panel) SetContentSize(w, h float32) {
	C.ygui_panel_set_content_size(p.handle, C.float(w), C.float(h))
}

// Progress is a progress bar widget
type Progress struct {
	Widget
}

// SetValue sets progress value (0-1)
func (p *Progress) SetValue(value float32) {
	C.ygui_progress_set_value(p.handle, C.float(value))
}

// Engine is the YGui engine
type Engine struct {
	handle   *C.ygui_engine_t
	callback EventCallback
	mu       sync.Mutex
}

var (
	engineCallbacks = make(map[uintptr]EventCallback)
	callbackMu      sync.Mutex
)

//export goEventCallback
func goEventCallback(event *C.ygui_event_t, userdata unsafe.Pointer) {
	enginePtr := uintptr(userdata)
	callbackMu.Lock()
	cb, ok := engineCallbacks[enginePtr]
	callbackMu.Unlock()

	if ok && cb != nil {
		e := Event{
			Type:       EventType(event._type),
			FloatValue: float32(event.data.float_value),
		}
		if event.widget_id != nil {
			e.WidgetID = C.GoString(event.widget_id)
		}
		cb(e)
	}
}

// NewEngine creates a new engine
func NewEngine(width, height float32) *Engine {
	handle := C.ygui_engine_create(nil)
	if handle == nil {
		return nil
	}

	C.ygui_engine_set_size(handle, C.float(width), C.float(height))

	return &Engine{handle: handle}
}

// Destroy destroys the engine
func (e *Engine) Destroy() {
	if e.handle != nil {
		callbackMu.Lock()
		delete(engineCallbacks, uintptr(unsafe.Pointer(e.handle)))
		callbackMu.Unlock()

		C.ygui_engine_destroy(e.handle)
		e.handle = nil
	}
}

// SetSize sets engine size
func (e *Engine) SetSize(width, height float32) {
	C.ygui_engine_set_size(e.handle, C.float(width), C.float(height))
}

// OnEvent sets the event callback
func (e *Engine) OnEvent(callback EventCallback) {
	e.mu.Lock()
	defer e.mu.Unlock()

	e.callback = callback

	callbackMu.Lock()
	engineCallbacks[uintptr(unsafe.Pointer(e.handle))] = callback
	callbackMu.Unlock()

	C.ygui_engine_set_event_callback(
		e.handle,
		(C.ygui_event_callback_t)(C.goEventCallback),
		unsafe.Pointer(e.handle),
	)
}

// IsDirty returns true if engine needs rebuild
func (e *Engine) IsDirty() bool {
	return C.ygui_engine_is_dirty(e.handle) != 0
}

// MarkDirty marks engine as dirty
func (e *Engine) MarkDirty() {
	C.ygui_engine_mark_dirty(e.handle)
}

// Rebuild rebuilds all widgets
func (e *Engine) Rebuild() {
	C.ygui_engine_rebuild(e.handle)
}

// Clear clears all widgets
func (e *Engine) Clear() {
	C.ygui_engine_clear(e.handle)
}

// MouseMove handles mouse move
func (e *Engine) MouseMove(x, y float32) {
	C.ygui_engine_mouse_move(e.handle, C.float(x), C.float(y))
}

// MouseDown handles mouse down
func (e *Engine) MouseDown(x, y float32, button int) {
	C.ygui_engine_mouse_down(e.handle, C.float(x), C.float(y), C.int(button))
}

// MouseUp handles mouse up
func (e *Engine) MouseUp(x, y float32, button int) {
	C.ygui_engine_mouse_up(e.handle, C.float(x), C.float(y), C.int(button))
}

// MouseScroll handles mouse scroll
func (e *Engine) MouseScroll(x, y, dx, dy float32) {
	C.ygui_engine_mouse_scroll(e.handle, C.float(x), C.float(y), C.float(dx), C.float(dy))
}

// Button creates a button widget
func (e *Engine) Button(id string, x, y, w, h float32, label string) *Button {
	cID := C.CString(id)
	defer C.free(unsafe.Pointer(cID))
	cLabel := C.CString(label)
	defer C.free(unsafe.Pointer(cLabel))

	handle := C.ygui_button(e.handle, cID, C.float(x), C.float(y), C.float(w), C.float(h), cLabel)
	if handle == nil {
		return nil
	}
	return &Button{Widget{handle: handle}}
}

// Label creates a label widget
func (e *Engine) Label(id string, x, y float32, text string) *Label {
	cID := C.CString(id)
	defer C.free(unsafe.Pointer(cID))
	cText := C.CString(text)
	defer C.free(unsafe.Pointer(cText))

	handle := C.ygui_label(e.handle, cID, C.float(x), C.float(y), cText)
	if handle == nil {
		return nil
	}
	return &Label{Widget{handle: handle}}
}

// Slider creates a slider widget
func (e *Engine) Slider(id string, x, y, w, h, min, max, value float32) *Slider {
	cID := C.CString(id)
	defer C.free(unsafe.Pointer(cID))

	handle := C.ygui_slider(e.handle, cID, C.float(x), C.float(y), C.float(w), C.float(h),
		C.float(min), C.float(max), C.float(value))
	if handle == nil {
		return nil
	}
	return &Slider{Widget{handle: handle}}
}

// Checkbox creates a checkbox widget
func (e *Engine) Checkbox(id string, x, y, w, h float32, label string, checked bool) *Checkbox {
	cID := C.CString(id)
	defer C.free(unsafe.Pointer(cID))
	cLabel := C.CString(label)
	defer C.free(unsafe.Pointer(cLabel))

	c := C.int(0)
	if checked {
		c = 1
	}

	handle := C.ygui_checkbox(e.handle, cID, C.float(x), C.float(y), C.float(w), C.float(h), cLabel, c)
	if handle == nil {
		return nil
	}
	return &Checkbox{Widget{handle: handle}}
}

// Panel creates a panel widget
func (e *Engine) Panel(id string, x, y, w, h float32) *Panel {
	cID := C.CString(id)
	defer C.free(unsafe.Pointer(cID))

	handle := C.ygui_panel(e.handle, cID, C.float(x), C.float(y), C.float(w), C.float(h))
	if handle == nil {
		return nil
	}
	return &Panel{Widget{handle: handle}}
}

// Progress creates a progress bar widget
func (e *Engine) Progress(id string, x, y, w, h, value float32) *Progress {
	cID := C.CString(id)
	defer C.free(unsafe.Pointer(cID))

	handle := C.ygui_progress(e.handle, cID, C.float(x), C.float(y), C.float(w), C.float(h), C.float(value))
	if handle == nil {
		return nil
	}
	return &Progress{Widget{handle: handle}}
}

// Version returns the YGui version
func Version() string {
	return C.GoString(C.ygui_version())
}
