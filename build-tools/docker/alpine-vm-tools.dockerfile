# Alpine Linux build environment for static VM tools
# Builds x86_64 musl-linked static binaries for JSLinux Alpine VM
FROM alpine:3.21

# Install build dependencies
RUN apk add --no-cache \
    build-base \
    cmake \
    python3 \
    git \
    curl \
    openssl-dev \
    openssl-libs-static \
    fontconfig-dev \
    fontconfig-static \
    freetype-dev \
    freetype-static \
    file-dev \
    autoconf \
    automake \
    libtool \
    linux-headers \
    perl \
    pkgconf \
    expat-dev \
    expat-static \
    bzip2-dev \
    bzip2-static \
    libpng-dev \
    libpng-static \
    brotli-dev \
    brotli-static \
    zlib-dev \
    zlib-static

# Create build user (match host UID if passed)
ARG UID=1000
ARG GID=1000
RUN addgroup -g ${GID} builder && \
    adduser -D -u ${UID} -G builder builder

# Set working directory
WORKDIR /src

# Default command - builds all vm-tools
CMD ["sh", "-c", "\
    mkdir -p /build/vm-tools && \
    cmake -S /src/build-tools/cmake/host-tools -B /build/vm-tools-build \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_EXE_LINKER_FLAGS=-static \
        -DBUILD_VM_TOOLS=ON && \
    cmake --build /build/vm-tools-build --target yecho-static ycat-static ybrowser-static -j$(nproc) && \
    cp /build/vm-tools-build/yecho /build/vm-tools/ && \
    cp /build/vm-tools-build/ycat /build/vm-tools/ && \
    cp /build/vm-tools-build/ybrowser /build/vm-tools/ && \
    cp /build/vm-tools-build/_deps/libmagic/share/misc/magic.mgc /build/vm-tools/ && \
    echo '=== Build complete ===' && \
    ls -la /build/vm-tools/ && \
    file /build/vm-tools/*"]
