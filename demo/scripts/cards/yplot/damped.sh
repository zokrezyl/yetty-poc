#!/bin/bash
# Demo: yplot - damped oscillation exp(-x)*sin(x)

printf '\033]666666;run -c yplot -w 80 -h 25 -r;{f=exp(-x*0.3)*sin(x*3); g=exp(-x*0.3); h=-exp(-x*0.3) @range=0..10,-1.2..1.2 @grid=on @axes=on};\033\\'
