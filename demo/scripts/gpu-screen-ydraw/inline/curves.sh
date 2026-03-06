#!/bin/bash
# Screen Draw Layer Demo: Bezier curves and arcs (INLINE positioning)
# Content appears at cursor position and terminal scrolls by content height

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

YAML_PAYLOAD=$(cat <<'EOF'
background: "#00000000"

body:
  # Title
  - text:
      position: [50, 30]
      content: "Curves & Arcs"
      font-size: 28
      color: "#ffffff"

  # Arc showcase - different angles
  - text:
      position: [50, 70]
      content: "Arcs:"
      font-size: 16
      color: "#bdc3c7"

  - arc:
      position: [100, 140]
      radius: 40
      thickness: 10
      angle: 90
      fill: "#e74c3c"

  - arc:
      position: [200, 140]
      radius: 40
      thickness: 10
      angle: 180
      fill: "#f39c12"

  - arc:
      position: [300, 140]
      radius: 40
      thickness: 10
      angle: 270
      fill: "#2ecc71"

  - arc:
      position: [400, 140]
      radius: 40
      thickness: 10
      angle: 360
      fill: "#3498db"

  - text:
      position: [85, 200]
      content: "90"
      font-size: 12
      color: "#ecf0f1"

  - text:
      position: [183, 200]
      content: "180"
      font-size: 12
      color: "#ecf0f1"

  - text:
      position: [283, 200]
      content: "270"
      font-size: 12
      color: "#ecf0f1"

  - text:
      position: [383, 200]
      content: "360"
      font-size: 12
      color: "#ecf0f1"

  # Bezier curves showcase
  - text:
      position: [50, 250]
      content: "Bezier Curves:"
      font-size: 16
      color: "#bdc3c7"

  # Smooth S-curve
  - bezier:
      p0: [50, 320]
      p1: [150, 280]
      p2: [250, 360]
      stroke: "#e74c3c"
      stroke-width: 3

  # Wave pattern
  - bezier:
      p0: [280, 320]
      p1: [330, 280]
      p2: [380, 320]
      stroke: "#f39c12"
      stroke-width: 3

  - bezier:
      p0: [380, 320]
      p1: [430, 360]
      p2: [480, 320]
      stroke: "#f39c12"
      stroke-width: 3

  # Tight curve
  - bezier:
      p0: [520, 350]
      p1: [550, 280]
      p2: [620, 350]
      stroke: "#9b59b6"
      stroke-width: 3

  # Nested arcs (ring pattern)
  - text:
      position: [50, 400]
      content: "Ring Patterns:"
      font-size: 16
      color: "#bdc3c7"

  - arc:
      position: [150, 500]
      radius: 60
      thickness: 8
      angle: 300
      fill: "#3498db"

  - arc:
      position: [150, 500]
      radius: 45
      thickness: 6
      angle: 240
      fill: "#2ecc71"

  - arc:
      position: [150, 500]
      radius: 32
      thickness: 5
      angle: 180
      fill: "#f39c12"

  - arc:
      position: [150, 500]
      radius: 22
      thickness: 4
      angle: 120
      fill: "#e74c3c"

  # Connected beziers forming a complex path
  - arc:
      position: [350, 500]
      radius: 50
      thickness: 3
      angle: 270
      stroke: "#1abc9c"
      stroke-width: 2

  - bezier:
      p0: [400, 500]
      p1: [450, 450]
      p2: [500, 500]
      stroke: "#1abc9c"
      stroke-width: 3

  - bezier:
      p0: [500, 500]
      p1: [550, 550]
      p2: [600, 500]
      stroke: "#1abc9c"
      stroke-width: 3
EOF
)

PAYLOAD=$(echo "$YAML_PAYLOAD" | base64 -w0)

# Query cursor position (DSR - Device Status Report)
get_cursor_pos() {
    exec < /dev/tty
    oldstty=$(stty -g)
    stty raw -echo min 0
    printf '\033[6n' > /dev/tty
    result=""
    while IFS= read -r -n1 -t1 char; do
        result+="$char"
        [[ "$char" == "R" ]] && break
    done
    stty "$oldstty"
    echo "$result" | sed 's/\x1b\[//' | sed 's/R//'
}

echo "Cursor BEFORE ydraw: $(get_cursor_pos)"

# INLINE positioning: no --row/--col, content appears at cursor and terminal scrolls
printf '\033]666673;--yaml;%s\033\\' "$PAYLOAD"

echo "Cursor AFTER ydraw: $(get_cursor_pos)"
echo "Screen draw layer: curves and arcs demo (inline positioning)"
