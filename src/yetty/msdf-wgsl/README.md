# MSDF-WGSL

GPU-based MSDF (Multi-channel Signed Distance Field) font atlas generation using WebGPU compute shaders.

## Status: Work in Progress

The basic pipeline works but there are **minor artifacts on most glyphs**.

### What Works

- WebGPU compute shader pipeline for MSDF generation
- FreeType glyph outline decomposition (line segments + quadratic beziers)
- Glyph serialization to GPU buffers (points, metadata)
- Atlas texture generation and readback
- CDB file export for use with yetty's msdf-font-cache
- Render-atlas viewer tool for debugging

### Letters That Render Correctly

Only glyphs composed primarily of straight lines work well:
- Capital: I, L, T, U, Z
- Lowercase: f, h, l, m, n, t, u, v

### Letters With Artifacts

Most letters with curves have issues:
- A, B, C, D, G, J, O, P, Q, R, S (and lowercase equivalents)
- Basically anything with quadratic bezier curves or complex contours

## Suspected Issues

### 1. Quadratic Bezier Distance Calculation

The `distance_to_quad()` function in `shaders/msdf_gen.wgsl` solves a cubic equation to find the closest point on the curve. Potential issues:

- **Cubic solver edge cases**: The depressed cubic solver may have numerical precision issues near discriminant boundaries
- **Multiple roots handling**: When multiple valid roots exist, the selection logic may pick the wrong one
- **Tangent calculation**: Using `ab + br * t` for sign determination at parameter t

### 2. Corner/Junction Handling

Where two segments meet (line-to-curve or curve-to-curve), there may be sign discontinuities. Standard MSDF implementations use "pseudo-distance" at corners to smooth transitions.

### 3. Endpoint Behavior

When the closest point is at a segment endpoint (t=0 or t=1), the cross-product sign determination uses a vector that may not correctly indicate inside/outside.

### 4. Winding Direction

The `winding` variable is computed per-contour but currently NOT used. However, attempting to apply it (`signed_dist * winding`) made things worse, suggesting the sign convention in the distance functions may already account for this implicitly, or the winding values in metadata are incorrect.

## Architecture

```
msdf-wgsl/
├── src/
│   └── msdf-wgsl.cpp      # Main library - FreeType integration, GPU pipeline
├── shaders/
│   └── msdf_gen.wgsl      # Compute shader - distance calculations
├── gen-cdb/
│   └── main.cpp           # CLI tool to generate CDB files
├── render-atlas/
│   └── main.cpp           # Viewer tool for debugging atlas
└── README.md
```

## Key Functions in Shader

### `distance_to_line(p0, p1, origin) -> vec3<f32>`
Returns (signed_distance, orthogonality, parameter_t)

### `distance_to_quad(p0, p1, p2, origin) -> vec3<f32>`
Solves cubic equation for closest point on quadratic bezier.
Returns (signed_distance, orthogonality, parameter_t)

### Sign Convention
- Negative distance = inside the glyph
- Positive distance = outside the glyph
- Sign determined by cross product of edge tangent with vector to origin

## Next Steps to Debug

1. **Isolate the bezier issue**: Create test glyphs with known simple curves and verify the math

2. **Visualize raw distances**: Modify viewer to show R/G/B channels separately to see which channel has artifacts

3. **Compare with reference**: Generate same glyphs with msdfgen and compare pixel values

4. **Add debug output**: Print intermediate values for a specific pixel to trace the calculation

5. **Review cubic solver**: The `solve_cubic_depressed()` function handles three cases based on discriminant - verify each branch

## Building

```bash
cmake --build build-desktop-dawn-debug --target msdf-gen-cdb
cmake --build build-desktop-dawn-debug --target msdf-render-atlas
```

## Usage

```bash
cd build-desktop-dawn-debug/sandbox/msdf-wgsl

# Generate CDB file
./gen-cdb/msdf-gen-cdb --font /path/to/font.ttf --output output.cdb

# View atlas
./render-atlas/msdf-render-atlas output.cdb
```

Controls in viewer:
- Mouse wheel: Zoom (centered on cursor)
- Left drag: Pan
- Window resize supported
