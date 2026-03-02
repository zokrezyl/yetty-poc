#!/bin/bash
# Demo: yplot - function compositions

printf '\033]666666;run -c yplot -w 80 -h 25 -r;{f=sin(cos(x*2)); g=cos(sin(x*2)); h=sin(x)*cos(x*2) @range=-pi..pi,-1.2..1.2 @grid=on @axes=on};\033\\'
