#!/bin/bash
# YHtml: Render a web page directly from URL
#
# Usage: ./download.sh [url] [width] [height]
#   Default URL: https://example.com
#
# The yhtml card fetches the page via cpr (libcurl) internally.
# External CSS referenced in the HTML is also fetched automatically.
#
# Requires: yetty built with -DYETTY_CARD_YHTML=ON

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

URL="${1:-https://news.google.com}"
WIDTH="${2:-80}"
HEIGHT="${3:-40}"

echo "Rendering: $URL"

uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$URL" \
    -w "$WIDTH" -H "$HEIGHT"
