#!/bin/bash
# Scroll area with many children (reproduces the full demo pattern)
YAML='
widgets:
  - type: scrollarea
    id: scroll1
    x: 0
    y: 0
    w: 200
    h: 200
    content_w: 200
    content_h: 500
    children:
      - type: button
        id: btn1
        x: 10
        y: 10
        w: 100
        h: 30
        label: "One"
      - type: button
        id: btn2
        x: 10
        y: 50
        w: 100
        h: 30
        label: "Two"
      - type: checkbox
        id: chk1
        x: 10
        y: 90
        w: 150
        h: 24
        label: "Check"
      - type: slider
        id: sld1
        x: 10
        y: 124
        w: 150
        h: 24
        value: 50
        min: 0
        max: 100
      - type: dropdown
        id: dd1
        x: 10
        y: 158
        w: 150
        h: 28
        options: ["A", "B", "C"]
        selected: 0
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 22 -h 12 -r;;%s\033\\' "$PAYLOAD"
echo
