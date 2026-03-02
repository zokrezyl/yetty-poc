#!/bin/bash
# ThorVG demo: SVG logo
#
# Displays a stylized logo using SVG paths

WIDTH="${1:-40}"
HEIGHT="${2:-20}"

SVG='<svg viewBox="0 0 300 200" xmlns="http://www.w3.org/2000/svg">
  <!-- Background gradient approximation with shapes -->
  <rect x="0" y="0" width="300" height="200" fill="#1a1a2e"/>
  
  <!-- Y letter using bezier curves -->
  <path d="M50 30 L90 80 L90 170" stroke="#00d4ff" stroke-width="12" fill="none" stroke-linecap="round"/>
  <path d="M130 30 L90 80" stroke="#00d4ff" stroke-width="12" fill="none" stroke-linecap="round"/>
  
  <!-- D letter -->
  <path d="M160 30 L160 170 M160 30 C250 30 250 170 160 170" stroke="#ff6b6b" stroke-width="12" fill="none" stroke-linecap="round"/>
  
  <!-- Decorative circles -->
  <circle cx="40" cy="180" r="15" fill="#ffd93d"/>
  <circle cx="270" cy="30" r="20" fill="#6bcb77"/>
  <circle cx="250" cy="170" r="10" fill="#4d96ff"/>
  
  <!-- Connecting lines -->
  <line x1="55" y1="180" x2="250" y2="30" stroke="#ffffff" stroke-width="1" stroke-opacity="0.3"/>
</svg>'

PAYLOAD=$(echo -n "$SVG" | base64 -w0)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
