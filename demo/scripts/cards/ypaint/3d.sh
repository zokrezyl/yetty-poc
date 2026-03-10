#!/bin/bash
# YPaint 3D Primitives: raymarched 3D SDF shapes

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

PAYLOAD=$(base64 -w0 < demo/assets/ydraw/3d-primitives.yaml)
printf '\033]666667;run -c ypaint -x 0 -y 0 -w 80 -h 30 -r;--yaml;%s\033\\' "$PAYLOAD"
echo
