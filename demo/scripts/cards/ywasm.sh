#!/bin/bash
# Run a WebAssembly program in yetty
#
# Usage: ./ywasm.sh [wasm-file] [width] [height]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

WASM_FILE="${1:-$PROJECT_ROOT/demo/wasm/hello.wasm}"
WIDTH="${2:-40}"
HEIGHT="${3:-20}"

if [ ! -f "$WASM_FILE" ]; then
    echo "Error: WASM file not found: $WASM_FILE" >&2
    echo "Build it first with: cd demo/wasm && ./build.sh" >&2
    exit 1
fi

PAYLOAD=$(base64 -w0 < "$WASM_FILE")
printf '\033]666666;run -c ywasm -x 0 -y 0 -w %d -h %d -r;;%s\033\\' "$WIDTH" "$HEIGHT" "$PAYLOAD"
echo
