#!/bin/bash
# Minimal: single button
YAML='
widgets:
  - type: button
    id: btn1
    x: 10
    y: 10
    w: 100
    h: 30
    label: "Click Me"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 5 -r;;%s\033\\' "$PAYLOAD"
echo
