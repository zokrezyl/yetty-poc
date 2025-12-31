"""Piano plugin - display a piano keyboard in terminal."""

import click


@click.command()
@click.option('--octaves', '-o', type=int, default=2, help='Number of octaves (1-8)')
@click.option('--start', '-s', type=int, default=4, help='Starting octave (0-9, 4=middle C)')
@click.pass_context
def piano(ctx, octaves, start):
    """Piano keyboard plugin.

    Renders an interactive piano keyboard in the terminal using WebGPU.

    Controls:
      - Mouse: Click keys to play
      - Keyboard: A-L = C to B (white keys), W,E,T,Y,U,O,P = black keys

    Examples:
        yetty-client run piano -w 80 -H 15
        yetty-client run piano -o 3 -s 3 -w 100 -H 20
    """
    ctx.ensure_object(dict)

    octaves = max(1, min(8, octaves))
    start = max(0, min(9, start))

    payload = f"{octaves},{start}"

    ctx.obj['payload'] = payload
    ctx.obj['plugin_name'] = 'piano'
