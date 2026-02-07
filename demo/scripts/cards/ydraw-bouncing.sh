#!/bin/bash
# YDraw Animation Demo: bouncing circle, sliding box, pulsing circle

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/x-draw/bouncing.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 80 -h 20 -r;;%s\033\\' "$PAYLOAD"
echo
