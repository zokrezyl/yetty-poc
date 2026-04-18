# Yetty - WebGPU Terminal Emulator
#
# Nixpkgs-style package using Clang (clangStdenv).
# All CPM dependencies are pre-fetched via cpm-sources.nix.
#
# Usage:
#   nix build .#yetty
#   nix run .#yetty
#
{ lib
, clangStdenv
, fetchFromGitHub
, fetchurl
, cmake
, ninja
, pkg-config
, python3
, git
, patchelf

  # System libraries from nixpkgs
, fontconfig
, expat
, util-linux
, openssl
, curl
, zlib

  # X11/Wayland
, libGL
, xorg ? null
, libX11 ? xorg.libX11
, libXcursor ? xorg.libXcursor
, libXrandr ? xorg.libXrandr
, libXi ? xorg.libXi
, libXinerama ? xorg.libXinerama
, libxkbcommon
, wayland
, vulkan-loader
, libdrm
, mesa
, addDriverRunpath
, makeWrapper
, meson
, nasm

  # Build options
, enableTests ? false
, logLevel ? "yinfo"  # "ytrace" for full logging, "yinfo" for minimal
}:

let
  # Import pre-fetched CPM sources
  cpmSources = import ./cpm-sources.nix {
    inherit fetchFromGitHub fetchurl lib;
  };

  # Generate CMake -D flags for CPM sources
  # CPM checks variables like CPM_<name>_SOURCE (case-sensitive, matching NAME in CPMAddPackage)
  # Filter out sources that need special handling (dawn binaries, djb cdb, libmagic, curl, thorvg, zlib)
  cpmCMakeFlags = lib.mapAttrsToList
    (name: src: "-DCPM_${name}_SOURCE=${src}")
    (lib.filterAttrs (n: _:
      !(lib.hasPrefix "dawn-" n) &&
      n != "cdb" &&
      n != "libmagic" &&
      n != "curl" &&
      n != "thorvg" &&
      n != "zlib" &&
      n != "dav1d" &&
      n != "openh264"
    ) cpmSources);

  # Dawn for the current platform
  dawnSrc = cpmSources."dawn-linux";

  # curl for cpr (FetchContent dependency)
  curlSrc = cpmSources.curl;

  # Sources that need writable directories (cmake modifies them)
  thorvgSrc = cpmSources.thorvg;
  zlibSrc = cpmSources.zlib;

  # libmagic tarball (needs extraction and passes via FETCHCONTENT_SOURCE_DIR)
  libmagicSrc = cpmSources.libmagic;

  # Video codecs (dav1d, openh264) - need extraction for ExternalProject
  dav1dSrc = cpmSources.dav1d;
  openh264Src = cpmSources.openh264;

in
clangStdenv.mkDerivation (finalAttrs: {
  pname = "yetty";
  version = "0.1.0";

  src = lib.cleanSourceWith {
    src = ../..;
    filter = path: type:
      let baseName = baseNameOf path;
      in
      # Exclude build output directories (build-desktop-*, etc.) but keep build-tools
      !(lib.hasPrefix "build-" baseName && baseName != "build-tools") &&
      !(baseName == ".git") &&
      !(baseName == "result") &&
      !(lib.hasPrefix ".#" baseName);
  };

  nativeBuildInputs = [
    cmake
    ninja
    pkg-config
    python3
    git
    patchelf
    addDriverRunpath
    makeWrapper
    meson  # for dav1d
    nasm   # for dav1d asm optimizations
  ];

  buildInputs = [
    fontconfig
    expat
    util-linux
    openssl
    curl
    zlib
    libGL
    libX11
    libXcursor
    libXrandr
    libXi
    libXinerama
    libxkbcommon
    wayland
    vulkan-loader
    libdrm
  ];

  # Patch cmake to use shared libraries and disable tests
  postPatch = ''
    substituteInPlace build-tools/cmake/targets/linux.cmake \
      --replace 'find_library(FONTCONFIG_STATIC_LIB libfontconfig.a' 'find_library(FONTCONFIG_STATIC_LIB fontconfig' \
      --replace 'find_library(EXPAT_STATIC_LIB libexpat.a' 'find_library(EXPAT_STATIC_LIB expat' \
      --replace 'find_library(UUID_STATIC_LIB libuuid.a' 'find_library(UUID_STATIC_LIB uuid'

    # Disable tests and performance benchmarks (avoid fetching ut framework)
    substituteInPlace build-tools/cmake/targets/linux.cmake \
      --replace 'enable_testing()' '# enable_testing()' \
      --replace 'add_subdirectory(''${YETTY_ROOT}/test/ut' '# add_subdirectory(''${YETTY_ROOT}/test/ut' \
      --replace 'add_subdirectory(''${YETTY_ROOT}/test/performance' '# add_subdirectory(''${YETTY_ROOT}/test/performance'

    # Disable ytrace control socket (avoids libc++ bind() vs std::bind() collision)
    substituteInPlace build-tools/cmake/libs/ytrace.cmake \
      --replace 'if(APPLE OR WIN32)' 'if(TRUE)  # Also Linux with libc++'

    # Enable YETTY_SHADERS_DIR env var on all platforms (not just Android)
    substituteInPlace src/yetty/shader-manager.cpp \
      --replace '#if defined(__ANDROID__)' '#if 1  // Check env var on all platforms'

    # Enable YETTY_ASSETS_DIR env var on all platforms (not just Android)
    substituteInPlace src/yetty/font-manager.cpp \
      --replace '#if defined(__ANDROID__)' '#if 1  // Check env var on all platforms'
  '';

  # Set up Dawn, curl, and writable sources before configure
  preConfigure = ''
    # Extract Dawn (strip the top-level directory from tarball)
    mkdir -p $PWD/dawn-prebuilt
    tar -xzf ${dawnSrc} -C $PWD/dawn-prebuilt --strip-components=1

    # Extract curl for cpr (strip top-level directory)
    mkdir -p $PWD/curl-src
    tar -xJf ${curlSrc} -C $PWD/curl-src --strip-components=1

    # Copy sources that need writable directories (cmake modifies them)
    mkdir -p $PWD/thorvg-src
    cp -r ${thorvgSrc}/* $PWD/thorvg-src/
    chmod -R u+w $PWD/thorvg-src/

    mkdir -p $PWD/zlib-src
    cp -r ${zlibSrc}/* $PWD/zlib-src/
    chmod -R u+w $PWD/zlib-src/

    # Extract libmagic (strip top-level directory)
    mkdir -p $PWD/libmagic-src
    tar -xzf ${libmagicSrc} -C $PWD/libmagic-src --strip-components=1

    # libjpeg-turbo CMake hardcodes the source path - copy to expected location
    mkdir -p $PWD/build/_deps
    cp -r ${cpmSources."libjpeg-turbo"} $PWD/build/_deps/libjpeg-turbo-src
    chmod -R u+w $PWD/build/_deps/libjpeg-turbo-src

    # Extract dav1d (tarball)
    mkdir -p $PWD/dav1d-src
    tar -xzf ${dav1dSrc} -C $PWD/dav1d-src --strip-components=1

    # Copy openh264 source (from GitHub)
    mkdir -p $PWD/openh264-src
    cp -r ${openh264Src}/* $PWD/openh264-src/
    chmod -R u+w $PWD/openh264-src/

    # Create CMake cache init file for FetchContent overrides (in source root)
    cat > $PWD/nix-cache.cmake << EOF
set(FETCHCONTENT_SOURCE_DIR_DAWN_PREBUILT "$PWD/dawn-prebuilt" CACHE PATH "Dawn pre-built binaries" FORCE)
set(FETCHCONTENT_SOURCE_DIR_CURL "$PWD/curl-src" CACHE PATH "curl source" FORCE)
set(FETCHCONTENT_SOURCE_DIR_LIBMAGIC "$PWD/libmagic-src" CACHE PATH "libmagic source" FORCE)
EOF
  '';

  # Don't audit for references to /build/ (Dawn prebuilt has embedded paths)
  noAuditTmpdir = true;
  # Don't let Nix fixup shrink RPATH (removes dlopen'd libs like vulkan)
  dontPatchELF = true;

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
    "-DWEBGPU_BACKEND=dawn"
    "-G Ninja"
    # Include the cache init file with Dawn path (relative to source, cmake runs from build/)
    "-C" "../nix-cache.cmake"
    # Use portable howerj/cdb (goes through CPM, works offline)
    "-DYETTY_USE_HOWERJ_CDB=ON"
    # Disable tests (avoids fetching ut framework)
    "-DYETTY_BUILD_TESTS=OFF"
    # Writable sources (cmake needs to modify these)
    "-DCPM_thorvg_SOURCE=../thorvg-src"
    "-DCPM_zlib_SOURCE=../zlib-src"
    # Video codecs
    "-DCPM_dav1d_SOURCE=../dav1d-src"
    "-DCPM_openh264_SOURCE=../openh264-src"
  ] ++ cpmCMakeFlags
    ++ lib.optionals (logLevel == "yinfo") [
    "-DYTRACE_ENABLE_YTRACE=0"
    "-DYTRACE_ENABLE_YDEBUG=0"
  ];

  enableParallelBuilding = true;

  doCheck = enableTests;
  checkPhase = lib.optionalString enableTests ''
    runHook preCheck
    ctest --output-on-failure
    runHook postCheck
  '';

  installPhase = ''
    runHook preInstall

    mkdir -p $out/bin $out/share/yetty

    # Install binaries and fix RPATH
    for bin in yetty yettyc ycat yecho; do
      if [ -f "$bin" ]; then
        cp "$bin" $out/bin/
        # Fix RPATH: replace /build/ paths with nixpkgs libs
        local current_rpath=$(patchelf --print-rpath $out/bin/$bin)
        # Remove /build/ path and add nixpkgs zlib + vulkan (dlopen'd at runtime)
        local new_rpath=$(echo "$current_rpath" | sed 's|/build/[^:]*||g' | sed 's|^:||;s|:$||;s|::*|:|g')
        new_rpath="${zlib}/lib:${vulkan-loader}/lib:${libdrm}/lib:$new_rpath"
        patchelf --set-rpath "$new_rpath" $out/bin/$bin || true
        # Add /run/opengl-driver/lib for NixOS GPU drivers (Vulkan ICDs, etc)
        addDriverRunpath $out/bin/$bin
      fi
    done

    # Install shaders
    cp -r $src/src/yetty/shaders $out/share/yetty/

    # Install fonts (TTF files)
    cp $src/assets/*.ttf $out/share/yetty/

    # Install CDB font files (generated during build)
    mkdir -p $out/share/yetty/fonts-cdb
    cp assets/fonts-cdb/*.cdb $out/share/yetty/fonts-cdb/ 2>/dev/null || \
      echo "Warning: No CDB files found in assets/fonts-cdb/"

    # Wrap yetty with proper environment
    wrapProgram $out/bin/yetty \
      --set YETTY_SHADERS_DIR "$out/share/yetty/shaders" \
      --set YETTY_ASSETS_DIR "$out/share/yetty" \
      --prefix VK_ICD_FILENAMES : "${mesa}/share/vulkan/icd.d/intel_icd.x86_64.json:${mesa}/share/vulkan/icd.d/radeon_icd.x86_64.json:${mesa}/share/vulkan/icd.d/lvp_icd.x86_64.json" \
      --prefix LD_LIBRARY_PATH : "${mesa}/lib:${libdrm}/lib"

    runHook postInstall
  '';

  meta = {
    description = "WebGPU Terminal Emulator";
    longDescription = ''
      Yetty is a modern terminal emulator built with WebGPU for
      GPU-accelerated rendering. Features syntax highlighting, image
      support, and runs on Linux, macOS, Windows, Android, and web.
    '';
    homepage = "https://github.com/user/yetty";
    license = lib.licenses.mit;
    maintainers = [ ];
    platforms = lib.platforms.linux;
    mainProgram = "yetty";
  };
})
