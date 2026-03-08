#!/bin/bash
# Absolute YDraw Overlay: Clear the overlay
# Removes all drawings from the absolute ydraw overlay

# Send OSC 666673 with --clear argument
printf '\033]666673;--clear;\033\\'

echo "Screen draw layer cleared"
