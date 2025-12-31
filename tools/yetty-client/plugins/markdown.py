"""Markdown plugin - display markdown content in terminal."""

import sys
import click
from pathlib import Path


@click.command()
@click.option('--input', '-i', 'input_', help='Markdown file (use - for stdin)')
@click.option('--inline', '-I', 'inline_', help='Inline markdown content')
@click.pass_context
def markdown(ctx, input_, inline_):
    """Markdown display plugin.

    Renders markdown content using MSDF font atlas with styled text.
    Supports headers, bold, italic, code, and bullet lists.

    Examples:
        yetty-client run markdown -i README.md
        yetty-client run markdown -I "# Hello\\n**Bold** and *italic*"
        cat README.md | yetty-client run markdown -i -

    Supported markdown features:
        # Header 1        - Level 1-6 headers with scaling
        **bold**          - Bold text
        *italic*          - Italic text
        ***bold italic*** - Bold + italic
        `code`            - Inline code (green)
        - item            - Bullet lists with indent
    """
    ctx.ensure_object(dict)

    if input_ is None and inline_ is None:
        raise click.UsageError("Either --input or --inline must be specified")

    if input_ and inline_:
        raise click.UsageError("Cannot use both --input and --inline")

    if inline_:
        # Inline content - unescape \n sequences
        content = inline_.replace('\\n', '\n')
        payload = f"inline:{content}"
    elif input_ == '-':
        # Read from stdin
        content = sys.stdin.read()
        payload = f"inline:{content}"
    else:
        # File path - plugin will load it directly
        path = Path(input_)
        if not path.exists():
            raise click.ClickException(f"File not found: {input_}")
        payload = str(path.absolute())

    ctx.obj['payload'] = payload
    ctx.obj['plugin_name'] = 'markdown'
