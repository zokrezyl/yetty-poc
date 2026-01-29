"""KDraw plugin - render 2D shapes using screen-space tile culling.

Passes YAML to C++ for parsing. Use -i to specify input file.
"""

import click
import sys
from pathlib import Path


@click.command(name='kdraw')
@click.option('--input', '-i', 'input_', help='YAML file (use - for stdin)')
@click.option('--show-tiles', is_flag=True, help='Show tile boundaries overlay')
@click.option('--show-eval-count', is_flag=True, help='Show heatmap of SDF evaluations')
@click.pass_context
def kdraw(ctx, input_, show_tiles, show_eval_count):
    """KDraw plugin - 2D SDF rendering with screen-space tile culling."""
    ctx.ensure_object(dict)

    # Read YAML input
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

    # Build args string
    args_parts = []
    if show_tiles:
        args_parts.append('--show-tiles')
    if show_eval_count:
        args_parts.append('--show-eval-count')

    # Pass YAML to C++ for parsing
    ctx.obj['payload'] = yaml_content
    ctx.obj['plugin_name'] = 'kdraw'
    ctx.obj['plugin_args'] = ' '.join(args_parts)


command = kdraw
