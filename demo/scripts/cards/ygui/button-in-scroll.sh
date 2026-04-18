#!/bin/bash
# Button inside a scroll area (tests offset rendering)
YAML='
widgets:
  - type: scrollarea
    id: scroll1
    x: 0
    y: 0
    w: 180
    h: 80
    content_w: 200
    content_h: 200
    children:
      - type: button
        id: btn1
        x: 10
        y: 10
        w: 100
        h: 30
        label: "Scrolled"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 5 -r;;%s\033\\' "$PAYLOAD"
echo
