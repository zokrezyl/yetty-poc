#!/bin/bash
# YDraw Giant Demo: ~17,500 SDF primitives with O(1) spatial hashing
# Stress test for comparing grid vs BVH vs tile culling approaches

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/x-draw/giant.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 80 -h 25 -r;--yaml;%s\033\\' "$PAYLOAD"
echo
