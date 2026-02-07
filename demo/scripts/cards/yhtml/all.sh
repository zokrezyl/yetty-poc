#!/bin/bash
# YHtml: Run all HTML demos side by side
#
# Usage: ./all.sh
#
# Requires: yetty built with -DYETTY_CARD_YHTML=ON

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"
cd "$PROJECT_ROOT"

DELAY=1

echo "=== YHtml Demo: HTML/CSS Rendering via litehtml + YDraw SDF ==="
echo ""

echo "--- Demo 1: Basic HTML (headings, lists, text styles) ---"
PAYLOAD=$(base64 -w0 < demo/assets/yhtml/basic.html)
printf '\033]666666;run -c yhtml -x 0 -y 0 -w 50 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 2: CSS Styled Dashboard ---"
PAYLOAD=$(base64 -w0 < demo/assets/yhtml/styled.html)
printf '\033]666666;run -c yhtml -x 0 -y 0 -w 50 -h 30 -r;bg-color=11111b;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 3: Table Layout ---"
PAYLOAD=$(base64 -w0 < demo/assets/yhtml/table.html)
printf '\033]666666;run -c yhtml -x 0 -y 0 -w 60 -h 30 -r;bg-color=11111b;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 4: Form Layout ---"
PAYLOAD=$(base64 -w0 < demo/assets/yhtml/form.html)
printf '\033]666666;run -c yhtml -x 0 -y 0 -w 50 -h 30 -r;bg-color=1e1e2e;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 5: Landing Page ---"
PAYLOAD=$(base64 -w0 < demo/assets/yhtml/landing.html)
printf '\033]666666;run -c yhtml -x 0 -y 0 -w 80 -h 45 -r;;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 6: Inline HTML ---"
HTML_CONTENT='<h1 style="color:#89b4fa">Inline</h1><p>Quick HTML without a file</p>'
PAYLOAD=$(echo -n "$HTML_CONTENT" | base64 -w0)
printf '\033]666666;run -c yhtml -x 0 -y 0 -w 40 -h 10 -r;;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "=== All demos loaded ==="
echo "Kill with: printf '\\033]666666;kill -c yhtml\\033\\\\'"
