#!/bin/bash
# Demo: Plot expression - cos(x)

printf '\033]666666;run -c plot -w 60 -h 20 -r;{x=-6.28..6.28; @range=-6.28..6.28,-1..1; @axes=on; @grid=on: cos(x)};\033\\'
