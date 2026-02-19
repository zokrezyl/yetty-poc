#!/bin/bash
# YGrid Card: Basic character rendering test
# Creates a 10x3 ygrid with ASCII characters. White text on black background.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

# Emit one GridCell (12 bytes): codepoint(LE u32) + fg(RGBA) + bg(RGB+style)
emit_cell() {
    local val=$(printf '%d' "'$1")
    printf "\\x$(printf '%02x' $val)\\x00\\x00\\x00"
    printf '\xc8\xc8\xc8\xff\x00\x00\x00\x00'
}

emit_row() {
    local text="$1" cols=$2
    for ((i=0; i<cols; i++)); do
        if ((i < ${#text})); then
            emit_cell "${text:$i:1}"
        else
            emit_cell " "
        fi
    done
}

PAYLOAD=$({
    printf '\x44\x52\x47\x59'  # magic YGRD (LE)
    printf '\x01\x00'           # flags = FULL
    printf '\x0a\x03'           # cols=10, rows=3
    emit_row "Hello ygrd" 10
    emit_row "ABCDEFGHIJ" 10
    emit_row "0123456789" 10
} | base64 -w0)

printf '\033]666666;run -c ygrid -x 0 -y 0 -w 10 -h 3 -r;;%s\033\\' "$PAYLOAD"
echo
