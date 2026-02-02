#!/bin/bash
# YDraw Big Demo: Hundreds of SDF primitives with O(1) spatial hashing
# Uses the card system to create an YDraw card with many shapes
#
# This uses the same primitives as hdraw-big.sh but with spatial hashing
# for O(1) lookup instead of BVH traversal.
#
# Options:
#   --show-grid        Show grid cell overlay
#   --show-eval-count  Show heatmap of SDF evaluations per pixel
#   -c, --cell-size    Grid cell size (default: 50)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Use the yetty-client Python tool to create the YDraw card
uv run python3 tools/yetty-client/main.py create ydraw -i demo/assets/cards/ydraw/big.yaml -w 80 -H 25

echo  # newline after the card
