#!/bin/bash
# Scroll area where content fits (no scrollbars needed)
YAML='
widgets:
  - type: scrollarea
    id: scroll1
    x: 0
    y: 0
    w: 180
    h: 160
    content_w: 100
    content_h: 100
    children:
      - type: button
        id: btn1
        x: 10
        y: 10
        w: 80
        h: 30
        label: "Hello"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 10 -r;;%s\033\\' "$PAYLOAD"
echo
