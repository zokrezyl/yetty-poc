"""
YGui Python Bindings

Pure Python bindings for YGui-C using ctypes.
The library handles OSC output internally - no manual OSC code needed.
"""

import ctypes
from ctypes import (
    c_void_p, c_char_p, c_float, c_int, c_uint32, c_int32, c_uint8,
    POINTER, Structure, Union, CFUNCTYPE, byref
)
from typing import Callable, Optional
from enum import IntEnum
import os

#=============================================================================
# Library loading
#=============================================================================

def _load_library():
    """Load libygui-c.a or find the built library."""
    # Look for the static library linked into a shared wrapper, or system lib
    search_paths = [
        # Build directory (most common during development)
        os.path.join(os.path.dirname(__file__), "..", "..", "..", "build-desktop-ytrace-release", "src", "yetty", "ygui-c", "libygui-c.a"),
        os.path.join(os.path.dirname(__file__), "..", "..", "build", "libygui-c.a"),
        os.path.join(os.path.dirname(__file__), "libygui.so"),
        "libygui.so",
    ]
    for path in search_paths:
        try:
            return ctypes.CDLL(path)
        except OSError:
            continue
    raise ImportError("Could not load ygui library")

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


# Callback types
_EventCallbackType = CFUNCTYPE(None, POINTER(_YGuiEvent), c_void_p)
_ClickCallbackType = CFUNCTYPE(None, c_void_p, c_void_p)
_ChangeCallbackType = CFUNCTYPE(None, c_void_p, c_float, c_void_p)
_CheckCallbackType = CFUNCTYPE(None, c_void_p, c_int, c_void_p)
_TextCallbackType = CFUNCTYPE(None, c_void_p, c_char_p, c_void_p)
_KeyCallbackType = CFUNCTYPE(None, c_void_p, c_uint32, c_int, c_void_p)


def _setup_functions(lib):
    """Set up ctypes function signatures."""

    # Library init/shutdown
    lib.ygui_init.argtypes = []
    lib.ygui_init.restype = c_int

    lib.ygui_shutdown.argtypes = []
    lib.ygui_shutdown.restype = None

    # Engine - NEW API
    lib.ygui_engine_create.argtypes = [c_char_p, c_float, c_float]
    lib.ygui_engine_create.restype = c_void_p

    lib.ygui_engine_destroy.argtypes = [c_void_p]
    lib.ygui_engine_destroy.restype = None

    lib.ygui_engine_show.argtypes = [c_void_p, c_int, c_int, c_int, c_int]
    lib.ygui_engine_show.restype = None

    lib.ygui_engine_render.argtypes = [c_void_p]
    lib.ygui_engine_render.restype = None

    lib.ygui_engine_run.argtypes = [c_void_p]
    lib.ygui_engine_run.restype = None

    lib.ygui_engine_stop.argtypes = [c_void_p]
    lib.ygui_engine_stop.restype = None

    lib.ygui_engine_set_size.argtypes = [c_void_p, c_float, c_float]
    lib.ygui_engine_set_size.restype = None

    lib.ygui_engine_is_dirty.argtypes = [c_void_p]
    lib.ygui_engine_is_dirty.restype = c_int

    lib.ygui_engine_mark_dirty.argtypes = [c_void_p]
    lib.ygui_engine_mark_dirty.restype = None

    lib.ygui_engine_on_key.argtypes = [c_void_p, _KeyCallbackType, c_void_p]
    lib.ygui_engine_on_key.restype = None

    lib.ygui_engine_set_event_callback.argtypes = [c_void_p, _EventCallbackType, c_void_p]
    lib.ygui_engine_set_event_callback.restype = None

    lib.ygui_engine_find.argtypes = [c_void_p, c_char_p]
    lib.ygui_engine_find.restype = c_void_p

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

    # Widget callbacks
    lib.ygui_button_on_click.argtypes = [c_void_p, _ClickCallbackType, c_void_p]
    lib.ygui_button_on_click.restype = None

    lib.ygui_slider_on_change.argtypes = [c_void_p, _ChangeCallbackType, c_void_p]
    lib.ygui_slider_on_change.restype = None

    lib.ygui_checkbox_on_change.argtypes = [c_void_p, _CheckCallbackType, c_void_p]
    lib.ygui_checkbox_on_change.restype = None

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
# Widget wrappers
#=============================================================================

class Widget:
    """Base widget wrapper."""

    def __init__(self, handle: c_void_p, engine: "Engine"):
        self._handle = handle
        self._engine = engine
        self._lib = _get_lib()
        self._callbacks = []  # prevent GC

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

    def on_click(self, callback: Callable[[], None]):
        """Set click callback."""
        def c_callback(widget, userdata):
            callback()
        cb = _ClickCallbackType(c_callback)
        self._callbacks.append(cb)
        self._lib.ygui_button_on_click(self._handle, cb, None)


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

    def on_change(self, callback: Callable[[float], None]):
        """Set change callback."""
        def c_callback(widget, value, userdata):
            callback(value)
        cb = _ChangeCallbackType(c_callback)
        self._callbacks.append(cb)
        self._lib.ygui_slider_on_change(self._handle, cb, None)


class Checkbox(Widget):
    """Checkbox widget."""

    def set_checked(self, checked: bool):
        self._lib.ygui_checkbox_set_checked(self._handle, 1 if checked else 0)

    def get_checked(self) -> bool:
        return bool(self._lib.ygui_checkbox_get_checked(self._handle))

    checked = property(get_checked, set_checked)

    def on_change(self, callback: Callable[[bool], None]):
        """Set change callback."""
        def c_callback(widget, checked, userdata):
            callback(bool(checked))
        cb = _CheckCallbackType(c_callback)
        self._callbacks.append(cb)
        self._lib.ygui_checkbox_on_change(self._handle, cb, None)


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
    """YGui engine - self-contained widget system with event loop.

    Creates a card in yetty terminal, handles all mouse events internally,
    dispatches to widget callbacks, and auto-renders when dirty.
    """

    def __init__(self, name: str, width: float = 800, height: float = 600):
        """
        Create YGui engine.

        Args:
            name: Card name (used for OSC commands)
            width: Canvas width in pixels
            height: Canvas height in pixels
        """
        self._lib = _get_lib()
        self._handle = self._lib.ygui_engine_create(name.encode("utf-8"), width, height)
        if not self._handle:
            raise RuntimeError("Failed to create YGui engine")

        self._widgets = {}
        self._callbacks = []  # prevent GC
        self._shown = False

    def __del__(self):
        if hasattr(self, "_handle") and self._handle:
            self._lib.ygui_engine_destroy(self._handle)

    def show(self, x: int = 0, y: int = 0, w: int = 80, h: int = 24):
        """Show the card at terminal cell position."""
        self._lib.ygui_engine_show(self._handle, x, y, w, h)
        self._shown = True

    def render(self):
        """Force render (usually auto-rendered when dirty)."""
        self._lib.ygui_engine_render(self._handle)

    def run(self):
        """Run event loop (blocks until stop() or 'q')."""
        self._lib.ygui_engine_run(self._handle)

    def stop(self):
        """Stop the event loop."""
        self._lib.ygui_engine_stop(self._handle)

    def set_size(self, width: float, height: float):
        self._lib.ygui_engine_set_size(self._handle, width, height)

    def is_dirty(self) -> bool:
        return bool(self._lib.ygui_engine_is_dirty(self._handle))

    def mark_dirty(self):
        self._lib.ygui_engine_mark_dirty(self._handle)

    def on_key(self, callback: Callable[[int, int], None]):
        """Set keyboard callback. Args: (key_code, modifiers)."""
        def c_callback(engine, key, mods, userdata):
            callback(key, mods)
        cb = _KeyCallbackType(c_callback)
        self._callbacks.append(cb)
        self._lib.ygui_engine_on_key(self._handle, cb, None)

    def find(self, widget_id: str) -> Optional[Widget]:
        handle = self._lib.ygui_engine_find(self._handle, widget_id.encode("utf-8"))
        if handle:
            return self._widgets.get(widget_id)
        return None

    # Widget creation methods
    def button(self, id: str, x: float, y: float, w: float, h: float, label: str) -> Button:
        handle = self._lib.ygui_button(
            self._handle, id.encode("utf-8"), x, y, w, h, label.encode("utf-8"))
        btn = Button(handle, self)
        self._widgets[id] = btn
        return btn

    def label(self, id: str, x: float, y: float, text: str) -> Label:
        handle = self._lib.ygui_label(
            self._handle, id.encode("utf-8"), x, y, text.encode("utf-8"))
        lbl = Label(handle, self)
        self._widgets[id] = lbl
        return lbl

    def slider(self, id: str, x: float, y: float, w: float, h: float,
               min_val: float = 0, max_val: float = 100, value: float = 50) -> Slider:
        handle = self._lib.ygui_slider(
            self._handle, id.encode("utf-8"), x, y, w, h, min_val, max_val, value)
        sld = Slider(handle, self)
        self._widgets[id] = sld
        return sld

    def checkbox(self, id: str, x: float, y: float, w: float, h: float,
                 label: str, checked: bool = False) -> Checkbox:
        handle = self._lib.ygui_checkbox(
            self._handle, id.encode("utf-8"), x, y, w, h,
            label.encode("utf-8"), 1 if checked else 0)
        chk = Checkbox(handle, self)
        self._widgets[id] = chk
        return chk

    def panel(self, id: str, x: float, y: float, w: float, h: float) -> Panel:
        handle = self._lib.ygui_panel(
            self._handle, id.encode("utf-8"), x, y, w, h)
        pnl = Panel(handle, self)
        self._widgets[id] = pnl
        return pnl

    def progress(self, id: str, x: float, y: float, w: float, h: float,
                 value: float = 0) -> Progress:
        handle = self._lib.ygui_progress(
            self._handle, id.encode("utf-8"), x, y, w, h, value)
        prg = Progress(handle, self)
        self._widgets[id] = prg
        return prg


def init() -> bool:
    """Initialize the library (sets up raw terminal mode).
    Must be called before using event loop functions.
    Returns True on success."""
    return _get_lib().ygui_init() == 0


def shutdown():
    """Shutdown the library (restores terminal settings)."""
    _get_lib().ygui_shutdown()


def version() -> str:
    """Get YGui version string."""
    return _get_lib().ygui_version().decode("utf-8")
