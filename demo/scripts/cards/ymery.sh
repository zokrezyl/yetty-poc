#!/bin/bash
# Ymery Card: YAML-driven ImGui widget demo
# Creates a card that renders ymery widgets from a YAML layout

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create ymery -i demo/ymery/simple -w 60 -H 30

echo  # newline after the card
