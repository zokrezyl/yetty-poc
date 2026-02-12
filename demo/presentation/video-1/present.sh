#!/bin/bash
# Yetty Video 1: Overview Presentation Script
#
# Usage: ./present.sh [step_number]
#
# Each step:
#   1. Shows narration text (read this aloud)
#   2. Shows the demo command to run
#   3. Drops you into a shell to run the demo
#   4. Type 'exit' to advance to the next step
#
# You can jump to a specific step: ./present.sh 5

DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$DIR/../../.." && pwd)"
cd "$PROJECT_ROOT"

# yetty-client
YC="uv run python3 tools/yetty-client/main.py"

# Colors
BOLD='\033[1m'
DIM='\033[2m'
CYAN='\033[0;36m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
WHITE='\033[1;37m'
MAGENTA='\033[0;35m'
NC='\033[0m'

# Track step number
STEP=0
START_AT="${1:-1}"

clear_cards() {
    $YC kill --all 2>/dev/null || true
    printf '\e]666667;0\a' 2>/dev/null || true
    printf '\e]666668;0\a' 2>/dev/null || true
}

step() {
    STEP=$((STEP + 1))

    if [ "$STEP" -lt "$START_AT" ]; then
        return 1
    fi

    clear_cards 2>/dev/null
    clear

    local title="$1"
    shift

    # Header
    echo ""
    echo -e "${DIM}────────────────────────────────────────────────────────────────${NC}"
    echo -e "${CYAN}  Step ${STEP}${NC}  ${WHITE}${BOLD}${title}${NC}"
    echo -e "${DIM}────────────────────────────────────────────────────────────────${NC}"
    echo ""

    # Narration lines
    while [ "$#" -gt 1 ]; do
        echo -e "  $1"
        shift
    done

    # Last argument is the demo hint
    local demo_hint="$1"
    echo ""
    echo -e "${DIM}────────────────────────────────────────────────────────────────${NC}"

    if [ -n "$demo_hint" ]; then
        echo -e "  ${YELLOW}Demo:${NC} ${GREEN}${demo_hint}${NC}"
    fi

    echo -e "  ${DIM}Type 'exit' when done to continue to next step${NC}"
    echo -e "${DIM}────────────────────────────────────────────────────────────────${NC}"
    echo ""

    return 0
}

drop_shell() {
    clear_cards 2>/dev/null || true
    "$SHELL" -i || true
    clear_cards 2>/dev/null || true
}

#=============================================================================
# STEP 1: INTRO
#=============================================================================

if step "Introduction" \
    "${WHITE}${BOLD}Yetty is a GPU-accelerated terminal emulator built on WebGPU.${NC}" \
    "" \
    "It does everything a normal terminal does — shell, scrollback, tabs." \
    "But underneath, every single pixel is rendered on the GPU." \
    "" \
    "In this video I will walk you through the main features." \
    "Let me start by showing you what makes the text rendering special." \
    ""; then
    drop_shell
fi

#=============================================================================
# STEP 2: MSDF FONT RENDERING
#=============================================================================

if step "MSDF Font Rendering" \
    "${WHITE}${BOLD}Yetty uses MSDF — Multi-channel Signed Distance Fields — for text.${NC}" \
    "" \
    "https://github.com/Chlumsky/msdfgen" \
    "Each glyph is stored as a distance field, not as pixels." \
    "Three color channels encode the distance to different edges of the letter." \
    "The GPU computes the exact shape boundary per pixel at render time." \
    "" \
    "This means text stays perfectly crisp at any zoom level." \
    "No blur, no aliasing. Try zooming in — the edges stay sharp." \
    "" \
    "${DIM}Zoom in and out with Ctrl+wheel to see the difference.${NC}" \
    "demo/scripts/text-style.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 3: SHADER GLYPHS
#=============================================================================

if step "Shader Glyphs" \
    "${WHITE}${BOLD}In Yetty, a character can be a tiny GPU program.${NC}" \
    "" \
    "We call them shader glyphs. Each one is a WGSL shader that runs" \
    "per-cell on the GPU. A single terminal character cell can contain" \
    "a spinning animation, a fractal, or even a small game." \
    "" \
    "There are 40+ built-in shader glyphs: spinners, fire, DNA helix," \
    "heartbeat, fractals, pong — all running in real time." \
    "" \
    "${DIM}The files below contain special Unicode codepoints that trigger shader glyphs.${NC}" \
    "${DIM}Just cat them and watch.${NC}" \
    "cat demo/files/shader-glyphs.txt"; then
    drop_shell
fi

#=============================================================================
# STEP 4: SHADER GLYPHS - FULL SCREEN
#=============================================================================

if step "Shader Glyphs — Full Screen" \
    "${WHITE}${BOLD}Shader glyphs also work as tiled full-screen effects.${NC}" \
    "" \
    "When you fill the terminal with a shader glyph character," \
    "each cell runs its own instance of the shader." \
    "But they share the same time uniform, so they animate in sync." \
    "The result is a seamless full-screen procedural animation." \
    "" \
    "${DIM}Try these one by one:${NC}" \
    "cat demo/files/shader-glyphs/mandelbrot-deco.txt " \
    "cat demo/files/shader-glyphs/biomine.txt  " \
    "cat demo/files/shader-glyphs/voronoi.txt" \
    "cat demo/files/shader-glyphs.txt" \
    ""; then
    drop_shell
fi

#=============================================================================
# STEP 5: CARDS — CONCEPT
#=============================================================================

if step "The Card System" \
    "${WHITE}${BOLD}Now the big one: Cards.${NC}" \
    "" \
    "Cards are GPU-rendered widgets that live inside the terminal grid." \
    "They scroll with your output, just like text." \
    "Think of it like Jupyter notebook cells, but in a real terminal." \
    "" \
    "Any program that writes to stdout can create a card" \
    "using a simple escape sequence. No special SDK needed." \
    "Python, Bash, C — if it can print, it can draw." \
    "" \
    "Let me show you the different card types." \
    ""; then
    drop_shell
fi

#=============================================================================
# STEP 6: IMAGE CARD
#=============================================================================

if step "Cards — Inline Images" \
    "${WHITE}${BOLD}The simplest card: images.${NC}" \
    "" \
    "PNG, JPEG, WebP — rendered as GPU textures directly in the terminal." \
    "They scroll up with your shell history, just like text." \
    "The GPU handles scaling with a compute shader." \
    "" \
    "demo/scripts/cards/logo.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 7: PLOT CARD
#=============================================================================

if step "Cards — GPU Plots" \
    "${WHITE}${BOLD}Data visualization, right in the terminal.${NC}" \
    "" \
    "The plot card renders line, bar, scatter, and area charts." \
    "Data is stored in a GPU storage buffer and rendered with shaders." \
    "No image conversion — the chart is computed on the GPU per pixel." \
    "" \
    "${DIM}The second command shows an animated plot using a math expression.${NC}" \
    "demo/scripts/cards/plot.sh  |  demo/scripts/cards/plot/animated.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 8: YDRAW — 2D VECTOR GRAPHICS
#=============================================================================

if step "Cards — YDraw: 2D Vector Graphics" \
    "${WHITE}${BOLD}YDraw renders 2D vector graphics using Signed Distance Fields.${NC}" \
    "" \
    "Circles, boxes, lines, bezier curves, stars, hearts — 40+ shape types." \
    "All evaluated mathematically per-pixel on the GPU." \
    "" \
    "For complex scenes, a spatial hash grid gives O(1) primitive lookup." \
    "Instead of checking every shape for every pixel," \
    "the shader only evaluates the few shapes near that pixel." \
    "This gives 10x to 100x speedup on complex scenes." \
    "" \
    "demo/scripts/cards/ydraw-big.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 9: YDRAW — ANIMATIONS
#=============================================================================

if step "Cards — YDraw: Animations" \
    "${WHITE}${BOLD}YDraw also supports animations and procedural content.${NC}" \
    "" \
    "Here is a bouncing ball demo and an infinite-zoom generative animation." \
    "And my favorite — a procedural maze with an animated solver." \
    "" \
    "demo/scripts/cards/ydraw-bouncing.sh  |  demo/scripts/cards/ydraw-zoo.sh  |  demo/scripts/cards/ydraw-maze.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 10: YDRAW 3D
#=============================================================================

if step "Cards — YDraw: 3D Raymarching" \
    "${WHITE}${BOLD}YDraw can also render 3D primitives via raymarching.${NC}" \
    "" \
    "Spheres, boxes, toruses — raymarched in the fragment shader." \
    "This is real 3D rendering happening inside a terminal card." \
    "" \
    "demo/scripts/cards/ydraw-3d.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 11: PDF
#=============================================================================

if step "Cards — PDF Rendering" \
    "${WHITE}${BOLD}You can view PDF documents directly in the terminal.${NC}" \
    "" \
    "The PDF is rendered to a datatype similar to ydraw presented above and then rendered inside the fragment shader to GPU screen"\
    "It scrolls with your output like everything else." \
    "" \
    "demo/scripts/cards/ypdf.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 12: SVG AND LOTTIE
#=============================================================================

if step "Cards — SVG and Lottie Animations" \
    "${WHITE}${BOLD}Vector graphics via ThorVG.${NC}" \
    "" \
    "ThorVG renders SVG files and Lottie JSON animations." \
    "Lottie is the format used by designers for motion graphics —" \
    "now playing inline in your terminal." \
    "" \
    "demo/scripts/cards/thorvg-svg.sh  |  demo/scripts/cards/thorvg-lottie.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 13: MERMAID
#=============================================================================

if step "Cards — Mermaid Diagrams" \
    "${WHITE}${BOLD}Mermaid diagrams rendered inline.${NC}" \
    "" \
    "Write a flowchart in Mermaid syntax, see it rendered" \
    "as a GPU-accelerated vector graphic in the terminal." \
    "" \
    "demo/scripts/cards/mermaid/simple-flowchart.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 14: YHTML
#=============================================================================

if step "Cards — HTML Rendering" \
    "${WHITE}${BOLD}HTML and CSS, rendered inside a terminal card.${NC}" \
    "" \
    "This is an early feature — but it already supports basic layouts," \
    "styled text, tables, and forms." \
    "" \
    "demo/scripts/cards/yhtml/basic.sh  |  demo/scripts/cards/yhtml/landing.sh  |  demo/scripts/cards/yhtml/table.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 15: YMERY — IMGUI
#=============================================================================

if step "Cards — ImGui Widgets" \
    "${WHITE}${BOLD}Interactive GUI widgets inside the terminal.${NC}" \
    "" \
    "Ymery embeds Dear ImGui widgets as cards." \
    "Sliders, buttons, plots, text editors —" \
    "all interactive, all rendered on the GPU." \
    "" \
    "demo/scripts/cards/ymery.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 16: QR CODE
#=============================================================================

if step "Cards — QR Code" \
    "${WHITE}${BOLD}GPU-rendered QR codes from any text.${NC}" \
    "" \
    "A small thing, but handy. Generate a QR code inline" \
    "from any string. Rendered entirely on the GPU." \
    "" \
    "demo/scripts/cards/qrcode.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 17: POST-PROCESSING EFFECTS
#=============================================================================

if step "Post-Processing Effects" \
    "${WHITE}${BOLD}17 real-time post-processing effects.${NC}" \
    "" \
    "After the terminal is rendered, a post-processing pass" \
    "can transform the entire screen." \
    "" \
    "CRT monitor, scanlines, matrix green, night vision," \
    "thermal camera, glitch, vaporwave, chromatic aberration —" \
    "all running as GPU shaders in real time." \
    "" \
    "${DIM}Effects are toggled with escape sequences: printf '\\e]666668;N:params\\a'${NC}" \
    "source demo/scripts/effects-demo.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 18: DISTORTION EFFECTS
#=============================================================================

if step "Distortion Effects" \
    "${WHITE}${BOLD}21 spatial distortion effects.${NC}" \
    "" \
    "These warp the screen geometry itself:" \
    "fisheye, barrel, swirl, jello, underwater," \
    "earthquake, melt, drunk, heartbeat, VHS tear." \
    "" \
    "${DIM}Try a few:${NC}" \
    "demo/scripts/effects/fisheye.sh  |  demo/scripts/effects/jello.sh  |  demo/scripts/effects/underwater.sh  |  demo/scripts/effects/swirl.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 19: FASTPLOTLIB
#=============================================================================

if step "Python Integration — Fastplotlib" \
    "${WHITE}${BOLD}Scientific visualization from Python, rendered in the terminal.${NC}" \
    "" \
    "Fastplotlib is a GPU-accelerated plotting library." \
    "With yetty, its output renders directly as a terminal card." \
    "" \
    "Lorenz attractors, 3D terrain, Mandelbrot sets —" \
    "interactive scientific plots without leaving the terminal." \
    "" \
    "demo/scripts/fastplotlib/lorenz.sh  |  demo/scripts/fastplotlib/terrain.sh  |  demo/scripts/fastplotlib/mandelbrot.sh"; then
    drop_shell
fi

#=============================================================================
# STEP 20: CLOSING
#=============================================================================

if step "Closing" \
    "${WHITE}${BOLD}That is Yetty.${NC}" \
    "" \
    "A terminal emulator where the GPU does the heavy lifting." \
    "MSDF text, shader glyphs, inline cards, real-time effects." \
    "" \
    "It is not a daily driver yet — we are still building." \
    "Test coverage is growing, documentation is sparse," \
    "and there is a lot left to do." \
    "" \
    "But the foundation is there: WebGPU, a card protocol" \
    "that any language can use, and a rendering pipeline" \
    "that treats the terminal as a first-class GPU surface." \
    "" \
    "It is MIT licensed and we welcome contributions." \
    "" \
    "${CYAN}github.com/zokrezyl/yetty${NC}" \
    "" \
    "${DIM}Thanks for watching.${NC}" \
    ""; then
    drop_shell
fi

#=============================================================================
# DONE
#=============================================================================

clear_cards 2>/dev/null
clear
echo ""
echo -e "${GREEN}Presentation complete.${NC}"
echo ""
