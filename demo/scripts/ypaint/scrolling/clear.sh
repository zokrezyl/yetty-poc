#!/bin/bash
# YPaint Scrolling Layer: Clear
# Removes all drawings from the scrolling layer (OSC 666674)

printf '\033]666674;--clear;\033\\'
echo "YPaint scrolling layer cleared"
