"""
YGui Python Bindings

Pure Python bindings for YGui-C using ctypes.
Includes OSC output for yetty terminal.
"""

import ctypes
from ctypes import (
    c_void_p, c_char_p, c_float, c_int, c_uint32, c_int32, c_uint8,
    POINTER, Structure, Union, CFUNCTYPE, byref
)
from typing import Callable, Optional, List, Tuple
from enum import IntEnum
import os
import sys
import base64

#=============================================================================
# OSC output helpers for yetty terminal
#=============================================================================

VENDOR_ID = 666666

def _write_osc(seq: bytes):
    """Write OSC sequence to stdout."""
    sys.stdout.buffer.write(seq)
    sys.stdout.buffer.flush()

def osc_create_ydraw(name: str, x: int, y: int, w: int, h: int, data: bytes):
    """Send OSC command to create a ydraw card."""
    b64 = base64.b64encode(data).decode('ascii')
    seq = f"\033]{VENDOR_ID};run -c ydraw -x {x} -y {y} -w {w} -h {h} -r --name {name};;{b64}\033\\".encode()
    _write_osc(seq)

def osc_update_ydraw(name: str, data: bytes):
    """Send OSC command to update a ydraw card."""
    b64 = base64.b64encode(data).decode('ascii')
    seq = f"\033]{VENDOR_ID};update --name {name};;{b64}\033\\".encode()
    _write_osc(seq)

def osc_kill(name: str):
    """Send OSC command to kill a card."""
    seq = f"\033]{VENDOR_ID};kill --name {name}\033\\".encode()
    _write_osc(seq)

#=============================================================================
# Library loading
#=============================================================================

def _load_library():
    """Load libygui.so from common locations."""
    search_paths = [
        os.path.join(os.path.dirname(__file__), "libygui.so"),
        os.path.join(os.path.dirname(__file__), "..", "..", "build", "libygui.so"),
        "libygui.so",
    ]
    for path in search_paths:
        try:
            return ctypes.CDLL(path)
        except OSError:
            continue
    raise ImportError("Could not load libygui.so")

_lib = None

def _get_lib():
    global _lib
    if _lib is None:
        _lib = _load_library()
        _setup_functions(_lib)
    return _lib


#=============================================================================
# Enums
#=============================================================================

class WidgetType(IntEnum):
    BUTTON = 0
    LABEL = 1
    SLIDER = 2
    CHECKBOX = 3
    TEXTINPUT = 4
    PANEL = 5
    HBOX = 6
    VBOX = 7
    DROPDOWN = 8
    LISTBOX = 9
    TABLE = 10
    TABBAR = 11
    COLORPICKER = 12
    SCROLLAREA = 13
    PROGRESS = 14
    SEPARATOR = 15
    CUSTOM = 16


class EventType(IntEnum):
    NONE = 0
    CLICK = 1
    PRESS = 2
    RELEASE = 3
    CHANGE = 4
    SCROLL = 5
    FOCUS = 6
    BLUR = 7
    KEY = 8
    TEXT = 9


class Flags(IntEnum):
    NONE = 0
    HOVER = 1 << 0
    PRESSED = 1 << 1
    FOCUSED = 1 << 2
    DISABLED = 1 << 3
    CHECKED = 1 << 4
    OPEN = 1 << 5
    VISIBLE = 1 << 6


#=============================================================================
# Ctypes structures
#=============================================================================

class _ColorData(Structure):
    _fields_ = [("r", c_float), ("g", c_float), ("b", c_float), ("a", c_float)]

class _ScrollData(Structure):
    _fields_ = [("x", c_float), ("y", c_float)]

class _KeyData(Structure):
    _fields_ = [("key", c_uint32), ("mods", c_int)]

class _EventDataUnion(Union):
    _fields_ = [
        ("float_value", c_float),
        ("int_value", c_int32),
        ("bool_value", c_int),
        ("string_value", c_char_p),
        ("color", _ColorData),
        ("scroll", _ScrollData),
        ("key", _KeyData),
    ]

class _YGuiEvent(Structure):
    _fields_ = [
        ("widget_id", c_char_p),
        ("type", c_int),
        ("data", _EventDataUnion),
    ]


_EventCallbackType = CFUNCTYPE(None, POINTER(_YGuiEvent), c_void_p)


def _setup_functions(lib):
    """Set up ctypes function signatures."""

    # Library init/shutdown
    lib.ygui_init.argtypes = []
    lib.ygui_init.restype = c_int

    lib.ygui_shutdown.argtypes = []
    lib.ygui_shutdown.restype = None

    # YDraw buffer
    lib.ydraw_buffer_create.argtypes = []
    lib.ydraw_buffer_create.restype = c_void_p

    lib.ydraw_buffer_destroy.argtypes = [c_void_p]
    lib.ydraw_buffer_destroy.restype = None

    lib.ydraw_buffer_clear.argtypes = [c_void_p]
    lib.ydraw_buffer_clear.restype = None

    lib.ydraw_buffer_serialize.argtypes = [c_void_p, POINTER(POINTER(c_uint8))]
    lib.ydraw_buffer_serialize.restype = c_uint32

    # Engine
    lib.ygui_engine_create.argtypes = [c_void_p]
    lib.ygui_engine_create.restype = c_void_p

    lib.ygui_engine_destroy.argtypes = [c_void_p]
    lib.ygui_engine_destroy.restype = None

    lib.ygui_engine_set_size.argtypes = [c_void_p, c_float, c_float]
    lib.ygui_engine_set_size.restype = None

    lib.ygui_engine_set_event_callback.argtypes = [c_void_p, _EventCallbackType, c_void_p]
    lib.ygui_engine_set_event_callback.restype = None

    lib.ygui_engine_is_dirty.argtypes = [c_void_p]
    lib.ygui_engine_is_dirty.restype = c_int

    lib.ygui_engine_mark_dirty.argtypes = [c_void_p]
    lib.ygui_engine_mark_dirty.restype = None

    lib.ygui_engine_rebuild.argtypes = [c_void_p]
    lib.ygui_engine_rebuild.restype = None

    lib.ygui_engine_clear.argtypes = [c_void_p]
    lib.ygui_engine_clear.restype = None

    lib.ygui_engine_mouse_move.argtypes = [c_void_p, c_float, c_float]
    lib.ygui_engine_mouse_move.restype = None

    lib.ygui_engine_mouse_down.argtypes = [c_void_p, c_float, c_float, c_int]
    lib.ygui_engine_mouse_down.restype = None

    lib.ygui_engine_mouse_up.argtypes = [c_void_p, c_float, c_float, c_int]
    lib.ygui_engine_mouse_up.restype = None

    lib.ygui_engine_mouse_scroll.argtypes = [c_void_p, c_float, c_float, c_float, c_float]
    lib.ygui_engine_mouse_scroll.restype = None

    lib.ygui_engine_find.argtypes = [c_void_p, c_char_p]
    lib.ygui_engine_find.restype = c_void_p

    # Event loop
    lib.ygui_engine_subscribe_clicks.argtypes = [c_void_p, c_int]
    lib.ygui_engine_subscribe_clicks.restype = None

    lib.ygui_engine_subscribe_moves.argtypes = [c_void_p, c_int]
    lib.ygui_engine_subscribe_moves.restype = None

    lib.ygui_engine_poll.argtypes = [c_void_p, c_int]
    lib.ygui_engine_poll.restype = c_int

    lib.ygui_engine_run.argtypes = [c_void_p]
    lib.ygui_engine_run.restype = None

    lib.ygui_engine_stop.argtypes = [c_void_p]
    lib.ygui_engine_stop.restype = None

    # Widgets
    lib.ygui_button.argtypes = [c_void_p, c_char_p, c_float, c_float, c_float, c_float, c_char_p]
    lib.ygui_button.restype = c_void_p

    lib.ygui_label.argtypes = [c_void_p, c_char_p, c_float, c_float, c_char_p]
    lib.ygui_label.restype = c_void_p

    lib.ygui_slider.argtypes = [c_void_p, c_char_p, c_float, c_float, c_float, c_float, c_float, c_float, c_float]
    lib.ygui_slider.restype = c_void_p

    lib.ygui_checkbox.argtypes = [c_void_p, c_char_p, c_float, c_float, c_float, c_float, c_char_p, c_int]
    lib.ygui_checkbox.restype = c_void_p

    lib.ygui_panel.argtypes = [c_void_p, c_char_p, c_float, c_float, c_float, c_float]
    lib.ygui_panel.restype = c_void_p

    lib.ygui_progress.argtypes = [c_void_p, c_char_p, c_float, c_float, c_float, c_float, c_float]
    lib.ygui_progress.restype = c_void_p

    # Widget hierarchy
    lib.ygui_widget_add_child.argtypes = [c_void_p, c_void_p]
    lib.ygui_widget_add_child.restype = None

    lib.ygui_widget_remove.argtypes = [c_void_p]
    lib.ygui_widget_remove.restype = None

    # Widget properties
    lib.ygui_widget_set_position.argtypes = [c_void_p, c_float, c_float]
    lib.ygui_widget_set_position.restype = None

    lib.ygui_widget_set_size.argtypes = [c_void_p, c_float, c_float]
    lib.ygui_widget_set_size.restype = None

    lib.ygui_widget_set_visible.argtypes = [c_void_p, c_int]
    lib.ygui_widget_set_visible.restype = None

    lib.ygui_widget_set_enabled.argtypes = [c_void_p, c_int]
    lib.ygui_widget_set_enabled.restype = None

    lib.ygui_widget_set_bg_color.argtypes = [c_void_p, c_uint32]
    lib.ygui_widget_set_bg_color.restype = None

    lib.ygui_slider_set_value.argtypes = [c_void_p, c_float]
    lib.ygui_slider_set_value.restype = None

    lib.ygui_slider_get_value.argtypes = [c_void_p]
    lib.ygui_slider_get_value.restype = c_float

    lib.ygui_checkbox_set_checked.argtypes = [c_void_p, c_int]
    lib.ygui_checkbox_set_checked.restype = None

    lib.ygui_checkbox_get_checked.argtypes = [c_void_p]
    lib.ygui_checkbox_get_checked.restype = c_int

    lib.ygui_progress_set_value.argtypes = [c_void_p, c_float]
    lib.ygui_progress_set_value.restype = None

    lib.ygui_button_set_label.argtypes = [c_void_p, c_char_p]
    lib.ygui_button_set_label.restype = None

    lib.ygui_label_set_text.argtypes = [c_void_p, c_char_p]
    lib.ygui_label_set_text.restype = None

    lib.ygui_panel_set_content_size.argtypes = [c_void_p, c_float, c_float]
    lib.ygui_panel_set_content_size.restype = None

    # Theme
    lib.ygui_theme_create_default.argtypes = []
    lib.ygui_theme_create_default.restype = c_void_p

    lib.ygui_theme_destroy.argtypes = [c_void_p]
    lib.ygui_theme_destroy.restype = None

    # Version
    lib.ygui_version.argtypes = []
    lib.ygui_version.restype = c_char_p


#=============================================================================
# YDraw Buffer wrapper
#=============================================================================

class YDrawBuffer:
    """YDraw buffer for primitives."""

    def __init__(self):
        self._lib = _get_lib()
        self._handle = self._lib.ydraw_buffer_create()
        if not self._handle:
            raise RuntimeError("Failed to create YDraw buffer")

    def __del__(self):
        if hasattr(self, "_handle") and self._handle:
            self._lib.ydraw_buffer_destroy(self._handle)

    @property
    def handle(self):
        return self._handle

    def clear(self):
        self._lib.ydraw_buffer_clear(self._handle)

    def serialize(self) -> bytes:
        """Serialize buffer to binary format for OSC transport."""
        data_ptr = POINTER(c_uint8)()
        size = self._lib.ydraw_buffer_serialize(self._handle, byref(data_ptr))
        if size == 0 or not data_ptr:
            return b""
        return bytes(data_ptr[:size])


#=============================================================================
# Event wrapper
#=============================================================================

class Event:
    """Widget event."""

    def __init__(self, widget_id: str, event_type: EventType, data=None):
        self.widget_id = widget_id
        self.type = event_type
        self.data = data

    @classmethod
    def _from_c(cls, c_event: _YGuiEvent) -> "Event":
        widget_id = c_event.widget_id.decode("utf-8") if c_event.widget_id else ""
        event_type = EventType(c_event.type)

        data = None
        if event_type == EventType.CHANGE:
            data = c_event.data.float_value
        elif event_type == EventType.SCROLL:
            data = (c_event.data.scroll.x, c_event.data.scroll.y)

        return cls(widget_id, event_type, data)

    def __repr__(self):
        return f"Event({self.widget_id!r}, {self.type.name}, {self.data!r})"


#=============================================================================
# Widget wrappers
#=============================================================================

class Widget:
    """Base widget wrapper."""

    def __init__(self, handle: c_void_p):
        self._handle = handle
        self._lib = _get_lib()

    @property
    def handle(self):
        return self._handle

    def set_position(self, x: float, y: float):
        self._lib.ygui_widget_set_position(self._handle, x, y)

    def set_size(self, w: float, h: float):
        self._lib.ygui_widget_set_size(self._handle, w, h)

    def set_visible(self, visible: bool):
        self._lib.ygui_widget_set_visible(self._handle, 1 if visible else 0)

    def set_enabled(self, enabled: bool):
        self._lib.ygui_widget_set_enabled(self._handle, 1 if enabled else 0)

    def set_bg_color(self, color: int):
        self._lib.ygui_widget_set_bg_color(self._handle, color)

    def add_child(self, child: "Widget"):
        self._lib.ygui_widget_add_child(self._handle, child._handle)

    def remove(self):
        self._lib.ygui_widget_remove(self._handle)
        self._handle = None


class Button(Widget):
    """Button widget."""

    def set_label(self, label: str):
        self._lib.ygui_button_set_label(self._handle, label.encode("utf-8"))


class Label(Widget):
    """Label widget."""

    def set_text(self, text):
        if isinstance(text, str):
            text = text.encode("utf-8")
        self._lib.ygui_label_set_text(self._handle, text)


class Slider(Widget):
    """Slider widget."""

    def set_value(self, value: float):
        self._lib.ygui_slider_set_value(self._handle, value)

    def get_value(self) -> float:
        return self._lib.ygui_slider_get_value(self._handle)

    value = property(get_value, set_value)


class Checkbox(Widget):
    """Checkbox widget."""

    def set_checked(self, checked: bool):
        self._lib.ygui_checkbox_set_checked(self._handle, 1 if checked else 0)

    def get_checked(self) -> bool:
        return bool(self._lib.ygui_checkbox_get_checked(self._handle))

    checked = property(get_checked, set_checked)


class Panel(Widget):
    """Panel widget."""

    def set_content_size(self, w: float, h: float):
        self._lib.ygui_panel_set_content_size(self._handle, w, h)


class Progress(Widget):
    """Progress bar widget."""

    def set_value(self, value: float):
        self._lib.ygui_progress_set_value(self._handle, value)


#=============================================================================
# Engine wrapper
#=============================================================================

class Engine:
    """YGui engine with OSC output support."""

    def __init__(self, buffer: Optional[YDrawBuffer] = None,
                 width: float = 800, height: float = 600,
                 name: str = "ygui-app",
                 x: int = 0, y: int = 0, w: int = 80, h: int = 24):
        """
        Create YGui engine.

        Args:
            buffer: Optional YDrawBuffer (created automatically if not provided)
            width: Canvas width in pixels
            height: Canvas height in pixels
            name: Card name for OSC commands
            x, y: Card position in terminal cells
            w, h: Card size in terminal cells
        """
        self._lib = _get_lib()

        # Create or use provided buffer
        if buffer is None:
            self._buffer = YDrawBuffer()
            self._owns_buffer = True
        else:
            self._buffer = buffer
            self._owns_buffer = False

        self._handle = self._lib.ygui_engine_create(self._buffer.handle)
        if not self._handle:
            raise RuntimeError("Failed to create YGui engine")

        self._lib.ygui_engine_set_size(self._handle, width, height)

        self._callback = None
        self._callback_fn = None
        self._widgets = {}

        # OSC settings
        self._name = name
        self._x = x
        self._y = y
        self._w = w
        self._h = h
        self._created = False

    def __del__(self):
        if hasattr(self, "_handle") and self._handle:
            self._lib.ygui_engine_destroy(self._handle)

    def set_size(self, width: float, height: float):
        self._lib.ygui_engine_set_size(self._handle, width, height)

    def on_event(self, callback: Callable[[Event], None]):
        """Set event callback."""
        def c_callback(event_ptr, userdata):
            if event_ptr:
                event = Event._from_c(event_ptr.contents)
                callback(event)

        self._callback_fn = _EventCallbackType(c_callback)
        self._lib.ygui_engine_set_event_callback(self._handle, self._callback_fn, None)
        self._callback = callback

    def is_dirty(self) -> bool:
        return bool(self._lib.ygui_engine_is_dirty(self._handle))

    def mark_dirty(self):
        self._lib.ygui_engine_mark_dirty(self._handle)

    def render(self):
        """Render a frame: clear buffer → rebuild UI → send to terminal."""
        self._buffer.clear()
        self._lib.ygui_engine_rebuild(self._handle)
        self._send_osc()

    def rebuild(self):
        """Alias for render() for backwards compatibility."""
        self.render()

    def _send_osc(self):
        """Send current buffer to terminal via OSC."""
        data = self._buffer.serialize()
        if not data:
            return

        if not self._created:
            osc_create_ydraw(self._name, self._x, self._y, self._w, self._h, data)
            self._created = True
        else:
            osc_update_ydraw(self._name, data)

    def kill(self):
        """Kill the card in the terminal."""
        if self._created:
            osc_kill(self._name)
            self._created = False

    def clear(self):
        self._lib.ygui_engine_clear(self._handle)
        self._widgets.clear()

    def mouse_move(self, x: float, y: float):
        self._lib.ygui_engine_mouse_move(self._handle, x, y)

    def mouse_down(self, x: float, y: float, button: int = 0):
        self._lib.ygui_engine_mouse_down(self._handle, x, y, button)

    def mouse_up(self, x: float, y: float, button: int = 0):
        self._lib.ygui_engine_mouse_up(self._handle, x, y, button)

    def mouse_scroll(self, x: float, y: float, dx: float, dy: float):
        self._lib.ygui_engine_mouse_scroll(self._handle, x, y, dx, dy)

    def subscribe_clicks(self, enable: bool = True):
        """Subscribe to card click events (sends DEC mode 1500)."""
        self._lib.ygui_engine_subscribe_clicks(self._handle, 1 if enable else 0)

    def subscribe_moves(self, enable: bool = True):
        """Subscribe to card move events (sends DEC mode 1501)."""
        self._lib.ygui_engine_subscribe_moves(self._handle, 1 if enable else 0)

    def poll(self, timeout_ms: int = 50) -> int:
        """Poll for events. Returns 1 if event processed, 0 if timeout, -1 on error."""
        return self._lib.ygui_engine_poll(self._handle, timeout_ms)

    def run(self):
        """Run event loop until stop() called or 'q' pressed."""
        self._lib.ygui_engine_run(self._handle)

    def stop(self):
        """Stop the event loop."""
        self._lib.ygui_engine_stop(self._handle)

    def find(self, widget_id: str) -> Optional[Widget]:
        handle = self._lib.ygui_engine_find(self._handle, widget_id.encode("utf-8"))
        if handle:
            return self._widgets.get(id(handle), Widget(handle))
        return None

    # Widget creation methods
    def button(self, id: str, x: float, y: float, w: float, h: float, label: str) -> Button:
        handle = self._lib.ygui_button(
            self._handle, id.encode("utf-8"), x, y, w, h, label.encode("utf-8"))
        btn = Button(handle)
        self._widgets[id] = btn
        return btn

    def label(self, id: str, x: float, y: float, text: str) -> Label:
        handle = self._lib.ygui_label(
            self._handle, id.encode("utf-8"), x, y, text.encode("utf-8"))
        lbl = Label(handle)
        self._widgets[id] = lbl
        return lbl

    def slider(self, id: str, x: float, y: float, w: float, h: float,
               min_val: float = 0, max_val: float = 100, value: float = 50) -> Slider:
        handle = self._lib.ygui_slider(
            self._handle, id.encode("utf-8"), x, y, w, h, min_val, max_val, value)
        sld = Slider(handle)
        self._widgets[id] = sld
        return sld

    def checkbox(self, id: str, x: float, y: float, w: float, h: float,
                 label: str, checked: bool = False) -> Checkbox:
        handle = self._lib.ygui_checkbox(
            self._handle, id.encode("utf-8"), x, y, w, h,
            label.encode("utf-8"), 1 if checked else 0)
        chk = Checkbox(handle)
        self._widgets[id] = chk
        return chk

    def panel(self, id: str, x: float, y: float, w: float, h: float) -> Panel:
        handle = self._lib.ygui_panel(
            self._handle, id.encode("utf-8"), x, y, w, h)
        pnl = Panel(handle)
        self._widgets[id] = pnl
        return pnl

    def progress(self, id: str, x: float, y: float, w: float, h: float,
                 value: float = 0) -> Progress:
        handle = self._lib.ygui_progress(
            self._handle, id.encode("utf-8"), x, y, w, h, value)
        prg = Progress(handle)
        self._widgets[id] = prg
        return prg


def init() -> bool:
    """Initialize the library (sets up raw terminal mode).
    Must be called before using event loop functions.
    Returns True on success, False on error."""
    return _get_lib().ygui_init() == 0


def shutdown():
    """Shutdown the library (restores terminal settings)."""
    _get_lib().ygui_shutdown()


def version() -> str:
    """Get YGui version string."""
    return _get_lib().ygui_version().decode("utf-8")


# Example usage
if __name__ == "__main__":
    print(f"YGui version: {version()}")

    engine = Engine(width=800, height=600, name="test-app", w=40, h=20)

    def handle_event(event):
        print(f"Event: {event}")

    engine.on_event(handle_event)

    # Create widgets
    btn = engine.button("ok", 10, 10, 80, 30, "OK")
    slider = engine.slider("volume", 10, 50, 200, 24, 0, 100, 50)
    checkbox = engine.checkbox("mute", 10, 90, 150, 24, "Mute", False)

    # Build and send to terminal
    engine.rebuild()

    print("Done!")
