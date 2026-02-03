#!/bin/bash
# YText: GPU-animated scrolling text demo with 3D effects
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

DELAY=4

echo "=== YText Demo: GPU-Animated Scrolling Text with 3D Effects ==="
echo ""

# ============================================================================
# BASIC SCROLLING DEMOS
# ============================================================================

echo "--- Demo 1: Vertical Scroll (seamless loop) ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Hello, YText!\nThis is GPU-animated scrolling text.\nNo CPU rebuild needed!\nAnimation happens entirely in the fragment shader.\nUsing the time uniform directly.\nLine 6\nLine 7\nLine 8\nLine 9\nLine 10" \
    --scroll-y 30 --loop \
    -w 45 -H 8
sleep $DELAY

echo ""
echo "--- Demo 2: Horizontal Marquee (seamless loop) ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I ">>> Breaking News: YText card now supports GPU-accelerated text scrolling with zero CPU overhead! <<<" \
    --scroll-x 80 --loop \
    -w 50 -H 3
sleep $DELAY

echo ""
echo "--- Demo 3: Ping-Pong Bounce ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Bouncing text back and forth...\nWatch it go!\nPing...\nPong..." \
    --scroll-y 40 --pingpong \
    -w 35 -H 6
sleep $DELAY

# ============================================================================
# CYLINDER EFFECT DEMOS
# ============================================================================

echo ""
echo "--- Demo 4: Horizontal Cylinder (rolling drum) ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Line 1: Text on a cylinder\nLine 2: Rolling like a drum\nLine 3: Curves at top and bottom\nLine 4: With realistic lighting\nLine 5: Seamless loop\nLine 6: GPU-powered\nLine 7: No CPU rebuild\nLine 8: Pure shader magic" \
    --scroll-y 35 --loop --cylinder --effect-strength 0.7 \
    -w 40 -H 10
sleep $DELAY

echo ""
echo "--- Demo 5: Vertical Cylinder (horizontal marquee) ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I ">>> Scrolling on a vertical cylinder - text wraps around the sides! <<<" \
    --scroll-x 60 --loop --cylinder-v --effect-strength 0.6 \
    -w 50 -H 5
sleep $DELAY

echo ""
echo "--- Demo 6: Strong Cylinder Curvature ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "EXTREME CURVE\nVery tight cylinder\nText nearly wraps\nAll the way around" \
    --scroll-y 50 --loop --cylinder --effect-strength 0.95 \
    -w 35 -H 10
sleep $DELAY

# ============================================================================
# SPHERE EFFECT DEMOS
# ============================================================================

echo ""
echo "--- Demo 7: Sphere Effect ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "SPHERE\nText on a globe\nRotating in space\nBoth X and Y\nCurve away\nAt the edges" \
    --scroll-y 25 --loop --sphere --effect-strength 0.7 \
    -w 35 -H 12
sleep $DELAY

# ============================================================================
# WAVE EFFECT DEMOS
# ============================================================================

echo ""
echo "--- Demo 8: Horizontal Wave (ripples) ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Wavy text effect\nRipples move along X axis\nAnimated in real-time\nPure GPU shader magic" \
    --wave --effect-strength 0.6 --frequency 4.0 \
    -w 40 -H 8
sleep $DELAY

echo ""
echo "--- Demo 9: Vertical Wave ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Vertical waves\nRipples along Y\nLike a flag waving" \
    --wave-v --effect-strength 0.5 --frequency 3.0 \
    -w 35 -H 8
sleep $DELAY

echo ""
echo "--- Demo 10: Wave with Scrolling ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Scrolling wavy text\nCombined effects\nWave + vertical scroll\nMesmerizing!" \
    --scroll-y 25 --loop --wave --effect-strength 0.4 --frequency 3.0 \
    -w 40 -H 8
sleep $DELAY

echo ""
echo "--- Demo 11: Subtle Wave ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Subtle gentle wave\nAlmost calm water\nJust a slight ripple" \
    --wave --effect-strength 0.2 --frequency 2.0 \
    -w 35 -H 6
sleep $DELAY

echo ""
echo "=== Demo Complete ==="
echo "Cards will remain visible until killed with: yetty-client kill --plugin ytext"
