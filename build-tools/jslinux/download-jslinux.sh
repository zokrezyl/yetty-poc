#!/bin/bash
# Download JSLinux files for yetty integration
# These files are Copyright (c) Fabrice Bellard, MIT License

set -e

DEST_DIR="${1:-jslinux}"
BASE_URL="https://bellard.org/jslinux"

mkdir -p "$DEST_DIR"

echo "Downloading JSLinux files to $DEST_DIR..."

# Core JS files
curl -s "$BASE_URL/jslinux.js" -o "$DEST_DIR/jslinux.js"
curl -s "$BASE_URL/term.js" -o "$DEST_DIR/term-original.js"

# x86_64 emulator (wasm)
curl -s "$BASE_URL/x86_64emu-wasm.js" -o "$DEST_DIR/x86_64emu-wasm.js"
curl -s "$BASE_URL/x86_64emu-wasm.wasm" -o "$DEST_DIR/x86_64emu-wasm.wasm"

# x86_64 kernel
curl -s "$BASE_URL/kernel-x86_64.bin" -o "$DEST_DIR/kernel-x86_64.bin"

# VM config
curl -s "$BASE_URL/alpine-x86_64.cfg" -o "$DEST_DIR/alpine-x86_64.cfg"

# Also get RISC-V for variety
curl -s "$BASE_URL/riscvemu64-wasm.js" -o "$DEST_DIR/riscvemu64-wasm.js"
curl -s "$BASE_URL/riscvemu64-wasm.wasm" -o "$DEST_DIR/riscvemu64-wasm.wasm"

echo "Files downloaded:"
ls -la "$DEST_DIR"

echo ""
echo "Note: Filesystem images are loaded from vfsync.org at runtime."
echo "The VM config references: https://vfsync.org/u/os/alpine-x86_64"
