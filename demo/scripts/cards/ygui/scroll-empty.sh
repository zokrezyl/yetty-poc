#!/bin/bash
# Bare scroll area, no children
YAML='
widgets:
  - type: scrollarea
    id: scroll1
    x: 0
    y: 0
    w: 180
    h: 160
    content_w: 300
    content_h: 400
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 10 -r;;%s\033\\' "$PAYLOAD"
echo
