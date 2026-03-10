#!/bin/bash
# YPaint Polygon Demo: Various polygon shapes

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/x-draw/polygon.yaml)
printf '\033]666675;run -c ypaint -x 0 -y 0 -w 80 -h 20 -r;--yaml;%s\033\\' "$PAYLOAD"
echo
