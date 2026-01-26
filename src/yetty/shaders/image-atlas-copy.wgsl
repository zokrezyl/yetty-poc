// Compute shader: GPU-driven image atlas population
//
// =============================================================================
// DESIGN: Fully GPU-Driven Atlas
// =============================================================================
//
// The CPU uploads:
//   - cardMetadata buffer (image info: dataOffset, dimensions, etc.)
//   - cardImageData buffer (raw RGBA pixels)
//
// This compute shader:
//   1. Scans metadata slots to find valid images (width > 0, height > 0)
//   2. Allocates atlas positions using atomic counter
//   3. Copies pixels from cardImageData to atlas texture
//   4. Writes atlasX/atlasY back to cardMetadata
//
// =============================================================================
// CHANGE LOG:
// -----------------------------------------------------------------------------
// Original implementation (main branch, commit c1a0446 "Image card (#39)"):
//   - Scanned CELL BUFFER to find visible image card glyphs (0x100000)
//   - Extracted metadata slot index from fg color
//   - Only processed images that were currently visible on screen
//
// Current implementation (ui-refactor branch):
//   - Scans METADATA SLOTS directly (no cell buffer dependency)
//   - Processes ALL allocated images, not just visible ones
//   - Simpler integration: flush() can call prepareAtlas() without cell buffer
//
// TODO: Re-add visibility optimization. GPUScreen will notify CardBufferManager
// which cards are visible/off-screen, allowing us to skip off-screen images.
// The original cell-scanning approach is preserved in scanAndAllocate_ORIGINAL().
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
    gridCols: u32,
    gridRows: u32,
    _padding: u32,
}

// Image card glyph constant
const IMAGE_SHADER_GLYPH: u32 = 0x100000u;

// Bindings
@group(0) @binding(0) var<storage, read> cellBuffer: array<Cell>;
@group(0) @binding(1) var<storage, read_write> cardMetadata: array<u32>;
@group(0) @binding(2) var<storage, read> cardImageData: array<u32>;
@group(0) @binding(3) var atlasTexture: texture_storage_2d<rgba8unorm, write>;
@group(0) @binding(4) var<storage, read_write> atlasState: AtlasState;
@group(0) @binding(5) var<storage, read_write> processedCards: array<atomic<u32>>;

// =============================================================================
// Phase 1: Scan metadata slots directly (CURRENT - no cell buffer needed)
// =============================================================================
// Iterates over all metadata slots and processes those with valid image data.
// This processes ALL images, not just visible ones.
// Dispatch with: workgroups = ceil(maxSlots / 256)
@compute @workgroup_size(256, 1, 1)
fn scanMetadataSlots(@builtin(global_invocation_id) gid: vec3<u32>) {
    let slotIndex = gid.x;

    // atlasState.gridCols is repurposed as maxSlots for this entry point
    let maxSlots = atlasState.gridCols;
    if (slotIndex >= maxSlots) {
        return;
    }

    // Read metadata for this slot
    let metaOffset = slotIndex * 8u;  // 32-byte slots = 8 u32s
    let imageDataOffset = cardMetadata[metaOffset + 0u];
    let imageWidth = cardMetadata[metaOffset + 1u];
    let imageHeight = cardMetadata[metaOffset + 2u];

    // Skip if no valid image data
    if (imageWidth == 0u || imageHeight == 0u) {
        return;
    }

    // Mark as processed
    atomicStore(&processedCards[slotIndex], 1u);

    // Allocate atlas position using simple row-based packing
    var atlasX: u32;
    var atlasY: u32;

    // Atomically allocate X position
    let prevX = atomicAdd(&atlasState.nextX, imageWidth);

    if (prevX + imageWidth <= atlasState.atlasWidth) {
        // Fits in current row
        atlasX = prevX;
        atlasY = atomicLoad(&atlasState.nextY);
        atomicMax(&atlasState.rowHeight, imageHeight);
    } else {
        // Need new row
        let currentRowHeight = atomicLoad(&atlasState.rowHeight);
        let newY = atomicAdd(&atlasState.nextY, currentRowHeight);
        atomicStore(&atlasState.nextX, imageWidth);
        atomicStore(&atlasState.rowHeight, imageHeight);
        atlasX = 0u;
        atlasY = newY + currentRowHeight;
    }

    // Check if we exceeded atlas bounds
    if (atlasY + imageHeight > atlasState.atlasHeight) {
        atlasX = 0xFFFFFFFFu;
        atlasY = 0xFFFFFFFFu;
    }

    // Write atlas position back to metadata
    cardMetadata[metaOffset + 3u] = atlasX;
    cardMetadata[metaOffset + 4u] = atlasY;
}

// =============================================================================
// Phase 1 ORIGINAL: Scan cells for visible images (PRESERVED FOR FUTURE USE)
// =============================================================================
// This is the original implementation from main branch (commit c1a0446).
// It scans the cell buffer to find visible image card glyphs, which means
// only images currently on screen get atlas space - more efficient but
// requires cell buffer dependency.
//
// TODO: Re-enable this when GPUScreen notifies CardBufferManager about
// visible cards. For now, use scanMetadataSlots() instead.
@compute @workgroup_size(256, 1, 1)
fn scanAndAllocate_ORIGINAL(@builtin(global_invocation_id) gid: vec3<u32>) {
    let cellIndex = gid.x;
    let totalCells = atlasState.gridCols * atlasState.gridRows;

    if (cellIndex >= totalCells) {
        return;
    }

    let cell = cellBuffer[cellIndex];

    // Check if this is an image card glyph
    if (cell.glyph != IMAGE_SHADER_GLYPH) {
        return;
    }

    // Extract metadata slot index from fg (24 bits)
    let slotIndex = cell.fg & 0xFFFFFFu;

    // Check if this card was already processed (use atomic to ensure only one thread processes each card)
    let wasProcessed = atomicExchange(&processedCards[slotIndex], 1u);
    if (wasProcessed != 0u) {
        return;
    }

    // Read metadata for this card
    let metaOffset = slotIndex * 8u;  // 32-byte slots = 8 u32s
    let imageDataOffset = cardMetadata[metaOffset + 0u];
    let imageWidth = cardMetadata[metaOffset + 1u];
    let imageHeight = cardMetadata[metaOffset + 2u];

    // Skip if no image data
    if (imageWidth == 0u || imageHeight == 0u) {
        return;
    }

    // Allocate atlas position using simple row-based packing
    // Try to fit in current row, otherwise start new row
    var atlasX: u32;
    var atlasY: u32;

    // Atomically allocate X position
    let prevX = atomicAdd(&atlasState.nextX, imageWidth);

    if (prevX + imageWidth <= atlasState.atlasWidth) {
        // Fits in current row
        atlasX = prevX;
        atlasY = atomicLoad(&atlasState.nextY);
        // Update row height if this image is taller
        atomicMax(&atlasState.rowHeight, imageHeight);
    } else {
        // Need new row - this is a simplification, may have race conditions
        // In production, would need more sophisticated packing
        let currentRowHeight = atomicLoad(&atlasState.rowHeight);
        let newY = atomicAdd(&atlasState.nextY, currentRowHeight);
        atomicStore(&atlasState.nextX, imageWidth);
        atomicStore(&atlasState.rowHeight, imageHeight);
        atlasX = 0u;
        atlasY = newY + currentRowHeight;
    }

    // Check if we exceeded atlas bounds
    if (atlasY + imageHeight > atlasState.atlasHeight) {
        // Atlas full - mark as invalid position
        atlasX = 0xFFFFFFFFu;
        atlasY = 0xFFFFFFFFu;
    }

    // Write atlas position back to metadata
    cardMetadata[metaOffset + 3u] = atlasX;  // atlasX
    cardMetadata[metaOffset + 4u] = atlasY;  // atlasY
}

// Phase 2: Copy pixels to atlas
@compute @workgroup_size(16, 16, 1)
fn copyPixels(@builtin(global_invocation_id) gid: vec3<u32>,
              @builtin(workgroup_id) wgid: vec3<u32>) {
    // wgid.z is the card slot index (dispatched per visible card)
    let slotIndex = wgid.z;

    // Check if this card was processed
    if (atomicLoad(&processedCards[slotIndex]) == 0u) {
        return;
    }

    // Read metadata
    let metaOffset = slotIndex * 8u;
    let imageDataOffset = cardMetadata[metaOffset + 0u];
    let imageWidth = cardMetadata[metaOffset + 1u];
    let imageHeight = cardMetadata[metaOffset + 2u];
    let atlasX = cardMetadata[metaOffset + 3u];
    let atlasY = cardMetadata[metaOffset + 4u];

    // Check for invalid atlas position
    if (atlasX == 0xFFFFFFFFu || atlasY == 0xFFFFFFFFu) {
        return;
    }

    // Local pixel coordinates within this image
    let localX = gid.x;
    let localY = gid.y;

    if (localX >= imageWidth || localY >= imageHeight) {
        return;
    }

    // Read pixel from buffer
    let pixelIndex = imageDataOffset / 4u + localY * imageWidth + localX;
    let packedColor = cardImageData[pixelIndex];

    // Unpack RGBA8
    let color = vec4<f32>(
        f32(packedColor & 0xFFu) / 255.0,
        f32((packedColor >> 8u) & 0xFFu) / 255.0,
        f32((packedColor >> 16u) & 0xFFu) / 255.0,
        f32((packedColor >> 24u) & 0xFFu) / 255.0
    );

    // Write to atlas
    let atlasPos = vec2<u32>(atlasX + localX, atlasY + localY);
    textureStore(atlasTexture, atlasPos, color);
}

// Combined single-pass version (alternative - simpler but less efficient)
@compute @workgroup_size(8, 8, 1)
fn main(@builtin(global_invocation_id) gid: vec3<u32>) {
    // This is a placeholder - use scanAndAllocate + copyPixels for proper implementation
    // The two-phase approach is needed because allocation must complete before copying
}
