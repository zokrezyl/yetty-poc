#!/bin/bash
# YHtml: Inline HTML demo
#
# Usage: ./inline.sh [width] [height]
#
# Passes HTML directly as inline content instead of a file.
#
# Requires: yetty built with -DYETTY_CARD_YHTML=ON

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

WIDTH="${1:-60}"
HEIGHT="${2:-20}"

uv run python3 "$PROJECT_ROOT/tools/yetty-client/main.py" create yhtml \
    -I '<html><head><style>
body { color: #cdd6f4; font-size: 14px; margin: 15px; }
h1 { color: #89b4fa; border-bottom: 2px solid #45475a; padding-bottom: 5px; }
.box { background-color: #313244; border: 1px solid #585b70; border-radius: 6px; padding: 10px; margin: 8px 0; }
.green { color: #a6e3a1; }
.yellow { color: #f9e2af; }
</style></head><body>
<h1>Inline HTML</h1>
<div class="box">
<p>This HTML was passed <b>directly on the command line</b>,
not loaded from a file.</p>
<p>Status: <span class="green">OK</span> |
   Mode: <span class="yellow">Inline</span></p>
</div>
<p><i>Useful for quick cards without creating files.</i></p>
</body></html>' \
    -w "$WIDTH" -H "$HEIGHT"
