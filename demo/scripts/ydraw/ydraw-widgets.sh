#!/bin/bash
# YDraw Card: UI widgets - buttons, sliders, progress bars with SDF
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/files/sdf/widgets-example.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 40 -h 20 -r;;%s\033\\' "$PAYLOAD"
echo
