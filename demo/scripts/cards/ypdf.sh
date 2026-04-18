#!/bin/bash
# Display a PDF file using YPdf card (vector text rendering)
#
# Usage: ./ypdf.sh [width] [height]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

WIDTH="${1:-80}"
HEIGHT="${2:-40}"

PDF_FILE="$PROJECT_ROOT/demo/assets/sample-local-pdf.pdf"

if [ ! -f "$PDF_FILE" ]; then
    echo "Error: Sample PDF not found at $PDF_FILE" >&2
    exit 1
fi

PAYLOAD=$(base64 -w0 < "$PDF_FILE")
printf '\033]666666;run -c ypdf -x 0 -y 0 -w %d -h %d -r;-i -;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
