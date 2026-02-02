#!/bin/bash
# Yetty Demo Showcase - Curated demos for animated GIF recording
# Displays explanatory text before each demo, then runs it
# Usage: ./demo/scripts/demo.sh

set -e

DIR="$(cd "$(dirname "$0")" && pwd)"
DEMO_ROOT="$(cd "$DIR/../.." && pwd)"
cd "$DIR"

# Colors
BOLD='\033[1m'
CYAN='\033[0;36m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
MAGENTA='\033[0;35m'
WHITE='\033[1;37m'
NC='\033[0m'

# Timing (adjust for GIF recording)
INTRO_PAUSE=0.1
DEMO_DURATION=1
BETWEEN_PAUSE=0.1

# Clear screen and show title card
clear
echo ""
echo -e "${GREEN}╔══════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║${NC}                                                                  ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}   ${WHITE}${BOLD}Y E T T Y${NC}  ${CYAN}- Terminal Unchained${NC}                            ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}                                                                  ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}   ${YELLOW}WebGPU-powered terminal with inline graphics${NC}                 ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}                                                                  ${GREEN}║${NC}"
echo -e "${GREEN}╚══════════════════════════════════════════════════════════════════╝${NC}"
echo ""
sleep $INTRO_PAUSE

# Function to show demo intro
show_intro() {
    local title="$1"
    local description="$2"
    clear
    echo ""
    echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${WHITE}${BOLD}$title${NC}"
    echo -e "${YELLOW}$description${NC}"
    echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo ""
    sleep $INTRO_PAUSE
}

# Function to run demo
run_demo() {
    local script="$1"
    bash "$script" 2>/dev/null || true
    sleep $DEMO_DURATION
}

# Function to clear cards
clear_cards() {
    # Try to clear any visible cards
    "$DEMO_ROOT/demo/yetty-client/yetty-client" clear 2>/dev/null || true
    sleep $BETWEEN_PAUSE
}

#=============================================================================
# DEMO SEQUENCE - Curated for best showcase
#=============================================================================

# 1. Logo/Image Card
show_intro "📷 Image Card" "Display inline images that scroll with terminal content"
run_demo "$DIR/cards/logo.sh"
clear_cards

# 2. PDF Rendering
show_intro "📄 PDF Card" "View PDF documents directly in the terminal"
run_demo "$DIR/cards/pdf.sh"
clear_cards

# 3. Plot Card
show_intro "📊 Plot Card" "GPU-accelerated data visualization with shader glyphs"
run_demo "$DIR/cards/plot.sh"
clear_cards

# 4. YDraw - SDF Primitives
show_intro "🎨 YDraw Card" "2D vector graphics with SDF primitives and BVH acceleration"
run_demo "$DIR/cards/ydraw-big.sh"
clear_cards

# 5. YDraw Zoo - Animated shapes
show_intro "🎪 YDraw Zoo" "Infinite zoom animation with procedural shapes"
run_demo "$DIR/cards/ydraw-zoo.sh"
clear_cards

# 6. ThorVG - Lottie Animation
show_intro "🎬 ThorVG Lottie" "Vector animations via ThorVG rendering engine"
run_demo "$DIR/cards/thorvg-lottie.sh"
clear_cards

# 7. ThorVG - SVG
show_intro "🖼️ ThorVG SVG" "Scalable vector graphics rendering"
run_demo "$DIR/cards/thorvg-svg.sh"
clear_cards

# 8. Ymery - ImGui Widgets
show_intro "🎛️ Ymery Card" "Interactive ImGui widgets embedded in terminal"
run_demo "$DIR/cards/ymery.sh"
clear_cards

# 11. YDraw 3D
show_intro "🧊 YDraw 3D" "3D primitives rendered via SDF"
run_demo "$DIR/cards/ydraw-3d.sh"
clear_cards

# 12. Text Styles
show_intro "✨ Rich Text" "MSDF font rendering - crisp at any zoom"
run_demo "$DIR/text-style.sh"
sleep $DEMO_DURATION

#=============================================================================
# SHADER GLYPH FILES (cat demos)
#=============================================================================

# 13. Shader Glyphs Overview
show_intro "🔤 Shader Glyphs" "GPU-rendered glyphs — each character is a tiny shader program"
cat "$DEMO_ROOT/demo/files/shader-glyphs.txt"
sleep $DEMO_DURATION

# 14. Shader Glyph: Biomine
show_intro "🧬 Shader Glyph: Biomine" "Organic procedural pattern rendered per-glyph"
cat "$DEMO_ROOT/demo/files/shader-glyphs/biomine.txt"
sleep $DEMO_DURATION

# 15. Shader Glyph: Butterfly Flock
show_intro "🦋 Shader Glyph: Butterfly Flock" "Animated butterfly swarm via shader glyphs"
cat "$DEMO_ROOT/demo/files/shader-glyphs/butterfly-flock.txt"
sleep $DEMO_DURATION

# 16. Shader Glyph: HG-SDF
show_intro "💎 Shader Glyph: HG-SDF" "Mercury's HG-SDF library rendered as glyphs"
cat "$DEMO_ROOT/demo/files/shader-glyphs/hg-sdf.txt"
sleep $DEMO_DURATION

# 17. Shader Glyph: Looping Spline
show_intro "🔀 Shader Glyph: Looping Spline" "Smooth animated spline curves"
cat "$DEMO_ROOT/demo/files/shader-glyphs/looping-spline.txt"
sleep $DEMO_DURATION

# 18. Shader Glyph: Mandelbrot Deco
show_intro "🌀 Shader Glyph: Mandelbrot Deco" "Mandelbrot fractal with decorative styling"
cat "$DEMO_ROOT/demo/files/shader-glyphs/mandelbrot-deco.txt"
sleep $DEMO_DURATION

# 19. Shader Glyph: Minkowski Tube
show_intro "🔮 Shader Glyph: Minkowski Tube" "Minkowski space tube visualization"
cat "$DEMO_ROOT/demo/files/shader-glyphs/minkowski-tube.txt"
sleep $DEMO_DURATION

# 20. Shader Glyph: Voronoi
show_intro "🔷 Shader Glyph: Voronoi" "Voronoi tessellation pattern"
cat "$DEMO_ROOT/demo/files/shader-glyphs/voronoi.txt"
sleep $DEMO_DURATION

#=============================================================================
# OTHER TEXT FILES (cat demos)
#=============================================================================

# 21. Fractal Matrix
show_intro "🧮 Fractal Matrix" "Shader glyph tiled fractal matrix effect"
cat "$DEMO_ROOT/demo/files/fractal-matrix.txt"
sleep $DEMO_DURATION

# 22. Tiled Effects
show_intro "🎭 Tiled Effects" "Wave, Sparkle, and Plasma — shader glyphs side by side"
cat "$DEMO_ROOT/demo/files/tiled-effects.txt"
sleep $DEMO_DURATION

# 23. Emojis
show_intro "😀 Emoji Rendering" "Unicode emoji support in the terminal"
cat "$DEMO_ROOT/demo/files/emojis.txt"
sleep $DEMO_DURATION

# 24. Text Style
show_intro "📝 Text Styling" "Bold, italic, underline, strikethrough and colors"
cat "$DEMO_ROOT/demo/files/text-style.txt"
sleep $DEMO_DURATION

#=============================================================================
# FASTPLOTLIB DEMOS (Python scientific visualization)
#=============================================================================

# 25. Fastplotlib: Scatter
show_intro "📊 Fastplotlib: Scatter" "Interactive scatter plot via Python fastplotlib"
run_demo "$DIR/fastplotlib/scatter.sh"
clear_cards

# 26. Fastplotlib: Lines
show_intro "📈 Fastplotlib: Lines" "Multiple line plots with GPU acceleration"
run_demo "$DIR/fastplotlib/lines.sh"
clear_cards

# 27. Fastplotlib: Heatmap
show_intro "🌡️ Fastplotlib: Heatmap" "2D heatmap with color mapping"
run_demo "$DIR/fastplotlib/heatmap.sh"
clear_cards

# 28. Fastplotlib: Image
show_intro "🖼️ Fastplotlib: Image" "Image display and manipulation"
run_demo "$DIR/fastplotlib/image.sh"
clear_cards

# 29. Fastplotlib: Sine Wave
show_intro "〰️ Fastplotlib: Sine Wave" "Animated sine wave visualization"
run_demo "$DIR/fastplotlib/sine.sh"
clear_cards

# 30. Fastplotlib: Gaussian
show_intro "📐 Fastplotlib: Gaussian" "3D Gaussian surface plot"
run_demo "$DIR/fastplotlib/gaussian.sh"
clear_cards

# 31. Fastplotlib: Circles
show_intro "⭕ Fastplotlib: Circles" "Animated circles visualization"
run_demo "$DIR/fastplotlib/circles.sh"
clear_cards

# 32. Fastplotlib: Colormap Line
show_intro "🌈 Fastplotlib: Colormap" "Line plot with gradient coloring"
run_demo "$DIR/fastplotlib/colormap_line.sh"
clear_cards

# 33. Fastplotlib: Spiral
show_intro "🌀 Fastplotlib: Spiral" "3D spiral curve"
run_demo "$DIR/fastplotlib/spiral.sh"
clear_cards

# 34. Fastplotlib: Lorenz Attractor
show_intro "🦋 Fastplotlib: Lorenz" "Lorenz strange attractor"
run_demo "$DIR/fastplotlib/lorenz.sh"
clear_cards

# 35. Fastplotlib: Lissajous
show_intro "∞ Fastplotlib: Lissajous" "Lissajous curve patterns"
run_demo "$DIR/fastplotlib/lissajous.sh"
clear_cards

# 36. Fastplotlib: Waveform
show_intro "🔊 Fastplotlib: Waveform" "Audio-style waveform display"
run_demo "$DIR/fastplotlib/waveform.sh"
clear_cards

# 37. Fastplotlib: Ripple
show_intro "💧 Fastplotlib: Ripple" "Ripple wave propagation"
run_demo "$DIR/fastplotlib/ripple.sh"
clear_cards

# 38. Fastplotlib: Orbits
show_intro "🪐 Fastplotlib: Orbits" "Planetary orbit simulation"
run_demo "$DIR/fastplotlib/orbits.sh"
clear_cards

# 39. Fastplotlib: Sphere
show_intro "🌍 Fastplotlib: Sphere" "3D sphere wireframe"
run_demo "$DIR/fastplotlib/sphere.sh"
clear_cards

# 40. Fastplotlib: Terrain
show_intro "🏔️ Fastplotlib: Terrain" "3D terrain heightmap"
run_demo "$DIR/fastplotlib/terrain.sh"
clear_cards

# 41. Fastplotlib: Wave3D
show_intro "🌊 Fastplotlib: Wave3D" "3D wave surface animation"
run_demo "$DIR/fastplotlib/wave3d.sh"
clear_cards

# 42. Fastplotlib: Spectrum
show_intro "📡 Fastplotlib: Spectrum" "Frequency spectrum visualization"
run_demo "$DIR/fastplotlib/spectrum.sh"
clear_cards

# 43. Fastplotlib: Noise
show_intro "🔲 Fastplotlib: Noise" "Procedural noise pattern"
run_demo "$DIR/fastplotlib/noise.sh"
clear_cards

# 44. Fastplotlib: Parametric
show_intro "🎯 Fastplotlib: Parametric" "Parametric curve plotting"
run_demo "$DIR/fastplotlib/parametric.sh"
clear_cards

# 45. Fastplotlib: Grid Plot
show_intro "📋 Fastplotlib: Grid Layout" "Multiple subplots in a grid"
run_demo "$DIR/fastplotlib/gridplot.sh"
clear_cards

# 46. Fastplotlib: Julia Set
show_intro "🌀 Fastplotlib: Julia Set" "Julia set fractal via fastplotlib"
run_demo "$DIR/fastplotlib/julia.sh"
clear_cards

# 47. Fastplotlib: Mandelbrot
show_intro "🔬 Fastplotlib: Mandelbrot" "Mandelbrot fractal visualization"
run_demo "$DIR/fastplotlib/mandelbrot.sh"
clear_cards

#=============================================================================
# FINALE
#=============================================================================

clear
echo ""
echo -e "${GREEN}╔══════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║${NC}                                                                  ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}   ${WHITE}${BOLD}Demo Complete!${NC}                                               ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}                                                                  ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}   ${CYAN}github.com/zokrezyl/yetty${NC}                                     ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}   ${YELLOW}MIT License - Contributions welcome!${NC}                         ${GREEN}║${NC}"
echo -e "${GREEN}║${NC}                                                                  ${GREEN}║${NC}"
echo -e "${GREEN}╚══════════════════════════════════════════════════════════════════╝${NC}"
echo ""
sleep $INTRO_PAUSE
