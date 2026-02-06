#!/bin/bash
# YEcho All: Combined demo of all yecho features
# NOTE: Run this inside yetty terminal for full experience!
DIR="$(cd "$(dirname "$0")" && pwd)"
YECHO="$DIR/../../../build-desktop-dawn-release/src/yetty/yecho/yecho"

echo "╔════════════════════════════════════════════════════════════╗"
echo "║                    YEcho Feature Demo                       ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo

# Section 1: Glyphs
echo "─── Shader Glyphs ───"
$YECHO "@spinner Loading... @check Done! @warning Caution @error Failed"
echo
echo

# Section 2: ANSI Colors
echo "─── ANSI Colors ───"
$YECHO "{color=#ff0000: Red} {color=#00ff00: Green} {color=#0000ff: Blue} {color=#ffff00: Yellow}"
echo
echo

# Section 3: ANSI Styles
echo "─── ANSI Styles ───"
$YECHO "{style=bold: Bold} {style=italic: Italic} {style=underline: Underline} {style=bold|italic: Both}"
echo
echo

# Section 4: Combined
echo "─── Mixed Content ───"
$YECHO "@check {color=#00ff00: SUCCESS}: Build completed in {style=bold: 2.3s}"
echo
$YECHO "@warning {color=#ffaa00: WARNING}: {style=italic: Memory usage at 85%}"
echo
$YECHO "@error {color=#ff0000; style=bold: CRITICAL}: System overload!"
echo
echo

# Section 5: Cards (requires yetty terminal)
echo "─── GPU Cards (yetty terminal only) ───"
echo
echo "YText card (explicit):"
$YECHO "{card=ytext; w=40; h=4: Hello from ytext!}"
echo
echo
echo "YText card (implicit via @font-size):"
$YECHO "{@font-size=24; @fg-color=#FF00FF: Big magenta text}"
echo
echo
echo "Plot card:"
$YECHO "{card=plot; w=50; h=12: f1=sin(x); @f1.color=#FF0000; f2=cos(x); @f2.color=#00FF00}"
echo
echo
echo "QR Code card:"
$YECHO "{card=qr; w=20; h=20: https://yetty.dev}"
echo
