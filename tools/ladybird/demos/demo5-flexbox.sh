#!/usr/bin/env bash
# Demo 5: Flexbox layout
set -e
cd "$(dirname "$0")/.."

cat > /tmp/demo5.html <<'EOF'
<!DOCTYPE html>
<html>
<head>
  <title>Flexbox Demo</title>
  <style>
    body { font-family: sans-serif; background: #1e1e1e; color: #fff; margin: 0; padding: 20px; }
    h1 { color: #569cd6; text-align: center; }
    .container { display: flex; gap: 10px; margin-top: 20px; }
    .box { flex: 1; padding: 20px; text-align: center; border-radius: 8px; }
    .red { background: #f44336; }
    .green { background: #4caf50; }
    .blue { background: #2196f3; }
    .yellow { background: #ffeb3b; color: #000; }
  </style>
</head>
<body>
  <h1>Flexbox Layout</h1>
  <div class="container">
    <div class="box red">Box 1</div>
    <div class="box green">Box 2</div>
    <div class="box blue">Box 3</div>
    <div class="box yellow">Box 4</div>
  </div>
</body>
</html>
EOF

./result/bin/libweb-to-ydraw-full /tmp/demo5.html --width 640 --height 480
sleep 5
