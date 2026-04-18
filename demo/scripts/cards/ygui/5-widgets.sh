#!/bin/bash
# 5 widgets
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
  - type: button
    id: btn4
    x: 10
    y: 130
    w: 100
    h: 30
    label: "Four"
  - type: button
    id: btn5
    x: 10
    y: 170
    w: 100
    h: 30
    label: "Five"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 12 -r;;%s\033\\' "$PAYLOAD"
echo
