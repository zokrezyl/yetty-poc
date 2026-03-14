#!/bin/bash
# Run all YGui-C Python demos
source "$(dirname "$0")/python-env"

echo "=== YGui-C Python Demos ==="
echo ""

for i in 01 02 03 04 05 06 07 08 09 10; do
    script="$(dirname "$0")/${i}-*.sh"
    script=$(ls $script 2>/dev/null | head -1)
    if [ -n "$script" ] && [ -f "$script" ]; then
        name=$(basename "$script" .sh)
        echo ">>> Running: $name"
        echo "---"
        bash "$script"
        echo ""
        echo "---"
        echo ""
    fi
done

echo "=== All demos complete ==="
