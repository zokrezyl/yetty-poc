#!/bin/bash
# YPaint Scrolling Layer: Performance Test
# Runs diagram.sh 1000 times to stress test scrolling

DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Starting performance test: 1000 iterations of diagram.sh"
echo ""

for i in $(seq 1 1000); do
    echo "iteration $i"
    "$DIR/diagram.sh"
    if (( i % 100 == 0 )); then
        echo "Completed $i iterations"
    fi
done

echo ""
echo "Performance test complete: 1000 iterations"
