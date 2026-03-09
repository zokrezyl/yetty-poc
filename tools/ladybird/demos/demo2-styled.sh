#!/usr/bin/env bash
# Demo 2: Styled HTML with CSS
set -e
cd "$(dirname "$0")/.."

cat > /tmp/demo2.html <<'EOF'
<!DOCTYPE html>
<html>
<head>
  <title>Styled Demo</title>
  <style>
    body { font-family: sans-serif; background: #1a1a2e; color: #eee; padding: 20px; }
    h1 { color: #e94560; }
    .box { background: #16213e; padding: 15px; border-radius: 8px; margin: 10px 0; }
    .highlight { color: #0f3460; background: #e94560; padding: 2px 8px; }
  </style>
</head>
<body>
  <h1>Styled Content</h1>
  <div class="box">
    <p>This box has <span class="highlight">highlighted</span> text.</p>
    <p>LibWeb renders CSS styles to YDraw primitives.</p>
  </div>
</body>
</html>
EOF

./result/bin/libweb-to-ydraw-full /tmp/demo2.html --width 640 --height 480
sleep 5
