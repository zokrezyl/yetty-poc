#!/bin/bash
# YEcho Plot Cards: GPU-rendered plots via OSC escape sequences
# NOTE: Run this inside yetty terminal to see the plots!
DIR="$(cd "$(dirname "$0")" && pwd)"
YECHO="$DIR/../../../build-desktop-dawn-release/src/yetty/yecho/yecho"

echo "=== YEcho Plot Card Demo ==="
echo "Run inside yetty terminal to see GPU-rendered plots"
echo

# Simple sine wave
echo "Sine wave:"
$YECHO "{card=plot; w=60; h=15: f1=sin(x)}"
echo
echo

# Cosine with color
echo "Colored cosine:"
$YECHO "{card=plot; w=60; h=15: f1=cos(x); @f1.color=#00FF00}"
echo
echo

# Multiple functions
echo "Multiple functions:"
$YECHO "{card=plot; w=60; h=20: f1=sin(x); @f1.color=#FF0000; f2=cos(x); @f2.color=#00FF00}"
echo
echo

# Custom viewport
echo "Custom viewport [-π, π]:"
$YECHO "{card=plot; w=60; h=15; @view=-3.14..3.14,-1..1: sin(x)}"
echo
echo

# Parabola
echo "Parabola:"
$YECHO "{card=plot; w=60; h=15; @view=-2..2,-1..4: x*x}"
echo
echo

# Complex expression
echo "Damped oscillation:"
$YECHO "{card=plot; w=60; h=15; @view=-1..10,-1..1: sin(x*3)*exp(-x*0.3)}"
echo
