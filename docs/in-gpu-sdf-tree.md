# GPU-Accelerated SDF Rendering with BVH

This document describes how to implement efficient SDF (Signed Distance Function) rendering using a BVH (Bounding Volume Hierarchy) acceleration structure, built either on CPU or GPU compute shader.

## Overview

When rendering many SDF primitives, evaluating all SDFs for every pixel is expensive (O(pixels × primitives)). A BVH allows us to skip entire groups of primitives that don't affect the current pixel.

```
Without BVH: Each pixel evaluates ALL 1000 shapes = slow
With BVH:    Each pixel evaluates ~10-20 shapes = fast
```

## Input Format (HDraw YAML)

The input is a YAML-based scene description:

```yaml
- body:
    - sdCircle:
        position: [100, 100]
        radius: 50
        fill: "#e74c3c"

    - sdBox:
        position: [200, 150]
        size: [60, 40]
        fill: "#3498db"
        round: 8

    - sdBezier:
        p0: [50, 200]
        p1: [150, 100]
        p2: [250, 200]
        stroke: "#2ecc71"
        stroke-width: 3
```

## Step 1: Parse to Compact CPU Structure

Convert YAML to a flat array of primitives with computed AABBs:

```cpp
struct SDFPrimitive {
    uint32_t type;           // Circle, Box, Bezier, etc.
    float params[12];        // Type-specific parameters
    float fill[4];           // RGBA
    float stroke[4];         // RGBA
    float strokeWidth;
    float round;
    float rotate;
    float _pad;
    // Computed AABB
    float aabbMinX, aabbMinY;
    float aabbMaxX, aabbMaxY;
};

// Total: 80 bytes per primitive (aligned to 16)
```

### AABB Computation per Primitive Type

```cpp
void computeAABB(SDFPrimitive& prim) {
    switch (prim.type) {
        case Circle:
            // params: [cx, cy, radius]
            prim.aabbMinX = prim.params[0] - prim.params[2];
            prim.aabbMinY = prim.params[1] - prim.params[2];
            prim.aabbMaxX = prim.params[0] + prim.params[2];
            prim.aabbMaxY = prim.params[1] + prim.params[2];
            break;
        case Box:
            // params: [cx, cy, halfW, halfH] + rotation
            // For rotated box, expand AABB to encompass all corners
            float corners[4][2] = computeRotatedCorners(prim);
            prim.aabbMinX = min(corners);
            // ... etc
            break;
        case Bezier:
            // params: [p0x, p0y, p1x, p1y, p2x, p2y]
            // AABB is bounding box of control points (conservative)
            prim.aabbMinX = min(p0x, p1x, p2x);
            // ... etc
            break;
    }

    // Expand by stroke width
    float expand = prim.strokeWidth * 0.5f;
    prim.aabbMinX -= expand;
    prim.aabbMinY -= expand;
    prim.aabbMaxX += expand;
    prim.aabbMaxY += expand;
}
```

## Step 2: Build BVH

### Option A: CPU BVH Construction (Recommended for < 10k primitives)

```cpp
struct BVHNode {
    float aabbMinX, aabbMinY;
    float aabbMaxX, aabbMaxY;
    uint32_t leftChild;   // Index of left child (or primitive index if leaf)
    uint32_t rightChild;  // Index of right child (or 0xFFFFFFFF if leaf)
    uint32_t primIndex;   // Primitive index (only valid for leaves)
    uint32_t primCount;   // Number of primitives (0 for internal nodes)
};

// Morton code for spatial sorting
uint32_t morton2D(float x, float y, float sceneMinX, float sceneMinY,
                   float sceneMaxX, float sceneMaxY) {
    // Normalize to 0-1
    float nx = (x - sceneMinX) / (sceneMaxX - sceneMinX);
    float ny = (y - sceneMinY) / (sceneMaxY - sceneMinY);

    // Convert to integer grid (10 bits each = 1024 x 1024)
    uint32_t ix = (uint32_t)(nx * 1023.0f);
    uint32_t iy = (uint32_t)(ny * 1023.0f);

    // Interleave bits
    return interleaveBits(ix, iy);
}

std::vector<BVHNode> buildBVH(std::vector<SDFPrimitive>& prims) {
    // 1. Compute Morton code for each primitive's center
    std::vector<std::pair<uint32_t, uint32_t>> mortonPairs; // (morton, primIndex)
    for (uint32_t i = 0; i < prims.size(); i++) {
        float cx = (prims[i].aabbMinX + prims[i].aabbMaxX) * 0.5f;
        float cy = (prims[i].aabbMinY + prims[i].aabbMaxY) * 0.5f;
        mortonPairs.push_back({morton2D(cx, cy, ...), i});
    }

    // 2. Sort by Morton code
    std::sort(mortonPairs.begin(), mortonPairs.end());

    // 3. Build tree recursively
    std::vector<BVHNode> nodes;
    buildBVHRecursive(nodes, mortonPairs, 0, mortonPairs.size(), prims);

    return nodes;
}

uint32_t buildBVHRecursive(std::vector<BVHNode>& nodes,
                           std::vector<std::pair<uint32_t, uint32_t>>& sorted,
                           uint32_t start, uint32_t end,
                           std::vector<SDFPrimitive>& prims) {
    BVHNode node = {};
    uint32_t nodeIndex = nodes.size();
    nodes.push_back(node); // Reserve space

    uint32_t count = end - start;

    if (count <= 4) {
        // Leaf node - store primitives directly
        node.primIndex = start;
        node.primCount = count;
        node.leftChild = 0xFFFFFFFF;
        node.rightChild = 0xFFFFFFFF;

        // Compute AABB of all primitives in leaf
        for (uint32_t i = start; i < end; i++) {
            uint32_t pi = sorted[i].second;
            expandAABB(node, prims[pi]);
        }
    } else {
        // Internal node - split and recurse
        uint32_t mid = start + count / 2;

        node.leftChild = buildBVHRecursive(nodes, sorted, start, mid, prims);
        node.rightChild = buildBVHRecursive(nodes, sorted, mid, end, prims);
        node.primCount = 0;

        // AABB is union of children
        combineAABB(node, nodes[node.leftChild], nodes[node.rightChild]);
    }

    nodes[nodeIndex] = node;
    return nodeIndex;
}
```

### Option B: GPU Compute Shader BVH Construction (For > 10k primitives or frequent updates)

```wgsl
// Phase 1: Compute Morton codes
@compute @workgroup_size(64)
fn computeMortonCodes(@builtin(global_invocation_id) id: vec3<u32>) {
    if (id.x >= primCount) { return; }

    let prim = primitives[id.x];
    let center = vec2<f32>(
        (prim.aabbMinX + prim.aabbMaxX) * 0.5,
        (prim.aabbMinY + prim.aabbMaxY) * 0.5
    );

    // Normalize to scene bounds
    let norm = (center - sceneMin) / (sceneMax - sceneMin);

    // 10-bit precision per axis
    let ix = u32(clamp(norm.x, 0.0, 1.0) * 1023.0);
    let iy = u32(clamp(norm.y, 0.0, 1.0) * 1023.0);

    // Interleave bits for Morton code
    mortonCodes[id.x] = interleaveBits(ix, iy);
    primIndices[id.x] = id.x;
}

// Phase 2: Parallel radix sort (multiple passes)
@compute @workgroup_size(256)
fn radixSortPass(@builtin(global_invocation_id) id: vec3<u32>,
                 @builtin(local_invocation_id) lid: vec3<u32>) {
    // Standard GPU radix sort implementation
    // Sort mortonCodes and primIndices together
}

// Phase 3: Build tree structure
@compute @workgroup_size(64)
fn buildInternalNodes(@builtin(global_invocation_id) id: vec3<u32>) {
    if (id.x >= primCount - 1u) { return; }

    // Karras 2012 algorithm: find split based on highest differing bit
    let idx = id.x;

    // Determine range of keys covered by this internal node
    let d = sign(i32(commonPrefixLength(idx, idx + 1)) -
                 i32(commonPrefixLength(idx, idx - 1)));

    // Find the other end of the range
    let deltaMin = commonPrefixLength(idx, idx - d);
    var lMax = 2u;
    while (commonPrefixLength(idx, idx + lMax * d) > deltaMin) {
        lMax *= 2u;
    }

    // Binary search for exact end
    var l = 0u;
    var t = lMax / 2u;
    while (t >= 1u) {
        if (commonPrefixLength(idx, idx + (l + t) * d) > deltaMin) {
            l += t;
        }
        t /= 2u;
    }
    let j = idx + l * d;

    // Find split position
    let deltaNode = commonPrefixLength(idx, j);
    var s = 0u;
    t = (l + 1u) / 2u;
    while (t >= 1u) {
        if (commonPrefixLength(idx, idx + (s + t) * d) > deltaNode) {
            s += t;
        }
        t /= 2u;
    }
    let gamma = idx + s * d + min(d, 0);

    // Output node
    let nodeIdx = idx;
    if (min(idx, j) == gamma) {
        bvhNodes[nodeIdx].leftChild = gamma + primCount; // Leaf
    } else {
        bvhNodes[nodeIdx].leftChild = gamma; // Internal
    }
    if (max(idx, j) == gamma + 1u) {
        bvhNodes[nodeIdx].rightChild = gamma + 1u + primCount; // Leaf
    } else {
        bvhNodes[nodeIdx].rightChild = gamma + 1u; // Internal
    }
}

// Phase 4: Compute AABBs bottom-up
@compute @workgroup_size(64)
fn computeNodeAABBs(@builtin(global_invocation_id) id: vec3<u32>) {
    // Atomic counter approach: each leaf increments parent's counter
    // When counter reaches 2, parent computes its AABB from children
    // Propagates up the tree
}
```

## Step 3: GPU Buffer Layout

```cpp
// Upload to GPU
struct GPUBuffers {
    WGPUBuffer primitiveBuffer;  // Array of SDFPrimitive
    WGPUBuffer bvhNodeBuffer;    // Array of BVHNode
    WGPUBuffer sortedIndices;    // Primitive indices sorted by Morton code
};

// Bind group layout
@group(0) @binding(0) var<storage, read> primitives: array<SDFPrimitive>;
@group(0) @binding(1) var<storage, read> bvhNodes: array<BVHNode>;
@group(0) @binding(2) var<storage, read> sortedIndices: array<u32>;
@group(0) @binding(3) var<uniform> sceneUniforms: SceneUniforms;
```

## Step 4: Fragment Shader BVH Traversal

```wgsl
fn traverseBVH(pixelPos: vec2<f32>) -> vec4<f32> {
    var resultColor = vec4<f32>(0.0);
    var resultDist = 1e10;

    // Stack-based traversal (avoid recursion)
    var stack: array<u32, 32>;
    var stackPtr = 0;
    stack[stackPtr] = 0u; // Root node
    stackPtr++;

    while (stackPtr > 0) {
        stackPtr--;
        let nodeIdx = stack[stackPtr];
        let node = bvhNodes[nodeIdx];

        // AABB test
        if (pixelPos.x < node.aabbMinX || pixelPos.x > node.aabbMaxX ||
            pixelPos.y < node.aabbMinY || pixelPos.y > node.aabbMaxY) {
            continue; // Skip this branch
        }

        if (node.primCount > 0u) {
            // Leaf node - evaluate primitives
            for (var i = 0u; i < node.primCount; i++) {
                let primIdx = sortedIndices[node.primIndex + i];
                let prim = primitives[primIdx];

                let d = evaluateSDF(prim, pixelPos);
                if (d < resultDist) {
                    resultDist = d;
                    resultColor = primColorAt(prim, d);
                }
            }
        } else {
            // Internal node - push children
            stack[stackPtr] = node.leftChild;
            stackPtr++;
            stack[stackPtr] = node.rightChild;
            stackPtr++;
        }
    }

    return resultColor;
}

fn evaluateSDF(prim: SDFPrimitive, p: vec2<f32>) -> f32 {
    switch (prim.type) {
        case 0u: { // Circle
            let center = vec2<f32>(prim.params[0], prim.params[1]);
            let radius = prim.params[2];
            return length(p - center) - radius;
        }
        case 1u: { // Box
            let center = vec2<f32>(prim.params[0], prim.params[1]);
            let half = vec2<f32>(prim.params[2], prim.params[3]);
            let q = abs(p - center) - half + prim.round;
            return length(max(q, vec2<f32>(0.0))) + min(max(q.x, q.y), 0.0) - prim.round;
        }
        // ... other primitives
        default: { return 1e10; }
    }
}
```

## Step 5: Animation Support

For animated scenes where primitive positions change:

### Strategy A: Full Rebuild (Simple, good for < 1000 primitives)

```cpp
void updateAnimation(float time) {
    // Update primitive positions based on animation
    for (auto& prim : primitives) {
        applyAnimation(prim, time);
        computeAABB(prim);
    }

    // Rebuild BVH
    bvhNodes = buildBVH(primitives);

    // Upload to GPU
    wgpuQueueWriteBuffer(queue, primitiveBuffer, 0,
                         primitives.data(), primitives.size() * sizeof(SDFPrimitive));
    wgpuQueueWriteBuffer(queue, bvhNodeBuffer, 0,
                         bvhNodes.data(), bvhNodes.size() * sizeof(BVHNode));
}
```

### Strategy B: BVH Refit (Fast, preserves tree structure)

When only positions change (no primitives added/removed), refit AABBs without rebuilding:

```cpp
void refitBVH() {
    // Bottom-up: update leaf AABBs, then propagate up
    for (int i = bvhNodes.size() - 1; i >= 0; i--) {
        auto& node = bvhNodes[i];
        if (node.primCount > 0) {
            // Leaf: recompute from primitives
            resetAABB(node);
            for (uint32_t j = 0; j < node.primCount; j++) {
                expandAABB(node, primitives[sortedIndices[node.primIndex + j]]);
            }
        } else {
            // Internal: combine children
            combineAABB(node, bvhNodes[node.leftChild], bvhNodes[node.rightChild]);
        }
    }
}
```

### Strategy C: GPU Compute Refit (Fastest for large scenes)

```wgsl
// Only update AABBs, tree structure unchanged
@compute @workgroup_size(64)
fn refitLeaves(@builtin(global_invocation_id) id: vec3<u32>) {
    let leafIdx = id.x + internalNodeCount;
    if (leafIdx >= totalNodeCount) { return; }

    var node = bvhNodes[leafIdx];
    node.aabbMinX = 1e10; node.aabbMinY = 1e10;
    node.aabbMaxX = -1e10; node.aabbMaxY = -1e10;

    for (var i = 0u; i < node.primCount; i++) {
        let prim = primitives[sortedIndices[node.primIndex + i]];
        node.aabbMinX = min(node.aabbMinX, prim.aabbMinX);
        node.aabbMinY = min(node.aabbMinY, prim.aabbMinY);
        node.aabbMaxX = max(node.aabbMaxX, prim.aabbMaxX);
        node.aabbMaxY = max(node.aabbMaxY, prim.aabbMaxY);
    }

    bvhNodes[leafIdx] = node;

    // Signal parent to update (atomic)
    atomicAdd(&parentReadyCount[node.parent], 1);
}

@compute @workgroup_size(64)
fn refitInternals(@builtin(global_invocation_id) id: vec3<u32>) {
    // Process nodes whose children are ready
    // Propagate AABBs up the tree
}
```

## Performance Characteristics

| Primitives | Without BVH | With BVH | Speedup |
|------------|-------------|----------|---------|
| 100        | ~100 evals/pixel | ~8 evals/pixel | 12x |
| 1,000      | ~1000 evals/pixel | ~12 evals/pixel | 80x |
| 10,000     | ~10000 evals/pixel | ~16 evals/pixel | 600x |

BVH build time (CPU):
- 1,000 primitives: < 1ms
- 10,000 primitives: ~5ms
- 100,000 primitives: ~50ms

BVH refit time:
- 10x faster than full rebuild
- GPU compute: nearly instant for any size

## Summary

1. **Parse YAML** → flat array of `SDFPrimitive` with computed AABBs
2. **Build BVH** → Morton code sort + recursive split (CPU or GPU)
3. **Upload buffers** → primitives, BVH nodes, sorted indices
4. **Render** → fragment shader traverses BVH, evaluates only relevant SDFs
5. **Animate** → refit AABBs (fast) or rebuild tree (if topology changes)
