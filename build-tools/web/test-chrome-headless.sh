#!/bin/bash
# Headless Chrome test for yetty WebAssembly
# Tests actual browser loading with software WebGPU and verifies render loop

set -e

BUILD_DIR="${1:-build-webasm-dawn-release}"
PORT="${2:-8199}"
TEST_MODE="${3:-full}"  # "full", "jslinux", or "jslinux-local"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
YETTY_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "=============================================="
echo "Headless Chrome Test (Software WebGPU)"
echo "Build directory: $BUILD_DIR"
echo "Port: $PORT"
echo "Test mode: $TEST_MODE"
echo "=============================================="

cd "$YETTY_ROOT/$BUILD_DIR"

# Find Chrome/Chromium
CHROME=""
for cmd in google-chrome chromium chromium-browser google-chrome-stable; do
    if command -v "$cmd" &> /dev/null; then
        CHROME="$cmd"
        break
    fi
done

if [ -z "$CHROME" ]; then
    echo -e "${YELLOW}WARNING: Chrome/Chromium not found, skipping headless test${NC}"
    exit 0
fi

echo "Using browser: $CHROME"

# Start server in background
echo "Starting server on port $PORT..."
python serve.py $PORT . > /tmp/yetty-server.log 2>&1 &
SERVER_PID=$!

cleanup() {
    echo "Cleaning up..."
    kill $SERVER_PID 2>/dev/null || true
}
trap cleanup EXIT

# Wait for server to start
sleep 2

# Check server is running
if ! curl -s -o /dev/null http://localhost:$PORT/; then
    echo -e "${RED}ERROR: Server failed to start${NC}"
    cat /tmp/yetty-server.log
    exit 1
fi
echo "Server started"

#=============================================================================
# Build Asset Verification
#=============================================================================
echo ""
echo "=== Build Asset Verification ==="

BUILD_RESULT=0

# Check vfsync filesystem
echo "Checking vfsync filesystem..."
if [ -d "$YETTY_ROOT/$BUILD_DIR/vfsync/u/os/yetty-alpine" ]; then
    echo -e "${GREEN}OK: vfsync directory exists${NC}"
    VFSYNC_FILES=$(ls "$YETTY_ROOT/$BUILD_DIR/vfsync/u/os/yetty-alpine/files/" 2>/dev/null | wc -l)
    echo "  vfsync files: $VFSYNC_FILES"
    if [ "$VFSYNC_FILES" -lt 100 ]; then
        echo -e "${RED}ERROR: vfsync has too few files ($VFSYNC_FILES < 100)${NC}"
        BUILD_RESULT=1
    fi
else
    echo -e "${RED}ERROR: vfsync directory missing${NC}"
    BUILD_RESULT=1
fi

# Check vfsync head file for demo presence
if [ -f "$YETTY_ROOT/$BUILD_DIR/vfsync/u/os/yetty-alpine/head" ]; then
    FS_COUNT=$(grep "FSFileCount:" "$YETTY_ROOT/$BUILD_DIR/vfsync/u/os/yetty-alpine/head" | awk '{print $2}')
    echo "  FSFileCount: $FS_COUNT"
fi

# Check jslinux files
echo "Checking jslinux files..."
JSLINUX_DIR="$YETTY_ROOT/$BUILD_DIR/jslinux"
JSLINUX_OK=1
for file in vm-bridge.html term-bridge.js x86_64emu-wasm.js x86_64emu-wasm.wasm kernel-x86_64.bin; do
    if [ -f "$JSLINUX_DIR/$file" ]; then
        echo -e "  ${GREEN}OK: jslinux/$file${NC}"
    else
        echo -e "  ${RED}MISSING: jslinux/$file${NC}"
        JSLINUX_OK=0
        BUILD_RESULT=1
    fi
done

# Check shader glyphs
echo "Checking shader glyphs..."
GLYPH_DIR="$YETTY_ROOT/$BUILD_DIR/assets/shaders/glyphs"
if [ -d "$GLYPH_DIR" ]; then
    GLYPH_COUNT=$(ls "$GLYPH_DIR"/*.wgsl 2>/dev/null | wc -l)
    echo -e "${GREEN}OK: $GLYPH_COUNT shader glyph files${NC}"
    if [ "$GLYPH_COUNT" -lt 30 ]; then
        echo -e "${RED}ERROR: Too few shader glyphs ($GLYPH_COUNT < 30)${NC}"
        BUILD_RESULT=1
    fi
else
    echo -e "${RED}ERROR: Shader glyphs directory missing${NC}"
    BUILD_RESULT=1
fi

# Check card shaders
CARD_DIR="$YETTY_ROOT/$BUILD_DIR/assets/shaders/cards"
if [ -d "$CARD_DIR" ]; then
    CARD_COUNT=$(ls "$CARD_DIR"/*.wgsl 2>/dev/null | wc -l)
    echo -e "${GREEN}OK: $CARD_COUNT card shader files${NC}"
else
    echo -e "${RED}ERROR: Card shaders directory missing${NC}"
    BUILD_RESULT=1
fi

# Check yetty.data contains demo
echo "Checking yetty.data..."
if [ -f "$YETTY_ROOT/$BUILD_DIR/yetty.data" ]; then
    DATA_SIZE=$(stat -c%s "$YETTY_ROOT/$BUILD_DIR/yetty.data" 2>/dev/null || stat -f%z "$YETTY_ROOT/$BUILD_DIR/yetty.data")
    DATA_MB=$((DATA_SIZE / 1024 / 1024))
    echo -e "${GREEN}OK: yetty.data exists (${DATA_MB}MB)${NC}"
else
    echo -e "${RED}ERROR: yetty.data missing${NC}"
    BUILD_RESULT=1
fi

if [ "$BUILD_RESULT" -ne 0 ]; then
    echo ""
    echo -e "${RED}=== BUILD ASSET VERIFICATION FAILED ===${NC}"
    exit 1
fi
echo ""

# Determine test URL
if [ "$TEST_MODE" = "jslinux" ]; then
    TEST_URL="http://localhost:$PORT/jslinux/vm-bridge.html?ptyId=test1&url=alpine-x86_64.cfg&cpu=x86_64&cols=80&rows=25&mem=256"
    echo "Testing JSLinux (vfsync.org proxy) at: $TEST_URL"
elif [ "$TEST_MODE" = "jslinux-local" ]; then
    TEST_URL="http://localhost:$PORT/jslinux/vm-bridge.html?ptyId=test1&url=yetty-alpine.cfg&cpu=x86_64&cols=80&rows=25&mem=256"
    echo "Testing JSLinux (local vfsync) at: $TEST_URL"
else
    TEST_URL="http://localhost:$PORT/"
    echo "Testing full yetty at: $TEST_URL"
fi

# Run Chrome with software WebGPU rendering
echo "Running headless Chrome with software WebGPU..."

CONSOLE_LOG="/tmp/yetty-chrome-console.log"

# Chrome flags for software WebGPU:
# --enable-unsafe-webgpu: Enable WebGPU
# --use-webgpu-adapter=swiftshader: Force SwiftShader software adapter
# --enable-features=Vulkan: Enable Vulkan (needed for SwiftShader)
# --disable-vulkan-surface: Don't require actual display surface
# --disable-gpu-sandbox: Allow software rendering
timeout 120 $CHROME \
    --headless=new \
    --no-sandbox \
    --disable-dev-shm-usage \
    --disable-gpu-sandbox \
    --enable-logging=stderr \
    --enable-unsafe-webgpu \
    --use-webgpu-adapter=swiftshader \
    --enable-features=Vulkan,WebGPU \
    --disable-vulkan-surface \
    --v=1 \
    --virtual-time-budget=60000 \
    "$TEST_URL" \
    2>&1 | tee "$CONSOLE_LOG" || true

echo ""
echo "=== Chrome Output Analysis ==="

RESULT=0

# Check for fatal errors first
if grep -q "function signature mismatch" "$CONSOLE_LOG"; then
    echo -e "${RED}ERROR: WASM function signature mismatch${NC}"
    grep -B2 -A2 "function signature mismatch" "$CONSOLE_LOG" || true
    RESULT=1
elif grep -q "RuntimeError" "$CONSOLE_LOG"; then
    echo -e "${RED}ERROR: WASM RuntimeError${NC}"
    grep -B2 -A2 "RuntimeError" "$CONSOLE_LOG" || true
    RESULT=1
elif grep -q "Uncaught" "$CONSOLE_LOG"; then
    echo -e "${RED}ERROR: Uncaught exception${NC}"
    grep "Uncaught" "$CONSOLE_LOG" | head -5 || true
    RESULT=1
elif grep -q "processPackageData" "$CONSOLE_LOG"; then
    echo -e "${RED}ERROR: processPackageData error${NC}"
    grep -A2 "processPackageData" "$CONSOLE_LOG" || true
    RESULT=1
fi

if [ "$TEST_MODE" = "jslinux" ] || [ "$TEST_MODE" = "jslinux-local" ]; then
    # JSLinux-specific checks
    echo ""
    echo "=== JSLinux Output ==="
    grep -E "\[vm-bridge\]|\[term-bridge\]" "$CONSOLE_LOG" | head -50 || echo "(no vm-bridge output)"

    # Check JSLinux checkpoints
    echo ""
    echo "=== JSLinux Checkpoints ==="

    JSLINUX_CHECKPOINTS=(
        "term-bridge.js loaded"
        "Term constructor called"
        "start_vm called"
        "Loading emulator"
        "Module.preRun called"
        "vm_start returned successfully"
    )

    for checkpoint in "${JSLINUX_CHECKPOINTS[@]}"; do
        if grep -q "$checkpoint" "$CONSOLE_LOG"; then
            echo -e "${GREEN}OK: $checkpoint${NC}"
        else
            echo -e "${RED}MISSING: $checkpoint${NC}"
            RESULT=1
        fi
    done

    # Check if kernel was downloaded (shows emulator is running)
    if grep -q "kernel-x86_64.bin" "$CONSOLE_LOG"; then
        echo -e "${GREEN}OK: Kernel download started${NC}"
    else
        echo -e "${YELLOW}WARN: Kernel download not seen${NC}"
    fi

    # Check for successful VM boot
    if grep -q "Welcome to JS/Linux" "$CONSOLE_LOG" || grep -q "Welcome to yetty Alpine" "$CONSOLE_LOG"; then
        echo -e "${GREEN}OK: VM booted - Welcome message received${NC}"
    else
        echo -e "${RED}MISSING: VM boot welcome message${NC}"
        RESULT=1
    fi

    if grep -q "localhost:~#" "$CONSOLE_LOG"; then
        echo -e "${GREEN}OK: Shell prompt received${NC}"
    else
        echo -e "${YELLOW}WARN: Shell prompt not seen (may need more time)${NC}"
    fi

    # Check for demo directory in boot output
    echo ""
    echo "=== Demo Files Check ==="
    if grep -q "/home/demo contents" "$CONSOLE_LOG"; then
        echo -e "${GREEN}OK: Demo directory listing found in boot${NC}"
        grep -A 20 "/home/demo contents" "$CONSOLE_LOG" | head -25 || true
    else
        echo -e "${YELLOW}WARN: Demo directory listing not in boot output${NC}"
    fi

    # Check for shader-glyphs.txt specifically
    if grep -q "shader-glyphs.txt" "$CONSOLE_LOG"; then
        echo -e "${GREEN}OK: shader-glyphs.txt found${NC}"
    else
        echo -e "${YELLOW}WARN: shader-glyphs.txt not seen in output${NC}"
    fi
else
    # Full yetty checks
    if [ "$RESULT" -eq 0 ]; then
        echo "Checking expected ytrace output..."

        # Required checkpoints
        CHECKPOINTS=(
            "Yetty starting"
            "Config::init done"
            "Web platform created"
            "initWebGPU: Creating instance"
            "ShaderFont: loaded"
        )

        # Desired checkpoints (render loop)
        RENDER_CHECKPOINTS=(
            "Starting render loop"
            "Surface configured"
        )

        for checkpoint in "${CHECKPOINTS[@]}"; do
            if grep -q "$checkpoint" "$CONSOLE_LOG"; then
                echo -e "${GREEN}OK: $checkpoint${NC}"
            else
                echo -e "${RED}MISSING: $checkpoint${NC}"
                RESULT=1
            fi
        done

        # Check WebGPU initialization
        if grep -q "WebGPU device ready" "$CONSOLE_LOG" || grep -q "initWebGPU OK" "$CONSOLE_LOG"; then
            echo -e "${GREEN}OK: WebGPU initialized${NC}"

            # If WebGPU works, check render loop
            for checkpoint in "${RENDER_CHECKPOINTS[@]}"; do
                if grep -q "$checkpoint" "$CONSOLE_LOG"; then
                    echo -e "${GREEN}OK: $checkpoint${NC}"
                else
                    echo -e "${YELLOW}MISSING: $checkpoint${NC}"
                fi
            done
        else
            # WebGPU failed - check why
            if grep -q "Failed to get WebGPU adapter" "$CONSOLE_LOG"; then
                echo -e "${YELLOW}WARNING: WebGPU adapter not available (SwiftShader may not be working)${NC}"
                echo -e "${YELLOW}This is acceptable in CI without proper GPU support${NC}"
            else
                echo -e "${RED}ERROR: WebGPU initialization failed for unknown reason${NC}"
                RESULT=1
            fi
        fi
    fi

    # Show shader loading output
    echo ""
    echo "=== Shader Loading ==="
    grep -E "ShaderFont:|ShaderManager:" "$CONSOLE_LOG" | head -30 || echo "(no shader output)"

    # Verify shader glyphs loaded
    if grep -q "ShaderFont: loaded.*glyph shaders" "$CONSOLE_LOG"; then
        GLYPH_COUNT=$(grep "ShaderFont: loaded.*glyph shaders" "$CONSOLE_LOG" | grep -oP '\d+ glyph' | head -1)
        echo -e "${GREEN}OK: Shader glyphs loaded ($GLYPH_COUNT)${NC}"
    else
        echo -e "${RED}MISSING: Shader glyphs not loaded${NC}"
        RESULT=1
    fi

    # Check for shader glyph rendering (when cat shader-glyphs.txt is run)
    if grep -q "SHADER GLYPH:" "$CONSOLE_LOG"; then
        echo -e "${GREEN}OK: Shader glyphs rendered${NC}"
        grep "SHADER GLYPH:" "$CONSOLE_LOG" | head -5
    fi

    # Check for shader glyph enabling
    if grep -q "ShaderFont::getGlyphIndex: enabled" "$CONSOLE_LOG"; then
        echo -e "${GREEN}OK: Shader glyphs enabled for compilation${NC}"
        grep "ShaderFont::getGlyphIndex: enabled" "$CONSOLE_LOG" | head -5
    fi

    # Show yetty-specific log output
    echo ""
    echo "=== Yetty Console Output ==="
    grep -E "\[yetty\]" "$CONSOLE_LOG" | head -40 || echo "(no yetty output)"
fi

# Show any errors
echo ""
echo "=== Errors (if any) ==="
grep -iE "error|failed|exception" "$CONSOLE_LOG" | grep -v "VERBOSE" | head -20 || echo "(no errors)"

echo ""
if [ "$RESULT" -eq 0 ]; then
    echo -e "${GREEN}=== TEST PASSED ===${NC}"
else
    echo -e "${RED}=== TEST FAILED ===${NC}"
fi

exit $RESULT
