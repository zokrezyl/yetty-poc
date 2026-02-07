#!/bin/bash
# YText: GPU-animated scrolling text demo with 3D effects
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

DELAY=1

echo "=== YText Demo: GPU-Animated Scrolling Text with Effects ==="
echo ""

# ============================================================================
# BASIC SCROLLING DEMOS
# ============================================================================

echo "--- Demo 1: Vertical Scroll (seamless loop) ---"
TEXT="Hello, YText!
This is GPU-animated scrolling text.
No CPU rebuild needed!
Animation happens entirely in the fragment shader.
Using the time uniform directly.
Line 6
Line 7
Line 8
Line 9
Line 10"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 45 -h 8 -r;--scroll-y 30 --loop;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 2: Horizontal Marquee (seamless loop) ---"
TEXT=">>> Breaking News: YText card now supports GPU-accelerated text scrolling with zero CPU overhead! <<<"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 50 -h 3 -r;--scroll-x 80 --loop;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 3: Ping-Pong Bounce ---"
TEXT="Bouncing text back and forth...
Watch it go!
Ping...
Pong..."
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 35 -h 6 -r;--scroll-y 40 --pingpong;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

# ============================================================================
# PROJECTION EFFECTS (with foreshortening)
# ============================================================================

echo ""
echo "--- Demo 4: Horizontal Cylinder Projection ---"
TEXT="Line 1: Text on a cylinder
Line 2: Rolling like a drum
Line 3: Curves at top and bottom
Line 4: With realistic lighting
Line 5: Seamless loop
Line 6: GPU-powered
Line 7: No CPU rebuild
Line 8: Pure shader magic"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 40 -h 10 -r;--scroll-y 35 --loop --cylinder --effect-strength 0.7;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 5: Vertical Cylinder Projection ---"
TEXT=">>> Scrolling on a vertical cylinder - text wraps around the sides! <<<"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 50 -h 5 -r;--scroll-x 60 --loop --cylinder-v --effect-strength 0.6;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 6: Sphere Projection ---"
TEXT="SPHERE
Text on a globe
Rotating in space
Both X and Y
Curve away
At the edges"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 35 -h 12 -r;--scroll-y 25 --loop --sphere --effect-strength 0.7;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 7: Horizontal Wave Projection (foreshortening) ---"
TEXT="Text on wavy surface
Compressed on slopes
Stretched on peaks
True projection math"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 40 -h 8 -r;--scroll-y 25 --loop --wave-proj --effect-strength 0.6 --frequency 3.0;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 8: Vertical Wave Projection ---"
TEXT="Vertical wave projection
Text warps with surface
Foreshortening effect"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 40 -h 8 -r;--scroll-x 40 --loop --wave-proj-v --effect-strength 0.5 --frequency 3.0;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 9: Water Ripple Projection ---"
TEXT="RIPPLE
Water drop effect
Concentric waves
Radial foreshortening
From center outward"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 35 -h 10 -r;--scroll-y 20 --loop --ripple --effect-strength 0.5 --frequency 4.0;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 10: Star Wars Perspective Crawl ---"
SW_WIDTH=$(($(tput cols 2>/dev/null || echo 80) - 4))
SW_HEIGHT=$(($(tput lines 2>/dev/null || echo 24) - 4))
TEXT="




A long time ago in a galaxy far, far away....




                         STAR WARS


It is a period of civil war. Rebel spaceships,
striking from a hidden base, have won their
first victory against the evil Galactic Empire.

During the battle, Rebel spies managed to steal
secret plans to the Empire's ultimate weapon,
the DEATH STAR, an armored space station with
enough power to destroy an entire planet.



"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 2 -y 2 -w %d -h %d -r;--scroll-y 20 --loop --perspective --effect-strength 0.8;%s\033\\' "$SW_WIDTH" "$SW_HEIGHT" "$PAYLOAD"
echo
sleep $DELAY

# ============================================================================
# DISPLACEMENT EFFECTS (no foreshortening)
# ============================================================================

echo ""
echo "--- Demo 11: Horizontal Wave Displacement ---"
TEXT="Wavy text effect
Ripples move along X axis
Animated in real-time
Pure GPU shader magic"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 40 -h 8 -r;--wave-disp --effect-strength 0.6 --frequency 4.0;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 12: Vertical Wave Displacement ---"
TEXT="Vertical waves
Ripples along Y
Like a flag waving"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 35 -h 8 -r;--wave-disp-v --effect-strength 0.5 --frequency 3.0;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "--- Demo 13: Wave Displacement with Scrolling ---"
TEXT="Scrolling wavy text
Combined effects
Wave + vertical scroll
Mesmerizing!"
PAYLOAD=$(echo -n "$TEXT" | base64 -w0)
printf '\033]666666;run -c ytext -x 0 -y 0 -w 40 -h 8 -r;--scroll-y 25 --loop --wave-disp --effect-strength 0.4 --frequency 3.0;%s\033\\' "$PAYLOAD"
echo
sleep $DELAY

echo ""
echo "=== Demo Complete ==="
echo "Cards will remain visible until killed with: printf '\\033]666666;kill -c ytext\\033\\\\'"
