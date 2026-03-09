#!/usr/bin/env bash
# Demo 7: Color palette
set -e
cd "$(dirname "$0")/.."

cat > /tmp/demo7.html <<'EOF'
<!DOCTYPE html>
<html>
<head>
  <title>Color Palette</title>
  <style>
    body { font-family: monospace; background: #111; color: #fff; padding: 20px; }
    h1 { text-align: center; margin-bottom: 20px; }
    .row { display: flex; gap: 5px; margin: 5px 0; }
    .swatch { width: 60px; height: 40px; display: flex; align-items: center; justify-content: center; font-size: 10px; border-radius: 4px; }
  </style>
</head>
<body>
  <h1>Terminal Color Palette</h1>
  <div class="row">
    <div class="swatch" style="background:#000;color:#fff">Black</div>
    <div class="swatch" style="background:#c00">Red</div>
    <div class="swatch" style="background:#0c0">Green</div>
    <div class="swatch" style="background:#cc0">Yellow</div>
    <div class="swatch" style="background:#00c">Blue</div>
    <div class="swatch" style="background:#c0c">Magenta</div>
    <div class="swatch" style="background:#0cc">Cyan</div>
    <div class="swatch" style="background:#ccc;color:#000">White</div>
  </div>
  <div class="row">
    <div class="swatch" style="background:#555">Bright</div>
    <div class="swatch" style="background:#f55">Red</div>
    <div class="swatch" style="background:#5f5">Green</div>
    <div class="swatch" style="background:#ff5;color:#000">Yellow</div>
    <div class="swatch" style="background:#55f">Blue</div>
    <div class="swatch" style="background:#f5f">Magenta</div>
    <div class="swatch" style="background:#5ff;color:#000">Cyan</div>
    <div class="swatch" style="background:#fff;color:#000">White</div>
  </div>
</body>
</html>
EOF

./result/bin/libweb-to-ydraw-full /tmp/demo7.html --width 640 --height 300
sleep 5
