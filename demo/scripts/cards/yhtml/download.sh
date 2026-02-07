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

URL="${1:-https://news.google.com}"
WIDTH="${2:-80}"
HEIGHT="${3:-40}"

echo "Rendering: $URL"

PAYLOAD=$(echo -n "$URL" | base64 -w0)
printf '\033]666666;run -c yhtml -x 0 -y 0 -w %d -h %d -r;url;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
