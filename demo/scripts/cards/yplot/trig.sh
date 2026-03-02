#!/bin/bash
# Demo: yplot - trigonometric functions using yfsvm bytecode interpreter

printf '\033]666666;run -c yplot -w 80 -h 25 -r;{f=sin(x); g=cos(x); h=tan(x)*0.2 @range=-pi..pi,-2..2 @grid=on @axes=on};\033\\'
