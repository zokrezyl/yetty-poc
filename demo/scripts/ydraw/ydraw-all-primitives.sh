#!/bin/bash
# YDraw Card: All 2D primitives - complete SDF shape collection
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/files/sdf/all-primitives.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 40 -h 15 -r;;%s\033\\' "$PAYLOAD"
echo
