#!/bin/bash
# YDraw Maze: procedural maze with animated solver
#
# Arguments:
#   --cols N            Maze columns (default 15)
#   --rows N            Maze rows (default 10)
#   --speed F           Actor movement speed in cells/sec (default 4)
#   --wall-color 0xCOL  Wall color ABGR hex (default 0xFF808080)
#   --actor-color 0xCOL Actor color ABGR hex (default 0xFF00CCFF)
#   --start-color 0xCOL Start marker color (default 0xFF00FF00)
#   --end-color 0xCOL   End marker color (default 0xFF0000FF)
#   --bg-color 0xCOL    Background color (default 0xFF1A1A2E)
#   --wall-width F      Wall thickness (default 1.5)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

MODE="${1:-default}"

case "$MODE" in
    large)
        # Large maze: more cells, slower actor
        ARGS="--cols 30 --rows 20 --speed 6 --wall-width 1.0"
        ;;
    small)
        # Small maze: few cells, fast actor
        ARGS="--cols 8 --rows 6 --speed 3 --wall-width 2.0"
        ;;
    fast)
        # Fast actor racing through medium maze
        ARGS="--cols 20 --rows 14 --speed 12"
        ;;
    neon)
        # Neon-themed colors
        ARGS="--cols 15 --rows 10 --wall-color 0xFF00FFFF --actor-color 0xFFFF00FF --start-color 0xFF00FF88 --end-color 0xFF8800FF --bg-color 0xFF0A0A1A"
        ;;
    *)
        # Default
        ARGS=""
        ;;
esac

printf '\033]666666;run -c ydraw-maze -x 0 -y 0 -w 80 -h 30 -r;%s;\033\\' "$ARGS"
echo
