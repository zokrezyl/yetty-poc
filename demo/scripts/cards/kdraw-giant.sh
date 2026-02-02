#!/bin/bash
# KDraw Giant Demo: ~17,500 SDF primitives with CPU tile culling
# Stress test for comparing CPU vs GPU tile culling (see jdraw-giant.sh)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create kdraw -i demo/assets/cards/x-draw/giant.yaml -w 80 -H 25

echo  # newline after the card
