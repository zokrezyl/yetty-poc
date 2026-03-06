#!/bin/bash
# Build local files for JSLinux VM import
# Creates jslinux/local-files/ with demo scripts and a manifest
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

# Collect demo scripts (shell scripts and .txt files only)
echo "=== Collecting demo scripts ==="
if [ -d "$YETTY_ROOT/demo" ]; then
    # Only collect demo scripts and relevant files, skip build artifacts
    while IFS= read -r -d '' file; do
        # Skip build directories
        case "$file" in
            */build/*) continue ;;
            */CMakeFiles/*) continue ;;
            *CMakeCache*) continue ;;
        esac
        # Get relative path from demo/
        relpath="${file#$YETTY_ROOT/demo/}"
        # Create target directory
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

# Create manifest JSON with proper formatting
echo ""
echo "=== Creating manifest ==="
{
    echo "{"
    echo "  \"version\": 1,"
    echo "  \"base_url\": \"local-files/\","
    echo "  \"import_dir\": \"/tmp/yetty\","
    echo "  \"files\": ["
    # Add files with proper JSON formatting
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

# Count files
FILE_COUNT=$(grep -c '"' "$OUTPUT_DIR/manifest.json" 2>/dev/null || echo 0)
echo "Created manifest with $((FILE_COUNT/2)) files"

# Create a filelist.txt for CMake dependency tracking
find "$OUTPUT_DIR" -type f -name "*.sh" -o -name "*.py" -o -name "*.txt" > "$OUTPUT_DIR/filelist.txt"

echo ""
echo "=== Done! ==="
echo "Local files: $OUTPUT_DIR"
du -sh "$OUTPUT_DIR"
