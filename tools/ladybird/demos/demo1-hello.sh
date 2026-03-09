#!/usr/bin/env bash
# Demo 1: Simple Hello World HTML
set -e
cd "$(dirname "$0")/.."

cat > /tmp/demo1.html <<'EOF'
<!DOCTYPE html>
<html>
<head><title>Hello World</title></head>
<body>
  <h1>Hello World!</h1>
  <p>This is LibWeb rendering to YDraw primitives.</p>
</body>
</html>
EOF

./result/bin/libweb-to-ydraw-full /tmp/demo1.html --width 640 --height 480
sleep 5
