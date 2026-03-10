#!/bin/bash
# YPaint: Text rendering demo
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/ydraw/text.yaml)
printf '\033]666667;run -c ypaint -x 0 -y 0 -w 80 -h 40 -r;--yaml;%s\033\\' "$PAYLOAD"
echo
