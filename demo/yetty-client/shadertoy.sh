#!/bin/bash
# Demo: ShaderToy plugin - animated plasma shader with mouse interaction
# Run this inside yetty terminal (works locally and remotely)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CLIENT_DIR="$SCRIPT_DIR/../../tools/yetty-client"

cd "$CLIENT_DIR" && cat "$SCRIPT_DIR/plasma.wgsl" | uv run python main.py run shadertoy -i - -w 60 -H 30
