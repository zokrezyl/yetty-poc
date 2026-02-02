#!/bin/bash
# YDraw: Text rendering demo
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."
uv run python3 tools/yetty-client/main.py create ydraw -i demo/assets/cards/ydraw/text.yaml -w 50 -H 20
