# Build Speedup Strategies

This document describes strategies for speeding up yetty builds, both locally and on GitHub CI.

## Local Builds

### ccache (Compiler Cache)

ccache caches compiled object files and reuses them when the same source is compiled again with the same flags.

**Installation:**
```bash
# Ubuntu/Debian
sudo apt install ccache

# macOS
brew install ccache
```

**Usage with yetty:**
```bash
USE_CCACHE=1 make build-desktop-ytrace-release
```

**Configuration (~/.ccache/ccache.conf):**
```
max_size = 20G
compression = true
compression_level = 6
```

**Typical speedup:** 5-10x for incremental builds where few files changed.

### distcc (Distributed Compilation)

distcc distributes compilation across multiple machines on the network.

**Installation:**
```bash
# Ubuntu/Debian
sudo apt install distcc

# macOS
brew install distcc
```

**Setup:**
1. Run `distccd --daemon` on build servers
2. Set `DISTCC_HOSTS` to list of servers
3. Use with ccache for best results

**Usage:**
```bash
export DISTCC_HOSTS="localhost/4 server1/8 server2/8"
USE_CCACHE=1 USE_DISTCC=1 make build-desktop-ytrace-release
```

**Typical speedup:** Near-linear with number of cores across machines.

### Combining ccache + distcc

For maximum local build speed:
```bash
# ccache wraps distcc, distcc distributes, remote compiles, ccache caches result
export CCACHE_PREFIX="distcc"
USE_CCACHE=1 make build-desktop-ytrace-release
```

## GitHub CI Builds

### Problem

Full CI builds take 15-25 minutes per platform. Most time is spent rebuilding dependencies that rarely change (freetype, libuv, webgpu/dawn, etc.).

### Solution: Subsystem Library Caching

Cache pre-built libraries keyed by a hash of their source files and build configuration.

#### Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Build Pipeline                        │
├─────────────────────────────────────────────────────────┤
│                                                          │
│  ┌──────────┐    ┌──────────┐    ┌──────────┐           │
│  │ libuv    │    │ freetype │    │ dawn     │  ...      │
│  │ sources  │    │ sources  │    │ sources  │           │
│  └────┬─────┘    └────┬─────┘    └────┬─────┘           │
│       │               │               │                  │
│       ▼               ▼               ▼                  │
│  ┌──────────┐    ┌──────────┐    ┌──────────┐           │
│  │ hash()   │    │ hash()   │    │ hash()   │           │
│  │ +platform│    │ +platform│    │ +platform│           │
│  └────┬─────┘    └────┬─────┘    └────┬─────┘           │
│       │               │               │                  │
│       ▼               ▼               ▼                  │
│  ┌──────────────────────────────────────────┐           │
│  │         GitHub Actions Cache             │           │
│  │  Key: subsystem-platform-hash            │           │
│  │  Value: compiled .a/.lib files           │           │
│  └──────────────────────────────────────────┘           │
│                                                          │
└─────────────────────────────────────────────────────────┘
```

#### Cache Key Design

For each subsystem, the cache key includes:

```
{subsystem}-{platform}-{compiler}-{hash}

Examples:
  libuv-linux-gcc-a1b2c3d4
  freetype-windows-msvc-e5f6g7h8
  dawn-macos-clang-i9j0k1l2
```

The hash is computed from:
- All source files (`*.c`, `*.cpp`, `*.h`)
- CMake configuration files
- Relevant build flags

#### Implementation

**Workflow snippet:**
```yaml
- name: Cache libuv
  uses: actions/cache@v4
  with:
    path: build/deps/libuv
    key: libuv-${{ runner.os }}-${{ hashFiles('external/libuv/**/*.c', 'external/libuv/**/*.h', 'cmake/libs/libuv.cmake') }}
    restore-keys: |
      libuv-${{ runner.os }}-

- name: Build libuv
  if: steps.cache-libuv.outputs.cache-hit != 'true'
  run: cmake --build build --target uv_a
```

#### Subsystems to Cache

| Subsystem | Approx Build Time | Source Stability |
|-----------|-------------------|------------------|
| dawn/webgpu | 8-12 min | Very stable |
| freetype | 1-2 min | Very stable |
| libuv | 30 sec | Very stable |
| msdfgen | 30 sec | Very stable |
| openh264 | 2-3 min | Very stable |
| dav1d | 1-2 min | Very stable |
| tree-sitter | 1 min | Stable |
| glfw | 30 sec | Very stable |

**Priority:** Cache dawn/webgpu first - it's the slowest to build.

#### GitHub Actions Cache Limits

- **Total size:** 10 GB per repository
- **Retention:** 7 days (evicted if not accessed)
- **Scope:** Branch-scoped with fallback to default branch

#### Estimated Savings

| Scenario | Current | With Caching |
|----------|---------|--------------|
| Full rebuild | 20 min | 20 min |
| Deps unchanged | 20 min | 3-5 min |
| Typical PR | 20 min | 5-8 min |

### Alternative: Pre-built Dependencies

For maximum speed, pre-build dependencies and store as release artifacts:

1. Create a `deps-vX.Y` release with compiled libraries per platform
2. Download in CI instead of building
3. Update release only when dependencies change

**Pros:** Fastest possible CI, no cache eviction issues
**Cons:** Manual maintenance, storage costs

## Implementation Roadmap

1. **Phase 1:** Enable ccache in CI (simple, immediate benefit)
2. **Phase 2:** Cache dawn/webgpu (biggest time saver)
3. **Phase 3:** Cache remaining heavy dependencies
4. **Phase 4:** Consider pre-built releases for ultra-stable deps

## Monitoring

Track build times with GitHub Actions timing:
```yaml
- name: Build
  run: |
    start=$(date +%s)
    cmake --build build
    echo "::notice::Build took $(($(date +%s) - start)) seconds"
```
