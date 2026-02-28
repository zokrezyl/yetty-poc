#!/bin/bash
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

TINYEMU_URL="https://bellard.org/tinyemu/tinyemu-2019-12-21.tar.gz"
DOCKER_IMAGE="yetty-alpine-rootfs"

echo "=============================================="
echo "Building Alpine vfsync filesystem (Docker)"
echo "=============================================="
echo "Build dir: $BUILD_DIR"
echo "Output: $OUTPUT_DIR"
echo ""

mkdir -p "$TOOL_BUILD_DIR"

# Step 1: Build build_filelist tool
echo "=== Step 1: Building build_filelist tool ==="
if [ ! -f "$TOOL_BUILD_DIR/build_filelist" ]; then
    if [ ! -d "$TOOL_BUILD_DIR/tinyemu-2019-12-21" ]; then
        echo "Downloading TinyEMU..."
        curl -sL "$TINYEMU_URL" -o "$TOOL_BUILD_DIR/tinyemu.tar.gz"
        tar xzf "$TOOL_BUILD_DIR/tinyemu.tar.gz" -C "$TOOL_BUILD_DIR"
        rm "$TOOL_BUILD_DIR/tinyemu.tar.gz"
    fi
    echo "Compiling build_filelist..."
    gcc -o "$TOOL_BUILD_DIR/build_filelist" \
        "$TOOL_BUILD_DIR/tinyemu-2019-12-21/build_filelist.c" \
        "$TOOL_BUILD_DIR/tinyemu-2019-12-21/fs_utils.c" \
        "$TOOL_BUILD_DIR/tinyemu-2019-12-21/cutils.c" \
        -I"$TOOL_BUILD_DIR/tinyemu-2019-12-21"
else
    echo "build_filelist already exists"
fi

# Step 2: Build Docker image and export rootfs
echo ""
echo "=== Step 2: Building Alpine rootfs via Docker ==="
if [ -d "$ROOTFS_DIR" ]; then
    chmod -R u+w "$ROOTFS_DIR" 2>/dev/null || true
    rm -rf "$ROOTFS_DIR"
fi
mkdir -p "$ROOTFS_DIR"

docker build -t "$DOCKER_IMAGE" "$SCRIPT_DIR"
CONTAINER_ID=$(docker create "$DOCKER_IMAGE")
docker export "$CONTAINER_ID" | tar x -C "$ROOTFS_DIR"
docker rm "$CONTAINER_ID" > /dev/null

# Step 3: Add yetty content
echo ""
echo "=== Step 3: Adding yetty files ==="

# Demo files
if [ -d "$YETTY_ROOT/demo" ]; then
    mkdir -p "$ROOTFS_DIR/home/demo"
    cp -r "$YETTY_ROOT/demo/"* "$ROOTFS_DIR/home/demo/" 2>/dev/null || true
    echo "Copied demo/ to /home/demo/"
fi

# Source tree
mkdir -p "$ROOTFS_DIR/home/src"
for dir in src include build-tools assets; do
    if [ -d "$YETTY_ROOT/$dir" ]; then
        cp -r "$YETTY_ROOT/$dir" "$ROOTFS_DIR/home/src/$dir"
        echo "Copied $dir/ to /home/src/$dir/"
    fi
done
for f in CMakeLists.txt Makefile flake.nix flake.lock; do
    [ -f "$YETTY_ROOT/$f" ] && cp "$YETTY_ROOT/$f" "$ROOTFS_DIR/home/src/"
done

# Pre-generated demo outputs
if [ -d "$BUILD_DIR/demo-output" ]; then
    cp -r "$BUILD_DIR/demo-output" "$ROOTFS_DIR/home/demo/output"
fi

# Tools
mkdir -p "$ROOTFS_DIR/usr/local/bin"
if [ -d "$YETTY_ROOT/tools" ]; then
    find "$YETTY_ROOT/tools" -maxdepth 1 -name "*.sh" -exec cp {} "$ROOTFS_DIR/usr/local/bin/" \;
fi
if [ -d "$BUILD_DIR/vm-tools" ]; then
    cp "$BUILD_DIR/vm-tools"/* "$ROOTFS_DIR/usr/local/bin/" 2>/dev/null || true
fi

mkdir -p "$ROOTFS_DIR/root"

# Step 4: Init system
echo ""
echo "=== Step 4: Creating init system ==="
rm -f "$ROOTFS_DIR/sbin/init"
cat > "$ROOTFS_DIR/sbin/init" << 'INITEOF'
#!/bin/sh
mount -t proc proc /proc
mount -t sysfs sys /sys
mount -t devtmpfs dev /dev 2>/dev/null || true
mount -t tmpfs tmpfs /tmp
mount -t tmpfs tmpfs /var
mount -t tmpfs tmpfs /run
mount -t tmpfs tmpfs /root
mkdir -p /var/log /var/tmp
hostname yetty
export HOME=/root
cd /root
clear
cat /etc/motd
exec /bin/bash
INITEOF
chmod +x "$ROOTFS_DIR/sbin/init"

cat > "$ROOTFS_DIR/etc/motd" << 'EOF'

Welcome to yetty Alpine Linux!

Source tree: /home/src/
Demo files:  /home/demo/
Tools:       /usr/local/bin/

EOF

# Step 5: Build vfsync filesystem
echo ""
echo "=== Step 5: Building vfsync filesystem ==="
mkdir -p "$OUTPUT_DIR"
rm -rf "$OUTPUT_DIR"/*
"$TOOL_BUILD_DIR/build_filelist" -m 500 "$ROOTFS_DIR" "$OUTPUT_DIR"

# Step 6: Create config
echo ""
echo "=== Step 6: Creating JSLinux config ==="
mkdir -p "$BUILD_DIR/jslinux"
cat > "$BUILD_DIR/jslinux/yetty-alpine.cfg" << 'EOF'
{
    version: 1,
    machine: "pc",
    memory_size: 256,
    kernel: "kernel-x86_64.bin",
    cmdline: "loglevel=3 console=hvc0 root=root rootfstype=9p rootflags=trans=virtio ro TZ=${TZ}",
    fs0: { file: "../vfsync/u/os/yetty-alpine" },
    eth0: { driver: "user" },
}
EOF

echo ""
echo "=== Done! ==="
echo "Filesystem: $OUTPUT_DIR"
du -sh "$OUTPUT_DIR"
