#!/bin/bash
# YDraw Card: SDF primitives rendering demo
# Uses OSC 666666 to create a YDraw card with circles, boxes, and lines
#
# This demonstrates the BVH-accelerated SDF rendering system.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Use the yetty-client Python tool to create the YDraw card
# The --new flag uses the card system (OSC 666666) instead of legacy (OSC 999999)
python3 tools/yetty-client/main.py --new create ydraw --demo simple -w 40 -H 20

echo  # newline after the card
