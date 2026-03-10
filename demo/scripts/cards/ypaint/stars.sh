#!/bin/bash
# YPaint Card Demo: Stars only
# Tests Star primitive rendering in card mode

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

# Create YAML payload with multiple stars
YAML_PAYLOAD=$(cat <<'EOF'
background: "#1a1a2e"

body:
  # Large central star
  - star:
      position: [200, 150]
      outer-radius: 50
      points: 5
      inner-ratio: 2.2
      fill: "#FFD700"
      stroke: "#FF8C00"
      stroke-width: 3

  # Corner stars
  - star:
      position: [50, 50]
      outer-radius: 25
      points: 5
      inner-ratio: 2.2
      fill: "#FF0000"
  - star:
      position: [350, 50]
      outer-radius: 25
      points: 5
      inner-ratio: 2.2
      fill: "#00FF00"
  - star:
      position: [50, 250]
      outer-radius: 25
      points: 5
      inner-ratio: 2.2
      fill: "#0000FF"
  - star:
      position: [350, 250]
      outer-radius: 25
      points: 5
      inner-ratio: 2.2
      fill: "#FF00FF"

  # Ring of small stars around center
  - star:
      position: [200, 70]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#FFFFFF"
  - star:
      position: [280, 100]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#FFFFFF"
  - star:
      position: [300, 150]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#FFFFFF"
  - star:
      position: [280, 200]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#FFFFFF"
  - star:
      position: [200, 230]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#FFFFFF"
  - star:
      position: [120, 200]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#FFFFFF"
  - star:
      position: [100, 150]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#FFFFFF"
  - star:
      position: [120, 100]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#FFFFFF"

  # Different point counts row
  - star:
      position: [80, 320]
      outer-radius: 20
      points: 4
      inner-ratio: 2.0
      fill: "#FF5555"
  - star:
      position: [140, 320]
      outer-radius: 20
      points: 5
      inner-ratio: 2.2
      fill: "#55FF55"
  - star:
      position: [200, 320]
      outer-radius: 20
      points: 6
      inner-ratio: 2.2
      fill: "#5555FF"
  - star:
      position: [260, 320]
      outer-radius: 20
      points: 7
      inner-ratio: 2.5
      fill: "#FFFF55"
  - star:
      position: [320, 320]
      outer-radius: 20
      points: 8
      inner-ratio: 2.5
      fill: "#FF55FF"

  - text:
      position: [150, 360]
      content: "Star Primitive Test"
      font-size: 16
      color: "#ffffff"
EOF
)

# Encode as base64
PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)

printf '\033]666675;run -c ypaint -x 0 -y 0 -w 50 -h 25 --yaml;%s;\033\\' "$PAYLOAD"
echo "Stars card demo sent"
