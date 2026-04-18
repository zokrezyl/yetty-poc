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

## Fetching External Assets from Inside the VM

Apps running inside the VM may need to fetch files from external URLs (e.g., GitHub Pages). The challenge is that the JSLinux WASM build (`jsemu.c:207`) hardcodes a limit of ONE 9p filesystem, so you cannot simply add `fs1` for a second mount.

### Possible Approach: .fscmd xhr command (UNTESTED)

TinyEMU source code (`fs_net.c:2470-2584`) contains an undocumented `xhr` command interface via the `/.fscmd` special file. If present in the x86 WASM build, apps could potentially fetch URLs:

```bash
# Theoretical usage (needs verification)
echo 'xhr "https://user.github.io/repo/file.bin" "" "" "" "/tmp/file.bin" "" 0' > /.fscmd
```

**Status:** This is undocumented and the x86 WASM emulator is closed-source. Test if `/.fscmd` exists in the running VM before relying on this.

### Alternative: fs_import_file from JavaScript

The confirmed working approach is to use `fs_import_file` from JavaScript to push files into the VM:

```javascript
// From browser JS, fetch and inject into VM
fetch('https://user.github.io/repo/file.bin')
  .then(r => r.arrayBuffer())
  .then(data => {
    const bytes = new Uint8Array(data);
    const ptr = Module._malloc(bytes.length);
    Module.HEAPU8.set(bytes, ptr);
    fs_import_file('/tmp/file.bin', ptr, bytes.length);
    Module._free(ptr);
  });
```

This is one-way (JS to VM) and requires the browser JS to initiate the fetch.

### Alternative: XMLHttpRequest Interception (Same-Origin GitHub Pages)

Since TinyEMU's vfsync uses `XMLHttpRequest` internally (via `emscripten_async_wget3_data`), you can intercept and redirect requests at the JS level. This works for same-origin requests (e.g., yetty on `zokrezyl.github.io` fetching assets from `zokrezyl.github.io`).

**Why same-origin matters:** Cross-origin requests (e.g., to `raw.githubusercontent.com`) are blocked by CORS. GitHub Pages doesn't send `Access-Control-Allow-Origin` headers for arbitrary requests.

#### vfsync Format Background

The vfsync filelist format (parsed in `fs_net.c:filelist_load_rec`) contains file metadata:
```
# Regular file: <octal_mode> <uid> <gid> <size> <mtime> <filename> <file_id>
100644 0 0 12345 1700000000.0 my-asset.bin 00000000000040c5

# Directory: <mode> <uid> <gid> <mtime> <dirname> followed by contents, ending with .
040755 0 0 1700000000.0 subdir
100644 0 0 100 1700000000.0 file.txt 00000000000040c6
.
```

File content is fetched lazily via: `<base_url>/files/<file_id>`

#### Approach: Inject Virtual File Entries

1. **Add entries to the filelist** with reserved high file_ids (e.g., `ffff...`) to avoid collision with real content-addressed hashes:
   ```
   100644 0 0 12345 1700000000.0 my-asset.bin ffff000000000001
   100644 0 0 67890 1700000000.0 another.dat  ffff000000000002
   ```

2. **Intercept XMLHttpRequest** in `vm-bridge.html`:
   ```javascript
   const originalXHROpen = XMLHttpRequest.prototype.open;
   XMLHttpRequest.prototype.open = function(method, url, ...args) {
       const match = url.match(/\/files\/([0-9a-f]{16})/);
       if (match && match[1].startsWith('ffff')) {
           // Map reserved file_id to same-origin asset path
           const assetPath = fileIdToAssetPath(match[1]);
           url = '/assets/' + assetPath;  // Same origin, no CORS issues
       }
       return originalXHROpen.call(this, method, url, ...args);
   };

   function fileIdToAssetPath(fileId) {
       // Lookup table populated from manifest
       return assetManifest[fileId]?.path || 'unknown';
   }
   ```

3. **VM app accesses the file**: `cat /my-asset.bin`
   - Kernel requests file via VirtIO-9p
   - Emulator triggers XMLHttpRequest for `ffff000000000001`
   - JS intercepts → rewrites to `/assets/my-asset.bin` (same origin)
   - Content returned to VM

#### File Size Requirements

The filelist must declare correct file sizes. Incorrect sizes cause problems:
- Declared size < actual → content truncated
- Declared size > actual → app reads garbage/zeros past EOF

**GitHub Pages does NOT support directory listing**, so you cannot dynamically discover files and sizes at runtime.

**Solution: Manifest file** - Generate at deploy time:
```json
// /assets/manifest.json
{
  "files": [
    {"id": "ffff000000000001", "path": "data/foo.bin", "size": 12345},
    {"id": "ffff000000000002", "path": "data/bar.txt", "size": 678}
  ]
}
```

At VM boot, JS fetches the manifest and either:
- Injects entries into the filelist dynamically (intercept the filelist/head request), or
- Uses pre-built filelist with matching sizes

#### Dynamic Filelist Injection

For fully dynamic file discovery, intercept the filelist request too:

```javascript
const originalSend = XMLHttpRequest.prototype.send;
XMLHttpRequest.prototype.send = function(...args) {
    const xhr = this;
    const originalOnload = xhr.onload;
    xhr.onload = function(e) {
        if (xhr.responseURL.includes('/files/') && isFilelistRequest(xhr)) {
            // Append our virtual entries to the filelist
            let filelist = xhr.responseText;
            for (const file of assetManifest.files) {
                filelist += `100644 0 0 ${file.size} 1700000000.0 ${file.path} ${file.id}\n`;
            }
            // Override response (requires careful handling)
        }
        if (originalOnload) originalOnload.call(this, e);
    };
    return originalSend.apply(this, args);
};
```

This is more complex but allows adding files without rebuilding the vfsync.

### Alternative: Modify Cloudflare Proxy

Extend the existing `cloudflare-vfsync-proxy.js` to route certain paths to GitHub:

```javascript
// In cloudflare-vfsync-proxy.js
if (url.pathname.startsWith('/gh/')) {
    const ghPath = url.pathname.slice(4);
    const ghUrl = 'https://raw.githubusercontent.com/' + ghPath;
    const resp = await fetch(ghUrl);
    return new Response(resp.body, {
        headers: { 'Access-Control-Allow-Origin': '*' }
    });
}
// ... existing vfsync.org proxy code
```

Then from inside the VM, use the proxy URL to fetch GitHub content through the existing vfsync mechanism.

### Limitations

- **Network (eth0):** Requires WebSocket relay (`wss://relay.widgetry.org`), not available for standalone GitHub Pages hosting
- **Multiple 9p filesystems:** Blocked by `assert(p->fs_count == 1)` in `jsemu.c:207`
- **x86 WASM source:** Closed source, cannot verify which features are compiled in

### TinyEMU Source Reference

Key files analyzed from `tinyemu-2019-12-21`:
- `jsemu.c:206-207` - Hardcoded fs_count == 1 assertion
- `fs_net.c:2470-2584` - Undocumented xhr command via /.fscmd
- `fs_net.c:2871-2901` - fs_import_file implementation
- `machine.c:310-333` - Config parser supports fs0, fs1, fs2... (but blocked by jsemu.c)
- `x86_machine.c:2102-2105` - Would support multiple 9p mounts if jsemu.c allowed
