#!/bin/bash
# Demo: yplot - multiple functions using yfsvm bytecode interpreter

printf '\033]666666;run -c yplot -w 80 -h 25 -r;{f=sin(x); g=cos(x); h=sin(x)*cos(x) @range=-pi..pi,-1.5..1.5 @grid=on @axes=on};\033\\'
