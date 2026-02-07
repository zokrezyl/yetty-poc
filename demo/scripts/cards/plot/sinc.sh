#!/bin/bash
# Demo: Plot expression - sin(x)/x (sinc function)

printf '\033]666666;run -c plot -w 60 -h 20 -r;{x=-10..10; @range=-10..10,-0.5..1.2; @axes=on; @grid=on: sin(x)/x};\033\\'
