#!/bin/bash
#
# Download pre-built dependencies for Android (Dawn backend):
# - Dawn WebGPU (Google's C++ implementation)
# - toybox (BSD-licensed shell utilities)
#
# Dawn release URL: https://github.com/google/dawn/releases
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
OUTPUT_DIR="$PROJECT_ROOT/build-android/dawn-libs"
INCLUDE_DIR="$PROJECT_ROOT/build-android/dawn-include"
ASSETS_DIR="$PROJECT_ROOT/build-android/assets"

# Versions - must match Dawn.cmake
DAWN_VERSION="20260214.164635"
DAWN_COMMIT="1a3afc99a7ef7dacaab73b71d44575c4f1bf2dd7"

mkdir -p "$OUTPUT_DIR/arm64-v8a"
mkdir -p "$INCLUDE_DIR"
mkdir -p "$ASSETS_DIR"

#-----------------------------------------------------------------------------
# Dawn WebGPU
#-----------------------------------------------------------------------------
if [ -f "$OUTPUT_DIR/arm64-v8a/libwebgpu_dawn.a" ] && [ -f "$INCLUDE_DIR/webgpu/webgpu.h" ]; then
    echo "Dawn already exists: $OUTPUT_DIR/arm64-v8a/libwebgpu_dawn.a"
else
    echo "Downloading pre-built Dawn ${DAWN_VERSION} for Android..."

    # Android build uses different naming: dawn-android-{commit}.tar.gz
    DAWN_URL="https://github.com/google/dawn/releases/download/v${DAWN_VERSION}/dawn-android-${DAWN_COMMIT}.tar.gz"
    HEADERS_URL="https://github.com/google/dawn/releases/download/v${DAWN_VERSION}/dawn-headers-${DAWN_COMMIT}.tar.gz"

    echo "Downloading: $DAWN_URL"
    curl -fsSL -o "/tmp/dawn-android.tar.gz" "$DAWN_URL"

    echo "Downloading headers: $HEADERS_URL"
    curl -fsSL -o "/tmp/dawn-headers.tar.gz" "$HEADERS_URL"

    echo "Extracting library..."
    mkdir -p /tmp/dawn-android
    tar -xzf /tmp/dawn-android.tar.gz -C /tmp/dawn-android

    echo "Extracting headers..."
    mkdir -p /tmp/dawn-headers
    tar -xzf /tmp/dawn-headers.tar.gz -C /tmp/dawn-headers

    # Find and copy the library (Dawn Android provides static .a library)
    DAWN_EXTRACTED="/tmp/dawn-android/dawn-android-${DAWN_COMMIT}"
    if [ -f "$DAWN_EXTRACTED/arm64-v8a/libwebgpu_dawn.a" ]; then
        cp "$DAWN_EXTRACTED/arm64-v8a/libwebgpu_dawn.a" "$OUTPUT_DIR/arm64-v8a/"
        echo "Copied static library: libwebgpu_dawn.a"
    elif [ -f "/tmp/dawn-android/arm64-v8a/libwebgpu_dawn.a" ]; then
        cp "/tmp/dawn-android/arm64-v8a/libwebgpu_dawn.a" "$OUTPUT_DIR/arm64-v8a/"
        echo "Copied static library: libwebgpu_dawn.a"
    else
        echo "Looking for Dawn library..."
        find /tmp/dawn-android -name "*.so" -o -name "*.a" 2>/dev/null
        echo "ERROR: Could not find Dawn library in extracted archive"
        exit 1
    fi

    # Copy headers (structure: dawn-headers/dawn-headers/include/{webgpu,dawn}/)
    # Need both webgpu/ and dawn/ directories since webgpu.h includes dawn/webgpu.h
    if [ -d "/tmp/dawn-headers/dawn-headers/include" ]; then
        cp -r "/tmp/dawn-headers/dawn-headers/include/"* "$INCLUDE_DIR/"
    elif [ -d "/tmp/dawn-headers/dawn-headers-${DAWN_COMMIT}/include" ]; then
        cp -r "/tmp/dawn-headers/dawn-headers-${DAWN_COMMIT}/include/"* "$INCLUDE_DIR/"
    elif [ -d "/tmp/dawn-headers/include" ]; then
        cp -r "/tmp/dawn-headers/include/"* "$INCLUDE_DIR/"
    else
        echo "Looking for headers..."
        find /tmp/dawn-headers -name "webgpu.h" 2>/dev/null
        echo "ERROR: Could not find Dawn headers"
        exit 1
    fi

    rm -rf "/tmp/dawn-android" "/tmp/dawn-android.tar.gz" "/tmp/dawn-headers" "/tmp/dawn-headers.tar.gz"

    echo "Dawn downloaded to: $OUTPUT_DIR/arm64-v8a/"
fi

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

#-----------------------------------------------------------------------------
# Copy assets
#-----------------------------------------------------------------------------
if [ -d "$PROJECT_ROOT/assets" ]; then
    cp -f "$PROJECT_ROOT/assets"/*.ttf "$PROJECT_ROOT/assets"/*.png "$PROJECT_ROOT/assets"/*.json "$ASSETS_DIR/" 2>/dev/null || true
    echo "Assets copied to: $ASSETS_DIR/"
fi

echo ""
echo "Android Dawn dependencies ready:"
echo "  Dawn:   $OUTPUT_DIR/arm64-v8a/"
echo "  toybox: $OUTPUT_DIR/arm64-v8a/libtoybox.so"
echo "  headers: $INCLUDE_DIR/"
echo "  assets:  $ASSETS_DIR/"
