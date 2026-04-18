#!/bin/bash
# 10 buttons
YAML='
widgets:
  - type: button
    id: b1
    x: 10
    y: 10
    w: 100
    h: 30
    label: "Btn 1"
  - type: button
    id: b2
    x: 10
    y: 50
    w: 100
    h: 30
    label: "Btn 2"
  - type: button
    id: b3
    x: 10
    y: 90
    w: 100
    h: 30
    label: "Btn 3"
  - type: button
    id: b4
    x: 10
    y: 130
    w: 100
    h: 30
    label: "Btn 4"
  - type: button
    id: b5
    x: 10
    y: 170
    w: 100
    h: 30
    label: "Btn 5"
  - type: button
    id: b6
    x: 10
    y: 210
    w: 100
    h: 30
    label: "Btn 6"
  - type: button
    id: b7
    x: 10
    y: 250
    w: 100
    h: 30
    label: "Btn 7"
  - type: button
    id: b8
    x: 10
    y: 290
    w: 100
    h: 30
    label: "Btn 8"
  - type: button
    id: b9
    x: 10
    y: 330
    w: 100
    h: 30
    label: "Btn 9"
  - type: button
    id: b10
    x: 10
    y: 370
    w: 100
    h: 30
    label: "Btn 10"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 20 -h 22 -r;;%s\033\\' "$PAYLOAD"
echo
