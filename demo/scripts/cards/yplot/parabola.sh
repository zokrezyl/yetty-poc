#!/bin/bash
# Demo: yplot - parabola x^2 using yfsvm bytecode interpreter

printf '\033]666666;run -c yplot -w 60 -h 20 -r;{f=x*x @range=-3..3,-1..9 @grid=on @axes=on};\033\\'
