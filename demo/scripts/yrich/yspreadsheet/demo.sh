#!/bin/bash
# YSpreadsheet Demo - displays a sample spreadsheet (dump mode)
# Usage: ./demo.sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

YSPREADSHEET="$PROJECT_ROOT/build-desktop-ytrace-release/tools/yspreadsheet/yspreadsheet"
ASSET="$PROJECT_ROOT/demo/assets/yrich/yspreadsheet/sample.ysheet.yaml"

if [[ ! -f "$YSPREADSHEET" ]]; then
    echo "Error: yspreadsheet not found at $YSPREADSHEET"
    echo "Please build the project first: make build-desktop-ytrace-release"
    exit 1
fi

if [[ ! -f "$ASSET" ]]; then
    echo "Error: demo asset not found at $ASSET"
    exit 1
fi

exec "$YSPREADSHEET" -f "$ASSET" --dump -h 20
