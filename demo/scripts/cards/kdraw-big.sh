#!/bin/bash
# KDraw Big Demo: Hundreds of SDF primitives with screen-space tile culling
# Uses OSC 666666 to create a KDraw card with many shapes
#
# This uses the same primitives as ydraw-big.sh but with screen-space
# tile culling for better GPU cache coherency.
#
# Options:
#   --show-tiles       Show 8x8 pixel tile boundaries
#   --show-eval-count  Show heatmap of SDF evaluations per pixel

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Use the yetty-client Python tool to create the KDraw card
python3 tools/yetty-client/main.py --new create kdraw -i demo/assets/cards/ydraw/big.yaml -w 80 -H 25

echo  # newline after the card
