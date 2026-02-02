#!/bin/bash
# ThorVG Card: YAML-defined vector graphics with shapes and colors
# Uses the card system to create a ThorVG card via SwCanvas software renderer

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create thorvg --yaml demo/assets/thorvg/shapes.yaml -w 60 -H 30

echo  # newline after the card
