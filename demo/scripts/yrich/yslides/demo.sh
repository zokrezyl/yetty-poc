#!/bin/bash
# YSlides Demo - displays a sample presentation
# Usage: ./demo.sh [--view]
#   --view: interactive view mode (press 'q' to quit)
#   default: dump mode (display and exit immediately)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

YSLIDES="$PROJECT_ROOT/build-desktop-ytrace-release/tools/yslides/yslides"
ASSET="$PROJECT_ROOT/demo/assets/yrich/yslides/sample.yslides.yaml"

if [[ ! -f "$YSLIDES" ]]; then
    echo "Error: yslides not found at $YSLIDES"
    echo "Please build the project first: make build-desktop-ytrace-release"
    exit 1
fi

if [[ ! -f "$ASSET" ]]; then
    echo "Error: demo asset not found at $ASSET"
    exit 1
fi

if [[ "$1" == "--view" ]]; then
    exec "$YSLIDES" -f "$ASSET" -h 50
else
    exec "$YSLIDES" -f "$ASSET" --dump -h 50
fi
