#!/bin/bash
# YPaint Scrolling Layer Demo: Basic shapes
# Draws SDF shapes that scroll with terminal content (OSC 666674)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

YAML_PAYLOAD=$(cat <<'EOF'
# Basic shapes demo for scrolling layer
# Y coordinates are LOCAL (relative to line), starting from 0
background: "#00000000"

body:
  # Title
  - text:
      position: [50, 0]
      content: "Scrolling Layer - Basic Shapes"
      font-size: 28
      color: "#ffffff"

  # Red circle
  - circle:
      position: [100, 90]
      radius: 40
      fill: "#e74c3c"
      stroke: "#c0392b"
      stroke-width: 2

  - text:
      position: [75, 150]
      content: "Circle"
      font-size: 14
      color: "#ecf0f1"

  # Blue rounded box
  - box:
      position: [250, 90]
      size: [100, 80]
      fill: "#3498db"
      stroke: "#2980b9"
      stroke-width: 2
      round: 8

  - text:
      position: [260, 150]
      content: "Box"
      font-size: 14
      color: "#ecf0f1"

  # Green triangle
  - triangle:
      p0: [400, 130]
      p1: [350, 50]
      p2: [450, 50]
      fill: "#2ecc71"
      stroke: "#27ae60"
      stroke-width: 2

  - text:
      position: [365, 150]
      content: "Triangle"
      font-size: 14
      color: "#ecf0f1"

  # Yellow ellipse
  - ellipse:
      position: [550, 90]
      radii: [60, 35]
      fill: "#f1c40f"
      stroke: "#f39c12"
      stroke-width: 2

  - text:
      position: [520, 150]
      content: "Ellipse"
      font-size: 14
      color: "#ecf0f1"

  # Info about scrolling
  - text:
      position: [50, 190]
      content: "These shapes scroll with terminal output!"
      font-size: 16
      color: "#9b59b6"
EOF
)

echo "This is a simple demo that checks the displays"
echo "few simple shapes"
echo "into the ypaint grid"

PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)
printf '\033]666674;--yaml;%s\033\\' "$PAYLOAD"
echo "YPaint scrolling layer: shapes demo"
