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

PAYLOAD=$(base64 -w0 < "$HTML_FILE")
printf '\033]666666;run -c yhtml -x 0 -y 0 -w %d -h %d -r;-i - --bg-color 0x1e1e2e;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
