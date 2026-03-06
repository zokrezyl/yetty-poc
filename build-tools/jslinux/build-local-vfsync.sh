#!/bin/bash
# Build local files for JSLinux VM import
# Creates jslinux/local-files/ with vm-tools, demo scripts and manifest
# Files are imported via fs_import_file after VM boots
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
YETTY_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
BUILD_DIR_ARG="${1:-build-webasm-dawn-release}"

if [[ "$BUILD_DIR_ARG" != /* ]]; then
    BUILD_DIR="$YETTY_ROOT/$BUILD_DIR_ARG"
else
    BUILD_DIR="$BUILD_DIR_ARG"
fi

OUTPUT_DIR="$BUILD_DIR/jslinux/local-files"

echo "=============================================="
echo "Building local files for JSLinux VM import"
echo "=============================================="
echo "Build dir: $BUILD_DIR"
echo "Output: $OUTPUT_DIR"
echo ""

rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"

# Temporary manifest file
MANIFEST_TMP=$(mktemp)

# Collect vm-tools binaries (ycat, yecho, ybrowser)
echo "=== Collecting vm-tools binaries ==="
if [ -d "$BUILD_DIR/vm-tools" ]; then
    mkdir -p "$OUTPUT_DIR/bin"
    for tool in ycat yecho ybrowser; do
        if [ -f "$BUILD_DIR/vm-tools/$tool" ]; then
            cp "$BUILD_DIR/vm-tools/$tool" "$OUTPUT_DIR/bin/"
            echo "bin/$tool" >> "$MANIFEST_TMP"
            size=$(du -h "$BUILD_DIR/vm-tools/$tool" | cut -f1)
            echo "  bin/$tool ($size)"
        fi
    done
    # magic.mgc for file type detection
    if [ -f "$BUILD_DIR/vm-tools/magic.mgc" ]; then
        mkdir -p "$OUTPUT_DIR/share"
        cp "$BUILD_DIR/vm-tools/magic.mgc" "$OUTPUT_DIR/share/"
        echo "share/magic.mgc" >> "$MANIFEST_TMP"
        echo "  share/magic.mgc"
    fi
else
    echo "WARNING: vm-tools not found at $BUILD_DIR/vm-tools"
    echo "Run 'make build-webasm-dawn-release' first"
fi

# Collect demo scripts (shell scripts and .txt files only)
echo ""
echo "=== Collecting demo scripts ==="
if [ -d "$YETTY_ROOT/demo" ]; then
    while IFS= read -r -d '' file; do
        # Skip build directories
        case "$file" in
            */build/*) continue ;;
            */CMakeFiles/*) continue ;;
            *CMakeCache*) continue ;;
        esac
        relpath="${file#$YETTY_ROOT/demo/}"
        target_dir="$OUTPUT_DIR/demo/$(dirname "$relpath")"
        mkdir -p "$target_dir"
        cp "$file" "$target_dir/"
        echo "demo/$relpath" >> "$MANIFEST_TMP"
        echo "  demo/$relpath"
    done < <(find "$YETTY_ROOT/demo/scripts" "$YETTY_ROOT/demo/files" -type f \( -name "*.sh" -o -name "*.py" -o -name "*.txt" \) -print0 2>/dev/null)
fi

# Collect tools scripts
echo ""
echo "=== Collecting tools scripts ==="
if [ -d "$YETTY_ROOT/tools" ]; then
    mkdir -p "$OUTPUT_DIR/tools"
    while IFS= read -r -d '' file; do
        relpath="${file#$YETTY_ROOT/tools/}"
        cp "$file" "$OUTPUT_DIR/tools/$relpath"
        echo "tools/$relpath" >> "$MANIFEST_TMP"
        echo "  tools/$relpath"
    done < <(find "$YETTY_ROOT/tools" -maxdepth 1 -type f \( -name "*.sh" -o -name "*.py" \) -print0)
fi

# Create manifest JSON
echo ""
echo "=== Creating manifest ==="
{
    echo "{"
    echo "  \"version\": 1,"
    echo "  \"base_url\": \"local-files/\","
    echo "  \"files\": ["
    first=true
    while IFS= read -r file; do
        if [ "$first" = true ]; then
            echo "    \"$file\""
            first=false
        else
            echo "    ,\"$file\""
        fi
    done < "$MANIFEST_TMP"
    echo "  ]"
    echo "}"
} > "$OUTPUT_DIR/manifest.json"

rm -f "$MANIFEST_TMP"

FILE_COUNT=$(grep -c '"' "$OUTPUT_DIR/manifest.json" 2>/dev/null || echo 0)
echo "Created manifest with $((FILE_COUNT/2)) files"

# Create filelist for CMake dependency tracking
find "$OUTPUT_DIR" -type f > "$OUTPUT_DIR/filelist.txt"

echo ""
echo "=== Done! ==="
echo "Local files: $OUTPUT_DIR"
du -sh "$OUTPUT_DIR"
