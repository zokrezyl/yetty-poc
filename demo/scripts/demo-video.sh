#!/bin/bash
# Yetty Video Demo - Comprehensive showcase
# Every demo has: ytext description + content + distortion effects
# Usage: ./demo/scripts/demo-video.sh

set -e

DIR="$(cd "$(dirname "$0")" && pwd)"
DEMO_ROOT="$(cd "$DIR/../.." && pwd)"
cd "$DEMO_ROOT"

# Timing
EFFECT_PAUSE=3
DEMO_DURATION=4
BETWEEN_PAUSE=1
STARWARS_TIME=25
DESC_TIME=5

# Terminal dimensions
TERM_COLS=$(tput cols 2>/dev/null || echo 80)
TERM_ROWS=$(tput lines 2>/dev/null || echo 24)

# yetty-client command
YC="uv run python3 tools/yetty-client/main.py"

#=============================================================================
# HELPER FUNCTIONS
#=============================================================================

clear_all() {
    $YC kill --all 2>/dev/null || true
    printf '\e]666667;0\a'
    printf '\e]666668;0\a'
    sleep 0.3
}

kill_ytext() {
    $YC kill --plugin ytext 2>/dev/null || true
    sleep 0.2
}

# Show description card (horizontal scroll at top)
show_desc() {
    local text="$1"
    $YC create ytext \
        -I "$text" \
        --scroll-x 80 --loop \
        -w $((TERM_COLS - 4)) -H 2 \
        -x 2 -y 1 2>/dev/null || true
}

# Apply post-effects sequence
apply_effects() {
    echo "    >> CRT"
    printf '\e]666668;2:0.4:0.15:0.3\a'
    sleep $EFFECT_PAUSE

    echo "    >> Scanlines"
    printf '\e]666668;1:0.3:2.0\a'
    sleep $EFFECT_PAUSE

    echo "    >> Chromatic Aberration"
    printf '\e]666668;3:0.5\a'
    sleep $EFFECT_PAUSE

    echo "    >> Wave"
    printf '\e]666668;8:0.3:0.05:2.0\a'
    sleep $EFFECT_PAUSE

    echo "    >> Matrix"
    printf '\e]666668;5:0.7:1.5\a'
    sleep $EFFECT_PAUSE

    echo "    >> Night Vision"
    printf '\e]666668;10:0.8:0.15:0.5\a'
    sleep $EFFECT_PAUSE

    echo "    >> Glitch"
    printf '\e]666668;13:0.4:15.0:4.0\a'
    sleep $EFFECT_PAUSE

    printf '\e]666668;0\a'
}

# Apply fewer effects (for shorter demos)
apply_few_effects() {
    echo "    >> CRT"
    printf '\e]666668;2:0.4:0.15:0.3\a'
    sleep $EFFECT_PAUSE

    echo "    >> Matrix"
    printf '\e]666668;5:0.7:1.5\a'
    sleep $EFFECT_PAUSE

    echo "    >> Glitch"
    printf '\e]666668;13:0.4:15.0:4.0\a'
    sleep $EFFECT_PAUSE

    printf '\e]666668;0\a'
}

#=============================================================================
# OPENING: STAR WARS CRAWL
#=============================================================================

clear
clear_all

SW_WIDTH=$((TERM_COLS - 2))
SW_HEIGHT=$((TERM_ROWS - 2))


STARWARS_TEXT="










































A long time ago in a terminal far, far away....




                              Y E T T Y

                         Terminal Unchained



          It is a period of innovation. A new terminal
          has emerged, powered by WebGPU.

          Inline graphics, shader glyphs, and GPU-accelerated
          effects bring unprecedented power to the command line.

          This demo showcases the capabilities of YETTY,
          the terminal that breaks all boundaries....












































"
$YC create ytext \
    -I "$STARWARS_TEXT" \
    --scroll-y 20 --loop \
    --perspective --effect-strength 0.8 \
    --font-size 32 \
    -w "$SW_WIDTH" -H "$SW_HEIGHT" \
    -x 1 -y 1 2>/dev/null || true

sleep $STARWARS_TIME
kill_ytext
sleep $BETWEEN_PAUSE

#=============================================================================
# IMAGE CARD
#=============================================================================

clear
clear_all
show_desc ">>> IMAGE CARD - Display inline images with GPU texture rendering - Images scroll with terminal content <<<"
sleep $DESC_TIME

bash "$DIR/cards/logo.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# PDF CARD
#=============================================================================

clear
clear_all
show_desc ">>> PDF CARD - View PDF documents directly in your terminal - Multi-page support with GPU rendering <<<"
sleep $DESC_TIME

bash "$DIR/cards/pdf.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# PLOT CARD
#=============================================================================

clear
clear_all
show_desc ">>> PLOT CARD - GPU-accelerated data visualization - Real-time plotting with shader glyphs <<<"
sleep $DESC_TIME

bash "$DIR/cards/plot.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# YDRAW - 2D VECTOR GRAPHICS
#=============================================================================

clear
clear_all
show_desc ">>> YDRAW - 2D Vector Graphics with SDF primitives - O(1) spatial hash lookup for hundreds of shapes <<<"
sleep $DESC_TIME

bash "$DIR/cards/ydraw-big.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_effects
kill_ytext
clear_all

#=============================================================================
# YDRAW ZOO
#=============================================================================

clear
clear_all
show_desc ">>> YDRAW ZOO - Infinite zoom animation with procedural shapes - GPU-accelerated SDF rendering <<<"
sleep $DESC_TIME

bash "$DIR/cards/ydraw-zoo.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# YDRAW 3D
#=============================================================================

clear
clear_all
show_desc ">>> YDRAW 3D - Raymarched 3D SDF primitives - Spheres, boxes, toruses rendered via raymarching <<<"
sleep $DESC_TIME

bash "$DIR/cards/ydraw-3d.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_effects
kill_ytext
clear_all

#=============================================================================
# THORVG LOTTIE
#=============================================================================

clear
clear_all
show_desc ">>> THORVG LOTTIE - Vector animations via ThorVG engine - Smooth Lottie JSON animations <<<"
sleep $DESC_TIME

bash "$DIR/cards/thorvg-lottie.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# THORVG SVG
#=============================================================================

clear
clear_all
show_desc ">>> THORVG SVG - Scalable Vector Graphics rendering - SVG files displayed inline <<<"
sleep $DESC_TIME

bash "$DIR/cards/thorvg-svg.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# SHADER GLYPHS
#=============================================================================

clear
clear_all
show_desc ">>> SHADER GLYPHS - Each character is a tiny GPU shader program - Procedural patterns per glyph <<<"
sleep $DESC_TIME

cat "$DEMO_ROOT/demo/files/shader-glyphs.txt" 2>/dev/null || true
sleep $DEMO_DURATION
apply_effects
kill_ytext
clear_all

#=============================================================================
# SHADER GLYPH: BIOMINE
#=============================================================================

clear
clear_all
show_desc ">>> BIOMINE - Organic procedural pattern shader glyph - Living cellular animation <<<"
sleep $DESC_TIME

cat "$DEMO_ROOT/demo/files/shader-glyphs/biomine.txt" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# SHADER GLYPH: MANDELBROT
#=============================================================================

clear
clear_all
show_desc ">>> MANDELBROT DECO - Fractal shader glyph - Mandelbrot set with decorative styling <<<"
sleep $DESC_TIME

cat "$DEMO_ROOT/demo/files/shader-glyphs/mandelbrot-deco.txt" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# SHADER GLYPH: VORONOI
#=============================================================================

clear
clear_all
show_desc ">>> VORONOI - Voronoi tessellation shader glyph - Animated cellular pattern <<<"
sleep $DESC_TIME

cat "$DEMO_ROOT/demo/files/shader-glyphs/voronoi.txt" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# TEXT STYLES
#=============================================================================

clear
clear_all
show_desc ">>> TEXT STYLES - MSDF font rendering - Bold, italic, underline, colors - Crisp at any zoom <<<"
sleep $DESC_TIME

bash "$DIR/text-style.sh" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# EMOJIS
#=============================================================================

clear
clear_all
show_desc ">>> EMOJI RENDERING - Full Unicode emoji support - Color emojis rendered inline <<<"
sleep $DESC_TIME

cat "$DEMO_ROOT/demo/files/emojis.txt" 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# POST-PROCESSING EFFECTS SHOWCASE
#=============================================================================

clear
clear_all
show_desc ">>> POST-PROCESSING EFFECTS - 17 real-time GPU effects - CRT, Matrix, Glitch, Thermal, and more <<<"
sleep $DESC_TIME

echo ""
echo "    The quick brown fox jumps over the lazy dog."
echo "    ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789"
echo ""

echo "    [1] Scanlines"
printf '\e]666668;1:0.3:2.0\a'
sleep 2

echo "    [2] CRT Monitor"
printf '\e]666668;2:0.4:0.15:0.3\a'
sleep 2

echo "    [3] Chromatic Aberration"
printf '\e]666668;3:0.5\a'
sleep 2

echo "    [4] Broken TV"
printf '\e]666668;4:0.5:0.3:1.0\a'
sleep 2

echo "    [5] Matrix"
printf '\e]666668;5:0.8:2.0\a'
sleep 2

echo "    [6] Sepia"
printf '\e]666668;6:0.8:0.1\a'
sleep 2

echo "    [7] Pixelate"
printf '\e]666668;7:4.0:8.0\a'
sleep 2

echo "    [8] Wave"
printf '\e]666668;8:0.3:0.05:2.0\a'
sleep 2

echo "    [9] Invert"
printf '\e]666668;9:1.0\a'
sleep 2

echo "    [10] Night Vision"
printf '\e]666668;10:0.9:0.15:0.6\a'
sleep 2

echo "    [11] Vaporwave"
printf '\e]666668;11:0.4:0.5\a'
sleep 2

echo "    [12] Thermal"
printf '\e]666668;12:1.0\a'
sleep 2

echo "    [13] Glitch"
printf '\e]666668;13:0.5:20.0:5.0\a'
sleep 2

echo "    [14] Emboss"
printf '\e]666668;14:1.0:0.785\a'
sleep 2

echo "    [15] Rain"
printf '\e]666668;15:0.3:3.0:0.15\a'
sleep 2

echo "    [16] Matrix Rain"
printf '\e]666668;16:0.5:1.5:15.0:0.9:0.5\a'
sleep 2

echo "    [17] Drip"
printf '\e]666668;17:0.8:10.0:0.8:0.0\a'
sleep 2

kill_ytext
clear_all

#=============================================================================
# PRE-PROCESSING EFFECTS
#=============================================================================

clear
clear_all
show_desc ">>> PRE-PROCESSING EFFECTS - Text transformations before rendering - Scramble, Wave, Matrix falling <<<"
sleep $DESC_TIME

echo ""
echo "    Pre-processing effects demo"
echo ""

echo "    [1] Scramble"
printf '\e]666667;1:0.3:1.0\a'
sleep 3

echo "    [2] Rain Characters"
printf '\e]666667;2:0.15:2.0\a'
sleep 3

echo "    [3] Wave Text"
printf '\e]666667;3:0.3:0.3:2.0\a'
sleep 3

echo "    [4] Matrix Falling"
printf '\e]666667;4:0.4:1.5:12.0\a'
sleep 3

kill_ytext
clear_all

#=============================================================================
# YTEXT PROJECTION EFFECTS
#=============================================================================

clear
clear_all
show_desc ">>> YTEXT PROJECTIONS - GPU-animated text with 3D projection effects - Cylinder, Sphere, Ripple <<<"
sleep $DESC_TIME

# Cylinder
$YC create ytext \
    -I "\n\n\nCylinder Projection\nText on a rolling drum\nCurves at top and bottom\nWith realistic lighting\nGPU-powered animation\n\n\n" \
    --scroll-y 30 --loop --cylinder --effect-strength 0.7 \
    -w 45 -H 12 -x 5 -y 5 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

# Sphere
clear
show_desc ">>> SPHERE PROJECTION - Text mapped onto a spherical surface - Curves away at all edges <<<"
sleep $DESC_TIME

$YC create ytext \
    -I "\n\n\nSphere Projection\nText on a globe\nBoth X and Y curve\nAt the edges\n\n\n" \
    --scroll-y 25 --loop --sphere --effect-strength 0.7 \
    -w 40 -H 14 -x 5 -y 5 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

# Ripple
clear
show_desc ">>> WATER RIPPLE - Concentric waves from center - Text on water surface effect <<<"
sleep $DESC_TIME

$YC create ytext \
    -I "\n\n\nWater Ripple Effect\nConcentric waves\nRadial foreshortening\nLike a water drop\n\n\n" \
    --scroll-y 20 --loop --ripple --effect-strength 0.5 --frequency 4.0 \
    -w 40 -H 12 -x 5 -y 5 2>/dev/null || true
sleep $DEMO_DURATION
apply_few_effects
kill_ytext
clear_all

#=============================================================================
# FASTPLOTLIB (if available)
#=============================================================================

if [ -d "$DIR/fastplotlib" ]; then

    clear
    clear_all
    show_desc ">>> FASTPLOTLIB - Python scientific visualization - GPU-accelerated plots in terminal <<<"
    sleep $DESC_TIME

    bash "$DIR/fastplotlib/lorenz.sh" 2>/dev/null || true
    sleep $DEMO_DURATION
    apply_few_effects
    kill_ytext
    clear_all

    clear
    clear_all
    show_desc ">>> FASTPLOTLIB WAVE3D - 3D wave surface animation - Real-time GPU rendering <<<"
    sleep $DESC_TIME

    bash "$DIR/fastplotlib/wave3d.sh" 2>/dev/null || true
    sleep $DEMO_DURATION
    apply_few_effects
    kill_ytext
    clear_all

    clear
    clear_all
    show_desc ">>> FASTPLOTLIB MANDELBROT - Mandelbrot fractal visualization - Interactive zooming <<<"
    sleep $DESC_TIME

    bash "$DIR/fastplotlib/mandelbrot.sh" 2>/dev/null || true
    sleep $DEMO_DURATION
    apply_few_effects
    kill_ytext
    clear_all

fi

#=============================================================================
# FINALE
#=============================================================================

clear
clear_all

# Finale credits - same structure as opening
FINALE_TEXT="




































































































                              Y E T T Y

                         Terminal Unchained



                     github.com/zokrezyl/yetty


                           MIT License
                     Contributions Welcome!


                           Powered by:
                             WebGPU
                            Dawn/wgpu
                             ThorVG
                           MSDF Fonts


                     Thank you for watching!




































































































"

$YC create ytext \
    -I "$FINALE_TEXT" \
    --scroll-y 18 --loop \
    --perspective --effect-strength 0.8 \
    --font-size 32 \
    -w "$SW_WIDTH" -H "$SW_HEIGHT" \
    -x 1 -y 1 2>/dev/null || true

sleep 15

clear_all
clear

echo ""
echo "    Demo Complete!"
echo ""
echo "    github.com/zokrezyl/yetty"
echo ""
