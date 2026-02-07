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

PAYLOAD=$(base64 -w0 < demo/assets/cards/ydraw/big.yaml)
printf '\033]666666;run -c hdraw -x 0 -y 0 -w 80 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
