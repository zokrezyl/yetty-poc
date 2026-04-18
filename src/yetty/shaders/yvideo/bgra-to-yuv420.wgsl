// BGRA to YUV420 conversion compute shader
// Converts BGRA framebuffer to YUV420 planar format for H.264 encoding
//
// =============================================================================
// OUTPUT FORMAT: YUV420 planar (I420)
// =============================================================================
//
// Y plane:  Full resolution (width x height), 1 byte per pixel
// U plane:  Half resolution (width/2 x height/2), 1 byte per pixel
// V plane:  Half resolution (width/2 x height/2), 1 byte per pixel
//
// Memory layout in output buffer:
//   [Y plane: yStride * height bytes]
//   [U plane: uvStride * (height/2) bytes]
//   [V plane: uvStride * (height/2) bytes]
//
// =============================================================================
// COLOR MATRIX: BT.709 (HD content)
// =============================================================================
//
// For video range (16-235 for Y, 16-240 for UV):
//   Y  = 16  + 219 * (0.2126 * R + 0.7152 * G + 0.0722 * B)
//   Cb = 128 + 224 * (-0.1146 * R - 0.3854 * G + 0.5 * B)
//   Cr = 128 + 224 * (0.5 * R - 0.4542 * G - 0.0458 * B)
//
// =============================================================================
// DISPATCH STRATEGY (no atomics needed):
// =============================================================================
//
// computeY:  Each thread processes 4 horizontal Y pixels -> writes 1 u32
//            Dispatch: ceil(width/4) x height workgroups of 16x16
//
// computeUV: Each thread processes 4 horizontal UV pairs -> writes 1 u32 U + 1 u32 V
//            Dispatch: ceil(uvWidth/4) x uvHeight workgroups of 16x16
//
// =============================================================================

struct Params {
    width: u32,
    height: u32,
    yStride: u32,      // Stride for Y plane (must be multiple of 4)
    uvStride: u32,     // Stride for U/V planes (must be multiple of 4)
}

@group(0) @binding(0) var inputTexture: texture_2d<f32>;
@group(0) @binding(1) var<storage, read_write> outputBuffer: array<u32>;
@group(0) @binding(2) var<uniform> params: Params;

// BT.709 RGB to YUV conversion (video range 16-235/16-240)
fn rgbToY(r: f32, g: f32, b: f32) -> u32 {
    let y = 16.0 + 219.0 * (0.2126 * r + 0.7152 * g + 0.0722 * b);
    return u32(clamp(y, 16.0, 235.0));
}

fn rgbToCb(r: f32, g: f32, b: f32) -> u32 {
    let cb = 128.0 + 224.0 * (-0.1146 * r - 0.3854 * g + 0.5 * b);
    return u32(clamp(cb, 16.0, 240.0));
}

fn rgbToCr(r: f32, g: f32, b: f32) -> u32 {
    let cr = 128.0 + 224.0 * (0.5 * r - 0.4542 * g - 0.0458 * b);
    return u32(clamp(cr, 16.0, 240.0));
}

// Helper: sample pixel, handling BGRA->RGB swap
fn sampleRGB(px: u32, py: u32) -> vec3<f32> {
    let bgra = textureLoad(inputTexture, vec2<u32>(px, py), 0);
    // WebGPU texture format BGRA8Unorm: .r=B, .g=G, .b=R, .a=A
    return vec3<f32>(bgra.b, bgra.g, bgra.r);
}

// =============================================================================
// Pass 1: Convert BGRA to Y plane (full resolution)
// Each thread processes 4 horizontal pixels, writes 1 aligned u32
// =============================================================================
@compute @workgroup_size(16, 16, 1)
fn computeY(@builtin(global_invocation_id) gid: vec3<u32>) {
    let x4 = gid.x;  // x in units of 4 pixels
    let y = gid.y;

    let px = x4 * 4u;
    if (px >= params.width || y >= params.height) {
        return;
    }

    var yPacked: u32 = 0u;

    for (var i: u32 = 0u; i < 4u; i++) {
        let pixelX = px + i;
        if (pixelX < params.width) {
            let rgb = sampleRGB(pixelX, y);
            let yVal = rgbToY(rgb.r, rgb.g, rgb.b);
            yPacked |= (yVal & 0xFFu) << (i * 8u);
        }
    }

    // Y plane starts at offset 0, stride is in bytes
    // Buffer index = byte offset / 4
    let byteOffset = y * params.yStride + px;
    outputBuffer[byteOffset / 4u] = yPacked;
}

// =============================================================================
// Pass 2: Convert BGRA to U and V planes (half resolution)
// Each thread processes 4 horizontal UV samples, writes 1 u32 to U + 1 u32 to V
// Each UV sample is averaged from a 2x2 block of source pixels
// =============================================================================
@compute @workgroup_size(16, 16, 1)
fn computeUV(@builtin(global_invocation_id) gid: vec3<u32>) {
    let uvX4 = gid.x;  // x in units of 4 UV pixels
    let uvY = gid.y;

    let uvWidth = params.width / 2u;
    let uvHeight = params.height / 2u;

    let uvPx = uvX4 * 4u;
    if (uvPx >= uvWidth || uvY >= uvHeight) {
        return;
    }

    var uPacked: u32 = 0u;
    var vPacked: u32 = 0u;

    for (var i: u32 = 0u; i < 4u; i++) {
        let uvX = uvPx + i;
        if (uvX >= uvWidth) {
            break;
        }

        // Source 2x2 block top-left corner
        let srcX = uvX * 2u;
        let srcY = uvY * 2u;

        // Average 2x2 block
        var sumR: f32 = 0.0;
        var sumG: f32 = 0.0;
        var sumB: f32 = 0.0;
        var count: f32 = 0.0;

        for (var dy: u32 = 0u; dy < 2u; dy++) {
            for (var dx: u32 = 0u; dx < 2u; dx++) {
                let px = srcX + dx;
                let py = srcY + dy;
                if (px < params.width && py < params.height) {
                    let rgb = sampleRGB(px, py);
                    sumR += rgb.r;
                    sumG += rgb.g;
                    sumB += rgb.b;
                    count += 1.0;
                }
            }
        }

        if (count > 0.0) {
            let avgR = sumR / count;
            let avgG = sumG / count;
            let avgB = sumB / count;

            let cb = rgbToCb(avgR, avgG, avgB);
            let cr = rgbToCr(avgR, avgG, avgB);

            uPacked |= (cb & 0xFFu) << (i * 8u);
            vPacked |= (cr & 0xFFu) << (i * 8u);
        } else {
            // Fill with neutral chroma (128)
            uPacked |= 128u << (i * 8u);
            vPacked |= 128u << (i * 8u);
        }
    }

    // Calculate plane offsets
    let yPlaneSize = params.yStride * params.height;
    let uvPlaneSize = params.uvStride * uvHeight;

    // U plane byte offset
    let uByteOffset = yPlaneSize + uvY * params.uvStride + uvPx;
    outputBuffer[uByteOffset / 4u] = uPacked;

    // V plane byte offset
    let vByteOffset = yPlaneSize + uvPlaneSize + uvY * params.uvStride + uvPx;
    outputBuffer[vByteOffset / 4u] = vPacked;
}
