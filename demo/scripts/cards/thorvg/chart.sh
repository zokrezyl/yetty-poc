#!/bin/bash
# ThorVG demo: Chart visualization
#
# Demonstrates a simple line chart using SVG paths

WIDTH="${1:-60}"
HEIGHT="${2:-25}"

SVG='<svg viewBox="0 0 500 300" xmlns="http://www.w3.org/2000/svg">
  <!-- Background -->
  <rect x="0" y="0" width="500" height="300" fill="#0d1117"/>
  
  <!-- Grid lines -->
  <line x1="50" y1="250" x2="480" y2="250" stroke="#30363d" stroke-width="1"/>
  <line x1="50" y1="200" x2="480" y2="200" stroke="#30363d" stroke-width="1"/>
  <line x1="50" y1="150" x2="480" y2="150" stroke="#30363d" stroke-width="1"/>
  <line x1="50" y1="100" x2="480" y2="100" stroke="#30363d" stroke-width="1"/>
  <line x1="50" y1="50" x2="480" y2="50" stroke="#30363d" stroke-width="1"/>
  
  <!-- Y-axis -->
  <line x1="50" y1="30" x2="50" y2="260" stroke="#8b949e" stroke-width="2"/>
  <!-- X-axis -->
  <line x1="40" y1="250" x2="490" y2="250" stroke="#8b949e" stroke-width="2"/>
  
  <!-- Data line 1 (blue) - smooth bezier curve through points -->
  <path d="M70 200 C100 180 130 220 160 190 C190 160 220 140 250 150 
           C280 160 310 100 340 80 C370 60 400 90 430 70 C460 50 470 40 470 40" 
        stroke="#58a6ff" stroke-width="3" fill="none" stroke-linecap="round"/>
  
  <!-- Data line 2 (green) -->
  <path d="M70 230 C100 220 130 210 160 200 C190 190 220 180 250 160 
           C280 140 310 150 340 130 C370 110 400 120 430 100 C460 80 470 90 470 90" 
        stroke="#3fb950" stroke-width="3" fill="none" stroke-linecap="round"/>
  
  <!-- Data points (blue) -->
  <circle cx="70" cy="200" r="5" fill="#58a6ff"/>
  <circle cx="160" cy="190" r="5" fill="#58a6ff"/>
  <circle cx="250" cy="150" r="5" fill="#58a6ff"/>
  <circle cx="340" cy="80" r="5" fill="#58a6ff"/>
  <circle cx="430" cy="70" r="5" fill="#58a6ff"/>
  
  <!-- Data points (green) -->
  <circle cx="70" cy="230" r="5" fill="#3fb950"/>
  <circle cx="160" cy="200" r="5" fill="#3fb950"/>
  <circle cx="250" cy="160" r="5" fill="#3fb950"/>
  <circle cx="340" cy="130" r="5" fill="#3fb950"/>
  <circle cx="430" cy="100" r="5" fill="#3fb950"/>
  
  <!-- Legend -->
  <line x1="360" y1="20" x2="390" y2="20" stroke="#58a6ff" stroke-width="3"/>
  <line x1="420" y1="20" x2="450" y2="20" stroke="#3fb950" stroke-width="3"/>
</svg>'

PAYLOAD=$(echo -n "$SVG" | base64 -w0)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
