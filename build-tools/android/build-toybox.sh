#!/bin/bash
#
# Build Toybox for Android from source
# License: BSD 0-Clause (public domain equivalent)
#
# Supports: arm64-v8a (default) and x86_64 (for emulator)
# Set ANDROID_ABI=x86_64 for emulator builds
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"

# Configuration
TOYBOX_VERSION="${TOYBOX_VERSION:-0.8.11}"
TOYBOX_REPO="https://github.com/landley/toybox.git"

# Android NDK (use env var or default)
ANDROID_NDK_HOME="${ANDROID_NDK_HOME:-$HOME/android-sdk/ndk/26.1.10909125}"
ANDROID_API="${ANDROID_API:-28}"
ANDROID_ABI="${ANDROID_ABI:-arm64-v8a}"

# Build directories
BUILD_DIR="${ANDROID_BUILD_DIR:-$PROJECT_ROOT/build-android-wgpu}"
TOYBOX_SRC="$BUILD_DIR/toybox-src"
OUTPUT_DIR="$BUILD_DIR/assets"

# Output binary location
TOYBOX_BIN="$OUTPUT_DIR/toybox"

if [ -f "$TOYBOX_BIN" ]; then
    echo "Toybox already built: $TOYBOX_BIN"
    ls -la "$TOYBOX_BIN"
    exit 0
fi

echo "=== Building Toybox $TOYBOX_VERSION for Android $ANDROID_ABI ==="

# Check NDK
if [ ! -d "$ANDROID_NDK_HOME" ]; then
    echo "ERROR: Android NDK not found at $ANDROID_NDK_HOME"
    echo "Set ANDROID_NDK_HOME environment variable"
    exit 1
fi

# Find NDK toolchain
TOOLCHAIN="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64"
if [ ! -d "$TOOLCHAIN" ]; then
    # Try macOS
    TOOLCHAIN="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64"
fi
if [ ! -d "$TOOLCHAIN" ]; then
    echo "ERROR: Could not find NDK toolchain"
    exit 1
fi

# Select compiler based on ABI
case "$ANDROID_ABI" in
    arm64-v8a)
        CC="$TOOLCHAIN/bin/aarch64-linux-android${ANDROID_API}-clang"
        CROSS_PREFIX="aarch64-linux-android-"
        ;;
    x86_64)
        CC="$TOOLCHAIN/bin/x86_64-linux-android${ANDROID_API}-clang"
        CROSS_PREFIX="x86_64-linux-android-"
        ;;
    *)
        echo "ERROR: Unsupported ANDROID_ABI: $ANDROID_ABI"
        echo "Supported: arm64-v8a, x86_64"
        exit 1
        ;;
esac

if [ ! -f "$CC" ]; then
    echo "ERROR: Compiler not found: $CC"
    exit 1
fi

echo "Using compiler: $CC"

# Clone or update toybox
mkdir -p "$BUILD_DIR"
if [ -d "$TOYBOX_SRC" ]; then
    echo "Updating toybox source..."
    cd "$TOYBOX_SRC"
    git fetch origin
    git checkout "$TOYBOX_VERSION" 2>/dev/null || git checkout "origin/master"
else
    echo "Cloning toybox..."
    git clone "$TOYBOX_REPO" "$TOYBOX_SRC"
    cd "$TOYBOX_SRC"
    git checkout "$TOYBOX_VERSION" 2>/dev/null || true
fi

# Clean previous build
make clean 2>/dev/null || true

# Configure for Android - use defconfig then enable more commands
echo "Configuring toybox..."
make defconfig

# Enable all POSIX and useful commands by setting them to y
# Shell
sed -i 's/# CONFIG_SH is not set/CONFIG_SH=y/' .config
sed -i 's/# CONFIG_TELNETD is not set/CONFIG_TELNETD=y/' .config

# File operations
for cmd in CAT CHGRP CHMOD CHOWN CP CUT DD DIFF DIRNAME DU ECHO ENV EXPAND \
    FALSE FILE FIND GREP HEAD LN LS MKDIR MKFIFO MKTEMP MV OD PASTE PATCH \
    PRINTF PWD RM RMDIR SED SLEEP SORT SPLIT STAT TAIL TAR TEE TEST TOUCH \
    TR TRUE TRUNCATE UNIQ WC WHICH XARGS YES READLINK REALPATH BASENAME \
    COMM CMP SEQ SHRED; do
    sed -i "s/# CONFIG_${cmd} is not set/CONFIG_${cmd}=y/" .config
done

# Process management
for cmd in KILL KILLALL PS TOP NICE NOHUP TIMEOUT WATCH PGREP PKILL PIDOF; do
    sed -i "s/# CONFIG_${cmd} is not set/CONFIG_${cmd}=y/" .config
done

# System info
for cmd in DATE DF DMESG FREE HOSTNAME ID MOUNT UMOUNT UNAME UPTIME WHO WHOAMI \
    GROUPS PRINTENV TTY STTY CAL CLEAR; do
    sed -i "s/# CONFIG_${cmd} is not set/CONFIG_${cmd}=y/" .config
done

# Text processing
for cmd in AWK VI LESS MORE FOLD FMT STRINGS HEXDUMP XXD BASE64 MD5SUM SHA1SUM \
    SHA256SUM SHA512SUM; do
    sed -i "s/# CONFIG_${cmd} is not set/CONFIG_${cmd}=y/" .config
done

# Network
for cmd in NETCAT NETSTAT PING WGET IFCONFIG ARP ROUTE TRACEROUTE HOST; do
    sed -i "s/# CONFIG_${cmd} is not set/CONFIG_${cmd}=y/" .config
done

# Disable features that require crypt() or libraries not in Android NDK
sed -i 's/CONFIG_SU=y/CONFIG_SU=n/' .config
sed -i 's/CONFIG_SULOGIN=y/CONFIG_SULOGIN=n/' .config
sed -i 's/CONFIG_LOGIN=y/CONFIG_LOGIN=n/' .config
sed -i 's/CONFIG_PASSWD=y/CONFIG_PASSWD=n/' .config
sed -i 's/CONFIG_USERADD=y/CONFIG_USERADD=n/' .config
sed -i 's/CONFIG_GROUPADD=y/CONFIG_GROUPADD=n/' .config
sed -i 's/CONFIG_MKPASSWD=y/CONFIG_MKPASSWD=n/' .config
sed -i 's/CONFIG_VLOCK=y/CONFIG_VLOCK=n/' .config
sed -i 's/CONFIG_TOYBOX_LIBCRYPTO=y/CONFIG_TOYBOX_LIBCRYPTO=n/' .config
sed -i 's/CONFIG_TOYBOX_LIBZ=y/CONFIG_TOYBOX_LIBZ=n/' .config

# Regenerate config headers
yes "" | make oldconfig

# Build with Android NDK
# Note: Don't use CROSS_COMPILE with full CC path - toybox concatenates them
echo "Building toybox for $ANDROID_ABI..."
LDFLAGS="--static" \
CFLAGS="-Os -fPIE" \
CC="$CC" \
HOSTCC="cc" \
make toybox -j$(nproc)

# Verify build
if [ ! -f "toybox" ]; then
    echo "ERROR: Build failed - toybox binary not created"
    exit 1
fi

# Strip and copy to output
mkdir -p "$OUTPUT_DIR"
"$TOOLCHAIN/bin/llvm-strip" toybox -o "$TOYBOX_BIN"
chmod +x "$TOYBOX_BIN"

# Also copy to jniLibs as .so for execution (Android SELinux allows executing from lib/)
JNILIBS_DIR="$BUILD_DIR/jniLibs/$ANDROID_ABI"
mkdir -p "$JNILIBS_DIR"
cp "$TOYBOX_BIN" "$JNILIBS_DIR/libtoybox.so"
chmod +x "$JNILIBS_DIR/libtoybox.so"
echo "Copied toybox to jniLibs: $JNILIBS_DIR/libtoybox.so"

# Create symlinks for ALL toybox applets in jniLibs
cd "$JNILIBS_DIR"

# Get list of all enabled commands from the generated header
TOYBOX_COMMANDS=$(grep 'USE_.*NEWTOY' "$TOYBOX_SRC/generated/newtoys.h" 2>/dev/null | sed 's/.*NEWTOY(\([^,]*\),.*/\1/' | sort -u)

# If that fails, get from the built binary's help output pattern
if [ -z "$TOYBOX_COMMANDS" ]; then
    # Extract command names from Config.in
    TOYBOX_COMMANDS=$(grep -E "^config [A-Z]" "$TOYBOX_SRC/generated/Config.in" 2>/dev/null | awk '{print tolower($2)}' | sort -u)
fi

# Create symlinks for each command (skip "toybox" itself)
for cmd in $TOYBOX_COMMANDS; do
    [ -n "$cmd" ] && [ "$cmd" != "toybox" ] && ln -sf libtoybox.so "lib${cmd}.so" 2>/dev/null || true
done
echo "Created symlinks for $(echo $TOYBOX_COMMANDS | wc -w) toybox commands -> libtoybox.so"

# Build shell wrapper (liblogin.so) that handles -h flag from telnetd
# telnetd calls: login_program -h hostname
# toybox sh doesn't support -h, so we need a wrapper
echo "Building shell wrapper..."
WRAPPER_SRC="$BUILD_DIR/login_wrapper.c"
cat > "$WRAPPER_SRC" << 'WRAPPER_EOF'
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

// Shell wrapper for telnetd login
// Ignores -h hostname flag and execs toybox sh
int main(int argc, char *argv[]) {
    // Find libtoybox.so in same directory as this binary
    char self[4096];
    ssize_t len = readlink("/proc/self/exe", self, sizeof(self) - 1);
    if (len <= 0) return 1;
    self[len] = '\0';

    // Replace binary name with libtoybox.so
    char *dir = dirname(self);
    char toybox_path[4096];
    snprintf(toybox_path, sizeof(toybox_path), "%s/libtoybox.so", dir);

    // Exec toybox as "sh" (argv[0] determines applet)
    char *sh_argv[] = { "sh", NULL };
    execv(toybox_path, sh_argv);

    // Fallback to system shell
    execv("/system/bin/sh", sh_argv);
    return 1;
}
WRAPPER_EOF

"$CC" -Os -fPIE -pie -o "$JNILIBS_DIR/liblogin.so" "$WRAPPER_SRC"
"$TOOLCHAIN/bin/llvm-strip" "$JNILIBS_DIR/liblogin.so"
chmod +x "$JNILIBS_DIR/liblogin.so"
echo "Built shell wrapper: $JNILIBS_DIR/liblogin.so"

echo ""
echo "=== Toybox build complete ==="
ls -la "$TOYBOX_BIN"
file "$TOYBOX_BIN"
echo ""
echo "Commands available:"
"$TOYBOX_BIN" 2>&1 | head -5 || true
