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
│  - Linear storage buffer    │      │  - Dynamic texture atlas    │
│  - Free-list allocator      │      │  - Strip packing algorithm  │
│  - Direct CPU write access  │      │  - Auto-grow (2K→8K)        │
│  - Dirty region coalescing  │      │  - Rebuild on card change   │
└─────────────────────────────┘      └─────────────────────────────┘
              │                                    │
              ▼                                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│                          Card Base Class                             │
│  - needsBuffer() / needsTexture() - declare resource type           │
│  - declareBufferNeeds() → allocateBuffers() → allocateTextures()    │
│  - render(time) - per-frame update                                   │
│  - suspend() / dispose() - lifecycle management                      │
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

### Storage Buffer (CardBufferManager)

Used by cards that store structured data: plots (float arrays), YDraw (SDF primitives, spatial hash grid).

**Characteristics:**
- Linear GPU storage buffer (SSBO)
- Direct CPU write access via raw pointers
- 16-byte alignment, max 32MB
- Dirty region tracking with coalesced uploads

**Three-Phase Allocation Protocol:**

```cpp
// Phase 1: Cards declare total space needed
card->declareBufferNeeds();     // calls bufferMgr->reserve(size)

// Phase 2: Buffer pre-allocated to fit all needs
bufferMgr->commitReservations();

// Phase 3: Cards receive stable pointers
StorageHandle handle = bufferMgr->allocateBuffer(size);
memcpy(handle.data, myData, size);  // Write directly to CPU buffer
bufferMgr->markBufferDirty(handle);
```

**Why Three Phases:**
- Ensures buffer grows exactly once to fit all needs
- Pointers remain stable after Phase 2 (no reallocation)
- Cards can write directly without intermediate copies

**StorageHandle Structure:**
```cpp
struct StorageHandle {
    uint8_t* data;      // Direct CPU pointer - write here
    uint32_t offset;    // Offset in GPU buffer (bytes)
    uint32_t size;      // Allocation size
};
```

### Texture Atlas (CardTextureManager)

Used by cards that render images: Image, PDF, ThorVG.

**Characteristics:**
- Single RGBA8 texture atlas
- Initial size: 2048×2048, grows to 8192×8192
- Strip packing algorithm (sort by height)
- Rebuilt when cards enter/leave viewport

**Allocation Protocol:**

```cpp
// Allocate space in atlas
TextureHandle handle = textureMgr->allocate(width, height);

// Write pixel data
textureMgr->write(handle, pixelData);

// Query position after atlas is built
AtlasPosition pos = textureMgr->getAtlasPosition(handle);
```

**Dynamic Atlas Rebuilding:**

When texture cards enter or leave the visible viewport:

1. GPU-screen sets `_textureLayoutChanged = true`
2. On next frame, all texture cards call `allocateTextures()`
3. `CardTextureManager::createAtlas()` repacks the atlas
4. Cards query their new atlas positions
5. Old deallocated textures are excluded from new packing

```
┌─────────────────────────────────────────┐
│  Card A enters viewport                 │
│  ─────────────────────────────────────  │
│  1. card->allocateTextures()            │
│  2. textureMgr->allocate(w, h)          │
│  3. textureMgr->write(handle, pixels)   │
│  4. gpu-screen: createAtlas()           │
│  5. Strip packing runs                  │
│  6. Atlas texture uploaded to GPU       │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│  Card B leaves viewport                 │
│  ─────────────────────────────────────  │
│  1. card->suspend() called              │
│  2. Later: card->dispose()              │
│  3. textureMgr->deallocate(handle)      │
│  4. Next texture change: createAtlas()  │
│  5. Repacking excludes Card B           │
└─────────────────────────────────────────┘
```

## Card Lifecycle

### Lifecycle Methods

```cpp
class Card : public base::EventListener {
public:
    // Resource declaration (called once during init)
    virtual bool needsBuffer() const { return false; }
    virtual bool needsTexture() const { return false; }

    // Three-loop allocation (called when cards enter viewport)
    virtual void declareBufferNeeds() {}           // Loop 1: reserve space
    virtual Result<void> allocateBuffers() {}      // Loop 2: get pointers
    virtual Result<void> allocateTextures() {}     // Loop 3: texture handles

    // Per-frame rendering
    virtual Result<void> render(float time) = 0;

    // Lifecycle termination
    virtual void suspend() {}                      // Pause, keep resources
    virtual Result<void> dispose() = 0;            // Release all resources
};
```

### GPU-Screen Integration

The three-loop lifecycle runs in `gpu-screen.cpp` when cards change:

```cpp
void GPUScreen::render() {
    if (_bufferLayoutChanged) {
        // Loop 1: Declare buffer needs
        for (auto& card : _cards) {
            if (card->needsBuffer())
                card->declareBufferNeeds();
        }

        // Pre-allocate total buffer
        _bufferMgr->commitReservations();

        // Loop 2: Allocate buffers
        for (auto& card : _cards) {
            if (card->needsBuffer())
                card->allocateBuffers();
        }

        _bufferLayoutChanged = false;
    }

    if (_textureLayoutChanged) {
        // Loop 3: Allocate textures
        for (auto& card : _cards) {
            if (card->needsTexture())
                card->allocateTextures();
        }

        // Pack atlas
        _textureMgr->createAtlas();

        _textureLayoutChanged = false;
    }

    // Per-frame render
    for (auto& card : _cards)
        card->render(time);

    // Upload dirty regions to GPU
    _cardMgr->flush(queue);
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
