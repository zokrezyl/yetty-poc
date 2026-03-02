#!/bin/bash
# Patch bellard's Alpine vfsync to fix terminal size initialization
# Downloads from vfsync.org and patches the init script
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${1:-build-webasm-dawn-release}"
YETTY_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

if [[ "$BUILD_DIR" != /* ]]; then
    BUILD_DIR="$YETTY_ROOT/$BUILD_DIR"
fi

VFSYNC_URL="https://vfsync.org/u/os/alpine-x86_64"
OUTPUT_DIR="$BUILD_DIR/vfsync/u/os/alpine-x86_64"

echo "=== Patching bellard's Alpine vfsync ==="
echo "Source: $VFSYNC_URL"
echo "Output: $OUTPUT_DIR"

mkdir -p "$OUTPUT_DIR/files"

# Download head file
echo "Downloading head..."
curl -sL "$VFSYNC_URL/head" -o "$OUTPUT_DIR/head"
cat "$OUTPUT_DIR/head"

# Get root directory ID
ROOT_ID=$(grep "RootID:" "$OUTPUT_DIR/head" | awk '{print $2}')
echo "Root ID: $ROOT_ID"

# Download root directory
echo "Downloading root directory..."
curl -sL "$VFSYNC_URL/files/$ROOT_ID" -o "$OUTPUT_DIR/files/$ROOT_ID"

# Function to download a file if not exists
download_file() {
    local fid="$1"
    local path="$OUTPUT_DIR/files/$fid"
    if [ ! -f "$path" ]; then
        curl -sL "$VFSYNC_URL/files/$fid" -o "$path" 2>/dev/null || true
    fi
}

# Parse directory and find sbin
echo "Finding /sbin/init..."

# The vfsync format stores directories as text files with entries
# Format: <mode> <uid> <gid> <size> <mtime> <name> <fileid>
# We need to find sbin directory, then init file

# Download files referenced in root to find sbin
# This is a simplified approach - download what we need

# First, let's just download the init file directly by searching
# The init script has a known content pattern

echo "Downloading filesystem structure..."

# Download first 1000 files to search (bellard's alpine has ~3000 files)
for i in $(seq 1 500); do
    fid=$(printf "%016x" $i)
    download_file "$fid" &
done
wait

echo "Searching for init script..."
INIT_FILE=""
for f in "$OUTPUT_DIR/files/"*; do
    if head -1 "$f" 2>/dev/null | grep -q "^#!/bin/sh$"; then
        if grep -q "exec </dev/hvc0" "$f" 2>/dev/null; then
            if grep -q "stty" "$f" 2>/dev/null; then
                echo "Found init candidate: $f"
                INIT_FILE="$f"
                break
            fi
        fi
    fi
done

if [ -z "$INIT_FILE" ]; then
    echo "ERROR: Could not find init script"
    echo "Trying to download more files..."
    for i in $(seq 501 2000); do
        fid=$(printf "%016x" $i)
        download_file "$fid"
    done

    for f in "$OUTPUT_DIR/files/"*; do
        if head -1 "$f" 2>/dev/null | grep -q "^#!/bin/sh$"; then
            if grep -q "exec </dev/hvc0" "$f" 2>/dev/null; then
                echo "Found init: $f"
                INIT_FILE="$f"
                break
            fi
        fi
    done
fi

if [ -z "$INIT_FILE" ]; then
    echo "ERROR: Still could not find init script"
    exit 1
fi

echo "=== Original init script ==="
cat "$INIT_FILE"

echo ""
echo "=== Patching init script ==="

# Remove hardcoded terminal size from stty command
sed -i 's/stty sane rows [0-9]* cols [0-9]*/stty sane/' "$INIT_FILE"
sed -i 's/stty rows [0-9]* cols [0-9]*/stty sane/' "$INIT_FILE"

# Add debug output after stty
if ! grep -q "Terminal size at init" "$INIT_FILE"; then
    sed -i '/stty sane/a echo "Terminal size at init: $(stty size)"' "$INIT_FILE"
fi

echo "=== Patched init script ==="
cat "$INIT_FILE"

# Create config file pointing to this vfsync
echo ""
echo "=== Creating JSLinux config ==="
mkdir -p "$BUILD_DIR/jslinux"
cat > "$BUILD_DIR/jslinux/alpine-patched.cfg" << 'EOF'
{
    version: 1,
    machine: "pc",
    memory_size: 256,
    kernel: "kernel-x86_64.bin",
    cmdline: "loglevel=3 console=hvc0 root=root rootfstype=9p rootflags=trans=virtio ro TZ=${TZ}",
    fs0: { file: "../vfsync/u/os/alpine-x86_64" },
    eth0: { driver: "user" },
}
EOF

echo ""
echo "=== Done! ==="
echo "Use: ?url=alpine-patched.cfg"
