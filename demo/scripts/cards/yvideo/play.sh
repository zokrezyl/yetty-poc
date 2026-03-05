#!/bin/bash
# YVideo Demo: Play video in yetty terminal
#
# This demo streams MP4/AV1 video files directly to the yvideo card.
# Supports: MP4 (H.264), raw AV1 OBU
#
# Usage: ./play.sh <video.mp4> [width] [height]

set -e

VIDEO="${1:-docs/yetty-unchained-0.mp4}"
WIDTH="${2:-40}"
HEIGHT="${3:-20}"

if [ ! -f "$VIDEO" ]; then
    echo "Error: Video file not found: $VIDEO"
    exit 1
fi

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

echo "Streaming $VIDEO to yvideo card..."

# Create the yvideo card with video data
PAYLOAD=$(base64 -w0 < "$VIDEO")
printf '\033]666666;run -c yvideo -x 0 -y 0 -w %d -h %d\033\\' "$WIDTH" "$HEIGHT"

# Send the video data (format: vendor;cmd;cardargs;payload - empty cardargs)
printf '\033]666666;update -c yvideo;;%s\033\\' "$PAYLOAD"

echo "Done!"
