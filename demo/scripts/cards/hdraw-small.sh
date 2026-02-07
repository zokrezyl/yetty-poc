#!/bin/bash
# HDraw Small Demo - Testing new SDF primitives

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/ydraw/small.yaml)
printf '\033]666666;run -c hdraw -x 0 -y 0 -w 40 -h 10 -r;;%s\033\\' "$PAYLOAD"
echo
