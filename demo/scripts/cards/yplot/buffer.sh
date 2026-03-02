#!/bin/bash
# Demo: yplot - buffer mode (data array instead of expression)
# This demonstrates plotting pre-computed data values

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

# Generate sine wave data (100 points)
POINTS=100
VALUES=""
for i in $(seq 0 $((POINTS - 1))); do
    VAL=$(awk "BEGIN {printf \"%.3f\", sin($i * 3.14159265 * 2 / $POINTS)}")
    [ -n "$VALUES" ] && VALUES="$VALUES,"
    VALUES="$VALUES$VAL"
done

# Base64 encode the payload (required by OSC protocol)
PAYLOAD=$(echo -n "$VALUES" | base64 -w0)

# yplot card with buffer mode
# The payload contains comma-separated float values (base64 encoded)
# Auto-range will calculate y min/max from the data
printf '\033]666666;run -c yplot -w 80 -h 25 -r;{@grid=on @axes=on};%s\033\\' "$PAYLOAD"
