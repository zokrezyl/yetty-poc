#!/bin/bash
# Table widget
YAML='
widgets:
  - type: table
    id: tbl1
    x: 10
    y: 10
    w: 300
    h: 80
    columns: [100, 100, 100]
    row_height: 24
    children:
      - type: header
        h: 24
        children:
          - type: cell
            label: "Name"
          - type: cell
            label: "Value"
          - type: cell
            label: "Status"
      - type: row
        children:
          - type: cell
            label: "Alpha"
          - type: cell
            label: "123"
          - type: cell
            label: "OK"
'
PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 35 -h 8 -r;;%s\033\\' "$PAYLOAD"
echo
