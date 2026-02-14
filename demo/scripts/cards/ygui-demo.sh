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
    content_h: 900
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
        h: 150
        color: "#4488FF"

      - type: colorpicker
        id: bg
        x: 230
        y: 260
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

      # TabBar with content panels
      - type: tabbar
        id: tabs1
        x: 10
        y: 420
        w: 300
        h: 28
        active: 0
        children:
          - type: tab
            label: "General"
            w: 80
          - type: tab
            label: "Display"
            w: 80
          - type: tab
            label: "About"
            w: 80

      # Tab content panels (positioned below tabbar)
      - type: panel
        id: tab_panel_0
        x: 10
        y: 450
        w: 300
        h: 120
        children:
          - type: checkbox
            id: auto_save
            x: 10
            y: 10
            w: 200
            h: 24
            label: "Auto-save enabled"
            checked: true
          - type: slider
            id: volume
            x: 10
            y: 40
            w: 200
            h: 24
            label: "Volume"
            value: 75
            min: 0
            max: 100
          - type: button
            id: reset_btn
            x: 10
            y: 80
            w: 100
            h: 28
            label: "Reset"

      # ListBox example (moved down)
      - type: listbox
        id: list1
        x: 10
        y: 580
        w: 180
        h: 120
        row_height: 24
        options:
          - "Item One"
          - "Item Two"
          - "Item Three"
          - "Item Four"
          - "Item Five"
          - "Item Six"
          - "Item Seven"
        selected: 1

      # Table example
      - type: table
        id: tbl1
        x: 10
        y: 710
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
          - type: row
            children:
              - type: cell
                label: "Beta"
              - type: cell
                label: "456"
              - type: cell
                label: "Warn"

      # Button with tooltip
      - type: button
        id: btn_bottom
        x: 10
        y: 800
        w: 150
        h: 30
        label: "Hover Me"
        tooltip: "This is a tooltip!"
'

PAYLOAD=$(echo -n "$YAML" | base64 -w0)
printf '\033]666666;run -c ygui -x 0 -y 0 -w 45 -h 20 -r;;%s\033\\' "$PAYLOAD"
echo
