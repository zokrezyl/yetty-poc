#!/bin/bash
# YGrid Card: Random character updates at random positions
# Creates a 40x12 ygrid then sends partial updates with random chars.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

COLS=40
ROWS=12
NAME="ygrid-random"

emit_cell() {
    local val=$(printf '%d' "'$1")
    local fgr=$2 fgg=$3 fgb=$4 bgr=${5:-0} bgg=${6:-0} bgb=${7:-0}
    printf "\\x$(printf '%02x' $val)\\x00\\x00\\x00"
    printf "\\x$(printf '%02x' $fgr)\\x$(printf '%02x' $fgg)\\x$(printf '%02x' $fgb)\\xff"
    printf "\\x$(printf '%02x' $bgr)\\x$(printf '%02x' $bgg)\\x$(printf '%02x' $bgb)\\x00"
}

# Create empty grid (all spaces)
PAYLOAD=$({
    printf '\x44\x52\x47\x59\x01\x00'
    printf "\\x$(printf '%02x' $COLS)\\x$(printf '%02x' $ROWS)"
    for ((i=0; i<COLS*ROWS; i++)); do
        emit_cell " " 200 200 200
    done
} | base64 -w0)

printf '\033]666666;run -c ygrid -x 0 -y 0 -w %d -h %d -r --name %s;;%s\033\\' "$COLS" "$ROWS" "$NAME" "$PAYLOAD"
echo

CHARS="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%&*+-=<>?"

# Send random partial updates in a loop
for ((frame=0; frame<200; frame++)); do
    COUNT=$((RANDOM % 10 + 1))

    PAYLOAD=$({
        printf '\x44\x52\x47\x59\x00\x00'
        printf "\\x$(printf '%02x' $COLS)\\x$(printf '%02x' $ROWS)"
        printf "\\x$(printf '%02x' $((COUNT & 0xff)))\\x$(printf '%02x' $(((COUNT>>8) & 0xff)))\\x00\\x00"

        for ((c=0; c<COUNT; c++)); do
            row=$((RANDOM % ROWS))
            col=$((RANDOM % COLS))
            ch="${CHARS:$((RANDOM % ${#CHARS})):1}"
            fgr=$((RANDOM % 256))
            fgg=$((RANDOM % 256))
            fgb=$((RANDOM % 256))

            printf "\\x$(printf '%02x' $row)\\x$(printf '%02x' $col)"
            emit_cell "$ch" $fgr $fgg $fgb
        done
    } | base64 -w0)

    printf '\033]666666;update --name %s;;%s\033\\' "$NAME" "$PAYLOAD"
    sleep 0.05
done
