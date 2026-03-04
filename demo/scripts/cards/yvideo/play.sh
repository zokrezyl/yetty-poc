#!/bin/bash
# YVideo Demo: Play AV1 video in yetty terminal
#
# This demo converts an MP4 to AV1 and streams frames to the yvideo card.
# Requires: ffmpeg with libaom or libsvtav1 encoder
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

# Check for ffmpeg
if ! command -v ffmpeg &> /dev/null; then
    echo "Error: ffmpeg is required but not installed"
    exit 1
fi

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

AV1_FILE="$TMPDIR/video.obu"

echo "Converting $VIDEO to AV1..."

# Convert to AV1 OBU format (low-delay, fast encoding)
ffmpeg -y -i "$VIDEO" \
    -c:v libaom-av1 \
    -cpu-used 8 \
    -crf 30 \
    -b:v 0 \
    -strict experimental \
    -f obu \
    "$AV1_FILE" 2>/dev/null

if [ ! -f "$AV1_FILE" ]; then
    echo "Error: Failed to convert to AV1"
    exit 1
fi

echo "Streaming to yvideo card..."

# Create the yvideo card
PAYLOAD=$(base64 -w0 < "$AV1_FILE")
printf '\033]666666;run -c yvideo -x 0 -y 0 -w %d -h %d\033\\' "$WIDTH" "$HEIGHT"

# Send the video data
printf '\033]666666;update -c yvideo;%s\033\\' "$PAYLOAD"

echo "Done!"
