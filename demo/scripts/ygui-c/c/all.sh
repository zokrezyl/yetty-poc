#!/bin/bash
# Run all YGui-C demos sequentially
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "=== YGui-C Demos ==="
echo ""

for script in "$SCRIPT_DIR"/[0-9]*.sh; do
    [ -f "$script" ] || continue
    name=$(basename "$script" .sh)
    echo ">>> Running: $name"
    "$script"
    echo ""
    echo "Press Enter for next demo (or Ctrl+C to quit)..."
    read
done

echo "=== All demos complete ==="
