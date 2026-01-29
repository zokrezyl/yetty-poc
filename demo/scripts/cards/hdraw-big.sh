#!/bin/bash
# HDraw Big Demo: Hundreds of SDF primitives with O(1) spatial hashing
# Uses OSC 666666 to create an HDraw card with many shapes
#
# This uses the same primitives as ydraw-big.sh but with spatial hashing
# for O(1) lookup instead of BVH traversal.
#
# Options:
#   --show-grid        Show grid cell overlay
#   --show-eval-count  Show heatmap of SDF evaluations per pixel
#   -c, --cell-size    Grid cell size (default: 50)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Use the yetty-client Python tool to create the HDraw card
python3 tools/yetty-client/main.py --new create hdraw -i demo/assets/cards/ydraw/big.yaml -w 80 -H 25

echo  # newline after the card
