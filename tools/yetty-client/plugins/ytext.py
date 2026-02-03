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
@click.option('--loop', 'loop_mode', is_flag=True, help='Loop scroll mode (seamless)')
@click.option('--pingpong', 'pingpong_mode', is_flag=True, help='Ping-pong scroll mode')
@click.option('--font-size', '-fs', 'font_size', type=float, default=0.0,
              help='Base font size (default: 16)')
@click.option('--cylinder', 'cylinder_mode', is_flag=True,
              help='Horizontal cylinder effect (drum rolling vertically)')
@click.option('--cylinder-v', 'cylinder_v_mode', is_flag=True,
              help='Vertical cylinder effect (drum rolling horizontally)')
@click.option('--sphere', 'sphere_mode', is_flag=True,
              help='Sphere effect')
@click.option('--wave-disp', 'wave_disp_mode', is_flag=True,
              help='Horizontal wave displacement (no foreshortening)')
@click.option('--wave-disp-v', 'wave_disp_v_mode', is_flag=True,
              help='Vertical wave displacement (no foreshortening)')
@click.option('--wave-proj', 'wave_proj_mode', is_flag=True,
              help='Horizontal wave projection (text on wavy surface)')
@click.option('--wave-proj-v', 'wave_proj_v_mode', is_flag=True,
              help='Vertical wave projection (text on wavy surface)')
@click.option('--ripple', 'ripple_mode', is_flag=True,
              help='Concentric water ripple effect (text on water surface)')
@click.option('--effect-strength', '-es', 'effect_strength', type=float, default=0.0,
              help='Effect intensity (0.0-1.0, default: 0.8)')
@click.option('--frequency', '-freq', 'frequency', type=float, default=0.0,
              help='Wave frequency (default: 3.0)')
@click.pass_context
def ytext(ctx, input_, inline_, scroll_x, scroll_y, loop_mode, pingpong_mode, font_size,
          cylinder_mode, cylinder_v_mode, sphere_mode, wave_disp_mode, wave_disp_v_mode,
          wave_proj_mode, wave_proj_v_mode, ripple_mode,
          effect_strength, frequency):
    """GPU-animated scrolling text card with 3D effects.

    Animation happens entirely in the fragment shader using the time uniform -
    no CPU-side rebuild needed.

    Examples:
        # Basic scrolling
        yetty-client create ytext -I "Hello World" --scroll-y 50 --loop

        # Cylinder effect (text on rotating drum)
        yetty-client create ytext -I "Rolling..." --scroll-y 40 --loop --cylinder

        # Wave displacement (text moves with ripples, no foreshortening)
        yetty-client create ytext -I "Wavy text" --wave-disp --effect-strength 0.5

        # Wave projection (text on wavy surface, foreshortening)
        yetty-client create ytext -I "Surface text" --wave-proj --effect-strength 0.5

        # Water ripple effect
        yetty-client create ytext -I "Water drop" --ripple --effect-strength 0.5

        # Sphere effect
        yetty-client create ytext -I "Globe" --scroll-y 30 --loop --sphere

    Scroll modes:
        (default)   Clamp - stops at content bounds
        --loop      Loop - seamless wrap-around
        --pingpong  Ping-pong - bounces back and forth

    Projection Effects (foreshortening):
        --cylinder      Horizontal cylinder projection
        --cylinder-v    Vertical cylinder projection
        --sphere        Sphere projection
        --wave-proj     Horizontal wave projection
        --wave-proj-v   Vertical wave projection
        --ripple        Concentric water ripple projection

    Displacement Effects (no foreshortening):
        --wave-disp     Horizontal wave displacement
        --wave-disp-v   Vertical wave displacement

    Parameters:
        --effect-strength  Effect intensity (0.0-1.0)
        --frequency        Wave/ripple frequency
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
    if wave_disp_mode:
        args_parts.append("--wave-disp")
    if wave_disp_v_mode:
        args_parts.append("--wave-disp-v")
    if wave_proj_mode:
        args_parts.append("--wave-proj")
    if wave_proj_v_mode:
        args_parts.append("--wave-proj-v")
    if ripple_mode:
        args_parts.append("--ripple")
    if effect_strength > 0.0:
        args_parts.append(f"--effect-strength {effect_strength}")
    if frequency > 0.0:
        args_parts.append(f"--tilt-x {frequency}")  # Using tilt-x as frequency param

    ctx.obj['plugin_args'] = ' '.join(args_parts)

    # Get content
    if inline_:
        content = inline_.replace('\\n', '\n')
    elif input_ == '-':
        content = sys.stdin.read()
    else:
        path = Path(input_)
        if not path.exists():
            raise click.ClickException(f"File not found: {input_}")
        content = path.read_text()

    ctx.obj['payload'] = content
    ctx.obj['plugin_name'] = 'ytext'
