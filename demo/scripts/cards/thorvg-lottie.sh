#!/bin/bash
# ThorVG Card: Lottie animation - bouncing ball vector animation
# Uses the card system to create a ThorVG card via SwCanvas software renderer

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

uv run python3 tools/yetty-client/main.py create thorvg --lottie demo/assets/thorvg/bouncing-ball.json -w 40 -H 25

echo  # newline after the card
