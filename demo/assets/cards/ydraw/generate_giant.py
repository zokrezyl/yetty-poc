#!/usr/bin/env python3
"""Generate giant.yaml with ~17,500 SDF primitives."""

import random
import os

random.seed(42)

COLORS = [
    "#00bcd4", "#27ae60", "#e67e22", "#2ecc71", "#e74c3c", "#1abc9c",
    "#d35400", "#9b59b6", "#2980b9", "#009688", "#34495e", "#c0392b",
    "#f1c40f", "#f39c12", "#e91e63", "#8e44ad", "#3498db", "#2c3e50",
    "#bdc3c7", "#ecf0f1", "#ff5722", "#795548", "#16a085",
]

PRIMITIVE_TYPES = [
    "circle", "box", "ellipse", "triangle", "pentagon", "hexagon",
    "star", "heart", "pie", "arc", "ring", "cross", "rounded_x",
    "capsule", "rhombus", "moon", "egg", "segment",
]

COL_SPACING = 50
ROW_SPACING = 40
NUM_COLS = 100
NUM_ROWS = 100

def rc():
    return random.choice(COLORS)

def rf(lo, hi):
    return round(random.uniform(lo, hi), 1)

def ri(lo, hi):
    return random.randint(lo, hi)

def gen_primitive(ptype, cx, cy):
    lines = []
    if ptype == "circle":
        lines.append(f"  - circle:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      radius: {rf(5, 18)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "box":
        lines.append(f"  - box:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      size: [{rf(10, 28)}, {rf(10, 26)}]")
        lines.append(f"      fill: \"{rc()}\"")
        lines.append(f"      round: {rf(0.1, 5)}")
    elif ptype == "ellipse":
        lines.append(f"  - ellipse:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      radii: [{rf(5, 18)}, {rf(5, 12)}]")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "triangle":
        h = rf(10, 24)
        w = rf(10, 28)
        lines.append(f"  - triangle:")
        lines.append(f"      p0: [{cx}, {round(cy - h/2, 1)}]")
        lines.append(f"      p1: [{round(cx + w/2, 1)}, {round(cy + h/2, 1)}]")
        lines.append(f"      p2: [{round(cx - w/2, 1)}, {round(cy + h/2, 1)}]")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "pentagon":
        lines.append(f"  - pentagon:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      radius: {rf(5, 18)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "hexagon":
        lines.append(f"  - hexagon:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      radius: {rf(5, 18)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "star":
        lines.append(f"  - star:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      radius: {rf(8, 18)}")
        lines.append(f"      points: {ri(5, 8)}")
        lines.append(f"      inner: {rf(2, 4)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "heart":
        lines.append(f"  - heart:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      scale: {rf(10, 18)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "pie":
        lines.append(f"  - pie:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      angle: {rf(5, 90)}")
        lines.append(f"      radius: {rf(5, 18)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "arc":
        lines.append(f"  - arc:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      angle: {rf(5, 90)}")
        lines.append(f"      radius: {rf(5, 18)}")
        lines.append(f"      thickness: {rf(2, 6)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "ring":
        lines.append(f"  - ring:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      angle: {rf(5, 90)}")
        lines.append(f"      radius: {rf(5, 18)}")
        lines.append(f"      thickness: {rf(2, 6)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "cross":
        lines.append(f"  - cross:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      size: [{rf(8, 18)}, {rf(3, 8)}]")
        lines.append(f"      round: {rf(0.1, 5)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "rounded_x":
        lines.append(f"  - rounded_x:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      width: {rf(8, 20)}")
        lines.append(f"      round: {rf(0.1, 5)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "capsule":
        offset = rf(5, 15)
        lines.append(f"  - capsule:")
        lines.append(f"      from: [{round(cx - offset, 1)}, {cy}]")
        lines.append(f"      to: [{round(cx + offset, 1)}, {cy}]")
        lines.append(f"      radius: {rf(3, 8)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "rhombus":
        lines.append(f"  - rhombus:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      size: [{rf(8, 18)}, {rf(8, 18)}]")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "moon":
        lines.append(f"  - moon:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      distance: {rf(5, 10)}")
        lines.append(f"      radius_outer: {rf(10, 16)}")
        lines.append(f"      radius_inner: {rf(8, 12)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "egg":
        lines.append(f"  - egg:")
        lines.append(f"      position: [{cx}, {cy}]")
        lines.append(f"      radius_bottom: {rf(10, 14)}")
        lines.append(f"      radius_top: {rf(5, 8)}")
        lines.append(f"      fill: \"{rc()}\"")
    elif ptype == "segment":
        dx = rf(5, 15)
        dy = rf(5, 12)
        lines.append(f"  - segment:")
        lines.append(f"      from: [{round(cx - dx, 1)}, {round(cy - dy, 1)}]")
        lines.append(f"      to: [{round(cx + dx, 1)}, {round(cy + dy, 1)}]")
        lines.append(f"      stroke: \"{rc()}\"")
        lines.append(f"      stroke-width: {rf(0.3, 4)}")
    return lines


def main():
    output_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "giant.yaml")
    count = 0

    with open(output_path, "w") as f:
        f.write('background: "#1a1a2e"\n')
        f.write("body:\n")

        # Part 1: Grid primitives - 100 cols x 100 rows = 10,000
        num_ptypes = len(PRIMITIVE_TYPES)
        for row in range(NUM_ROWS):
            for col in range(NUM_COLS):
                cx = 25 + col * COL_SPACING
                cy = 20 + row * ROW_SPACING
                ptype = PRIMITIVE_TYPES[(row * NUM_COLS + col) % num_ptypes]
                lines = gen_primitive(ptype, cx, cy)
                for line in lines:
                    f.write(line + "\n")
                count += 1

        # Part 2: ~5,000 bezier curves scattered randomly
        scene_w = 5000
        scene_h = 4000
        for _ in range(5000):
            x = rf(50, scene_w - 50)
            y = rf(50, scene_h - 50)
            dx1 = rf(-40, 40)
            dy1 = rf(-40, 40)
            dx2 = rf(-40, 40)
            dy2 = rf(-40, 40)
            f.write(f"  - bezier:\n")
            f.write(f"      p0: [{x}, {y}]\n")
            f.write(f"      p1: [{round(x + dx1, 1)}, {round(y + dy1, 1)}]\n")
            f.write(f"      p2: [{round(x + dx2, 1)}, {round(y + dy2, 1)}]\n")
            f.write(f"      stroke: \"{rc()}\"\n")
            f.write(f"      stroke-width: {rf(0.3, 4)}\n")
            count += 1

        # Part 3: ~2,500 circles in concentric ring patterns
        # 50 ring centers, each with 50 concentric circles
        num_centers = 50
        circles_per_center = 50
        for _ in range(num_centers):
            center_x = rf(100, scene_w - 100)
            center_y = rf(100, scene_h - 100)
            base_color_idx = ri(0, len(COLORS) - 1)
            for j in range(circles_per_center):
                r = 3 + j * 1.5
                color = COLORS[(base_color_idx + j) % len(COLORS)]
                f.write(f"  - circle:\n")
                f.write(f"      position: [{round(center_x, 1)}, {round(center_y, 1)}]\n")
                f.write(f"      radius: {round(r, 1)}\n")
                f.write(f"      stroke: \"{color}\"\n")
                f.write(f"      stroke-width: {rf(0.3, 2)}\n")
                count += 1

    file_size = os.path.getsize(output_path)
    print(f"Total primitives: {count}")
    print(f"File size: {file_size} bytes ({file_size / 1024 / 1024:.2f} MB)")
    print(f"Output: {output_path}")


if __name__ == "__main__":
    main()
