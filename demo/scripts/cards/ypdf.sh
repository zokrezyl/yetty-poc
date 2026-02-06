#!/bin/bash
# Demo script to display a PDF file as vector text using YPdf card
#
# Usage: ./ypdf.sh [width] [height]
#   Default: 80x40 cells
#
# This uses pdfio to parse PDF content streams and renders text as
# MSDF glyphs via the YDraw shader. Text stays crisp at any zoom level.
# Scroll to zoom, drag to pan.
#
# Requires: yetty built with -DYETTY_CARD_YPDF=ON

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

# Use the Python client for card system
uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create ypdf -i "$PDF_FILE" -w "$WIDTH" -H "$HEIGHT"
