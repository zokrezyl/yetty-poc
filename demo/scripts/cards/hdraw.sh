#!/bin/bash
# HDraw Card: SDF primitives rendering demo
# Uses the card system to create a HDraw card with circles, boxes, and lines
#
# This demonstrates the BVH-accelerated SDF rendering system.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/hdraw/simple.yaml)
printf '\033]666666;run -c hdraw -x 0 -y 0 -w 40 -h 20 -r;;%s\033\\' "$PAYLOAD"
echo
