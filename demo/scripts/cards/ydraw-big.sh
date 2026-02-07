#!/bin/bash
# YDraw Big Demo: Hundreds of SDF primitives with O(1) spatial hashing

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/ydraw/big.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 80 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
