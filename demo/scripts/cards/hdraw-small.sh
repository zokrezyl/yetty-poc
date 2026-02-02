#!/bin/bash
# HDraw Small Demo - Testing new SDF primitives

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

python3 tools/yetty-client/main.py create hdraw -f demo/assets/cards/ydraw/small.yaml -w 40 -H 10

echo  # newline after the card
