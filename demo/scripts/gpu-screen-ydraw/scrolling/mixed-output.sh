#!/bin/bash
# Mixed terminal output and shapes - simulates real usage
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

echo "=== Mixed Output Test ==="
echo "Simulating real terminal usage with shapes..."
echo ""

# Simulate command output
echo "$ ls -la"
echo "total 128"
echo "drwxr-xr-x  15 user  staff   480 Mar  9 10:00 ."
echo "drwxr-xr-x   5 user  staff   160 Mar  9 09:00 .."
echo "-rw-r--r--   1 user  staff  1234 Mar  9 10:00 file.txt"
echo ""

# Show a chart
YAML1=$(cat <<'EOF'
background: "#00000000"
body:
  - box:
      position: [10, 5]
      size: [300, 100]
      fill: "#2c3e50"
      round: 8
  - text:
      position: [20, 30]
      content: "CPU Usage: 45%"
      font-size: 16
      color: "#2ecc71"
  - box:
      position: [20, 50]
      size: [135, 20]
      fill: "#27ae60"
      round: 4
  - box:
      position: [20, 50]
      size: [270, 20]
      fill: "#34495e"
      round: 4
  - text:
      position: [20, 90]
      content: "Memory: 8.2GB / 16GB"
      font-size: 14
      color: "#ecf0f1"
EOF
)
printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML1" | base64 -w0)"

echo ""
echo "$ docker ps"
echo "CONTAINER ID   IMAGE          STATUS"
echo "abc123def456   nginx:latest   Up 2 hours"
echo "789xyz000111   redis:alpine   Up 5 hours"
echo ""

# Show status indicator
YAML2=$(cat <<'EOF'
background: "#00000000"
body:
  - circle:
      position: [25, 20]
      radius: 12
      fill: "#2ecc71"
  - text:
      position: [50, 25]
      content: "All systems operational"
      font-size: 14
      color: "#2ecc71"
EOF
)
printf '\033]666674;--yaml;%s\033\\' "$(echo "$YAML2" | base64 -w0)"

echo ""
echo "$ git status"
echo "On branch main"
echo "Your branch is up to date with 'origin/main'."
echo ""
echo "nothing to commit, working tree clean"
echo ""
echo "=== Test Complete ==="
sleep 3
