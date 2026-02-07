#!/bin/bash
# ThorVG Card: SVG rendering - static vector graphics (sunset scene)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/thorvg/sunset.svg)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w 50 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
