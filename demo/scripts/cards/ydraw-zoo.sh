#!/bin/bash
# YDraw Zoo: infinite zoom with random shapes

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# ydraw-zoo doesn't need a payload - it generates random shapes
printf '\033]666666;run -c ydraw-zoo -x 0 -y 0 -w 80 -h 30 -r;;\033\\'
echo
