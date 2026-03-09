#!/bin/bash
# Scrolling YDraw Overlay Demo: Multi-page scrolling test
# Sends ONE primitive group per OSC sequence to force multi-page scrolling
# Each echo advances cursor, testing cursor-relative positioning

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Helper to send YAML payload
send_yaml() {
    local yaml="$1"
    local payload=$(echo "$yaml" | base64 -w0)
    printf '\033]666674;--yaml;%s\033\\' "$payload"
}

# Page 1: Circles
echo "=== PAGE 1: CIRCLES ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - circle:
      position: [100, 30]
      radius: 25
      fill: "#e74c3c"
      stroke: "#c0392b"
      stroke-width: 2
  - circle:
      position: [200, 30]
      radius: 25
      fill: "#3498db"
      stroke: "#2980b9"
      stroke-width: 2
  - circle:
      position: [300, 30]
      radius: 25
      fill: "#2ecc71"
      stroke: "#27ae60"
      stroke-width: 2
  - text:
      position: [50, 70]
      content: "Page 1: Circles"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 2: Boxes
echo "=== PAGE 2: BOXES ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - box:
      position: [100, 30]
      size: [50, 40]
      fill: "#9b59b6"
      stroke: "#8e44ad"
      stroke-width: 2
      round: 5
  - box:
      position: [200, 30]
      size: [50, 40]
      fill: "#f1c40f"
      stroke: "#f39c12"
      stroke-width: 2
      round: 5
  - box:
      position: [300, 30]
      size: [50, 40]
      fill: "#1abc9c"
      stroke: "#16a085"
      stroke-width: 2
      round: 5
  - text:
      position: [50, 70]
      content: "Page 2: Boxes"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 3: Triangles
echo "=== PAGE 3: TRIANGLES ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - triangle:
      p0: [75, 55]
      p1: [125, 55]
      p2: [100, 15]
      fill: "#e67e22"
      stroke: "#d35400"
      stroke-width: 2
  - triangle:
      p0: [175, 55]
      p1: [225, 55]
      p2: [200, 15]
      fill: "#34495e"
      stroke: "#2c3e50"
      stroke-width: 2
  - triangle:
      p0: [275, 55]
      p1: [325, 55]
      p2: [300, 15]
      fill: "#e74c3c"
      stroke: "#c0392b"
      stroke-width: 2
  - text:
      position: [50, 70]
      content: "Page 3: Triangles"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 4: Ellipses
echo "=== PAGE 4: ELLIPSES ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - ellipse:
      position: [100, 35]
      radii: [35, 20]
      fill: "#3498db"
      stroke: "#2980b9"
      stroke-width: 2
  - ellipse:
      position: [200, 35]
      radii: [25, 30]
      fill: "#2ecc71"
      stroke: "#27ae60"
      stroke-width: 2
  - ellipse:
      position: [300, 35]
      radii: [40, 15]
      fill: "#9b59b6"
      stroke: "#8e44ad"
      stroke-width: 2
  - text:
      position: [50, 70]
      content: "Page 4: Ellipses"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 5: Segments
echo "=== PAGE 5: SEGMENTS ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - segment:
      from: [50, 30]
      to: [150, 30]
      stroke: "#e74c3c"
      stroke-width: 4
  - segment:
      from: [170, 20]
      to: [250, 50]
      stroke: "#3498db"
      stroke-width: 3
  - segment:
      from: [270, 50]
      to: [350, 20]
      stroke: "#2ecc71"
      stroke-width: 5
  - text:
      position: [50, 70]
      content: "Page 5: Segments"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 6: Polygons
echo "=== PAGE 6: POLYGONS ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - polygon:
      vertices: [80, 10, 100, 30, 95, 55, 65, 55, 60, 30]
      fill: "#f1c40f"
      stroke: "#f39c12"
      stroke-width: 2
  - polygon:
      vertices: [180, 10, 200, 30, 195, 55, 165, 55, 160, 30]
      fill: "#1abc9c"
      stroke: "#16a085"
      stroke-width: 2
  - polygon:
      vertices: [280, 10, 300, 30, 295, 55, 265, 55, 260, 30]
      fill: "#e67e22"
      stroke: "#d35400"
      stroke-width: 2
  - text:
      position: [50, 70]
      content: "Page 6: Polygons"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 7: Bezier curves
echo "=== PAGE 7: BEZIERS ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - bezier:
      p0: [50, 50]
      p1: [100, 10]
      p2: [150, 50]
      stroke: "#e74c3c"
      stroke-width: 3
  - bezier:
      p0: [170, 50]
      p1: [220, 20]
      p2: [270, 60]
      stroke: "#3498db"
      stroke-width: 3
  - bezier:
      p0: [290, 30]
      p1: [320, 70]
      p2: [350, 30]
      stroke: "#2ecc71"
      stroke-width: 3
  - text:
      position: [50, 70]
      content: "Page 7: Beziers"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 8: Arcs
echo "=== PAGE 8: ARCS ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - arc:
      position: [100, 35]
      radius: 30
      thickness: 8
      angle: 270
      fill: "#f1c40f"
  - arc:
      position: [200, 35]
      radius: 25
      thickness: 10
      angle: 180
      fill: "#1abc9c"
  - arc:
      position: [300, 35]
      radius: 35
      thickness: 6
      angle: 320
      fill: "#e67e22"
  - text:
      position: [50, 70]
      content: "Page 8: Arcs"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 9: Mixed primitives
echo "=== PAGE 9: MIXED ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - circle:
      position: [80, 30]
      radius: 20
      fill: "#e74c3c"
  - box:
      position: [160, 30]
      size: [40, 30]
      fill: "#3498db"
      round: 4
  - triangle:
      p0: [230, 50]
      p1: [270, 50]
      p2: [250, 15]
      fill: "#2ecc71"
  - ellipse:
      position: [330, 30]
      radii: [25, 18]
      fill: "#9b59b6"
  - text:
      position: [50, 70]
      content: "Page 9: Mixed"
      font-size: 16
      color: "#ffffff"
EOF
)"
echo ""
echo ""

# Page 10: Final page
echo "=== PAGE 10: FINAL ==="
send_yaml "$(cat <<'EOF'
background: "#00000000"
body:
  - box:
      position: [200, 30]
      size: [200, 40]
      fill: "#2c3e50"
      round: 10
  - text:
      position: [120, 55]
      content: "Multi-page scrolling complete!"
      font-size: 18
      color: "#ecf0f1"
EOF
)"
echo ""

echo "=== END OF DEMO ==="
echo "All 10 pages sent. Shapes should be visible at different scroll positions."
