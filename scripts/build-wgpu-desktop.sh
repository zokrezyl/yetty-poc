#!/bin/bash
#
# Download pre-built wgpu-native for Linux desktop
# No build dependencies required - just downloads from GitHub releases
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
OUTPUT_DIR="$PROJECT_ROOT/external/wgpu-native"

# wgpu-native version - same as Android for consistency
WGPU_VERSION="v27.0.4.0"

mkdir -p "$OUTPUT_DIR"

if [ -f "$OUTPUT_DIR/lib/libwgpu_native.so" ] && [ -f "$OUTPUT_DIR/include/webgpu/webgpu.h" ]; then
    echo "wgpu-native already exists: $OUTPUT_DIR"
    exit 0
fi

echo "Downloading pre-built wgpu-native ${WGPU_VERSION} for Linux..."

WGPU_URL="https://github.com/gfx-rs/wgpu-native/releases/download/${WGPU_VERSION}/wgpu-linux-x86_64-release.zip"
curl -L -o "/tmp/wgpu-linux.zip" "$WGPU_URL"

echo "Extracting..."
unzip -o "/tmp/wgpu-linux.zip" -d "/tmp/wgpu-linux"

# Copy library and headers
cp -r "/tmp/wgpu-linux/lib" "$OUTPUT_DIR/"
cp -r "/tmp/wgpu-linux/include" "$OUTPUT_DIR/"

rm -rf "/tmp/wgpu-linux" "/tmp/wgpu-linux.zip"

echo ""
echo "wgpu-native downloaded:"
echo "  Library: $OUTPUT_DIR/lib/libwgpu_native.so"
echo "  Headers: $OUTPUT_DIR/include/"
