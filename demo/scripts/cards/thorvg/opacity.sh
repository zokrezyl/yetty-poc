#!/bin/bash
# Demo: SVG opacity support in ThorVG-to-YDraw rendering
# Demonstrates paint opacity being applied to fill/stroke alpha values

SCRIPT_DIR="$(dirname "$0")"
ASSET="$SCRIPT_DIR/../../assets/cards/thorvg/opacity.svg"

exec yetty "$ASSET" "$@"
