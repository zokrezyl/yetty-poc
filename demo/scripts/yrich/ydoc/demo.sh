#!/bin/bash
# YDoc Demo - displays a sample document (dump mode)
# Usage: ./demo.sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../../.." && pwd)"

YDOC="$PROJECT_ROOT/build-desktop-ytrace-release/tools/ydoc/ydoc"
ASSET="$PROJECT_ROOT/demo/assets/yrich/ydoc/sample.ydoc.yaml"

if [[ ! -f "$YDOC" ]]; then
    echo "Error: ydoc not found at $YDOC"
    echo "Please build the project first: make build-desktop-ytrace-release"
    exit 1
fi

if [[ ! -f "$ASSET" ]]; then
    echo "Error: demo asset not found at $ASSET"
    exit 1
fi

exec "$YDOC" -f "$ASSET" --dump -h 25
