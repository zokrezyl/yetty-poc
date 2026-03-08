#!/bin/bash
# Test interleaved text and ydraw overlay

echo "=== BEFORE FIRST OVERLAY ==="
echo "Line 1"
echo "Line 2"
echo "Line 3"

# First overlay - red circle
YAML1=$(cat <<'EOF'
body:
  - circle:
      position: [100, 100]
      radius: 50
      fill: "#ff0000"
EOF
)
printf '\033]666673;--yaml;%s\033\\' "$(echo "$YAML1" | base64 -w0)"

echo ""
echo "=== AFTER FIRST OVERLAY ==="
echo "Line 4"
echo "Line 5"

# Second overlay - blue box
YAML2=$(cat <<'EOF'
body:
  - box:
      position: [300, 100]
      size: [80, 60]
      fill: "#0000ff"
EOF
)
printf '\033]666673;--yaml;%s\033\\' "$(echo "$YAML2" | base64 -w0)"

echo ""
echo "=== AFTER SECOND OVERLAY ==="
echo "Line 6"
echo "Line 7"

sleep 2
