# Compute Shader Image Scaling - Optimization Potential

## Current Implementation

The image atlas system stores images at their **original resolution**:

```
Source Image          Compute Shader         Atlas Texture
(4K JPEG)             (copy only)            (2048x2048)
3840x2160 ─────────► copy pixels ─────────► stores 3840x2160
                      no scaling              (8.3M pixels!)
```

The fragment shader then scales down during rendering to fit the terminal cell area.

### Problems with Current Approach

1. **Atlas space waste**: A 4K image displayed in 20x14 cells (~200x140 screen pixels) still consumes 8.3M pixels in the atlas
2. **Limited capacity**: 2048x2048 atlas = 4M pixels total, can't even fit one 4K image
3. **Redundant work**: Fragment shader downscales every frame

## Proposed Optimization

Use a compute shader to scale images to their **display resolution** before storing in the atlas:

```
Source Image          Compute Shader         Atlas Texture
(4K JPEG)             (scale + filter)       (2048x2048)
3840x2160 ─────────► Lanczos downscale ───► stores 200x140
                                             (28K pixels)
                                             297x smaller!
```

### Benefits

| Metric | Current | Optimized |
|--------|---------|-----------|
| 4K image in atlas | 8.3M pixels | 28K pixels |
| Images per atlas | <1 | ~140 |
| Fragment shader work | Scale every frame | Direct sample |
| Quality | Bilinear only | Lanczos/bicubic |

### Quality Advantage

GPU texture sampling in fragment shaders only supports **bilinear filtering** (4 pixels). High-quality downscaling requires **Lanczos** or **bicubic** filters that sample 16+ pixels with proper weighting.

```
Bilinear (current):     Lanczos (proposed):
    ┌───┬───┐               ┌───┬───┬───┬───┐
    │ x │ x │               │ x │ x │ x │ x │
    ├───┼───┤               ├───┼───┼───┼───┤
    │ x │ x │               │ x │ x │ x │ x │
    └───┴───┘               ├───┼───┼───┼───┤
    4 samples               │ x │ x │ x │ x │
    (blocky artifacts)      ├───┼───┼───┼───┤
                            │ x │ x │ x │ x │
                            └───┴───┴───┴───┘
                            16 samples
                            (smooth result)
```

### Implementation Sketch

```wgsl
// Lanczos weight function
fn lanczos(x: f32, a: f32) -> f32 {
    if (x == 0.0) { return 1.0; }
    if (abs(x) >= a) { return 0.0; }
    let pi_x = 3.14159265 * x;
    return (a * sin(pi_x) * sin(pi_x / a)) / (pi_x * pi_x);
}

@compute @workgroup_size(16, 16, 1)
fn scaleImage(@builtin(global_invocation_id) gid: vec3<u32>) {
    let dstX = gid.x;
    let dstY = gid.y;

    if (dstX >= dstWidth || dstY >= dstHeight) { return; }

    // Map destination to source coordinates
    let scaleX = f32(srcWidth) / f32(dstWidth);
    let scaleY = f32(srcHeight) / f32(dstHeight);
    let srcX = (f32(dstX) + 0.5) * scaleX;
    let srcY = (f32(dstY) + 0.5) * scaleY;

    // Lanczos resampling (a=2 for 4x4 kernel)
    var color = vec4(0.0);
    var weightSum = 0.0;

    for (var ky = -2; ky <= 2; ky++) {
        for (var kx = -2; kx <= 2; kx++) {
            let sx = i32(srcX) + kx;
            let sy = i32(srcY) + ky;

            if (sx >= 0 && sx < srcWidth && sy >= 0 && sy < srcHeight) {
                let wx = lanczos(srcX - f32(sx), 2.0);
                let wy = lanczos(srcY - f32(sy), 2.0);
                let weight = wx * wy;

                color += sampleSourceImage(sx, sy) * weight;
                weightSum += weight;
            }
        }
    }

    color /= weightSum;
    textureStore(atlasTexture, vec2(atlasX + dstX, atlasY + dstY), color);
}
```

## GPU-Rendered Content

For content rendered by GPU (plots, vector graphics, 3D), scaling is **not needed** because there's no fixed source resolution:

```
Plot Data              GPU Render             Atlas
(vertices)             (rasterize)
mathematical ────────► render at 200x140 ───► stores 200x140
                       directly               (exact fit)
```

Examples:
- **fastplotlib**: Renders plots at requested resolution
- **ThorVG**: Rasterizes SVG at target size
- **PlotCard**: Currently renders procedurally in fragment shader (no atlas)

These already produce output at the exact target size - no scaling overhead.

## Migration Path

1. **Phase 1**: Add optional compute shader scaling for ImageCard
   - Flag to enable/disable
   - Calculate target size from cell dimensions × pixel density

2. **Phase 2**: Integrate external GPU renderers
   - Render directly to atlas region at target size
   - No intermediate buffers needed

3. **Phase 3**: Dynamic resolution adjustment
   - Reduce stored resolution when atlas is full
   - Increase resolution for zoomed/focused images

## Performance Comparison

| Operation | CPU (Lanczos) | GPU Compute |
|-----------|---------------|-------------|
| 4K → 200x140 | ~80ms | ~0.3ms |
| Memory bandwidth | ~50 GB/s | ~500 GB/s |
| Parallelism | 8-16 cores | 1000s threads |

The GPU is ~250x faster for this workload, making real-time scaling practical even for large images.
