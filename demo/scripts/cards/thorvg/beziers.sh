#!/bin/bash
# ThorVG demo: Bezier curves showcase
#
# Demonstrates cubic bezier curve rendering

WIDTH="${1:-50}"
HEIGHT="${2:-25}"

SVG='<svg viewBox="0 0 400 300" xmlns="http://www.w3.org/2000/svg">
  <rect x="0" y="0" width="400" height="300" fill="#0d1117"/>
  
  <!-- Wave pattern -->
  <path d="M0 150 C100 50 150 250 200 150 C250 50 300 250 400 150" 
        stroke="#58a6ff" stroke-width="4" fill="none"/>
  
  <path d="M0 170 C100 70 150 270 200 170 C250 70 300 270 400 170" 
        stroke="#3fb950" stroke-width="3" fill="none"/>
  
  <path d="M0 190 C100 90 150 290 200 190 C250 90 300 290 400 190" 
        stroke="#f85149" stroke-width="2" fill="none"/>
  
  <!-- Spiral-like curves -->
  <path d="M50 50 C100 0 150 100 100 100 C50 100 100 150 150 100" 
        stroke="#d2a8ff" stroke-width="3" fill="none"/>
  
  <path d="M350 50 C300 0 250 100 300 100 C350 100 300 150 250 100" 
        stroke="#ffa657" stroke-width="3" fill="none"/>
  
  <!-- Heart shape using beziers -->
  <path d="M200 250 C180 230 150 230 150 260 C150 290 200 320 200 320 
           C200 320 250 290 250 260 C250 230 220 230 200 250" 
        fill="#f85149" stroke="#ff7b72" stroke-width="2"/>
  
  <!-- Infinity symbol -->
  <path d="M150 80 C120 50 80 50 80 80 C80 110 120 110 150 80 
           C180 50 220 50 220 80 C220 110 180 110 150 80" 
        stroke="#79c0ff" stroke-width="3" fill="none"/>
</svg>'

PAYLOAD=$(echo -n "$SVG" | base64 -w0)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
