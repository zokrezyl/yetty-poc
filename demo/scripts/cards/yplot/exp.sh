#!/bin/bash
# Demo: yplot - exponential function using yfsvm bytecode interpreter

printf '\033]666666;run -c yplot -w 60 -h 20 -r;{f=exp(x) @range=-2..2,0..8 @grid=on @axes=on};\033\\'
