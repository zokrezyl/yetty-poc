"""YDraw plugin - render 2D and 3D shapes using signed distance functions."""

import click
from pathlib import Path


@click.command(name='ydraw')
@click.option('--file', '-f', type=click.Path(exists=True), help='YAML file with primitives')
@click.option('--demo', '-d', type=click.Choice(['2d', '3d', 'mixed', 'showcase']),
              help='Run a built-in demo')
@click.pass_context
def ydraw(ctx, file, demo):
    """YDraw plugin - 2D and 3D SDF rendering.

    GPU-accelerated rendering using signed distance functions.
    Based on Inigo Quilez's SDF functions (iquilezles.org).

    2D Primitives:
        circle, box, segment, triangle, bezier, arc,
        ellipse, cubicBezier, ellipseArc

    3D Primitives (raymarched):
        sphere, box3d, torus, cylinder, capsule, cone,
        octahedron, pyramid

    Examples:
        yetty-client create ydraw -f shapes.yaml -w 80 -H 40
        yetty-client create ydraw --demo 3d -w 60 -H 40
        yetty-client create ydraw --demo mixed -w 80 -H 50
    """
    ctx.ensure_object(dict)

    if file:
        with open(file, 'r') as f:
            payload = f.read()
    elif demo:
        payload = DEMOS.get(demo, DEMOS['2d'])
    else:
        payload = DEMOS['2d']

    ctx.obj['payload'] = payload
    ctx.obj['plugin_name'] = 'ydraw'


command = ydraw


DEMOS = {
    '2d': """
# YDraw 2D Demo
- body:
    - circle:
        position: [100, 80]
        radius: 35
        fill: "#e74c3c"
        stroke: "#c0392b"
        stroke-width: 2

    - box:
        position: [200, 80]
        size: [40, 30]
        fill: "#3498db"
        round: 8

    - triangle:
        p0: [300, 50]
        p1: [340, 110]
        p2: [260, 110]
        fill: "#2ecc71"

    - ellipse:
        position: [100, 160]
        radii: [40, 25]
        fill: "#9b59b6"

    - segment:
        from: [150, 160]
        to: [350, 160]
        stroke: "#34495e"
        stroke-width: 3
""",

    '3d': """
# YDraw 3D Demo - Raymarched Primitives
- body:
    # Row 1
    - sphere:
        position: [-0.6, 0.3, 0]
        radius: 0.2
        fill: "#e74c3c"

    - box3d:
        position: [0, 0.3, 0]
        size: [0.18, 0.18, 0.18]
        fill: "#3498db"

    - torus:
        position: [0.6, 0.3, 0]
        major-radius: 0.18
        minor-radius: 0.06
        fill: "#2ecc71"

    # Row 2
    - cylinder:
        position: [-0.6, -0.3, 0]
        radius: 0.12
        height: 0.25
        fill: "#9b59b6"

    - capsule:
        position: [0, -0.3, 0]
        radius: 0.08
        height: 0.25
        fill: "#f39c12"

    - cone:
        position: [0.6, -0.3, 0]
        radius1: 0.15
        radius2: 0.0
        height: 0.25
        fill: "#1abc9c"
""",

    'mixed': """
# YDraw Mixed 2D/3D Demo
- body:
    # 3D scene (normalized coordinates)
    - sphere:
        position: [0, 0, 0]
        radius: 0.25
        fill: "#e74c3c"

    - torus:
        position: [0, 0, 0.3]
        major-radius: 0.4
        minor-radius: 0.08
        fill: "#3498db"

    # 2D overlays (pixel coordinates)
    - circle:
        position: [40, 30]
        radius: 15
        fill: "#e74c3c"
        stroke: "#fff"
        stroke-width: 2

    - circle:
        position: [70, 30]
        radius: 15
        fill: "#3498db"
        stroke: "#fff"
        stroke-width: 2

    - circle:
        position: [100, 30]
        radius: 15
        fill: "#2ecc71"
        stroke: "#fff"
        stroke-width: 2

    - segment:
        from: [20, 55]
        to: [130, 55]
        stroke: "#7f8c8d"
        stroke-width: 2
""",

    'showcase': """
# YDraw Showcase - All Primitives
- widget:
    name: badge
    args: [x, y, r, color]
    body:
      - circle:
          position: [$x, $y]
          radius: $r
          fill: $color
          stroke: "#ffffff"
          stroke-width: 2

- body:
    # 3D primitives
    - sphere:
        position: [-0.5, 0.4, 0]
        radius: 0.15
        fill: "#e74c3c"

    - box3d:
        position: [0, 0.4, 0]
        size: [0.12, 0.12, 0.12]
        fill: "#3498db"

    - torus:
        position: [0.5, 0.4, 0]
        major-radius: 0.12
        minor-radius: 0.04
        fill: "#2ecc71"

    - octahedron:
        position: [-0.5, -0.1, 0]
        size: 0.15
        fill: "#9b59b6"

    - cylinder:
        position: [0, -0.1, 0]
        radius: 0.08
        height: 0.2
        fill: "#f39c12"

    - pyramid:
        position: [0.5, -0.1, 0]
        height: 0.2
        fill: "#1abc9c"

    # 2D header badges
    - badge: { x: 30, y: 20, r: 12, color: "#e74c3c" }
    - badge: { x: 55, y: 20, r: 12, color: "#3498db" }
    - badge: { x: 80, y: 20, r: 12, color: "#2ecc71" }

    # 2D footer
    - segment:
        from: [20, 380]
        to: [380, 380]
        stroke: "#95a5a6"
        stroke-width: 2

    - box:
        position: [200, 360]
        size: [80, 15]
        fill: "#34495e"
        round: 4
"""
}
