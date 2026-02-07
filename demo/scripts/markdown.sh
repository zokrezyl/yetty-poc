#!/bin/bash
# Markdown Card: Renders markdown with headers, bold, italic, code blocks, lists
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../.."

MARKDOWN_FILE="README.md"

if [[ ! -f "$MARKDOWN_FILE" ]]; then
    echo "Error: Markdown file not found at $MARKDOWN_FILE"
    exit 1
fi

PAYLOAD=$(base64 -w0 < "$MARKDOWN_FILE")
printf '\033]666666;run -c markdown -x 2 -y 2 -w 76 -h 35 -r;;%s\033\\' "$PAYLOAD"
echo
