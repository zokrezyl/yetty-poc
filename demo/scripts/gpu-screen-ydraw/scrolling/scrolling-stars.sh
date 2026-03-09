#!/bin/bash
# Scrolling YDraw Overlay Demo: Stars only
# Tests Star primitive rendering in scrolling mode

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Create YAML payload with multiple stars
YAML_PAYLOAD=$(cat <<'EOF'
background: "#00000000"

body:
  # Row 1: Different sized stars
  - star:
      position: [50, 30]
      outer-radius: 20
      points: 5
      inner-ratio: 2.2
      fill: "#FF0000"
  - star:
      position: [120, 30]
      outer-radius: 15
      points: 5
      inner-ratio: 2.2
      fill: "#00FF00"
  - star:
      position: [180, 30]
      outer-radius: 25
      points: 5
      inner-ratio: 2.2
      fill: "#0000FF"

  # Row 2: Different point counts
  - star:
      position: [50, 90]
      outer-radius: 18
      points: 4
      inner-ratio: 2.0
      fill: "#FFFF00"
  - star:
      position: [120, 90]
      outer-radius: 18
      points: 6
      inner-ratio: 2.2
      fill: "#FF00FF"
  - star:
      position: [180, 90]
      outer-radius: 18
      points: 8
      inner-ratio: 2.5
      fill: "#00FFFF"

  # Row 3: Different inner ratios
  - star:
      position: [50, 150]
      outer-radius: 20
      points: 5
      inner-ratio: 1.5
      fill: "#FF8800"
  - star:
      position: [120, 150]
      outer-radius: 20
      points: 5
      inner-ratio: 2.5
      fill: "#88FF00"
  - star:
      position: [180, 150]
      outer-radius: 20
      points: 5
      inner-ratio: 3.5
      fill: "#0088FF"

  # Row 4: Stars with strokes
  - star:
      position: [50, 210]
      outer-radius: 18
      points: 5
      inner-ratio: 2.2
      fill: "#FF0000"
      stroke: "#FFFFFF"
      stroke-width: 2
  - star:
      position: [120, 210]
      outer-radius: 18
      points: 6
      inner-ratio: 2.0
      fill: "#00FF00"
      stroke: "#FFFFFF"
      stroke-width: 2
  - star:
      position: [180, 210]
      outer-radius: 18
      points: 7
      inner-ratio: 2.5
      fill: "#0000FF"
      stroke: "#FFFFFF"
      stroke-width: 2

  # Row 5: Small stars
  - star:
      position: [40, 270]
      outer-radius: 10
      points: 5
      inner-ratio: 2.2
      fill: "#FF5555"
  - star:
      position: [70, 270]
      outer-radius: 10
      points: 5
      inner-ratio: 2.2
      fill: "#55FF55"
  - star:
      position: [100, 270]
      outer-radius: 10
      points: 5
      inner-ratio: 2.2
      fill: "#5555FF"
  - star:
      position: [130, 270]
      outer-radius: 10
      points: 5
      inner-ratio: 2.2
      fill: "#FFFF55"
  - star:
      position: [160, 270]
      outer-radius: 10
      points: 5
      inner-ratio: 2.2
      fill: "#FF55FF"
  - star:
      position: [190, 270]
      outer-radius: 10
      points: 5
      inner-ratio: 2.2
      fill: "#55FFFF"

  # Labels
  - text:
      position: [250, 30]
      content: "Sizes"
      font-size: 14
      color: "#ffffff"
  - text:
      position: [250, 90]
      content: "Points"
      font-size: 14
      color: "#ffffff"
  - text:
      position: [250, 150]
      content: "Inner ratio"
      font-size: 14
      color: "#ffffff"
  - text:
      position: [250, 210]
      content: "Strokes"
      font-size: 14
      color: "#ffffff"
  - text:
      position: [250, 270]
      content: "Small"
      font-size: 14
      color: "#ffffff"
EOF
)

# Encode as base64
PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)

# Send OSC 666674
printf '\033]666674;--yaml;%s\033\\' "$PAYLOAD"
echo "Stars demo sent"
