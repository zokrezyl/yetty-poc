"""Rich text plugin - display styled text from YAML in terminal."""

import sys
import click
from pathlib import Path


@click.command(name='rich-text')
@click.option('--input', '-i', 'input_', help='YAML file (use - for stdin)')
@click.option('--inline', '-I', 'inline_', help='Inline YAML content')
@click.pass_context
def rich_text(ctx, input_, inline_):
    """Rich text display plugin.

    Renders styled text from YAML using MSDF font atlas.
    Supports positioned text spans with colors, sizes, and styles.

    Examples:
        yetty-client run rich-text -i text.yaml
        yetty-client run rich-text -I "spans:\\n  - text: Hello\\n    size: 24"
        cat text.yaml | yetty-client run rich-text -i -

    YAML format:
        spans:
          - text: "Hello "
            x: 10              # optional, position X
            y: 20              # optional, position Y
            size: 24           # optional, font size (default: 16)
            style: bold        # optional: regular, bold, italic, bolditalic
            color: [1, 0.5, 0, 1]  # optional, RGBA (default: white)
            wrap: true         # optional, word wrap
            maxWidth: 400      # optional, max width for wrapping
            lineHeight: 30     # optional, line height (0 = auto)
          - text: "World"
            size: 16
            color: [0, 1, 0.5, 1]
    """
    ctx.ensure_object(dict)

    if input_ is None and inline_ is None:
        raise click.UsageError("Either --input or --inline must be specified")

    if input_ and inline_:
        raise click.UsageError("Cannot use both --input and --inline")

    if inline_:
        # Inline content - unescape \n sequences
        content = inline_.replace('\\n', '\n')
        payload = content
    elif input_ == '-':
        # Read from stdin
        content = sys.stdin.read()
        payload = content
    else:
        # Read file content
        path = Path(input_)
        if not path.exists():
            raise click.ClickException(f"File not found: {input_}")
        with open(path, 'r') as f:
            payload = f.read()

    ctx.obj['payload'] = payload
    ctx.obj['plugin_name'] = 'rich-text'
