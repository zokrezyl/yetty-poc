#!/bin/bash
# Scrolling YDraw Overlay: Clear the overlay
# Removes all drawings from the scrolling ydraw overlay

# Send OSC 666674 with --clear argument
printf '\033]666674;--clear;\033\\'

echo "Screen draw layer cleared"
