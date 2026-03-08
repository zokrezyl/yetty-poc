# ThorVG to YDraw Rendering Evaluation

## Executive Summary

**Feasibility: YES** - ThorVG can be patched to render to YDraw, but with significant constraints. YDraw is an SDF-based GPU renderer with a fixed primitive vocabulary, while ThorVG is a general-purpose vector graphics library supporting arbitrary bezier paths. The integration requires a translation layer that converts ThorVG's path-based shapes to YDraw's SDF primitives.

## System Analysis

### YDraw Architecture

YDraw is a GPU-accelerated SDF (Signed Distance Field) renderer with:

**Primitives supported** (from `ydraw-primitives.yaml`):
- Circle, Ellipse
- Box, RoundedBox
- Segment (line)
- Triangle
- Bezier2 (quadratic bezier)
- Bezier3 (cubic bezier)
- Arc, Pie, Ring
- Polygon shapes: Pentagon, Hexagon, Rhombus, Star, Heart, Cross
- ColorWheel (specialized UI element)

**Data flow:**
```
YDrawBuffer (pure data container)
    ↓ addCircle(), addBox(), addBezier3(), etc.
YDrawBuilder (computes AABBs, spatial grid)
    ↓ calculate() → writes GPU buffers
GPU shader (O(1) grid lookup + SDF evaluation)
```

**Key characteristics:**
- Fixed primitive types - no arbitrary polygon support
- Per-primitive fill/stroke colors
- Spatial hash grid for O(1) pixel→primitive lookup
- Renders via SDF evaluation (smooth antialiasing, resolution-independent)

### ThorVG Architecture

ThorVG is a full-featured vector graphics library with:

**Paint types:**
- Shape (paths with fill/stroke)
- Scene (group of paints)
- Picture (image/SVG container)
- Text

**Shape path commands:**
- MoveTo, LineTo, CubicTo, Close
- appendRect(), appendCircle() (convenience)
- appendPath() (raw commands)

**Rendering backends:**
- `SwRenderer` - software rasterizer (scan-line)
- `GlRenderer` - OpenGL ES
- `WgRenderer` - WebGPU

**Renderer interface (`RenderMethod`):**
```cpp
prepare(RenderShape, transform, clips, opacity, flags) → RenderData
renderShape(RenderData)
renderImage(RenderData)
```

## Integration Approaches

### Approach 1: YDrawRenderer (New ThorVG Backend)

Create a new `YDrawRenderer : RenderMethod` that translates ThorVG shapes to YDraw primitives.

**Pros:**
- Clean integration with ThorVG's existing API
- Supports all ThorVG features (transforms, opacity, scenes)
- Works with existing SVG/Lottie loaders

**Cons:**
- Complex translation layer needed
- Not all ThorVG features map cleanly to YDraw

**Implementation sketch:**
```cpp
class YDrawRenderer : public RenderMethod {
    YDrawBuffer::Ptr _buffer;
    YDrawBuilder::Ptr _builder;
    
    RenderData prepare(const RenderShape& shape, ...) override {
        // Translate shape.path → YDraw primitives
        PathToYDraw translator(_buffer, shape);
        translator.convert();
        return handle;
    }
    
    bool postRender() override {
        _builder->clear();
        _builder->addYdrawBuffer(_buffer);
        // trigger GPU upload
    }
};
```

### Approach 2: Shape Decomposition Library

Standalone converter that takes ThorVG scenes/shapes and emits YDraw primitives.

**Pros:**
- Simpler, doesn't require modifying ThorVG
- Can be used offline (pre-process SVGs)

**Cons:**
- No real-time animation support without custom plumbing
- Loses ThorVG's scene graph management

### Approach 3: Hybrid Tessellation

For complex paths that don't map to YDraw primitives, tessellate to triangles.

**Pros:**
- Handles arbitrary paths
- YDraw already has Triangle primitive

**Cons:**
- Loses SDF benefits (smooth edges, infinite zoom)
- Triangle explosion for smooth curves

## Path-to-Primitive Mapping

| ThorVG Shape | YDraw Primitive | Notes |
|--------------|-----------------|-------|
| appendRect(x,y,w,h,rx,ry) | RoundedBox | Direct mapping ✓ |
| appendCircle(cx,cy,rx,ry) where rx==ry | Circle | Direct mapping ✓ |
| appendCircle(cx,cy,rx,ry) where rx!=ry | Ellipse | Direct mapping ✓ |
| LineTo segment | Segment | Direct mapping ✓ |
| CubicTo | Bezier3 | Direct mapping ✓ |
| Closed polygon (N points) | Polygon → N-2 Triangles | Ear-clipping triangulation ✓ |
| Polygon with holes | PolygonGroup → Triangles | Bridge merging + triangulation ✓ |
| Arc | Arc | Needs parameter conversion |
| Complex path | Multiple primitives | Path decomposition |

## Challenges

### 1. Fill Rules
- ThorVG: EvenOdd, NonZero
- YDraw: Implicit (SDF inside = fill)
- **Solution:** For complex fills, decompose to non-overlapping primitives

### 2. Gradients
- ThorVG: LinearGradient, RadialGradient fills
- YDraw: Solid colors only
- **Solution:** Not supported initially; could add gradient types to YDraw

### 3. Masking & Composition
- ThorVG: Alpha/Luma masks, blend modes
- YDraw: Layer-based z-ordering only
- **Solution:** Limited support; complex masks require multi-pass

### 4. Strokes
- ThorVG: Variable width, dash patterns, caps, joins
- YDraw: Fixed strokeWidth per primitive, no dash/cap support
- **Solution:** Convert strokes to filled shapes (stroke-to-path)

### 5. Path Complexity
- ThorVG: Arbitrary paths with holes
- YDraw: Simple primitives only
- **Solution:** Decompose complex paths to bezier segments + triangulation

## Recommended Implementation

### Phase 1: Basic Shape Support
1. Create `YDrawRenderer` class implementing `RenderMethod`
2. Map simple shapes: rect, circle, ellipse, line
3. Basic fill colors and transforms

### Phase 2: Bezier Path Support
1. Walk path commands and emit Bezier2/Bezier3 primitives
2. Handle closed paths by connecting segments
3. Stroke rendering via strokeWidth parameter

### Phase 3: Complex Paths
1. Implement path tessellation for unsupported shapes
2. Triangulate fills when needed
3. Stroke-to-path conversion for styled strokes

### Phase 4: Advanced Features (Optional)
1. Add gradient support to YDraw primitives
2. Implement compositing layers
3. Text rendering via YDraw's glyph system

## Code Location in ThorVG

New files needed:
```
tmp/thorvg/src/renderer/ydraw_engine/
├── tvgYDrawRenderer.h
├── tvgYDrawRenderer.cpp
├── tvgYDrawPathConverter.h
├── tvgYDrawPathConverter.cpp
└── meson.build
```

## Effort Estimate

| Phase | Complexity | Description |
|-------|------------|-------------|
| Phase 1 | Medium | Basic renderer + simple shapes |
| Phase 2 | Medium | Bezier paths + strokes |
| Phase 3 | High | Tessellation + complex paths |
| Phase 4 | High | Gradients + composition |

## Conclusion

Patching ThorVG to render to YDraw is **feasible** and would enable GPU-accelerated SVG/Lottie rendering through YDraw's SDF system. The main work is building a path-to-primitive translator. Simple shapes (rect, circle, beziers) map directly; complex paths require tessellation to triangles.

**Recommended starting point:** Implement `YDrawRenderer` with basic shape support, then iterate.

## Progress

### Implemented ✓

| Feature | Implementation | Notes |
|---------|----------------|-------|
| **Circle/Ellipse** | `tryRenderAsEllipse()` | Detects MoveTo+4×CubicTo+Close pattern → `addEllipse()` |
| **Rectangle/Box** | `tryRenderAsBox()` | Detects MoveTo+LineTo/CubicTo+Close pattern → `addBox()` |
| **Cubic Bezier** | `addBezier3()` | Direct mapping for stroke curves |
| **Line Segment** | `addSegment()` | Direct mapping for LineTo commands |
| **Basic fill colors** | RGBA→ABGR conversion | Solid fills working |
| **Transform accumulation** | `computeWorldTransform()` | Walks parent chain for correct world transforms |
| **Lottie animation** | `setFrame()` + re-render | Frame updates working |
| **SVG static rendering** | Scene graph traversal | Via ThorVG's Accessor API |

### Not Yet Implemented

| Feature | Status | Difficulty | Notes |
|---------|--------|------------|-------|
| **Stroke caps/joins** | ❌ | Medium | Round, bevel, miter not supported |
| **Variable stroke width** | ❌ | Medium | YDraw has fixed strokeWidth |
| **Fill rules** (EvenOdd/NonZero) | ❌ | High | Complex path decomposition needed |
| **Alpha masks** | ❌ | High | Multi-pass rendering needed |
| **Blend modes** | ❌ | High | Not in YDraw primitive model |
| **Text rendering** | ❌ | Medium | ThorVG Text has no getters for content |
| **Images/Pictures** | ❌ | Medium | YDraw has Image type, needs atlas coords |
| **Multi-stop gradients** | ❌ | Medium | Currently only 2-stop gradients supported |

### Recently Implemented

| Feature | Implementation | Notes |
|---------|----------------|-------|
| **Arbitrary polygons** | `tryRenderAsPolygon()` → `addPolygonWithVertices()` | Closed paths without curves become Polygon primitives |
| **Polygons with holes** | `PolygonGroup` + triangulation | Ear-clipping with bridge merging for holes |
| **Multiple holes** | Hole sorting + sequential merge | Holes sorted by rightmost x-coordinate |
| **Axis-aligned rect detection** | `tryRenderAsBox()` validation | Verifies 2 unique X/Y values to avoid misdetecting pentagons |
| **Rounded rectangle corner radius** | `tryRenderAsBox()` cubic analysis | Extracts corner radius from bezier control points |
| **Stroke dash patterns** | `renderDashedSegment()` | Breaks strokes into dash/gap segments for LineTo |
| **Paint opacity** | `shape->opacity()` applied to alpha | Multiplies fill/stroke alpha by paint opacity |
| **Linear gradients** | `LinearGradientBox`, `LinearGradientCircle` | 2-color gradient fills for rectangles |
| **Radial gradients** | `RadialGradientCircle` | 2-color radial gradient from center to edge |

### Files Created

```
src/yetty/thorvg/
├── thorvg-renderer.h      # ThorVgRenderer interface (Object pattern)
├── thorvg-renderer.cpp    # Scene traversal + primitive emission
└── CMakeLists.txt         # yetty_thorvg library

src/yetty/cards/thorvg/
├── thorvg.h               # ThorVG card wrapper
├── thorvg.cpp             # YDrawBuilder + animation integration
└── CMakeLists.txt         # yetty_card_thorvg library

src/yetty/ydraw/
├── triangulate.h          # Triangulation API
└── triangulate.cpp        # Ear-clipping + hole merging

build-tools/cmake/
└── thorvg.cmake           # CPM download of ThorVG v1.0.1

test/ut/thorvg/
├── CMakeLists.txt         # ThorVG test build config
├── main.cpp               # Test entry point
├── thorvg_test.cpp        # ThorVG rendering tests (14 tests)
└── thorvg_rounded_rect_test.cpp  # Rounded rectangle tests (7 tests)

test/ut/ydraw/
└── triangulate_test.cpp   # Triangulation tests (51 tests)

demo/scripts/cards/thorvg/
├── simple-shapes.sh       # SVG shapes demo
├── beziers.sh             # Bezier curves demo
├── polygon-shapes.sh      # Polygon shapes demo
├── lottie-bounce.sh       # Bouncing ball animation
├── lottie-spinner.sh      # Spinning loader animation
└── all.sh                 # Run all demos

demo/scripts/cards/ydraw/
├── ydraw-polygon.sh       # Simple polygon demo
└── ydraw-polygon-holes.sh # Polygon with holes demo
```

### Key Technical Decisions

1. **Shape Detection over Path Walking**: For filled shapes (ellipse, rectangle), we detect the path command pattern and emit a single YDraw primitive rather than individual bezier strokes. This enables proper SDF fill rendering.

2. **Transform Accumulation**: ThorVG's `Paint::parent()` is used to walk the hierarchy and accumulate transforms from root to leaf, ensuring correct world-space coordinates.

3. **Animation via Re-render**: Each frame, `setFrame()` updates ThorVG's animation state, then `render()` clears and re-populates the YDrawBuffer with new primitive positions.

---

## Current Status & Known Issues

### Working Demos ✅

| Demo | Script | Description | Status |
|------|--------|-------------|--------|
| YDraw circles | `demo/scripts/cards/ydraw/circles.sh` | Pure YDraw Circle primitives | **WORKING** |
| YDraw ellipses | `demo/scripts/cards/ydraw/ellipses.sh` | Pure YDraw Ellipse primitives | **WORKING** (minor artifacts) |
| YDraw gradients | `demo/scripts/cards/ydraw/gradients.sh` | Linear/Radial gradients | **WORKING** |
| ThorVG simple shapes | `demo/scripts/cards/thorvg/simple-shapes.sh` | SVG rectangles, paths | **PARTIALLY WORKING** |
| ThorVG beziers | `demo/scripts/cards/thorvg/beziers.sh` | Cubic bezier strokes | **WORKING** |
| ThorVG polygons | `demo/scripts/cards/thorvg/polygon-shapes.sh` | Polygon triangulation | **WORKING** |

### Broken Demos ❌

| Demo | Script | Issue | Investigation Status |
|------|--------|-------|---------------------|
| ThorVG opacity | `demo/scripts/cards/thorvg/opacity.sh` | **Ellipses/circles not rendering** | See debugging notes below |

### Debugging Notes: ThorVG Ellipse Rendering Issue

**Problem:** SVG circles rendered via ThorVG are not visible, while identical YDraw ellipses work correctly.

**What we confirmed works:**
- ThorVG correctly parses SVG circles and converts them to 4×CubicTo+Close paths
- `tryRenderAsEllipse()` correctly detects the pattern and creates Ellipse primitives (type=6)
- Debug output shows 7 primitives created: 4 boxes (type=1) + 3 ellipses (type=6)
- Ellipse data is correct: cx=160, cy=60, rx=40, ry=40, fillColor=0x990000ff (blue with 60% opacity)
- Grid calculation is correct: 2x2 grid, ellipses assigned to correct cells
- GPU buffer allocation succeeds: primHandle.offset=0, derivedHandle at offset 80
- Grid index-to-offset translation works: prim indices [4,5,6] → word offsets [40,50,60]
- Metadata is flushed correctly: primOff=0, primCnt=7, gridOff=80

**What's different from working YDraw ellipses:**
- Both use same shader (0x100003)
- Both use same YDrawBuilder and buffer system  
- Both set FLAG_UNIFORM_SCALE
- Both have similar scene bounds and grid configurations

**Suspected issues (not yet verified):**
1. Scene position computation in shader may differ due to view/zoom calculation
2. Potential issue with how widgetUV maps to scenePos for the ThorVG card
3. Possible timing issue - buffer written but not flushed to GPU before render

**Debug output sample:**
```
Ellipse: cx=160 cy=60 rx=40 ry=40 fill=0x990000ff stroke=0x0
ThorVgRenderer::render: prims=7
YDrawBuilder::calculate: grid=2x2 scene=[0,0]-[200,200] prims=7
ThorVG::allocateBuffers: prims=7
writeGPU: prim[4] id=2147483652 type=6 words=10 dataOff=40
writeDerived: gridSize=22 gridOffset=80 primWordOffsets.size=7
  cell[1]: packedOff=9 cnt=5 prim:1->10 prim:3->30 prim:4->40 prim:5->50 prim:6->60
flushMetadata: primOff=0 primCnt=7 gridOff=80 grid=2x2 scene=[0,0]-[200,200]
```

### Test Coverage

| Test Suite | File | Tests | Status |
|------------|------|-------|--------|
| ThorVG rendering | `test/ut/thorvg/thorvg_test.cpp` | 14 | ✅ All pass |
| ThorVG rounded rect | `test/ut/thorvg/thorvg_rounded_rect_test.cpp` | 7 | ✅ All pass |
| ThorVG circles | `test/ut/thorvg/thorvg_circle_test.cpp` | 9 | ✅ All pass |
| Triangulation | `test/ut/ydraw/triangulate_test.cpp` | 51 | ✅ All pass |
| YDraw gradients | `test/ut/ydraw/ydraw_gradient_test.cpp` | 7 | ✅ All pass |
| YDraw opacity | `test/ut/ydraw/ydraw_opacity_test.cpp` | 6 | ✅ All pass |

**Note:** All unit tests pass, but the ThorVG circle tests verify CPU-side primitive creation only. The actual GPU rendering issue is not caught by unit tests.

### Recent Fixes

1. **Gradient AABB computation** - Added bounding box calculation for `LinearGradientBox`, `LinearGradientCircle`, `RadialGradientCircle` in `ydraw-builder.cpp`

2. **3D type detection fix** - Changed `type >= 100` to `type >= 100 && type < 128` to properly count gradient primitives (types 132-134) as 2D

3. **Shader alpha blending** - Changed from `unpackColor()` (RGB only) to `unpackColorAlpha()` (RGBA) in `0x0003-ydraw.wgsl` for proper opacity support

4. **setView() call** - Added `_builder->setView()` in ThorVG card init to match YDraw card behavior

### Additional Demo Files Created

```
demo/assets/cards/x-draw/
├── circles.yaml      # YDraw Circle primitives
└── ellipses.yaml     # YDraw Ellipse primitives

demo/scripts/cards/ydraw/
├── circles.sh        # Run circles.yaml demo
└── ellipses.sh       # Run ellipses.yaml demo
```

---

## ThorVG v1.0.1 API Reference

**Release:** v1.0.1 (February 12, 2026)  
**Status:** Stable release, recommended for production use

### RenderMethod Interface

The `RenderMethod` interface is ThorVG's renderer backend abstraction. To create a custom renderer (like YDraw), implement this interface:

```cpp
struct RenderMethod {
    // Lifecycle
    virtual bool preUpdate() = 0;
    virtual bool postUpdate() = 0;
    virtual bool preRender() = 0;
    virtual bool postRender() = 0;
    virtual bool clear() = 0;
    virtual bool sync() = 0;
    
    // Shape preparation - receives shape data with transform, clips, opacity
    virtual RenderData prepare(const RenderShape& rshape, RenderData data,
                               const Matrix& transform, Array<RenderData>& clips,
                               uint8_t opacity, RenderUpdateFlag flags, bool clipper) = 0;
    
    // Image preparation - receives surface (texture) data
    virtual RenderData prepare(RenderSurface* surface, RenderData data,
                               const Matrix& transform, Array<RenderData>& clips,
                               uint8_t opacity, RenderUpdateFlag flags) = 0;
    
    // Rendering callbacks - THIS IS WHERE TEXT COMES AS PATHS!
    virtual bool renderShape(RenderData data) = 0;  // shapes AND text (as bezier outlines)
    virtual bool renderImage(RenderData data) = 0;  // images/textures
    
    // Disposal
    virtual void dispose(RenderData data) = 0;
    
    // Region/bounds queries
    virtual RenderRegion region(RenderData data) = 0;
    virtual bool bounds(RenderData data, Point* pt4, const Matrix& m) = 0;
    virtual bool intersectsShape(RenderData data, const RenderRegion& region) = 0;
    virtual bool intersectsImage(RenderData data, const RenderRegion& region) = 0;
    
    // Compositing
    virtual RenderCompositor* target(const RenderRegion& region, ColorSpace cs, CompositionFlag flags) = 0;
    virtual bool beginComposite(RenderCompositor* cmp, MaskMethod method, uint8_t opacity) = 0;
    virtual bool endComposite(RenderCompositor* cmp) = 0;
    
    // Effects
    virtual void prepare(RenderEffect* effect, const Matrix& transform) = 0;
    virtual bool region(RenderEffect* effect) = 0;
    virtual bool render(RenderCompositor* cmp, const RenderEffect* effect, bool direct) = 0;
    virtual void dispose(RenderEffect* effect) = 0;
    
    // Misc
    virtual bool blend(BlendMethod method) = 0;
    virtual ColorSpace colorSpace() = 0;
    virtual const RenderSurface* mainSurface() = 0;
    virtual void damage(RenderData rd, const RenderRegion& region) = 0;
    virtual bool partial(bool disable) = 0;
};
```

### Text Rendering Architecture

**Key insight:** ThorVG internally converts text to bezier paths before rendering. There is **no `renderText()` callback** - text arrives through `renderShape()` as outline paths.

This is standard SVG behavior and works in our favor:
1. User creates `Text` object with font/size/content
2. ThorVG internally converts glyphs to bezier outlines
3. Custom renderer receives paths via `renderShape()`
4. We decompose paths to YDraw primitives (beziers, polygons)

**Text class API (v1.0.1):**

```cpp
struct Text : Paint {
    // Setters
    Result font(const char* name) noexcept;       // Font family name
    Result size(float size) noexcept;             // Font size in points
    Result text(const char* text) noexcept;       // UTF-8 text content
    Result align(float x, float y) noexcept;      // Alignment [0..1]
    Result layout(float w, float h) noexcept;     // Layout box constraints
    Result wrap(TextWrap mode) noexcept;          // Wrapping mode
    Result italic(float shear = 0.18f) noexcept;  // Italic simulation
    Result outline(float width, uint8_t r, uint8_t g, uint8_t b) noexcept;  // Text stroke
    Result fill(uint8_t r, uint8_t g, uint8_t b) noexcept;  // Text color
    Result spacing(float spacing) noexcept;       // Line spacing
    
    // Static loaders
    static Result load(const char* path) noexcept;           // Load font file
    static Result unload(const char* path) noexcept;         // Unload font
    
    // Factory
    static std::unique_ptr<Text> gen() noexcept;
};
```

### RenderShape Structure

The `prepare()` callback receives `RenderShape` containing path data:

```cpp
struct RenderShape {
    struct {
        PathCommand* cmds;     // MoveTo, LineTo, CubicTo, Close
        uint32_t cmdCnt;
        Point* pts;            // Control points
        uint32_t ptsCnt;
    } path;
    
    Fill* fill;                // Gradient or solid fill
    uint8_t color[4];          // RGBA fill color
    
    struct {
        float width;
        uint8_t color[4];      // Stroke RGBA
        float* dashPattern;
        uint32_t dashCnt;
        StrokeCap cap;
        StrokeJoin join;
        Fill* fill;            // Stroke gradient
    } stroke;
    
    FillRule rule;             // EvenOdd or NonZero
};
```

### Integration Pattern

To create a YDraw-based ThorVG renderer:

```cpp
class YDrawRenderMethod : public RenderMethod {
    YDrawBuffer::Ptr buffer_;
    YDrawBuilder::Ptr builder_;
    
public:
    RenderData prepare(const RenderShape& rshape, RenderData data,
                       const Matrix& transform, Array<RenderData>& clips,
                       uint8_t opacity, RenderUpdateFlag flags, bool clipper) override
    {
        // Convert RenderShape.path to YDraw primitives
        // Try pattern matching first: ellipse, rectangle, polygon
        // Fall back to individual bezier/segment primitives
        
        if (tryRenderAsEllipse(rshape, transform, opacity)) return handle;
        if (tryRenderAsBox(rshape, transform, opacity)) return handle;
        if (tryRenderAsPolygon(rshape, transform, opacity)) return handle;
        
        // Decompose path commands to individual primitives
        walkPathCommands(rshape.path, transform, opacity);
        return handle;
    }
    
    bool renderShape(RenderData data) override {
        // Primitives already added in prepare()
        return true;
    }
    
    bool postRender() override {
        builder_->calculate();  // Compute grid, write GPU buffers
        return true;
    }
};
```

### Notes

- **No separate text API needed** - text comes as bezier paths through standard shape rendering
- **v1.0.1 is stable** - 47 commits behind main branch, but all core features present
- **Main branch additions** (experimental, not in v1.0.1):
  - `const char* text() const` - getter for text content
  - `uint32_t lines()` - get line count after wrapping
  - Glyph metrics API
  - Image filter method API

---

## ThorVG2: RenderMethod-Based Implementation Plan

### Overview

The current ThorVG integration uses tree traversal via `Accessor` API to extract shapes after parsing. This approach has limitations:
- Manual transform accumulation
- Complex shape detection heuristics
- Missing proper compositing and masking support
- Text requires special handling

**ThorVG2** will implement ThorVG's `RenderMethod` interface directly. ThorVG will call our renderer methods with pre-processed, ready-to-render data.

### Architecture Comparison

**Current (thorvg card):**
```
SVG/Lottie → ThorVG Parser → Scene Tree
                                ↓
                     Our Accessor traversal
                                ↓
                     Shape pattern detection
                                ↓
                     YDrawBuffer primitives
                                ↓
                     YDrawBuilder → GPU
```

**ThorVG2 (thorvg2 card):**
```
SVG/Lottie → ThorVG Parser → Scene Tree
                                ↓
                     ThorVG internal render()
                                ↓
                     YDrawRenderMethod callbacks
                        ├── prepare(RenderShape) → store shape data
                        ├── renderShape() → emit YDraw primitives
                        └── renderImage() → emit YDraw images
                                ↓
                     YDrawBuilder → GPU
```

### RenderMethod Interface Analysis

```cpp
struct RenderMethod {
    // LIFECYCLE - called once per frame
    virtual bool preUpdate() = 0;      // Before scene update
    virtual bool postUpdate() = 0;     // After scene update
    virtual bool preRender() = 0;      // Before render pass
    virtual bool postRender() = 0;     // After render pass (FLUSH BUFFERS HERE)
    virtual bool clear() = 0;          // Clear render target
    virtual bool sync() = 0;           // Wait for async completion

    // SHAPE PREPARATION - called per shape during update phase
    virtual RenderData prepare(
        const RenderShape& rshape,     // Path + fill + stroke data
        RenderData data,               // Previous RenderData (reuse)
        const Matrix& transform,       // WORLD transform (already accumulated!)
        Array<RenderData>& clips,      // Clip shapes
        uint8_t opacity,               // Combined opacity (already accumulated!)
        RenderUpdateFlag flags,        // What changed
        bool clipper                   // Is this a clipper shape
    ) = 0;

    // IMAGE PREPARATION - called per image during update phase
    virtual RenderData prepare(
        RenderSurface* surface,        // Image data
        RenderData data,
        const Matrix& transform,
        Array<RenderData>& clips,
        uint8_t opacity,
        FilterMethod filter,
        RenderUpdateFlag flags
    ) = 0;

    // RENDERING - called per shape/image during render phase
    virtual bool renderShape(RenderData data) = 0;   // Shapes AND text outlines
    virtual bool renderImage(RenderData data) = 0;   // Images/textures

    // DISPOSAL
    virtual void dispose(RenderData data) = 0;

    // REGION QUERIES
    virtual RenderRegion region(RenderData data) = 0;
    virtual bool bounds(RenderData data, Point* pt4, const Matrix& m) = 0;
    virtual bool intersectsShape(RenderData data, const RenderRegion& region) = 0;
    virtual bool intersectsImage(RenderData data, const RenderRegion& region) = 0;

    // COMPOSITING (for masks, blend modes)
    virtual RenderCompositor* target(const RenderRegion& region, ColorSpace cs, CompositionFlag flags) = 0;
    virtual bool beginComposite(RenderCompositor* cmp, MaskMethod method, uint8_t opacity) = 0;
    virtual bool endComposite(RenderCompositor* cmp) = 0;

    // EFFECTS
    virtual void prepare(RenderEffect* effect, const Matrix& transform) = 0;
    virtual bool region(RenderEffect* effect) = 0;
    virtual bool render(RenderCompositor* cmp, const RenderEffect* effect, bool direct) = 0;
    virtual void dispose(RenderEffect* effect) = 0;

    // MISC
    virtual bool blend(BlendMethod method) = 0;
    virtual ColorSpace colorSpace() = 0;
    virtual const RenderSurface* mainSurface() = 0;
    virtual void damage(RenderData rd, const RenderRegion& region) = 0;
    virtual bool partial(bool disable) = 0;
};
```

### Key Data Structures

**RenderShape** (received in prepare):
```cpp
struct RenderShape {
    RenderPath path;           // Commands + points
    Fill* fill;                // Gradient fill (or nullptr)
    RenderColor color;         // Solid fill RGBA
    RenderStroke* stroke;      // Stroke data (or nullptr)
    FillRule rule;             // EvenOdd or NonZero
};

struct RenderPath {
    Array<PathCommand> cmds;   // MoveTo, LineTo, CubicTo, Close
    Array<Point> pts;          // Control points
};

struct RenderStroke {
    float width;
    RenderColor color;
    Fill* fill;                // Gradient stroke
    struct Dash {
        float* pattern;
        uint32_t count;
        float offset;
    } dash;
    float miterlimit;
    StrokeCap cap;
    StrokeJoin join;
};
```

### Implementation Steps

#### Step 1: Create YDrawRenderMethod Class

**Files:**
- `src/yetty/thorvg2/ydraw-render-method.h`
- `src/yetty/thorvg2/ydraw-render-method.cpp`

**Tasks:**
1. Create `YDrawRenderMethod` class extending `tvg::RenderMethod`
2. Implement all pure virtual methods (stubs returning true initially)
3. Add `YDrawBuffer::Ptr` and `YDrawBuilder::Ptr` members
4. Add `setTarget(width, height)` method for viewport setup

#### Step 2: Implement Lifecycle Methods

**Methods:**
- `preUpdate()` → Return true (nothing to prepare)
- `postUpdate()` → Return true
- `preRender()` → Clear YDrawBuffer
- `postRender()` → Call `YDrawBuilder::calculate()`, flush to GPU
- `clear()` → Clear internal state
- `sync()` → Return true (synchronous rendering)

#### Step 3: Define YDrawRenderData Structure

**Structure to store shape data between prepare() and render():**
```cpp
struct YDrawRenderData {
    enum class Type { Shape, Image };
    Type type;
    
    // For shapes
    RenderPath path;           // Copy of path data
    Matrix transform;          // World transform
    uint8_t opacity;
    RenderColor fillColor;
    Fill* fill;                // Gradient (borrowed pointer)
    RenderStroke* stroke;      // Stroke data (borrowed pointer)
    FillRule rule;
    
    // Computed
    RenderRegion bounds;
    bool valid = false;
    
    // For images
    RenderSurface* surface;
    FilterMethod filter;
};
```

#### Step 4: Implement prepare(RenderShape)

**Logic:**
1. Create or reuse `YDrawRenderData`
2. Copy path commands and points
3. Store transform matrix (ALREADY IN WORLD SPACE!)
4. Store opacity (ALREADY ACCUMULATED!)
5. Store fill color and gradient pointer
6. Store stroke data pointer
7. Compute bounding box
8. Return pointer as `RenderData`

#### Step 5: Implement renderShape()

**Logic:**
1. Cast `RenderData` to `YDrawRenderData*`
2. Try pattern detection:
   - `tryRenderAsEllipse()` → Circle/Ellipse primitive
   - `tryRenderAsBox()` → Box/RoundedBox primitive
   - `tryRenderAsPolygon()` → Polygon primitive or triangulation
3. If no pattern matches, decompose path:
   - Each LineTo → Segment primitive (for strokes)
   - Each CubicTo → Bezier3 primitive
4. Handle gradients:
   - LinearGradient → LinearGradientBox/LinearGradientCircle
   - RadialGradient → RadialGradientCircle
5. Handle stroke if present:
   - Walk path commands emitting stroke primitives
   - Apply dash pattern if specified

#### Step 6: Implement prepare(RenderSurface) and renderImage()

**Logic:**
1. Store surface pointer, transform, opacity
2. In renderImage(), emit YDraw Image primitive with atlas coords
3. Handle image filtering mode

#### Step 7: Implement Region/Bounds Methods

- `region()` → Return computed bounding box from RenderData
- `bounds()` → Transform bounding box by matrix
- `intersectsShape()` → Check region intersection
- `intersectsImage()` → Check region intersection

#### Step 8: Implement Stub Compositing

**Initial implementation (no masking):**
- `target()` → Return nullptr
- `beginComposite()` → Return true
- `endComposite()` → Return true
- `blend()` → Store blend method (ignore for now)

#### Step 9: Create ThorVG2 Card

**Files:**
- `src/yetty/cards/thorvg2/thorvg2.h`
- `src/yetty/cards/thorvg2/thorvg2.cpp`
- `src/yetty/cards/thorvg2/CMakeLists.txt`

**Tasks:**
1. Create card that uses `YDrawRenderMethod` instead of `SwCanvas`
2. Load SVG/Lottie via ThorVG's `Picture::load()`
3. Create custom canvas wrapper that uses our `YDrawRenderMethod`
4. On render: call ThorVG's `Canvas::update()` then `Canvas::draw()`

#### Step 10: Custom Canvas Class

ThorVG's Canvas classes are hardcoded to specific renderers. We need to either:

**Option A: Patch ThorVG** (requires modifying ThorVG source)
- Add `YDrawCanvas` class in ThorVG
- Register our renderer in initializer

**Option B: Direct Scene Rendering** (no ThorVG modifications)
- Get root `Scene` from `Picture`
- Call `Scene::pImpl->update(ourRenderer, ...)` directly
- Call `Scene::pImpl->render(ourRenderer)` directly
- Requires accessing internal headers (`tvgPaint.h`, `tvgScene.h`)

**Option C: Minimal Canvas Wrapper** (recommended)
- Create our own canvas-like wrapper outside ThorVG
- Manages Scene lifecycle
- Calls our RenderMethod directly

#### Step 11: Build System Integration

**Tasks:**
1. Create `src/yetty/thorvg2/CMakeLists.txt`
2. Link against ThorVG (same as current)
3. Include ThorVG internal headers:
   - `tmp/thorvg/src/renderer/tvgRender.h`
   - `tmp/thorvg/src/renderer/tvgPaint.h`
   - `tmp/thorvg/src/renderer/tvgScene.h`
4. Add to demo scripts: `demo/scripts/cards/thorvg2/`

#### Step 12: Reuse Existing Code

**From current thorvg-renderer.cpp:**
- `tryRenderAsEllipse()` logic
- `tryRenderAsBox()` logic
- `tryRenderAsPolygon()` logic
- `computeGradient()` logic
- Color conversion (ThorVG RGBA → YDraw ABGR)

**From ydraw-builder.cpp:**
- All YDraw primitive emission
- Grid calculation
- Buffer management

#### Step 13: Demo Scripts

Create demo scripts in `demo/scripts/cards/thorvg2/`:
- `simple-shapes.sh` - SVG shapes
- `beziers.sh` - Bezier curves
- `opacity.sh` - Opacity/circles test
- `gradients.sh` - Gradient fills
- `lottie-bounce.sh` - Lottie animation

#### Step 14: Tests

Create tests in `test/ut/thorvg2/`:
- `ydraw_render_method_test.cpp` - RenderMethod implementation tests
- `thorvg2_shape_test.cpp` - Shape conversion tests

### File Structure

```
src/yetty/thorvg2/
├── CMakeLists.txt
├── ydraw-render-method.h       # YDrawRenderMethod class
├── ydraw-render-method.cpp     # Implementation
├── ydraw-render-data.h         # YDrawRenderData structure
└── ydraw-canvas.h              # Optional canvas wrapper

src/yetty/cards/thorvg2/
├── CMakeLists.txt
├── thorvg2.h                   # ThorVG2 card
└── thorvg2.cpp                 # Card implementation

demo/scripts/cards/thorvg2/
├── simple-shapes.sh
├── opacity.sh
├── gradients.sh
└── lottie-bounce.sh

test/ut/thorvg2/
├── CMakeLists.txt
├── main.cpp
└── ydraw_render_method_test.cpp
```

### Benefits Over Current Implementation

1. **Transform handling** - ThorVG passes world transforms; no manual accumulation
2. **Opacity handling** - ThorVG passes combined opacity; no manual multiplication
3. **Text support** - Text arrives as bezier outlines through renderShape()
4. **Correct render order** - ThorVG controls draw order
5. **Animation** - Frame updates flow naturally through render cycle
6. **Compositing** - Foundation for future mask/blend support
7. **Cleaner code** - Callback-based instead of tree traversal

### Dependencies

- ThorVG v1.0.1 (or later)
- Access to ThorVG internal headers (renderer/)
- Existing YDraw infrastructure (ydraw-buffer, ydraw-builder, triangulate)
