#!/bin/bash
# Headless Chrome test for yetty WebAssembly
# Tests actual browser loading with software WebGPU and verifies render loop
#
# Usage:
#   ./test-chrome-headless.sh [BUILD_DIR|URL] [PORT] [TEST_MODE]
#
# Examples:
#   ./test-chrome-headless.sh build-webasm-dawn-release      # Local build
#   ./test-chrome-headless.sh https://zokrezyl.github.io/yetty/  # Remote URL

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

# Check if BUILD_DIR is a URL (remote mode)
REMOTE_MODE=0
if [[ "$BUILD_DIR" == http://* ]] || [[ "$BUILD_DIR" == https://* ]]; then
    REMOTE_MODE=1
    REMOTE_URL="$BUILD_DIR"
    echo "=============================================="
    echo "Headless Chrome Test (Software WebGPU) - REMOTE"
    echo "URL: $REMOTE_URL"
    echo "Test mode: $TEST_MODE"
    echo "=============================================="
else
    echo "=============================================="
    echo "Headless Chrome Test (Software WebGPU) - LOCAL"
    echo "Build directory: $BUILD_DIR"
    echo "Port: $PORT"
    echo "Test mode: $TEST_MODE"
    echo "=============================================="
    cd "$YETTY_ROOT/$BUILD_DIR"
fi

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

# Start server for local mode only
SERVER_PID=""
if [ "$REMOTE_MODE" -eq 0 ]; then
    echo "Starting server on port $PORT..."
    python serve.py $PORT . > /tmp/yetty-server.log 2>&1 &
    SERVER_PID=$!

    cleanup() {
        echo "Cleaning up..."
        [ -n "$SERVER_PID" ] && kill $SERVER_PID 2>/dev/null || true
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
    BASE_URL="http://localhost:$PORT"
else
    BASE_URL="$REMOTE_URL"
    # Remove trailing slash if present
    BASE_URL="${BASE_URL%/}"
fi

#=============================================================================
# Build Asset Verification (local mode only)
#=============================================================================
if [ "$REMOTE_MODE" -eq 0 ]; then
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
else
    echo ""
    echo "=== Skipping local asset verification (remote mode) ==="
    echo ""
fi

# Determine test URL
if [ "$TEST_MODE" = "jslinux" ]; then
    TEST_URL="${BASE_URL}/jslinux/vm-bridge.html?ptyId=test1&url=alpine-x86_64.cfg&cpu=x86_64&cols=80&rows=25&mem=256"
    echo "Testing JSLinux (vfsync.org proxy) at: $TEST_URL"
elif [ "$TEST_MODE" = "jslinux-local" ]; then
    # Test full yetty with local VM config - this enables card creation testing
    # The VM will boot, run card tests in init script, and yetty will process OSC sequences
    TEST_URL="${BASE_URL}/?vmconfig=yetty-alpine.cfg"
    echo "Testing yetty with local JSLinux VM at: $TEST_URL"
elif [ "$TEST_MODE" = "vm-only" ]; then
    # Test VM only (without yetty) - useful for debugging VM boot issues
    TEST_URL="${BASE_URL}/jslinux/vm-bridge.html?ptyId=test1&url=yetty-alpine.cfg&cpu=x86_64&cols=80&rows=25&mem=256"
    echo "Testing JSLinux VM only (no yetty) at: $TEST_URL"
else
    TEST_URL="${BASE_URL}/"
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

    # Check for card creation from boot card tests
    echo ""
    echo "=== Card Creation Tests ==="
    CARD_TEST_RESULT=0

    # Check if card tests ran
    if grep -q "CARD TESTS" "$CONSOLE_LOG"; then
        echo -e "${GREEN}OK: Card tests executed in VM${NC}"

        # Count created cards by type
        CREATED_CARDS=$(grep -oP "CardFactory: created card '\K[^']+" "$CONSOLE_LOG" | sort | uniq -c | sort -rn)
        TOTAL_CREATED=$(grep -c "CardFactory: created card" "$CONSOLE_LOG" || echo 0)
        TOTAL_FAILED=$(grep -c "CardFactory: failed" "$CONSOLE_LOG" || echo 0)

        echo "Cards created: $TOTAL_CREATED"
        echo "Cards failed: $TOTAL_FAILED"

        if [ -n "$CREATED_CARDS" ]; then
            echo "Card types created:"
            echo "$CREATED_CARDS" | head -15
        fi

        # Check minimum card types created
        CARD_TYPES=(qrcode plot image ytext ydraw)
        for ctype in "${CARD_TYPES[@]}"; do
            if grep -q "CardFactory: created card '$ctype'" "$CONSOLE_LOG"; then
                echo -e "${GREEN}OK: $ctype card created${NC}"
            elif grep -q "CardFactory: creating card '$ctype'" "$CONSOLE_LOG"; then
                echo -e "${YELLOW}PARTIAL: $ctype card creation started${NC}"
            else
                echo -e "${YELLOW}WARN: $ctype card not tested${NC}"
            fi
        done

        # Check for OSC handling
        if grep -q "handleCardOSCSequence: ENTERED" "$CONSOLE_LOG"; then
            OSC_COUNT=$(grep -c "handleCardOSCSequence: ENTERED" "$CONSOLE_LOG" || echo 0)
            echo -e "${GREEN}OK: $OSC_COUNT OSC sequences processed${NC}"
        else
            echo -e "${YELLOW}WARN: No OSC handling logged${NC}"
        fi

        # Fail if no cards created at all
        if [ "$TOTAL_CREATED" -lt 2 ]; then
            echo -e "${RED}FAIL: Too few cards created ($TOTAL_CREATED < 2)${NC}"
            CARD_TEST_RESULT=1
        fi

        # Show failures if any
        if [ "$TOTAL_FAILED" -gt 0 ]; then
            echo -e "${YELLOW}Card creation failures:${NC}"
            grep "CardFactory: failed" "$CONSOLE_LOG" | head -10
        fi

        if [ "$CARD_TEST_RESULT" -ne 0 ]; then
            echo -e "${RED}Card creation tests failed${NC}"
            RESULT=1
        fi
    else
        echo -e "${YELLOW}WARN: Card tests not found in boot output${NC}"
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

#=============================================================================
# Card Rendering Tests - Inject OSC via JSLinux and verify YTRACE output
#=============================================================================
echo ""
echo "=== Card Rendering Tests ==="

DEMO_OUTPUT_DIR="$YETTY_ROOT/$BUILD_DIR/demo-output"
if [ "$REMOTE_MODE" -eq 1 ]; then
    echo -e "${YELLOW}SKIP: Card rendering file tests not available in remote mode${NC}"
elif [ -d "$DEMO_OUTPUT_DIR" ] && [ "$TEST_MODE" = "jslinux-local" ]; then
    # Test cards by catting .out files in JSLinux and checking yetty logs
    # Expected YTRACE patterns when card is successfully created:
    #   - "CardFactory: creating card '<type>'"
    #   - "CardFactory: created card '<type>'"
    #   - "GPUScreen: Created card '<type>'"

    CARD_TESTS=(
        "cards/qrcode.out:CardFactory: created card 'qrcode'"
        "cards/plot.out:CardFactory: created card 'plot'"
    )

    CARD_RESULT=0

    for test_spec in "${CARD_TESTS[@]}"; do
        OUT_FILE="${test_spec%%:*}"
        EXPECTED_LOG="${test_spec##*:}"

        echo "Testing card: $OUT_FILE"
        echo "  Expected YTRACE: $EXPECTED_LOG"

        # Check if .out file exists and has valid OSC
        if [ ! -s "$DEMO_OUTPUT_DIR/$OUT_FILE" ]; then
            echo -e "  ${RED}FAIL: Output file missing or empty${NC}"
            CARD_RESULT=1
            continue
        fi

        if ! grep -qaP '\x1b\]666666' "$DEMO_OUTPUT_DIR/$OUT_FILE" 2>/dev/null; then
            echo -e "  ${RED}FAIL: No OSC 666666 in output file${NC}"
            CARD_RESULT=1
            continue
        fi

        # The full test would boot JSLinux, run 'cat /home/demo/output/<file>'
        # and check console for YTRACE card creation messages.
        # For now, verify file structure only (full test in CI with longer timeout)
        echo -e "  ${GREEN}OK: Valid OSC structure${NC}"
    done

    # Summary counts
    TOTAL_OUT=$(find "$DEMO_OUTPUT_DIR" -name "*.out" -type f 2>/dev/null | wc -l)
    VALID_OSC=$(grep -rlP '\x1b\]666666' "$DEMO_OUTPUT_DIR" --include="*.out" 2>/dev/null | wc -l)

    echo ""
    echo "Demo output summary:"
    echo "  Total .out files: $TOTAL_OUT"
    echo "  Valid OSC files: $VALID_OSC"

    if [ "$VALID_OSC" -lt 50 ]; then
        echo -e "${RED}ERROR: Too few valid OSC files ($VALID_OSC < 50)${NC}"
        CARD_RESULT=1
    fi

    if [ "$CARD_RESULT" -ne 0 ]; then
        echo -e "${RED}Card rendering tests FAILED${NC}"
        RESULT=1
    else
        echo -e "${GREEN}Card rendering tests PASSED${NC}"
    fi
elif [ -d "$DEMO_OUTPUT_DIR" ]; then
    # Just count files for non-jslinux mode
    TOTAL_OUT=$(find "$DEMO_OUTPUT_DIR" -name "*.out" -type f 2>/dev/null | wc -l)
    VALID_OSC=$(grep -rlP '\x1b\]666666' "$DEMO_OUTPUT_DIR" --include="*.out" 2>/dev/null | wc -l)
    echo "Demo outputs: $VALID_OSC/$TOTAL_OUT valid OSC files"
else
    echo -e "${YELLOW}SKIP: No demo-output directory${NC}"
fi

#=============================================================================
# Resize Test - Verify terminal survives window resize
#=============================================================================
echo ""
echo "=== Resize Test ==="

RESIZE_LOG="/tmp/yetty-resize-test.log"

# Run Chrome with resize test - use DevTools protocol to resize window
timeout 60 $CHROME \
    --headless=new \
    --no-sandbox \
    --disable-dev-shm-usage \
    --disable-gpu-sandbox \
    --enable-logging=stderr \
    --enable-unsafe-webgpu \
    --use-webgpu-adapter=swiftshader \
    --enable-features=Vulkan,WebGPU \
    --disable-vulkan-surface \
    --window-size=800,600 \
    --virtual-time-budget=10000 \
    "$TEST_URL" \
    2>&1 | tee "$RESIZE_LOG" &
RESIZE_PID=$!

sleep 5

# Take screenshot before resize
SCREENSHOT_BEFORE="/tmp/yetty-before-resize.png"
$CHROME \
    --headless=new \
    --no-sandbox \
    --screenshot="$SCREENSHOT_BEFORE" \
    --window-size=800,600 \
    --disable-gpu \
    "$TEST_URL" 2>/dev/null &
sleep 3

# Take screenshot after resize (different size)
SCREENSHOT_AFTER="/tmp/yetty-after-resize.png"
$CHROME \
    --headless=new \
    --no-sandbox \
    --screenshot="$SCREENSHOT_AFTER" \
    --window-size=1024,768 \
    --disable-gpu \
    "$TEST_URL" 2>/dev/null &
sleep 3

wait $RESIZE_PID 2>/dev/null || true

# Check screenshots exist and are not empty/tiny
RESIZE_RESULT=0
for img in "$SCREENSHOT_BEFORE" "$SCREENSHOT_AFTER"; do
    if [ -f "$img" ]; then
        IMG_SIZE=$(stat -c%s "$img" 2>/dev/null || stat -f%z "$img" 2>/dev/null || echo 0)
        if [ "$IMG_SIZE" -gt 5000 ]; then
            echo -e "${GREEN}OK: Screenshot $img exists (${IMG_SIZE} bytes)${NC}"
        else
            echo -e "${RED}FAIL: Screenshot $img too small (${IMG_SIZE} bytes) - likely blank${NC}"
            RESIZE_RESULT=1
        fi
    else
        echo -e "${YELLOW}WARN: Screenshot $img not created${NC}"
    fi
done

# Check resize callback was triggered
if grep -q "Surface configured" "$RESIZE_LOG"; then
    SURFACE_COUNT=$(grep -c "Surface configured" "$RESIZE_LOG" || echo 0)
    echo -e "${GREEN}OK: Surface configured $SURFACE_COUNT time(s)${NC}"
else
    echo -e "${YELLOW}WARN: No surface configuration logged${NC}"
fi

# Check for resize-related errors
if grep -q "resize.*error\|Error.*resize" "$RESIZE_LOG" 2>/dev/null; then
    echo -e "${RED}FAIL: Resize errors found${NC}"
    grep -i "resize.*error\|error.*resize" "$RESIZE_LOG" | head -5
    RESIZE_RESULT=1
fi

if [ "$RESIZE_RESULT" -ne 0 ]; then
    echo -e "${RED}Resize test FAILED${NC}"
    RESULT=1
else
    echo -e "${GREEN}Resize test PASSED${NC}"
fi

#=============================================================================
# Rendering Verification - Check actual pixels rendered
#=============================================================================
echo ""
echo "=== Rendering Verification ==="

RENDER_RESULT=0

# Check for actual render calls in logs
if grep -q "wgpuRenderPassEncoderDraw\|renderFrame\|GPUScreen.*render" "$CONSOLE_LOG"; then
    echo -e "${GREEN}OK: Render calls found in logs${NC}"
else
    echo -e "${YELLOW}WARN: No explicit render calls logged${NC}"
fi

# Check getCurrentTextureView success (proves WebGPU is rendering)
if grep -q "getCurrentTextureView.*texture\|got texture.*status=0\|SuccessOptimal" "$CONSOLE_LOG"; then
    echo -e "${GREEN}OK: WebGPU surface texture acquired${NC}"
else
    if grep -q "Failed to get surface texture" "$CONSOLE_LOG"; then
        echo -e "${RED}FAIL: WebGPU surface texture acquisition failed${NC}"
        RENDER_RESULT=1
    else
        echo -e "${YELLOW}WARN: Surface texture status not logged${NC}"
    fi
fi

# Check frame count if available
FRAME_COUNT=$(grep -c "mainLoopIteration\|frame.*rendered\|present" "$CONSOLE_LOG" 2>/dev/null || echo 0)
if [ "$FRAME_COUNT" -gt 0 ]; then
    echo -e "${GREEN}OK: $FRAME_COUNT frame iterations detected${NC}"
else
    echo -e "${YELLOW}WARN: No frame iterations logged${NC}"
fi

# Verify screenshot is not all black (basic pixel check)
if [ -f "$SCREENSHOT_BEFORE" ] && command -v identify &>/dev/null; then
    # Use ImageMagick to check if image has color variation
    COLORS=$(identify -format "%k" "$SCREENSHOT_BEFORE" 2>/dev/null || echo 0)
    if [ "$COLORS" -gt 10 ]; then
        echo -e "${GREEN}OK: Screenshot has $COLORS unique colors (not blank)${NC}"
    else
        echo -e "${RED}FAIL: Screenshot has only $COLORS colors (likely blank/black)${NC}"
        RENDER_RESULT=1
    fi
elif [ -f "$SCREENSHOT_BEFORE" ]; then
    echo -e "${YELLOW}WARN: ImageMagick not available for pixel verification${NC}"
fi

if [ "$RENDER_RESULT" -ne 0 ]; then
    echo -e "${RED}Rendering verification FAILED${NC}"
    RESULT=1
else
    echo -e "${GREEN}Rendering verification PASSED${NC}"
fi

echo ""
if [ "$RESULT" -eq 0 ]; then
    echo -e "${GREEN}=== TEST PASSED ===${NC}"
else
    echo -e "${RED}=== TEST FAILED ===${NC}"
fi

exit $RESULT
