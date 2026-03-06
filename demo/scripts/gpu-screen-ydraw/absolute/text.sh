#!/bin/bash
# Screen Draw Layer Demo: Text (ABSOLUTE positioning)
# Content is positioned at fixed screen coordinates (row=0, col=0)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

YAML_PAYLOAD=$(cat <<'EOF'
# Text overlay demo for screen draw layer
background: "#00000000"

body:
  # Large title
  - text:
      position: [50, 50]
      content: "Screen Draw Layer"
      font-size: 32
      color: "#3498db"

  # Subtitle
  - text:
      position: [50, 90]
      content: "Text at pixel coordinates (50, 90)"
      font-size: 18
      color: "#2ecc71"

  # Info text
  - text:
      position: [50, 120]
      content: "Independent of terminal cells!"
      font-size: 18
      color: "#e74c3c"

  # Different sizes
  - text:
      position: [50, 180]
      content: "Size 12"
      font-size: 12
      color: "#95a5a6"

  - text:
      position: [50, 200]
      content: "Size 16"
      font-size: 16
      color: "#7f8c8d"

  - text:
      position: [50, 230]
      content: "Size 24"
      font-size: 24
      color: "#ecf0f1"

  - text:
      position: [50, 270]
      content: "Size 36"
      font-size: 36
      color: "#ffffff"

  # Colored text
  - text:
      position: [300, 180]
      content: "Red"
      font-size: 20
      color: "#e74c3c"

  - text:
      position: [360, 180]
      content: "Orange"
      font-size: 20
      color: "#e67e22"

  - text:
      position: [450, 180]
      content: "Yellow"
      font-size: 20
      color: "#f1c40f"

  - text:
      position: [300, 210]
      content: "Green"
      font-size: 20
      color: "#2ecc71"

  - text:
      position: [385, 210]
      content: "Blue"
      font-size: 20
      color: "#3498db"

  - text:
      position: [450, 210]
      content: "Purple"
      font-size: 20
      color: "#9b59b6"

  # Positioned text
  - text:
      position: [400, 350]
      content: "Bottom-right area"
      font-size: 24
      color: "#f39c12"

  # Small fine print
  - text:
      position: [50, 400]
      content: "This text renders at exact pixel coordinates, allowing precise positioning."
      font-size: 14
      color: "#7f8c8d"
EOF
)

PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)
# ABSOLUTE positioning: --row=0 --col=0 fixes content at screen origin
printf '\033]666673;--row=0 --col=0 --yaml;%s\033\\' "$PAYLOAD"
echo "Screen draw layer: text demo (absolute positioning)"
