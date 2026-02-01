"""
Audio waveform visualization - animated oscilloscope
"""
import os
os.environ['RENDERCANVAS_FORCE_OFFSCREEN'] = '1'

import yetty_card
from python.result import Result, Ok, Err

_tex = None
_fig = None
_np = None
_waveform = None
_t_vals = None
_phase = 0.0

PX_PER_CELL_X = 10
PX_PER_CELL_Y = 20


def setup():
    try:
        yetty_card.ensure_packages(['numpy', 'fastplotlib', 'rendercanvas'])
    except Exception as e:
        return Result.error("setup: failed to install packages", e)
    return Ok(None)


def init(width_cells, height_cells):
    global _tex, _fig, _np, _waveform, _t_vals, _phase

    try:
        import numpy as np
        import fastplotlib as fpl
    except ImportError as e:
        return Result.error("init: failed to import dependencies", e)

    _np = np
    _phase = 0.0
    px_w = max(width_cells * PX_PER_CELL_X, 64)
    px_h = max(height_cells * PX_PER_CELL_Y, 64)

    _tex = yetty_card.allocate_texture_handle(px_w, px_h)
    _fig = fpl.Figure(size=(px_w, px_h))

    n_points = 1000
    _t_vals = np.linspace(0, 4 * np.pi, n_points)
    y = np.zeros(n_points)
    _waveform = _fig[0, 0].add_line(np.column_stack([_t_vals, y]), thickness=2, cmap="viridis")

    def animate():
        global _phase
        _phase += 0.1
        y = (
            np.sin(_t_vals + _phase) * 0.5 +
            np.sin(2 * _t_vals + _phase * 1.3) * 0.3 +
            np.sin(5 * _t_vals + _phase * 0.7) * 0.15 +
            np.sin(11 * _t_vals - _phase * 2) * 0.05
        )
        _waveform.data = np.column_stack([_t_vals, y])

    _fig.add_animations(animate)
    _fig.show()
    return Ok(None)


def render(time):
    if _fig is None or _tex is None or _np is None:
        return Result.error("render: not initialized")

    canvas = _fig.canvas
    canvas.force_draw()
    img = canvas.draw()
    if img is None:
        return Ok(None)

    pixels = _np.asarray(img, dtype=_np.uint8).ravel()
    buf = memoryview(_tex)
    n = min(len(pixels), len(buf))
    buf[:n] = pixels[:n]

    yetty_card.link_texture_data(_tex)
    return Ok(None)


def suspend():
    return Ok(None)


# ─── Event forwarding to rendercanvas ───────────────────────────────
_CTRL = 0x0002
_SHIFT = 0x0001

def _modifiers(mods):
    m = []
    if mods & _CTRL:
        m.append("Control")
    if mods & _SHIFT:
        m.append("Shift")
    return tuple(m)

def _submit(ev):
    if _fig is not None and hasattr(_fig, 'canvas'):
        c = _fig.canvas
        if hasattr(c, '_handle_event_and_flush'):
            c._handle_event_and_flush(ev)

def on_scroll(x, y, dx, dy, mods):
    _submit({"event_type": "wheel", "x": x, "y": y,
             "dx": dx * 100, "dy": -dy * 100,
             "modifiers": _modifiers(mods)})

def on_mouse_down(x, y, button):
    _submit({"event_type": "pointer_down", "x": x, "y": y,
             "button": button + 1, "modifiers": ()})

def on_mouse_move(x, y):
    _submit({"event_type": "pointer_move", "x": x, "y": y,
             "button": 0, "modifiers": ()})

def on_mouse_up(x, y, button):
    _submit({"event_type": "pointer_up", "x": x, "y": y,
             "button": button + 1, "modifiers": ()})


def shutdown():
    global _tex, _fig, _waveform
    if _tex and _tex.valid:
        yetty_card.deallocate_texture_handle(_tex)
    _tex = None
    _fig = None
    _waveform = None
    return Ok(None)
