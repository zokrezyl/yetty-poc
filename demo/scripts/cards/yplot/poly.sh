#!/bin/bash
# Demo: yplot - polynomial functions

printf '\033]666666;run -c yplot -w 80 -h 25 -r;{f=x*x*x-x; g=x*x-2; h=x*x*x*x-x*x @range=-2..2,-4..4 @grid=on @axes=on};\033\\'
