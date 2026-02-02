#!/bin/bash
# YDraw Animation Demo: bouncing circle, sliding box, pulsing circle

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create ydraw -i demo/assets/cards/x-draw/bouncing.yaml -w 80 -H 20

echo  # newline after the card
