#!/bin/bash
# YDraw Card: Mixed 2D/3D - combined primitives in single widget
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/files/sdf/mixed-2d-3d.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 50 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
