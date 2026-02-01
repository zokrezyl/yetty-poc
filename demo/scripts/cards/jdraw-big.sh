#!/bin/bash
# JDraw Big Demo: Hundreds of SDF primitives with GPU compute shader tile culling
# Uses the card system to create a JDraw card with many shapes
#
# Same as kdraw-big.sh but tile lists are built on GPU via compute shader
# instead of CPU. Compare log output to see timing differences.
#
# Options:
#   --show-tiles       Show 8x8 pixel tile boundaries
#   --show-eval-count  Show heatmap of SDF evaluations per pixel

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Use the yetty-client Python tool to create the JDraw card
uv run python3 tools/yetty-client/main.py create jdraw -i demo/assets/cards/ydraw/big.yaml -w 80 -H 25

echo  # newline after the card
