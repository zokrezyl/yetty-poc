#!/bin/bash
# YGui card demo - retained mode UI widgets

YAML='
widgets:
  - type: button
    id: btn1
    x: 10
    y: 10
    w: 100
    h: 30
    label: "Click Me"
    on_click: btn_clicked

  - type: checkbox
    id: chk1
    x: 10
    y: 50
    w: 150
    h: 24
    label: "Enable Feature"
    checked: true
    on_change: chk_changed

  - type: slider
    id: sld1
    x: 10
    y: 90
    w: 200
    h: 24
    value: 50
    min: 0
    max: 100
    on_change: sld_changed

  - type: dropdown
    id: dd1
    x: 10
    y: 130
    w: 150
    h: 28
    options:
      - "Option A"
      - "Option B"
      - "Option C"
    selected: 0
    on_change: dd_changed

  - type: progress
    id: prog1
    x: 10
    y: 180
    w: 200
    h: 20
    value: 75
    min: 0
    max: 100

  - type: label
    x: 10
    y: 220
    label: "YGui Demo - Retained Mode UI"
'

PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 40 -h 15 -r;;%s\033\\' "$PAYLOAD"
echo
