#!/bin/bash
# Demo: yplot - Gaussian curves with different widths

printf '\033]666666;run -c yplot -w 80 -h 25 -r;{f=exp(-x*x); g=exp(-x*x*0.5)*0.8; h=exp(-x*x*2)*1.2 @range=-4..4,-0.2..1.4 @grid=on @axes=on};\033\\'
