"""YDraw Zoo plugin - infinite zoom with procedurally spawned shapes."""

import click


@click.command(name='ydraw-zoo')
@click.pass_context
def ydraw_zoo(ctx):
    """YDraw Zoo - infinite zoom demo with random shapes.

    Shapes spawn near the center, grow outward, and get recycled
    when they exit the viewport. Each shape has a decorative
    opacity animation.

    Example:
        yetty-client create ydraw-zoo -w 80 -H 30
    """
    ctx.ensure_object(dict)
    ctx.obj['plugin_name'] = 'ydraw-zoo'
    ctx.obj['plugin_args'] = ''
    ctx.obj['payload'] = ''


command = ydraw_zoo
