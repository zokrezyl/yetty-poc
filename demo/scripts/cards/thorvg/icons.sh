#!/bin/bash
# ThorVG demo: Icon set
#
# Demonstrates multiple SVG icons rendered with ThorVG

WIDTH="${1:-60}"
HEIGHT="${2:-20}"

SVG='<svg viewBox="0 0 480 160" xmlns="http://www.w3.org/2000/svg">
  <rect x="0" y="0" width="480" height="160" fill="#161b22"/>
  
  <!-- Home icon -->
  <g transform="translate(20, 20)">
    <path d="M60 50 L30 80 L30 110 L50 110 L50 90 L70 90 L70 110 L90 110 L90 80 Z" 
          fill="#58a6ff" stroke="#79c0ff" stroke-width="2"/>
    <path d="M25 80 L60 45 L95 80" stroke="#79c0ff" stroke-width="4" fill="none" stroke-linecap="round"/>
  </g>
  
  <!-- Settings gear icon -->
  <g transform="translate(140, 20)">
    <circle cx="60" cy="60" r="20" fill="none" stroke="#8b949e" stroke-width="6"/>
    <circle cx="60" cy="60" r="8" fill="#8b949e"/>
    <!-- Gear teeth as lines -->
    <line x1="60" y1="30" x2="60" y2="15" stroke="#8b949e" stroke-width="6" stroke-linecap="round"/>
    <line x1="60" y1="90" x2="60" y2="105" stroke="#8b949e" stroke-width="6" stroke-linecap="round"/>
    <line x1="30" y1="60" x2="15" y2="60" stroke="#8b949e" stroke-width="6" stroke-linecap="round"/>
    <line x1="90" y1="60" x2="105" y2="60" stroke="#8b949e" stroke-width="6" stroke-linecap="round"/>
  </g>
  
  <!-- User icon -->
  <g transform="translate(260, 20)">
    <circle cx="60" cy="40" r="20" fill="#3fb950"/>
    <path d="M30 110 C30 70 90 70 90 110" fill="#3fb950"/>
  </g>
  
  <!-- Star icon -->
  <g transform="translate(380, 20)">
    <path d="M60 20 L70 50 L100 50 L75 70 L85 100 L60 80 L35 100 L45 70 L20 50 L50 50 Z" 
          fill="#ffd93d" stroke="#f0c000" stroke-width="2"/>
  </g>
</svg>'

PAYLOAD=$(echo -n "$SVG" | base64 -w0)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
