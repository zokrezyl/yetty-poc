#!/bin/bash
# Demo: Flexible widget dimensions
# Demonstrates: width=0 (full width), negative heights

# Simple shader code - gradient based on UV
SHADER='fn mainImage(fragCoord: vec2<f32>) -> vec4<f32> {
    let uv = fragCoord / u.resolution;
    let color = vec3<f32>(uv.x, uv.y, 0.5 + 0.5 * sin(u.time));
    return vec4<f32>(color, 1.0);
}'

# Base94 encode function
base94_encode() {
    local input="$1"
    local output=""
    for ((i=0; i<${#input}; i++)); do
        local byte=$(printf '%d' "'${input:$i:1}")
        local c1=$((byte / 94 + 33))
        local c2=$((byte % 94 + 33))
        output+=$(printf "\\x$(printf '%x' $c1)")
        output+=$(printf "\\x$(printf '%x' $c2)")
    done
    echo -n "$output"
}

ENCODED=$(base94_encode "$SHADER")

# OSC format: ESC ] 99999 ; <plugin> ; <mode> ; <x> ; <y> ; <w> ; <h> ; <payload> ST
#
# Flexible dimensions:
#   w=0  -> stretch from x to right edge (termCols - x)
#   h=0  -> stretch from y to bottom edge (termRows - y)
#   w=-N -> terminal columns - N
#   h=-N -> terminal rows - N

echo "Shader full-width demo"
echo ""
echo "  w=0: full width from cursor position"
echo "  h=5: fixed 5 rows"
echo ""

# Full width shader, 5 rows tall
printf '\033]99999;shader;R;0;0;0;5;%s\033\\' "$ENCODED"
