#!/bin/bash
# Demo: yplot - Fourier-like sum of harmonics

printf '\033]666666;run -c yplot -w 80 -h 25 -r;{f=sin(x)+sin(x*2)*0.5+sin(x*3)*0.33; g=sin(x); h=sin(x*2)*0.5 @range=-pi..pi,-2..2 @grid=on @axes=on};\033\\'
