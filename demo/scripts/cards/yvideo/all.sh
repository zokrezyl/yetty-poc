#!/bin/bash
# YVideo Demo: Play all unchained videos in sequence
# Shows all three unchained videos one after another

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

for i in 0 1 2; do
    echo "Playing yetty-unchained-$i.mp4..."
    "$DIR/play.sh" "docs/yetty-unchained-$i.mp4" 60 30
    [[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 2 || true
done

echo "All videos played!"
