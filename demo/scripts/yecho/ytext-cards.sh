#!/bin/bash
# YEcho YText Cards: GPU-rendered styled text via OSC escape sequences
# NOTE: Run this inside yetty terminal to see the styled text cards!
DIR="$(cd "$(dirname "$0")" && pwd)"
YECHO="$DIR/../../../build-desktop-dawn-release/src/yetty/yecho/yecho"

echo "=== YEcho YText Card Demo ==="
echo "Run inside yetty terminal to see GPU-rendered text"
echo

# Explicit ytext card
echo "Explicit ytext card:"
$YECHO "{card=ytext; w=50; h=5: Hello World!}"
echo
echo

# Implicit ytext via @font-size
echo "Large text (implicit ytext via @font-size):"
$YECHO "{@font-size=32: Big Text!}"
echo
echo

# Implicit ytext via @font-family
echo "Monospace font:"
$YECHO "{@font-family=monospace: Code style text}"
echo
echo

# Colored text card
echo "Colored text:"
$YECHO "{card=ytext; w=40; h=5; @fg-color=#FF0000: Red Text}"
echo
echo

# With background
echo "Text with background:"
$YECHO "{card=ytext; w=50; h=6; @fg-color=#FFFFFF; @bg-color=#0000FF: White on Blue}"
echo
echo

# Multiple font attributes
echo "Styled text:"
$YECHO "{@font-size=24; @font-family=serif; @fg-color=#00FF00: Green Serif 24pt}"
echo
