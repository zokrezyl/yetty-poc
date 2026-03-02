#!/bin/bash
# ThorVG SVG Tests
# Based on openscad/svg-tests - tests various SVG features
#
# Usage: ./svg-tests.sh [test_name]
# Without args, runs all tests sequentially

WIDTH="${WIDTH:-60}"
HEIGHT="${HEIGHT:-30}"
DELAY="${DELAY:-2}"

show_svg() {
    local name="$1"
    local svg="$2"
    echo "=== Test: $name ==="
    PAYLOAD=$(echo -n "$svg" | base64 -w0)
    printf '\033]666666;run -c thorvg -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
    echo
}

# Test 1: Basic shapes
test_basic_shapes() {
    show_svg "basic-shapes" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <rect x="10" y="10" width="80" height="80" fill="red"/>
  <circle cx="150" cy="50" r="40" fill="green"/>
  <ellipse cx="260" cy="50" rx="50" ry="30" fill="blue"/>
  <line x1="320" y1="10" x2="390" y2="90" stroke="purple" stroke-width="4"/>
  <polygon points="50,150 10,190 90,190" fill="orange"/>
  <polyline points="120,150 140,190 160,150 180,190" stroke="cyan" stroke-width="3" fill="none"/>
</svg>'
}

# Test 2: Stroke properties
test_strokes() {
    show_svg "strokes" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <line x1="10" y1="20" x2="390" y2="20" stroke="black" stroke-width="1"/>
  <line x1="10" y1="50" x2="390" y2="50" stroke="black" stroke-width="5"/>
  <line x1="10" y1="90" x2="390" y2="90" stroke="red" stroke-width="10" stroke-linecap="butt"/>
  <line x1="10" y1="120" x2="390" y2="120" stroke="green" stroke-width="10" stroke-linecap="round"/>
  <line x1="10" y1="150" x2="390" y2="150" stroke="blue" stroke-width="10" stroke-linecap="square"/>
  <path d="M10 180 L100 180 L100 195" stroke="purple" stroke-width="8" fill="none" stroke-linejoin="miter"/>
  <path d="M150 180 L240 180 L240 195" stroke="purple" stroke-width="8" fill="none" stroke-linejoin="round"/>
  <path d="M290 180 L380 180 L380 195" stroke="purple" stroke-width="8" fill="none" stroke-linejoin="bevel"/>
</svg>'
}

# Test 3: Stroke dashes
test_dashes() {
    show_svg "dashes" '<svg viewBox="0 0 400 150" xmlns="http://www.w3.org/2000/svg">
  <line x1="10" y1="20" x2="390" y2="20" stroke="black" stroke-width="3" stroke-dasharray="10,5"/>
  <line x1="10" y1="50" x2="390" y2="50" stroke="red" stroke-width="3" stroke-dasharray="20,10,5,10"/>
  <line x1="10" y1="80" x2="390" y2="80" stroke="blue" stroke-width="3" stroke-dasharray="5"/>
  <rect x="10" y="100" width="380" height="40" stroke="green" stroke-width="3" stroke-dasharray="15,5" fill="none"/>
</svg>'
}

# Test 4: Path commands
test_paths() {
    show_svg "paths" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <!-- M L commands -->
  <path d="M10 50 L50 10 L90 50 L50 90 Z" fill="red"/>
  <!-- Cubic bezier C -->
  <path d="M110 50 C130 10 170 10 190 50 C210 90 170 90 150 50" fill="none" stroke="blue" stroke-width="3"/>
  <!-- Quadratic bezier Q -->
  <path d="M210 50 Q250 10 290 50 Q330 90 370 50" fill="none" stroke="green" stroke-width="3"/>
  <!-- Arc A -->
  <path d="M50 150 A40 40 0 1 1 130 150" fill="none" stroke="purple" stroke-width="3"/>
  <!-- Smooth cubic S -->
  <path d="M150 150 C170 110 190 110 210 150 S250 190 270 150" fill="none" stroke="orange" stroke-width="3"/>
  <!-- Relative commands -->
  <path d="M300 150 l30 -40 l30 40 l-30 40 z" fill="cyan"/>
</svg>'
}

# Test 5: Transforms
test_transforms() {
    show_svg "transforms" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <!-- Original -->
  <rect x="10" y="10" width="40" height="40" fill="gray"/>
  <!-- Translate -->
  <rect x="10" y="10" width="40" height="40" fill="red" transform="translate(60,0)"/>
  <!-- Scale -->
  <rect x="130" y="10" width="40" height="40" fill="green" transform="scale(0.5)"/>
  <!-- Rotate -->
  <rect x="200" y="30" width="40" height="40" fill="blue" transform="rotate(45,220,50)"/>
  <!-- SkewX -->
  <rect x="280" y="10" width="40" height="40" fill="purple" transform="skewX(20)"/>
  <!-- Combined -->
  <rect x="10" y="120" width="40" height="40" fill="orange" transform="translate(50,0) rotate(30) scale(1.2)"/>
</svg>'
}

# Test 6: Opacity and fill-opacity
test_opacity() {
    show_svg "opacity" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <rect x="10" y="10" width="100" height="100" fill="red"/>
  <rect x="60" y="40" width="100" height="100" fill="blue" opacity="0.5"/>
  <rect x="200" y="10" width="100" height="100" fill="green" fill-opacity="0.7"/>
  <rect x="250" y="40" width="100" height="100" fill="yellow" fill-opacity="0.5" stroke="black" stroke-width="3" stroke-opacity="0.3"/>
</svg>'
}

# Test 7: Text
test_text() {
    show_svg "text" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <text x="10" y="30" font-size="24" fill="black">Hello SVG</text>
  <text x="10" y="70" font-size="18" fill="red" font-weight="bold">Bold Text</text>
  <text x="10" y="100" font-size="18" fill="blue" font-style="italic">Italic Text</text>
  <text x="10" y="140" font-size="14" fill="green">
    <tspan x="10" dy="0">Line 1</tspan>
    <tspan x="10" dy="20">Line 2</tspan>
  </text>
  <text x="200" y="100" font-size="36" fill="purple" transform="rotate(-15,200,100)">Rotated</text>
</svg>'
}

# Test 8: Gradients
test_gradients() {
    show_svg "gradients" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <linearGradient id="lg1" x1="0%" y1="0%" x2="100%" y2="0%">
      <stop offset="0%" stop-color="red"/>
      <stop offset="100%" stop-color="blue"/>
    </linearGradient>
    <linearGradient id="lg2" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" stop-color="yellow"/>
      <stop offset="100%" stop-color="green"/>
    </linearGradient>
    <radialGradient id="rg1" cx="50%" cy="50%" r="50%">
      <stop offset="0%" stop-color="white"/>
      <stop offset="100%" stop-color="purple"/>
    </radialGradient>
  </defs>
  <rect x="10" y="10" width="120" height="80" fill="url(#lg1)"/>
  <rect x="140" y="10" width="120" height="80" fill="url(#lg2)"/>
  <circle cx="330" cy="50" r="45" fill="url(#rg1)"/>
  <ellipse cx="70" cy="150" rx="60" ry="35" fill="url(#lg1)"/>
</svg>'
}

# Test 9: Clip paths
test_clippath() {
    show_svg "clippath" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <clipPath id="clip1">
      <circle cx="100" cy="100" r="60"/>
    </clipPath>
    <clipPath id="clip2">
      <polygon points="250,40 210,160 290,80 210,80 290,160"/>
    </clipPath>
  </defs>
  <rect x="20" y="20" width="160" height="160" fill="red" clip-path="url(#clip1)"/>
  <rect x="170" y="20" width="160" height="160" fill="blue" clip-path="url(#clip2)"/>
</svg>'
}

# Test 10: Groups
test_groups() {
    show_svg "groups" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <g fill="red" stroke="black" stroke-width="2">
    <rect x="10" y="10" width="50" height="50"/>
    <rect x="70" y="10" width="50" height="50"/>
  </g>
  <g transform="translate(150,0)" opacity="0.6">
    <circle cx="40" cy="40" r="30" fill="blue"/>
    <circle cx="90" cy="40" r="30" fill="green"/>
  </g>
  <g transform="translate(0,80) scale(0.8)">
    <rect x="10" y="10" width="100" height="100" fill="purple"/>
    <g transform="translate(50,50)">
      <circle cx="0" cy="0" r="30" fill="yellow"/>
    </g>
  </g>
</svg>'
}

# Test 11: Use/Symbol
test_use() {
    show_svg "use" '<svg viewBox="0 0 400 200" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <symbol id="star" viewBox="0 0 100 100">
      <polygon points="50,5 20,95 95,35 5,35 80,95" fill="gold" stroke="orange" stroke-width="2"/>
    </symbol>
    <rect id="box" width="40" height="40" fill="blue"/>
  </defs>
  <use href="#star" x="10" y="10" width="80" height="80"/>
  <use href="#star" x="100" y="10" width="60" height="60"/>
  <use href="#star" x="170" y="20" width="40" height="40"/>
  <use href="#box" x="250" y="30"/>
  <use href="#box" x="300" y="30" fill="red"/>
  <use href="#box" x="350" y="30" transform="rotate(45,370,50)"/>
</svg>'
}

# Test 12: Markers (arrows)
test_markers() {
    show_svg "markers" '<svg viewBox="0 0 400 150" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <marker id="arrow" viewBox="0 0 10 10" refX="5" refY="5" markerWidth="6" markerHeight="6" orient="auto-start-reverse">
      <path d="M 0 0 L 10 5 L 0 10 z" fill="red"/>
    </marker>
    <marker id="dot" viewBox="0 0 10 10" refX="5" refY="5" markerWidth="5" markerHeight="5">
      <circle cx="5" cy="5" r="4" fill="blue"/>
    </marker>
  </defs>
  <line x1="30" y1="30" x2="200" y2="30" stroke="black" stroke-width="2" marker-end="url(#arrow)"/>
  <line x1="30" y1="70" x2="200" y2="70" stroke="black" stroke-width="2" marker-start="url(#arrow)" marker-end="url(#arrow)"/>
  <polyline points="30,110 80,130 130,100 180,130" stroke="green" stroke-width="2" fill="none" marker-start="url(#dot)" marker-mid="url(#dot)" marker-end="url(#dot)"/>
</svg>'
}

run_all() {
    test_basic_shapes; sleep "$DELAY"
    test_strokes; sleep "$DELAY"
    test_dashes; sleep "$DELAY"
    test_paths; sleep "$DELAY"
    test_transforms; sleep "$DELAY"
    test_opacity; sleep "$DELAY"
    test_text; sleep "$DELAY"
    test_gradients; sleep "$DELAY"
    test_clippath; sleep "$DELAY"
    test_groups; sleep "$DELAY"
    test_use; sleep "$DELAY"
    test_markers
    echo "=== All tests complete ==="
}

# Run specific test or all
case "${1:-all}" in
    basic-shapes|shapes) test_basic_shapes ;;
    strokes) test_strokes ;;
    dashes) test_dashes ;;
    paths) test_paths ;;
    transforms) test_transforms ;;
    opacity) test_opacity ;;
    text) test_text ;;
    gradients) test_gradients ;;
    clippath) test_clippath ;;
    groups) test_groups ;;
    use) test_use ;;
    markers) test_markers ;;
    all) run_all ;;
    list)
        echo "Available tests:"
        echo "  basic-shapes, strokes, dashes, paths, transforms,"
        echo "  opacity, text, gradients, clippath, groups, use, markers"
        echo "  all (run all tests), list (show this)"
        ;;
    *) echo "Unknown test: $1. Use 'list' to see available tests." ;;
esac
