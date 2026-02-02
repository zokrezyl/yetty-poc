// Shader glyph: HDraw widget (codepoint 1048578 / U+100002)
// GPU-accelerated SDF primitives with BVH acceleration
//
// Uses shared SDF functions from lib/distfunctions.wgsl
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  primitiveOffset (u32)
//   offset 1:  primitiveCount (u32)
//   offset 2:  textSpanOffset (u32)
//   offset 3:  textSpanCount (u32)
//   offset 4:  bvhOffset (u32)
//   offset 5:  bvhNodeCount (u32)
//   offset 6:  glyphOffset (u32)
//   offset 7:  glyphCount (u32)
//   offset 8:  sceneMinX (f32)
//   offset 9:  sceneMinY (f32)
//   offset 10: sceneMaxX (f32)
//   offset 11: sceneMaxY (f32)
//   offset 12: widthCells (u32)
//   offset 13: heightCells (u32)
//   offset 14: flags (u32)
//   offset 15: bgColor (u32)
// =============================================================================

const HDRAW_FLAG_SHOW_BOUNDS: u32 = 1u;
const HDRAW_FLAG_SHOW_BVH: u32 = 2u;
const HDRAW_FLAG_SHOW_EVAL_COUNT: u32 = 4u;

// =============================================================================
// BVH Traversal helpers
// =============================================================================

fn getBVHNodeChildren(nodeOffset: u32) -> vec4<u32> {
    return vec4<u32>(
        bitcast<u32>(cardStorage[nodeOffset + 4u]),
        bitcast<u32>(cardStorage[nodeOffset + 5u]),
        bitcast<u32>(cardStorage[nodeOffset + 6u]),
        bitcast<u32>(cardStorage[nodeOffset + 7u])
    );
}

// =============================================================================
// Main shader function
// =============================================================================

fn shaderGlyph_1048578(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // Decode ANSI-compatible cell encoding
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // Read metadata
    let primitiveOffset = cardMetadata[metaOffset + 0u];
    let primitiveCount = cardMetadata[metaOffset + 1u];
    let bvhOffset = cardMetadata[metaOffset + 4u];
    let bvhNodeCount = cardMetadata[metaOffset + 5u];
    let sceneMinX = bitcast<f32>(cardMetadata[metaOffset + 8u]);
    let sceneMinY = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let sceneMaxX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let sceneMaxY = bitcast<f32>(cardMetadata[metaOffset + 11u]);
    let widthCells = cardMetadata[metaOffset + 12u];
    let heightCells = cardMetadata[metaOffset + 13u];
    let flags = cardMetadata[metaOffset + 14u];
    let bgColorPacked = cardMetadata[metaOffset + 15u];

    let bgColor = unpackColor(bgColorPacked);

    // Compute scene-space position
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    let scenePos = vec2<f32>(
        sceneMinX + widgetUV.x * (sceneMaxX - sceneMinX),
        sceneMinY + widgetUV.y * (sceneMaxY - sceneMinY)
    );

    if (primitiveCount == 0u) {
        return vec3<f32>(1.0, 0.0, 1.0);  // Magenta = no primitives
    }

    // Render primitives using BVH traversal
    var resultColor = bgColor;

    let primSize = 24u;
    let bvhNodeSize = 8u;
    let sortedIndicesOffset = bvhOffset + bvhNodeCount * bvhNodeSize;

    // Stack-based BVH traversal
    var stack: array<u32, 32>;
    var stackPtr = 0;
    var evalCount = 0u;

    if (bvhNodeCount > 0u) {
        stack[stackPtr] = 0u;
        stackPtr++;
    }

    var bestLayer = 0xFFFFFFFFu;

    while (stackPtr > 0) {
        stackPtr--;
        let nodeIdx = stack[stackPtr];
        let nodeOffset = bvhOffset + nodeIdx * bvhNodeSize;

        // Read BVH node AABB
        let nodeMinX = cardStorage[nodeOffset + 0u];
        let nodeMinY = cardStorage[nodeOffset + 1u];
        let nodeMaxX = cardStorage[nodeOffset + 2u];
        let nodeMaxY = cardStorage[nodeOffset + 3u];

        // AABB test - skip entire subtree if pixel is outside
        if (scenePos.x < nodeMinX || scenePos.x > nodeMaxX ||
            scenePos.y < nodeMinY || scenePos.y > nodeMaxY) {
            continue;
        }

        // Read node children/primitive info
        let leftChild = bitcast<u32>(cardStorage[nodeOffset + 4u]);
        let rightChild = bitcast<u32>(cardStorage[nodeOffset + 5u]);
        let primIndex = bitcast<u32>(cardStorage[nodeOffset + 6u]);
        let primCount = bitcast<u32>(cardStorage[nodeOffset + 7u]);

        if (primCount > 0u) {
            // Leaf node - evaluate primitives
            for (var i = 0u; i < primCount; i++) {
                let sortedIdx = bitcast<u32>(cardStorage[sortedIndicesOffset + primIndex + i]);
                let primOffset = primitiveOffset + sortedIdx * primSize;

                // Per-primitive AABB check
                let primMinX = cardStorage[primOffset + 18u];
                let primMinY = cardStorage[primOffset + 19u];
                let primMaxX = cardStorage[primOffset + 20u];
                let primMaxY = cardStorage[primOffset + 21u];
                if (scenePos.x < primMinX || scenePos.x > primMaxX ||
                    scenePos.y < primMinY || scenePos.y > primMaxY) {
                    continue;
                }

                evalCount++;

                let d = evaluateSDF(primOffset, scenePos);
                let colors = getPrimitiveColors(primOffset);
                let fillColor = unpackColor(colors.x);
                let strokeColor = unpackColor(colors.y);
                let layer = colors.z;
                let strokeWidth = getPrimitiveStrokeWidth(primOffset);

                // Fill
                if (d < 0.0 && colors.x != 0u) {
                    let alpha = clamp(-d * 2.0, 0.0, 1.0);
                    if (layer <= bestLayer) {
                        resultColor = mix(resultColor, fillColor, alpha);
                        bestLayer = layer;
                    }
                }

                // Stroke
                if (strokeWidth > 0.0 && colors.y != 0u) {
                    let strokeDist = abs(d) - strokeWidth * 0.5;
                    if (strokeDist < 0.0) {
                        let alpha = clamp(-strokeDist * 2.0, 0.0, 1.0);
                        resultColor = mix(resultColor, strokeColor, alpha);
                    }
                }
            }
        } else {
            // Internal node - push children onto stack
            if (rightChild != 0xFFFFFFFFu && stackPtr < 31) {
                stack[stackPtr] = rightChild;
                stackPtr++;
            }
            if (leftChild != 0xFFFFFFFFu && stackPtr < 31) {
                stack[stackPtr] = leftChild;
                stackPtr++;
            }
        }
    }

    // Debug: show evaluation count heatmap
    if ((flags & HDRAW_FLAG_SHOW_EVAL_COUNT) != 0u) {
        let t = clamp(f32(evalCount) / 20.0, 0.0, 1.0);
        return vec3<f32>(t, 1.0 - t * 0.5, 0.0);
    }

    // Debug: show bounds
    if ((flags & HDRAW_FLAG_SHOW_BOUNDS) != 0u) {
        let edgeDist = min(
            min(scenePos.x - sceneMinX, sceneMaxX - scenePos.x),
            min(scenePos.y - sceneMinY, sceneMaxY - scenePos.y)
        );
        let edgeWidth = (sceneMaxX - sceneMinX) * 0.01;
        if (edgeDist < edgeWidth) {
            resultColor = mix(resultColor, vec3<f32>(1.0, 0.0, 0.0), 0.5);
        }
    }

    return resultColor;
}
