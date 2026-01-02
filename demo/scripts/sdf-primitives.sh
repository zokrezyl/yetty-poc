#!/bin/bash
# SDF Primitives demo - GPU-accelerated 2D shapes

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"

cd "$ROOT_DIR"
~/.local/bin/uv run python tools/yetty-client/main.py run sdf_primitives \
    --file demo/files/sdf/showcase.yaml \
    -w 70 -H 45
