#!/bin/bash
# YHtml: Form layout demo
#
# Usage: ./form.sh [width] [height]
#
# Shows styled form fields, buttons, blockquotes, and notes.
# Demonstrates CSS box model rendering.
#
# Requires: yetty built with -DYETTY_CARD_YHTML=ON

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

WIDTH="${1:-70}"
HEIGHT="${2:-35}"

HTML_FILE="$PROJECT_ROOT/demo/assets/yhtml/form.html"

if [ ! -f "$HTML_FILE" ]; then
    echo "Error: HTML file not found at $HTML_FILE" >&2
    exit 1
fi

uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$HTML_FILE" \
    --bg-color 1e1e2e \
    -w "$WIDTH" -H "$HEIGHT"
