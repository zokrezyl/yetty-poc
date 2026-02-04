#!/bin/bash
# Demo: Plot expression - x*x (parabola)

printf '\033]666666;create -p plot -x 0 -y 0 -w 60 -h 20 -r;--expr "x*x" --domain -2,2 --min 0 --max 4 --grid --axes;\033\\'
