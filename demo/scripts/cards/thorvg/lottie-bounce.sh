#!/bin/bash
# ThorVG demo: Lottie animation
#
# Displays an animated Lottie file using the thorvg card

WIDTH="${1:-40}"
HEIGHT="${2:-20}"

# Simple bouncing ball animation in Lottie JSON format
LOTTIE='{
  "v": "5.5.2",
  "fr": 30,
  "ip": 0,
  "op": 60,
  "w": 200,
  "h": 200,
  "nm": "Bouncing Ball",
  "ddd": 0,
  "assets": [],
  "layers": [
    {
      "ddd": 0,
      "ind": 1,
      "ty": 4,
      "nm": "Ball",
      "sr": 1,
      "ks": {
        "o": { "a": 0, "k": 100 },
        "r": { "a": 0, "k": 0 },
        "p": {
          "a": 1,
          "k": [
            {
              "i": { "x": 0.667, "y": 1 },
              "o": { "x": 0.333, "y": 0 },
              "t": 0,
              "s": [100, 40, 0],
              "to": [0, 20, 0],
              "ti": [0, 0, 0]
            },
            {
              "i": { "x": 0.667, "y": 1 },
              "o": { "x": 0.333, "y": 0 },
              "t": 15,
              "s": [100, 160, 0],
              "to": [0, 0, 0],
              "ti": [0, 20, 0]
            },
            {
              "i": { "x": 0.667, "y": 1 },
              "o": { "x": 0.333, "y": 0 },
              "t": 30,
              "s": [100, 40, 0],
              "to": [0, 20, 0],
              "ti": [0, 0, 0]
            },
            {
              "i": { "x": 0.667, "y": 1 },
              "o": { "x": 0.333, "y": 0 },
              "t": 45,
              "s": [100, 160, 0],
              "to": [0, 0, 0],
              "ti": [0, 20, 0]
            },
            {
              "t": 60,
              "s": [100, 40, 0]
            }
          ]
        },
        "a": { "a": 0, "k": [0, 0, 0] },
        "s": { "a": 0, "k": [100, 100, 100] }
      },
      "ao": 0,
      "shapes": [
        {
          "ty": "el",
          "s": { "a": 0, "k": [60, 60] },
          "p": { "a": 0, "k": [0, 0] },
          "nm": "Circle"
        },
        {
          "ty": "fl",
          "c": { "a": 0, "k": [1, 0.4, 0.4, 1] },
          "o": { "a": 0, "k": 100 },
          "r": 1,
          "nm": "Fill"
        }
      ],
      "ip": 0,
      "op": 60,
      "st": 0
    }
  ]
}'

PAYLOAD=$(echo -n "$LOTTIE" | base64 -w0)
printf '\033]666666;run -c thorvg -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
