# Yetty WebAssembly Build

Yetty can run entirely in the browser using WebAssembly. This enables a fully functional terminal emulator with GPU-accelerated rendering via WebGPU, running a real Linux VM in the browser.

## Architecture

The WebASM build consists of:

1. **Yetty (WebAssembly)** - The terminal emulator compiled to WASM using Emscripten with Dawn WebGPU backend
2. **JSLinux** - A JavaScript/WebAssembly x86 emulator by Fabrice Bellard that runs a real Linux kernel
3. **Alpine Linux** - A lightweight Linux distribution running inside JSLinux with nvim, gcc, and development tools

```
┌─────────────────────────────────────────────────────┐
│                     Browser                          │
├─────────────────────────────────────────────────────┤
│  Yetty (WASM)          │  JSLinux (WASM)            │
│  - WebGPU rendering    │  - x86_64 emulator         │
│  - Terminal emulation  │  - Linux kernel            │
│  - Card system         │  - Alpine rootfs           │
├─────────────────────────────────────────────────────┤
│              postMessage bridge                      │
│         (term-bridge.js / vm-bridge.html)           │
└─────────────────────────────────────────────────────┘
```

## JSLinux

JSLinux is a PC emulator written in JavaScript by Fabrice Bellard. It can run Linux and other operating systems in the browser.

- **Website**: https://bellard.org/jslinux/
- **Features**: x86/x86_64/RISC-V emulation, VirtIO devices, 9p filesystem

Yetty uses JSLinux to run Alpine Linux in an iframe, communicating via `postMessage` for terminal I/O.

## Building

### Prerequisites

- Emscripten SDK (for WebAssembly compilation)
- Docker (for building the Alpine rootfs)
- Python 3 (for the development server)

### Build Commands

```bash
# Full WebASM build
make build-webasm-dawn-release

# This will:
# 1. Compile yetty to WebAssembly with Dawn WebGPU
# 2. Download JSLinux emulator files
# 3. Build the Alpine Linux vfsync filesystem (requires Docker)
# 4. Copy demo files and assets
```

### Running Locally

```bash
cd build-webasm-dawn-release
python3 serve.py 8080 .
# Open http://localhost:8080/ in browser
```

## Alpine Linux Filesystem (vfsync)

The Alpine rootfs is packaged as a **vfsync** filesystem - a content-addressed filesystem format used by JSLinux for efficient lazy loading over HTTP.

### Directory Structure

```
build-webasm-dawn-release/
├── vfsync/u/os/yetty-alpine/    # vfsync filesystem
│   ├── head                      # Metadata (file count, root ID)
│   └── files/                    # Content-addressed file storage
├── jslinux/
│   ├── vm-bridge.html            # JSLinux loader with yetty bridge
│   ├── term-bridge.js            # Terminal I/O bridge
│   ├── x86_64emu-wasm.js         # x86_64 emulator
│   ├── x86_64emu-wasm.wasm
│   ├── kernel-x86_64.bin         # Linux kernel
│   └── yetty-alpine.cfg          # VM configuration
└── yetty.js, yetty.wasm          # Yetty WebAssembly
```

### Building the Filesystem

The filesystem is built using Docker:

```bash
# Build script location
build-tools/jslinux/alpine/build-vfsync.sh [BUILD_DIR]

# What it does:
# 1. Builds a Docker image with Alpine + packages
# 2. Exports the rootfs from the container
# 3. Adds yetty demo files and source tree
# 4. Creates a custom init script
# 5. Packages into vfsync format using build_filelist tool
```

### Adding Packages to Alpine

To add packages to the Alpine filesystem, edit the Dockerfile:

**File**: `build-tools/jslinux/alpine/Dockerfile`

```dockerfile
FROM alpine:3.23
RUN apk add --no-cache \
    bash \
    neovim \
    gcc g++ make cmake \
    git python3 \
    # Add your packages here:
    nodejs \
    npm \
    rust \
    go
```

Then rebuild:

```bash
# Remove existing rootfs to force rebuild
rm -rf build-webasm-dawn-release/_vfsync-build/rootfs

# Rebuild
make build-webasm-dawn-release
```

### Customizing the Init Script

The init script runs when the VM boots. It's defined in:

**File**: `build-tools/jslinux/alpine/build-vfsync.sh` (line ~109)

```bash
cat > "$ROOTFS_DIR/sbin/init" << 'INITEOF'
#!/bin/sh
mount -t proc proc /proc
mount -t sysfs sys /sys
mount -t devtmpfs dev /dev 2>/dev/null || true
exec </dev/hvc0 >/dev/hvc0 2>&1
mount -t tmpfs tmpfs /tmp
mount -t tmpfs tmpfs /var
mount -t tmpfs tmpfs /run
mount -t tmpfs tmpfs /root
mkdir -p /var/log /var/tmp
hostname yetty
export HOME=/root
export TERM=xterm-256color
stty sane 2>/dev/null
cd /root
cat /etc/motd
while true; do
    setsid -c /bin/bash -l </dev/hvc0 >/dev/hvc0 2>&1
done
INITEOF
```

### Quick Patching (Without Docker Rebuild)

For quick changes to existing files in the vfsync, you can patch them directly:

```bash
# Find and patch a file by content
build-tools/jslinux/patch-vfsync-init.sh build-webasm-dawn-release
```

## VM Configuration

The VM configuration is in `jslinux/yetty-alpine.cfg`:

```javascript
{
    version: 1,
    machine: "pc",
    memory_size: 256,        // RAM in MB
    kernel: "kernel-x86_64.bin",
    cmdline: "loglevel=3 console=hvc0 root=root rootfstype=9p rootflags=trans=virtio ro TZ=${TZ}",
    fs0: { file: "../vfsync/u/os/yetty-alpine" },
    eth0: { driver: "user" },
}
```

## Terminal Bridge

Communication between Yetty and JSLinux uses `postMessage`:

### Messages from Yetty to JSLinux:
- `term-input`: Keyboard input from user
- `term-resize`: Terminal size changed

### Messages from JSLinux to Yetty:
- `term-output`: Terminal output from VM
- `term-ready`: VM terminal initialized

### Files:
- `build-tools/jslinux/term-bridge.js` - Replaces JSLinux's term.js
- `build-tools/jslinux/vm-bridge.html` - Modified JSLinux loader

## Troubleshooting

### Terminal size issues
If nvim or other apps show wrong size at startup, ensure the init script does NOT hardcode terminal dimensions:
```bash
# BAD: stty sane rows 24 cols 80
# GOOD: stty sane
```

### Resize crashes
WebGPU scissor rect must be within render target bounds. The fix is in `gpu-screen.cpp` which clamps scissor rect to `GPUContext.renderTargetWidth/Height`.

### Slow boot
The VM needs to download and decompress the kernel and filesystem. First boot is slower; subsequent loads use browser cache.

### Missing packages
Packages must be added to the Dockerfile and the filesystem rebuilt. The VM filesystem is read-only at runtime.

## Testing

```bash
# Run headless Chrome test
build-tools/web/test-chrome-headless.sh build-webasm-dawn-release 8080 term-size 120 40

# Test modes:
# - full: Test full yetty
# - jslinux: Test JSLinux with remote vfsync
# - jslinux-local: Test with local vfsync
# - vm-only: Test VM without yetty
# - term-size: Test terminal size initialization
```
