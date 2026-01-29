// Compute shader: GPU-driven image atlas population with scaling
//
// =============================================================================
// DESIGN: GPU-Driven Atlas with Image Scaling
// =============================================================================
//
// The CPU (Texture card) uploads:
//   - cardMetadata buffer (image info: dataOffset, dimensions, scaledWidth/Height)
//   - cardTextureData buffer (raw RGBA pixels at original resolution)
//
// The Texture card is responsible for calculating scaledWidth/scaledHeight
// based on its cell dimensions and writing them to metadata.
//
// This compute shader:
//   1. Scans metadata slots to find valid images (width > 0, height > 0)
//   2. Reads scaledWidth/scaledHeight from metadata (set by Texture card)
//   3. Falls back to original dimensions if scaled dimensions are 0
//   4. Allocates atlas positions for scaled images
//   5. Copies pixels from cardTextureData to atlas WITH SCALING (bilinear)
//   6. Writes atlasX/atlasY back to cardMetadata
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  imageDataOffset (u32)  - byte offset in cardTextureData buffer
//   offset 1:  imageWidth (u32)       - ORIGINAL image width in pixels
//   offset 2:  imageHeight (u32)      - ORIGINAL image height in pixels
//   offset 3:  atlasX (u32)           - X position in atlas (written by compute)
//   offset 4:  atlasY (u32)           - Y position in atlas (written by compute)
//   offset 5:  widthCells (u32)       - card width in cells
//   offset 6:  heightCells (u32)      - card height in cells
//   offset 7:  zoom (f32)             - zoom level (1.0 = fit)
//   offset 8:  centerX (f32)          - pan center X [0,1]
//   offset 9:  centerY (f32)          - pan center Y [0,1]
//   offset 10: flags (u32)            - bit 0: bilinear, bit 1: preserve aspect
//   offset 11: bgColor (u32)          - background color (packed RGBA)
//   offset 12: scaledWidth (u32)      - SCALED width for atlas (written by compute)
//   offset 13: scaledHeight (u32)     - SCALED height for atlas (written by compute)
//   offset 14-15: reserved
// =============================================================================

// Cell structure - matches CPU Cell struct
struct Cell {
    glyph: u32,
    fg: u32,
    bg: u32,
}

// Atlas allocation state
struct AtlasState {
    nextX: atomic<u32>,
    nextY: atomic<u32>,
    rowHeight: atomic<u32>,
    atlasWidth: u32,
    atlasHeight: u32,
    maxSlots: u32,       // Max metadata slots to scan
    _padding: array<u32, 4>,
}

// Image card glyph constant
const IMAGE_SHADER_GLYPH: u32 = 0x100000u;

// Flags
const IMAGE_FLAG_PRESERVE_ASPECT: u32 = 2u;

// Bindings
@group(0) @binding(0) var<storage, read> cellBuffer: array<Cell>;
@group(0) @binding(1) var<storage, read_write> cardMetadata: array<u32>;
@group(0) @binding(2) var<storage, read> cardTextureData: array<u32>;
@group(0) @binding(3) var atlasTexture: texture_storage_2d<rgba8unorm, write>;
@group(0) @binding(4) var<storage, read_write> atlasState: AtlasState;
@group(0) @binding(5) var<storage, read_write> processedCards: array<atomic<u32>>;

// =============================================================================
// Helper: Bilinear sample from cardTextureData buffer
// =============================================================================
fn sampleImageBilinear(imageDataOffset: u32, imageWidth: u32, imageHeight: u32, uv: vec2<f32>) -> vec4<f32> {
    // Clamp UV to valid range
    let clampedUV = clamp(uv, vec2<f32>(0.0), vec2<f32>(1.0));

    // Convert to pixel coordinates (0 to width-1, 0 to height-1)
    let pixelCoord = clampedUV * vec2<f32>(f32(imageWidth) - 1.0, f32(imageHeight) - 1.0);

    // Get integer and fractional parts
    let x0 = u32(floor(pixelCoord.x));
    let y0 = u32(floor(pixelCoord.y));
    let x1 = min(x0 + 1u, imageWidth - 1u);
    let y1 = min(y0 + 1u, imageHeight - 1u);
    let fx = fract(pixelCoord.x);
    let fy = fract(pixelCoord.y);

    // Calculate buffer indices
    let baseOffset = imageDataOffset / 4u;
    let idx00 = baseOffset + y0 * imageWidth + x0;
    let idx10 = baseOffset + y0 * imageWidth + x1;
    let idx01 = baseOffset + y1 * imageWidth + x0;
    let idx11 = baseOffset + y1 * imageWidth + x1;

    // Read packed colors
    let c00 = cardTextureData[idx00];
    let c10 = cardTextureData[idx10];
    let c01 = cardTextureData[idx01];
    let c11 = cardTextureData[idx11];

    // Unpack RGBA8
    let color00 = vec4<f32>(
        f32(c00 & 0xFFu) / 255.0,
        f32((c00 >> 8u) & 0xFFu) / 255.0,
        f32((c00 >> 16u) & 0xFFu) / 255.0,
        f32((c00 >> 24u) & 0xFFu) / 255.0
    );
    let color10 = vec4<f32>(
        f32(c10 & 0xFFu) / 255.0,
        f32((c10 >> 8u) & 0xFFu) / 255.0,
        f32((c10 >> 16u) & 0xFFu) / 255.0,
        f32((c10 >> 24u) & 0xFFu) / 255.0
    );
    let color01 = vec4<f32>(
        f32(c01 & 0xFFu) / 255.0,
        f32((c01 >> 8u) & 0xFFu) / 255.0,
        f32((c01 >> 16u) & 0xFFu) / 255.0,
        f32((c01 >> 24u) & 0xFFu) / 255.0
    );
    let color11 = vec4<f32>(
        f32(c11 & 0xFFu) / 255.0,
        f32((c11 >> 8u) & 0xFFu) / 255.0,
        f32((c11 >> 16u) & 0xFFu) / 255.0,
        f32((c11 >> 24u) & 0xFFu) / 255.0
    );

    // Bilinear interpolation
    let top = mix(color00, color10, fx);
    let bottom = mix(color01, color11, fx);
    return mix(top, bottom, fy);
}

// =============================================================================
// Helper: Calculate scaled dimensions preserving aspect ratio
// =============================================================================
fn calculateScaledDimensions(imageWidth: u32, imageHeight: u32,
                              targetWidth: u32, targetHeight: u32,
                              preserveAspect: bool) -> vec2<u32> {
    if (!preserveAspect) {
        return vec2<u32>(targetWidth, targetHeight);
    }

    let imageAspect = f32(imageWidth) / f32(imageHeight);
    let targetAspect = f32(targetWidth) / f32(targetHeight);

    var scaledWidth: u32;
    var scaledHeight: u32;

    if (imageAspect > targetAspect) {
        // Image is wider - fit to width
        scaledWidth = targetWidth;
        scaledHeight = u32(f32(targetWidth) / imageAspect);
    } else {
        // Image is taller - fit to height
        scaledHeight = targetHeight;
        scaledWidth = u32(f32(targetHeight) * imageAspect);
    }

    // Ensure at least 1 pixel
    scaledWidth = max(scaledWidth, 1u);
    scaledHeight = max(scaledHeight, 1u);

    return vec2<u32>(scaledWidth, scaledHeight);
}

// =============================================================================
// Phase 1: Scan metadata slots, read scaled dimensions, allocate atlas
// =============================================================================
// NOTE: The Texture card is responsible for calculating and storing
// scaledWidth/scaledHeight in its metadata. This shader just reads them.
// If scaledWidth/scaledHeight are 0, we fall back to original dimensions.
// =============================================================================
@compute @workgroup_size(256, 1, 1)
fn scanMetadataSlots(@builtin(global_invocation_id) gid: vec3<u32>) {
    let slotIndex = gid.x;

    if (slotIndex >= atlasState.maxSlots) {
        return;
    }

    // Read metadata for this slot (64 bytes = 16 u32s per slot)
    let metaOffset = slotIndex * 16u;
    let imageDataOffset = cardMetadata[metaOffset + 0u];
    let imageWidth = cardMetadata[metaOffset + 1u];
    let imageHeight = cardMetadata[metaOffset + 2u];
    var scaledWidth = cardMetadata[metaOffset + 12u];
    var scaledHeight = cardMetadata[metaOffset + 13u];

    // Skip if no valid image data
    if (imageWidth == 0u || imageHeight == 0u) {
        return;
    }

    // Use scaled dimensions if set by Texture card, otherwise fall back to original
    if (scaledWidth == 0u || scaledHeight == 0u) {
        scaledWidth = imageWidth;
        scaledHeight = imageHeight;
        // Write back so copyPixels can read them
        cardMetadata[metaOffset + 12u] = scaledWidth;
        cardMetadata[metaOffset + 13u] = scaledHeight;
    }

    // Mark as processed
    atomicStore(&processedCards[slotIndex], 1u);

    // Allocate atlas position using scaled dimensions
    var atlasX: u32;
    var atlasY: u32;

    let prevX = atomicAdd(&atlasState.nextX, scaledWidth);

    if (prevX + scaledWidth <= atlasState.atlasWidth) {
        atlasX = prevX;
        atlasY = atomicLoad(&atlasState.nextY);
        atomicMax(&atlasState.rowHeight, scaledHeight);
    } else {
        let currentRowHeight = atomicLoad(&atlasState.rowHeight);
        let newY = atomicAdd(&atlasState.nextY, currentRowHeight);
        atomicStore(&atlasState.nextX, scaledWidth);
        atomicStore(&atlasState.rowHeight, scaledHeight);
        atlasX = 0u;
        atlasY = newY + currentRowHeight;
    }

    // Check if we exceeded atlas bounds
    if (atlasY + scaledHeight > atlasState.atlasHeight) {
        atlasX = 0xFFFFFFFFu;
        atlasY = 0xFFFFFFFFu;
    }

    // Write atlas position back to metadata
    cardMetadata[metaOffset + 3u] = atlasX;
    cardMetadata[metaOffset + 4u] = atlasY;
}

// =============================================================================
// Phase 2: Copy pixels to atlas WITH SCALING (bilinear interpolation)
// =============================================================================
@compute @workgroup_size(16, 16, 1)
fn copyPixels(@builtin(global_invocation_id) gid: vec3<u32>,
              @builtin(workgroup_id) wgid: vec3<u32>) {
    // wgid.z is the card slot index (dispatched per visible card)
    let slotIndex = wgid.z;

    // Check if this card was processed
    if (atomicLoad(&processedCards[slotIndex]) == 0u) {
        return;
    }

    // Read metadata (64 bytes = 16 u32s per slot)
    let metaOffset = slotIndex * 16u;
    let imageDataOffset = cardMetadata[metaOffset + 0u];
    let imageWidth = cardMetadata[metaOffset + 1u];   // Original dimensions
    let imageHeight = cardMetadata[metaOffset + 2u];
    let atlasX = cardMetadata[metaOffset + 3u];
    let atlasY = cardMetadata[metaOffset + 4u];
    let scaledWidth = cardMetadata[metaOffset + 12u];  // Scaled dimensions
    let scaledHeight = cardMetadata[metaOffset + 13u];

    // Check for invalid atlas position
    if (atlasX == 0xFFFFFFFFu || atlasY == 0xFFFFFFFFu) {
        return;
    }

    // Local pixel coordinates within the SCALED output
    let localX = gid.x;
    let localY = gid.y;

    if (localX >= scaledWidth || localY >= scaledHeight) {
        return;
    }

    // Calculate UV in source image (0-1 range)
    let sourceUV = vec2<f32>(
        (f32(localX) + 0.5) / f32(scaledWidth),
        (f32(localY) + 0.5) / f32(scaledHeight)
    );

    // Sample from source with bilinear interpolation
    let color = sampleImageBilinear(imageDataOffset, imageWidth, imageHeight, sourceUV);

    // Write to atlas at scaled position
    let atlasPos = vec2<u32>(atlasX + localX, atlasY + localY);
    textureStore(atlasTexture, atlasPos, color);
}

// =============================================================================
// DEPRECATED: scanAndAllocate_ORIGINAL - no longer used
// Kept for reference only. Use scanMetadataSlots instead.
// =============================================================================

// Placeholder main
@compute @workgroup_size(8, 8, 1)
fn main(@builtin(global_invocation_id) gid: vec3<u32>) {
}
