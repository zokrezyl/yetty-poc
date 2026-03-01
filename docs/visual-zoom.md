# Visual Zoom Design

Visual Zoom is a shader-based zoom feature that magnifies the terminal output without changing the terminal structure (rows/columns). This contrasts with the existing Ctrl+Wheel zoom which resizes the terminal grid.

## Naming Rationale

| Option | Description |
|--------|-------------|
| Magnify Mode | Clear, but conflicts with existing magnify effect |
| Loupe Mode | Like a jeweler's loupe - visual inspection |
| Lens Zoom | Implies optical/shader transformation |
| **Visual Zoom** | Distinguishes from structural zoom - chosen |

## Key Differences: Structural Zoom vs Visual Zoom

| Aspect | Ctrl+Wheel (Structural) | Ctrl+Shift+Wheel (Visual) |
|--------|------------------------|---------------------------|
| What changes | Terminal structure (cols/rows) | Shader coordinates only |
| Cell buffer | Resized & rebuilt | Unchanged |
| Font rendering | Re-rasterized at new size | Same cells, scaled in shader |
| Scrolling | Terminal scrollback | Pan the magnified view |
| Use case | Permanent accessibility zoom | Temporary inspection |
| MSDF text | Re-rendered | Scales beautifully |
| Shader glyphs | Re-rendered | Scales beautifully |
| Rasterized cards | Re-rendered | Pixelated (expected) |

## Architecture

### 1. Uniform Changes

**C++ (`gpu-screen.cpp` Uniforms struct)** - Add after `effectParams[6]`:
```cpp
// Visual Zoom (shader-based, no terminal resize)
float visualZoomScale = 1.0f;      // 4 bytes: 1.0 = off, >1.0 = zoomed
float visualZoomOffsetX = 0.0f;    // 4 bytes: pan X in pixels
float visualZoomOffsetY = 0.0f;    // 4 bytes: pan Y in pixels
float _pad0 = 0.0f;                // 4 bytes: padding for 16-byte alignment
```
New total: 256 bytes (still 16-byte aligned)

**WGSL (`gpu-screen.wgsl` GridUniforms struct)**:
```wgsl
// Visual Zoom (shader-based magnification)
visualZoomScale: f32,      // 4 bytes: 1.0 = off, >1.0 = zoomed
visualZoomOffsetX: f32,    // 4 bytes: pan X in pixels
visualZoomOffsetY: f32,    // 4 bytes: pan Y in pixels
_pad0: f32,                // 4 bytes: alignment padding
```

### 2. State Variables (GPUScreenImpl)

```cpp
// Visual Zoom state
bool _visualZoomActive = false;
float _visualZoomScale = 1.0f;     // 1.0 to 8.0
float _visualZoomOffsetX = 0.0f;   // Pan in source pixels
float _visualZoomOffsetY = 0.0f;
```

### 3. Shader Coordinate Transform

Insert after `let pixelPos = fbPixelPos - grid.viewportOrigin;`:

```wgsl
// ==== VISUAL ZOOM: transform coordinates before all other processing ====
var zoomedPos = pixelPos;
if (grid.visualZoomScale > 1.0) {
    // Zoom centered on viewport center, then apply pan offset
    let viewportCenter = grid.screenSize * 0.5;
    // Inverse transform: divide by scale (2x zoom = sample half the area)
    zoomedPos = (pixelPos - viewportCenter) / grid.visualZoomScale
              + viewportCenter
              + vec2<f32>(grid.visualZoomOffsetX, grid.visualZoomOffsetY);
}
```

Then use `zoomedPos` instead of `pixelPos` for grid bounds check and distortion input.

### 4. Scroll Event Handling

```cpp
bool ctrlPressed = (event.scroll.mods & 0x0002) != 0;   // GLFW_MOD_CONTROL
bool shiftPressed = (event.scroll.mods & 0x0001) != 0;  // GLFW_MOD_SHIFT

if (ctrlPressed && shiftPressed) {
    // Ctrl+Shift+scroll: Visual Zoom
    if (!_visualZoomActive && event.scroll.dy > 0) {
        _visualZoomActive = true;
        _visualZoomScale = 1.0f;
        _visualZoomOffsetX = 0.0f;
        _visualZoomOffsetY = 0.0f;
    }
    if (_visualZoomActive) {
        _visualZoomScale += event.scroll.dy * 0.1f;
        _visualZoomScale = std::clamp(_visualZoomScale, 1.0f, 8.0f);
        if (_visualZoomScale <= 1.0f) {
            exitVisualZoom();  // Exit when zoomed all the way out
        }
        clampVisualZoomOffset();  // Re-clamp offsets
        yinfo("GPUScreen {} visual zoom: {:.1f}x", _id, _visualZoomScale);
    }
} else if (_visualZoomActive) {
    // In visual zoom mode: scroll pans the view
    if (shiftPressed) {
        // Shift+scroll: horizontal pan
        _visualZoomOffsetX -= event.scroll.dy * getCellWidth();
    } else {
        // Plain scroll: vertical pan
        _visualZoomOffsetY -= event.scroll.dy * getCellHeight() * 3;
    }
    clampVisualZoomOffset();
} else if (ctrlPressed) {
    // Ctrl+scroll: structural zoom (existing)
    ...
} else {
    // Normal scroll: scrollback (existing)
    ...
}
```

### 5. Helper Functions

```cpp
void exitVisualZoom() {
    _visualZoomActive = false;
    _visualZoomScale = 1.0f;
    _visualZoomOffsetX = 0.0f;
    _visualZoomOffsetY = 0.0f;
}

void clampVisualZoomOffset() {
    if (_visualZoomScale <= 1.0f) return;

    float cellW = getCellWidth();
    float cellH = getCellHeight();
    float contentW = _cols * cellW;
    float contentH = (_rows + getScrollbackSize()) * cellH;

    // Visible area in source coordinates
    float visibleW = _viewportWidth / _visualZoomScale;
    float visibleH = _viewportHeight / _visualZoomScale;

    // Max offset is half the hidden content
    float maxOffsetX = (contentW - visibleW) / 2.0f;
    float maxOffsetY = (contentH - visibleH) / 2.0f;

    maxOffsetX = std::max(0.0f, maxOffsetX);
    maxOffsetY = std::max(0.0f, maxOffsetY);

    _visualZoomOffsetX = std::clamp(_visualZoomOffsetX, -maxOffsetX, maxOffsetX);
    _visualZoomOffsetY = std::clamp(_visualZoomOffsetY, -maxOffsetY, maxOffsetY);
}
```

### 6. Uniform Upload (render function)

```cpp
// Visual Zoom
_uniforms.visualZoomScale = _visualZoomActive ? _visualZoomScale : 1.0f;
_uniforms.visualZoomOffsetX = _visualZoomOffsetX;
_uniforms.visualZoomOffsetY = _visualZoomOffsetY;
```

## Input Behavior

| Input | Normal Mode | Visual Zoom Active |
|-------|-------------|-------------------|
| Ctrl+Shift+Wheel Up | Enter visual zoom | Increase zoom |
| Ctrl+Shift+Wheel Down | (nothing) | Decrease zoom (exit if <=1.0) |
| Wheel | Scrollback | Pan vertically |
| Shift+Wheel | (nothing) | Pan horizontally |
| Escape | - | Exit visual zoom |
| Ctrl+Wheel | Structural zoom | Structural zoom |

## Flow Diagram

```
┌─────────────────────────────────────────────────────────┐
│                    Normal Mode                          │
│  Ctrl+Wheel → Structural zoom (resize terminal)         │
└──────────────────────┬──────────────────────────────────┘
                       │ Ctrl+Shift+Wheel
                       ▼
┌─────────────────────────────────────────────────────────┐
│                   Visual Zoom Mode                      │
│  • Ctrl+Shift+Wheel → Adjust zoom scale                 │
│  • Wheel → Vertical pan                                 │
│  • Shift+Wheel → Horizontal pan                         │
│  • Escape / scale<=1.0 → Exit                           │
│                                                         │
│  Shader applies: zoomedPos = (pixelPos - center)/scale  │
│                            + center + offset            │
└─────────────────────────────────────────────────────────┘
```

## Tradeoffs

- **MSDF text**: Scales beautifully due to signed distance field rendering
- **Shader glyphs/cards**: Scale beautifully (vector-based)
- **Rasterized cards** (images): Will be pixelated at high zoom - this is expected and acceptable for temporary inspection use case
