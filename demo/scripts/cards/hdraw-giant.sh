#!/bin/bash
# HDraw Giant Demo: ~17,500 SDF primitives with O(1) spatial hashing
# Stress test for comparing grid vs BVH vs tile culling approaches

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create hdraw -i demo/assets/cards/x-draw/giant.yaml -w 80 -H 25

echo  # newline after the card
