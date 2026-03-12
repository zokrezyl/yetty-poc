// YPaint Overlay Shader
// Renders ypaint SDF primitives as a fullscreen transparent overlay.
// Uses the same SDF evaluation as the ypaint card shader.
//
// Bind group layout:
//   @group(0) @binding(0): uniform  YPaintOverlayUniforms
//   @group(0) @binding(1): storage  cardStorage (prim + grid + glyph data)
//   @group(0) @binding(2): texture  fontTexture (shared MSDF atlas)
//   @group(0) @binding(3): sampler  fontSampler
//   @group(0) @binding(4): storage  glyphMetadata (font glyph UV metadata)

struct YPaintOverlayUniforms {
    screenWidth: f32,
    screenHeight: f32,
    primitiveOffset: u32,
    primitiveCount: u32,
    gridOffset: u32,
    gridWidth: u32,
    gridHeight: u32,
    cellSizeXY: u32,  // f16 pair
    glyphOffset: u32,
    glyphCount: u32,
    sceneMinX: u32,   // f32 as bits
    sceneMinY: u32,
    sceneMaxX: u32,
    sceneMaxY: u32,
    widthCells: u32,
    heightCells: u32,
    flags: u32,
    bgColor: u32,
    pixelRange: f32,
    _pad: vec3<f32>,
};

struct GlyphMetadata {
    uvMin: vec2<f32>,
    uvMax: vec2<f32>,
    size: vec2<f32>,
    bearing: vec2<f32>,
    advance: f32,
    _pad: f32,
};

@group(0) @binding(0) var<uniform> overlay: YPaintOverlayUniforms;
@group(0) @binding(1) var<storage, read> cardStorage: array<u32>;
@group(0) @binding(2) var fontTexture: texture_2d<f32>;
@group(0) @binding(3) var fontSampler: sampler;
@group(0) @binding(4) var<storage, read> glyphMetadata: array<GlyphMetadata>;

// === Fullscreen triangle vertex shader ===

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
};

@vertex
fn vs_main(@builtin(vertex_index) vertexIndex: u32) -> VertexOutput {
    var out: VertexOutput;
    let x = f32(i32(vertexIndex & 1u)) * 4.0 - 1.0;
    let y = f32(i32(vertexIndex >> 1u)) * 4.0 - 1.0;
    out.position = vec4<f32>(x, y, 0.0, 1.0);
    out.uv = vec2<f32>((x + 1.0) * 0.5, (1.0 - y) * 0.5);
    return out;
}

// === Helper functions ===

const GLYPH_BIT: u32 = 0x80000000u;
const INDEX_MASK: u32 = 0x7FFFFFFFu;
const MAX_ENTRIES_PER_CELL: u32 = 4096u;

fn f16ToF32(h: u32) -> f32 {
    let sign = (h & 0x8000u) << 16u;
    let exp = (h >> 10u) & 0x1Fu;
    let mant = h & 0x3FFu;
    var f32bits: u32;
    if (exp == 0u) {
        f32bits = sign;
    } else if (exp == 31u) {
        f32bits = sign | 0x7F800000u;
    } else {
        f32bits = sign | ((exp + 112u) << 23u) | (mant << 13u);
    }
    return bitcast<f32>(f32bits);
}

fn unpackColor(packed: u32) -> vec3<f32> {
    return vec3<f32>(
        f32(packed & 0xFFu) / 255.0,
        f32((packed >> 8u) & 0xFFu) / 255.0,
        f32((packed >> 16u) & 0xFFu) / 255.0
    );
}

fn unpackAlpha(packed: u32) -> f32 {
    return f32((packed >> 24u) & 0xFFu) / 255.0;
}

fn median(r: f32, g: f32, b: f32) -> f32 {
    return max(min(r, g), min(max(r, g), b));
}

// === Fragment shader ===

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    // Convert UV to screen pixel coordinates
    let screenPos = vec2<f32>(in.uv.x * overlay.screenWidth, in.uv.y * overlay.screenHeight);

    // Get scene bounds
    let sceneMinX = bitcast<f32>(overlay.sceneMinX);
    let sceneMinY = bitcast<f32>(overlay.sceneMinY);
    let sceneMaxX = bitcast<f32>(overlay.sceneMaxX);
    let sceneMaxY = bitcast<f32>(overlay.sceneMaxY);

    // Map screen position to scene position (1:1 for overlay)
    let scenePos = screenPos;

    // Get cell sizes
    let cellSizeX = f16ToF32(overlay.cellSizeXY & 0xFFFFu);
    let cellSizeY = f16ToF32((overlay.cellSizeXY >> 16u) & 0xFFFFu);

    // Check if inside scene bounds
    if (scenePos.x < sceneMinX || scenePos.x > sceneMaxX ||
        scenePos.y < sceneMinY || scenePos.y > sceneMaxY) {
        return vec4<f32>(0.0, 0.0, 0.0, 0.0);
    }

    // No content check
    if (overlay.primitiveCount == 0u && overlay.glyphCount == 0u) {
        return vec4<f32>(0.0, 0.0, 0.0, 0.0);
    }

    // Compute grid cell
    let gridX = u32(floor((scenePos.x - sceneMinX) / cellSizeX));
    let gridY = u32(floor((scenePos.y - sceneMinY) / cellSizeY));

    if (gridX >= overlay.gridWidth || gridY >= overlay.gridHeight) {
        return vec4<f32>(0.0, 0.0, 0.0, 0.0);
    }

    // Get cell data from grid
    let cellIndex = gridY * overlay.gridWidth + gridX;
    let gridBaseOffset = overlay.gridOffset;  // Already in u32/float word offset
    let cellOffset = cardStorage[gridBaseOffset + cellIndex];

    if (cellOffset == 0u) {
        return vec4<f32>(0.0, 0.0, 0.0, 0.0);
    }

    // Process entries in cell
    let entryCount = min(cardStorage[gridBaseOffset + cellOffset], MAX_ENTRIES_PER_CELL);
    var bestDist: f32 = 1e10;
    var bestColor: vec4<f32> = vec4<f32>(0.0, 0.0, 0.0, 0.0);

    for (var i = 0u; i < entryCount; i++) {
        let entry = cardStorage[gridBaseOffset + cellOffset + 1u + i];

        if ((entry & GLYPH_BIT) != 0u) {
            // This is a glyph
            let glyphIdx = entry & INDEX_MASK;
            let glyphBaseOffset = overlay.glyphOffset + glyphIdx * 5u;  // offset in u32 words

            // Read glyph data (5 u32s = 20 bytes)
            let gx = bitcast<f32>(cardStorage[glyphBaseOffset + 0u]);
            let gy = bitcast<f32>(cardStorage[glyphBaseOffset + 1u]);
            let wh = cardStorage[glyphBaseOffset + 2u];
            let glyphLayerFlags = cardStorage[glyphBaseOffset + 3u];
            let colorPacked = cardStorage[glyphBaseOffset + 4u];

            let gw = f16ToF32(wh & 0xFFFFu);
            let gh = f16ToF32((wh >> 16u) & 0xFFFFu);
            let glyphIndex = glyphLayerFlags & 0xFFFFu;

            // Check if inside glyph bounds
            if (scenePos.x >= gx && scenePos.x <= gx + gw &&
                scenePos.y >= gy && scenePos.y <= gy + gh) {

                // Sample MSDF
                let glyphMeta = glyphMetadata[glyphIndex];
                let localUV = (scenePos - vec2<f32>(gx, gy)) / vec2<f32>(gw, gh);
                let atlasUV = mix(glyphMeta.uvMin, glyphMeta.uvMax, localUV);

                let msd = textureSampleLevel(fontTexture, fontSampler, atlasUV, 0.0);
                let sd = median(msd.r, msd.g, msd.b);
                let screenPxDist = (sd - 0.5) * overlay.pixelRange;
                let alpha = clamp(screenPxDist + 0.5, 0.0, 1.0);

                if (alpha > 0.01) {
                    let color = unpackColor(colorPacked);
                    let packedAlpha = unpackAlpha(colorPacked);
                    bestColor = vec4<f32>(color, alpha * packedAlpha);
                }
            }
        } else {
            // This is a primitive - simplified SDF evaluation
            // TODO: Full primitive evaluation using ypaint-sdf.gen
            let primOffset = overlay.primitiveOffset + entry;  // offset in u32 words
            // For now, just check if we're close to any primitive
            // Full implementation would call evaluateYpaintSDF
        }
    }

    return bestColor;
}
