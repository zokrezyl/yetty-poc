#!/bin/bash
# JDraw Giant Demo: ~17,500 SDF primitives with GPU compute shader tile culling
# Stress test for comparing GPU vs CPU tile culling (see kdraw-giant.sh)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/x-draw/giant.yaml)
printf '\033]666666;run -c jdraw -x 0 -y 0 -w 80 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
