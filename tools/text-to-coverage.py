#!/usr/bin/env python3
"""Convert text to vector coverage font codepoints (U+F1000 + ASCII)."""
import sys

COVERAGE_BASE = 0xF1000

for line in sys.stdin:
    out = ""
    for c in line:
        if c == '\n':
            out += c
        elif ord(c) < 128:
            out += chr(COVERAGE_BASE + ord(c))
        else:
            out += c  # Pass through non-ASCII
    sys.stdout.write(out)
