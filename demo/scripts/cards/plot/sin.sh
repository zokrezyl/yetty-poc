#!/bin/bash
# Demo: Plot expression - sin(x)

# OSC format: ESC ] 666666;run -c plot -w W -h H -r;{card-args: expression};payload ESC \

printf '\033]666666;run -c plot -w 60 -h 20 -r;{x=-pi..pi; @axes=on; @grid=on: sin(x)};\033\\'
