#!/bin/bash
# Multi-shape scrolling test - sends 3 shapes in sequence
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

echo "=== Multi-shape scrolling test ==="

# Shape 1: Red circle
YAML1=$(cat <<'EOF'
background: "#00000000"
body:
  - circle:
      position: [50, 30]
      radius: 25
      fill: "#e74c3c"
  - text:
      position: [30, 70]
      content: "Circle 1"
      font-size: 14
      color: "#ffffff"
EOF
)
printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML1" | base64 -w0)"
echo "Shape 1: Red circle"
sleep 0.3

# Shape 2: Blue box
YAML2=$(cat <<'EOF'
background: "#00000000"
body:
  - box:
      position: [50, 30]
      size: [100, 60]
      fill: "#3498db"
      round: 5
  - text:
      position: [60, 100]
      content: "Box 2"
      font-size: 14
      color: "#ffffff"
EOF
)
printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML2" | base64 -w0)"
echo "Shape 2: Blue box"
sleep 0.3

# Shape 3: Green triangle
YAML3=$(cat <<'EOF'
background: "#00000000"
body:
  - triangle:
      p0: [80, 60]
      p1: [40, 20]
      p2: [120, 20]
      fill: "#2ecc71"
  - text:
      position: [50, 80]
      content: "Triangle 3"
      font-size: 14
      color: "#ffffff"
EOF
)
printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML3" | base64 -w0)"
echo "Shape 3: Green triangle"

echo ""
echo "All 3 shapes should be visible above, stacked vertically"
sleep 3
