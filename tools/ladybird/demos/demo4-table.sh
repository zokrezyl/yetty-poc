#!/usr/bin/env bash
# Demo 4: HTML Table
set -e
cd "$(dirname "$0")/.."

cat > /tmp/demo4.html <<'EOF'
<!DOCTYPE html>
<html>
<head>
  <title>Table Demo</title>
  <style>
    body { font-family: sans-serif; background: #0d1117; color: #c9d1d9; padding: 20px; }
    h1 { color: #58a6ff; }
    table { border-collapse: collapse; width: 100%; }
    th { background: #21262d; color: #58a6ff; padding: 10px; text-align: left; }
    td { padding: 10px; border-bottom: 1px solid #30363d; }
    tr:hover { background: #161b22; }
  </style>
</head>
<body>
  <h1>System Information</h1>
  <table>
    <tr><th>Component</th><th>Status</th><th>Version</th></tr>
    <tr><td>LibWeb</td><td>Active</td><td>Ladybird main</td></tr>
    <tr><td>YDraw</td><td>Active</td><td>Yetty</td></tr>
    <tr><td>WebGPU</td><td>Active</td><td>Dawn</td></tr>
    <tr><td>Font</td><td>Active</td><td>MSDF</td></tr>
  </table>
</body>
</html>
EOF

./result/bin/libweb-to-ydraw-full /tmp/demo4.html --width 640 --height 480
sleep 5
