#!/bin/bash
# Scrolling YDraw Overlay Demo: Polygons and all primitive types
# Tests multiple instances of every primitive type

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Create YAML payload with all primitive types - multiple instances each
YAML_PAYLOAD=$(cat <<'EOF'
background: "#00000000"

body:
  # Row 1: Circles (3 instances)
  - circle:
      position: [80, 50]
      radius: 30
      fill: "#e74c3c"
      stroke: "#c0392b"
      stroke-width: 2
  - circle:
      position: [180, 50]
      radius: 25
      fill: "#3498db"
      stroke: "#2980b9"
      stroke-width: 2
  - circle:
      position: [280, 50]
      radius: 35
      fill: "#2ecc71"
      stroke: "#27ae60"
      stroke-width: 2

  # Row 2: Boxes (3 instances)
  - box:
      position: [80, 130]
      size: [60, 40]
      fill: "#9b59b6"
      stroke: "#8e44ad"
      stroke-width: 2
      round: 5
  - box:
      position: [180, 130]
      size: [50, 50]
      fill: "#f1c40f"
      stroke: "#f39c12"
      stroke-width: 2
      round: 8
  - box:
      position: [280, 130]
      size: [70, 35]
      fill: "#1abc9c"
      stroke: "#16a085"
      stroke-width: 2
      round: 3

  # Row 3: Triangles (3 instances)
  - triangle:
      p0: [50, 210]
      p1: [110, 210]
      p2: [80, 170]
      fill: "#e67e22"
      stroke: "#d35400"
      stroke-width: 2
  - triangle:
      p0: [150, 210]
      p1: [210, 210]
      p2: [180, 175]
      fill: "#34495e"
      stroke: "#2c3e50"
      stroke-width: 2
  - triangle:
      p0: [250, 210]
      p1: [310, 210]
      p2: [280, 165]
      fill: "#e74c3c"
      stroke: "#c0392b"
      stroke-width: 2

  # Row 4: Ellipses (3 instances)
  - ellipse:
      position: [80, 270]
      radii: [40, 25]
      fill: "#3498db"
      stroke: "#2980b9"
      stroke-width: 2
  - ellipse:
      position: [180, 270]
      radii: [30, 35]
      fill: "#2ecc71"
      stroke: "#27ae60"
      stroke-width: 2
  - ellipse:
      position: [280, 270]
      radii: [45, 20]
      fill: "#9b59b6"
      stroke: "#8e44ad"
      stroke-width: 2

  # Row 5: Segments (3 instances)
  - segment:
      from: [30, 330]
      to: [130, 330]
      stroke: "#e74c3c"
      stroke-width: 4
  - segment:
      from: [150, 310]
      to: [230, 350]
      stroke: "#3498db"
      stroke-width: 3
  - segment:
      from: [250, 350]
      to: [330, 310]
      stroke: "#2ecc71"
      stroke-width: 5

  # Row 6: Polygons - Pentagon, Hexagon, Star
  - polygon:
      vertices: [80, 360, 100, 380, 95, 405, 65, 405, 60, 380]
      fill: "#f1c40f"
      stroke: "#f39c12"
      stroke-width: 2
  - polygon:
      vertices: [160, 365, 185, 375, 195, 400, 180, 420, 155, 420, 140, 400, 145, 375]
      fill: "#1abc9c"
      stroke: "#16a085"
      stroke-width: 2
  - polygon:
      vertices: [280, 360, 290, 390, 320, 390, 295, 405, 305, 435, 280, 415, 255, 435, 265, 405, 240, 390, 270, 390]
      fill: "#e67e22"
      stroke: "#d35400"
      stroke-width: 2

  # Row 7: More complex polygons - Arrow, Cross
  - polygon:
      vertices: [50, 480, 100, 480, 100, 460, 130, 490, 100, 520, 100, 500, 50, 500]
      fill: "#9b59b6"
      stroke: "#8e44ad"
      stroke-width: 2
  - polygon:
      vertices: [180, 460, 200, 460, 200, 480, 220, 480, 220, 500, 200, 500, 200, 520, 180, 520, 180, 500, 160, 500, 160, 480, 180, 480]
      fill: "#34495e"
      stroke: "#2c3e50"
      stroke-width: 2

  # Row 8: Bezier curves (3 instances)
  - bezier:
      p0: [30, 560]
      p1: [80, 530]
      p2: [130, 560]
      stroke: "#e74c3c"
      stroke-width: 3
  - bezier:
      p0: [150, 560]
      p1: [200, 540]
      p2: [250, 580]
      stroke: "#3498db"
      stroke-width: 3
  - bezier:
      p0: [270, 540]
      p1: [300, 600]
      p2: [330, 540]
      stroke: "#2ecc71"
      stroke-width: 3

  # Row 9: Arcs (3 instances)
  - arc:
      position: [80, 640]
      radius: 35
      thickness: 8
      angle: 270
      fill: "#f1c40f"
  - arc:
      position: [180, 640]
      radius: 30
      thickness: 10
      angle: 180
      fill: "#1abc9c"
  - arc:
      position: [280, 640]
      radius: 40
      thickness: 6
      angle: 320
      fill: "#e67e22"

  # Text labels
  - text:
      position: [350, 50]
      content: "Circles"
      font-size: 14
      color: "#ecf0f1"
  - text:
      position: [350, 130]
      content: "Boxes"
      font-size: 14
      color: "#ecf0f1"
  - text:
      position: [350, 190]
      content: "Triangles"
      font-size: 14
      color: "#ecf0f1"
  - text:
      position: [350, 270]
      content: "Ellipses"
      font-size: 14
      color: "#ecf0f1"
  - text:
      position: [350, 330]
      content: "Segments"
      font-size: 14
      color: "#ecf0f1"
  - text:
      position: [350, 400]
      content: "Polygons"
      font-size: 14
      color: "#ecf0f1"
  - text:
      position: [350, 490]
      content: "Complex Polygons"
      font-size: 14
      color: "#ecf0f1"
  - text:
      position: [350, 560]
      content: "Beziers"
      font-size: 14
      color: "#ecf0f1"
  - text:
      position: [350, 640]
      content: "Arcs"
      font-size: 14
      color: "#ecf0f1"
EOF
)

# Encode as base64
PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)

# Send OSC 666674
printf '\033]666674;--yaml;%s\033\\' "$PAYLOAD"
echo "Polygons demo: all primitive types sent"
