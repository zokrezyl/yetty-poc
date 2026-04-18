#!/bin/bash
# YVideo Demo: Play yetty-unchained-2.mp4
# Simple demo showing the third unchained video

DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$DIR/play.sh" "docs/yetty-unchained-2.mp4" 40 20
