#!/bin/bash
# Demo: Plot expression - animated with time

printf '\033]666666;create -p plot -x 0 -y 0 -w 60 -h 20 -r;--expr "sin(x + time)" --grid --axes;\033\\'
