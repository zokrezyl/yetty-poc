#!/usr/bin/env bash
# Demo 3: Lists and nested content
set -e
cd "$(dirname "$0")/.."

cat > /tmp/demo3.html <<'EOF'
<!DOCTYPE html>
<html>
<head>
  <title>Lists Demo</title>
  <style>
    body { font-family: monospace; background: #282c34; color: #abb2bf; padding: 20px; }
    h2 { color: #61afef; }
    ul { list-style-type: square; }
    ol { list-style-type: decimal; }
    li { margin: 5px 0; }
  </style>
</head>
<body>
  <h2>Unordered List</h2>
  <ul>
    <li>HTML Parsing</li>
    <li>CSS Layout</li>
    <li>DisplayList Generation</li>
    <li>YDraw Rendering</li>
  </ul>

  <h2>Ordered List</h2>
  <ol>
    <li>Parse HTML document</li>
    <li>Build DOM tree</li>
    <li>Apply CSS styles</li>
    <li>Generate display list</li>
    <li>Convert to YDraw primitives</li>
  </ol>
</body>
</html>
EOF

./result/bin/libweb-to-ydraw-full /tmp/demo3.html --width 640 --height 480
sleep 5
