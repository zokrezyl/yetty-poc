"""Musical score plugin - display a musical score sheet in terminal."""

import click


@click.command(name='musical-score')
@click.option('--staves', '-s', type=int, default=4, help='Number of staves (1-16)')
@click.pass_context
def musical_score(ctx, staves):
    """Musical score sheet plugin.

    Renders a musical score sheet with staff lines in the terminal using WebGPU.

    Examples:
        yetty-client run musical-score -w 80 -H 30
        yetty-client run musical-score -s 6 -w 100 -H 40
    """
    ctx.ensure_object(dict)

    staves = max(1, min(16, staves))

    # Sheet width is derived from terminal width, passed via -w option
    # We just pass the number of staves as payload
    payload = f"800,{staves}"

    ctx.obj['payload'] = payload
    ctx.obj['plugin_name'] = 'musical-score'
