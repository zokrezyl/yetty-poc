#!/bin/bash
# YPaint Animation Demo: bouncing circle, sliding box, pulsing circle

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/x-draw/bouncing.yaml)
printf '\033]666667;run -c ypaint -x 0 -y 0 -w 80 -h 20 -r;--yaml;%s\033\\' "$PAYLOAD"
echo
