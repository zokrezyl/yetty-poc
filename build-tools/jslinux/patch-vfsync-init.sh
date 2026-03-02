#!/bin/bash
# Patch vfsync init script to fix terminal size initialization
# Finds and patches the init script directly in the vfsync files directory
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${1:-build-webasm-dawn-release}"
YETTY_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

if [[ "$BUILD_DIR" != /* ]]; then
    BUILD_DIR="$YETTY_ROOT/$BUILD_DIR"
fi

VFSYNC_DIR="$BUILD_DIR/vfsync/u/os/yetty-alpine/files"

if [ ! -d "$VFSYNC_DIR" ]; then
    echo "ERROR: vfsync directory not found: $VFSYNC_DIR"
    exit 1
fi

echo "=== Patching vfsync init script ==="
echo "Directory: $VFSYNC_DIR"

# Find init script by content pattern:
# - Starts with #!/bin/sh
# - Contains exec </dev/hvc0
# - Contains stty
# - Is small (< 50 lines)
echo "Searching for init script..."

INIT_FILE=""
for f in "$VFSYNC_DIR"/*; do
    # Skip large files
    lines=$(wc -l < "$f" 2>/dev/null || echo 9999)
    if [ "$lines" -gt 50 ]; then
        continue
    fi

    # Check content pattern
    if head -1 "$f" 2>/dev/null | grep -q "^#!/bin/sh$"; then
        if grep -q "exec </dev/hvc0" "$f" 2>/dev/null; then
            if grep -q "setsid.*bash" "$f" 2>/dev/null; then
                echo "Found init script: $f"
                INIT_FILE="$f"
                break
            fi
        fi
    fi
done

if [ -z "$INIT_FILE" ]; then
    echo "ERROR: Could not find init script"
    echo "Looking for any file with 'stty sane rows'..."
    grep -l "stty sane rows" "$VFSYNC_DIR"/* 2>/dev/null | while read f; do
        echo "Candidate: $f"
        head -5 "$f"
        echo "---"
    done
    exit 1
fi

echo ""
echo "=== Current init script ==="
cat "$INIT_FILE"

# Check if already patched
if grep -q "stty sane rows" "$INIT_FILE"; then
    echo ""
    echo "=== Patching... ==="

    # Remove hardcoded terminal size
    sed -i 's/stty sane rows [0-9]* cols [0-9]* 2>\/dev\/null/stty sane 2>\/dev\/null\necho "Terminal size at init: $(stty size)"/' "$INIT_FILE"

    echo ""
    echo "=== Patched init script ==="
    cat "$INIT_FILE"
    echo ""
    echo "SUCCESS: Init script patched!"
else
    echo ""
    echo "Init script already patched or has different format"
    cat "$INIT_FILE"
fi
