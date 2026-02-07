#!/bin/bash
# Display the yetty logo using the image card
#
# Usage: ./logo.sh [width] [height]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

WIDTH="${1:-40}"
HEIGHT="${2:-20}"

LOGO="$PROJECT_ROOT/docs/logo.jpeg"

if [ ! -f "$LOGO" ]; then
    echo "Error: Logo file not found at $LOGO" >&2
    exit 1
fi

PAYLOAD=$(base64 -w0 < "$LOGO")
printf '\033]666666;run -c image -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
