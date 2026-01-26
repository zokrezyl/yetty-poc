#!/bin/bash
# YDraw Big Demo: Hundreds of SDF primitives
# Uses OSC 666666 to create a YDraw card with many shapes
#
# This demonstrates all the SDF primitive types supported:
# - Circle, Box, Ellipse, Triangle
# - Pentagon, Hexagon, Star
# - Heart, Pie, Arc, Ring
# - Cross, RoundedX, Capsule
# - Rhombus, Moon, Egg
# - Segment, Bezier curves

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Use the yetty-client Python tool to create the YDraw card
# The --new flag uses the card system (OSC 666666) instead of legacy (OSC 999999)
python3 tools/yetty-client/main.py --new create ydraw -f demo/assets/cards/ydraw/big.yaml -w 80 -H 25

echo  # newline after the card
