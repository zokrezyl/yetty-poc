#!/usr/bin/env python3
"""Convert text to vector font codepoints (U+F0000 + ASCII)."""
import sys

VECTOR_BASE = 0xF0000

for line in sys.stdin:
    out = ""
    for c in line:
        if c == '\n':
            out += c
        elif ord(c) < 128:
            out += chr(VECTOR_BASE + ord(c))
        else:
            out += c  # Pass through non-ASCII
    sys.stdout.write(out)
