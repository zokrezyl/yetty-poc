#!/bin/bash
# YGrid Card: Empty grid test (no payload)

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

printf '\033]666666;run -c ygrid -x 0 -y 0 -w 10 -h 5 -r;;\033\\'
echo
