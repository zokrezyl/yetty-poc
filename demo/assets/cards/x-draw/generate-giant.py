#!/usr/bin/env python3
"""Generate giant.yaml with ~17,500 SDF primitives for kdraw/jdraw stress testing."""

import random
import os

random.seed(42)

COLORS = [
    "#00bcd4", "#27ae60", "#e67e22", "#2ecc71", "#e74c3c", "#1abc9c",
    "#d35400", "#9b59b6", "#2980b9", "#009688", "#34495e", "#c0392b",
    "#f1c40f", "#f39c12", "#e91e63", "#8e44ad", "#3498db", "#2c3e50",
    "#bdc3c7", "#ecf0f1", "#ff5722", "#795548", "#16a085",
]

STROKE_COLORS = [
    "#d35400", "#795548", "#f1c40f", "#ecf0f1", "#00bcd4", "#ff5722",
    "#e91e63", "#bdc3c7", "#9b59b6", "#16a085",
]


def rc():
    return random.choice(COLORS)


def sc():
    return random.choice(STROKE_COLORS)


def rf(lo, hi):
    return round(random.uniform(lo, hi), 6)


def ri(lo, hi):
    return random.randint(lo, hi)


def gen_circle(x, y):
    r = rf(5, 18)
    lines = [
        f"  - circle:",
        f"      position: [{x}, {y}]",
        f"      radius: {r}",
        f'      fill: "{rc()}"',
    ]
    if random.random() < 0.3:
        lines.append(f'      stroke: "{sc()}"')
        lines.append(f"      stroke-width: {rf(0.3, 2.0)}")
    return lines


def gen_box(x, y):
    return [
        f"  - box:",
        f"      position: [{x}, {y}]",
        f"      size: [{rf(10, 28)}, {rf(10, 26)}]",
        f'      fill: "{rc()}"',
        f"      round: {rf(0.1, 5)}",
    ]


def gen_ellipse(x, y):
    return [
        f"  - ellipse:",
        f"      position: [{x}, {y}]",
        f"      radii: [{rf(8, 17)}, {rf(5, 12)}]",
        f'      fill: "{rc()}"',
    ]


def gen_triangle(x, y):
    h = rf(10, 25)
    w = rf(8, 20)
    return [
        f"  - triangle:",
        f"      p0: [{x}, {round(y - h * 0.6, 6)}]",
        f"      p1: [{round(x + w * 0.5, 6)}, {round(y + h * 0.4, 6)}]",
        f"      p2: [{round(x - w * 0.5, 6)}, {round(y + h * 0.4, 6)}]",
        f'      fill: "{rc()}"',
    ]


def gen_pentagon(x, y):
    return [
        f"  - pentagon:",
        f"      position: [{x}, {y}]",
        f"      radius: {rf(8, 16)}",
        f'      fill: "{rc()}"',
    ]


def gen_hexagon(x, y):
    return [
        f"  - hexagon:",
        f"      position: [{x}, {y}]",
        f"      radius: {rf(8, 16)}",
        f'      fill: "{rc()}"',
    ]


def gen_star(x, y):
    return [
        f"  - star:",
        f"      position: [{x}, {y}]",
        f"      radius: {rf(10, 18)}",
        f"      points: {ri(5, 8)}",
        f"      inner: {rf(2, 4)}",
        f'      fill: "{rc()}"',
    ]


def gen_heart(x, y):
    return [
        f"  - heart:",
        f"      position: [{x}, {y}]",
        f"      scale: {rf(10, 18)}",
        f'      fill: "#e74c3c"',
    ]


def gen_pie(x, y):
    return [
        f"  - pie:",
        f"      position: [{x}, {y}]",
        f"      angle: {rf(15, 80)}",
        f"      radius: {rf(10, 16)}",
        f'      fill: "{rc()}"',
    ]


def gen_arc(x, y):
    return [
        f"  - arc:",
        f"      position: [{x}, {y}]",
        f"      angle: {rf(30, 90)}",
        f"      radius: {rf(10, 18)}",
        f"      thickness: {rf(2, 6)}",
        f'      fill: "{rc()}"',
    ]


def gen_ring(x, y):
    return [
        f"  - ring:",
        f"      position: [{x}, {y}]",
        f"      angle: {rf(5, 50)}",
        f"      radius: {rf(10, 15)}",
        f"      thickness: {rf(3, 6)}",
        f'      fill: "{rc()}"',
    ]


def gen_cross(x, y):
    return [
        f"  - cross:",
        f"      position: [{x}, {y}]",
        f"      size: [{rf(10, 18)}, {rf(3, 7)}]",
        f"      round: {rf(0.1, 2)}",
        f'      fill: "{rc()}"',
    ]


def gen_rounded_x(x, y):
    return [
        f"  - rounded_x:",
        f"      position: [{x}, {y}]",
        f"      width: {rf(14, 20)}",
        f"      round: {rf(1, 4)}",
        f'      fill: "{rc()}"',
    ]


def gen_capsule(x, y):
    hw = rf(8, 14)
    return [
        f"  - capsule:",
        f"      from: [{round(x - hw, 6)}, {y}]",
        f"      to: [{round(x + hw, 6)}, {y}]",
        f"      radius: {rf(4, 8)}",
        f'      fill: "{rc()}"',
    ]


def gen_rhombus(x, y):
    return [
        f"  - rhombus:",
        f"      position: [{x}, {y}]",
        f"      size: [{rf(8, 15)}, {rf(10, 18)}]",
        f'      fill: "{rc()}"',
    ]


def gen_moon(x, y):
    return [
        f"  - moon:",
        f"      position: [{x}, {y}]",
        f"      distance: {rf(5, 10)}",
        f"      radius_outer: {rf(10, 16)}",
        f"      radius_inner: {rf(8, 12)}",
        f'      fill: "{rc()}"',
    ]


def gen_egg(x, y):
    return [
        f"  - egg:",
        f"      position: [{x}, {y}]",
        f"      radius_bottom: {rf(10, 14)}",
        f"      radius_top: {rf(5, 8)}",
        f'      fill: "{rc()}"',
    ]


def gen_segment(x, y):
    return [
        f"  - segment:",
        f"      from: [{round(x - 12, 6)}, {round(y - 8, 6)}]",
        f"      to: [{round(x + 12, 6)}, {round(y + 8, 6)}]",
        f'      stroke: "{sc()}"',
        f"      stroke-width: {rf(2, 4)}",
    ]


def gen_bezier(x, y, spread=60):
    x0 = round(x + rf(-spread, spread), 6)
    y0 = round(y + rf(-spread, spread), 6)
    x1 = round(x + rf(-spread, spread), 6)
    y1 = round(y + rf(-spread, spread), 6)
    x2 = round(x + rf(-spread, spread), 6)
    y2 = round(y + rf(-spread, spread), 6)
    return [
        f"  - bezier:",
        f"      p0: [{x0}, {y0}]",
        f"      p1: [{x1}, {y1}]",
        f"      p2: [{x2}, {y2}]",
        f'      stroke: "{rc()}"',
        f"      stroke-width: {rf(2, 4)}",
    ]


GRID_GENERATORS = [
    gen_circle, gen_box, gen_ellipse, gen_triangle,
    gen_pentagon, gen_hexagon, gen_star, gen_heart,
    gen_pie, gen_arc, gen_ring, gen_cross,
    gen_rounded_x, gen_capsule, gen_rhombus, gen_moon,
    gen_egg, gen_segment,
]

COL_SPACING = 50
ROW_SPACING = 40
COLS = 100
ROWS = 100

out_dir = os.path.dirname(os.path.abspath(__file__))
out_path = os.path.join(out_dir, "giant.yaml")

count = 0
with open(out_path, "w") as f:
    f.write('# Giant SDF demo - ~17,500 primitives for kdraw/jdraw stress testing\n')
    f.write('# Generated by generate-giant.py\n')
    f.write('background: "#1a1a2e"\n')
    f.write('body:\n')

    # Grid primitives: 100x100 = 10,000
    for row in range(ROWS):
        y = 20 + row * ROW_SPACING
        for col in range(COLS):
            x = 25 + col * COL_SPACING
            gen = GRID_GENERATORS[(row * COLS + col) % len(GRID_GENERATORS)]
            for line in gen(x, y):
                f.write(line + "\n")
            count += 1

    # Bezier curves scattered across the scene: 5,000
    scene_w = COLS * COL_SPACING
    scene_h = ROWS * ROW_SPACING
    for _ in range(5000):
        cx = rf(50, scene_w - 50)
        cy = rf(50, scene_h - 50)
        for line in gen_bezier(cx, cy, spread=80):
            f.write(line + "\n")
        count += 1

    # Concentric ring clusters: 50 clusters × 50 circles each = 2,500
    import math
    for cluster in range(50):
        center_x = rf(100, scene_w - 100)
        center_y = rf(100, scene_h - 100)
        base_r = rf(30, 80)
        n_circles = 50
        for i in range(n_circles):
            angle = 2 * math.pi * i / n_circles
            r = base_r + rf(-10, 10)
            cx = round(center_x + r * math.cos(angle), 6)
            cy = round(center_y + r * math.sin(angle), 6)
            radius = rf(3, 10)
            f.write(f"  - circle:\n")
            f.write(f"      position: [{cx}, {cy}]\n")
            f.write(f"      radius: {radius}\n")
            f.write(f'      fill: "{rc()}"\n')
            count += 1

print(f"Generated {count} primitives")
print(f"File: {out_path}")
print(f"Size: {os.path.getsize(out_path)} bytes ({os.path.getsize(out_path) / 1024:.1f} KB)")
