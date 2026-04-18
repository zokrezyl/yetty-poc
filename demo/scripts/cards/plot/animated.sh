#!/bin/bash
# Demo: Plot expression - animated with time

printf '\033]666666;run -c plot -w 60 -h 20 -r;{x=-pi..pi; @axes=on; @grid=on: sin(x + time)};\033\\'
