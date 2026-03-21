#!/bin/bash
# YPaint Scrolling Layer Demo: Simple bar chart
# A chart that scrolls with terminal output (OSC 666674)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

YAML_PAYLOAD=$(cat <<'EOF'
# Simple bar chart for scrolling layer
# Y coordinates are LOCAL (relative to line), starting from 0
background: "#00000000"

body:
  # Chart title
  - text:
      position: [50, 0]
      content: "Monthly Sales"
      font-size: 20
      color: "#ffffff"

  # Y-axis
  - segment:
      from: [50, 15]
      to: [50, 155]
      stroke: "#7f8c8d"
      stroke-width: 2

  # X-axis
  - segment:
      from: [50, 155]
      to: [350, 155]
      stroke: "#7f8c8d"
      stroke-width: 2

  # Bars
  - box:
      position: [70, 75]
      size: [40, 80]
      fill: "#3498db"
      round: 4

  - box:
      position: [120, 35]
      size: [40, 120]
      fill: "#2ecc71"
      round: 4

  - box:
      position: [170, 55]
      size: [40, 100]
      fill: "#e74c3c"
      round: 4

  - box:
      position: [220, 95]
      size: [40, 60]
      fill: "#f39c12"
      round: 4

  - box:
      position: [270, 25]
      size: [40, 130]
      fill: "#9b59b6"
      round: 4

  # Labels
  - text:
      position: [75, 170]
      content: "Jan"
      font-size: 12
      color: "#bdc3c7"

  - text:
      position: [125, 170]
      content: "Feb"
      font-size: 12
      color: "#bdc3c7"

  - text:
      position: [175, 170]
      content: "Mar"
      font-size: 12
      color: "#bdc3c7"

  - text:
      position: [225, 170]
      content: "Apr"
      font-size: 12
      color: "#bdc3c7"

  - text:
      position: [275, 170]
      content: "May"
      font-size: 12
      color: "#bdc3c7"
EOF
)

PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)
printf '\033]666674;--yaml;%s\033\\' "$PAYLOAD"
echo "YPaint scrolling layer: bar chart demo"
