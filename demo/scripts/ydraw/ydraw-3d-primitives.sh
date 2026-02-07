#!/bin/bash
# YDraw Card: 3D primitives - spheres, boxes, cylinders with raymarching
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/files/sdf/3d-primitives.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 50 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
