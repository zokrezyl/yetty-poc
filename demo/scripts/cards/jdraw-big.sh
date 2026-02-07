#!/bin/bash
# JDraw Big Demo: Hundreds of SDF primitives with GPU compute shader tile culling
# Uses the card system to create a JDraw card with many shapes
#
# Same as kdraw-big.sh but tile lists are built on GPU via compute shader
# instead of CPU. Compare log output to see timing differences.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/ydraw/big.yaml)
printf '\033]666666;run -c jdraw -x 0 -y 0 -w 80 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
