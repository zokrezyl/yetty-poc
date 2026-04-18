#!/bin/bash
# Several widgets without scroll area
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
    w: 150
    h: 24
    value: 50
    min: 0
    max: 100
  - type: progress
    id: prog1
    x: 10
    y: 118
    w: 150
    h: 20
    value: 75
    min: 0
    max: 100
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 10 -r;;%s\033\\' "$PAYLOAD"
echo
