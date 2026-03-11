#!/bin/bash
# YEcho QR Cards: GPU-rendered QR codes via OSC escape sequences
# NOTE: Run this inside yetty terminal to see the QR codes!
DIR="$(cd "$(dirname "$0")" && pwd)"
YECHO="$DIR/../../../build-desktop-dawn-release/src/yetty/yecho/yecho"

echo "=== YEcho QR Card Demo ==="
echo "Run inside yetty terminal to see GPU-rendered QR codes"
echo

# URL
echo "GitHub URL:"
$YECHO "{card=qr; w=30; h=30: https://github.com}"
echo
echo

# Text content
echo "Plain text:"
$YECHO "{card=qr; w=25; h=25: Hello World!}"
echo
echo

# Wifi config
echo "WiFi config:"
$YECHO "{card=qr; w=30; h=30: WIFI:T:WPA;S:MyNetwork;P:password123;;}"
echo
