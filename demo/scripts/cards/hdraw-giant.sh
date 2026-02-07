#!/bin/bash
# HDraw Giant Demo: ~17,500 SDF primitives with BVH acceleration
# Stress test for comparing BVH vs grid vs tile culling approaches

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/x-draw/giant.yaml)
printf '\033]666666;run -c hdraw -x 0 -y 0 -w 80 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
