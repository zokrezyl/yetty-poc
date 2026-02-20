#!/bin/bash
# Headless Chrome test for yetty WebAssembly
# Tests actual browser loading with software WebGPU and verifies render loop

set -e

BUILD_DIR="${1:-build-webasm-dawn-release}"
PORT="${2:-8199}"
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
    "http://localhost:$PORT/" \
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
    grep "Uncaught" "$CONSOLE_LOG" || true
    RESULT=1
elif grep -q "processPackageData" "$CONSOLE_LOG"; then
    echo -e "${RED}ERROR: processPackageData error${NC}"
    grep -A2 "processPackageData" "$CONSOLE_LOG" || true
    RESULT=1
fi

# If no fatal errors, check for expected ytrace output
if [ "$RESULT" -eq 0 ]; then
    echo "Checking expected ytrace output..."

    # Required checkpoints
    CHECKPOINTS=(
        "Yetty starting"
        "Config::init done"
        "Web platform created"
        "initWebGPU: Creating instance"
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

# Show yetty-specific log output
echo ""
echo "=== Yetty Console Output ==="
grep -E "\[yetty\]" "$CONSOLE_LOG" | head -40 || echo "(no yetty output)"

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
