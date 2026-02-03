"""YText plugin - GPU-animated scrolling text display with 3D effects."""

import sys
import click
from pathlib import Path


@click.command()
@click.option('--input', '-i', 'input_', help='Text file (use - for stdin)')
@click.option('--inline', '-I', 'inline_', help='Inline text content')
@click.option('--scroll-x', '-sx', 'scroll_x', type=float, default=0.0,
              help='Horizontal scroll speed (pixels/sec)')
@click.option('--scroll-y', '-sy', 'scroll_y', type=float, default=0.0,
              help='Vertical scroll speed (pixels/sec)')
@click.option('--loop', 'loop_mode', is_flag=True, help='Loop scroll mode (seamless cylinder)')
@click.option('--pingpong', 'pingpong_mode', is_flag=True, help='Ping-pong scroll mode')
@click.option('--font-size', '-fs', 'font_size', type=float, default=0.0,
              help='Base font size (default: 16)')
@click.option('--cylinder', 'cylinder_mode', is_flag=True,
              help='Horizontal cylinder effect (for vertical scroll)')
@click.option('--cylinder-v', 'cylinder_v_mode', is_flag=True,
              help='Vertical cylinder effect (for horizontal scroll)')
@click.option('--sphere', 'sphere_mode', is_flag=True,
              help='Sphere effect (for both directions)')
@click.option('--effect-strength', '-es', 'effect_strength', type=float, default=0.0,
              help='Effect curvature intensity (0.0-1.0, default: 0.8)')
@click.option('--tilt-x', '-tx', 'tilt_x', type=float, default=0.0,
              help='Tilt around X axis (radians)')
@click.option('--tilt-y', '-ty', 'tilt_y', type=float, default=0.0,
              help='Tilt around Y axis (radians)')
@click.pass_context
def ytext(ctx, input_, inline_, scroll_x, scroll_y, loop_mode, pingpong_mode, font_size,
          cylinder_mode, cylinder_v_mode, sphere_mode, effect_strength, tilt_x, tilt_y):
    """GPU-animated scrolling text card with 3D effects.

    Animation happens entirely in the fragment shader using the time uniform -
    no CPU-side rebuild needed.

    Examples:
        # Basic scrolling
        yetty-client create ytext -I "Hello World" --scroll-y 50 --loop
        yetty-client create ytext -i story.txt --scroll-y 30

        # Horizontal marquee
        cat text.txt | yetty-client create ytext -i - --scroll-x 60 --loop

        # 3D cylinder effect (text on rotating drum)
        yetty-client create ytext -I "Rolling text..." --scroll-y 40 --loop --cylinder

        # Sphere effect (text on globe)
        yetty-client create ytext -I "Globe" --scroll-x 30 --scroll-y 20 --loop --sphere

        # Tilted cylinder
        yetty-client create ytext -I "Tilted" --scroll-y 50 --loop --cylinder --tilt-y 0.3

    Scroll modes:
        (default)   Clamp - stops at content bounds
        --loop      Loop - seamless wrap-around (cylinder effect)
        --pingpong  Ping-pong - bounces back and forth

    3D Effects:
        --cylinder    Horizontal cylinder (best with vertical scroll)
        --cylinder-v  Vertical cylinder (best with horizontal scroll)
        --sphere      Spherical surface (both directions)
        --effect-strength  Curvature intensity (0.0-1.0)
        --tilt-x/y    Tilt the effect axis (radians)
    """
    ctx.ensure_object(dict)

    if input_ is None and inline_ is None:
        raise click.UsageError("Either --input or --inline must be specified")

    if input_ and inline_:
        raise click.UsageError("Cannot use both --input and --inline")

    # Build plugin args string
    args_parts = []
    if scroll_x != 0.0:
        args_parts.append(f"--scroll-x {scroll_x}")
    if scroll_y != 0.0:
        args_parts.append(f"--scroll-y {scroll_y}")
    if loop_mode:
        args_parts.append("--loop")
    if pingpong_mode:
        args_parts.append("--pingpong")
    if font_size > 0.0:
        args_parts.append(f"--font-size {font_size}")

    # 3D effect options
    if cylinder_mode:
        args_parts.append("--cylinder")
    if cylinder_v_mode:
        args_parts.append("--cylinder-v")
    if sphere_mode:
        args_parts.append("--sphere")
    if effect_strength > 0.0:
        args_parts.append(f"--effect-strength {effect_strength}")
    if tilt_x != 0.0:
        args_parts.append(f"--tilt-x {tilt_x}")
    if tilt_y != 0.0:
        args_parts.append(f"--tilt-y {tilt_y}")

    ctx.obj['plugin_args'] = ' '.join(args_parts)

    # Get content
    if inline_:
        # Inline content - unescape \n sequences
        content = inline_.replace('\\n', '\n')
    elif input_ == '-':
        # Read from stdin
        content = sys.stdin.read()
    else:
        # Read from file
        path = Path(input_)
        if not path.exists():
            raise click.ClickException(f"File not found: {input_}")
        content = path.read_text()

    ctx.obj['payload'] = content
    ctx.obj['plugin_name'] = 'ytext'
