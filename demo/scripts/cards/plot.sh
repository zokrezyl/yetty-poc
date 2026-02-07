#!/bin/bash
# Plot Card: Interactive chart - sine wave demo

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Generate sine wave data (100 points)
POINTS=100
VALUES=""
for i in $(seq 0 $((POINTS - 1))); do
    VAL=$(awk "BEGIN {printf \"%.3f\", sin($i * 3.14159265 * 2 / $POINTS)}")
    [ -n "$VALUES" ] && VALUES="$VALUES,"
    VALUES="$VALUES$VAL"
done

PAYLOAD=$(echo -n "$VALUES" | base64 -w0)
printf '\033]666666;run -c plot -x 0 -y 0 -w 60 -h 20 -r;--type line --grid --axes;%s\033\\' "$PAYLOAD"
echo
