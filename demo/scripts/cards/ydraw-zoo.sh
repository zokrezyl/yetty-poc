#!/bin/bash
# YDraw Zoo: infinite zoom with random shapes

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create ydraw-zoo -w 80 -H 30

echo  # newline after the card
