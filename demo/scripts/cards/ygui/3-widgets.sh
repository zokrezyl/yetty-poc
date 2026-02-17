#!/bin/bash
# 3 widgets
YAML='
widgets:
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
  - type: button
    id: btn3
    x: 10
    y: 90
    w: 100
    h: 30
    label: "Three"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 8 -r;;%s\033\\' "$PAYLOAD"
echo
