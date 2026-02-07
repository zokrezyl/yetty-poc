#!/bin/bash
# ThorVG Card: Long Lottie animation - 14 second confetti celebration
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/files/thorvg/animation-long.json)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w 40 -h 25 -r;lottie;%s\033\\' "$PAYLOAD"
echo
