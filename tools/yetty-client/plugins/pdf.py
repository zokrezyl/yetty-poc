"""PDF plugin - display PDF documents in terminal."""

import sys
import click
from pathlib import Path


@click.command()
@click.option('--input', '-i', 'input_', required=True, help='PDF file (use - for stdin)')
@click.option('--page', '-p', default=None, type=int, help='Initial page number (0-based)')
@click.option('--zoom', '-z', default=None, type=float, help='Initial zoom level (e.g. 1.0, 2.0)')
@click.pass_context
def pdf(ctx, input_, page, zoom):
    """PDF display plugin.

    Displays PDF documents in the terminal using WebGPU rendering.
    Scroll to navigate pages, Ctrl+Scroll to zoom.

    Example:
        yetty-client create pdf -i document.pdf -w 80 -H 40
        cat document.pdf | yetty-client create pdf -i - -w 80 -H 40
    """
    ctx.ensure_object(dict)

    if input_ == '-':
        pdf_data = sys.stdin.buffer.read()
    else:
        pdf_data = Path(input_).read_bytes()

    ctx.obj['payload_bytes'] = pdf_data
    ctx.obj['plugin_name'] = 'pdf'

    # Build plugin args from options
    args_parts = []
    if page is not None:
        args_parts.append(f'--page {page}')
    if zoom is not None:
        args_parts.append(f'--zoom {zoom}')
    if args_parts:
        ctx.obj['plugin_args'] = ' '.join(args_parts)
