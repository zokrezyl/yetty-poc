#!/bin/bash
# Post-effect: drip — text bleeds downward through empty space (glyph mode)
# params: speed(0.8), max_drip(10.0), fade(0.8), glyph_mode(1)
printf '\e]666668;17:0.8:10.0:0.8:1.0\a'
