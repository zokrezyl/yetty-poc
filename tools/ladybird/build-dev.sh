#!/usr/bin/env bash
# Incremental build with ccache - for development
set -e
cd "$(dirname "$0")"

BUILD_DIR="build-dev"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure only if needed
if [ ! -f CMakeCache.txt ]; then
    nix develop ..#default --command cmake .. \
        -GNinja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
        -DBUILD_LIBWEB=ON \
        -DLADYBIRD_SOURCE_DIR="$HOME/work/my/yetty/tmp/ladybird"
fi

# Build with ccache
nix develop ..#default --command ninja -j$(nproc) "$@"
