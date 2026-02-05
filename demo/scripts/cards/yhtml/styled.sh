#!/bin/bash
# YHtml: CSS styled dashboard demo
#
# Usage: ./styled.sh [width] [height]
#
# Demonstrates backgrounds, borders, border-radius, and CSS colors
# rendered as SDF primitives on the GPU.
#
# Requires: yetty built with -DYETTY_CARD_YHTML=ON

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

WIDTH="${1:-80}"
HEIGHT="${2:-40}"

HTML_FILE="$PROJECT_ROOT/demo/assets/yhtml/styled.html"

if [ ! -f "$HTML_FILE" ]; then
    echo "Error: HTML file not found at $HTML_FILE" >&2
    exit 1
fi

uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$HTML_FILE" \
    --bg-color 11111b \
    -w "$WIDTH" -H "$HEIGHT"
