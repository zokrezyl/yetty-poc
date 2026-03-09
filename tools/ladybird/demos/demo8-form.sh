#!/usr/bin/env bash
# Demo 8: HTML Form elements
set -e
cd "$(dirname "$0")/.."

cat > /tmp/demo8.html <<'EOF'
<!DOCTYPE html>
<html>
<head>
  <title>Form Demo</title>
  <style>
    body { font-family: sans-serif; background: #2d2d2d; color: #e0e0e0; padding: 20px; }
    h1 { color: #81c784; }
    form { background: #3d3d3d; padding: 20px; border-radius: 8px; max-width: 400px; }
    label { display: block; margin: 10px 0 5px; }
    input, select, textarea { width: 100%; padding: 8px; border: 1px solid #555; border-radius: 4px; background: #2d2d2d; color: #e0e0e0; box-sizing: border-box; }
    button { background: #81c784; color: #000; padding: 10px 20px; border: none; border-radius: 4px; margin-top: 15px; cursor: pointer; }
  </style>
</head>
<body>
  <h1>Contact Form</h1>
  <form>
    <label>Name</label>
    <input type="text" placeholder="Your name">
    <label>Email</label>
    <input type="email" placeholder="your@email.com">
    <label>Subject</label>
    <select><option>General</option><option>Support</option><option>Sales</option></select>
    <label>Message</label>
    <textarea rows="3" placeholder="Your message..."></textarea>
    <button type="submit">Send Message</button>
  </form>
</body>
</html>
EOF

../../build-ladybird/bin/libweb-to-ydraw-full /tmp/demo8.html --width 640 --height 500
sleep 5
