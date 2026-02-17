#!/bin/bash
# Collapsing header with children
YAML='
widgets:
  - type: collapsing
    id: col1
    x: 10
    y: 10
    w: 180
    h: 24
    label: "Settings"
    open: true
    children:
      - type: checkbox
        id: chk1
        x: 0
        y: 0
        w: 160
        h: 24
        label: "Option A"
      - type: checkbox
        id: chk2
        x: 0
        y: 0
        w: 160
        h: 24
        label: "Option B"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 22 -h 8 -r;;%s\033\\' "$PAYLOAD"
echo
