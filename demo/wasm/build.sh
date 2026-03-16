#!/bin/bash
# Build wasm demo programs
# Requires: clang with wasm target (apt install clang lld)

set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# Check for clang
if ! command -v clang &> /dev/null; then
    echo "Error: clang not found. Install with: apt install clang lld"
    exit 1
fi

# Compile hello.c to wasm
echo "Building hello.wasm..."
clang \
    --target=wasm32 \
    -nostdlib \
    -Wl,--no-entry \
    -Wl,--export=_start \
    -Wl,--allow-undefined \
    -o hello.wasm \
    hello.c

echo "Built: hello.wasm ($(stat -c%s hello.wasm) bytes)"
