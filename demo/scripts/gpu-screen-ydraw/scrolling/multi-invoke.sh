#!/bin/bash
# Multi-invocation scrolling test
# Tests that multiple addYdrawBuffer calls work correctly with cursor sync

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

echo "=== Multi-invocation scrolling test ==="
echo "Sending 3 shapes in sequence..."
echo ""

# Shape 1: Red circle (small, ~3 lines)
YAML1=$(cat <<'EOF'
background: "#00000000"
body:
  - circle:
      position: [50, 30]
      radius: 25
      fill: "#e74c3c"
  - text:
      position: [30, 70]
      content: "Shape 1"
      font-size: 14
      color: "#ffffff"
EOF
)
PAYLOAD1=$(echo "$YAML1" | base64 -w0)
printf '\033]666674;--yaml;%s\033\\' "$PAYLOAD1"
echo "Shape 1 sent"

sleep 0.5

# Shape 2: Blue box (medium, ~5 lines)
YAML2=$(cat <<'EOF'
background: "#00000000"
body:
  - box:
      position: [50, 30]
      size: [120, 80]
      fill: "#3498db"
      round: 8
  - text:
      position: [60, 130]
      content: "Shape 2"
      font-size: 14
      color: "#ffffff"
EOF
)
PAYLOAD2=$(echo "$YAML2" | base64 -w0)
printf '\033]666674;--yaml;%s\033\\' "$PAYLOAD2"
echo "Shape 2 sent"

sleep 0.5

# Shape 3: Green triangle (medium, ~5 lines)
YAML3=$(cat <<'EOF'
background: "#00000000"
body:
  - triangle:
      p0: [100, 80]
      p1: [50, 20]
      p2: [150, 20]
      fill: "#2ecc71"
  - text:
      position: [60, 100]
      content: "Shape 3"
      font-size: 14
      color: "#ffffff"
EOF
)
PAYLOAD3=$(echo "$YAML3" | base64 -w0)
printf '\033]666674;--yaml;%s\033\\' "$PAYLOAD3"
echo "Shape 3 sent"

echo ""
echo "All 3 shapes should be visible above"
echo "Press Enter to exit..."
read
