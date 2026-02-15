#!/bin/bash
#
# Download pre-built Toybox for Android (BSD-licensed shell utilities)
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"

# Use ANDROID_BUILD_DIR env var or default
BUILD_DIR="${ANDROID_BUILD_DIR:-$PROJECT_ROOT/build-android-wgpu}"
OUTPUT_DIR="$BUILD_DIR/wgpu-libs"

mkdir -p "$OUTPUT_DIR/arm64-v8a"

#-----------------------------------------------------------------------------
# Toybox (BSD-licensed shell utilities)
#-----------------------------------------------------------------------------
if [ -f "$OUTPUT_DIR/arm64-v8a/libtoybox.so" ]; then
    echo "Toybox already exists: $OUTPUT_DIR/arm64-v8a/libtoybox.so"
else
    echo "Downloading Toybox for Android ARM64..."
    curl -fsSL "https://landley.net/toybox/bin/toybox-aarch64" -o "$OUTPUT_DIR/arm64-v8a/libtoybox.so"
    chmod +x "$OUTPUT_DIR/arm64-v8a/libtoybox.so"
    echo "Toybox downloaded: $OUTPUT_DIR/arm64-v8a/libtoybox.so"
fi

echo ""
echo "Toybox ready: $OUTPUT_DIR/arm64-v8a/libtoybox.so"
