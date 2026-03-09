#!/bin/bash
# Interleaved text and shapes - terminal text between ydraw shapes
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

echo "=== Interleaved Text and Shapes Test ==="
echo "Terminal line 1"
echo "Terminal line 2"

# Shape 1
YAML1=$(cat <<'EOF'
background: "#00000000"
body:
  - box:
      position: [20, 10]
      size: [200, 40]
      fill: "#9b59b6"
      round: 8
  - text:
      position: [40, 40]
      content: "YDraw Box 1"
      font-size: 18
      color: "#ffffff"
EOF
)
printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML1" | base64 -w0)"

echo "Terminal line after shape 1"
echo "More terminal text..."

# Shape 2
YAML2=$(cat <<'EOF'
background: "#00000000"
body:
  - ellipse:
      position: [80, 30]
      radii: [60, 25]
      fill: "#e67e22"
  - text:
      position: [40, 70]
      content: "YDraw Ellipse 2"
      font-size: 14
      color: "#ffffff"
EOF
)
printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML2" | base64 -w0)"

echo "Terminal line after shape 2"
echo "Even more text!"

# Shape 3
YAML3=$(cat <<'EOF'
background: "#00000000"
body:
  - circle:
      position: [50, 30]
      radius: 25
      fill: "#1abc9c"
  - text:
      position: [30, 70]
      content: "Circle 3"
      font-size: 14
      color: "#ffffff"
EOF
)
printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML3" | base64 -w0)"

echo "Final terminal line"
echo ""
echo "You should see: text, shape, text, shape, text, shape, text"
sleep 3
