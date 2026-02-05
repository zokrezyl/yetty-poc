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

# CJK Unified Ideographs (first 2000 only for demo - full range is huge)
echo ""
echo "=== CJK Unified Ideographs (U+4E00 - U+9FFF) - showing first 2000 ==="
count=0
for ((cp=0x4E00; cp<=0x4E00+1999 && cp<=0x9FFF; cp++)); do
    printf "\\U$(printf '%08X' $cp)"
    ((count++))
    if ((count % CHARS_PER_LINE == 0)); then
        echo ""
    fi
done
echo ""
echo "($count of 20992 characters shown)"

# Hangul Syllables (first 2000 only - full range is 11172 chars)
echo ""
echo "=== Hangul Syllables (U+AC00 - U+D7AF) - showing first 2000 ==="
count=0
for ((cp=0xAC00; cp<=0xAC00+1999 && cp<=0xD7AF; cp++)); do
    printf "\\U$(printf '%08X' $cp)"
    ((count++))
    if ((count % CHARS_PER_LINE == 0)); then
        echo ""
    fi
done
echo ""
echo "($count of 11172 characters shown)"

# Halfwidth and Fullwidth Forms
print_range 0xFF00 0xFFEF "Halfwidth and Fullwidth Forms"

echo ""
echo "Use --all flag to show all CJK Unified Ideographs and Hangul Syllables"

if [[ "$1" == "--all" ]]; then
    echo ""
    echo "=== CJK Unified Ideographs (U+4E00 - U+9FFF) - ALL ==="
    count=0
    for ((cp=0x4E00; cp<=0x9FFF; cp++)); do
        printf "\\U$(printf '%08X' $cp)"
        ((count++))
        if ((count % CHARS_PER_LINE == 0)); then
            echo ""
        fi
    done
    echo ""
    echo "($count characters)"

    echo ""
    echo "=== Hangul Syllables (U+AC00 - U+D7AF) - ALL ==="
    count=0
    for ((cp=0xAC00; cp<=0xD7AF; cp++)); do
        printf "\\U$(printf '%08X' $cp)"
        ((count++))
        if ((count % CHARS_PER_LINE == 0)); then
            echo ""
        fi
    done
    echo ""
    echo "($count characters)"
fi
