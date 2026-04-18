#!/bin/bash
# YVideo Demo: Play yetty-unchained-0.mp4
# Simple demo showing the first unchained video

DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$DIR/play.sh" "docs/yetty-unchained-0.mp4" 40 20
