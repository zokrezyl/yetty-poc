#!/usr/bin/env bash
# Print CJK characters, 80 per line

CHARS_PER_LINE=80

print_range() {
    local start=$1
    local end=$2
    local name=$3
    local count=0
    
    echo ""
    echo "=== $name (U+$(printf '%04X' $start) - U+$(printf '%04X' $end)) ==="
    
    for ((cp=start; cp<=end; cp++)); do
        printf "\\U$(printf '%08X' $cp)"
        ((count++))
        if ((count % CHARS_PER_LINE == 0)); then
            echo ""
        fi
    done
    
    if ((count % CHARS_PER_LINE != 0)); then
        echo ""
    fi
    echo "($count characters)"
}

# CJK Symbols and Punctuation
print_range 0x3000 0x303F "CJK Symbols and Punctuation"

# Hiragana
print_range 0x3040 0x309F "Hiragana"

# Katakana
print_range 0x30A0 0x30FF "Katakana"

# Bopomofo
print_range 0x3100 0x312F "Bopomofo"

# Hangul Compatibility Jamo
print_range 0x3130 0x318F "Hangul Compatibility Jamo"

# Katakana Phonetic Extensions
print_range 0x31F0 0x31FF "Katakana Phonetic Extensions"

# Enclosed CJK Letters and Months
print_range 0x3200 0x32FF "Enclosed CJK Letters and Months"

# CJK Compatibility
print_range 0x3300 0x33FF "CJK Compatibility"

# CJK Unified Ideographs - ALL
print_range 0x4E00 0x9FFF "CJK Unified Ideographs"

# Hangul Syllables - ALL
print_range 0xAC00 0xD7AF "Hangul Syllables"

# Halfwidth and Fullwidth Forms
print_range 0xFF00 0xFFEF "Halfwidth and Fullwidth Forms"
