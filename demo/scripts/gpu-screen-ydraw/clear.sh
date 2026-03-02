#!/bin/bash
# Screen Draw Layer: Clear the overlay
# Removes all drawings from the screen draw layer

# Send OSC 666673 with --clear argument
printf '\033]666673;--clear;\033\\'

echo "Screen draw layer cleared"
