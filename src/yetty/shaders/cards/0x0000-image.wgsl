// Shader glyph: Image widget (codepoint 1048576 / U+100000)
// Samples from atlas texture populated by compute shader
//
// =============================================================================
// DESIGN: GPU-Driven Atlas Sampling
// =============================================================================
//
// The compute shader (image-atlas-copy.wgsl) has:
//   1. Scanned visible cells for image card glyphs
//   2. Allocated atlas positions for visible cards
//   3. Copied pixels to atlas texture
//   4. Written atlasX/atlasY to cardMetadata
//
// This fragment shader reads atlasX/atlasY from metadata and samples the atlas.
//
// =============================================================================
// Per-cell encoding (ANSI true-color compatible):
//
//   fg (24 bits): Metadata SLOT INDEX (encodes byte offset / 32)
//     - slotIndex = byteOffset / 32
//     - Shader: metaOffset_u32 = slotIndex * 8 = byteOffset / 4
//
//   bg (24 bits): Relative position within widget
//     - Lower 12 bits = relCol, upper 12 bits = relRow
//     - Shader computes widget-wide UV from cell position
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  imageDataOffset (u32) - byte offset in cardImageData buffer
//   offset 1:  imageWidth (u32)      - image width in pixels
//   offset 2:  imageHeight (u32)     - image height in pixels
//   offset 3:  atlasX (u32)          - X position in atlas (written by compute shader)
//   offset 4:  atlasY (u32)          - Y position in atlas (written by compute shader)
//   offset 5:  widthCells (u32)      - widget width in cells
//   offset 6:  heightCells (u32)     - widget height in cells
//   offset 7:  zoom (f32)            - zoom level (1.0 = fit)
//   offset 8:  centerX (f32)         - pan center X [0,1]
//   offset 9:  centerY (f32)         - pan center Y [0,1]
//   offset 10: flags (u32)           - bit 0: bilinear, bit 1: preserve aspect
//   offset 11: bgColor (u32)         - background color (packed RGBA)
//
// Note: Cell dimensions come from grid.cellSize uniform (GridUniforms)
// =============================================================================

const IMAGE_FLAG_BILINEAR: u32 = 1u;
const IMAGE_FLAG_PRESERVE_ASPECT: u32 = 2u;

fn shaderGlyph_1048576(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    // ==========================================================================
    // Decode ANSI-compatible cell encoding
    // ==========================================================================

    // fg (24 bits): metadata slot index
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 8u;  // 32-byte slots = 8 u32s per slot

    // bg (24 bits): relative position within widget
    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;           // Bits 0-11
    let relRow = (bg24 >> 12u) & 0xFFFu;  // Bits 12-23

    // ==========================================================================
    // Read metadata from cardMetadata buffer
    // ==========================================================================
    let imageDataOffset = cardMetadata[metaOffset + 0u];
    let imageWidth = cardMetadata[metaOffset + 1u];
    let imageHeight = cardMetadata[metaOffset + 2u];
    let atlasX = cardMetadata[metaOffset + 3u];
    let atlasY = cardMetadata[metaOffset + 4u];
    let widthCells = cardMetadata[metaOffset + 5u];
    let heightCells = cardMetadata[metaOffset + 6u];
    let zoom = bitcast<f32>(cardMetadata[metaOffset + 7u]);
    let centerX = bitcast<f32>(cardMetadata[metaOffset + 8u]);
    let centerY = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let flags = cardMetadata[metaOffset + 10u];
    let bgColorPacked = cardMetadata[metaOffset + 11u];

    let bgColor = unpackColor(bgColorPacked);

    // ==========================================================================
    // Compute widget-wide UV from cell position + local UV
    // ==========================================================================
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    // ==========================================================================
    // Apply zoom and pan
    // ==========================================================================
    var imageUV = (widgetUV - vec2<f32>(0.5)) / zoom + vec2<f32>(centerX, centerY);

    // ==========================================================================
    // Check bounds - show background if outside image
    // ==========================================================================
    if (imageUV.x < 0.0 || imageUV.x > 1.0 || imageUV.y < 0.0 || imageUV.y > 1.0) {
        return bgColor;
    }

    // ==========================================================================
    // Sample from atlas texture
    // ==========================================================================
    let atlasSize = vec2<f32>(textureDimensions(cardImageAtlas));
    let atlasUV = (vec2<f32>(f32(atlasX), f32(atlasY)) +
                   imageUV * vec2<f32>(f32(imageWidth), f32(imageHeight))) / atlasSize;

    var color: vec4<f32>;
    if ((flags & IMAGE_FLAG_BILINEAR) != 0u) {
        // Use textureSampleLevel for Dawn compatibility (no derivatives needed)
        color = textureSampleLevel(cardImageAtlas, cardImageSampler, atlasUV, 0.0);
    } else {
        let texelCoord = vec2<i32>(atlasUV * atlasSize);
        color = textureLoad(cardImageAtlas, texelCoord, 0);
    }

    // Alpha blending with background
    return mix(bgColor, color.rgb, color.a);
}
