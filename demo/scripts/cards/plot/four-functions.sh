#!/bin/bash
# Demo: Four mathematical functions in a single plot

# Using new multi-function grammar:
# f1 = sin(x); @f1.color = #00FF00; f2 = cos(x); @f2.color = #FF0000; ...

printf '\033]666666;run -c plot -w 80 -h 20 -r;{x=-pi..pi; @axes=on; @grid=on: f1 = sin(x); @f1.color = #00FF00; f2 = cos(x); @f2.color = #FF0000; f3 = sin(x)*cos(x); @f3.color = #FFFF00; f4 = sin(2*x)*0.5; @f4.color = #00FFFF};\033\\'
