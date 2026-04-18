#!/bin/bash
# Demo: yplot - wave interference patterns

printf '\033]666666;run -c yplot -w 80 -h 25 -r;{f=sin(x)+cos(x*1.5); g=sin(x*2)-cos(x); h=sin(x)+sin(x*1.1) @range=-pi..pi,-2.5..2.5 @grid=on @axes=on};\033\\'
