#!/bin/bash
# YDraw: Combined text and SDF primitives demo
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/ydraw/text-sdf.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 50 -h 22 -r;--yaml;%s\033\\' "$PAYLOAD"
echo
