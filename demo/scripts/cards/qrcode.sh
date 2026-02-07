#!/bin/bash
# QRCode Card: GPU-rendered QR code
#
# Usage: ./qrcode.sh [text]

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

TEXT="${1:-https://github.com/anthropics/claude-code}"

PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c qrcode -x 0 -y 0 -w 15 -h 15 -r;ecl=M;%s\033\\' "$PAYLOAD"
echo
