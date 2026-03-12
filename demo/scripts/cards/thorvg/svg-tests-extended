#!/bin/bash
# ThorVG Random SVG Test - Downloads random SVGs from openclipart.org
#
# Generates random IDs and fetches SVGs directly

WIDTH="${WIDTH:-60}"
HEIGHT="${HEIGHT:-30}"
DELAY="${DELAY:-0}"
MAX_ID=3550000
MIN_ID=1000

show_svg() {
    local svg="$1"
    local name="$2"
    PAYLOAD=$(echo -n "$svg" | base64 -w0)
    printf '\033]666666;run -c thorvg -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
    echo
}

COUNT="${1:-0}"
i=0

while true; do
    # Generate random ID
    id=$((RANDOM % (MAX_ID - MIN_ID) + MIN_ID))

    # Get redirect to extract filename
    location=$(curl -sI "https://openclipart.org/download/$id" 2>/dev/null | grep -i "^location:" | sed 's/.*\///' | tr -d '\r\n')

    if [[ -z "$location" ]]; then
        continue
    fi

    # Extract name from filename (remove ID suffix and .svg)
    name=$(echo "$location" | sed 's/-[0-9]*\.svg$//' | sed 's/-/ /g')

    # Download SVG
    svg=$(curl -sfL "https://openclipart.org/download/$id" 2>/dev/null)

    # Check if it's valid SVG
    if [[ "$svg" == *"<svg"* ]]; then
        echo "Showing: $name"
        show_svg "$svg" "$name"
        sleep "$DELAY"
        ((i++))
    fi

    if [[ "$COUNT" -gt 0 && "$i" -ge "$COUNT" ]]; then
        break
    fi
done
