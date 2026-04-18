#!/bin/bash
# Demo: yplot - sin(x) using yfsvm bytecode interpreter

printf '\033]666666;run -c yplot -w 60 -h 20 -r;{f=sin(x) @range=-pi..pi,-1.5..1.5 @grid=on @axes=on};\033\\'
