#!/bin/bash
# HDraw Big Demo: Hundreds of SDF primitives
# Uses the card system to create a HDraw card with many shapes
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

# Use the yetty-client Python tool to create the HDraw card
python3 tools/yetty-client/main.py create hdraw -f demo/assets/cards/ydraw/big.yaml -w 80 -H 25

echo  # newline after the card
