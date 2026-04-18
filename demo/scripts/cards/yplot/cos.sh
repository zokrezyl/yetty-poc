#!/bin/bash
# Demo: yplot - cos(x) using yfsvm bytecode interpreter

printf '\033]666666;run -c yplot -w 60 -h 20 -r;{f=cos(x) @range=-pi..pi,-1.5..1.5 @grid=on @axes=on};\033\\'
