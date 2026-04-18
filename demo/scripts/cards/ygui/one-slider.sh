#!/bin/bash
# Minimal: single slider
YAML='
widgets:
  - type: slider
    id: sld1
    x: 10
    y: 10
    w: 150
    h: 24
    value: 50
    min: 0
    max: 100
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 5 -r;;%s\033\\' "$PAYLOAD"
echo
