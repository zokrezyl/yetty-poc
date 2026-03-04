#!/bin/bash
#
# Download pre-built wgpu-native for Android
# Supports: arm64-v8a (default) and x86_64 (for emulator)
# Set ANDROID_ABI=x86_64 for emulator builds
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"

# Use ANDROID_BUILD_DIR env var or default
BUILD_DIR="${ANDROID_BUILD_DIR:-$PROJECT_ROOT/build-android-wgpu}"
OUTPUT_DIR="$BUILD_DIR/wgpu-libs"
INCLUDE_DIR="$BUILD_DIR/wgpu-include"
ASSETS_DIR="$BUILD_DIR/assets"

# Architecture (default: arm64-v8a, or x86_64 for emulator)
ANDROID_ABI="${ANDROID_ABI:-arm64-v8a}"

# Map Android ABI to wgpu-native release naming
case "$ANDROID_ABI" in
    arm64-v8a)
        WGPU_ARCH="aarch64"
        ;;
    x86_64)
        WGPU_ARCH="x86_64"
        ;;
    *)
        echo "ERROR: Unsupported ANDROID_ABI: $ANDROID_ABI"
        echo "Supported: arm64-v8a, x86_64"
        exit 1
        ;;
esac

# Versions
WGPU_VERSION="v27.0.4.0"

mkdir -p "$OUTPUT_DIR/$ANDROID_ABI"
mkdir -p "$INCLUDE_DIR"
mkdir -p "$ASSETS_DIR"

#-----------------------------------------------------------------------------
# wgpu-native
#-----------------------------------------------------------------------------
if [ -f "$OUTPUT_DIR/$ANDROID_ABI/libwgpu_native.so" ] && [ -f "$INCLUDE_DIR/webgpu/webgpu.h" ]; then
    echo "wgpu-native already exists: $OUTPUT_DIR/$ANDROID_ABI/libwgpu_native.so"
else
    echo "Downloading pre-built wgpu-native ${WGPU_VERSION} for Android $ANDROID_ABI..."

    WGPU_URL="https://github.com/gfx-rs/wgpu-native/releases/download/${WGPU_VERSION}/wgpu-android-${WGPU_ARCH}-release.zip"
    echo "URL: $WGPU_URL"
    curl -fsSL -o "/tmp/wgpu-android.zip" "$WGPU_URL"

    echo "Extracting..."
    unzip -o "/tmp/wgpu-android.zip" -d "/tmp/wgpu-android"
    cp "/tmp/wgpu-android/lib/libwgpu_native.so" "$OUTPUT_DIR/$ANDROID_ABI/"
    cp -r "/tmp/wgpu-android/include/webgpu" "$INCLUDE_DIR/"

    rm -rf "/tmp/wgpu-android" "/tmp/wgpu-android.zip"

    echo "wgpu-native downloaded: $OUTPUT_DIR/$ANDROID_ABI/libwgpu_native.so"
fi

#-----------------------------------------------------------------------------
# Copy assets
#-----------------------------------------------------------------------------
if [ -d "$PROJECT_ROOT/assets" ]; then
    cp -f "$PROJECT_ROOT/assets"/*.ttf "$PROJECT_ROOT/assets"/*.png "$PROJECT_ROOT/assets"/*.json "$ASSETS_DIR/" 2>/dev/null || true
    echo "Assets copied to: $ASSETS_DIR/"
fi

echo ""
echo "wgpu-native ready ($ANDROID_ABI):"
echo "  library: $OUTPUT_DIR/$ANDROID_ABI/libwgpu_native.so"
echo "  headers: $INCLUDE_DIR/"
echo "  assets:  $ASSETS_DIR/"
