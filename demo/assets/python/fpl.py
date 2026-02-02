"""
Fastplotlib sine wave demo for yetty card system.

Uses yetty_card module for buffer management and offscreen pygfx rendering.
Renders fastplotlib figure to CPU pixels, then uploads to card texture.

Usage:
    yetty-client run python -i demo/assets/python/fpl.py
"""

import os
os.environ['RENDERCANVAS_FORCE_OFFSCREEN'] = '1'

import yetty_card
from python.result import Result, Ok, Err

_tex = None
_fig = None
_phase = 0.0
_line = None
_np = None

# Pixel dimensions per terminal cell (approximate)
PX_PER_CELL_X = 10
PX_PER_CELL_Y = 20


def setup():
    """Called once per script (first card load). Install dependencies."""
    try:
        yetty_card.ensure_packages(['numpy', 'fastplotlib', 'rendercanvas'])
    except Exception as e:
        return Result.error("setup: failed to install packages", e)
    return Ok(None)


def init(width_cells, height_cells):
    global _tex, _fig, _line, _np

    try:
        import numpy as np
        import fastplotlib as fpl
    except ImportError as e:
        return Result.error("init: failed to import dependencies", e)

    _np = np

    px_w = max(width_cells * PX_PER_CELL_X, 64)
    px_h = max(height_cells * PX_PER_CELL_Y, 64)

    # Allocate card texture (RGBA8, width * height * 4 bytes)
    _tex = yetty_card.allocate_texture(px_w, px_h)

    # Create fastplotlib figure with offscreen canvas
    _fig = fpl.Figure(size=(px_w, px_h))

    # Create sine wave data (float32 for GPU)
    x = np.linspace(0, 4 * np.pi, 500, dtype=np.float32)
    y = np.sin(x)

    _line = _fig[0, 0].add_line(
        np.column_stack([x, y]), thickness=8, cmap='hot'
    )

    _fig.show()
    return Ok(None)


def allocate_textures():
    if _tex is not None and _tex.valid:
        yetty_card.write_texture(_tex)
    return Ok(None)


def render(time):
    global _phase

    if _fig is None or _tex is None or _np is None:
        return Result.error("render: not initialized")

    # Animate: shift the sine wave phase
    _phase += 0.05
    x = _np.linspace(0, 4 * _np.pi, 500, dtype=_np.float32)
    y = _np.sin(x + _phase)
    _line.data[:, 1] = y

    # Render and upload
    canvas = _fig.canvas
    canvas.force_draw()
    img = canvas.draw()
    if img is None:
        return Ok(None)  # Frame not ready yet, not an error

    pixels = _np.asarray(img, dtype=_np.uint8).ravel()
    buf = memoryview(_tex)
    n = min(len(pixels), len(buf))
    buf[:n] = pixels[:n]

    yetty_card.write_texture(_tex)
    return Ok(None)


def suspend():
    return Ok(None)


def shutdown():
    global _tex, _fig, _line
    if _tex and _tex.valid:
        yetty_card.deallocate_texture(_tex)
    _tex = None
    _fig = None
    _line = None
    return Ok(None)
