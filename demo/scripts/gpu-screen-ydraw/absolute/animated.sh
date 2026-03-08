#!/bin/bash
# Absolute YDraw Overlay Demo: Animated shapes
# Updates the overlay repeatedly to show animation

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Animation: bouncing circle
for i in {1..100}; do
    # Calculate position
    x=$((100 + (i * 5) % 400))
    y=$((50 + (i * 3) % 200))

    # Color changes
    r=$(( (i * 5) % 256 ))
    g=$(( (255 - i * 3) % 256 ))
    b=$(( (i * 7) % 256 ))
    color=$(printf "#%02x%02x%02x" $r $g $b)

    YAML_PAYLOAD=$(cat <<EOF
background: "#00000000"
body:
  - circle:
      center: [$x, $y]
      radius: 30
      fill-color: "$color"
  - text:
      position: [50, 30]
      content: "Frame $i"
      font-size: 20
      color: "#ffffff"
EOF
)

    PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)
    printf '\033]666673;--yaml;%s\033\\' "$PAYLOAD"

    sleep 0.05
done

echo "Animation complete"
