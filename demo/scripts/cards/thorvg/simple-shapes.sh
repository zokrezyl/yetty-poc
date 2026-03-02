#!/bin/bash
# ThorVG demo: Simple SVG shapes
#
# Displays basic SVG shapes using the thorvg card

WIDTH="${1:-40}"
HEIGHT="${2:-20}"

SVG='<svg viewBox="0 0 200 200" xmlns="http://www.w3.org/2000/svg">
  <!-- Red circle -->
  <circle cx="50" cy="50" r="40" fill="#FF5555"/>
  
  <!-- Blue rectangle -->
  <rect x="100" y="20" width="80" height="60" fill="#5555FF"/>
  
  <!-- Green line -->
  <line x1="20" y1="120" x2="180" y2="120" stroke="#55FF55" stroke-width="4"/>
  
  <!-- Yellow path (triangle) -->
  <path d="M100 140 L60 190 L140 190 Z" fill="#FFFF55"/>
  
  <!-- Cyan cubic bezier -->
  <path d="M20 180 C60 140 140 220 180 180" stroke="#55FFFF" stroke-width="3" fill="none"/>
</svg>'

PAYLOAD=$(echo -n "$SVG" | base64 -w0)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
