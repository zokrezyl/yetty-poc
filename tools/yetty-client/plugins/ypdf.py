"""YPdf plugin - display PDF documents as vector text in terminal."""

import click
from pathlib import Path


@click.command()
@click.option('--input', '-i', 'input_', required=True, help='PDF file path')
@click.option('--page', '-p', default=None, type=int, help='Initial page number (0-based)')
@click.option('--font-size', '-f', default=None, type=float, help='Override font size (PDF points)')
@click.option('--color', default=None, help='Text color (hex, e.g. FFE6E6E6)')
@click.option('--bg-color', default=None, help='Background color (hex, e.g. FF2E1A1A)')
@click.pass_context
def ypdf(ctx, input_, page, font_size, color, bg_color):
    """Vector PDF display plugin.

    Renders PDF text as MSDF glyphs using pdfio for parsing.
    Unlike the rasterized 'pdf' card, this produces resolution-independent
    vector text that stays crisp at any zoom level.

    Scroll to zoom, drag to pan.

    Examples:
        yetty-client create ypdf -i document.pdf -w 80 -H 40
        yetty-client create ypdf -i document.pdf -p 2 -w 60 -H 30
    """
    ctx.ensure_object(dict)

    path = Path(input_)
    if not path.exists():
        raise click.ClickException(f"File not found: {input_}")

    # YPdf card takes a file path as payload (pdfio reads files directly)
    ctx.obj['payload'] = str(path.absolute())
    ctx.obj['plugin_name'] = 'ypdf'

    # Build plugin args from options
    args_parts = []
    if page is not None:
        args_parts.append(f'--page {page}')
    if font_size is not None:
        args_parts.append(f'--font-size {font_size}')
    if color is not None:
        args_parts.append(f'--color 0x{color}')
    if bg_color is not None:
        args_parts.append(f'--bg-color 0x{bg_color}')
    if args_parts:
        ctx.obj['plugin_args'] = ' '.join(args_parts)
