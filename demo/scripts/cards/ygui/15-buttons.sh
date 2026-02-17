#!/bin/bash
# 15 buttons (no scroll area) - tests if many prims alone break it
YAML='
widgets:
  - { type: button, id: b1,  x: 10, y: 10,  w: 100, h: 28, label: "Btn 1" }
  - { type: button, id: b2,  x: 10, y: 42,  w: 100, h: 28, label: "Btn 2" }
  - { type: button, id: b3,  x: 10, y: 74,  w: 100, h: 28, label: "Btn 3" }
  - { type: button, id: b4,  x: 10, y: 106, w: 100, h: 28, label: "Btn 4" }
  - { type: button, id: b5,  x: 10, y: 138, w: 100, h: 28, label: "Btn 5" }
  - { type: button, id: b6,  x: 10, y: 170, w: 100, h: 28, label: "Btn 6" }
  - { type: button, id: b7,  x: 10, y: 202, w: 100, h: 28, label: "Btn 7" }
  - { type: button, id: b8,  x: 10, y: 234, w: 100, h: 28, label: "Btn 8" }
  - { type: button, id: b9,  x: 10, y: 266, w: 100, h: 28, label: "Btn 9" }
  - { type: button, id: b10, x: 10, y: 298, w: 100, h: 28, label: "Btn 10" }
  - { type: button, id: b11, x: 10, y: 330, w: 100, h: 28, label: "Btn 11" }
  - { type: button, id: b12, x: 10, y: 362, w: 100, h: 28, label: "Btn 12" }
  - { type: button, id: b13, x: 10, y: 394, w: 100, h: 28, label: "Btn 13" }
  - { type: button, id: b14, x: 10, y: 426, w: 100, h: 28, label: "Btn 14" }
  - { type: button, id: b15, x: 10, y: 458, w: 100, h: 28, label: "Btn 15" }
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 26 -r;;%s\033\\' "$PAYLOAD"
echo
