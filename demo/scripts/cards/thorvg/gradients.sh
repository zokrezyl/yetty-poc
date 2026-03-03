#!/bin/bash
# Demo: SVG gradient fills in ThorVG-to-YDraw rendering
# Demonstrates linear and radial gradient support

SCRIPT_DIR="$(dirname "$0")"
ASSET="$SCRIPT_DIR/../../assets/cards/thorvg/gradients.svg"

exec yetty "$ASSET" "$@"
