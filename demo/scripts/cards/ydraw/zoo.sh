#!/bin/bash
# YDraw Zoo: infinite zoom with connected control points and curves
#
# Arguments:
#   --points N          Number of control points (default 15)
#   --connections N     Target connections per point (default 3)
#   --growth F          Exponential growth/drift rate (default 0.65)
#   --max-dist F        Max connection distance before breaking (default 200)
#   --marker-size F     Control point marker radius (default 3)
#   --stroke-min F      Min stroke width (default 0.5)
#   --stroke-max F      Max stroke width (default 2.5)
#   --curve-ratio F     Proportion of bezier curves 0-1 (default 0.8)
#   --spawn-radius F,F  Spawn radius min,max (default 15,60)
#   --bg-color 0xCOLOR  Background color ABGR hex (default 0xFF1A1A2E)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

MODE="${1:-default}"

case "$MODE" in
    dense)
        # Dense network: many points, many connections
        ARGS="--points 300 --connections 4 --growth 0.80 --max-dist 300 --stroke-min 0.3 --stroke-max 1.5"
        ;;
    sparse)
        # Sparse network: few points, long connections, thick strokes
        ARGS="--points 8 --connections 2 --growth 0.15 --max-dist 400 --stroke-min 1.0 --stroke-max 3.5 --marker-size 5"
        ;;
    fast)
        # Fast evolution: quick drift, short-lived connections
        ARGS="--points 20 --connections 3 --growth 1.0 --max-dist 120 --spawn-radius 15,60"
        ;;
    curves)
        # All curves, no shapes or segments
        ARGS="--points 15 --connections 3 --curve-ratio 1.0 --stroke-min 0.8 --stroke-max 3.0"
        ;;
    midnight)
        # Dark theme with wide spawn radius
        ARGS="--points 12 --connections 3 --bg-color 0xFF0A0A1A --spawn-radius 20,90 --stroke-max 2.0"
        ;;
    *)
        # Default
        ARGS=""
        ;;
esac

printf '\033]666666;run -c ydraw-zoo -x 0 -y 0 -w 80 -h 30 -r;%s;\033\\' "$ARGS"
echo
