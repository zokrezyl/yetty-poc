#!/bin/bash
# Two columns of widgets (like the full demo layout)
YAML='
widgets:
  - type: button
    id: btn1
    x: 10
    y: 10
    w: 100
    h: 30
    label: "Click Me"
  - type: checkbox
    id: chk1
    x: 10
    y: 50
    w: 150
    h: 24
    label: "Enable"
    checked: true
  - type: slider
    id: sld1
    x: 10
    y: 84
    w: 200
    h: 24
    value: 50
    min: 0
    max: 100
  - type: choicebox
    id: theme
    x: 230
    y: 10
    w: 150
    options: ["Light", "Dark", "System"]
    selected: 1
  - type: colorpicker
    id: accent
    x: 230
    y: 100
    w: 150
    h: 150
    color: "#4488FF"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 45 -h 16 -r;;%s\033\\' "$PAYLOAD"
echo
