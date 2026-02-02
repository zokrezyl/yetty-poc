// JDraw Compute Shader: GPU-based tile culling
//
// For each screen-space tile, determines which SDF primitives overlap it
// by testing primitive AABBs against tile bounds. Writes a compact list
// of primitive indices per tile.
//
// Dispatch: (tileCountX, tileCountY, 1) workgroups
// Each workgroup handles one tile.
//
// Bindings:
//   binding 0: primData (read-only)  - SDF primitives uploaded by jdraw
//   binding 1: tileData (read-write) - tile list output
//   binding 2: uniforms

struct TileCullUniforms {
    primitiveCount: u32,
    tileCountX: u32,
    tileCountY: u32,
    maxPrimsPerTile: u32,
    sceneMinX: f32,
    sceneMinY: f32,
    sceneMaxX: f32,
    sceneMaxY: f32,
};

// Input: primitives (read-only, uploaded by jdraw to its own temp buffer)
@group(0) @binding(0) var<storage, read> primData: array<u32>;

// Output: tile lists (written by compute shader)
@group(0) @binding(1) var<storage, read_write> tileData: array<u32>;

// Uniforms
@group(0) @binding(2) var<uniform> uniforms: TileCullUniforms;

// Helper: read f32 from primData at a given index
fn readF32(index: u32) -> f32 {
    return bitcast<f32>(primData[index]);
}

// SDFPrimitive is 24 floats (96 bytes). AABB fields:
//   offset +18: aabbMinX (f32)
//   offset +19: aabbMinY (f32)
//   offset +20: aabbMaxX (f32)
//   offset +21: aabbMaxY (f32)
const PRIM_SIZE: u32 = 24u;
const AABB_MIN_X_OFFSET: u32 = 18u;
const AABB_MIN_Y_OFFSET: u32 = 19u;
const AABB_MAX_X_OFFSET: u32 = 20u;
const AABB_MAX_Y_OFFSET: u32 = 21u;

@compute @workgroup_size(1, 1, 1)
fn tileCull(@builtin(global_invocation_id) gid: vec3<u32>) {
    let tileX = gid.x;
    let tileY = gid.y;

    // Bounds check
    if (tileX >= uniforms.tileCountX || tileY >= uniforms.tileCountY) {
        return;
    }

    // Compute tile bounds in scene coordinates
    let sceneWidth = uniforms.sceneMaxX - uniforms.sceneMinX;
    let sceneHeight = uniforms.sceneMaxY - uniforms.sceneMinY;
    let invTileCountX = 1.0 / f32(uniforms.tileCountX);
    let invTileCountY = 1.0 / f32(uniforms.tileCountY);

    let tileMinX = uniforms.sceneMinX + f32(tileX) * invTileCountX * sceneWidth;
    let tileMaxX = uniforms.sceneMinX + f32(tileX + 1u) * invTileCountX * sceneWidth;
    let tileMinY = uniforms.sceneMinY + f32(tileY) * invTileCountY * sceneHeight;
    let tileMaxY = uniforms.sceneMinY + f32(tileY + 1u) * invTileCountY * sceneHeight;

    // Output offset for this tile (starts at 0 in tileData)
    let tileIndex = tileY * uniforms.tileCountX + tileX;
    let tileStride = 1u + uniforms.maxPrimsPerTile;
    let outOffset = tileIndex * tileStride;

    var count = 0u;

    // Test each primitive's AABB against tile bounds
    for (var pi = 0u; pi < uniforms.primitiveCount; pi++) {
        if (count >= uniforms.maxPrimsPerTile) {
            break;
        }

        // Prims start at offset 0 in primData
        let primBase = pi * PRIM_SIZE;

        let primMinX = readF32(primBase + AABB_MIN_X_OFFSET);
        let primMinY = readF32(primBase + AABB_MIN_Y_OFFSET);
        let primMaxX = readF32(primBase + AABB_MAX_X_OFFSET);
        let primMaxY = readF32(primBase + AABB_MAX_Y_OFFSET);

        // AABB overlap test
        if (primMaxX >= tileMinX && primMinX <= tileMaxX &&
            primMaxY >= tileMinY && primMinY <= tileMaxY) {
            tileData[outOffset + 1u + count] = pi;
            count++;
        }
    }

    // Write count as first element of tile
    tileData[outOffset] = count;
}
