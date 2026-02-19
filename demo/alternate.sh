#!/bin/bash
# Test cards on alternate screen
# Usage: ./alternate.sh <command> [args...]
#
# Switches to alternate screen, runs the command, waits for ESC or Ctrl-C,
# then restores the original screen.

if [ $# -eq 0 ]; then
    echo "Usage: $0 <command> [args...]"
    echo "Example: $0 demo/scripts/cards/yhtml/download.sh https://example.com"
    exit 1
fi

cleanup() {
    # Restore original screen
    printf '\033[?1049l'
    stty sane
    exit 0
}

trap cleanup INT TERM

# Switch to alternate screen
printf '\033[?1049h'
# Clear alternate screen
printf '\033[2J\033[H'

# Run the command
"$@"

# Wait for ESC or Ctrl-C
echo ""
echo "Press ESC or Ctrl-C to exit..."

# Set raw mode to catch single keypresses
stty raw -echo

while true; do
    ch=$(dd bs=1 count=1 2>/dev/null)
    case "$ch" in
        $'\033') cleanup ;;  # ESC
        $'\003') cleanup ;;  # Ctrl-C
    esac
done
