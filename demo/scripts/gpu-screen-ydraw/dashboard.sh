#!/bin/bash
# Screen Draw Layer Demo: Dashboard-style overlay

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

YAML_PAYLOAD=$(cat <<'EOF'
background: "#00000000"

body:
  # Header bar background
  - box:
      position: [400, 25]
      size: [780, 40]
      fill: "#2c3e50cc"
      round: 5

  # Header title
  - text:
      position: [20, 35]
      content: "System Dashboard"
      font-size: 24
      color: "#ecf0f1"

  # Status indicators
  - circle:
      position: [700, 25]
      radius: 8
      fill: "#2ecc71"
  - text:
      position: [715, 30]
      content: "Online"
      font-size: 12
      color: "#2ecc71"

  # CPU panel
  - box:
      position: [100, 120]
      size: [160, 120]
      fill: "#34495e99"
      stroke: "#3498db"
      stroke-width: 2
      round: 8

  - text:
      position: [45, 80]
      content: "CPU"
      font-size: 18
      color: "#3498db"

  - arc:
      position: [100, 140]
      radius: 35
      thickness: 8
      angle: 270
      fill: "#3498db"

  - text:
      position: [85, 145]
      content: "75%"
      font-size: 16
      color: "#ffffff"

  # Memory panel
  - box:
      position: [280, 120]
      size: [160, 120]
      fill: "#34495e99"
      stroke: "#2ecc71"
      stroke-width: 2
      round: 8

  - text:
      position: [220, 80]
      content: "Memory"
      font-size: 18
      color: "#2ecc71"

  - arc:
      position: [280, 140]
      radius: 35
      thickness: 8
      angle: 180
      fill: "#2ecc71"

  - text:
      position: [265, 145]
      content: "50%"
      font-size: 16
      color: "#ffffff"

  # Disk panel
  - box:
      position: [460, 120]
      size: [160, 120]
      fill: "#34495e99"
      stroke: "#e74c3c"
      stroke-width: 2
      round: 8

  - text:
      position: [410, 80]
      content: "Disk"
      font-size: 18
      color: "#e74c3c"

  - arc:
      position: [460, 140]
      radius: 35
      thickness: 8
      angle: 324
      fill: "#e74c3c"

  - text:
      position: [445, 145]
      content: "90%"
      font-size: 16
      color: "#ffffff"

  # Network activity - wave pattern using beziers
  - box:
      position: [280, 280]
      size: [520, 100]
      fill: "#34495e99"
      stroke: "#9b59b6"
      stroke-width: 2
      round: 8

  - text:
      position: [40, 240]
      content: "Network"
      font-size: 18
      color: "#9b59b6"

  - bezier:
      p0: [50, 280]
      p1: [150, 250]
      p2: [250, 310]
      stroke: "#9b59b6"
      stroke-width: 2

  - bezier:
      p0: [250, 310]
      p1: [350, 250]
      p2: [450, 290]
      stroke: "#9b59b6"
      stroke-width: 2

  - bezier:
      p0: [450, 290]
      p1: [520, 260]
      p2: [520, 280]
      stroke: "#9b59b6"
      stroke-width: 2

  # Footer
  - segment:
      from: [20, 360]
      to: [780, 360]
      stroke: "#7f8c8d"
      stroke-width: 1

  - text:
      position: [20, 380]
      content: "Last updated: just now"
      font-size: 12
      color: "#95a5a6"
EOF
)

PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)
printf '\033]666673;--yaml;%s\033\\' "$PAYLOAD"
echo "Screen draw layer: dashboard demo"
