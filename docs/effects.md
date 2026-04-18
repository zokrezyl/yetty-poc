# Yetty Shader Effects

Yetty supports real-time GPU shader effects that transform the terminal display. Effects are written in WGSL and are hot-loaded at startup from the `shaders/pre-effects/` and `shaders/post-effects/` directories.

## Effect Types

| Type | When Applied | What It Modifies |
|------|--------------|------------------|
| **Pre-effect** | Before glyph rendering | Glyph index (which character is displayed) |
| **Post-effect** | After all rendering | Final pixel color |

```
┌─────────────────────────────────────────────────────────────────┐
│                      Rendering Pipeline                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│   Cell Buffer ──▶ PRE-EFFECT ──▶ Glyph Rendering ──▶ POST-EFFECT ──▶ Display
│                   (modify glyph)   (MSDF/bitmap)    (modify color)
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

## Enabling Effects via OSC Sequences

Effects are controlled via OSC escape sequences:

| OSC Code | Purpose | Format |
|----------|---------|--------|
| `666667` | Pre-effect | `ESC ] 666667 ; index : p0 : p1 : ... BEL` |
| `666668` | Post-effect | `ESC ] 666668 ; index : p0 : p1 : ... BEL` |

**Enable an effect:**
```bash
# Post-effect: CRT monitor (index 2) with parameters
printf '\e]666668;2:0.4:0.15:0.3\a'

# Pre-effect: scramble (index 1) with intensity 0.3
printf '\e]666667;1:0.3:1.0\a'
```

**Disable an effect:**
```bash
# Disable post-effect (index 0)
printf '\e]666668;0\a'

# Disable pre-effect
printf '\e]666667;0\a'
```

## Built-in Pre-Effects

Pre-effects modify the glyph index before rendering, creating text manipulation effects.

| Index | Name | Parameters | Description |
|-------|------|------------|-------------|
| 1 | **scramble** | `intensity, speed` | Randomly swaps characters for glitchy text |
| 2 | **rain** | `density, speed` | Replaces characters with falling rain drops |
| 3 | **wave** | `amplitude, frequency, speed` | Wavy text distortion |
| 4 | **matrix** | `density, speed, column_height` | Matrix-style falling characters |

### Pre-Effect Function Signature

```wgsl
fn preEffect_name(
    glyphIndex: u32,           // Current glyph to render
    cellCol: f32,              // Cell column (0-based)
    cellRow: f32,              // Cell row (0-based)
    time: f32,                 // Animation time in seconds
    params: array<f32, 6>      // User parameters (p0-p5)
) -> u32;                      // Modified glyph index
```

### Example: Scramble Effect

```wgsl
// Pre-effect: scramble
// Index: 1
fn preEffect_scramble(
    glyphIndex: u32,
    cellCol: f32,
    cellRow: f32,
    time: f32,
    params: array<f32, 6>
) -> u32 {
    let intensity = select(0.3, params[0], params[0] > 0.0);
    let speed = select(1.0, params[1], params[1] > 0.0);

    // Hash based on cell position and time
    let t = floor(time * speed * 8.0);
    let h = fract(sin(cellCol * 127.1 + cellRow * 311.7 + t * 758.5) * 43758.5453);

    // Only affect a fraction of cells based on intensity
    if (h < intensity && glyphIndex > 32u && glyphIndex < 127u) {
        let newGlyph = u32(fract(sin(h * 999.9 + t) * 43758.5453) * 94.0) + 33u;
        return newGlyph;
    }
    return glyphIndex;
}
```

## Built-in Post-Effects

Post-effects modify the final pixel color after all rendering is complete.

| Index | Name | Parameters | Description |
|-------|------|------------|-------------|
| 1 | **scanlines** | `intensity, line_width` | Classic CRT scanline darkening |
| 2 | **crt** | `vignette, scanline, color_bleed` | Full CRT monitor simulation |
| 3 | **chromatic** | `amount` | RGB color channel separation |
| 4 | **broken-tv** | `intensity, noise, roll_speed` | Damaged TV static and roll |
| 5 | **matrix** | `intensity, speed` | Green Matrix-style color tint |
| 6 | **sepia** | `intensity, contrast` | Vintage sepia tone |
| 7 | **pixelate** | `pixel_width, pixel_height` | Mosaic/pixelation effect |
| 8 | **wave** | `amplitude, frequency, speed` | Screen wave distortion |
| 9 | **invert** | `intensity` | Color inversion |
| 10 | **nightvision** | `intensity, noise, vignette` | Green night vision goggles |
| 11 | **vaporwave** | `intensity, shift` | Retro vaporwave color palette |
| 12 | **thermal** | `intensity` | Thermal camera heat map |
| 13 | **glitch** | `intensity, block_size, speed` | Digital glitch artifacts |
| 14 | **emboss** | `intensity, angle` | Embossed/raised text effect |
| 15 | **rain** | `density, speed, opacity` | Rain droplet overlay |
| 16 | **matrix-rain** | `density, speed, size, trail, glow` | MSDF-rendered Matrix rain |
| 17 | **drip** | `intensity, speed, length, glyph_mode` | Color/glyph dripping streaks |

### Post-Effect Function Signature

```wgsl
fn postEffect_name(
    color: vec3<f32>,          // Current pixel color (RGB, 0-1)
    pixelPos: vec2<f32>,       // Pixel position on screen
    screenSize: vec2<f32>,     // Screen dimensions in pixels
    time: f32,                 // Animation time in seconds
    params: array<f32, 6>      // User parameters (p0-p5)
) -> vec3<f32>;                // Modified pixel color
```

### Example: CRT Effect

```wgsl
// Post-effect: CRT monitor
// Index: 2
fn postEffect_crt(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let vignetteStr = select(0.4, params[0], params[0] > 0.0);
    let scanlineStr = select(0.15, params[1], params[1] > 0.0);
    let colorBleed = select(0.3, params[2], params[2] > 0.0);

    let uv = pixelPos / screenSize;
    var result = color;

    // Vignette - darken edges
    let center = vec2<f32>(0.5, 0.5);
    let dist = length(uv - center) * 1.4;
    let vignette = 1.0 - vignetteStr * dist * dist;
    result *= vignette;

    // Scanlines
    let scanline = sin(pixelPos.y * 3.14159) * 0.5 + 0.5;
    result *= 1.0 - scanlineStr * (1.0 - scanline);

    // Subtle brightness flicker
    let flicker = 1.0 - 0.01 * sin(time * 8.0);
    result *= flicker;

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
```

## Architecture

### ShaderManager

The `ShaderManager` handles effect loading and shader compilation:

1. **Startup**: Scans `shaders/pre-effects/` and `shaders/post-effects/` directories
2. **Parsing**: Extracts index from filename (`N-name.wgsl`) or `// Index: N` comment
3. **Injection**: Inserts effect functions into base shader at placeholders
4. **Dispatch**: Generates runtime dispatch code based on `grid.preEffectIndex` / `grid.postEffectIndex`

### File Naming Convention

Effect files follow the pattern: `{index}-{name}.wgsl`

```
src/yetty/shaders/
├── pre-effects/
│   ├── 1-scramble.wgsl
│   ├── 2-rain.wgsl
│   ├── 3-wave.wgsl
│   └── 4-matrix.wgsl
└── post-effects/
    ├── 1-scanlines.wgsl
    ├── 2-crt.wgsl
    ├── 3-chromatic.wgsl
    └── ...
```

### Shader Placeholders

The base shader (`gpu-screen.wgsl`) contains placeholders that are replaced at compile time:

```wgsl
// PRE_EFFECT_FUNCTIONS_PLACEHOLDER   → All pre-effect function code
// PRE_EFFECT_APPLY_PLACEHOLDER       → Dispatch: if (index == N) { ... }
// POST_EFFECT_FUNCTIONS_PLACEHOLDER  → All post-effect function code
// POST_EFFECT_APPLY_PLACEHOLDER      → Dispatch: if (index == N) { ... }
```

### Effect Parameters

Effects receive up to 6 float parameters via the grid uniform:

```wgsl
struct GridUniforms {
    // ...
    preEffectIndex: u32,
    preEffectP0: f32,
    preEffectP1: f32,
    preEffectP2: f32,
    preEffectP3: f32,
    preEffectP4: f32,
    preEffectP5: f32,
    postEffectIndex: u32,
    postEffectP0: f32,
    postEffectP1: f32,
    postEffectP2: f32,
    postEffectP3: f32,
    postEffectP4: f32,
    postEffectP5: f32,
};
```

## Creating Custom Effects

### 1. Create the shader file

Create a new `.wgsl` file in the appropriate directory:

```wgsl
// Post-effect: my_effect
// Index: 20
//
// Description of what the effect does.
// params[0] = first parameter (default value)
// params[1] = second parameter (default value)

fn postEffect_my_effect(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let param0 = select(0.5, params[0], params[0] > 0.0);

    // Your effect logic here
    var result = color;

    return result;
}
```

### 2. Name the file correctly

Save as `src/yetty/shaders/post-effects/20-my_effect.wgsl`

### 3. Restart Yetty

Effects are loaded at startup. Restart Yetty to load the new effect.

### 4. Enable the effect

```bash
printf '\e]666668;20:0.5\a'
```

## Demo Script

Run the built-in effects demo:

```bash
source demo/scripts/effects-demo.sh
```

This cycles through all pre and post effects with example parameters.

## Combining Effects

You can enable one pre-effect and one post-effect simultaneously:

```bash
# Matrix characters + Matrix green tint
printf '\e]666667;4:0.4:1.5:12.0\a'
printf '\e]666668;5:0.8:2.0\a'

# Disable both
printf '\e]666667;0\a'
printf '\e]666668;0\a'
```

## Key Source Files

| File | Purpose |
|------|---------|
| `include/yetty/shader-manager.h` | ShaderManager interface |
| `src/yetty/shader-manager.cpp` | Effect loading and shader compilation |
| `src/yetty/shaders/gpu-screen.wgsl` | Base shader with placeholders |
| `src/yetty/shaders/pre-effects/*.wgsl` | Pre-effect implementations |
| `src/yetty/shaders/post-effects/*.wgsl` | Post-effect implementations |
| `demo/scripts/effects-demo.sh` | Demo script for all effects |
