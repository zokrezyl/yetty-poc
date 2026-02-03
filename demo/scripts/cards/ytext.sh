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
# PROJECTION EFFECTS (with foreshortening)
# ============================================================================

echo ""
echo "--- Demo 4: Horizontal Cylinder Projection ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Line 1: Text on a cylinder\nLine 2: Rolling like a drum\nLine 3: Curves at top and bottom\nLine 4: With realistic lighting\nLine 5: Seamless loop\nLine 6: GPU-powered\nLine 7: No CPU rebuild\nLine 8: Pure shader magic" \
    --scroll-y 35 --loop --cylinder --effect-strength 0.7 \
    -w 40 -H 10
sleep $DELAY

echo ""
echo "--- Demo 5: Vertical Cylinder Projection ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I ">>> Scrolling on a vertical cylinder - text wraps around the sides! <<<" \
    --scroll-x 60 --loop --cylinder-v --effect-strength 0.6 \
    -w 50 -H 5
sleep $DELAY

echo ""
echo "--- Demo 6: Sphere Projection ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "SPHERE\nText on a globe\nRotating in space\nBoth X and Y\nCurve away\nAt the edges" \
    --scroll-y 25 --loop --sphere --effect-strength 0.7 \
    -w 35 -H 12
sleep $DELAY

echo ""
echo "--- Demo 7: Horizontal Wave Projection (foreshortening) ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Text on wavy surface\nCompressed on slopes\nStretched on peaks\nTrue projection math" \
    --scroll-y 25 --loop --wave-proj --effect-strength 0.6 --frequency 3.0 \
    -w 40 -H 8
sleep $DELAY

echo ""
echo "--- Demo 8: Vertical Wave Projection ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Vertical wave projection\nText warps with surface\nForeshortening effect" \
    --scroll-x 40 --loop --wave-proj-v --effect-strength 0.5 --frequency 3.0 \
    -w 40 -H 8
sleep $DELAY

echo ""
echo "--- Demo 9: Water Ripple Projection ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "RIPPLE\nWater drop effect\nConcentric waves\nRadial foreshortening\nFrom center outward" \
    --scroll-y 20 --loop --ripple --effect-strength 0.5 --frequency 4.0 \
    -w 35 -H 10
sleep $DELAY

echo ""
echo "--- Demo 10: Star Wars Perspective Crawl ---"
# Full terminal width for authentic Star Wars effect
SW_WIDTH=$(($(tput cols 2>/dev/null || echo 80) - 4))
SW_HEIGHT=$(($(tput lines 2>/dev/null || echo 24) - 4))
uv run python3 tools/yetty-client/main.py create ytext \
    -I "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nA long time ago in a galaxy far, far away....\n\n\n\n\n                         STAR WARS\n\n\nIt is a period of civil war. Rebel spaceships,\nstriking from a hidden base, have won their\nfirst victory against the evil Galactic Empire.\n\nDuring the battle, Rebel spies managed to steal\nsecret plans to the Empire's ultimate weapon,\nthe DEATH STAR, an armored space station with\nenough power to destroy an entire planet.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" \
    --scroll-y 20 --loop --perspective --effect-strength 0.8 \
    -w "$SW_WIDTH" -H "$SW_HEIGHT" -x 2 -y 2
sleep $DELAY

# ============================================================================
# DISPLACEMENT EFFECTS (no foreshortening)
# ============================================================================

echo ""
echo "--- Demo 11: Horizontal Wave Displacement ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Wavy text effect\nRipples move along X axis\nAnimated in real-time\nPure GPU shader magic" \
    --wave-disp --effect-strength 0.6 --frequency 4.0 \
    -w 40 -H 8
sleep $DELAY

echo ""
echo "--- Demo 12: Vertical Wave Displacement ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Vertical waves\nRipples along Y\nLike a flag waving" \
    --wave-disp-v --effect-strength 0.5 --frequency 3.0 \
    -w 35 -H 8
sleep $DELAY

echo ""
echo "--- Demo 13: Wave Displacement with Scrolling ---"
uv run python3 tools/yetty-client/main.py create ytext \
    -I "Scrolling wavy text\nCombined effects\nWave + vertical scroll\nMesmerizing!" \
    --scroll-y 25 --loop --wave-disp --effect-strength 0.4 --frequency 3.0 \
    -w 40 -H 8
sleep $DELAY

echo ""
echo "=== Demo Complete ==="
echo "Cards will remain visible until killed with: yetty-client kill --plugin ytext"
