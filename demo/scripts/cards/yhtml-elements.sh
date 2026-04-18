#!/bin/bash
# Test YDraw primitives used by yhtml (Box, Segment) to verify rendering

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/ydraw/yhtml-elements.yaml)
printf '\033]666666;run -c ydraw -x 0 -y 0 -w 50 -h 15 -r;--yaml;%s\033\\' "$PAYLOAD"
echo
