#!/bin/bash
# Performance profiling script for yetty
# Usage: ./tools/perf.sh

set -e

#cd "$(dirname "$0")/../build"

PERF_DATA="/tmp/yetty_perf.data"
PERF_REPORT="/tmp/yetty_perf_report.txt"

echo "Starting perf record..."
echo "  1. Run: cat ../demo/files/logo.encoded.yetty"
echo "  2. Wait for image to load"
echo "  3. Press Ctrl+D to exit"
echo ""

perf record -g -o "$PERF_DATA" ./build/yetty 2>/dev/null

echo ""
echo "=== Top CPU consumers ==="
perf report -i "$PERF_DATA" --stdio --no-children 2>/dev/null | head -60

echo ""
echo "Full report: perf report -i $PERF_DATA"
