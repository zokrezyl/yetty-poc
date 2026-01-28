# msdfgl (Abandoned)

This was an attempt to port the original [msdfgl](https://github.com/pboettch/msdfgl) OpenGL library to WebGPU.

## Why Abandoned

The original msdfgl uses a render pipeline approach where:
- Each glyph is rendered as a quad to the atlas texture
- The fragment shader calculates MSDF distances per-pixel

While the port compiles and runs without GPU crashes, the fragment shader output isn't reaching the atlas texture correctly. After significant debugging of:
- Buffer format mismatches (metadata byte vs uint32)
- Buffer lifecycle issues
- Uniform buffer synchronization
- Shader loading from files

We decided a compute shader approach would be:
1. Cleaner and easier to debug
2. More efficient (no rasterization overhead)
3. Better suited for this type of per-pixel computation

## New Approach

See `sandbox/msdf-wgsl/` for the compute shader-based implementation.
