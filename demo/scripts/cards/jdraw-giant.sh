#!/bin/bash
# JDraw Giant Demo: ~17,500 SDF primitives with GPU compute shader tile culling
# Stress test for comparing GPU vs CPU tile culling (see kdraw-giant.sh)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create jdraw -i demo/assets/cards/x-draw/giant.yaml -w 80 -H 25

echo  # newline after the card
