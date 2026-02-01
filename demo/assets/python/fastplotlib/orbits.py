"""
Orbital mechanics - animated planetary orbits
"""
import os
os.environ['RENDERCANVAS_FORCE_OFFSCREEN'] = '1'

import yetty_card
from python.result import Result, Ok, Err

_tex = None
_fig = None
_np = None
_planet_scatters = None
_planets = None
_t = 0.0

PX_PER_CELL_X = 10
PX_PER_CELL_Y = 20


def setup():
    try:
        yetty_card.ensure_packages(['numpy', 'fastplotlib', 'rendercanvas'])
    except Exception as e:
        return Result.error("setup: failed to install packages", e)
    return Ok(None)


def init(width_cells, height_cells):
    global _tex, _fig, _np, _planet_scatters, _planets, _t

    try:
        import numpy as np
        import fastplotlib as fpl
    except ImportError as e:
        return Result.error("init: failed to import dependencies", e)

    _np = np
    _t = 0.0
    px_w = max(width_cells * PX_PER_CELL_X, 64)
    px_h = max(height_cells * PX_PER_CELL_Y, 64)

    _tex = yetty_card.allocate_texture_handle(px_w, px_h)
    _fig = fpl.Figure(size=(px_w, px_h))

    _planets = [
        (1.0, 0.2, 1.0, "gray"),
        (1.8, 0.1, 1.6, "orange"),
        (2.5, 0.02, 2.0, "blue"),
        (3.5, 0.09, 2.8, "red"),
    ]

    # Draw orbits
    for a, e, _, color in _planets:
        theta = np.linspace(0, 2 * np.pi, 200)
        r = a * (1 - e**2) / (1 + e * np.cos(theta))
        x = r * np.cos(theta)
        y = r * np.sin(theta)
        z = np.zeros_like(x)
        _fig[0, 0].add_line(np.column_stack([x, y, z]), thickness=1, colors="gray", alpha=0.5)

    # Sun
    _fig[0, 0].add_scatter(np.array([[0, 0, 0]]), sizes=20, colors="yellow")

    # Planets
    _planet_scatters = []
    for a, e, _, color in _planets:
        scatter = _fig[0, 0].add_scatter(np.array([[a, 0, 0]]), sizes=8, colors=color)
        _planet_scatters.append(scatter)

    def animate():
        global _t
        _t += 0.02
        for i, (a, e, period, _) in enumerate(_planets):
            theta = _t / period
            r = a * (1 - e**2) / (1 + e * np.cos(theta))
            x = r * np.cos(theta)
            y = r * np.sin(theta)
            _planet_scatters[i].data = np.array([[x, y, 0]])

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
    global _tex, _fig, _planet_scatters
    if _tex and _tex.valid:
        yetty_card.deallocate_texture_handle(_tex)
    _tex = None
    _fig = None
    _planet_scatters = None
    return Ok(None)
