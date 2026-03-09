#!/bin/bash
# Test scrolling sync between terminal and ydraw overlay

echo "=== SCROLL SYNC TEST ==="

for i in {1..100}; do
    echo "Line $i"

    # Add ydraw circle at current cursor position
    YAML=$(cat <<EOF
body:
  - circle:
      position: [50, 10]
      radius: 8
      fill: "#ff0000"
  - text:
      position: [70, 15]
      content: "L$i"
      font-size: 12
      color: "#00ff00"
EOF
)
    printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML" | base64 -w0)"
    sleep 0.02
done

echo "DONE - circles should have scrolled with text!"
sleep 2
