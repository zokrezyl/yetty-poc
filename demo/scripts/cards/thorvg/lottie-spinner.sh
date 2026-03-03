#!/bin/bash
# ThorVG Demo: lottie-spinner (Lottie animation)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

LOTTIE_PATH="demo/assets/cards/thorvg/lottie-spinner.json"
LOTTIE_CONTENT=$(cat "$LOTTIE_PATH")
PAYLOAD=$(echo -n "$LOTTIE_CONTENT" | base64 -w0)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w 80 -h 20 -r;-i - --lottie;%s\033\\' "$PAYLOAD"
echo
