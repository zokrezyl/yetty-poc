"""
Grid layout - 2x2 subplots with different visualizations
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

    _tex = yetty_card.allocate_texture(px_w, px_h)
    _fig = fpl.Figure(size=(px_w, px_h), shape=(2, 2))

    xs = np.linspace(0, 4 * np.pi, 200)

    # Top-left: sine wave
    _fig[0, 0].add_line(np.column_stack([xs, np.sin(xs)]), thickness=3, colors="cyan")

    # Top-right: scatter plot
    n = 500
    data = np.random.randn(n, 3) * 5
    _fig[0, 1].add_scatter(data, sizes=3, cmap="plasma", alpha=0.7)

    # Bottom-left: heatmap
    hm = np.sin(np.linspace(0, 10, 100)[:, None]) * np.cos(np.linspace(0, 10, 100)[None, :])
    _fig[1, 0].add_image(hm, cmap="inferno")

    # Bottom-right: cosine wave
    _fig[1, 1].add_line(np.column_stack([xs, np.cos(xs)]), thickness=3, colors="magenta")

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
    global _tex, _fig
    if _tex and _tex.valid:
        yetty_card.deallocate_texture(_tex)
    _tex = None
    _fig = None
    return Ok(None)
