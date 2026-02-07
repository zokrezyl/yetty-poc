#!/bin/bash
# YDraw Card: 3D showcase - advanced scene with lighting and shadows
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/files/sdf/3d-showcase.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 50 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
