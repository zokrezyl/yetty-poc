#!/bin/bash
# Demo: Plot expression - sin(x)/x (sinc function)

printf '\033]666666;create -p plot -x 0 -y 0 -w 60 -h 20 -r;--expr "sin(x)/x" --domain -10,10 --min -0.5 --max 1.2 --grid --axes;\033\\'
