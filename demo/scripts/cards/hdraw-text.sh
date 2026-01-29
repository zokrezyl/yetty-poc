#!/bin/bash
# HDraw: Text rendering demo
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."
uv run python3 tools/yetty-client/main.py --new create hdraw -i demo/assets/cards/hdraw/text.yaml -w 50 -H 20
