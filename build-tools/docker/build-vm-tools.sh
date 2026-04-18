#!/bin/bash
#
# Build static VM tools using Alpine Docker
# Creates x86_64 musl-linked static binaries for JSLinux Alpine VM
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
YETTY_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
BUILD_DIR_ARG="${1:-build-webasm-dawn-release}"

if [[ "$BUILD_DIR_ARG" != /* ]]; then
    BUILD_DIR="$YETTY_ROOT/$BUILD_DIR_ARG"
else
    BUILD_DIR="$BUILD_DIR_ARG"
fi

DOCKERFILE="$SCRIPT_DIR/alpine-vm-tools.dockerfile"
IMAGE_NAME="yetty-vm-tools-builder"

echo "=============================================="
echo "Building VM tools with Alpine Docker"
echo "=============================================="
echo "YETTY_ROOT: $YETTY_ROOT"
echo "BUILD_DIR: $BUILD_DIR"
echo ""

# Check Docker is available
if ! command -v docker &> /dev/null; then
    echo "ERROR: Docker not found. Install Docker or use 'make build-vm-tools' with musl-gcc."
    exit 1
fi

# Build Docker image if needed
echo "=== Building Docker image ==="
docker build -t "$IMAGE_NAME" \
    --build-arg UID=$(id -u) \
    --build-arg GID=$(id -g) \
    -f "$DOCKERFILE" \
    "$YETTY_ROOT"

# Create output directory
mkdir -p "$BUILD_DIR/vm-tools"

# Run build in Docker
echo ""
echo "=== Running Alpine build ==="
docker run --rm \
    -v "$YETTY_ROOT:/src:ro" \
    -v "$BUILD_DIR:/build" \
    -u $(id -u):$(id -g) \
    "$IMAGE_NAME"

# Verify output
echo ""
echo "=== Results ==="
FAILED=0
for tool in yecho ycat ybrowser; do
    if [ -f "$BUILD_DIR/vm-tools/$tool" ]; then
        echo "OK: $tool"
        file "$BUILD_DIR/vm-tools/$tool"
    else
        echo "MISSING: $tool"
        FAILED=1
    fi
done
echo ""
ls -la "$BUILD_DIR/vm-tools/"
if [ "$FAILED" -ne 0 ]; then
    echo "ERROR: Some tools failed to build"
    exit 1
fi
