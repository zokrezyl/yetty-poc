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
        _builder->calculate();
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
| **Gradients** (Linear/Radial) | ❌ | High | YDraw needs gradient primitive types |
| **Stroke dash patterns** | ❌ | Medium | Needs stroke-to-path conversion |
| **Stroke caps/joins** | ❌ | Medium | Round, bevel, miter not supported |
| **Variable stroke width** | ❌ | Medium | YDraw has fixed strokeWidth |
| **Fill rules** (EvenOdd/NonZero) | ❌ | High | Complex path decomposition needed |
| **Alpha masks** | ❌ | High | Multi-pass rendering needed |
| **Blend modes** | ❌ | High | Not in YDraw primitive model |
| **Text rendering** | ❌ | Medium | Could wire to YDraw's glyph system |
| **Images/Pictures** | ❌ | Medium | YDraw has Image type |
| **Quadratic Bezier** | ❌ | Low | Add Bezier2 detection |
| **Arc/Pie shapes** | ❌ | Low | Add pattern detection |

### Recently Implemented

| Feature | Implementation | Notes |
|---------|----------------|-------|
| **Arbitrary polygons** | `tryRenderAsPolygon()` → `addPolygonWithVertices()` | Closed paths without curves become Polygon primitives |
| **Polygons with holes** | `PolygonGroup` + triangulation | Ear-clipping with bridge merging for holes |
| **Multiple holes** | Hole sorting + sequential merge | Holes sorted by rightmost x-coordinate |
| **Axis-aligned rect detection** | `tryRenderAsBox()` validation | Verifies 2 unique X/Y values to avoid misdetecting pentagons |

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
└── thorvg_test.cpp        # ThorVG rendering tests (13 tests)

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
