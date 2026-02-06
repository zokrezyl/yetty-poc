#!/bin/bash
# YHtml: Landing page mockup demo
#
# Usage: ./landing.sh [width] [height]
#
# Demonstrates a full page layout with hero section, feature cards,
# CTA button, and footer.
#
# Requires: yetty built with -DYETTY_CARD_YHTML=ON

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

WIDTH="${1:-80}"
HEIGHT="${2:-45}"

HTML_FILE="$PROJECT_ROOT/demo/assets/yhtml/landing.html"

if [ ! -f "$HTML_FILE" ]; then
    echo "Error: HTML file not found at $HTML_FILE" >&2
    exit 1
fi

uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$HTML_FILE" \
    -w "$WIDTH" -H "$HEIGHT"
