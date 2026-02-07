#!/bin/bash
# ThorVG Card: YAML-defined vector graphics with shapes and colors

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/thorvg/shapes.yaml)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w 60 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
