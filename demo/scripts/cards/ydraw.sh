#!/bin/bash
# YDraw Card: SDF primitives rendering demo
# Uses the card system to create a YDraw card with circles, boxes, and lines
#
# This demonstrates the BVH-accelerated SDF rendering system.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Use the yetty-client Python tool to create the YDraw card
python3 tools/yetty-client/main.py create ydraw --demo simple -w 40 -H 20

echo  # newline after the card
