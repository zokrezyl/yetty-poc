#!/bin/bash
# ThorVG Card: SVG rendering - static vector graphics (sunset scene)
# Uses the card system to create a ThorVG card via SwCanvas software renderer

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

python3 tools/yetty-client/main.py create thorvg --svg demo/assets/thorvg/sunset.svg -w 50 -H 25

echo  # newline after the card
