#!/bin/bash
# Demo: Plot expression - sin(x)

# OSC format: ESC ] 666666;create -p plot -x X -y Y -w W -h H -r;plugin-args;payload ESC \
# For expression mode, no payload needed

printf '\033]666666;create -p plot -x 0 -y 0 -w 60 -h 20 -r;--expr "sin(x)" --grid --axes;\033\\'
