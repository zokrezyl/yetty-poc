#!/usr/bin/env bash
# Demo 6: Fetch URL (requires network)
set -e
cd "$(dirname "$0")/.."

URL="${1:-https://example.com}"
echo "Fetching: $URL"

./result/bin/libweb-to-ydraw-full "$URL" --width 800 --height 600
sleep 5
