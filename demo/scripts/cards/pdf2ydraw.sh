#!/bin/bash
# PDF via YDraw: Renders PDF text through the ydraw binary v2 pipeline
#
# This demonstrates moving PDF rendering logic outside the terminal.
# The pdf2ydraw tool parses the PDF, extracts fonts, and outputs ydraw
# binary with text spans + embedded font data.
#
# Usage: ./pdf2ydraw.sh [pdf-file] [width] [height]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

PDF_FILE="${1:-$PROJECT_ROOT/demo/assets/sample-local-pdf.pdf}"
WIDTH="${2:-80}"
HEIGHT="${3:-40}"

TOOL="$PROJECT_ROOT/build-desktop-dawn-release/tools/pdf2ydraw/pdf2ydraw"

if [ ! -f "$TOOL" ]; then
    echo "Error: pdf2ydraw not built. Run: cmake --build build-desktop-dawn-release --target pdf2ydraw" >&2
    exit 1
fi

if [ ! -f "$PDF_FILE" ]; then
    echo "Error: PDF not found at $PDF_FILE" >&2
    exit 1
fi

"$TOOL" --osc -x 0 -y 0 -w "$WIDTH" -H "$HEIGHT" "$PDF_FILE"
