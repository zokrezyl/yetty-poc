#!/bin/bash
# YDraw Giant Demo: ~17,500 SDF primitives with BVH acceleration
# Stress test for comparing BVH vs grid vs tile culling approaches

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create ydraw -f demo/assets/cards/x-draw/giant.yaml -w 80 -H 25
