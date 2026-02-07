#!/bin/bash
# Plot Card: Interactive multi-line chart with sine waves, pan/zoom support
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../.."

# Generate 4 sine waves with 200 points each
printf '\033]666666;run -c plot -x 0 -y 0 -w 60 -h 30 -r;generator=sine,lines=4,points=200;\033\\'
echo
