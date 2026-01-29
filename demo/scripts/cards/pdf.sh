#!/bin/bash
# Demo script to display a PDF file using the new card system (PdfCard)
#
# Usage: ./pdf.sh [width] [height]
#   Default: 80x40 cells
#
# This uses the new card system with shader glyph U+100000 (same as image card)
# which renders PDF pages via PDFium + GPU texture atlas.
# Scroll to navigate pages, Ctrl+Scroll to zoom.
#
# Requires: yetty built with -DYETTY_CARD_PDF=ON

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

# Default size
WIDTH="${1:-80}"
HEIGHT="${2:-40}"

# Sample PDF bundled with the project
PDF_FILE="$PROJECT_ROOT/demo/assets/sample-local-pdf.pdf"

if [ ! -f "$PDF_FILE" ]; then
    echo "Error: Sample PDF not found at $PDF_FILE" >&2
    exit 1
fi

# Use the Python client with --new flag for card system (OSC 666666)
cat "$PDF_FILE" | python3 "$PROJECT_ROOT/tools/yetty-client/main.py" --new create pdf -i - -w "$WIDTH" -H "$HEIGHT"
