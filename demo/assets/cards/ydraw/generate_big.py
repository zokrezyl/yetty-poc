#!/usr/bin/env python3
"""Generate a big demo YAML file with hundreds of SDF shapes."""

import math
import random

# Set seed for reproducibility
random.seed(42)

# Color palettes
COLORS = [
    "#e74c3c", "#c0392b",  # Red
    "#e67e22", "#d35400",  # Orange
    "#f1c40f", "#f39c12",  # Yellow
    "#2ecc71", "#27ae60",  # Green
    "#1abc9c", "#16a085",  # Teal
    "#3498db", "#2980b9",  # Blue
    "#9b59b6", "#8e44ad",  # Purple
    "#34495e", "#2c3e50",  # Dark gray
    "#ecf0f1", "#bdc3c7",  # Light gray
    "#e91e63", "#9c27b0",  # Pink/Magenta
    "#00bcd4", "#009688",  # Cyan/Teal
    "#ff5722", "#795548",  # Deep orange/Brown
]

def random_color():
    return random.choice(COLORS)

def generate_primitives():
    primitives = []
    layer = 0

    # Grid parameters
    cols = 15
    rows = 10
    cell_w = 50
    cell_h = 40

    # Generate a grid of various shapes
    for row in range(rows):
        for col in range(cols):
            x = 25 + col * cell_w
            y = 20 + row * cell_h

            shape_type = (row * cols + col) % 18
            fill = random_color()
            stroke = random_color()

            if shape_type == 0:
                # Circle
                primitives.append({
                    'circle': {
                        'position': [x, y],
                        'radius': random.uniform(8, 15),
                        'fill': fill,
                        'stroke': stroke,
                        'stroke-width': random.uniform(0, 2)
                    }
                })
            elif shape_type == 1:
                # Box
                primitives.append({
                    'box': {
                        'position': [x, y],
                        'size': [random.uniform(15, 30), random.uniform(12, 25)],
                        'fill': fill,
                        'round': random.uniform(0, 5)
                    }
                })
            elif shape_type == 2:
                # Ellipse
                primitives.append({
                    'ellipse': {
                        'position': [x, y],
                        'radii': [random.uniform(10, 18), random.uniform(6, 12)],
                        'fill': fill
                    }
                })
            elif shape_type == 3:
                # Triangle
                s = random.uniform(10, 18)
                primitives.append({
                    'triangle': {
                        'p0': [x, y - s],
                        'p1': [x + s, y + s * 0.7],
                        'p2': [x - s, y + s * 0.7],
                        'fill': fill
                    }
                })
            elif shape_type == 4:
                # Pentagon
                primitives.append({
                    'pentagon': {
                        'position': [x, y],
                        'radius': random.uniform(10, 16),
                        'fill': fill
                    }
                })
            elif shape_type == 5:
                # Hexagon
                primitives.append({
                    'hexagon': {
                        'position': [x, y],
                        'radius': random.uniform(10, 16),
                        'fill': fill
                    }
                })
            elif shape_type == 6:
                # Star
                primitives.append({
                    'star': {
                        'position': [x, y],
                        'radius': random.uniform(12, 18),
                        'points': random.randint(5, 8),
                        'inner': random.uniform(2.0, 3.5),
                        'fill': fill
                    }
                })
            elif shape_type == 7:
                # Heart
                primitives.append({
                    'heart': {
                        'position': [x, y],
                        'scale': random.uniform(12, 18),
                        'fill': "#e74c3c"  # Hearts are red!
                    }
                })
            elif shape_type == 8:
                # Pie (pac-man style)
                primitives.append({
                    'pie': {
                        'position': [x, y],
                        'angle': random.uniform(30, 60),
                        'radius': random.uniform(10, 16),
                        'fill': fill
                    }
                })
            elif shape_type == 9:
                # Arc
                primitives.append({
                    'arc': {
                        'position': [x, y],
                        'angle': random.uniform(45, 90),
                        'radius': random.uniform(12, 18),
                        'thickness': random.uniform(2, 5),
                        'fill': fill
                    }
                })
            elif shape_type == 10:
                # Ring
                primitives.append({
                    'ring': {
                        'position': [x, y],
                        'angle': random.uniform(0, 45),
                        'radius': random.uniform(10, 15),
                        'thickness': random.uniform(3, 6),
                        'fill': fill
                    }
                })
            elif shape_type == 11:
                # Cross
                primitives.append({
                    'cross': {
                        'position': [x, y],
                        'size': [random.uniform(12, 18), random.uniform(3, 6)],
                        'round': random.uniform(0, 2),
                        'fill': fill
                    }
                })
            elif shape_type == 12:
                # Rounded X
                primitives.append({
                    'rounded_x': {
                        'position': [x, y],
                        'width': random.uniform(15, 22),
                        'round': random.uniform(2, 4),
                        'fill': fill
                    }
                })
            elif shape_type == 13:
                # Capsule
                primitives.append({
                    'capsule': {
                        'from': [x - 10, y],
                        'to': [x + 10, y],
                        'radius': random.uniform(4, 8),
                        'fill': fill
                    }
                })
            elif shape_type == 14:
                # Rhombus
                primitives.append({
                    'rhombus': {
                        'position': [x, y],
                        'size': [random.uniform(8, 14), random.uniform(12, 18)],
                        'fill': fill
                    }
                })
            elif shape_type == 15:
                # Moon
                primitives.append({
                    'moon': {
                        'position': [x, y],
                        'distance': random.uniform(5, 10),
                        'radius_outer': random.uniform(12, 16),
                        'radius_inner': random.uniform(8, 12),
                        'fill': fill
                    }
                })
            elif shape_type == 16:
                # Egg
                primitives.append({
                    'egg': {
                        'position': [x, y],
                        'radius_bottom': random.uniform(10, 14),
                        'radius_top': random.uniform(5, 8),
                        'fill': fill
                    }
                })
            else:
                # Segment (line)
                primitives.append({
                    'segment': {
                        'from': [x - 12, y - 8],
                        'to': [x + 12, y + 8],
                        'stroke': fill,
                        'stroke-width': random.uniform(2, 4)
                    }
                })

            layer += 1

    # Add some bezier curves
    for i in range(15):
        x = random.uniform(50, 700)
        y = random.uniform(50, 350)
        primitives.append({
            'bezier': {
                'p0': [x, y],
                'p1': [x + random.uniform(-30, 30), y + random.uniform(-30, 30)],
                'p2': [x + random.uniform(30, 60), y + random.uniform(-20, 20)],
                'stroke': random_color(),
                'stroke-width': random.uniform(2, 4)
            }
        })

    # Add some overlapping shapes in center
    cx, cy = 375, 200
    for i in range(10):
        angle = i * (2 * math.pi / 10)
        r = 60
        x = cx + r * math.cos(angle)
        y = cy + r * math.sin(angle)
        primitives.append({
            'circle': {
                'position': [x, y],
                'radius': 15,
                'fill': COLORS[i % len(COLORS)],
            }
        })

    return primitives


def main():
    primitives = generate_primitives()

    # Write YAML - single document with background and body
    yaml_content = """# HDraw Big Demo - Hundreds of SDF shapes
# Generated demo showcasing all primitive types
background: "#1a1a2e"
body:
"""

    for prim in primitives:
        for prim_type, params in prim.items():
            yaml_content += f"  - {prim_type}:\n"
            for key, value in params.items():
                if isinstance(value, list):
                    yaml_content += f"      {key}: [{', '.join(str(v) for v in value)}]\n"
                elif isinstance(value, str):
                    yaml_content += f"      {key}: \"{value}\"\n"
                else:
                    yaml_content += f"      {key}: {value}\n"

    with open('/home/misi/work/my/yetty/demo/assets/cards/ydraw/big.yaml', 'w') as f:
        f.write(yaml_content)

    print(f"Generated {len(primitives)} primitives")


if __name__ == '__main__':
    main()
