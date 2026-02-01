"""JDraw plugin - render 2D shapes using GPU compute shader tile culling.

Passes YAML to C++ for parsing. Use -i to specify input file.
Same as KDraw but tile lists are built on GPU via compute shader.
"""

import click
import sys
from pathlib import Path


@click.command(name='jdraw')
@click.option('--input', '-i', 'input_', help='YAML file (use - for stdin)')
@click.option('--show-tiles', is_flag=True, help='Show tile boundaries overlay')
@click.option('--show-eval-count', is_flag=True, help='Show heatmap of SDF evaluations')
@click.pass_context
def jdraw(ctx, input_, show_tiles, show_eval_count):
    """JDraw plugin - 2D SDF rendering with GPU compute shader tile culling."""
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
    ctx.obj['plugin_name'] = 'jdraw'
    ctx.obj['plugin_args'] = ' '.join(args_parts)


command = jdraw
