#!/bin/bash
# Two buttons side by side
YAML='
widgets:
  - type: button
    id: btn1
    x: 10
    y: 10
    w: 80
    h: 30
    label: "One"
  - type: button
    id: btn2
    x: 100
    y: 10
    w: 80
    h: 30
    label: "Two"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 25 -h 5 -r;;%s\033\\' "$PAYLOAD"
echo
