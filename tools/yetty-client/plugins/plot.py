"""Plot plugin - display line plots in terminal."""

import sys
import struct
import click
import json
from pathlib import Path
from typing import List, Tuple, Optional


def parse_csv(content: str) -> Tuple[List[List[float]], int, int]:
    """Parse CSV content into NxM matrix.

    Each row is a plot, each column is a data point.
    Returns (data, num_plots, num_points).
    """
    lines = [line.strip() for line in content.strip().split('\n') if line.strip()]
    if not lines:
        raise ValueError("Empty CSV data")

    data = []
    num_points = None

    for line in lines:
        # Handle both comma and whitespace separators
        if ',' in line:
            values = [float(v.strip()) for v in line.split(',') if v.strip()]
        else:
            values = [float(v) for v in line.split()]

        if num_points is None:
            num_points = len(values)
        elif len(values) != num_points:
            raise ValueError(f"Inconsistent row lengths: expected {num_points}, got {len(values)}")

        data.append(values)

    return data, len(data), num_points


def parse_json(content: str) -> Tuple[List[List[float]], int, int]:
    """Parse JSON content into NxM matrix.

    Expected format: [[y1, y2, ...], [y1, y2, ...], ...]
    Returns (data, num_plots, num_points).
    """
    obj = json.loads(content)

    if not isinstance(obj, list) or not obj:
        raise ValueError("JSON must be a non-empty array")

    # Check if it's a 1D array (single plot)
    if not isinstance(obj[0], list):
        obj = [obj]

    data = []
    num_points = None

    for row in obj:
        values = [float(v) for v in row]
        if num_points is None:
            num_points = len(values)
        elif len(values) != num_points:
            raise ValueError(f"Inconsistent row lengths: expected {num_points}, got {len(values)}")
        data.append(values)

    return data, len(data), num_points


def auto_viewport(data: List[List[float]], num_points: int) -> Tuple[float, float, float, float]:
    """Calculate automatic viewport from data.

    Returns (xmin, xmax, ymin, ymax).
    """
    xmin, xmax = 0.0, float(num_points - 1) if num_points > 1 else 1.0

    ymin = float('inf')
    ymax = float('-inf')
    for row in data:
        for v in row:
            ymin = min(ymin, v)
            ymax = max(ymax, v)

    # Add some padding
    yrange = ymax - ymin if ymax > ymin else 1.0
    ymin -= yrange * 0.05
    ymax += yrange * 0.05

    return xmin, xmax, ymin, ymax


def pack_binary(data: List[List[float]], num_plots: int, num_points: int,
                xmin: float, xmax: float, ymin: float, ymax: float) -> bytes:
    """Pack data into binary format for the plot plugin.

    Format: [uint32 N][uint32 M][float xmin][float xmax][float ymin][float ymax][N*M floats]
    """
    header = struct.pack('<II4f', num_plots, num_points, xmin, xmax, ymin, ymax)

    # Flatten data row-major
    floats = []
    for row in data:
        floats.extend(row)

    body = struct.pack(f'<{len(floats)}f', *floats)

    return header + body


@click.command()
@click.option('--input', '-i', 'input_', default=None,
              help='Data file (CSV, JSON, or binary). Use - for stdin')
@click.option('--format', '-f', 'fmt', type=click.Choice(['auto', 'csv', 'json', 'binary']),
              default='auto', help='Input format (default: auto-detect)')
@click.option('--xmin', type=float, default=None, help='X axis minimum (default: auto)')
@click.option('--xmax', type=float, default=None, help='X axis maximum (default: auto)')
@click.option('--ymin', type=float, default=None, help='Y axis minimum (default: auto)')
@click.option('--ymax', type=float, default=None, help='Y axis maximum (default: auto)')
@click.option('--generate', '-g', type=click.Choice(['sine', 'random', 'noise']),
              help='Generate test data instead of reading from file')
@click.option('--plots', '-n', type=int, default=3, help='Number of plots for generated data')
@click.option('--points', '-m', type=int, default=100, help='Number of points for generated data')
@click.pass_context
def plot(ctx, input_, fmt, xmin, xmax, ymin, ymax, generate, plots, points):
    """Plot data visualization plugin.

    Displays line plots in the terminal using GPU rendering.
    Supports multiple plots (up to 16) with auto-colored lines.

    Input formats:
      CSV: Each row is a plot, columns are Y values
      JSON: Array of arrays [[y1,y2,...], [y1,y2,...]]
      Binary: Raw format with header (for programmatic use)

    Examples:
        yetty-client run plot -i data.csv
        yetty-client run plot -i data.json --ymin=-1 --ymax=1
        yetty-client run plot -g sine -n 3 -m 200
        cat data.csv | yetty-client run plot -i -
    """
    ctx.ensure_object(dict)

    import math
    import random

    # Validate: need either input or generate
    if not generate and not input_:
        raise click.ClickException("Either --input or --generate is required")

    # Generate test data if requested
    if generate:
        data = []
        if generate == 'sine':
            for i in range(plots):
                phase = i * math.pi / plots
                freq = 1 + i * 0.5
                row = [math.sin(freq * x * math.pi * 2 / points + phase) for x in range(points)]
                data.append(row)
        elif generate == 'random':
            for i in range(plots):
                row = [random.random() * 2 - 1 for _ in range(points)]
                data.append(row)
        elif generate == 'noise':
            for i in range(plots):
                row = []
                v = 0.0
                for _ in range(points):
                    v += random.gauss(0, 0.1)
                    row.append(v)
                data.append(row)

        num_plots = len(data)
        num_points = points

    else:
        # Read input
        if input_ == '-':
            if fmt == 'binary':
                content = sys.stdin.buffer.read()
            else:
                content = sys.stdin.read()
        else:
            path = Path(input_)
            if not path.exists():
                raise click.ClickException(f"File not found: {input_}")

            if fmt == 'binary':
                content = path.read_bytes()
            else:
                content = path.read_text()

        # Auto-detect format
        if fmt == 'auto':
            if isinstance(content, bytes):
                fmt = 'binary'
            elif content.strip().startswith('['):
                fmt = 'json'
            else:
                fmt = 'csv'

        # Parse input
        if fmt == 'binary':
            # Already in binary format, pass through
            ctx.obj['payload_bytes'] = content
            ctx.obj['plugin_name'] = 'plot'
            return
        elif fmt == 'json':
            data, num_plots, num_points = parse_json(content)
        else:  # csv
            data, num_plots, num_points = parse_csv(content)

    # Calculate viewport
    auto_xmin, auto_xmax, auto_ymin, auto_ymax = auto_viewport(data, num_points)

    final_xmin = xmin if xmin is not None else auto_xmin
    final_xmax = xmax if xmax is not None else auto_xmax
    final_ymin = ymin if ymin is not None else auto_ymin
    final_ymax = ymax if ymax is not None else auto_ymax

    # Pack binary payload
    payload_bytes = pack_binary(data, num_plots, num_points,
                                 final_xmin, final_xmax, final_ymin, final_ymax)

    ctx.obj['payload_bytes'] = payload_bytes
    ctx.obj['plugin_name'] = 'plot'
