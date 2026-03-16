"""
YGui Python Bindings

Pure Python bindings for YGui-C using ctypes.
The library handles OSC output internally - no manual OSC code needed.

Set YGUI_LOG environment variable to enable logging:
  YGUI_LOG=stderr     - log to stderr
  YGUI_LOG=/path/file - log to file
"""

import ctypes
from ctypes import (
    c_void_p, c_char_p, c_float, c_int, c_uint32, c_int32, c_uint8,
    POINTER, Structure, Union, CFUNCTYPE, byref
)
from typing import Callable, Optional
from enum import IntEnum
import os
import sys
import logging

#=============================================================================
# Logging setup
#=============================================================================

_logger = logging.getLogger("ygui")
_logger.setLevel(logging.DEBUG)

def _setup_logging():
    log_dest = os.environ.get("YGUI_LOG")
    if not log_dest:
        _logger.addHandler(logging.NullHandler())
        return

    formatter = logging.Formatter(
        '%(asctime)s.%(msecs)03d [%(levelname)s] %(message)s',
        datefmt='%H:%M:%S'
    )

    if log_dest == "stderr":
        handler = logging.StreamHandler(sys.stderr)
    else:
        handler = logging.FileHandler(log_dest, mode='w')

    handler.setFormatter(formatter)
    handler.setLevel(logging.DEBUG)
    _logger.addHandler(handler)
    _logger.info("YGui logging initialized to: %s", log_dest)

_setup_logging()

#=============================================================================
# Library loading
#=============================================================================

def _load_library():
    """Load libygui.so shared library (self-contained, no external deps)."""
    search_paths = [
        # Build directory
        os.path.join(os.path.dirname(__file__), "..", "..", "..", "build-desktop-ytrace-release", "src", "yetty", "ygui-c", "libygui.so"),
        # Installed location
        "/usr/local/lib/libygui.so",
        "/usr/lib/libygui.so",
        # Current directory
        os.path.join(os.path.dirname(__file__), "libygui.so"),
        "libygui.so",
    ]
    _logger.debug("Searching for libygui.so in: %s", search_paths)
    for path in search_paths:
        _logger.debug("Trying: %s", path)
        try:
            lib = ctypes.CDLL(path)
            _logger.info("Loaded library from: %s", path)
            return lib
        except OSError as e:
            _logger.debug("Failed to load %s: %s", path, e)
            continue
    _logger.error("Could not load libygui.so from any path")
    raise ImportError("Could not load libygui.so")

_lib = None

def _get_lib():
    global _lib
    if _lib is None:
        _logger.debug("First call to _get_lib(), loading library...")
        _lib = _load_library()
        _logger.debug("Setting up function signatures...")
        _setup_functions(_lib)
        _logger.info("Library ready")
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


class CanvasMode(IntEnum):
    FIXED = 0   # Canvas size stays constant, coordinates are scaled
    FIT = 1     # Canvas size = display size (card_cells * cell_pixels), 1:1 coords


class ScaleMode(IntEnum):
    OFF = 0     # Widgets keep their original positions/sizes
    ON = 1      # Widgets scale proportionally when canvas resizes


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
    # ygui_engine_create(card_name, x, y, cols, rows)
    lib.ygui_engine_create.argtypes = [c_char_p, c_int, c_int, c_int, c_int]
    lib.ygui_engine_create.restype = c_void_p

    # ygui_engine_create_with_pixel_hint(card_name, x, y, width_hint, height_hint)
    lib.ygui_engine_create_with_pixel_hint.argtypes = [c_char_p, c_int, c_int, c_float, c_float]
    lib.ygui_engine_create_with_pixel_hint.restype = c_void_p

    lib.ygui_engine_destroy.argtypes = [c_void_p]
    lib.ygui_engine_destroy.restype = None

    # ygui_engine_show(engine) - no params, position/size set in create
    lib.ygui_engine_show.argtypes = [c_void_p]
    lib.ygui_engine_show.restype = None

    lib.ygui_engine_set_display_pixel_size.argtypes = [c_void_p, c_float, c_float]
    lib.ygui_engine_set_display_pixel_size.restype = None

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

    lib.ygui_engine_set_canvas_mode.argtypes = [c_void_p, c_int]
    lib.ygui_engine_set_canvas_mode.restype = None

    lib.ygui_engine_set_scale_mode.argtypes = [c_void_p, c_int]
    lib.ygui_engine_set_scale_mode.restype = None

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
        _logger.debug("Button.set_label(%r)", label)
        self._lib.ygui_button_set_label(self._handle, label.encode("utf-8"))

    def on_click(self, callback: Callable[[], None]):
        """Set click callback."""
        _logger.debug("Button.on_click() setting callback")
        def c_callback(widget, userdata):
            _logger.info("CALLBACK: Button clicked! widget=%s", widget)
            try:
                callback()
                _logger.debug("CALLBACK: Button callback completed")
            except Exception as e:
                _logger.exception("CALLBACK: Button callback raised exception: %s", e)
        cb = _ClickCallbackType(c_callback)
        self._callbacks.append(cb)
        self._lib.ygui_button_on_click(self._handle, cb, None)
        _logger.debug("Button.on_click() callback registered")


class Label(Widget):
    """Label widget."""

    def set_text(self, text):
        _logger.debug("Label.set_text(%r)", text)
        if isinstance(text, str):
            text = text.encode("utf-8")
        self._lib.ygui_label_set_text(self._handle, text)


class Slider(Widget):
    """Slider widget."""

    def set_value(self, value: float):
        _logger.debug("Slider.set_value(%s)", value)
        self._lib.ygui_slider_set_value(self._handle, value)

    def get_value(self) -> float:
        val = self._lib.ygui_slider_get_value(self._handle)
        _logger.debug("Slider.get_value() -> %s", val)
        return val

    value = property(get_value, set_value)

    def on_change(self, callback: Callable[[float], None]):
        """Set change callback."""
        _logger.debug("Slider.on_change() setting callback")
        def c_callback(widget, value, userdata):
            _logger.info("CALLBACK: Slider changed! widget=%s value=%s", widget, value)
            try:
                callback(value)
                _logger.debug("CALLBACK: Slider callback completed")
            except Exception as e:
                _logger.exception("CALLBACK: Slider callback raised exception: %s", e)
        cb = _ChangeCallbackType(c_callback)
        self._callbacks.append(cb)
        self._lib.ygui_slider_on_change(self._handle, cb, None)
        _logger.debug("Slider.on_change() callback registered")


class Checkbox(Widget):
    """Checkbox widget."""

    def set_checked(self, checked: bool):
        _logger.debug("Checkbox.set_checked(%s)", checked)
        self._lib.ygui_checkbox_set_checked(self._handle, 1 if checked else 0)

    def get_checked(self) -> bool:
        val = bool(self._lib.ygui_checkbox_get_checked(self._handle))
        _logger.debug("Checkbox.get_checked() -> %s", val)
        return val

    checked = property(get_checked, set_checked)

    def on_change(self, callback: Callable[[bool], None]):
        """Set change callback."""
        _logger.debug("Checkbox.on_change() setting callback")
        def c_callback(widget, checked, userdata):
            _logger.info("CALLBACK: Checkbox changed! widget=%s checked=%s", widget, checked)
            try:
                callback(bool(checked))
                _logger.debug("CALLBACK: Checkbox callback completed")
            except Exception as e:
                _logger.exception("CALLBACK: Checkbox callback raised exception: %s", e)
        cb = _CheckCallbackType(c_callback)
        self._callbacks.append(cb)
        self._lib.ygui_checkbox_on_change(self._handle, cb, None)
        _logger.debug("Checkbox.on_change() callback registered")


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

    def __init__(self, name: str, *, x: int = 0, y: int = 0,
                 cols: int = None, rows: int = None,
                 width: float = None, height: float = None):
        """
        Create YGui engine.

        Two modes:
        1. Cell dimensions (recommended): Engine("app", x=2, y=2, cols=50, rows=18)
           Canvas = actual card pixels after show().
        2. Pixel hints (legacy): Engine("app", x=2, y=2, width=400, height=300)
           Calculates closest cell dimensions from hints.

        Args:
            name: Card name (used for OSC commands)
            x: Card position X in terminal cells
            y: Card position Y in terminal cells
            cols: Card width in terminal cells
            rows: Card height in terminal cells
            width: Pixel width hint (legacy, calculates cols)
            height: Pixel height hint (legacy, calculates rows)
        """
        self._name = name
        self._lib = _get_lib()

        if cols is not None and rows is not None:
            # New API: cell dimensions
            _logger.info("Engine.__init__(name=%r, x=%d, y=%d, cols=%d, rows=%d)", name, x, y, cols, rows)
            self._handle = self._lib.ygui_engine_create(name.encode("utf-8"), x, y, cols, rows)
        elif width is not None and height is not None:
            # Legacy API: pixel hints
            _logger.info("Engine.__init__(name=%r, x=%d, y=%d, width=%s, height=%s)", name, x, y, width, height)
            self._handle = self._lib.ygui_engine_create_with_pixel_hint(
                name.encode("utf-8"), x, y, width, height)
        else:
            raise ValueError("Must provide either (cols, rows) or (width, height)")

        _logger.debug("ygui_engine_create returned handle=%s", self._handle)
        if not self._handle:
            _logger.error("ygui_engine_create returned NULL!")
            raise RuntimeError("Failed to create YGui engine")

        self._widgets = {}
        self._callbacks = []  # prevent GC
        self._shown = False
        _logger.info("Engine created successfully")

    def __del__(self):
        if hasattr(self, "_handle") and self._handle:
            _logger.debug("Engine.__del__() destroying handle=%s", self._handle)
            self._lib.ygui_engine_destroy(self._handle)

    def show(self):
        """Show the card (position/size were set in constructor)."""
        _logger.info("Engine.show()")
        self._lib.ygui_engine_show(self._handle)
        self._shown = True
        _logger.debug("Card shown")

    def set_display_pixel_size(self, width: float, height: float):
        """Set display pixel size directly (for testing)."""
        self._lib.ygui_engine_set_display_pixel_size(self._handle, width, height)

    def render(self):
        """Force render (usually auto-rendered when dirty)."""
        _logger.debug("Engine.render()")
        self._lib.ygui_engine_render(self._handle)

    def run(self):
        """Run event loop (blocks until stop() or 'q')."""
        _logger.info("Engine.run() - entering event loop")
        self._lib.ygui_engine_run(self._handle)
        _logger.info("Engine.run() - event loop exited")

    def stop(self):
        """Stop the event loop."""
        _logger.info("Engine.stop()")
        self._lib.ygui_engine_stop(self._handle)

    def set_size(self, width: float, height: float):
        self._lib.ygui_engine_set_size(self._handle, width, height)

    def is_dirty(self) -> bool:
        return bool(self._lib.ygui_engine_is_dirty(self._handle))

    def mark_dirty(self):
        self._lib.ygui_engine_mark_dirty(self._handle)

    def set_canvas_mode(self, mode: CanvasMode):
        """Set canvas mode. FIXED keeps canvas size constant (default).
        FIT resizes canvas to match display (card_cells * cell_pixels) for 1:1 coordinates."""
        self._lib.ygui_engine_set_canvas_mode(self._handle, int(mode))

    def set_scale_mode(self, mode: ScaleMode):
        """Set widget scale mode. OFF keeps widget positions fixed (default).
        ON scales widgets proportionally when canvas resizes."""
        self._lib.ygui_engine_set_scale_mode(self._handle, int(mode))

    def on_key(self, callback: Callable[[int, int], None]):
        """Set keyboard callback. Args: (key_code, modifiers)."""
        _logger.debug("Engine.on_key() setting callback")
        def c_callback(engine, key, mods, userdata):
            _logger.info("CALLBACK: Key pressed! key=%d (chr=%r) mods=%d", key, chr(key) if 32 <= key < 127 else '?', mods)
            try:
                callback(key, mods)
                _logger.debug("CALLBACK: Key callback completed")
            except Exception as e:
                _logger.exception("CALLBACK: Key callback raised exception: %s", e)
        cb = _KeyCallbackType(c_callback)
        self._callbacks.append(cb)
        self._lib.ygui_engine_on_key(self._handle, cb, None)
        _logger.debug("Engine.on_key() callback registered")

    def find(self, widget_id: str) -> Optional[Widget]:
        handle = self._lib.ygui_engine_find(self._handle, widget_id.encode("utf-8"))
        if handle:
            return self._widgets.get(widget_id)
        return None

    # Widget creation methods
    def button(self, id: str, x: float, y: float, w: float, h: float, label: str) -> Button:
        _logger.debug("Creating button id=%r at (%s,%s) size=(%s,%s) label=%r", id, x, y, w, h, label)
        handle = self._lib.ygui_button(
            self._handle, id.encode("utf-8"), x, y, w, h, label.encode("utf-8"))
        _logger.debug("ygui_button returned handle=%s", handle)
        btn = Button(handle, self)
        self._widgets[id] = btn
        return btn

    def label(self, id: str, x: float, y: float, text: str) -> Label:
        _logger.debug("Creating label id=%r at (%s,%s) text=%r", id, x, y, text)
        handle = self._lib.ygui_label(
            self._handle, id.encode("utf-8"), x, y, text.encode("utf-8"))
        _logger.debug("ygui_label returned handle=%s", handle)
        lbl = Label(handle, self)
        self._widgets[id] = lbl
        return lbl

    def slider(self, id: str, x: float, y: float, w: float, h: float,
               min_val: float = 0, max_val: float = 100, value: float = 50) -> Slider:
        _logger.debug("Creating slider id=%r at (%s,%s) size=(%s,%s) range=[%s,%s] value=%s",
                      id, x, y, w, h, min_val, max_val, value)
        handle = self._lib.ygui_slider(
            self._handle, id.encode("utf-8"), x, y, w, h, min_val, max_val, value)
        _logger.debug("ygui_slider returned handle=%s", handle)
        sld = Slider(handle, self)
        self._widgets[id] = sld
        return sld

    def checkbox(self, id: str, x: float, y: float, w: float, h: float,
                 label: str, checked: bool = False) -> Checkbox:
        _logger.debug("Creating checkbox id=%r at (%s,%s) size=(%s,%s) label=%r checked=%s",
                      id, x, y, w, h, label, checked)
        handle = self._lib.ygui_checkbox(
            self._handle, id.encode("utf-8"), x, y, w, h,
            label.encode("utf-8"), 1 if checked else 0)
        _logger.debug("ygui_checkbox returned handle=%s", handle)
        chk = Checkbox(handle, self)
        self._widgets[id] = chk
        return chk

    def panel(self, id: str, x: float, y: float, w: float, h: float) -> Panel:
        _logger.debug("Creating panel id=%r at (%s,%s) size=(%s,%s)", id, x, y, w, h)
        handle = self._lib.ygui_panel(
            self._handle, id.encode("utf-8"), x, y, w, h)
        _logger.debug("ygui_panel returned handle=%s", handle)
        pnl = Panel(handle, self)
        self._widgets[id] = pnl
        return pnl

    def progress(self, id: str, x: float, y: float, w: float, h: float,
                 value: float = 0) -> Progress:
        _logger.debug("Creating progress id=%r at (%s,%s) size=(%s,%s) value=%s",
                      id, x, y, w, h, value)
        handle = self._lib.ygui_progress(
            self._handle, id.encode("utf-8"), x, y, w, h, value)
        _logger.debug("ygui_progress returned handle=%s", handle)
        prg = Progress(handle, self)
        self._widgets[id] = prg
        return prg


def init() -> bool:
    """Initialize the library (sets up raw terminal mode).
    Must be called before using event loop functions.
    Returns True on success."""
    _logger.info("ygui.init() called")
    result = _get_lib().ygui_init()
    _logger.info("ygui_init() returned %d (success=%s)", result, result == 0)
    return result == 0


def shutdown():
    """Shutdown the library (restores terminal settings)."""
    _logger.info("ygui.shutdown() called")
    _get_lib().ygui_shutdown()
    _logger.info("ygui_shutdown() completed")


def version() -> str:
    """Get YGui version string."""
    ver = _get_lib().ygui_version().decode("utf-8")
    _logger.debug("ygui.version() -> %r", ver)
    return ver
