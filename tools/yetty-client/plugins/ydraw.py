"""YDraw plugin - render 2D shapes using signed distance functions."""

import struct
import click
import yaml
from typing import List, Dict, Any, Tuple


# SDF primitive types (must match C++ SDFType enum)
SDF_TYPE_CIRCLE = 0
SDF_TYPE_BOX = 1
SDF_TYPE_SEGMENT = 2
SDF_TYPE_TRIANGLE = 3
SDF_TYPE_BEZIER2 = 4
SDF_TYPE_BEZIER3 = 5
SDF_TYPE_ELLIPSE = 6
SDF_TYPE_ARC = 7


def parse_color(color_str: str) -> int:
    """Parse color string to ABGR packed uint32.

    The shader expects colors packed as 0xAABBGGRR (ABGR format).

    Supports:
      - "#RRGGBB" or "#RGB"
      - "#RRGGBBAA" or "#RGBA"
      - "0xAARRGGBB" (will be converted to ABGR)
    """
    if color_str.startswith('#'):
        hex_str = color_str[1:]
        if len(hex_str) == 3:
            # #RGB -> #RRGGBB
            hex_str = ''.join(c + c for c in hex_str)
        if len(hex_str) == 4:
            # #RGBA -> #RRGGBBAA
            hex_str = ''.join(c + c for c in hex_str)
        if len(hex_str) == 6:
            hex_str += 'FF'  # Add alpha
        r = int(hex_str[0:2], 16)
        g = int(hex_str[2:4], 16)
        b = int(hex_str[4:6], 16)
        a = int(hex_str[6:8], 16)
        # Pack as ABGR (0xAABBGGRR)
        return (a << 24) | (b << 16) | (g << 8) | r
    elif color_str.startswith('0x') or color_str.startswith('0X'):
        # Assume input is 0xAARRGGBB, convert to ABGR
        val = int(color_str, 16)
        a = (val >> 24) & 0xFF
        r = (val >> 16) & 0xFF
        g = (val >> 8) & 0xFF
        b = val & 0xFF
        return (a << 24) | (b << 16) | (g << 8) | r
    else:
        return int(color_str)


def pack_sdf_primitive(
    prim_type: int,
    layer: int,
    params: List[float],
    fill_color: int,
    stroke_color: int,
    stroke_width: float,
    corner_round: float,
    aabb: Tuple[float, float, float, float]  # minX, minY, maxX, maxY
) -> bytes:
    """Pack a single SDF primitive into 96 bytes matching C++ SDFPrimitive struct.

    Layout (96 bytes = 24 floats):
        uint32 type
        uint32 layer
        float params[12]
        uint32 fillColor
        uint32 strokeColor
        float strokeWidth
        float round
        float aabbMinX, aabbMinY, aabbMaxX, aabbMaxY
        uint32 _pad[2]
    """
    # Pad params to 12 floats
    params_padded = list(params) + [0.0] * (12 - len(params))

    return struct.pack(
        '<II12fIIff4fII',  # Little endian
        prim_type,           # type (u32)
        layer,               # layer (u32)
        *params_padded,      # params[12] (12 floats)
        fill_color,          # fillColor (u32)
        stroke_color,        # strokeColor (u32)
        stroke_width,        # strokeWidth (f32)
        corner_round,        # round (f32)
        *aabb,               # aabbMinX, aabbMinY, aabbMaxX, aabbMaxY (4 floats)
        0, 0                 # _pad[2] (2 u32)
    )


def compute_aabb(prim_type: int, params: List[float], stroke_width: float, corner_round: float) -> Tuple[float, float, float, float]:
    """Compute AABB for a primitive."""
    expand = stroke_width * 0.5

    if prim_type == SDF_TYPE_CIRCLE:
        # params: [cx, cy, radius]
        r = params[2] + expand
        return (params[0] - r, params[1] - r, params[0] + r, params[1] + r)

    elif prim_type == SDF_TYPE_BOX:
        # params: [cx, cy, halfW, halfH]
        hw = params[2] + corner_round + expand
        hh = params[3] + corner_round + expand
        return (params[0] - hw, params[1] - hh, params[0] + hw, params[1] + hh)

    elif prim_type == SDF_TYPE_SEGMENT:
        # params: [x0, y0, x1, y1]
        return (
            min(params[0], params[2]) - expand,
            min(params[1], params[3]) - expand,
            max(params[0], params[2]) + expand,
            max(params[1], params[3]) + expand
        )

    elif prim_type == SDF_TYPE_TRIANGLE:
        # params: [x0, y0, x1, y1, x2, y2]
        return (
            min(params[0], params[2], params[4]) - expand,
            min(params[1], params[3], params[5]) - expand,
            max(params[0], params[2], params[4]) + expand,
            max(params[1], params[3], params[5]) + expand
        )

    elif prim_type == SDF_TYPE_BEZIER2:
        # params: [x0, y0, x1, y1, x2, y2]
        return (
            min(params[0], params[2], params[4]) - expand,
            min(params[1], params[3], params[5]) - expand,
            max(params[0], params[2], params[4]) + expand,
            max(params[1], params[3], params[5]) + expand
        )

    elif prim_type == SDF_TYPE_ELLIPSE:
        # params: [cx, cy, rx, ry]
        return (
            params[0] - params[2] - expand,
            params[1] - params[3] - expand,
            params[0] + params[2] + expand,
            params[1] + params[3] + expand
        )

    elif prim_type == SDF_TYPE_ARC:
        # params: [cx, cy, sc.x, sc.y, ra, rb]
        r = max(params[4], params[5]) + expand
        return (params[0] - r, params[1] - r, params[0] + r, params[1] + r)

    else:
        # Unknown - use large bounds
        return (-1e10, -1e10, 1e10, 1e10)


def parse_primitive(prim_dict: Dict[str, Any], layer: int) -> bytes:
    """Parse a primitive dictionary and return binary data."""
    prim_type = None
    params = []
    fill_color = 0
    stroke_color = 0
    stroke_width = 0.0
    corner_round = 0.0

    # Check for primitive type
    if 'circle' in prim_dict:
        prim_type = SDF_TYPE_CIRCLE
        c = prim_dict['circle']
        pos = c.get('position', [0, 0])
        params = [pos[0], pos[1], c.get('radius', 10)]
        fill_color = parse_color(c.get('fill', '#ffffff'))
        if 'stroke' in c:
            stroke_color = parse_color(c['stroke'])
        stroke_width = c.get('stroke-width', 0)

    elif 'box' in prim_dict:
        prim_type = SDF_TYPE_BOX
        b = prim_dict['box']
        pos = b.get('position', [0, 0])
        size = b.get('size', [20, 20])
        params = [pos[0], pos[1], size[0] / 2, size[1] / 2]
        fill_color = parse_color(b.get('fill', '#ffffff'))
        if 'stroke' in b:
            stroke_color = parse_color(b['stroke'])
        stroke_width = b.get('stroke-width', 0)
        corner_round = b.get('round', 0)

    elif 'segment' in prim_dict:
        prim_type = SDF_TYPE_SEGMENT
        s = prim_dict['segment']
        p0 = s.get('from', [0, 0])
        p1 = s.get('to', [100, 100])
        params = [p0[0], p0[1], p1[0], p1[1]]
        if 'stroke' in s:
            stroke_color = parse_color(s['stroke'])
        else:
            stroke_color = parse_color('#ffffff')
        stroke_width = s.get('stroke-width', 1)

    elif 'triangle' in prim_dict:
        prim_type = SDF_TYPE_TRIANGLE
        t = prim_dict['triangle']
        p0 = t.get('p0', [0, 0])
        p1 = t.get('p1', [50, 100])
        p2 = t.get('p2', [100, 0])
        params = [p0[0], p0[1], p1[0], p1[1], p2[0], p2[1]]
        fill_color = parse_color(t.get('fill', '#ffffff'))
        if 'stroke' in t:
            stroke_color = parse_color(t['stroke'])
        stroke_width = t.get('stroke-width', 0)

    elif 'ellipse' in prim_dict:
        prim_type = SDF_TYPE_ELLIPSE
        e = prim_dict['ellipse']
        pos = e.get('position', [0, 0])
        radii = e.get('radii', [20, 10])
        params = [pos[0], pos[1], radii[0], radii[1]]
        fill_color = parse_color(e.get('fill', '#ffffff'))
        if 'stroke' in e:
            stroke_color = parse_color(e['stroke'])
        stroke_width = e.get('stroke-width', 0)

    elif 'bezier' in prim_dict:
        prim_type = SDF_TYPE_BEZIER2
        b = prim_dict['bezier']
        p0 = b.get('p0', [0, 0])
        p1 = b.get('p1', [50, 50])
        p2 = b.get('p2', [100, 0])
        params = [p0[0], p0[1], p1[0], p1[1], p2[0], p2[1]]
        if 'stroke' in b:
            stroke_color = parse_color(b['stroke'])
        else:
            stroke_color = parse_color('#ffffff')
        stroke_width = b.get('stroke-width', 1)

    else:
        return b''  # Unknown primitive type

    if prim_type is None:
        return b''

    # Compute AABB
    aabb = compute_aabb(prim_type, params, stroke_width, corner_round)

    return pack_sdf_primitive(
        prim_type, layer, params,
        fill_color, stroke_color, stroke_width, corner_round,
        aabb
    )


def parse_yaml_to_binary(yaml_content: str) -> Tuple[bytes, int]:
    """Parse YAML content and convert to binary primitives.

    Returns:
        (binary_data, bg_color)
    """
    primitives = []
    bg_color = 0xFF2E1A1A  # Default dark background (ABGR: alpha=FF, B=2E, G=1A, R=1A)

    try:
        docs = list(yaml.safe_load_all(yaml_content))
    except yaml.YAMLError as e:
        raise ValueError(f"Failed to parse YAML: {e}")

    layer = 0
    for doc in docs:
        if not doc:
            continue

        if isinstance(doc, list):
            # List of items
            for item in doc:
                if isinstance(item, dict):
                    if 'body' in item:
                        # Process body items
                        for prim in item['body']:
                            data = parse_primitive(prim, layer)
                            if data:
                                primitives.append(data)
                                layer += 1
                    else:
                        # Direct primitive
                        data = parse_primitive(item, layer)
                        if data:
                            primitives.append(data)
                            layer += 1
        elif isinstance(doc, dict):
            if 'body' in doc:
                for prim in doc['body']:
                    data = parse_primitive(prim, layer)
                    if data:
                        primitives.append(data)
                        layer += 1
            if 'background' in doc:
                bg_color = parse_color(doc['background'])

    # Header: [version (u32)][primitive_count (u32)][bg_color (u32)][flags (u32)]
    header = struct.pack('<IIII', 1, len(primitives), bg_color, 0)

    return header + b''.join(primitives), bg_color


@click.command(name='ydraw')
@click.option('--file', '-f', type=click.Path(exists=True), help='YAML file with primitives')
@click.option('--demo', '-d', type=click.Choice(['2d', 'simple']),
              help='Run a built-in demo')
@click.pass_context
def ydraw(ctx, file, demo):
    """YDraw plugin - 2D SDF rendering.

    GPU-accelerated rendering using signed distance functions.
    Based on Inigo Quilez's SDF functions (iquilezles.org).

    2D Primitives:
        circle, box, segment, triangle, bezier, ellipse

    Examples:
        yetty-client create ydraw -f shapes.yaml -w 80 -H 40
        yetty-client create ydraw --demo 2d -w 60 -H 40
    """
    ctx.ensure_object(dict)

    if file:
        with open(file, 'r') as f:
            yaml_content = f.read()
    elif demo:
        yaml_content = DEMOS.get(demo, DEMOS['simple'])
    else:
        yaml_content = DEMOS['simple']

    try:
        payload_bytes, bg_color = parse_yaml_to_binary(yaml_content)
    except ValueError as e:
        raise click.ClickException(str(e))

    ctx.obj['payload_bytes'] = payload_bytes
    ctx.obj['plugin_name'] = 'ydraw'
    ctx.obj['plugin_args'] = f'--bg-color 0x{bg_color:08X}'


command = ydraw


DEMOS = {
    'simple': """
# YDraw Simple Demo - Basic primitives
- body:
    - circle:
        position: [100, 80]
        radius: 35
        fill: "#e74c3c"
        stroke: "#c0392b"
        stroke-width: 3

    - box:
        position: [220, 80]
        size: [60, 45]
        fill: "#3498db"
        stroke: "#2980b9"
        stroke-width: 2
        round: 8

    - segment:
        from: [50, 150]
        to: [300, 150]
        stroke: "#2ecc71"
        stroke-width: 4
""",

    '2d': """
# YDraw 2D Demo - Multiple primitives
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
"""
}
