// Shader glyph: QRCode card (codepoint 0x100007 / 1048583)
// Renders QR code from bit-packed buffer data
//
// =============================================================================
// Metadata layout (32 bytes = 8 u32s):
//   offset 0: bufferOffset (u32)  - byte offset in cardStorage buffer
//   offset 1: qrSize (u32)        - modules per side (21, 25, 29, ...)
//   offset 2: fgColor (u32)       - foreground color (black modules) RGBA
//   offset 3: bgColor (u32)       - background color (white modules) RGBA
//   offset 4: widthCells (u32)    - card width in terminal cells
//   offset 5: heightCells (u32)   - card height in terminal cells
//   offset 6-7: reserved
//
// Buffer data: bit-packed modules (32 modules per u32, row-major)
// =============================================================================

fn shaderGlyph_1048583(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // Decode cell encoding
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;  // 64-byte slots = 16 u32s per slot
    
    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;
    
    // Read metadata
    let bufferOffset = cardMetadata[metaOffset + 0u];
    let qrSize = cardMetadata[metaOffset + 1u];
    let fgColorPacked = cardMetadata[metaOffset + 2u];
    let bgColorPacked = cardMetadata[metaOffset + 3u];
    let widthCells = cardMetadata[metaOffset + 4u];
    let heightCells = cardMetadata[metaOffset + 5u];
    
    // Unpack colors (RGBA format)
    let fgColor = vec3<f32>(
        f32((fgColorPacked >> 24u) & 0xFFu) / 255.0,
        f32((fgColorPacked >> 16u) & 0xFFu) / 255.0,
        f32((fgColorPacked >> 8u) & 0xFFu) / 255.0
    );
    let bgColor = vec3<f32>(
        f32((bgColorPacked >> 24u) & 0xFFu) / 255.0,
        f32((bgColorPacked >> 16u) & 0xFFu) / 255.0,
        f32((bgColorPacked >> 8u) & 0xFFu) / 255.0
    );
    
    // Calculate UV across entire card
    let cellWidth = grid.cellSize.x;
    let cellHeight = grid.cellSize.y;
    
    let cardPixelWidth = f32(widthCells) * cellWidth;
    let cardPixelHeight = f32(heightCells) * cellHeight;
    
    let cellPixelX = f32(relCol) * cellWidth + localUV.x * cellWidth;
    let cellPixelY = f32(relRow) * cellHeight + localUV.y * cellHeight;
    
    // Card-wide UV
    let cardU = cellPixelX / cardPixelWidth;
    let cardV = cellPixelY / cardPixelHeight;
    
    // Add quiet zone (1 module border)
    let qrSizeF = f32(qrSize);
    let totalSize = qrSizeF + 2.0;  // QR size + 2 modules quiet zone
    
    // Scale and center QR code (maintain aspect ratio)
    let aspect = cardPixelWidth / cardPixelHeight;
    var scale = 1.0;
    var offsetX = 0.0;
    var offsetY = 0.0;
    
    if (aspect > 1.0) {
        // Card is wider than tall
        scale = 1.0;
        offsetX = (1.0 - 1.0 / aspect) * 0.5;
    } else {
        // Card is taller than wide
        scale = 1.0;
        offsetY = (1.0 - aspect) * 0.5;
    }
    
    // Map to QR module coordinates
    var qrU = (cardU - offsetX) * max(aspect, 1.0);
    var qrV = (cardV - offsetY) * max(1.0 / aspect, 1.0);
    
    // Check if we're in quiet zone or outside
    if (qrU < 0.0 || qrU > 1.0 || qrV < 0.0 || qrV > 1.0) {
        return bgColor;
    }
    
    // Map to module index (including quiet zone)
    let moduleX = i32(qrU * totalSize) - 1;  // -1 to account for quiet zone
    let moduleY = i32(qrV * totalSize) - 1;
    
    // Quiet zone
    if (moduleX < 0 || moduleX >= i32(qrSize) || moduleY < 0 || moduleY >= i32(qrSize)) {
        return bgColor;
    }
    
    // Look up module in bit-packed buffer
    let moduleIdx = u32(moduleY) * qrSize + u32(moduleX);
    let wordIdx = moduleIdx / 32u;
    let bitIdx = moduleIdx % 32u;
    
    let bufferWordOffset = bufferOffset / 4u + wordIdx;
    let packedWord = bitcast<u32>(cardStorage[bufferWordOffset]);
    let isBlack = (packedWord >> bitIdx) & 1u;
    
    // Return color
    if (isBlack == 1u) {
        return fgColor;
    } else {
        return bgColor;
    }
}
