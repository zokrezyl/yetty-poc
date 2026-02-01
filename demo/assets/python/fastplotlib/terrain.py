"""
3D terrain - procedural heightmap as scatter
"""
import os
os.environ['RENDERCANVAS_FORCE_OFFSCREEN'] = '1'

import yetty_card
from python.result import Result, Ok, Err

_tex = None
_fig = None
_np = None

PX_PER_CELL_X = 10
PX_PER_CELL_Y = 20


def setup():
    try:
        yetty_card.ensure_packages(['numpy', 'fastplotlib', 'rendercanvas'])
    except Exception as e:
        return Result.error("setup: failed to install packages", e)
    return Ok(None)


def init(width_cells, height_cells):
    global _tex, _fig, _np

    try:
        import numpy as np
        import fastplotlib as fpl
    except ImportError as e:
        return Result.error("init: failed to import dependencies", e)

    _np = np
    px_w = max(width_cells * PX_PER_CELL_X, 64)
    px_h = max(height_cells * PX_PER_CELL_Y, 64)

    _tex = yetty_card.allocate_texture_handle(px_w, px_h)
    _fig = fpl.Figure(size=(px_w, px_h), cameras="3d", controller_types="orbit")

    def fractal_noise(size, octaves=6, persistence=0.5):
        result = np.zeros((size, size))
        amplitude = 1.0
        frequency = 1
        for _ in range(octaves):
            noise_size = max(2, size // frequency)
            noise = np.random.rand(noise_size, noise_size)
            x = np.linspace(0, noise_size - 1, size)
            y = np.linspace(0, noise_size - 1, size)
            xv, yv = np.meshgrid(x, y)
            xi = xv.astype(int).clip(0, noise_size - 2)
            yi = yv.astype(int).clip(0, noise_size - 2)
            xf = xv - xi
            yf = yv - yi
            n00 = noise[yi, xi]
            n10 = noise[yi, xi + 1]
            n01 = noise[yi + 1, xi]
            n11 = noise[yi + 1, xi + 1]
            upsampled = n00 * (1 - xf) * (1 - yf) + n10 * xf * (1 - yf) + n01 * (1 - xf) * yf + n11 * xf * yf
            result += amplitude * upsampled
            amplitude *= persistence
            frequency *= 2
        return result

    size = 80
    x = np.linspace(0, 10, size)
    y = np.linspace(0, 10, size)
    X, Y = np.meshgrid(x, y)
    Z = fractal_noise(size) * 3

    data = np.column_stack([X.flatten(), Y.flatten(), Z.flatten()])
    _fig[0, 0].add_scatter(data, cmap="terrain", sizes=2)

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
    global _tex, _fig
    if _tex and _tex.valid:
        yetty_card.deallocate_texture_handle(_tex)
    _tex = None
    _fig = None
    return Ok(None)
