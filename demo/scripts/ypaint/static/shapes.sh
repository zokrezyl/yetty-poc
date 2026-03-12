#!/bin/bash
# YPaint Static Layer Demo: Basic shapes
# Draws SDF shapes as fixed overlay (OSC 666675)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

YAML_PAYLOAD=$(cat <<'EOF'
# Basic shapes demo for static overlay layer
background: "#00000000"

body:
  # Title
  - text:
      position: [50, 30]
      content: "Static Overlay - Basic Shapes"
      font-size: 28
      color: "#ffffff"

  # Red circle
  - circle:
      position: [100, 120]
      radius: 40
      fill: "#e74c3c"
      stroke: "#c0392b"
      stroke-width: 2

  - text:
      position: [75, 180]
      content: "Circle"
      font-size: 14
      color: "#ecf0f1"

  # Blue rounded box
  - box:
      position: [250, 120]
      size: [100, 80]
      fill: "#3498db"
      stroke: "#2980b9"
      stroke-width: 2
      round: 8

  - text:
      position: [260, 180]
      content: "Box"
      font-size: 14
      color: "#ecf0f1"

  # Green triangle
  - triangle:
      p0: [400, 160]
      p1: [350, 80]
      p2: [450, 80]
      fill: "#2ecc71"
      stroke: "#27ae60"
      stroke-width: 2

  - text:
      position: [365, 180]
      content: "Triangle"
      font-size: 14
      color: "#ecf0f1"

  # Yellow ellipse
  - ellipse:
      position: [550, 120]
      radii: [60, 35]
      fill: "#f1c40f"
      stroke: "#f39c12"
      stroke-width: 2

  - text:
      position: [520, 180]
      content: "Ellipse"
      font-size: 14
      color: "#ecf0f1"

  # Purple line
  - segment:
      from: [50, 220]
      to: [650, 220]
      stroke: "#9b59b6"
      stroke-width: 3

  - text:
      position: [50, 250]
      content: "Segment"
      font-size: 14
      color: "#ecf0f1"

  # Orange bezier curve
  - bezier:
      p0: [50, 320]
      p1: [200, 270]
      p2: [350, 370]
      stroke: "#e67e22"
      stroke-width: 3

  - bezier:
      p0: [350, 370]
      p1: [500, 270]
      p2: [650, 320]
      stroke: "#e67e22"
      stroke-width: 3

  - text:
      position: [50, 390]
      content: "Bezier Curves"
      font-size: 14
      color: "#ecf0f1"

  # Arc
  - arc:
      position: [550, 350]
      radius: 50
      thickness: 12
      angle: 270
      fill: "#1abc9c"

  - text:
      position: [520, 420]
      content: "Arc (270)"
      font-size: 14
      color: "#ecf0f1"
EOF
)

PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)
printf '\033]666675;--yaml;%s\033\\' "$PAYLOAD"
echo "YPaint static layer: shapes demo"
