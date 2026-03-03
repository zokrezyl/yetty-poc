#!/bin/bash
# ThorVG Demo: dashed-strokes

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

SVG_PATH="demo/assets/cards/thorvg/dashed-strokes.svg"
SVG_CONTENT=$(cat "$SVG_PATH")
PAYLOAD=$(echo -n "$SVG_CONTENT" | base64 -w0)
printf '\033]666666;run -c thorvg2 -x 0 -y 0 -w 80 -h 20 -r;-i - --svg;%s\033\\' "$PAYLOAD"
echo
