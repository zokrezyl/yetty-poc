# Lottie Animation Support via SDF Rendering

This document evaluates implementing Lottie animation playback using yetty's existing SDF (Signed Distance Function) rendering infrastructure in `ydraw`/`hdraw`.

## Executive Summary

Lottie is a JSON-based animation format that describes vector graphics with keyframe animations. Our SDF rendering pipeline can handle the geometric primitives; the main work is building the animation infrastructure and format translation layer.

**Estimated coverage**: ~70-80% of typical Lottie animations with basic implementation.

---

## 1. Current SDF Infrastructure

### 1.1 Available Primitives (`SDFType` enum)

| Type | Parameters | Lottie Equivalent |
|------|------------|-------------------|
| `Circle` | center, radius | Ellipse (equal radii) |
| `Box` | center, halfSize, round | Rectangle |
| `Segment` | p0, p1 | Stroke path segment |
| `Triangle` | p0, p1, p2 | — |
| `Bezier2` | p0, p1, p2 (quadratic) | Path vertex |
| `Bezier3` | p0, p1, p2, p3 (cubic) | Path vertex |
| `Ellipse` | center, radii | Ellipse shape |
| `Arc` | center, sc, ra, rb | — |
| `RoundedBox` | center, halfSize, 4x radii | Rounded rectangle |
| `Star` | center, r, n, m | Polystar |
| `Pentagon/Hexagon` | center, r | Polystar (n=5,6) |
| `Heart/Moon/Egg/etc.` | various | — |

### 1.2 Primitive Properties

```cpp
struct SDFPrimitive {  // 96 bytes
    uint32_t type;           // SDFType
    uint32_t layer;          // Draw order
    float params[12];        // Shape parameters
    uint32_t fillColor;      // RGBA packed
    uint32_t strokeColor;    // RGBA packed
    float strokeWidth;
    float round;             // Corner rounding
    float aabbMinX, aabbMinY, aabbMaxX, aabbMaxY;  // Bounding box
};
```

### 1.3 Rendering Pipeline

1. **CPU**: Build primitives → spatial hash grid
2. **GPU**: Fragment shader evaluates SDFs via grid lookup
3. **Acceleration**: O(1) cell lookup, ~16 primitives per cell max

---

## 2. Lottie Format Overview

### 2.1 Structure

```json
{
  "v": "5.7.4",           // Version
  "fr": 30,               // Frame rate
  "ip": 0,                // In point (start frame)
  "op": 60,               // Out point (end frame)
  "w": 512, "h": 512,     // Dimensions
  "layers": [...]         // Layer array
}
```

### 2.2 Layer Types

| Type | Code | Description |
|------|------|-------------|
| Precomp | 0 | Nested composition |
| Solid | 1 | Solid color layer |
| Image | 2 | Bitmap image |
| Null | 3 | Null object (transform only) |
| **Shape** | 4 | Vector shapes ← primary target |
| Text | 5 | Text layer |

### 2.3 Shape Layer Contents

```json
{
  "ty": 4,              // Shape layer
  "shapes": [
    { "ty": "gr", ... },  // Group
    { "ty": "rc", ... },  // Rectangle
    { "ty": "el", ... },  // Ellipse
    { "ty": "sr", ... },  // Polystar
    { "ty": "sh", ... },  // Path (bezier)
    { "ty": "fl", ... },  // Fill
    { "ty": "st", ... },  // Stroke
    { "ty": "tr", ... }   // Transform
  ]
}
```

### 2.4 Animated Properties

Any property can be animated via keyframes:

```json
{
  "a": 1,              // Animated flag
  "k": [               // Keyframes
    {
      "t": 0,          // Time (frame)
      "s": [100],      // Start value
      "e": [200],      // End value (deprecated in newer versions)
      "i": {"x": 0.5, "y": 0},    // In tangent (bezier easing)
      "o": {"x": 0.5, "y": 1}     // Out tangent
    },
    { "t": 30, "s": [200] }
  ]
}
```

---

## 3. Feature Mapping & Implementation Options

### 3.1 ✅ Direct Mappings (Ready Now)

| Lottie | SDF | Notes |
|--------|-----|-------|
| Rectangle (`rc`) | `Box` / `RoundedBox` | Map `r` (roundness) to corner radii |
| Ellipse (`el`) | `Ellipse` / `Circle` | Circle when radii equal |
| Polystar (`sr`) | `Star` / `Pentagon` / `Hexagon` | Map point count |
| Fill (`fl`) | `fillColor` | Direct RGBA mapping |
| Stroke (`st`) | `strokeColor` + `strokeWidth` | Direct mapping |
| Layer order | `layer` field | Lower = rendered first |

### 3.2 ⚠️ Path Decomposition (Medium Effort)

Lottie paths are sequences of cubic bezier curves. Our system has individual `Bezier2`/`Bezier3` primitives.

**Challenge**: A single Lottie path may contain 10-50 bezier segments.

#### Option A: Multiple Primitives per Path
```
Lottie path with N vertices → N-1 Bezier3 primitives
```
- **Pros**: Works with current infrastructure
- **Cons**: Primitive count explosion, stroke joins need handling
- **Implementation**: Loop through path vertices, emit `Bezier3` for each segment

#### Option B: Path Primitive Type (New)
Add `SDFType::Path` that stores control points in extended buffer:
```cpp
struct PathPrimitive {
    uint32_t type = SDF_PATH;
    uint32_t pointCount;
    uint32_t pointsOffset;  // Offset into auxiliary buffer
    // ... colors, stroke
};
```
- **Pros**: Single primitive per path, better batching
- **Cons**: Requires shader changes, variable-length data

#### Option C: Path Texture
Rasterize paths to texture, sample in shader:
- **Pros**: Handles complex paths
- **Cons**: Loss of SDF benefits (infinite zoom), extra texture memory

**Recommendation**: Start with **Option A** for simplicity, consider **Option B** if performance becomes an issue.

### 3.3 ⚠️ Transform System (Medium Effort)

Lottie transforms include: position, anchor point, scale, rotation, skew.

#### Option A: CPU-Side Transform
Transform all coordinates on CPU before uploading:
```cpp
void applyTransform(SDFPrimitive& prim, const Transform& t) {
    for (int i = 0; i < paramCount; i += 2) {
        vec2 p = {prim.params[i], prim.params[i+1]};
        p = t.apply(p);  // Matrix multiply
        prim.params[i] = p.x;
        prim.params[i+1] = p.y;
    }
    // Recompute AABB
}
```
- **Pros**: No shader changes
- **Cons**: Must rebuild primitives every frame when animating transforms

#### Option B: Per-Primitive Transform Matrix
Extend `SDFPrimitive` with transform:
```cpp
struct SDFPrimitive {
    // ... existing fields ...
    float transform[6];  // 2D affine: [a, b, c, d, tx, ty]
};
```
Shader applies transform before SDF evaluation:
```wgsl
fn evaluateSDF(primOffset: u32, p: vec2<f32>) -> f32 {
    let t = getTransform(primOffset);
    let localP = applyInverseTransform(p, t);
    // ... evaluate SDF with localP
}
```
- **Pros**: Animate transforms without rebuilding geometry
- **Cons**: Increases primitive size (96 → 120 bytes), AABB computation more complex

#### Option C: Transform Hierarchy Buffer
Separate transform buffer, primitives reference transform index:
```cpp
struct SDFPrimitive {
    // ... existing ...
    uint32_t transformIndex;  // Index into transform buffer
};
```
- **Pros**: Transform inheritance (Lottie parenting), shared transforms
- **Cons**: Extra indirection in shader

**Recommendation**: **Option B** for simplicity, **Option C** if implementing full layer hierarchy.

### 3.4 ⚠️ Animation Timeline (Medium Effort)

Need to interpolate properties between keyframes.

#### Core Components

```cpp
struct Keyframe {
    float time;           // Frame number
    float value[4];       // Up to 4 components (e.g., position x,y)
    vec2 inTangent;       // Bezier easing handle
    vec2 outTangent;
};

struct AnimatedProperty {
    std::vector<Keyframe> keyframes;
    
    float evaluate(float time) const {
        // Find surrounding keyframes
        // Interpolate with bezier easing
    }
};

class LottieAnimation {
    float frameRate;
    float currentFrame;
    std::vector<AnimatedProperty> properties;
    
    void update(float deltaTime) {
        currentFrame += deltaTime * frameRate;
        // Wrap or clamp to duration
    }
    
    void apply(std::vector<SDFPrimitive>& primitives) {
        // Update primitive parameters from animated properties
    }
};
```

#### Bezier Easing Implementation

```cpp
float bezierEase(float t, vec2 p1, vec2 p2) {
    // Solve cubic bezier for x = t
    // Return corresponding y value
    // Use Newton-Raphson iteration
}

float interpolate(const Keyframe& k0, const Keyframe& k1, float t) {
    float normalizedT = (t - k0.time) / (k1.time - k0.time);
    float easedT = bezierEase(normalizedT, k0.outTangent, k1.inTangent);
    return lerp(k0.value, k1.value, easedT);
}
```

### 3.5 ⚠️ Opacity/Alpha (Low Effort)

Already supported via RGBA colors. Need to:
1. Parse Lottie opacity property (`o`)
2. Apply to fill/stroke alpha channel
3. Handle layer-level opacity (multiply with shape opacity)

```cpp
uint32_t applyOpacity(uint32_t color, float opacity) {
    uint8_t a = (color >> 24) & 0xFF;
    a = uint8_t(a * opacity);
    return (color & 0x00FFFFFF) | (a << 24);
}
```

### 3.6 ❌ Gradients (Medium-High Effort)

Lottie supports linear and radial gradients.

#### Option A: Gradient Texture
Pre-render gradient to 1D texture, sample in shader:
```wgsl
fn getGradientColor(primOffset: u32, p: vec2<f32>) -> vec4<f32> {
    let gradientType = ...; // Linear or radial
    let t = computeGradientT(p, gradientType, ...);
    return textureSample(gradientTexture, gradientSampler, vec2(t, gradientIndex));
}
```
- **Pros**: Flexible, handles complex color stops
- **Cons**: Extra texture, limited resolution

#### Option B: Analytical Gradient in Shader
Encode gradient parameters in primitive:
```cpp
struct GradientPrimitive {
    uint32_t type;        // SDF_GRADIENT_FILL
    uint32_t shapeIndex;  // Reference to shape primitive
    uint32_t gradientType; // Linear=0, Radial=1
    float startPoint[2];
    float endPoint[2];
    uint32_t colorStops[8]; // Up to 8 stops, packed
    float stopPositions[8];
};
```
- **Pros**: Resolution independent
- **Cons**: Limited stop count, complex shader code

**Recommendation**: **Option A** for initial implementation, supports arbitrary gradients.

### 3.7 ❌ Masks & Mattes (High Effort)

Lottie masks use alpha/luma from one layer to clip another.

#### Option A: SDF Boolean Operations
For simple masks, use SDF composition:
```wgsl
fn maskedSDF(p: vec2<f32>, shapeDist: f32, maskDist: f32) -> f32 {
    return max(shapeDist, maskDist);  // Intersection
}
```
- **Pros**: Works with existing SDF infrastructure
- **Cons**: Only works for simple convex masks

#### Option B: Stencil Buffer
Render mask to stencil, then render content:
- **Pros**: Accurate masking
- **Cons**: Multi-pass rendering, complexity

#### Option C: Render-to-Texture
Render masked content to intermediate texture:
- **Pros**: Full flexibility
- **Cons**: Memory overhead, compositing complexity

**Recommendation**: Start with **Option A** for simple cases, defer complex masks to future.

### 3.8 ❌ Effects (High Effort)

Lottie effects include: blur, drop shadow, glow, etc.

**Recommendation**: Defer to future. Most animations work without effects.

---

## 4. Proposed Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    LottiePlayer                         │
├─────────────────────────────────────────────────────────┤
│  - loadFromJSON(string)                                 │
│  - loadFromFile(path)                                   │
│  - play() / pause() / stop()                            │
│  - seek(frame)                                          │
│  - update(deltaTime)                                    │
│  - getDuration() / getCurrentFrame()                    │
└───────────────────────┬─────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────┐
│                  LottieDocument                         │
├─────────────────────────────────────────────────────────┤
│  - frameRate, duration, dimensions                      │
│  - layers: vector<LottieLayer>                          │
│  - assets: map<string, Asset>                           │
└───────────────────────┬─────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────┐
│                   LottieLayer                           │
├─────────────────────────────────────────────────────────┤
│  - type: LayerType                                      │
│  - transform: AnimatedTransform                         │
│  - shapes: vector<LottieShape>  (for shape layers)      │
│  - inPoint, outPoint                                    │
└───────────────────────┬─────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────┐
│                   LottieShape                           │
├─────────────────────────────────────────────────────────┤
│  - type: ShapeType (rect, ellipse, path, fill, etc.)    │
│  - properties: map<string, AnimatedProperty>            │
└───────────────────────┬─────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────┐
│              LottieToSDFConverter                       │
├─────────────────────────────────────────────────────────┤
│  - convert(LottieDocument, float frame) -> SDFScene     │
│  - evaluateTransforms(frame)                            │
│  - evaluateShapes(frame)                                │
│  - emitPrimitives()                                     │
└───────────────────────┬─────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────┐
│                 YDraw (existing)                        │
├─────────────────────────────────────────────────────────┤
│  - addPrimitive(SDFPrimitive)                           │
│  - render(time)                                         │
└─────────────────────────────────────────────────────────┘
```

---

## 5. Implementation Phases

### Phase 1: Static Lottie Rendering (1-2 weeks)

**Goal**: Render first frame of simple Lottie files.

- [ ] Lottie JSON parser (use nlohmann/json or rapidjson)
- [ ] Parse shape layers: rectangle, ellipse, polystar
- [ ] Parse fill and stroke
- [ ] Map to `SDFPrimitive` structs
- [ ] Test with simple static Lottie files

**Deliverable**: `LottieLoader::loadStatic(json) -> vector<SDFPrimitive>`

### Phase 2: Basic Animation (1-2 weeks)

**Goal**: Animate position, scale, rotation, opacity.

- [ ] Keyframe data structures
- [ ] Linear interpolation
- [ ] Bezier easing curves
- [ ] Animation timeline / playback control
- [ ] Transform application (CPU-side initially)

**Deliverable**: `LottiePlayer` with play/pause/seek

### Phase 3: Path Support (1 week)

**Goal**: Render bezier paths.

- [ ] Parse Lottie path vertices (`sh` type)
- [ ] Decompose into `Bezier3` primitives
- [ ] Handle closed vs open paths
- [ ] Stroke rendering for paths

**Deliverable**: Path animations working

### Phase 4: Transform Optimization (1 week)

**Goal**: GPU-side transforms for better performance.

- [ ] Extend `SDFPrimitive` with transform matrix
- [ ] Update shader to apply transforms
- [ ] Hierarchy support (parent transforms)

**Deliverable**: Smooth animation without CPU bottleneck

### Phase 5: Advanced Features (2+ weeks)

**Goal**: Broader Lottie compatibility.

- [ ] Gradient fills (texture-based)
- [ ] Simple masks (SDF intersection)
- [ ] Trim paths
- [ ] Repeaters
- [ ] Precompositions

---

## 6. Testing Strategy

### Test Files

1. **Static shapes**: Rectangle, ellipse, star only
2. **Simple animation**: Bouncing ball (position + scale)
3. **Easing test**: Various easing curves
4. **Path animation**: Morphing shapes
5. **Complex real-world**: LottieFiles.com samples

### Validation

- Visual comparison with reference renderer (lottie-web)
- Frame-by-frame diff testing
- Performance benchmarks (primitives/frame, GPU time)

---

## 7. Dependencies

### Required
- JSON parser: `nlohmann/json` (header-only) or existing YAML parser adaptation
- No external Lottie libraries needed

### Optional
- `rlottie` - Samsung's Lottie renderer (reference implementation)
- `lottie-web` - Browser reference for visual comparison

---

## 8. Limitations & Scope

### In Scope (Target Support)
- Shape layers with basic shapes
- Fill and stroke (solid colors)
- Position, scale, rotation animation
- Opacity animation
- Bezier paths
- Layer ordering

### Out of Scope (Initial Version)
- Text layers
- Image layers
- Effects (blur, shadow, glow)
- Complex masks/mattes
- Expressions (scripted animations)
- 3D layers
- Audio

### Known Limitations
- Path stroke joins may not match exactly (SDF limitation)
- Very complex paths may impact performance
- No sub-pixel antialiasing control

---

## 9. API Design

### Basic Usage

```cpp
#include <yetty/lottie/lottie-player.h>

// Load animation
auto player = LottiePlayer::create(ydrawCard);
player->loadFromFile("animation.json");

// In render loop
player->update(deltaTime);  // Advances animation
// YDraw automatically renders current frame
```

### Advanced Usage

```cpp
// Manual control
player->seek(30);           // Jump to frame 30
player->setSpeed(2.0f);     // 2x playback speed
player->setLoop(true);      // Loop animation

// Query state
float progress = player->getProgress();  // 0.0 - 1.0
bool playing = player->isPlaying();

// Events
player->onComplete([](){ /* animation finished */ });
player->onLoop([](int count){ /* loop count */ });
```

---

## 10. References

- [Lottie Documentation](https://lottiefiles.github.io/lottie-docs/)
- [Lottie JSON Schema](https://lottiefiles.github.io/lottie-docs/schema/)
- [Inigo Quilez 2D SDF Functions](https://iquilezles.org/articles/distfunctions2d/)
- [rlottie](https://github.com/Samsung/rlottie) - Reference implementation
- [lottie-web](https://github.com/airbnb/lottie-web) - Browser renderer
