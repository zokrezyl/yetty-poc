#!/bin/bash
# YPaint Static Layer: Clear the overlay
# Removes all drawings from the static overlay layer (OSC 666675)

printf '\033]666675;--clear;\033\\'
echo "YPaint static layer cleared"
