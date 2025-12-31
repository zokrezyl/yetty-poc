"""SDF Primitives plugin - render 2D shapes using signed distance functions."""

import click
from pathlib import Path


@click.command()
@click.option('--file', '-f', type=click.Path(exists=True), help='YAML file with primitives')
@click.option('--demo', '-d', type=click.Choice(['basic', 'shapes', 'widget']),
              help='Run a built-in demo')
@click.pass_context
def sdf_primitives(ctx, file, demo):
    """SDF Primitives plugin.

    Renders 2D shapes using GPU-accelerated signed distance functions.
    Based on Inigo Quilez's SDF functions (iquilezles.org).

    Supports both YAML primitives and SVG files:
    - YAML: sdCircle, sdBox, sdSegment, sdTriangle, sdBezier, sdArc,
            sdEllipse, sdCubicBezier, sdEllipseArc
    - SVG: circle, ellipse, rect, line, polyline, polygon, path
           (including all path commands: M, L, H, V, C, S, Q, T, A, Z)

    Examples:
        yetty-client run sdf_primitives -f shapes.yaml -w 80 -H 40
        yetty-client run sdf_primitives -f image.svg -w 80 -H 60
        yetty-client run sdf_primitives --demo basic -w 60 -H 30
    """
    ctx.ensure_object(dict)

    if file:
        with open(file, 'r') as f:
            payload = f.read()
    elif demo:
        payload = DEMOS.get(demo, DEMOS['basic'])
    else:
        payload = DEMOS['basic']

    ctx.obj['payload'] = payload
    ctx.obj['plugin_name'] = 'sdf-primitives'


# Alias for CLI (allows both sdf_primitives and sdf-primitives)
command = sdf_primitives


DEMOS = {
    'basic': """
- body:
    - sdCircle:
        position: [100, 80]
        radius: 30
        fill: "#ff5500"

    - sdBox:
        position: [200, 80]
        size: [40, 25]
        fill: "#3366cc"
        stroke: "#000000"
        stroke-width: 2
        round: 5

    - sdSegment:
        from: [50, 150]
        to: [250, 150]
        stroke: "#333333"
        stroke-width: 3

    - sdTriangle:
        p0: [300, 50]
        p1: [350, 110]
        p2: [250, 110]
        fill: "#22aa44"
        stroke: "#000000"
        stroke-width: 2
""",

    'shapes': """
- body:
    - sdCircle:
        position: [80, 60]
        radius: 40
        fill: "#e74c3c"

    - sdCircle:
        position: [80, 60]
        radius: 20
        fill: "#ffffff"

    - sdBox:
        position: [200, 60]
        size: [50, 35]
        fill: "#3498db"
        round: 8

    - sdBox:
        position: [320, 60]
        size: [45, 45]
        fill: "#9b59b6"
        rotate: 45

    - sdBezier:
        p0: [50, 150]
        p1: [150, 100]
        p2: [250, 180]
        stroke: "#e67e22"
        stroke-width: 4

    - sdArc:
        position: [350, 140]
        angle: [0, 180]
        radius: 35
        thickness: 5
        fill: "#1abc9c"

    - sdEllipse:
        position: [80, 220]
        size: [50, 30]
        fill: "#9b59b6"

    - sdCubicBezier:
        p0: [150, 200]
        p1: [200, 160]
        p2: [280, 250]
        p3: [350, 200]
        stroke: "#e74c3c"
        stroke-width: 3
""",

    'widget': """
- struct:
    name: button-style
    args: [color]
    body:
      fill: $color
      stroke: "#000000"
      stroke-width: 2
      round: 6

- widget:
    name: icon-button
    args: [pos, bg-color, icon-color]
    body:
      - sdBox:
          position: $pos
          size: [35, 25]
          fill: $bg-color
          stroke: "#333333"
          stroke-width: 1
          round: 5
      - sdCircle:
          position: $pos
          radius: 10
          fill: $icon-color

- body:
    - icon-button:
        pos: [80, 60]
        bg-color: "#3498db"
        icon-color: "#ffffff"

    - icon-button:
        pos: [160, 60]
        bg-color: "#e74c3c"
        icon-color: "#ffffff"

    - icon-button:
        pos: [240, 60]
        bg-color: "#2ecc71"
        icon-color: "#ffffff"

    - sdBox:
        position: [160, 130]
        size: [120, 40]
        fill: "#34495e"
        round: 8
"""
}
