#!/bin/bash
# Demo: yplot - sinc function sin(x)/x using yfsvm bytecode interpreter

printf '\033]666666;run -c yplot -w 60 -h 20 -r;{f=sin(x)/x @range=-10..10,-0.5..1.2 @grid=on @axes=on};\033\\'
