#!/bin/bash
# Demo script to display the yetty logo using the new card system (ImageCard)
#
# Usage: ./logo.sh [width] [height]
#   Default: 40x20 cells
#
# This uses the new card system with shader glyph U+100000 (image card)
# which renders images via a GPU texture atlas.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

# Default size
WIDTH="${1:-40}"
HEIGHT="${2:-20}"

# Logo file
LOGO="$PROJECT_ROOT/docs/logo.jpeg"

if [ ! -f "$LOGO" ]; then
    echo "Error: Logo file not found at $LOGO" >&2
    exit 1
fi

# Use the Python client with --new flag for card system (OSC 666666)
cat "$LOGO" | python3 "$PROJECT_ROOT/tools/yetty-client/main.py" --new create image -i - -w "$WIDTH" -H "$HEIGHT"
