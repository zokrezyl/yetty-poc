#!/bin/bash
# YGrid Card: Color test
# Creates a 8x3 ygrid demonstrating foreground and background colors.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

# Emit one GridCell: codepoint(LE u32) + fgRGBA + bgRGB+style
emit_cell() {
    local val=$(printf '%d' "'$1")
    local fgr=$2 fgg=$3 fgb=$4 bgr=${5:-0} bgg=${6:-0} bgb=${7:-0}
    printf "\\x$(printf '%02x' $val)\\x00\\x00\\x00"
    printf "\\x$(printf '%02x' $fgr)\\x$(printf '%02x' $fgg)\\x$(printf '%02x' $fgb)\\xff"
    printf "\\x$(printf '%02x' $bgr)\\x$(printf '%02x' $bgg)\\x$(printf '%02x' $bgb)\\x00"
}

PAYLOAD=$({
    printf '\x44\x52\x47\x59'  # magic
    printf '\x01\x00'           # flags = FULL
    printf '\x08\x03'           # cols=8, rows=3

    # Row 0: rainbow fg on black
    emit_cell "R" 255 0 0
    emit_cell "A" 255 127 0
    emit_cell "I" 255 255 0
    emit_cell "N" 0 255 0
    emit_cell "B" 0 255 255
    emit_cell "O" 0 127 255
    emit_cell "W" 0 0 255
    emit_cell "!" 255 0 255

    # Row 1: white on colored bg
    emit_cell "C" 255 255 255 128 0 0
    emit_cell "O" 255 255 255 0 128 0
    emit_cell "L" 255 255 255 0 0 128
    emit_cell "O" 255 255 255 128 128 0
    emit_cell "R" 255 255 255 128 0 128
    emit_cell "S" 255 255 255 0 128 128
    emit_cell "!" 255 255 255 64 64 64
    emit_cell "!" 255 255 255 128 64 0

    # Row 2: green on dark green
    emit_cell "M" 0 255 0 0 32 0
    emit_cell "A" 0 255 0 0 32 0
    emit_cell "T" 0 255 0 0 32 0
    emit_cell "R" 0 255 0 0 32 0
    emit_cell "I" 0 255 0 0 32 0
    emit_cell "X" 0 255 0 0 32 0
    emit_cell "!" 0 255 0 0 32 0
    emit_cell "!" 0 255 0 0 32 0
} | base64 -w0)

printf '\033]666666;run -c ygrid -x 0 -y 0 -w 8 -h 3 -r;;%s\033\\' "$PAYLOAD"
echo
