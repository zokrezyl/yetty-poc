"""
Lissajous curves - animated parametric curves
"""
import os
os.environ['RENDERCANVAS_FORCE_OFFSCREEN'] = '1'

import yetty_card
from python.result import Result, Ok, Err

_tex = None
_fig = None
_np = None
_curves = None
_params = None
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
    global _tex, _fig, _np, _curves, _params, _t_vals, _phase

    try:
        import numpy as np
        import fastplotlib as fpl
    except ImportError as e:
        return Result.error("init: failed to import dependencies", e)

    _np = np
    _phase = 0.0
    px_w = max(width_cells * PX_PER_CELL_X, 64)
    px_h = max(height_cells * PX_PER_CELL_Y, 64)

    _tex = yetty_card.allocate_texture(px_w, px_h)
    _fig = fpl.Figure(size=(px_w, px_h))

    _t_vals = np.linspace(0, 2 * np.pi, 1000)
    _params = [(3, 2), (5, 4), (7, 6), (3, 4), (5, 6), (7, 4)]
    colors = ["red", "green", "blue", "yellow", "cyan", "magenta"]

    _curves = []
    for i, (a, b) in enumerate(_params):
        x = np.sin(a * _t_vals + i * 0.5)
        y = np.cos(b * _t_vals) + i * 2.5
        z = np.zeros_like(x)
        data = np.column_stack([x, y, z])
        _curves.append(_fig[0, 0].add_line(data, thickness=3, colors=colors[i]))

    def animate():
        global _phase
        _phase += 0.05
        for i, (a, b) in enumerate(_params):
            x = np.sin(a * _t_vals + _phase + i * 0.5)
            y = np.cos(b * _t_vals + _phase * 0.7) + i * 2.5
            z = np.zeros_like(x)
            _curves[i].data = np.column_stack([x, y, z])

    _fig.add_animations(animate)
    _fig.show()
    return Ok(None)


def allocate_textures():
    if _tex is not None and _tex.valid:
        yetty_card.write_texture(_tex)
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

    yetty_card.write_texture(_tex)
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
    global _tex, _fig, _curves
    if _tex and _tex.valid:
        yetty_card.deallocate_texture(_tex)
    _tex = None
    _fig = None
    _curves = None
    return Ok(None)
