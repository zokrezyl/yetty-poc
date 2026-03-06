#!/bin/bash
# Screen Draw Layer Demo: Fun patterns and special shapes (ABSOLUTE positioning)
# Content is positioned at fixed screen coordinates (row=0, col=0)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

YAML_PAYLOAD=$(cat <<'EOF'
background: "#00000000"

body:
  # Title
  - text:
      position: [50, 30]
      content: "Fun Patterns"
      font-size: 28
      color: "#ffffff"

  # Cool S pattern (the classic)
  - cool_s:
      position: [100, 150]
      radius: 60
      fill: "#3498db"
      stroke: "#2980b9"
      stroke-width: 2

  - text:
      position: [65, 230]
      content: "Cool S"
      font-size: 14
      color: "#ecf0f1"

  # Quadratic circle
  - quadratic_circle:
      position: [250, 150]
      radius: 50
      fill: "#2ecc71"
      stroke: "#27ae60"
      stroke-width: 2

  - text:
      position: [195, 230]
      content: "Quad Circle"
      font-size: 14
      color: "#ecf0f1"

  # Grid of circles
  - text:
      position: [50, 280]
      content: "Circle Grid:"
      font-size: 16
      color: "#bdc3c7"

  - circle:
      position: [100, 340]
      radius: 15
      fill: "#e74c3c"
  - circle:
      position: [140, 340]
      radius: 15
      fill: "#f39c12"
  - circle:
      position: [180, 340]
      radius: 15
      fill: "#f1c40f"
  - circle:
      position: [220, 340]
      radius: 15
      fill: "#2ecc71"
  - circle:
      position: [260, 340]
      radius: 15
      fill: "#3498db"

  - circle:
      position: [100, 380]
      radius: 15
      fill: "#3498db"
  - circle:
      position: [140, 380]
      radius: 15
      fill: "#e74c3c"
  - circle:
      position: [180, 380]
      radius: 15
      fill: "#f39c12"
  - circle:
      position: [220, 380]
      radius: 15
      fill: "#f1c40f"
  - circle:
      position: [260, 380]
      radius: 15
      fill: "#2ecc71"

  # Overlapping shapes
  - text:
      position: [350, 280]
      content: "Overlapping:"
      font-size: 16
      color: "#bdc3c7"

  - circle:
      position: [400, 360]
      radius: 40
      fill: "#e74c3c99"

  - circle:
      position: [440, 360]
      radius: 40
      fill: "#3498db99"

  - circle:
      position: [420, 330]
      radius: 40
      fill: "#2ecc7199"

  # Triangular arrangement
  - text:
      position: [530, 280]
      content: "Triangles:"
      font-size: 16
      color: "#bdc3c7"

  - equilateral_triangle:
      position: [580, 340]
      radius: 30
      fill: "#9b59b6"
      stroke: "#8e44ad"
      stroke-width: 2

  - equilateral_triangle:
      position: [610, 380]
      radius: 30
      fill: "#1abc9c"
      stroke: "#16a085"
      stroke-width: 2

  - equilateral_triangle:
      position: [550, 380]
      radius: 30
      fill: "#e91e63"
      stroke: "#c2185b"
      stroke-width: 2

  # Segment lines pattern
  - text:
      position: [50, 440]
      content: "Line Pattern:"
      font-size: 16
      color: "#bdc3c7"

  - segment:
      from: [50, 480]
      to: [150, 520]
      stroke: "#e74c3c"
      stroke-width: 2
  - segment:
      from: [70, 480]
      to: [170, 520]
      stroke: "#f39c12"
      stroke-width: 2
  - segment:
      from: [90, 480]
      to: [190, 520]
      stroke: "#f1c40f"
      stroke-width: 2
  - segment:
      from: [110, 480]
      to: [210, 520]
      stroke: "#2ecc71"
      stroke-width: 2
  - segment:
      from: [130, 480]
      to: [230, 520]
      stroke: "#3498db"
      stroke-width: 2
  - segment:
      from: [150, 480]
      to: [250, 520]
      stroke: "#9b59b6"
      stroke-width: 2
EOF
)

PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)
# ABSOLUTE positioning: --row=0 --col=0 fixes content at screen origin
printf '\033]666673;--row=0 --col=0 --yaml;%s\033\\' "$PAYLOAD"
echo "Screen draw layer: patterns demo (absolute positioning)"
