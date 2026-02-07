#!/bin/bash
# YDraw 3D Primitives: raymarched 3D SDF shapes

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/files/sdf/3d-primitives.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 80 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
