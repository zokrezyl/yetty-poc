#!/bin/bash
# Build BusyBox for Android ARM64
set -e

# Go to project root (script is in build-tools/android/)
cd "$(dirname "$0")/../.."

BUSYBOX_VERSION="${BUSYBOX_VERSION:-1.34.1}"
BUSYBOX_DIR="build-android/external/busybox"
BUSYBOX_SRC="${BUSYBOX_DIR}/busybox-${BUSYBOX_VERSION}"
OUTPUT_DIR="build-android/assets"

# Check for NDK compiler
if ! command -v aarch64-linux-android26-clang &> /dev/null; then
    echo "ERROR: aarch64-linux-android26-clang not found in PATH"
    exit 1
fi

# Download if needed
mkdir -p "$BUSYBOX_DIR"
if [ ! -d "$BUSYBOX_SRC" ]; then
    echo "Downloading BusyBox ${BUSYBOX_VERSION}..."
    # Try multiple mirrors (busybox.net SSL cert sometimes expires)
    BUSYBOX_URL=""
    for mirror in \
        "https://www.busybox.net/downloads/busybox-${BUSYBOX_VERSION}.tar.bz2" \
        "https://busybox.net/downloads/busybox-${BUSYBOX_VERSION}.tar.bz2" \
        "https://github.com/nicupavel/busybox/archive/refs/tags/${BUSYBOX_VERSION}.tar.gz"; do
        echo "  Trying: $mirror"
        if curl -fsSL --connect-timeout 10 "$mirror" -o /dev/null 2>/dev/null; then
            BUSYBOX_URL="$mirror"
            break
        fi
    done

    if [ -z "$BUSYBOX_URL" ]; then
        echo "ERROR: Could not download BusyBox from any mirror"
        echo "Trying with certificate verification disabled..."
        curl -kL "https://busybox.net/downloads/busybox-${BUSYBOX_VERSION}.tar.bz2" | tar -xj -C "$BUSYBOX_DIR"
    elif [[ "$BUSYBOX_URL" == *".tar.gz" ]]; then
        # GitHub mirror has different archive structure
        curl -fsSL "$BUSYBOX_URL" | tar -xz -C "$BUSYBOX_DIR"
        mv "$BUSYBOX_DIR/busybox-${BUSYBOX_VERSION}" "$BUSYBOX_SRC" 2>/dev/null || true
    else
        curl -fsSL "$BUSYBOX_URL" | tar -xj -C "$BUSYBOX_DIR"
    fi
fi

cd "$BUSYBOX_SRC"

# Patch for Android NDK 26+
sed -i 's/#if defined(ANDROID) || defined(__ANDROID__)/#if __ANDROID_API__ < 21/' libbb/missing_syscalls.c
sed -i 's/^# undef HAVE_STRCHRNUL$/# if __ANDROID_API__ < 24\n#  undef HAVE_STRCHRNUL\n# endif/' include/platform.h

# Configure
make defconfig
sed -i 's/^CONFIG_STATIC=.*/CONFIG_STATIC=y/' .config
sed -i 's/^CONFIG_ASH_INTERNAL_GLOB=.*/CONFIG_ASH_INTERNAL_GLOB=y/' .config

for opt in HUSH SHELL_HUSH FEATURE_SH_NOFORK NTPD ADJTIMEX ZCIP FEATURE_UTMP FEATURE_WTMP \
    HOSTID LOGNAME FEATURE_SYNC_FANCY SU LOADFONT SETFONT CONSPY IFCONFIG ARP IFENSLAVE \
    FSCK_MINIX MKFS_MINIX IPCRM IPCS MOUNT UMOUNT SWAPON SWAPOFF ETHER_WAKE \
    SYSLOGD LOGGER LOGREAD KLOGD PIVOT_ROOT NSLOOKUP FEATURE_NSLOOKUP_BIG \
    FEATURE_INETD_RPC FEATURE_HTTPD_SETUID TRACEROUTE TRACEROUTE6 PING PING6 \
    ED VI; do
    sed -i "s/^CONFIG_${opt}=y/# CONFIG_${opt} is not set/" .config
done
yes "" | make oldconfig

# Build
make CC=aarch64-linux-android26-clang AR=llvm-ar STRIP=llvm-strip HOSTCC=gcc -j$(nproc)
llvm-strip busybox

# Copy to output
cd - > /dev/null
mkdir -p "$OUTPUT_DIR"
cp "$BUSYBOX_SRC/busybox" "$OUTPUT_DIR/"
echo "BusyBox built: $OUTPUT_DIR/busybox"
