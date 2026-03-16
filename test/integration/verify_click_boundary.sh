#!/bin/bash
# Verify click boundary fix by running the demo with tracing enabled
#
# Usage: ./verify_click_boundary.sh
#
# This runs the debug demo inside yetty and captures trace output.
# Click on the button edges and verify coordinates match.

set -e

YETTY="./build-desktop-ytrace-release/yetty"
DEMO="./demo/scripts/ygui-c/python/00-debug-events.sh"
LOG_FILE="tmp/click_boundary_test.log"

mkdir -p tmp

echo "Starting yetty with debug demo..."
echo "Trace output will be saved to: $LOG_FILE"
echo ""
echo "=== INSTRUCTIONS ==="
echo "1. Click on the LEFT edge of the 'Click Me!' button"
echo "2. Click on the RIGHT edge of the 'Click Me!' button"
echo "3. Click OUTSIDE the button (left and right)"
echo "4. Press 'q' to quit"
echo ""
echo "Watch the trace output for:"
echo "  - cardLocalCoords: shows the float coordinates computed by yetty"
echo "  - sendCardMouseOSC: shows the integer coordinates sent via OSC"
echo "  - SCALE/GRID output: shows ygui-c coordinate transformation"
echo ""
echo "The button after resize should be at approximately:"
echo "  left edge: 19.84, right edge: 218.24"
echo ""

YTRACE_DEFAULT_ON=yes $YETTY -c $DEMO 2>&1 | tee "$LOG_FILE"

echo ""
echo "=== ANALYSIS ==="
echo "Checking for coordinate mismatches..."

# Extract relevant lines
echo ""
echo "cardLocalCoords calls:"
grep -i "cardLocalCoords" "$LOG_FILE" | head -20 || echo "(none found)"

echo ""
echo "sendCardMouseOSC calls:"
grep -i "sendCardMouseOSC" "$LOG_FILE" | head -20 || echo "(none found)"

echo ""
echo "SCALE transformations:"
grep -i "\[SCALE\]" "$LOG_FILE" | head -20 || echo "(none found)"

echo ""
echo "GRID hit tests:"
grep -i "\[GRID\]" "$LOG_FILE" | head -20 || echo "(none found)"
