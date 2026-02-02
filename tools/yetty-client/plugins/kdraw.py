"""KDraw plugin - render 2D shapes using screen-space tile culling.

Use -i to specify input file. Use --bin when the input is a pre-converted
binary file (avoids slow YAML parsing in C++).
"""

import click
import sys
from pathlib import Path


@click.command(name='kdraw')
@click.option('--input', '-i', 'input_', help='Input file (YAML or binary, use - for stdin)')
@click.option('--bin', '-b', 'binary', is_flag=True, help='Input is pre-converted binary format')
@click.option('--show-tiles', is_flag=True, help='Show tile boundaries overlay')
@click.option('--show-eval-count', is_flag=True, help='Show heatmap of SDF evaluations')
@click.pass_context
def kdraw(ctx, input_, binary, show_tiles, show_eval_count):
    """KDraw plugin - 2D SDF rendering with screen-space tile culling."""
    ctx.ensure_object(dict)

    # Build args string
    args_parts = []
    if show_tiles:
        args_parts.append('--show-tiles')
    if show_eval_count:
        args_parts.append('--show-eval-count')

    if binary:
        # Binary mode: read file as raw bytes, send directly
        if not input_:
            raise click.ClickException("--bin requires --input/-i")
        if input_ == '-':
            payload_bytes = sys.stdin.buffer.read()
        else:
            path = Path(input_)
            if not path.exists():
                raise click.ClickException(f"File not found: {input_}")
            with open(path, 'rb') as f:
                payload_bytes = f.read()
        ctx.obj['payload_bytes'] = payload_bytes
    else:
        # YAML mode: read as text, pass to C++ for parsing
        yaml_content = ""
        if input_:
            if input_ == '-':
                yaml_content = sys.stdin.read()
            else:
                path = Path(input_)
                if not path.exists():
                    raise click.ClickException(f"File not found: {input_}")
                with open(path, 'r') as f:
                    yaml_content = f.read()
        ctx.obj['payload'] = yaml_content

    ctx.obj['plugin_name'] = 'kdraw'
    ctx.obj['plugin_args'] = ' '.join(args_parts)


command = kdraw
