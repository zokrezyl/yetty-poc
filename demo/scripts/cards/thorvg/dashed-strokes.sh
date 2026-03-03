#!/bin/bash
# ThorVG Dashed Strokes Demo: Various dash patterns

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

SVG_PATH="demo/assets/cards/thorvg/dashed-strokes.svg"
printf '\033]666666;run -c thorvg -x 0 -y 0 -w 80 -h 20 -r;--svg;%s\033\\' "$SVG_PATH"
echo
