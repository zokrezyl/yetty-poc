#!/bin/bash
#
# Build a minimal vfsync filesystem for GitHub Pages
# Uses cached Alpine minirootfs for fast rebuilds
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
YETTY_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
BUILD_DIR_ARG="${1:-build-webasm-dawn-release}"
if [[ "$BUILD_DIR_ARG" != /* ]]; then
    BUILD_DIR="$YETTY_ROOT/$BUILD_DIR_ARG"
else
    BUILD_DIR="$BUILD_DIR_ARG"
fi
OUTPUT_DIR="$BUILD_DIR/vfsync/u/os/yetty-alpine"
TOOL_BUILD_DIR="$BUILD_DIR/_vfsync-build"
ROOTFS_DIR="$TOOL_BUILD_DIR/rootfs"

# Cache directory - persistent across builds
CACHE_DIR="${YETTY_VFSYNC_CACHE:-$HOME/.cache/yetty}"
ROOTFS_CACHE="$CACHE_DIR/alpine-minirootfs-base"
CACHE_VERSION_FILE="$ROOTFS_CACHE/.cache-version"
# Bump this when Alpine version changes
CACHE_VERSION="3.21.3"

TINYEMU_URL="https://bellard.org/tinyemu/tinyemu-2019-12-21.tar.gz"
ALPINE_URL="https://dl-cdn.alpinelinux.org/alpine/v3.21/releases/x86_64/alpine-minirootfs-3.21.3-x86_64.tar.gz"

echo "=============================================="
echo "Building vfsync filesystem for GitHub Pages"
echo "=============================================="
echo "Build dir: $BUILD_DIR"
echo "Output: $OUTPUT_DIR"
echo "Cache: $ROOTFS_CACHE"
echo ""

mkdir -p "$TOOL_BUILD_DIR"
cd "$TOOL_BUILD_DIR"

# Step 1: Build build_filelist tool
echo "=== Step 1: Building build_filelist tool ==="
if [ ! -f "$TOOL_BUILD_DIR/build_filelist" ]; then
    if [ ! -d "$TOOL_BUILD_DIR/tinyemu-2019-12-21" ]; then
        echo "Downloading TinyEMU..."
        curl -sL "$TINYEMU_URL" -o tinyemu.tar.gz
        tar xzf tinyemu.tar.gz
        rm tinyemu.tar.gz
    fi
    echo "Compiling build_filelist..."
    cd "$TOOL_BUILD_DIR/tinyemu-2019-12-21"
    gcc -o "$TOOL_BUILD_DIR/build_filelist" build_filelist.c fs_utils.c cutils.c -I.
    echo "Done: $TOOL_BUILD_DIR/build_filelist"
else
    echo "build_filelist already exists"
fi
cd "$TOOL_BUILD_DIR"

# Step 2: Get Alpine minirootfs (cached or download)
echo ""
echo "=== Step 2: Getting Alpine minirootfs ==="

# Check if cache is valid
cache_valid=false
if [ -d "$ROOTFS_CACHE" ] && [ -f "$CACHE_VERSION_FILE" ]; then
    cached_version=$(cat "$CACHE_VERSION_FILE" 2>/dev/null || echo "0")
    if [ "$cached_version" = "$CACHE_VERSION" ]; then
        cache_valid=true
    else
        echo "Cache version mismatch ($cached_version != $CACHE_VERSION), re-downloading..."
    fi
fi

# Prepare working rootfs directory
if [ -d "$ROOTFS_DIR" ]; then
    rm -rf "$ROOTFS_DIR"
fi
mkdir -p "$ROOTFS_DIR"

if [ "$cache_valid" = true ]; then
    echo "Using cached minirootfs from $ROOTFS_CACHE"
    echo "Copying to working directory..."
    cp -a "$ROOTFS_CACHE/." "$ROOTFS_DIR/"
    echo "Done (cached)"
else
    echo "Downloading Alpine minirootfs..."
    curl -sL "$ALPINE_URL" -o alpine.tar.gz
    echo "Extracting..."
    cd "$ROOTFS_DIR"
    tar xzf "$TOOL_BUILD_DIR/alpine.tar.gz"
    rm "$TOOL_BUILD_DIR/alpine.tar.gz"
    cd "$TOOL_BUILD_DIR"

    # Save to cache for next time (only locally, CI can skip)
    if [ -z "$CI" ]; then
        echo "Saving minirootfs to cache..."
        mkdir -p "$CACHE_DIR"
        if [ -d "$ROOTFS_CACHE" ]; then
            rm -rf "$ROOTFS_CACHE"
        fi
        cp -a "$ROOTFS_DIR" "$ROOTFS_CACHE"
        echo "$CACHE_VERSION" > "$CACHE_VERSION_FILE"
        echo "Cached at $ROOTFS_CACHE"
    fi
fi

# Step 3: Add demo directory
echo ""
echo "=== Step 3: Adding demo files ==="
if [ -d "$YETTY_ROOT/demo" ]; then
    mkdir -p "$ROOTFS_DIR/home/demo"
    cp -r "$YETTY_ROOT/demo/"* "$ROOTFS_DIR/home/demo/" 2>/dev/null || true
    echo "Copied demo/ to /home/demo/"
else
    echo "Warning: demo/ directory not found"
fi

# Step 3b: Add source tree
echo ""
echo "=== Step 3b: Adding source tree ==="
mkdir -p "$ROOTFS_DIR/home/src"
for dir in src include build-tools assets; do
    if [ -d "$YETTY_ROOT/$dir" ]; then
        cp -r "$YETTY_ROOT/$dir" "$ROOTFS_DIR/home/src/$dir"
        echo "Copied $dir/ to /home/src/$dir/"
    fi
done
# Copy top-level build files
for f in CMakeLists.txt Makefile flake.nix flake.lock; do
    if [ -f "$YETTY_ROOT/$f" ]; then
        cp "$YETTY_ROOT/$f" "$ROOTFS_DIR/home/src/"
    fi
done
echo "Source tree available at /home/src/"

# Step 3c: Add pre-generated demo outputs
if [ -d "$BUILD_DIR/demo-output" ]; then
    cp -r "$BUILD_DIR/demo-output" "$ROOTFS_DIR/home/demo/output"
    echo "Copied demo-output/ to /home/demo/output/"
else
    echo "Warning: demo-output/ not found (run cmake build first)"
fi

# Step 4: Add tools executables
echo ""
echo "=== Step 4: Adding tools executables ==="
mkdir -p "$ROOTFS_DIR/usr/local/bin"

# Copy shell scripts from tools/
if [ -d "$YETTY_ROOT/tools" ]; then
    find "$YETTY_ROOT/tools" -maxdepth 1 -name "*.sh" -exec cp {} "$ROOTFS_DIR/usr/local/bin/" \;
    echo "Copied shell scripts from tools/"
fi

# Copy static VM tools (built by cmake)
if [ -d "$BUILD_DIR/vm-tools" ]; then
    cp -v "$BUILD_DIR/vm-tools"/* "$ROOTFS_DIR/usr/local/bin/" 2>/dev/null || true
    echo "Copied VM tools from $BUILD_DIR/vm-tools/"
fi

# Step 5: Create simple init system
echo ""
echo "=== Step 5: Creating simple init system ==="
rm -f "$ROOTFS_DIR/sbin/init"
cat > "$ROOTFS_DIR/sbin/init" << 'INITEOF'
#!/bin/sh
mount -t proc proc /proc
mount -t sysfs sys /sys
mount -t devtmpfs dev /dev 2>/dev/null || true
hostname yetty
clear
cat /etc/motd
exec /bin/sh
INITEOF
chmod +x "$ROOTFS_DIR/sbin/init"

cat > "$ROOTFS_DIR/etc/motd" << 'EOF'

Welcome to yetty Alpine Linux!

Source tree: /home/src/
Demo files:  /home/demo/
Tools:       /usr/local/bin/

EOF

# Step 6: Build vfsync filesystem
echo ""
echo "=== Step 6: Building vfsync filesystem ==="
mkdir -p "$OUTPUT_DIR"
rm -rf "$OUTPUT_DIR"/*
"$TOOL_BUILD_DIR/build_filelist" -m 500 "$ROOTFS_DIR" "$OUTPUT_DIR"

# Step 7: Create config file
echo ""
echo "=== Step 7: Creating JSLinux config ==="
mkdir -p "$BUILD_DIR/jslinux"
cat > "$BUILD_DIR/jslinux/yetty-alpine.cfg" << 'EOF'
/* Yetty Alpine - minimal filesystem for GitHub Pages */
{
    version: 1,
    machine: "pc",
    memory_size: 256,
    kernel: "kernel-x86_64.bin",
    cmdline: "loglevel=3 console=hvc0 root=root rootfstype=9p rootflags=trans=virtio ro TZ=${TZ}",
    fs0: { file: "vfsync/u/os/yetty-alpine" },
    eth0: { driver: "user" },
}
EOF

echo ""
echo "=== Done! ==="
echo "Filesystem: $OUTPUT_DIR"
ls -la "$OUTPUT_DIR/"
cat "$OUTPUT_DIR/head"
du -sh "$OUTPUT_DIR"
