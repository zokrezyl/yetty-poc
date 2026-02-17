#!/bin/bash
# Minimal: single label (text only, no SDF box)
YAML='
widgets:
  - type: label
    id: lbl1
    x: 10
    y: 10
    w: 150
    h: 24
    label: "Hello World"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 5 -r;;%s\033\\' "$PAYLOAD"
echo
