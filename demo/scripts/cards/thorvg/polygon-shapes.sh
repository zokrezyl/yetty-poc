#!/bin/bash
# ThorVG Polygon Shapes Demo: Various polygons rendered via ThorVG

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

SVG_PATH="demo/assets/cards/thorvg/polygon-shapes.svg"
printf '\033]666666;run -c thorvg -x 0 -y 0 -w 80 -h 20 -r;--svg;%s\033\\' "$SVG_PATH"
echo
