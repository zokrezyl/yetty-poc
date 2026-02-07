#!/bin/bash
# Demo: Plot expression - x*x (parabola)

printf '\033]666666;run -c plot -w 60 -h 20 -r;{x=-2..2; @range=-2..2,0..4; @axes=on; @grid=on: x*x};\033\\'
