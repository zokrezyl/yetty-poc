#!/bin/bash
# KDraw Big Demo: Hundreds of SDF primitives with screen-space tile culling
# Uses the card system to create a KDraw card with many shapes
#
# This uses the same primitives as hdraw-big.sh but with screen-space
# tile culling for better GPU cache coherency.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/ydraw/big.yaml)
printf '\033]666666;run -c kdraw -x 0 -y 0 -w 80 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
