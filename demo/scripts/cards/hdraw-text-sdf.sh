#!/bin/bash
# HDraw: Combined text and SDF primitives demo
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."
uv run python3 tools/yetty-client/main.py create hdraw -i demo/assets/cards/hdraw/text-sdf.yaml -w 50 -H 22
