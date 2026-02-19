#!/bin/bash
# YGrid Card: Text style attributes test
# Creates a 10x5 ygrid with bold, italic, underline, strikethrough.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

# Style bits: 0x01=bold, 0x02=italic, 0x04=underline, 0x10=strikethrough
emit_cell() {
    local val=$(printf '%d' "'$1")
    local fgr=$2 fgg=$3 fgb=$4 style=${5:-0}
    printf "\\x$(printf '%02x' $val)\\x00\\x00\\x00"
    printf "\\x$(printf '%02x' $fgr)\\x$(printf '%02x' $fgg)\\x$(printf '%02x' $fgb)\\xff"
    printf "\\x00\\x00\\x00\\x$(printf '%02x' $style)"
}

emit_row() {
    local text="$1" fgr=$2 fgg=$3 fgb=$4 style=$5 cols=$6
    for ((i=0; i<cols; i++)); do
        if ((i < ${#text})); then
            emit_cell "${text:$i:1}" "$fgr" "$fgg" "$fgb" "$style"
        else
            emit_cell " " "$fgr" "$fgg" "$fgb" 0
        fi
    done
}

PAYLOAD=$({
    printf '\x44\x52\x47\x59'  # magic
    printf '\x01\x00'           # flags = FULL
    printf '\x0a\x05'           # cols=10, rows=5

    emit_row "Normal    " 200 200 200 0 10
    emit_row "Bold text " 255 255 255 1 10
    emit_row "Italic    " 180 180 255 2 10
    emit_row "Underline " 255 200 100 4 10
    emit_row "Strikethr " 255 100 100 16 10
} | base64 -w0)

printf '\033]666666;run -c ygrid -x 0 -y 0 -w 10 -h 5 -r;;%s\033\\' "$PAYLOAD"
echo
