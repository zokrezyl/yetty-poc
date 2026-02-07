#!/bin/bash
# Python Card (fastplotlib): Perlin noise visualization
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/python/fastplotlib/noise.py)
printf '\033]666666;run -c python -x 0 -y 0 -w 60 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
