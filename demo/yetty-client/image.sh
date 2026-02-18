#!/bin/bash
# Demo: Image plugin - display logo image
# Run this inside yetty terminal (works locally and remotely)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CLIENT_DIR="$SCRIPT_DIR/../../tools/yetty-client"
YETTY_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

cd "$CLIENT_DIR" && cat "$YETTY_ROOT/docs/logo.jpeg" | uv run python main.py run image -i - -w 40 -H 20
