#!/bin/bash
# Demo: Plot expression - cos(x)

printf '\033]666666;create -p plot -x 0 -y 0 -w 60 -h 20 -r;--expr "cos(x)" --domain -6.28,6.28 --grid --axes;\033\\'
