#!/bin/bash
# YDraw Giant Demo (binary): ~17,500 SDF primitives with O(1) spatial hashing
# Uses pre-converted binary (tools/yaml2bin) to skip slow YAML parsing

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create ydraw -i demo/assets/cards/x-draw/giant.bin --bin -w 80 -H 25

echo  # newline after the card
