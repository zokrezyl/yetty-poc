#!/bin/bash
# Stress test - many shapes in rapid succession
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

echo "=== Scrolling Stress Test ==="
echo "Sending 10 shapes rapidly..."
echo ""

COLORS=("#e74c3c" "#3498db" "#2ecc71" "#f1c40f" "#9b59b6"
        "#e67e22" "#1abc9c" "#34495e" "#d35400" "#8e44ad")

for i in {1..10}; do
  COLOR=${COLORS[$((i-1))]}
  YAML=$(cat <<EOF
background: "#00000000"
body:
  - box:
      position: [20, 10]
      size: [150, 30]
      fill: "$COLOR"
      round: 4
  - text:
      position: [30, 30]
      content: "Shape $i"
      font-size: 14
      color: "#ffffff"
EOF
)
  printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML" | base64 -w0)"
  echo "Sent shape $i"
  sleep 0.1
done

echo ""
echo "All 10 shapes should be stacked vertically"
sleep 3
