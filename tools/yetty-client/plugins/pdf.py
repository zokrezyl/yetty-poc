"""PDF plugin - display PDF documents in terminal."""

import click
from pathlib import Path


@click.command()
@click.argument('file', type=click.Path(exists=True))
@click.option('--page', '-p', type=int, default=1, help='Initial page to display (1-indexed)')
@click.pass_context
def pdf(ctx, file, page):
    """PDF viewer plugin.

    Renders PDF documents using MuPDF and MSDF text rendering.
    Text is extracted from the PDF and rendered using the terminal font.

    Navigation:
        PageUp/PageDown - Previous/next page
        +/-             - Zoom in/out
        Mouse scroll    - Scroll within page

    Examples:
        yetty-client run pdf document.pdf
        yetty-client run pdf --page 5 manual.pdf
    """
    ctx.ensure_object(dict)

    path = Path(file)
    if not path.exists():
        raise click.ClickException(f"File not found: {file}")

    if not path.suffix.lower() == '.pdf':
        raise click.ClickException(f"File must be a PDF: {file}")

    # Payload is just the file path - the C++ plugin handles everything
    payload = str(path.absolute())

    ctx.obj['payload'] = payload
    ctx.obj['plugin_name'] = 'pdf'
    # Store page for potential future use (plugin would need to support it)
    ctx.obj['initial_page'] = page - 1  # Convert to 0-indexed
