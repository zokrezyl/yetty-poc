#!/bin/bash
# ThorVG demo: All demos combined
#
# Runs all ThorVG demos in sequence

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=== ThorVG Card Demos ==="
echo

echo "1. Simple shapes (circle, rect, line, path):"
"$SCRIPT_DIR/simple-shapes.sh" 40 15
sleep 1

echo "2. Logo with bezier curves:"
"$SCRIPT_DIR/logo.sh" 40 15
sleep 1

echo "3. Bezier curve showcase:"
"$SCRIPT_DIR/beziers.sh" 50 20
sleep 1

echo "4. Icon set:"
"$SCRIPT_DIR/icons.sh" 60 15
sleep 1

echo "5. Chart visualization:"
"$SCRIPT_DIR/chart.sh" 60 20

echo
echo "=== All demos complete ==="
