# Yetty Dependencies

This document lists all third-party dependencies and their licenses.

## Overview

Yetty is licensed under the **AGPL-3.0** (dual license). All dependencies use permissive licenses fully compatible with AGPL-3.0.

- **Desktop & Web builds:** 100% permissive (MIT, BSD, Zlib, Apache-2.0)
- **Android builds:** Include Apache 2.0 (Android NDK components)

## Dependency License Summary

| Dependency | Version | License | Type | Usage |
|-----------|---------|---------|------|-------|
| libvterm | 0.3.3 | MIT | Bundled | Terminal emulation |
| wgpu-native | 27.0.4.0 | MIT/Apache-2.0 | External | GPU graphics |
| glm | 1.0.1 | MIT | External | Math library |
| stb | master | MIT/Unlicense | External | Image loading |
| yaml-cpp | 0.8.0 | MIT | External | Config parsing |
| args | 6.4.6 | MIT | External | CLI parsing |
| spdlog | 1.16.0 | MIT | External | Logging |
| lz4 | 1.10.0 | BSD 2-Clause | External | Compression |
| FreeType | 2.13.2 | FreeType License (BSD-like) | External | Font rendering |
| msdfgen | 1.12 | MIT/Unlicense | External | Font atlas generation |
| glfw | 3.4 | Zlib | External | Window management |
| glfw3webgpu | main | MIT/Apache-2.0 | External | WebGPU bindings |
| libuv | 1.48.0 | MIT | External | Async I/O |
| Toybox | latest | BSD 0-Clause | External | Android shell utilities |
| native_app_glue | NDK | Apache 2.0 | External | Android app framework |
| Fontconfig | system | MIT | System | Font fallback (Unix only) |
| Python | optional | PSF License | Optional | Plugin support |
| DirectWrite/GDI | system | Proprietary | System | Font discovery (Windows) |
| msgpack-c | 7.0.0 | BSL-1.0 (Boost) | External | RPC serialization |
| Dawn | 20260117 | BSD-3-Clause | External | WebGPU backend |
| ThorVG | 1.0-pre34 | MIT | External | SVG/Lottie rendering |
| pdfio | 1.4.0 | Apache-2.0 | External | PDF rendering |
| PDFium | optional | BSD-3-Clause | Optional | PDF rendering (alt) |
| ImGui | 1.92.5 | MIT | External | GUI widgets (Ymery) |
| litehtml | 0.9 | BSD-3-Clause | External | HTML/CSS rendering |
| cpr | 1.14.1 | MIT | External | HTTP client |
| libcurl | system | MIT/X11 | System | HTTP transport |
| OpenSSL | system | Apache-2.0 | System | TLS |
| tree-sitter | 0.26.5 | MIT | External | Syntax highlighting |
| tinyxml2 | 10.0.0 | Zlib | External | XML parsing |
| libmagic | 5.45 | BSD-2-Clause | External | File type detection |
| Boost.UT | 2.1.0 | BSL-1.0 (Boost) | External | Testing framework |

## Platform-Specific Licensing

### Desktop (Linux/macOS/Windows)

**All permissive licenses, AGPL-3.0 compatible:**
- Core: AGPL-3.0
- All dependencies: MIT, BSD, Zlib, Apache-2.0, BSL-1.0

**Risk Level:** ✅ None

### Web (Emscripten/WASM)

**All permissive licenses, AGPL-3.0 compatible:**
- Core: AGPL-3.0
- All dependencies: MIT, BSD, Zlib

**Risk Level:** ✅ None

### Android

**All AGPL-3.0 compatible:**
- Core Yetty code: AGPL-3.0
- All dependencies: MIT, BSD, Apache 2.0
- Android NDK components: Apache 2.0 (native_app_glue)

**Risk Level:** ✅ None

**Compliance Requirements:**
- Include AGPL-3.0 and Apache 2.0 license texts when distributing APKs
- See [Apache 2.0 License](LICENSES/Apache-2.0)

## Detailed Dependency Information

### Core Dependencies (All Platforms)

#### libvterm (0.3.3)
- **License:** MIT
- **Purpose:** Terminal emulation engine (VT100/xterm compatibility)
- **Bundled:** Yes (src/libvterm-0.3.3)
- **Risk:** ✅ None

#### wgpu-native (27.0.4.0)
- **License:** MIT or Apache 2.0 (dual licensed)
- **Purpose:** WebGPU graphics abstraction layer
- **Repository:** https://github.com/gfx-rs/wgpu
- **Risk:** ✅ None (both licenses are permissive)

#### glm (1.0.1)
- **License:** MIT / Happy Bunny License (dual)
- **Purpose:** Math library for 3D graphics
- **Repository:** https://github.com/g-truc/glm
- **Risk:** ✅ None

#### yaml-cpp (0.8.0)
- **License:** MIT
- **Purpose:** YAML configuration file parsing
- **Repository:** https://github.com/jbeder/yaml-cpp
- **Risk:** ✅ None

#### spdlog (1.16.0)
- **License:** MIT
- **Purpose:** Fast logging library
- **Repository:** https://github.com/gabime/spdlog
- **Risk:** ✅ None

#### libuv (1.48.0)
- **License:** MIT
- **Purpose:** Async I/O library (PTY reading, event loops)
- **Repository:** https://github.com/libuv/libuv
- **Platforms:** Desktop, Android (Web uses Emscripten)
- **Risk:** ✅ None

### Desktop-Specific Dependencies

#### FreeType (2.13.2)
- **License:** FreeType License (BSD-like, MIT-compatible)
- **Purpose:** Font rendering and glyph management
- **Repository:** https://github.com/freetype/freetype
- **Configuration:** PNG support enabled, HarfBuzz/Brotli disabled
- **Dependencies:** zlib (MIT), libpng (BSD)
- **Risk:** ✅ None (modern version, GPL-free)

#### msdfgen (1.12)
- **License:** MIT / Unlicense (dual)
- **Purpose:** Multi-channel signed distance field font atlas generation
- **Repository:** https://github.com/Chlumsky/msdfgen
- **Risk:** ✅ None

#### glfw (3.4)
- **License:** Zlib (MIT-compatible)
- **Purpose:** Cross-platform window and input management
- **Repository:** https://github.com/glfw/glfw
- **Risk:** ✅ None

#### glfw3webgpu
- **License:** MIT / Apache 2.0 (dual licensed)
- **Purpose:** WebGPU bindings for GLFW
- **Repository:** https://github.com/eliemichel/glfw3webgpu
- **Risk:** ✅ None

#### args (6.4.6)
- **License:** MIT
- **Purpose:** Command-line argument parsing
- **Repository:** https://github.com/Taywee/args
- **Platforms:** Desktop only
- **Risk:** ✅ None

#### Fontconfig (system library)
- **License:** MIT
- **Purpose:** System font discovery and fallback (Unix/Linux)
- **Platforms:** Desktop Linux/macOS (Unix)
- **Risk:** ✅ None

### Compression & Performance

#### lz4 (1.10.0)
- **License:** BSD 2-Clause for library code
- **Purpose:** Fast font cache compression
- **Repository:** https://github.com/lz4/lz4
- **Note:** LZ4 offers both BSD (library) and GPL (CLI tools) versions
- **Usage:** Library-only, not CLI tools
- **Risk:** ✅ None (using BSD library version)

### Android-Specific Dependencies

#### Toybox (latest)
- **License:** BSD 0-Clause (Unlicense-like)
- **Purpose:** Lightweight shell utilities (sh, ls, cat, etc.)
- **Repository:** https://github.com/landley/toybox
- **Used for:** Android runtime shell environment
- **Risk:** ✅ None

#### native_app_glue (Android NDK)
- **License:** Apache License 2.0
- **Purpose:** Android native application framework
- **Source:** Android NDK (system component)
- **Platforms:** Android only
- **Risk:** ⚠️ Moderate - requires Apache 2.0 compliance for distribution

**Apache 2.0 Implications:**
- Required disclosure when distributing Android APKs
- Must include Apache 2.0 license text
- Patent grant clause applies
- Requires explicit notice to users
- See [Apache 2.0 License](LICENSES/Apache-2.0)

### Optional Dependencies

#### Python (optional, Unix only)
- **License:** Python Software Foundation License
- **Purpose:** Optional plugin support system
- **Platforms:** Desktop (Unix/Linux/macOS) only
- **Configuration:** `./configure && make` integration
- **Risk:** ✅ None (PSF license is MIT-compatible)

## License Compatibility Analysis

### AGPL-3.0 Project with Permissive Dependencies

All dependency licenses are permissive and fully compatible with AGPL-3.0.

| License Type | Count | Compatibility | Notes |
|------------|-------|--------------|-------|
| MIT | 30+ | ✅ Perfect | Permissive, compatible |
| MIT/Unlicense (dual) | 2 | ✅ Perfect | Permissive |
| MIT/Apache-2.0 (dual) | 2 | ✅ Perfect | Both compatible |
| BSD-2-Clause | 2 | ✅ Perfect | Permissive |
| BSD-3-Clause | 3 | ✅ Perfect | Permissive |
| BSD-like (FreeType) | 1 | ✅ Perfect | Permissive |
| Zlib | 3 | ✅ Perfect | Permissive |
| Apache 2.0 | 4 | ✅ Perfect | Compatible with AGPL-3.0 |
| BSL-1.0 (Boost) | 2 | ✅ Perfect | Permissive |
| PSF License | 1 | ✅ Perfect | Permissive |

**Total Risk:** ✅ NONE — all dependencies use permissive licenses compatible with AGPL-3.0

## Building and Licensing Implications

### Desktop Release
```bash
make build-desktop-release
```
- **Output:** `./yetty` executable
- **Licensing:** AGPL-3.0
- **Redistribution:** Must comply with AGPL-3.0 (source disclosure)

### Android Release
```bash
make build-android-release
```
- **Output:** `app-release.apk`
- **Licensing:** AGPL-3.0 + Apache 2.0 (NDK components)
- **Redistribution:** Must comply with AGPL-3.0 and include Apache 2.0 notice

### Web Release
```bash
make build-webasm-release
```
- **Output:** `yetty.js` WebAssembly binary
- **Licensing:** AGPL-3.0
- **Redistribution:** Must comply with AGPL-3.0 (source disclosure)

## Distribution Checklist

### All Platforms
- ✅ Include AGPL-3.0 License file
- ✅ List all dependencies and their licenses
- ✅ Link to dependency source repositories
- ✅ Provide source code access per AGPL-3.0

### Android APK Distribution
- ✅ Include AGPL-3.0 and Apache 2.0 license texts
- ✅ Provide source code access per AGPL-3.0

### Source Code Distribution
- ✅ Include LICENSE file
- ✅ Include DEPENDENCIES.md
- ✅ Preserve all vendored source licenses (libvterm)

## License Texts

Full license texts are available in the `LICENSES/` directory:
- `LICENSES/AGPL-3.0` - GNU Affero General Public License v3.0 (Yetty)
- `LICENSES/MIT` - MIT License (most dependencies)
- `LICENSES/Apache-2.0` - Apache License 2.0 (Android NDK)
- `LICENSES/BSD-2-Clause` - BSD 2-Clause License (lz4)
- `LICENSES/FreeType` - FreeType License
- `LICENSES/Zlib` - Zlib License (glfw)

## Updating Dependencies

When adding or updating dependencies:

1. **Check the license** - Must be AGPL-3.0 compatible (permissive licenses like MIT, BSD, Apache-2.0 are fine; GPL/LGPL are also compatible)
2. **Update this file** - Add to appropriate section
3. **Avoid proprietary-only licenses** - These are incompatible with AGPL-3.0
4. **Test build** - Ensure no unexpected dependencies are pulled in
5. **Document usage** - Why is this dependency needed?

## External References

- [MIT License](https://opensource.org/licenses/MIT)
- [Apache License 2.0](https://opensource.org/licenses/Apache-2.0)
- [BSD 2-Clause License](https://opensource.org/licenses/BSD-2-Clause)
- [Zlib License](https://opensource.org/licenses/Zlib)
- [SPDX License List](https://spdx.org/licenses/)

## Contact

For licensing questions or concerns, please open an issue on the Yetty GitHub repository.

---

Last updated: 2026-02-12
