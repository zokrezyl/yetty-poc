#!/bin/bash
# Musical-Score Card: Sheet music display with configurable staves
#
# Usage: ./demo/scripts/musical-score.sh [staves] [width] [height]
#   staves: 1-16 (default: 4)
#   width:  terminal cells (default: 80)
#   height: terminal cells (default: 30)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

STAVES="${1:-4}"
WIDTH="${2:-80}"
HEIGHT="${3:-30}"

printf '\033]666666;run -c musical-score -x 0 -y 0 -w %d -h %d -r;staves=%d;\033\\' "$WIDTH" "$HEIGHT" "$STAVES"
echo
