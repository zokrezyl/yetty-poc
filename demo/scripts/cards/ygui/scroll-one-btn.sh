#!/bin/bash
# Scroll area with just one button child
YAML='
widgets:
  - type: scrollarea
    id: scroll1
    x: 0
    y: 0
    w: 180
    h: 160
    content_w: 200
    content_h: 400
    children:
      - type: button
        id: btn1
        x: 10
        y: 10
        w: 100
        h: 30
        label: "Hello"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 10 -r;;%s\033\\' "$PAYLOAD"
echo
