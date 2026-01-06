"""Python plugin - execute Python code with pygfx rendering."""

import sys
import click
from pathlib import Path


@click.command()
@click.argument('script', type=click.Path(exists=True), required=False)
@click.option('--code', '-c', help='Python code to execute inline')
@click.pass_context
def python(ctx, script, code):
    """Python plugin for executing Python code with pygfx rendering.

    Execute Python scripts or inline code with access to pygfx graphics library.
    The plugin provides GPU-accelerated rendering through yetty's WebGPU context.

    Examples:
        yetty-client run python script.py
        yetty-client run python -c "print('Hello from Python')"
        cat script.py | yetty-client run python
    """
    ctx.ensure_object(dict)

    # Determine input source
    if code:
        # Inline code
        payload = code.encode('utf-8')
    elif script:
        # Script file
        payload = Path(script).read_bytes()
    else:
        # Read from stdin
        payload = sys.stdin.buffer.read()

    ctx.obj['payload_bytes'] = payload
    ctx.obj['plugin_name'] = 'python'
