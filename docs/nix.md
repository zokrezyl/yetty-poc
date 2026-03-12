# Nix Build System for Yetty

Yetty uses Nix for reproducible builds. This document describes the build system architecture, challenges encountered, and how to extend it for other platforms.

## Quick Start

```bash
# Build yetty
nix build .#yetty

# Run yetty
nix run .#yetty

# Enter development shell
nix develop
```

## Architecture Overview

The Nix build system consists of:

```
flake.nix                      # Entry point, defines packages and dev shells
build-tools/nix/
├── default.nix                # Main package definition
├── cpm-sources.nix            # Pre-fetched CPM dependencies with hashes
├── versions.txt               # Dependency versions (source of truth)
├── update-deps.sh             # Script to update dependency hashes
└── README.md                  # Quick reference
```

## Key Challenges and Solutions

### 1. Offline/Sandboxed Builds

**Challenge:** Nix builds run in a sandboxed environment without network access. CPM (CMake Package Manager) normally downloads dependencies at configure time.

**Solution:** Pre-fetch all CPM dependencies in `cpm-sources.nix` with their SHA256 hashes. Pass source paths to CMake via `-DCPM_<name>_SOURCE=<path>` flags.

```nix
# cpm-sources.nix
args = fetchFromGitHub {
  owner = "Taywee";
  repo = "args";
  rev = "6.4.6";
  hash = "sha256-2gH3h3QAl0+XVULt/fR2UUGKNIxTUmnRRdxPOFdpVy4=";
};
```

### 2. Dawn WebGPU Pre-built Binaries

**Challenge:** Building Dawn from source requires 30+ minutes and complex dependencies. Dawn releases pre-built binaries compiled with libstdc++.

**Solution:**
- Use pre-built Dawn binaries from GitHub releases
- Use `clangStdenv` (Clang compiler with libstdc++) for ABI compatibility
- Extract binaries to a known path and pass via CMake cache file

```nix
clangStdenv = pkgs.clangStdenv;  # Clang + libstdc++
```

### 3. Vulkan/GPU Driver Discovery

**Challenge:** Vulkan drivers are loaded dynamically via `dlopen()`. The Vulkan loader needs to find ICD (Installable Client Driver) JSON files.

**Solution:**
- Add `/run/opengl-driver/lib` to RPATH via `addDriverRunpath` (NixOS standard location)
- Wrap binary with `VK_ICD_FILENAMES` pointing to Mesa's Vulkan ICDs
- Include Mesa's lib in `LD_LIBRARY_PATH` for ICD dependencies

```nix
wrapProgram $out/bin/yetty \
  --prefix VK_ICD_FILENAMES : "${mesa}/share/vulkan/icd.d/intel_icd.x86_64.json:..." \
  --prefix LD_LIBRARY_PATH : "${mesa}/lib:${libdrm}/lib"
```

### 4. Runtime Asset Paths

**Challenge:** Shaders and fonts are looked up relative to `CMAKE_SOURCE_DIR` at compile time, which doesn't exist at runtime.

**Solution:**
- Patch source to check environment variables on all platforms (not just Android)
- Wrap binary with `YETTY_SHADERS_DIR` and `YETTY_ASSETS_DIR`

```nix
# Patch to enable env var checks on all platforms
substituteInPlace src/yetty/shader-manager.cpp \
  --replace '#if defined(__ANDROID__)' '#if 1'

# Wrap with correct paths
wrapProgram $out/bin/yetty \
  --set YETTY_SHADERS_DIR "$out/share/yetty/shaders" \
  --set YETTY_ASSETS_DIR "$out/share/yetty"
```

### 5. ExternalProject Dependencies (libmagic, dav1d, openh264)

**Challenge:** Some dependencies use CMake's `ExternalProject_Add` which downloads sources separately from CPM.

**Solution:**
- Add `FETCHCONTENT_SOURCE_DIR_<NAME>` support to CMake files
- Extract/copy sources in `preConfigure` phase
- Pass paths via CMake cache file

```nix
preConfigure = ''
  mkdir -p $PWD/libmagic-src
  tar -xzf ${libmagicSrc} -C $PWD/libmagic-src --strip-components=1

  cat > $PWD/nix-cache.cmake << EOF
  set(FETCHCONTENT_SOURCE_DIR_LIBMAGIC "$PWD/libmagic-src" CACHE PATH "" FORCE)
  EOF
'';
```

### 6. RPATH Handling

**Challenge:**
- Build-time paths (`/build/...`) leak into RPATH
- Nix's fixup phase runs `shrink-rpath` which removes dlopen'd libraries

**Solution:**
```nix
noAuditTmpdir = true;   # Allow /build/ references (Dawn has embedded paths)
dontPatchELF = true;    # Don't shrink RPATH (keeps Vulkan, zlib)

# Manually fix RPATH in installPhase
patchelf --set-rpath "${zlib}/lib:${vulkan-loader}/lib:..." $out/bin/yetty
addDriverRunpath $out/bin/yetty  # Add /run/opengl-driver/lib
```

## Dependency Categories

| Category | Examples | Handling |
|----------|----------|----------|
| CPM (simple) | args, lz4, glm, stb | Auto via `cpmCMakeFlags` |
| CPM (writable) | thorvg, zlib | Copy to writable dir, pass path |
| ExternalProject | libmagic, dav1d, openh264 | Extract, use FETCHCONTENT_SOURCE_DIR |
| Pre-built | Dawn | Extract tarball, CMake cache file |
| System libs | fontconfig, openssl, X11 | `buildInputs` |
| Runtime (dlopen) | Vulkan, Mesa | RPATH + wrapper |

## Updating Dependencies

```bash
cd build-tools/nix
./update-deps.sh  # Updates hashes in cpm-sources.nix from versions.txt
```

## Troubleshooting

### "cannot open shared object file"
Check RPATH: `patchelf --print-rpath result/bin/yetty`
Add missing library to buildInputs and RPATH fix in installPhase.

### "vkCreateInstance failed"
Vulkan ICD not found. Check:
- `VK_ICD_FILENAMES` in wrapper
- Mesa lib in `LD_LIBRARY_PATH`
- `/run/opengl-driver` exists (NixOS) or use nixGL (non-NixOS)

### Build downloads something
A dependency is missing from `cpm-sources.nix`. Add it with:
```bash
nix-prefetch-github owner repo --rev version
```

---

## Cross-Platform Build Design

### Current State Analysis

| Target | Build Tool | WebGPU Backend | Output | Cross-compile |
|--------|------------|----------------|--------|---------------|
| Linux | CMake/Ninja | Dawn (pre-built) | ELF binary | No |
| WebAssembly | Emscripten | emdawnwebgpu port | .wasm + .js | Yes (wasm32) |
| Windows | CMake/Ninja | Dawn (pre-built) | .exe | Yes (mingw) |
| Android | CMake/Ninja + Gradle | Dawn (pre-built) | .so + APK | Yes (aarch64) |

### Proposed Modular Structure

```
build-tools/nix/
├── cpm-sources.nix          # Shared: all CPM dependencies
├── common.nix               # Shared: source filtering, CPM flag generation
├── platforms/
│   ├── linux.nix            # Linux desktop package
│   ├── webasm.nix           # WebAssembly package
│   ├── windows.nix          # Windows cross-compile
│   └── android.nix          # Android cross-compile
├── lib/
│   ├── dawn.nix             # Dawn setup (platform-specific binaries)
│   ├── video-codecs.nix     # dav1d, openh264 setup
│   └── wrappers.nix         # Binary wrapping utilities
└── default.nix              # Backwards-compatible (imports linux.nix)
```

### Module Design

#### 1. `common.nix` - Shared Logic

```nix
# build-tools/nix/common.nix
{ lib, fetchFromGitHub, fetchurl }:

rec {
  # Import CPM sources
  cpmSources = import ./cpm-sources.nix { inherit fetchFromGitHub fetchurl lib; };

  # Source filtering (exclude build dirs, .git, etc.)
  sourceFilter = path: type:
    let baseName = baseNameOf path;
    in !(lib.hasPrefix "build-" baseName && baseName != "build-tools") &&
       !(baseName == ".git") &&
       !(baseName == "result") &&
       !(lib.hasPrefix ".#" baseName);

  # Generate CPM flags for simple dependencies
  mkCpmFlags = excludeList:
    lib.mapAttrsToList
      (name: src: "-DCPM_${name}_SOURCE=${src}")
      (lib.filterAttrs (n: _: !(builtins.elem n excludeList)) cpmSources);

  # Common patches for all platforms
  commonPatches = ''
    # Enable env var checks on all platforms
    substituteInPlace src/yetty/shader-manager.cpp \
      --replace '#if defined(__ANDROID__)' '#if 1'
    substituteInPlace src/yetty/font-manager.cpp \
      --replace '#if defined(__ANDROID__)' '#if 1'
  '';

  # Writable source setup (for deps that cmake modifies)
  setupWritableSources = { thorvg, zlib, ... }: ''
    mkdir -p $PWD/thorvg-src && cp -r ${thorvg}/* $PWD/thorvg-src/ && chmod -R u+w $PWD/thorvg-src/
    mkdir -p $PWD/zlib-src && cp -r ${zlib}/* $PWD/zlib-src/ && chmod -R u+w $PWD/zlib-src/
  '';
}
```

#### 2. `lib/dawn.nix` - Dawn Setup

```nix
# build-tools/nix/lib/dawn.nix
{ lib, stdenv, cpmSources }:

let
  # Select Dawn binary based on target platform
  dawnBinary = {
    "x86_64-linux" = cpmSources."dawn-linux";
    "aarch64-linux" = cpmSources."dawn-linux";  # Same for now
    "x86_64-darwin" = cpmSources."dawn-macos-x86_64";
    "aarch64-darwin" = cpmSources."dawn-macos-arm64";
    # Windows/Android use different approach
  }.${stdenv.hostPlatform.system} or (throw "Unsupported platform for Dawn");
in {
  inherit dawnBinary;

  # Setup commands for preConfigure
  setupDawn = ''
    mkdir -p $PWD/dawn-prebuilt
    tar -xzf ${dawnBinary} -C $PWD/dawn-prebuilt --strip-components=1
  '';

  # CMake cache entries
  dawnCacheEntries = ''
    set(FETCHCONTENT_SOURCE_DIR_DAWN_PREBUILT "$PWD/dawn-prebuilt" CACHE PATH "" FORCE)
  '';
}
```

#### 3. `platforms/linux.nix` - Linux Package

```nix
# build-tools/nix/platforms/linux.nix
{ lib, clangStdenv, callPackage, ... }@args:

let
  common = callPackage ../common.nix {};
  dawn = callPackage ../lib/dawn.nix { inherit (common) cpmSources; };
  videoCodecs = callPackage ../lib/video-codecs.nix { inherit (common) cpmSources; };
in
clangStdenv.mkDerivation {
  pname = "yetty";
  version = "0.1.0";

  src = lib.cleanSourceWith {
    src = ../../..;
    filter = common.sourceFilter;
  };

  nativeBuildInputs = [ /* cmake, ninja, etc. */ ];
  buildInputs = [ /* fontconfig, X11, vulkan, etc. */ ];

  postPatch = common.commonPatches + ''
    # Linux-specific patches
    substituteInPlace build-tools/cmake/targets/linux.cmake \
      --replace 'find_library(FONTCONFIG_STATIC_LIB libfontconfig.a' 'find_library(FONTCONFIG_STATIC_LIB fontconfig'
  '';

  preConfigure = dawn.setupDawn + videoCodecs.setupVideoCodecs + common.setupWritableSources common.cpmSources + ''
    cat > $PWD/nix-cache.cmake << EOF
    ${dawn.dawnCacheEntries}
    ${videoCodecs.videoCacheEntries}
    EOF
  '';

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
    "-DWEBGPU_BACKEND=dawn"
    "-G Ninja"
  ] ++ common.mkCpmFlags [ "dawn-linux" "dawn-macos-arm64" "dawn-macos-x86_64" "dav1d" "openh264" "curl" "libmagic" "thorvg" "zlib" ];

  # ... installPhase, meta, etc.
}
```

#### 4. `platforms/webasm.nix` - WebAssembly Package

```nix
# build-tools/nix/platforms/webasm.nix
{ lib, stdenv, emscripten, callPackage, ... }:

let
  common = callPackage ../common.nix {};
in
stdenv.mkDerivation {
  pname = "yetty-web";
  version = "0.1.0";

  src = lib.cleanSourceWith {
    src = ../../..;
    filter = common.sourceFilter;
  };

  nativeBuildInputs = [
    emscripten
    # Host tools for CDB generation
  ];

  # No buildInputs needed - all linked statically via emscripten

  postPatch = ''
    # WebAssembly doesn't need env var patches (uses /assets paths)
  '';

  preConfigure = common.setupWritableSources common.cpmSources + ''
    # Set up emscripten cache
    export EM_CACHE=$TMPDIR/emscripten-cache
  '';

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
    "-DCMAKE_TOOLCHAIN_FILE=${emscripten}/share/emscripten/cmake/Modules/Platform/Emscripten.cmake"
    "-G Ninja"
    # WebAssembly uses browser WebGPU via emdawnwebgpu port
  ] ++ common.mkCpmFlags [ "dawn-linux" "dawn-macos-arm64" "dawn-macos-x86_64" "dav1d" "openh264" "thorvg" "zlib" ];

  installPhase = ''
    mkdir -p $out/share/yetty-web
    cp yetty.wasm yetty.js $out/share/yetty-web/
    cp -r assets $out/share/yetty-web/
    # Copy HTML wrapper
    cp $src/build-tools/webasm/index.html $out/share/yetty-web/
  '';
}
```

#### 5. `platforms/windows.nix` - Windows Cross-Compile

```nix
# build-tools/nix/platforms/windows.nix
{ lib, pkgsCross, callPackage, ... }:

let
  mingw = pkgsCross.mingwW64;
  common = callPackage ../common.nix {};
in
mingw.stdenv.mkDerivation {
  pname = "yetty-windows";
  version = "0.1.0";

  src = lib.cleanSourceWith {
    src = ../../..;
    filter = common.sourceFilter;
  };

  depsBuildBuild = [
    # Host tools for CDB generation
    pkgs.cmake pkgs.ninja
  ];

  nativeBuildInputs = [
    mingw.cmake
    mingw.ninja
  ];

  # Note: Dawn for Windows needs MSVC-compiled binaries
  # Cross-compiling Dawn with MinGW is complex
  # Alternative: Build Dawn from source with MinGW

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
    "-DCMAKE_SYSTEM_NAME=Windows"
    "-G Ninja"
  ] ++ common.mkCpmFlags [ /* exclusions */ ];

  # Windows-specific: no RPATH, copy DLLs
  installPhase = ''
    mkdir -p $out/bin
    cp yetty.exe $out/bin/
    # Bundle required DLLs
  '';
}
```

#### 6. `platforms/android.nix` - Android Cross-Compile

```nix
# build-tools/nix/platforms/android.nix
{ lib, androidenv, callPackage, ... }:

let
  common = callPackage ../common.nix {};

  androidComposition = androidenv.composeAndroidPackages {
    platformVersions = [ "34" ];
    ndkVersions = [ "26.1.10909125" ];
    includeNDK = true;
  };

  androidNdk = "${androidComposition.androidsdk}/libexec/android-sdk/ndk/26.1.10909125";
in
stdenv.mkDerivation {
  pname = "yetty-android";
  version = "0.1.0";

  src = lib.cleanSourceWith {
    src = ../../..;
    filter = common.sourceFilter;
  };

  nativeBuildInputs = [
    androidComposition.androidsdk
    # Host tools for CDB generation
  ];

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
    "-DCMAKE_TOOLCHAIN_FILE=${androidNdk}/build/cmake/android.toolchain.cmake"
    "-DANDROID_ABI=arm64-v8a"
    "-DANDROID_PLATFORM=android-26"
    "-G Ninja"
  ] ++ common.mkCpmFlags [ /* exclusions */ ];

  # Build produces .so, needs Gradle for APK
  installPhase = ''
    mkdir -p $out/lib
    cp libyetty.so $out/lib/
    # Could also run Gradle here to produce APK
  '';
}
```

### Updated `flake.nix`

```nix
{
  outputs = { self, nixpkgs, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in {
        packages = {
          # Linux desktop (default)
          default = pkgs.callPackage ./build-tools/nix/platforms/linux.nix {};
          yetty = self.packages.${system}.default;

          # WebAssembly
          yetty-web = pkgs.callPackage ./build-tools/nix/platforms/webasm.nix {};

          # Windows (cross-compile from Linux)
          yetty-windows = pkgs.callPackage ./build-tools/nix/platforms/windows.nix {};

          # Android (cross-compile from Linux)
          yetty-android = pkgs.callPackage ./build-tools/nix/platforms/android.nix {};
        };
      }
    );
}
```

### Platform-Specific Challenges

#### WebAssembly
- **Already works in Nix devshell** - uses `nix develop .#web` + `make web`
- **Emscripten ports** - emdawnwebgpu provides WebGPU, no Dawn binary needed
- **Host tools** - CDB generation needs host-compiled `yetty-msdf-gen`
- **No runtime wrapping** - assets bundled in wasm

#### Windows
- **Dawn binaries** - Pre-built Dawn is MSVC; MinGW cross-compile needs Dawn from source
- **Alternative**: Use Wine + MSVC in Nix (complex) or accept MinGW limitations
- **Dependencies**: Most CPM deps cross-compile fine with MinGW
- **DLL bundling**: Need to copy runtime DLLs to output

#### Android
- **NDK toolchain** - Well-supported in Nix via `androidenv`
- **Dawn binaries** - Need Android ARM64 build (build from source or add to releases)
- **Gradle** - APK packaging requires Gradle, can run in Nix
- **Host tools** - CDB generation needs host-compiled tools (depsBuildBuild)

### Code Smell Avoidance

1. **DRY (Don't Repeat Yourself)**
   - `common.nix` extracts shared logic
   - `cpm-sources.nix` is single source of truth for deps
   - Platform modules import shared functions

2. **Single Responsibility**
   - Each platform file handles one target
   - `lib/` modules handle specific concerns (Dawn, video codecs)

3. **Dependency Injection**
   - Platforms receive dependencies via `callPackage`
   - Easy to test/override individual components

4. **Explicit over Implicit**
   - CPM exclusion lists are explicit per-platform
   - No magic - each platform declares its needs

5. **Avoid Premature Abstraction**
   - Start with Linux working
   - Extract common patterns as other platforms are added
   - Don't abstract until you see the pattern 3 times
