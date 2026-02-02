#!/bin/bash
# YDraw 3D Primitives: raymarched 3D SDF shapes

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create ydraw -w 80 -H 30 -i demo/files/sdf/3d-primitives.yaml

echo  # newline after the card
