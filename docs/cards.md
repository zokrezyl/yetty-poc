# Yetty Card System

Cards are GPU-rendered widgets that live within the terminal grid. They scroll with terminal content just like text, enabling rich visualizations, images, and interactive elements directly in your terminal.

## Creating Cards via OSC Sequences

Cards are created using **OSC 666666** escape sequences. Any program that can write to stdout can create cards.

### OSC Format

```
ESC ] 666666 ; <generic-args> ; <plugin-args> ; <payload> ST
```

Where:
- `ESC ]` = `\033]` (OSC introducer)
- `666666` = Yetty vendor ID
- `generic-args` = Command and positioning (Unix-style flags)
- `plugin-args` = Plugin-specific options
- `payload` = Base64-encoded data (file contents, YAML, binary)
- `ST` = `\033\\` (String Terminator)

### Generic Arguments

| Command | Arguments | Description |
|---------|-----------|-------------|
| `create` | `-p PLUGIN -x X -y Y -w W -h H [-r]` | Create a card |
| `ls` | `[--all]` | List active cards |
| `plugins` | | List available plugins |
| `kill` | `--id ID \| --plugin NAME` | Destroy card(s) |
| `stop` | `--id ID \| --plugin NAME` | Pause card(s) |
| `start` | `--id ID \| --plugin NAME` | Resume card(s) |
| `update` | `--id ID` | Update card payload |

**Create flags:**
- `-p, --plugin` - Plugin name (required): `image`, `plot`, `ydraw`, `pdf`, `thorvg`, `ymery`
- `-x, --x-pos` - X position in cells (default: 0)
- `-y, --y-pos` - Y position in cells (default: 0)
- `-w, --width` - Width in cells (0 = stretch to terminal width)
- `-h, --height` - Height in cells (0 = stretch)
- `-r, --relative` - Position relative to cursor (default: absolute)

### Plugin Arguments

Each plugin accepts specific arguments in the second field:

| Plugin | Arguments |
|--------|-----------|
| `plot` | `--type line\|bar\|scatter\|area --grid --axes` |
| `ydraw` | `--cell-size N --show-grid --show-eval-count` |
| `thorvg` | `--loop --speed N` |
| `image` | (none) |
| `pdf` | `--page N` |

### Payload Encoding

Payloads are **Base64 encoded** to be safe within escape sequences. Base64 is a standard encoding available in most programming languages and via the `base64` command-line tool:

```python
import base64

# Encoding
encoded = base64.b64encode(data).decode('ascii')

# Decoding
data = base64.b64decode(encoded)
```

```bash
# Command line encoding
echo -n "Hello" | base64
# SGVsbG8=

# Command line decoding
echo "SGVsbG8=" | base64 -d
# Hello
```

### Payload Content

The payload can be:

1. **File contents** - Raw image bytes, PDF bytes, etc. (base64 encoded)
2. **YAML definition** - For structured cards like YDraw, Plot
3. **Pre-compiled binary** - For performance (YDraw binary format)

### Examples

**Create an image card from a file:**
```bash
# Using yetty-client (recommended)
yetty-client create image -i logo.png -w 40 -H 20

# Raw escape sequence
python3 -c "
import sys
import base64

data = open('logo.png', 'rb').read()
encoded = base64.b64encode(data).decode('ascii')
sys.stdout.write(f'\033]666666;create -p image -w 40 -h 20 -r;;{encoded}\033\\\\')
"
```

**Create a plot card:**
```bash
# Generate sine wave data
yetty-client create plot -w 60 -H 20 --type line --grid --axes << 'EOF'
0.0,0.31,0.59,0.81,0.95,1.0,0.95,0.81,0.59,0.31,0.0,-0.31,-0.59,-0.81,-0.95,-1.0
EOF

# Or with inline Python
python3 -c "
import math, sys, base64

values = [math.sin(x * math.pi / 50) for x in range(100)]
payload = ','.join(f'{v:.3f}' for v in values)
encoded = base64.b64encode(payload.encode('utf-8')).decode('ascii')

args = '--type line --grid --axes'
sys.stdout.write(f'\033]666666;create -p plot -w 60 -h 20 -r;{args};{encoded}\033\\\\')
"
```

**Create a YDraw card from YAML file:**
```bash
# Using yetty-client with file input
yetty-client create ydraw -i shapes.yaml -w 80 -H 25

# With debug visualization
yetty-client create ydraw -i shapes.yaml -w 80 -H 25 --show-grid --show-eval-count
```

**YDraw YAML format:**
```yaml
# shapes.yaml
- background: "#1a1a2e"
- body:
    - circle:
        position: [100, 80]
        radius: 35
        fill: "#e74c3c"
        stroke: "#c0392b"
        stroke-width: 3

    - box:
        position: [220, 80]
        size: [60, 45]
        fill: "#3498db"
        round: 8

    - text:
        position: [50, 150]
        content: "Hello YDraw!"
        font-size: 24
        color: "#ffffff"
```

### yetty-client CLI Tool

The `yetty-client` tool (`tools/yetty-client/`) simplifies card creation:

```bash
# Install dependencies
cd tools/yetty-client && uv sync

# Create cards
uv run python main.py create image -i photo.png -w 40 -H 20
uv run python main.py create ydraw -i scene.yaml -w 80 -H 30
uv run python main.py create plot --demo sine -w 60 -H 15

# List active cards
uv run python main.py ls

# Kill a card
uv run python main.py kill --id abc123
```

**Available plugins:**
- `image` - PNG, JPEG, WebP, GIF images
- `plot` - Line, bar, scatter, area charts
- `ydraw` - 2D vector graphics (SDF + text)
- `pdf` - PDF documents
- `thorvg` - SVG and Lottie animations
- `ymery` - ImGui interactive widgets

### tmux Support

When running inside tmux, escape sequences must be wrapped in DCS passthrough:

```
ESC P tmux; <escaped_content> ESC \
```

Where ESC characters in content are doubled (`ESC` → `ESC ESC`). The `yetty-client` tool handles this automatically via `maybe_wrap_for_tmux()`.

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                           CardManager                                │
│  - Allocates metadata slots (32/64/128/256 byte pools)              │
│  - Coordinates lifecycle across all cards                            │
│  - Flushes dirty regions to GPU                                      │
└─────────────────────────────────────────────────────────────────────┘
              │                                    │
              ▼                                    ▼
┌─────────────────────────────┐      ┌─────────────────────────────┐
│    CardBufferManager        │      │   CardTextureManager        │
│  - Linear GPU storage buf   │      │  - Dynamic texture atlas    │
│  - Free-list sub-allocator  │      │  - Strip packing algorithm  │
│  - Direct CPU write access  │      │  - Right-sized per frame    │
│  - Dirty region coalescing  │      │  - Released when empty      │
│  - Fully cleared on realloc │      │  - Fully cleared on realloc │
└─────────────────────────────┘      └─────────────────────────────┘
              │                                    │
              ▼                                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│                          Card Base Class                             │
│  - needsBuffer() / needsTexture() — declare resource type           │
│  - declareBufferNeeds() → allocateBuffers()                         │
│  - allocateTextures() → writeTextures()                             │
│  - render(time) — per-frame update                                   │
│  - suspend() / dispose() — lifecycle management                      │
└─────────────────────────────────────────────────────────────────────┘
              │
    ┌─────────┴─────────┬─────────────┬─────────────┬────────────┐
    ▼                   ▼             ▼             ▼            ▼
┌────────┐        ┌──────────┐   ┌─────────┐   ┌─────────┐   ┌───────┐
│ Image  │        │   Plot   │   │   PDF   │   │  YDraw  │   │ Ymery │
│(texture)│       │ (buffer) │   │(texture)│   │(buffer) │   │(ImGui)│
└────────┘        └──────────┘   └─────────┘   └─────────┘   └───────┘
```

## Two Resource Types

Cards declare which resources they need via `needsBuffer()` and `needsTexture()`. A card can use one or both.

Both managers follow the same core principle: **when any card enters or leaves the visible viewport, ALL resources are cleared and ALL visible cards must re-declare their needs from scratch**. There is no per-handle deallocation. This ensures the GPU buffer/atlas is always right-sized to exactly what is currently needed, and resources are released to zero when no cards of that type are visible.

### Storage Buffer (CardBufferManager)

Used by cards that store structured data: plots (float arrays), YDraw (SDF primitives, spatial hash grid).

**Characteristics:**
- Linear GPU storage buffer (SSBO), binding 2 in the card shader
- Direct CPU write access via raw pointers (`handle.data`)
- 16-byte alignment
- Dirty region tracking with coalesced GPU uploads
- No per-handle deallocation — `commitReservations()` clears everything
- `allocateBuffer()` auto-replaces existing keys (for mid-cycle resizing)

**Why a storage buffer (not a texture)?** Buffer cards store structured numeric data (float arrays, primitive structs, grid indices) that the shader reads by offset. A linear buffer is the natural fit — cards get a raw pointer to write floats directly, and the shader indexes into it with `storage[offset]`. There is no 2D spatial locality, so a texture would add unnecessary complexity.

**Three-Phase Allocation Protocol:**

When a buffer card enters or leaves the viewport, `_bufferLayoutChanged` is set, triggering a full reallocation cycle on the next frame:

```
Phase 1: declareBufferNeeds()
  Each buffer card calls bufferMgr->reserve(size) to declare its total need.
  Cards save existing GPU data to CPU staging before this phase.

Phase 2: commitReservations()
  The buffer manager clears ALL sub-allocations and recreates the internal
  allocator sized to exactly the total reserved bytes. The GPU buffer is
  recreated if the size changed. All previous BufferHandle pointers are
  now invalid.

Phase 3: allocateBuffers()
  Each buffer card calls bufferMgr->allocateBuffer(slotIndex, scope, size)
  to get a fresh handle with a stable CPU pointer. Cards restore data from
  staging into the new allocation.
```

**Why three phases?**
- Phase 1 computes the exact total size needed across all visible cards
- Phase 2 creates a single GPU buffer of that exact size (right-sized, no waste)
- Phase 3 gives cards stable pointers that won't move (no reallocation after this)
- When all buffer cards leave, the total is zero and the GPU buffer shrinks to minimum

**BufferHandle Structure:**
```cpp
struct BufferHandle {
    uint8_t* data;      // Direct CPU pointer — write here
    uint32_t offset;    // Offset in GPU buffer (bytes)
    uint32_t size;      // Allocation size
};
```

**Mid-cycle resizing:** If a card needs to grow its buffer during render (e.g., adding primitives), it can call `allocateBuffer()` with the same `(slotIndex, scope)` key and a larger size. The manager auto-replaces the old allocation internally — no explicit deallocation needed.

### Texture Atlas (CardTextureManager)

Used by cards that render pixel data: Image, PDF, ThorVG, YHtml, Ymery.

**Characteristics:**
- Single RGBA8 texture atlas, binding 2-3 in the card shader
- Right-sized: smallest power-of-2 that fits all current textures (e.g., 256, 512, 2048)
- Strip packing algorithm (sort entries by height descending)
- Fully released when no texture cards are visible (GPU memory goes to zero)
- No per-handle deallocation — `clearHandles()` wipes everything
- `write()` only works after `createAtlas()` — enforced with error check

**Why a texture atlas (not a buffer)?** Texture cards provide 2D RGBA pixel data (rendered images, PDF pages, SVG frames). The GPU samples this data with `textureSample()` using 2D UV coordinates, getting hardware bilinear filtering for free. A buffer would require manual 2D indexing and lose filtering support.

**Four-Phase Allocation Protocol:**

When a texture card enters or leaves the viewport, `_textureLayoutChanged` is set, triggering a full reallocation cycle on the next frame:

```
Phase 1: clearHandles()
  The texture manager wipes ALL texture handles. Every card's handle
  is now invalid. This is the equivalent of commitReservations() for buffers.

Phase 2: allocateTextures()
  Each texture card calls textureMgr->allocate(width, height) to declare
  the size of its texture. This returns a TextureHandle but does NOT write
  any pixel data yet. Cards prepare their pixels in CPU memory.

Phase 3: createAtlas()
  The texture manager packs all declared handles using strip packing,
  finds the smallest power-of-2 atlas size that fits, and creates (or
  recreates) the GPU texture. Atlas positions are now assigned.

Phase 4: writeTextures()
  Each texture card calls textureMgr->write(handle, pixels) to push its
  pixel data into the atlas. This only works AFTER createAtlas() — calling
  write() before the atlas is created returns an error.
```

**Why four phases?**
- Phase 1 ensures a clean slate — no stale handles from departed cards
- Phase 2 collects all size requirements without touching the GPU
- Phase 3 computes the optimal atlas layout and creates the GPU texture at exactly the right size
- Phase 4 writes pixels now that atlas positions are known and the GPU texture exists
- The split between allocate (phase 2) and write (phase 4) prevents cards from writing to a texture that doesn't exist yet

**Right-sizing behavior:**
- If two 512x512 texture cards are visible, the atlas is 1024x512 (or 1024x1024 power-of-2)
- If one leaves, the atlas shrinks to 512x512 on the next reallocation
- If all leave, the atlas GPU resources are fully released (zero GPU memory)

**TextureHandle:**
```cpp
struct TextureHandle {
    uint32_t id;      // Opaque handle ID (0 = invalid)
};
```

### Key Difference: Buffer vs Texture

| Aspect | CardBufferManager | CardTextureManager |
|--------|-------------------|---------------------|
| GPU resource | Storage buffer (SSBO) | RGBA8 2D texture |
| Data type | Structured (floats, u32s) | Pixel data (RGBA8) |
| Shader access | `storage[offset]` | `textureSample(uv)` |
| Card writes via | Direct pointer (`handle.data`) | `write(handle, pixels)` |
| Sizing | Exact byte count | Power-of-2 atlas |
| Clear trigger | `commitReservations()` | `clearHandles()` |
| Write timing | During `allocateBuffers()` | During `writeTextures()` (after atlas) |
| Mid-cycle resize | `allocateBuffer()` auto-replaces | Not supported (full rebuild) |
| When empty | Shrinks to 4 bytes minimum | Fully released (zero GPU memory) |

## Card Lifecycle

### Lifecycle Methods

```cpp
class Card : public base::EventListener {
public:
    // Resource declaration
    virtual bool needsBuffer() const { return false; }
    virtual bool needsTexture() const { return false; }

    // Buffer allocation (Loops 1-2, when buffer cards change)
    virtual void declareBufferNeeds() {}
    virtual Result<void> allocateBuffers() { return Ok(); }

    // Texture allocation (Loops 3a-3b, when texture cards change)
    virtual Result<void> allocateTextures() { return Ok(); }
    virtual Result<void> writeTextures() { return Ok(); }

    // Per-frame rendering (every frame, after allocation)
    virtual Result<void> render(float time) { return Ok(); }

    // Lifecycle termination
    virtual void suspend();              // Card scrolled off-screen
    virtual Result<void> dispose() = 0;  // Card destroyed
};
```

### What Happens When a Card Enters or Leaves

When a card enters or leaves the visible viewport (scroll, creation, destruction), gpu-screen sets `_bufferLayoutChanged` and/or `_textureLayoutChanged` depending on the card's resource type. On the next render frame, the full reallocation cycle runs for the affected resource type. **All visible cards** of that type must re-declare their needs — not just the card that changed.

This means:
- **Card enters:** Its resources are allocated alongside all existing cards
- **Card leaves:** Its resources are gone. Remaining cards get fresh allocations in a right-sized buffer/atlas
- **No card visible:** GPU resources shrink to minimum (buffer) or are fully released (texture)

### GPU-Screen Render Loop

```cpp
void GPUScreen::render() {
    // === Buffer reallocation (when any buffer card enters/leaves) ===
    if (_bufferLayoutChanged) {
        // Loop 1: All buffer cards declare their total needs
        for (auto& card : _cards)
            if (card->needsBuffer())
                card->declareBufferNeeds();  // calls reserve()

        // commitReservations: CLEARS all sub-allocations, recreates
        // allocator at exactly the total reserved size
        _bufferMgr->commitReservations();

        // Loop 2: All buffer cards allocate fresh handles
        for (auto& card : _cards)
            if (card->needsBuffer())
                card->allocateBuffers();     // calls allocateBuffer()

        _bufferLayoutChanged = false;
    }

    // === Texture reallocation (when any texture card enters/leaves) ===
    if (_textureLayoutChanged) {
        // Phase 1: Clear all texture handles
        _textureMgr->clearHandles();

        // Phase 2: All texture cards declare their sizes
        for (auto& card : _cards)
            if (card->needsTexture())
                card->allocateTextures();    // calls allocate(w, h)

        // Phase 3: Pack atlas and create GPU texture
        _textureMgr->createAtlas();

        // Phase 4: All texture cards write pixel data
        for (auto& card : _cards)
            if (card->needsTexture())
                card->writeTextures();       // calls write(handle, pixels)

        _textureLayoutChanged = false;
    }

    // === Per-frame render (every frame) ===
    for (auto& card : _cards)
        card->render(time);

    // === GPU upload ===
    _cardMgr->flush(queue);  // uploads dirty buffer regions + atlas pixels
}
```

## Metadata System

Cards pass parameters to shaders via a metadata buffer. Each card allocates a slot (32, 64, 128, or 256 bytes) and the slot index is encoded in terminal cells using ANSI colors.

### Metadata Encoding in Cells

```cpp
struct Cell {
    uint32_t glyph;                    // Card glyph (U+100000-U+100FFF)
    uint8_t fgR, fgG, fgB, alpha;      // Foreground = metadata slot index
    uint8_t bgR, bgG, bgB, style;      // Background = relative position
};
```

**Foreground (24-bit):** Metadata slot index
- Allows addressing up to 512MB of metadata
- Shader decodes: `metaOffset = slotIndex * slotSize / 4`

**Background (24-bit):** Relative cell position
- Encoding: `(relRow << 12) | relCol`
- Supports cards up to 4096×4096 cells
- Used for multi-cell rendering and mouse hit testing

### Metadata Pools

```cpp
class MetadataAllocator {
    MetadataPool _pool32;   // 32-byte slots
    MetadataPool _pool64;   // 64-byte slots (most common)
    MetadataPool _pool128;  // 128-byte slots
    MetadataPool _pool256;  // 256-byte slots
};

// Allocation
MetadataHandle handle = cardMgr->allocateMetadata(64);  // 64 bytes
cardMgr->writeMetadata(handle, &myMetadata, sizeof(myMetadata));
```

## Card Implementations

### Image Card

Renders raster images (PNG, JPEG, WebP) via texture atlas.

**Shader Glyph:** `U+100000`
**Resource:** Texture

**Metadata (64 bytes):**
```cpp
struct ImageMetadata {
    uint32_t atlasX, atlasY;      // Position in texture atlas
    uint32_t atlasW, atlasH;      // Size in atlas
    uint32_t widthCells;          // Card width in cells
    uint32_t heightCells;         // Card height in cells
    // ... scaling, color info
};
```

**Features:**
- GPU-accelerated scaling via compute shader
- Automatic rescale when terminal zoom changes
- Supports transparency (alpha blending)

### Plot Card

Renders data visualizations: line, bar, scatter, area plots.

**Shader Glyph:** `U+100001`
**Resource:** Buffer

**Metadata (64 bytes):**
```cpp
struct PlotMetadata {
    uint32_t plotType;            // Line, Bar, Scatter, Area
    uint32_t widthCells, heightCells;
    uint32_t dataOffset;          // Offset in buffer (float units)
    uint32_t dataCount;           // Number of data points
    float minValue, maxValue;     // Data range
    uint32_t lineColor, fillColor;
    uint32_t flags;               // Grid, axes, etc.
    float zoom, panX, panY;       // View transform
};
```

**Data Storage:**
- Float array in CardBufferManager
- `dataOffset` points to first value
- Shader reads values directly from storage buffer

### PDF Card

Renders PDF pages as textures.

**Shader Glyph:** `U+100000` (same as Image)
**Resource:** Texture

**Implementation:**
- Renders PDF page to offscreen framebuffer
- Uploads rendered pixels to texture atlas
- Supports page navigation
- Uses PDFium or MuPDF backend

### YDraw Card

Renders 2D vector graphics with SDF primitives and MSDF text.

**Shader Glyph:** `U+100003`
**Resource:** Buffer

**Metadata (64 bytes):**
```cpp
struct YDrawMetadata {
    uint32_t primitiveOffset;     // Offset to SDFPrimitive array
    uint32_t primitiveCount;      // Number of primitives
    uint32_t gridOffset;          // Offset to spatial hash grid
    uint32_t gridWidth, gridHeight;
    float cellSize;               // Grid cell size
    uint32_t glyphOffset;         // Offset to text glyphs
    uint32_t glyphCount;
    float sceneMinX, sceneMinY;   // Scene bounds
    float sceneMaxX, sceneMaxY;
    uint32_t widthCells, heightCells;
    uint32_t flags;               // Debug flags
    uint32_t bgColor;             // Background color
};
```

## YDraw Spatial Hash Grid

YDraw uses a spatial hash grid for O(1) primitive culling in the fragment shader. Without this, every pixel would need to evaluate every primitive - impossibly slow for complex scenes.

### How It Works

**Build Phase (CPU):**

```cpp
void YDrawBase::buildGrid() {
    // For each primitive
    for (uint32_t i = 0; i < _primCount; i++) {
        SDFPrimitive& prim = _primitives[i];

        // Compute AABB (axis-aligned bounding box)
        computeAABB(prim);

        // Find grid cells that overlap this AABB
        uint32_t cellMinX = (prim.aabbMinX - sceneMinX) / cellSize;
        uint32_t cellMaxX = (prim.aabbMaxX - sceneMinX) / cellSize;
        uint32_t cellMinY = (prim.aabbMinY - sceneMinY) / cellSize;
        uint32_t cellMaxY = (prim.aabbMaxY - sceneMinY) / cellSize;

        // Add primitive index to each overlapping cell
        for (cy = cellMinY; cy <= cellMaxY; cy++) {
            for (cx = cellMinX; cx <= cellMaxX; cx++) {
                uint32_t cellIndex = cy * gridWidth + cx;
                grid[cellIndex].add(i);
            }
        }
    }
}
```

**Grid Structure:**

```
┌──────────────────────────────────────────────────┐
│  Grid Cell Layout (each cell = 1 + maxPrimsPerCell u32s)  │
├──────────────────────────────────────────────────┤
│  cell[0]: [count] [prim0] [prim1] ... [primN]    │
│  cell[1]: [count] [prim0] [prim1] ...            │
│  ...                                              │
│  cell[gridWidth*gridHeight-1]: [count] ...       │
└──────────────────────────────────────────────────┘
```

- `count`: Number of primitives in this cell
- `primN`: Index into primitives array
- Default `maxPrimsPerCell = 16`

**Fragment Shader (GPU):**

```wgsl
fn renderYDraw(pixelCoord: vec2<f32>, meta: YDrawMetadata) -> vec4<f32> {
    // Transform pixel to scene coordinates
    let scenePos = pixelToScene(pixelCoord, meta);

    // Find grid cell for this pixel
    let cellX = u32((scenePos.x - meta.sceneMinX) / meta.cellSize);
    let cellY = u32((scenePos.y - meta.sceneMinY) / meta.cellSize);
    let cellIndex = cellY * meta.gridWidth + cellX;

    // Read cell from grid
    let cellOffset = meta.gridOffset + cellIndex * cellStride;
    let primCount = storage[cellOffset];

    // Only evaluate primitives in THIS cell
    var color = vec4<f32>(0.0);
    for (var i = 0u; i < primCount; i++) {
        let primIndex = storage[cellOffset + 1 + i];
        let prim = loadPrimitive(meta.primitiveOffset, primIndex);

        let dist = evaluateSDF(scenePos, prim);
        color = blendPrimitive(color, dist, prim);
    }

    return color;
}
```

### Performance Impact

| Scene | Without Grid | With Grid |
|-------|-------------|-----------|
| 100 primitives | 100 SDF evals/pixel | ~3-5 SDF evals/pixel |
| 1000 primitives | 1000 SDF evals/pixel | ~5-10 SDF evals/pixel |
| 10000 primitives | 10000 SDF evals/pixel | ~10-20 SDF evals/pixel |

Typical speedup: **10-100x** for complex scenes.

### Grid Parameters

```cpp
// Auto-compute cell size from scene and primitive density
float cellSize = sqrt(avgPrimitiveArea) * 1.5f;
cellSize = clamp(cellSize, minCellSize, maxCellSize);

// Or set manually
ydraw->setGridCellSize(10.0f);

// Grid dimensions (capped at 512×512)
gridWidth = min(512, ceil(sceneWidth / cellSize));
gridHeight = min(512, ceil(sceneHeight / cellSize));
```

### Text in YDraw

YDraw also supports MSDF text at arbitrary positions and sizes:

```cpp
struct YDrawGlyph {
    float x, y;           // Position in scene coordinates
    float width, height;  // Rendered size
    uint32_t glyphIndex;  // Index into MSDF glyph metadata
    uint32_t color;       // Packed RGBA
    uint32_t layer;       // Z-ordering
};
```

Text glyphs are stored in the grid alongside SDF primitives (using bit 31 to distinguish):

```cpp
// In grid cell, index with bit 31 set = glyph, otherwise = primitive
grid[cellOffset + 1 + i] = glyphIndex | 0x80000000;  // Glyph
grid[cellOffset + 1 + i] = primIndex;                 // Primitive
```

## SDF Primitive Types

YDraw supports 40+ SDF primitive types:

### 2D Primitives (type < 100)

| Type | Parameters |
|------|------------|
| Circle | center(x,y), radius |
| Box | center(x,y), halfSize(w,h), round |
| Segment | point1(x,y), point2(x,y) |
| Triangle | p1(x,y), p2(x,y), p3(x,y) |
| Bezier2 | p0, p1(control), p2 |
| Bezier3 | p0, p1, p2, p3 |
| Ellipse | center, radii(rx,ry) |
| Arc | center, angles, radii |
| RoundedBox | center, halfSize, cornerRadii[4] |
| Rhombus | center, halfSize |
| Pentagon | center, radius |
| Hexagon | center, radius |
| Star | center, outerR, innerR, points |
| Heart | center, size |
| Cross | center, size, thickness |
| Capsule | p1, p2, radius |
| ... | 25+ more shapes |

### 3D Primitives (type >= 100)

Raymarched in shader, not grid-culled:

| Type | Parameters |
|------|------------|
| Sphere3D | center, radius |
| Box3D | center, halfSize |
| Torus3D | center, radii |
| Cylinder3D | height, radius |
| Cone3D | height, radius |
| ... | 15+ more 3D shapes |

## Shader Bindings

Card resources are bound to the main render shader:

```wgsl
// Group 0: Global uniforms
@group(0) @binding(0) var<uniform> globals: Globals;

// Group 1: Font resources (see fonts.md)

// Group 2: Card resources
@group(2) @binding(0) var<storage, read> cardMetadata: array<u32>;
@group(2) @binding(1) var<storage, read> cardStorage: array<f32>;
@group(2) @binding(2) var cardAtlas: texture_2d<f32>;
@group(2) @binding(3) var cardSampler: sampler;
```

## Key Source Files

| File | Purpose |
|------|---------|
| `include/yetty/card.h` | Card base class |
| `include/yetty/card-manager.h` | Metadata allocation |
| `include/yetty/card-buffer-manager.h` | Storage buffer management |
| `include/yetty/card-texture-manager.h` | Texture atlas management |
| `src/yetty/gpu-screen.cpp` | Lifecycle integration |
| `src/yetty/cards/image/` | Image card |
| `src/yetty/cards/plot/` | Plot card |
| `src/yetty/cards/pdf/` | PDF card |
| `src/yetty/cards/ydraw/` | YDraw card (parser) |
| `src/yetty/ydraw/ydraw-base.cpp` | YDraw core + spatial hash |
| `src/yetty/shaders/cards/` | Card shader implementations |
