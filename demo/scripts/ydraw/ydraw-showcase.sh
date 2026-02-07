#!/bin/bash
# YDraw Card: 2D showcase - circles, rectangles, lines with GPU SDF rendering
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/files/sdf/showcase.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 60 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
