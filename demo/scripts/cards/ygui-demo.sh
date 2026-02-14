#!/bin/bash
# YGui card demo - retained mode UI widgets with scrolling

YAML='
widgets:
  - type: scrollarea
    id: main_scroll
    x: 0
    y: 0
    w: 440
    h: 380
    content_w: 500
    content_h: 550
    children:
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

      - type: separator
        x: 10
        y: 210
        w: 200
        h: 10

      - type: choicebox
        id: theme
        x: 230
        y: 10
        w: 150
        options:
          - "Light"
          - "Dark"
          - "System"
        selected: 1
        on_change: theme_changed

      - type: colorpicker
        id: accent
        x: 230
        y: 100
        w: 150
        h: 24
        label: "Accent Color"
        color: "#4488FF"

      - type: colorpicker
        id: bg
        x: 230
        y: 130
        w: 150
        h: 24
        label: "Background"
        color: "#1A1A2E"

      - type: collapsing
        id: settings
        x: 10
        y: 230
        w: 180
        h: 24
        label: "Settings"
        open: true
        children:
          - type: checkbox
            id: notifications
            x: 0
            y: 0
            w: 160
            h: 24
            label: "Notifications"
            checked: true

          - type: collapsing
            id: advanced
            x: 0
            y: 0
            w: 160
            h: 24
            label: "Advanced"
            children:
              - type: checkbox
                id: debug
                x: 0
                y: 0
                w: 140
                h: 24
                label: "Debug mode"

              - type: checkbox
                id: verbose
                x: 0
                y: 0
                w: 140
                h: 24
                label: "Verbose logs"

              - type: collapsing
                id: experimental
                x: 0
                y: 0
                w: 140
                h: 24
                label: "Experimental"
                children:
                  - type: checkbox
                    id: beta
                    x: 0
                    y: 0
                    w: 120
                    h: 24
                    label: "Beta features"

                  - type: checkbox
                    id: unstable
                    x: 0
                    y: 0
                    w: 120
                    h: 24
                    label: "Unstable API"

      - type: label
        x: 10
        y: 450
        label: "Scroll down to see more! YGui Demo v1.0"

      - type: button
        id: btn_bottom
        x: 10
        y: 480
        w: 150
        h: 30
        label: "Bottom Button"
'

PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 45 -h 20 -r;;%s\033\\' "$PAYLOAD"
echo
