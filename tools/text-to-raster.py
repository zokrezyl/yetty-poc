#!/usr/bin/env python3
"""Convert text to raster font codepoints (U+F2000 + ASCII)."""
import sys

RASTER_BASE = 0xF2000

for line in sys.stdin:
    out = ""
    for c in line:
        if c == '\n':
            out += c
        elif ord(c) < 128:
            out += chr(RASTER_BASE + ord(c))
        else:
            out += c  # Pass through non-ASCII
    sys.stdout.write(out)
