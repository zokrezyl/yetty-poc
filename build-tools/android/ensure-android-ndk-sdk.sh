#!/bin/bash
#
# Ensure Android NDK, SDK, and JDK are available in project-local directory
#
# This script checks for and installs everything needed for Android builds into:
#   PROJECT_ROOT/build-android-ndk-sdk/
#
# This is fully isolated and does NOT interfere with any system installation.
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

# Isolated environment directory (sibling of build-android-ytrace-* directories)
ANDROID_ENV_DIR="$PROJECT_ROOT/build-android-ndk-sdk"
ANDROID_SDK="$ANDROID_ENV_DIR/sdk"
ANDROID_NDK_VERSION="26.1.10909125"
ANDROID_NDK="$ANDROID_SDK/ndk/$ANDROID_NDK_VERSION"

# Local JDK
JDK_VERSION="17.0.13+11"
JDK_DIR="$ANDROID_ENV_DIR/jdk"
LOCAL_JAVA_HOME="$JDK_DIR/jdk-${JDK_VERSION%%+*}"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

info() { echo -e "${BLUE}[INFO]${NC} $*"; }
warn() { echo -e "${YELLOW}[WARN]${NC} $*"; }
error() { echo -e "${RED}[ERROR]${NC} $*"; }
success() { echo -e "${GREEN}[OK]${NC} $*"; }

confirm() {
    local prompt="$1"
    read -p "$prompt [y/N] " -n 1 -r
    echo
    [[ $REPLY =~ ^[Yy]$ ]]
}

#-----------------------------------------------------------------------------
# Check basic system dependencies (wget/curl, unzip)
#-----------------------------------------------------------------------------
check_basic_deps() {
    local missing=()
    command -v wget &>/dev/null || command -v curl &>/dev/null || missing+=("wget")
    command -v unzip &>/dev/null || missing+=("unzip")
    command -v tar &>/dev/null || missing+=("tar")

    if [ ${#missing[@]} -gt 0 ]; then
        warn "Missing required tools: ${missing[*]}"
        echo ""
        warn "Will run: sudo apt-get install -y ${missing[*]}"
        echo ""
        if confirm "Install missing dependencies with sudo?"; then
            sudo apt-get update
            sudo apt-get install -y "${missing[@]}"
            success "Dependencies installed"
        else
            error "Cannot proceed without: ${missing[*]}"
            exit 1
        fi
    fi
}

#-----------------------------------------------------------------------------
# Check/Install JDK locally
#-----------------------------------------------------------------------------
install_jdk() {
    # Check if we already have a local JDK installed
    if [ -f "$LOCAL_JAVA_HOME/bin/java" ]; then
        export JAVA_HOME="$LOCAL_JAVA_HOME"
        export PATH="$JAVA_HOME/bin:$PATH"
        success "Using local JDK: $JAVA_HOME"
        return 0
    fi

    # Check if system Java works
    if command -v java &>/dev/null; then
        local java_path=$(command -v java)
        local java_real=$(readlink -f "$java_path" 2>/dev/null || echo "$java_path")
        local java_dir=$(dirname "$(dirname "$java_real")")

        if [ -f "$java_dir/bin/java" ] && "$java_dir/bin/java" -version &>/dev/null; then
            export JAVA_HOME="$java_dir"
            export PATH="$JAVA_HOME/bin:$PATH"
            success "Using system JDK: $JAVA_HOME"
            return 0
        fi
    fi

    # Need to install local JDK
    info "Java JDK not found or not working."
    echo ""
    echo "Will download Eclipse Temurin JDK 17 and install to:"
    echo "  $JDK_DIR"
    echo ""
    echo "Size: ~180 MB"
    echo ""

    if ! confirm "Install JDK 17 locally?"; then
        error "JDK required for Android build"
        exit 1
    fi

    check_basic_deps

    mkdir -p "$JDK_DIR"

    # Download Eclipse Temurin (Adoptium) JDK 17
    local jdk_url="https://github.com/adoptium/temurin17-binaries/releases/download/jdk-${JDK_VERSION/+/%2B}/OpenJDK17U-jdk_x64_linux_hotspot_${JDK_VERSION/+/_}.tar.gz"
    local tmp_tar="$ANDROID_ENV_DIR/jdk.tar.gz"

    mkdir -p "$ANDROID_ENV_DIR"

    info "Downloading JDK 17..."
    if command -v wget &>/dev/null; then
        wget -q --show-progress -O "$tmp_tar" "$jdk_url"
    else
        curl -L -o "$tmp_tar" "$jdk_url"
    fi

    info "Extracting JDK..."
    tar -xzf "$tmp_tar" -C "$JDK_DIR"
    rm -f "$tmp_tar"

    # Find the extracted directory (handle version variations)
    local extracted_dir=$(find "$JDK_DIR" -maxdepth 1 -type d -name "jdk-*" | head -1)
    if [ -z "$extracted_dir" ]; then
        error "Failed to find extracted JDK directory"
        exit 1
    fi

    export JAVA_HOME="$extracted_dir"
    export PATH="$JAVA_HOME/bin:$PATH"

    success "JDK installed: $JAVA_HOME"
}

#-----------------------------------------------------------------------------
# Install Android SDK command-line tools
#-----------------------------------------------------------------------------
install_cmdline_tools() {
    local cmdline_tools="$ANDROID_SDK/cmdline-tools/latest/bin/sdkmanager"

    if [ -f "$cmdline_tools" ]; then
        success "Android SDK cmdline-tools found"
        return 0
    fi

    info "Android SDK command-line tools not found."
    echo ""
    echo "Will download from Google and install to:"
    echo "  $ANDROID_SDK"
    echo ""
    echo "Size: ~150 MB"
    echo ""

    if ! confirm "Install Android SDK command-line tools?"; then
        error "Android SDK required to build"
        exit 1
    fi

    check_basic_deps

    mkdir -p "$ANDROID_SDK/cmdline-tools"

    local url="https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip"
    local tmp_zip="$ANDROID_ENV_DIR/cmdline-tools.zip"

    mkdir -p "$ANDROID_ENV_DIR"

    info "Downloading Android command-line tools..."
    if command -v wget &>/dev/null; then
        wget -q --show-progress -O "$tmp_zip" "$url"
    else
        curl -L -o "$tmp_zip" "$url"
    fi

    info "Extracting..."
    unzip -q -o "$tmp_zip" -d "$ANDROID_SDK/cmdline-tools"

    if [ -d "$ANDROID_SDK/cmdline-tools/cmdline-tools" ]; then
        rm -rf "$ANDROID_SDK/cmdline-tools/latest"
        mv "$ANDROID_SDK/cmdline-tools/cmdline-tools" "$ANDROID_SDK/cmdline-tools/latest"
    fi

    rm -f "$tmp_zip"

    info "Accepting licenses..."
    yes | "$ANDROID_SDK/cmdline-tools/latest/bin/sdkmanager" --licenses 2>/dev/null || true

    success "Android SDK command-line tools installed"
}

#-----------------------------------------------------------------------------
# Install Android NDK
#-----------------------------------------------------------------------------
install_ndk() {
    if [ -d "$ANDROID_NDK" ]; then
        success "Android NDK found"
        return 0
    fi

    local sdkmanager="$ANDROID_SDK/cmdline-tools/latest/bin/sdkmanager"

    info "Android NDK $ANDROID_NDK_VERSION not found."
    echo ""
    echo "Will download from Google and install to:"
    echo "  $ANDROID_NDK"
    echo ""
    echo "Size: ~1.5 GB"
    echo ""

    if ! confirm "Install Android NDK $ANDROID_NDK_VERSION?"; then
        error "Android NDK required to build"
        exit 1
    fi

    info "Installing NDK $ANDROID_NDK_VERSION (this may take a while)..."
    "$sdkmanager" --install "ndk;$ANDROID_NDK_VERSION"

    success "Android NDK installed"
}

#-----------------------------------------------------------------------------
# Install platform tools (adb, etc.)
#-----------------------------------------------------------------------------
install_platform_tools() {
    if [ -f "$ANDROID_SDK/platform-tools/adb" ]; then
        success "Platform tools found"
        return 0
    fi

    local sdkmanager="$ANDROID_SDK/cmdline-tools/latest/bin/sdkmanager"

    info "Installing platform-tools..."
    "$sdkmanager" --install "platform-tools"

    success "Platform tools installed"
}

#-----------------------------------------------------------------------------
# Write environment file for sourcing by other scripts/Makefile
#-----------------------------------------------------------------------------
write_env_file() {
    local env_file="$ANDROID_ENV_DIR/env.sh"

    cat > "$env_file" <<EOF
# Auto-generated by ensure-android-ndk-sdk.sh
export JAVA_HOME="$JAVA_HOME"
export ANDROID_HOME="$ANDROID_SDK"
export ANDROID_SDK_ROOT="$ANDROID_SDK"
export ANDROID_NDK_HOME="$ANDROID_NDK"
export PATH="\$JAVA_HOME/bin:\$ANDROID_SDK/cmdline-tools/latest/bin:\$ANDROID_SDK/platform-tools:\$PATH"
EOF

    echo ""
    echo "Environment configured:"
    echo "  JAVA_HOME=$JAVA_HOME"
    echo "  ANDROID_HOME=$ANDROID_SDK"
    echo "  ANDROID_SDK_ROOT=$ANDROID_SDK"
    echo "  ANDROID_NDK_HOME=$ANDROID_NDK"
    echo ""
    echo "Environment file written: $env_file"
}

#-----------------------------------------------------------------------------
# Main
#-----------------------------------------------------------------------------
main() {
    echo "========================================"
    echo "  Android Build Environment Check"
    echo "========================================"
    echo ""
    echo "Install directory: $ANDROID_ENV_DIR"
    echo ""

    # Install JDK first (required for sdkmanager)
    install_jdk

    # Now install Android tools
    install_cmdline_tools
    install_ndk
    install_platform_tools

    write_env_file

    echo ""
    success "Android build environment ready!"
}

main "$@"
