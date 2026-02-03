#!/bin/bash
# Post-effect: CRT monitor
# params: vignette(0.4), scanline(0.15), color_bleed(0.3)
printf '\e]666668;2:0.4:0.15:0.3\a'
