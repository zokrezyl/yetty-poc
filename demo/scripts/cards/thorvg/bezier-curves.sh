#!/bin/bash
# Demo: Cubic bezier curves in ThorVG-to-YDraw rendering
# Demonstrates Bezier3 primitive for smooth curves

SCRIPT_DIR="$(dirname "$0")"
ASSET="$SCRIPT_DIR/../../assets/cards/thorvg/bezier-curves.svg"

exec yetty "$ASSET" "$@"
