#!/bin/bash
# KDraw Giant Demo: ~17,500 SDF primitives with CPU tile culling
# Stress test for comparing CPU vs GPU tile culling (see jdraw-giant.sh)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/x-draw/giant.yaml)
printf '\033]666666;run -c kdraw -x 0 -y 0 -w 80 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
