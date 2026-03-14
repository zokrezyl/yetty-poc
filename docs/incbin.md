# Embedded Assets with incbin

All runtime assets are embedded directly into the yetty binary using incbin, making distributed binaries fully portable and self-contained.

## Overview

The yetty binary embeds:
- **113 WGSL shaders** from `src/yetty/shaders/`
- **4 TTF fonts** (DejaVuSansMNerdFontMono variants) - brotli compressed
- **4 MSDF CDB font databases** (pre-rendered glyph atlases) - brotli compressed
- **Logo** (`docs/logo.jpeg`)

Total: **121 embedded assets**, resulting in a **~72MB** self-contained binary.

### Compression

Large assets (fonts, CDBs) are brotli-compressed at build time and decompressed on first run:

| Asset Type | Original | Compressed | Ratio |
|------------|----------|------------|-------|
| 4x CDB | 160MB | ~31MB | 5.2x |
| 4x TTF | 10MB | ~5MB | 1.9x |
| **Total** | **170MB** | **~36MB** | **4.7x** |

This reduces the binary from ~200MB to ~72MB while maintaining full functionality.

## How It Works

### Build Time

1. **CMake configure** detects if MSDF CDB fonts exist in `assets/fonts-cdb/`
2. If missing, CMake automatically:
   - Builds `yetty-msdf-gen` (host tool)
   - Generates CDB files from TTF fonts
3. `incbin_add_directory()` embeds all assets into the binary
4. Asset manifests are generated for runtime registration

### Runtime

1. On first run, `IncbinAssets` extracts all embedded assets to cache directory
2. `YETTY_SHADERS_DIR` environment variable points to extracted shaders
3. Version marker tracks when re-extraction is needed

## Build Commands

Just run:
```bash
make build-desktop-ytrace-release
```

CMake handles everything automatically - no manual asset preparation needed.

## Implementation Files

| File | Purpose |
|------|---------|
| `include/yetty/incbin-assets.h` | IncbinAssets singleton interface |
| `src/yetty/incbin-assets.cpp` | Implementation with manifest-based registration |
| `build-tools/cmake/incbin.cmake` | `incbin_add_resources()` and `incbin_add_directory()` |
| `build-tools/cmake/prepare-assets.cmake` | Auto-generates CDBs at configure time |
| `build-tools/cmake/targets/*.cmake` | Platform-specific asset embedding |
| `src/yetty/yetty.cpp` | `initEmbeddedAssets()` - extraction on first run |

## Asset Embedding (CMake)

```cmake
# Embed shaders
incbin_add_directory(yetty "shaders" "${YETTY_ROOT}/src/yetty/shaders" "*.wgsl")

# Embed fonts
incbin_add_directory(yetty "fonts" "${YETTY_ROOT}/assets" "*.ttf")

# Embed MSDF CDB font databases
incbin_add_directory(yetty "fonts-cdb" "${YETTY_ROOT}/assets/fonts-cdb" "*.cdb")
```

Each call generates:
- `yetty_<prefix>_resources.cpp` - INCBIN macros for each file
- `yetty_<prefix>_manifest.h` - Registration function template
- `HAS_<PREFIX>_MANIFEST` compile definition

## Asset Extraction Directories

On first run (or after version upgrade), embedded assets are extracted to a platform-specific cache directory.

### Linux

```
~/.cache/yetty/
├── shaders/
│   ├── gpu-screen.wgsl
│   ├── cursor.wgsl
│   ├── cards/*.wgsl
│   ├── effects/*.wgsl
│   ├── glyphs/*.wgsl
│   ├── lib/*.wgsl
│   ├── pre-effects/*.wgsl
│   └── post-effects/*.wgsl
├── fonts/
│   └── *.ttf
├── fonts-cdb/
│   └── *.cdb
├── logo.jpeg
└── .yetty-assets/
    └── version
```

**Resolution order:**
1. `$XDG_CACHE_HOME/yetty/`
2. `$HOME/.cache/yetty/`
3. `/tmp/yetty-cache/` (fallback)

### macOS

```
~/Library/Caches/yetty/
├── shaders/
├── fonts/
├── fonts-cdb/
├── logo.jpeg
└── .yetty-assets/version
```

**Resolution order:**
1. `~/Library/Caches/yetty/`
2. `$HOME/.cache/yetty/`
3. `/tmp/yetty-cache/`

### Windows

```
%LOCALAPPDATA%\yetty\cache\
├── shaders\
├── fonts\
├── fonts-cdb\
├── logo.jpeg
└── .yetty-assets\version
```

**Resolution order:**
1. `%LOCALAPPDATA%\yetty\cache\`
2. `%APPDATA%\yetty\cache\`
3. `%TEMP%\yetty-cache\`

### Android

```
/data/data/com.yetty.app/cache/
├── shaders/
├── fonts/
├── fonts-cdb/
├── logo.jpeg
└── .yetty-assets/version
```

Uses `YETTY_CACHE_DIR` environment variable set by the Android app.

### WebAssembly (Emscripten)

**No extraction needed.** WebAssembly uses Emscripten's virtual filesystem. Assets are embedded as stubs (incbin inline assembly doesn't work with WASM), and shaders are loaded from the `--preload-file` data.

## Version Checking

The marker file `.yetty-assets/version` contains the build version. On startup:

1. If marker missing → extract all assets
2. If version differs from `YETTY_BUILD_VERSION` → re-extract
3. Set `YETTY_SHADERS_DIR` to extracted path

## Build Requirements

- **brotli CLI**: Required at configure time for asset compression
  - Ubuntu/Debian: `apt install brotli`
  - macOS: `brew install brotli`
  - GitHub Actions: `apt-get install -y brotli`

## Platform Notes

- **Emscripten**: incbin inline assembly not supported; provides empty stubs
- **MSVC**: Uses incbin tool to generate data.c (no inline assembly)
- **GCC/Clang**: Uses incbin.h with inline assembly for direct embedding

## Compression: Brotli vs XZ

Yetty uses **brotli** for asset compression. While **xz** offers faster compression (10 sec vs 1 min 10 sec for CDB files) with slightly better ratios, brotli was chosen for:

- **Faster decompression** at runtime (matters more for first-run extraction)
- **MIT license** (Google-maintained)
- **No supply chain concerns** - xz had a backdoor incident in early 2024 where malicious code was injected into the build system. While caught and fixed before widespread deployment, it raised concerns about the project's maintenance. Brotli's Google backing provides more confidence in supply chain security.
