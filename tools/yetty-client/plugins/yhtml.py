"""YHtml plugin - display HTML/CSS content in terminal via litehtml."""

import sys
import click
from pathlib import Path


def _is_url(s):
    return s.startswith('http://') or s.startswith('https://')


@click.command()
@click.option('--input', '-i', 'input_', help='HTML file path, URL, or - for stdin')
@click.option('--inline', '-I', 'inline_', help='Inline HTML content')
@click.option('--font-size', '-f', default=None, type=float, help='Font size (default: 16)')
@click.option('--bg-color', default=None, help='Background color (hex, e.g. FF1A1A2E)')
@click.pass_context
def yhtml(ctx, input_, inline_, font_size, bg_color):
    """HTML/CSS display plugin.

    Renders HTML content using litehtml for CSS layout and YDraw SDF
    primitives for GPU-accelerated rendering. Text is rendered as MSDF
    glyphs, boxes and borders as SDF shapes.

    The card can fetch web pages directly — just pass a URL.
    External CSS referenced in the HTML is also fetched automatically.

    Scroll to zoom, drag to pan.

    Examples:
        yetty-client create yhtml -i page.html
        yetty-client create yhtml -i https://example.com
        yetty-client create yhtml -I "<h1>Hello</h1><p>World</p>" -w 60 -H 20
        cat page.html | yetty-client create yhtml -i -
    """
    ctx.ensure_object(dict)

    if input_ is None and inline_ is None:
        raise click.UsageError("Either --input or --inline must be specified")

    if input_ and inline_:
        raise click.UsageError("Cannot use both --input and --inline")

    # Build plugin args — URL and file path go here (metadata)
    args_parts = []

    if inline_:
        # Inline HTML → payload
        content = inline_.replace('\\n', '\n')
        ctx.obj['payload'] = content
    elif input_ == '-':
        # Stdin → payload
        ctx.obj['payload'] = sys.stdin.read()
    elif _is_url(input_):
        # URL → plugin args (card fetches via cpr)
        args_parts.append(f'--input {input_}')
        ctx.obj['payload'] = ''
    else:
        # File path → plugin args (card reads from disk)
        path = Path(input_)
        if not path.exists():
            raise click.ClickException(f"File not found: {input_}")
        args_parts.append(f'--input {path.absolute()}')
        ctx.obj['payload'] = ''

    ctx.obj['plugin_name'] = 'yhtml'

    if font_size is not None:
        args_parts.append(f'--font-size {font_size}')
    if bg_color is not None:
        args_parts.append(f'--bg-color 0x{bg_color}')
    if args_parts:
        ctx.obj['plugin_args'] = ' '.join(args_parts)
