#!/bin/bash
#
# Download pre-built wgpu-native for Android
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
OUTPUT_DIR="$PROJECT_ROOT/build-android/wgpu-libs"
INCLUDE_DIR="$PROJECT_ROOT/build-android/wgpu-include"
ASSETS_DIR="$PROJECT_ROOT/build-android/assets"

# Versions
WGPU_VERSION="v27.0.4.0"

mkdir -p "$OUTPUT_DIR/arm64-v8a"
mkdir -p "$INCLUDE_DIR"
mkdir -p "$ASSETS_DIR"

#-----------------------------------------------------------------------------
# wgpu-native
#-----------------------------------------------------------------------------
if [ -f "$OUTPUT_DIR/arm64-v8a/libwgpu_native.so" ] && [ -f "$INCLUDE_DIR/webgpu/webgpu.h" ]; then
    echo "wgpu-native already exists: $OUTPUT_DIR/arm64-v8a/libwgpu_native.so"
else
    echo "Downloading pre-built wgpu-native ${WGPU_VERSION} for Android..."

    WGPU_URL="https://github.com/gfx-rs/wgpu-native/releases/download/${WGPU_VERSION}/wgpu-android-aarch64-release.zip"
    curl -fsSL -o "/tmp/wgpu-android.zip" "$WGPU_URL"

    echo "Extracting..."
    unzip -o "/tmp/wgpu-android.zip" -d "/tmp/wgpu-android"
    cp "/tmp/wgpu-android/lib/libwgpu_native.so" "$OUTPUT_DIR/arm64-v8a/"
    cp -r "/tmp/wgpu-android/include/webgpu" "$INCLUDE_DIR/"

    rm -rf "/tmp/wgpu-android" "/tmp/wgpu-android.zip"

    echo "wgpu-native downloaded: $OUTPUT_DIR/arm64-v8a/libwgpu_native.so"
fi

#-----------------------------------------------------------------------------
# Copy assets
#-----------------------------------------------------------------------------
if [ -d "$PROJECT_ROOT/assets" ]; then
    cp -f "$PROJECT_ROOT/assets"/*.ttf "$PROJECT_ROOT/assets"/*.png "$PROJECT_ROOT/assets"/*.json "$ASSETS_DIR/" 2>/dev/null || true
    echo "Assets copied to: $ASSETS_DIR/"
fi

echo ""
echo "wgpu-native ready:"
echo "  library: $OUTPUT_DIR/arm64-v8a/libwgpu_native.so"
echo "  headers: $INCLUDE_DIR/"
echo "  assets:  $ASSETS_DIR/"
