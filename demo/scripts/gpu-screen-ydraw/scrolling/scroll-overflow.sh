#!/bin/bash
# Test scrolling when shapes overflow the terminal
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

echo "=== Scroll Overflow Test ==="
echo "Adding shapes until they overflow the terminal..."
echo ""

# Add 20 shapes - should cause scrolling
for i in {1..20}; do
  # Alternate colors
  case $((i % 5)) in
    0) COLOR="#e74c3c" ;;
    1) COLOR="#3498db" ;;
    2) COLOR="#2ecc71" ;;
    3) COLOR="#f1c40f" ;;
    4) COLOR="#9b59b6" ;;
  esac

  YAML=$(cat <<EOF
background: "#00000000"
body:
  - box:
      position: [20, 10]
      size: [200, 40]
      fill: "$COLOR"
      round: 6
  - text:
      position: [40, 35]
      content: "Shape $i - overflow test"
      font-size: 16
      color: "#ffffff"
EOF
)
  printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML" | base64 -w0)"
  echo "Shape $i sent"
  sleep 0.15
done

echo ""
echo "Done! Shapes should have scrolled as they overflowed."
sleep 3
