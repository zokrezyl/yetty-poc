#!/bin/bash
# Minimal: single panel (just a box)
YAML='
widgets:
  - type: panel
    id: p1
    x: 10
    y: 10
    w: 150
    h: 100
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 8 -r;;%s\033\\' "$PAYLOAD"
echo
