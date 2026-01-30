"""Ymery plugin - display YAML-driven ImGui widgets as a card."""

import click
from pathlib import Path


@click.command()
@click.option('--input', '-i', 'input_', required=True, help='YAML layout file or directory')
@click.pass_context
def ymery(ctx, input_):
    """Ymery widget card plugin.

    Renders YAML-defined ImGui widgets into a terminal card.
    Input can be a .yaml file or a layout directory (must contain app.yaml).

    Example:
        yetty-client --new create ymery -i demo/ymery/simple -w 40 -H 20
        yetty-client --new create ymery -i demo/ymery/simple/app.yaml -w 40 -H 20
    """
    ctx.ensure_object(dict)

    path = Path(input_).resolve()
    if not path.exists():
        raise click.ClickException(f"Path not found: {input_}")

    ctx.obj['payload'] = str(path)
    ctx.obj['plugin_name'] = 'ymery'
