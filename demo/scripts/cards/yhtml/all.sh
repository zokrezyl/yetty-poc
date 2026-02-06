#!/bin/bash
# YHtml: Run all HTML demos side by side
#
# Usage: ./all.sh
#
# Requires: yetty built with -DYETTY_CARD_YHTML=ON

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

DELAY=1

echo "=== YHtml Demo: HTML/CSS Rendering via litehtml + YDraw SDF ==="
echo ""

echo "--- Demo 1: Basic HTML (headings, lists, text styles) ---"
uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$PROJECT_ROOT/demo/assets/yhtml/basic.html" \
    -w 50 -H 25
sleep $DELAY

echo ""
echo "--- Demo 2: CSS Styled Dashboard ---"
uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$PROJECT_ROOT/demo/assets/yhtml/styled.html" \
    --bg-color 11111b \
    -w 50 -H 30
sleep $DELAY

echo ""
echo "--- Demo 3: Table Layout ---"
uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$PROJECT_ROOT/demo/assets/yhtml/table.html" \
    --bg-color 11111b \
    -w 60 -H 30
sleep $DELAY

echo ""
echo "--- Demo 4: Form Layout ---"
uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$PROJECT_ROOT/demo/assets/yhtml/form.html" \
    --bg-color 1e1e2e \
    -w 50 -H 30
sleep $DELAY

echo ""
echo "--- Demo 5: Landing Page ---"
uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -i "$PROJECT_ROOT/demo/assets/yhtml/landing.html" \
    -w 80 -H 45
sleep $DELAY

echo ""
echo "--- Demo 6: Inline HTML ---"
uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -I '<h1 style="color:#89b4fa">Inline</h1><p>Quick HTML without a file</p>' \
    -w 40 -H 10
sleep $DELAY

echo ""
echo "=== All demos loaded ==="
echo "Kill with: yetty-client kill --plugin yhtml"
