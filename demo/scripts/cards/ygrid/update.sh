#!/bin/bash
# YGrid Card: Create + partial update test
# Creates an ygrid, waits 1s, then sends a partial update.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

emit_cell() {
    local val=$(printf '%d' "'$1")
    local fgr=$2 fgg=$3 fgb=$4 bgr=${5:-0} bgg=${6:-0} bgb=${7:-0}
    printf "\\x$(printf '%02x' $val)\\x00\\x00\\x00"
    printf "\\x$(printf '%02x' $fgr)\\x$(printf '%02x' $fgg)\\x$(printf '%02x' $fgb)\\xff"
    printf "\\x$(printf '%02x' $bgr)\\x$(printf '%02x' $bgg)\\x$(printf '%02x' $bgb)\\x00"
}

emit_row() {
    local text="$1" fgr=$2 fgg=$3 fgb=$4 cols=$5
    for ((i=0; i<cols; i++)); do
        if ((i < ${#text})); then
            emit_cell "${text:$i:1}" "$fgr" "$fgg" "$fgb"
        else
            emit_cell " " "$fgr" "$fgg" "$fgb"
        fi
    done
}

# Step 1: Create card
PAYLOAD=$({
    printf '\x44\x52\x47\x59\x01\x00\x0a\x03'  # header: cols=10, rows=3
    emit_row "Before upd" 180 180 180 10
    emit_row "Will chang" 180 180 180 10
    emit_row "Row2 stays" 180 180 180 10
} | base64 -w0)

printf '\033]666666;run -c ygrid -x 0 -y 0 -w 10 -h 3 -r --name ygrid-test;;%s\033\\' "$PAYLOAD"
echo

sleep 1

# Step 2: Partial update - change row 1
PAYLOAD=$({
    printf '\x44\x52\x47\x59\x00\x00\x0a\x03'  # header: flags=0 (partial)
    printf '\x0a\x00\x00\x00'                     # count=10 entries (LE u32)
    # 10 entries: row(u8) + col(u8) + GridCell(12)
    text="UPDATED!!!"
    for ((i=0; i<10; i++)); do
        printf "\\x01\\x$(printf '%02x' $i)"  # row=1, col=i
        if ((i < ${#text})); then
            emit_cell "${text:$i:1}" 0 255 0 0 32 0
        else
            emit_cell " " 0 255 0 0 32 0
        fi
    done
} | base64 -w0)

printf '\033]666666;update --name ygrid-test;;%s\033\\' "$PAYLOAD"
echo
