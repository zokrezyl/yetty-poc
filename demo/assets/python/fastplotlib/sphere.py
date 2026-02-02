"""
Animated sphere with ripple effect - scatter visualization
"""
import os
os.environ['RENDERCANVAS_FORCE_OFFSCREEN'] = '1'

import yetty_card
from python.result import Result, Ok, Err

_tex = None
_fig = None
_np = None
_sphere = None
_current_data = None
_z_ref = None
_phi = None
_theta_grid = None
_start = 0

PX_PER_CELL_X = 10
PX_PER_CELL_Y = 20

_RADIUS = 10
_NX, _NY = 100, 100
_RIPPLE_AMPLITUDE = 1.0
_RIPPLE_FREQUENCY = 20.0


def setup():
    try:
        yetty_card.ensure_packages(['numpy', 'fastplotlib', 'rendercanvas'])
    except Exception as e:
        return Result.error("setup: failed to install packages", e)
    return Ok(None)


def init(width_cells, height_cells):
    global _tex, _fig, _np, _sphere, _current_data, _z_ref, _phi, _theta_grid, _start

    try:
        import numpy as np
        import fastplotlib as fpl
    except ImportError as e:
        return Result.error("init: failed to import dependencies", e)

    _np = np
    _start = 0
    px_w = max(width_cells * PX_PER_CELL_X, 64)
    px_h = max(height_cells * PX_PER_CELL_Y, 64)

    _tex = yetty_card.allocate_texture(px_w, px_h)
    _fig = fpl.Figure(size=(px_w, px_h), cameras="3d")

    _phi = np.linspace(0, np.pi * 2, num=_NX, dtype=np.float32)
    theta = np.linspace(0, np.pi, num=_NY, dtype=np.float32)
    phi_grid, _theta_grid = np.meshgrid(_phi, theta)

    theta_grid_sin = np.sin(_theta_grid)
    x = _RADIUS * np.cos(phi_grid) * theta_grid_sin * -1
    y = _RADIUS * np.cos(_theta_grid)

    ripple = _RIPPLE_AMPLITUDE * np.sin(_RIPPLE_FREQUENCY * _theta_grid)
    _z_ref = _RADIUS * np.sin(phi_grid) * theta_grid_sin
    z = _z_ref * (1 + ripple / _RADIUS)

    _current_data = np.column_stack([x.flatten(), y.flatten(), z.flatten()])
    _sphere = _fig[0, 0].add_scatter(_current_data, cmap="jet", sizes=3)

    def animate():
        global _start, _current_data
        theta_anim = np.linspace(_start, _start + np.pi, num=_NY, dtype=np.float32)
        _, theta_grid_anim = np.meshgrid(_phi, theta_anim)
        ripple = _RIPPLE_AMPLITUDE * np.sin(_RIPPLE_FREQUENCY * theta_grid_anim)
        z = _z_ref * (1 + ripple / _RADIUS)
        _current_data[:, 2] = z.flatten()
        _sphere.data = _current_data
        _start += 0.005
        if _start > np.pi * 2:
            _start = 0

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
    global _tex, _fig, _sphere, _current_data
    if _tex and _tex.valid:
        yetty_card.deallocate_texture(_tex)
    _tex = None
    _fig = None
    _sphere = None
    _current_data = None
    return Ok(None)
