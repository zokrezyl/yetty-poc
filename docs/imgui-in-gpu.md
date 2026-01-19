# GPU-Driven ImGui Rendering with BVH Acceleration

This document explores rendering ImGui entirely on the GPU by uploading draw data to storage buffers and using a BVH (Bounding Volume Hierarchy) for efficient fragment shader triangle lookup.

## Overview

Traditional ImGui rendering uses many draw calls per frame:
```
CPU: Build UI → Generate DrawLists → For each DrawCmd: SetScissor, BindTexture, DrawIndexed()
```

GPU-driven approach:
```
CPU: Build UI → Upload everything to buffers → Single draw call → Fragment shader does the rest
```

## Data Layout

### Storage Buffers

```wgsl
struct Triangle {
    v0: vec2<f32>,      // Vertex 0 position
    v1: vec2<f32>,      // Vertex 1 position
    v2: vec2<f32>,      // Vertex 2 position
    uv0: vec2<f32>,     // Vertex 0 UV
    uv1: vec2<f32>,     // Vertex 1 UV
    uv2: vec2<f32>,     // Vertex 2 UV
    color: u32,         // Packed RGBA (shared or per-vertex)
    clipRectIdx: u32,   // Index into clip rects array
};

struct BVHNode {
    aabbMin: vec2<f32>,
    aabbMax: vec2<f32>,
    leftChild: u32,     // Index or triangle start if leaf
    rightChild: u32,    // Count if leaf (high bit = leaf flag)
};

struct ClipRect {
    min: vec2<f32>,
    max: vec2<f32>,
};

@group(0) @binding(0) var<storage, read> triangles: array<Triangle>;
@group(0) @binding(1) var<storage, read> bvhNodes: array<BVHNode>;
@group(0) @binding(2) var<storage, read> clipRects: array<ClipRect>;
@group(0) @binding(3) var atlas: texture_2d<f32>;
@group(0) @binding(4) var atlasSampler: sampler;
```

### Typical Frame Size

| Component | Count | Size |
|-----------|-------|------|
| Triangles | 5,000 - 15,000 | 300KB - 900KB |
| BVH Nodes | ~2× triangles | 150KB - 450KB |
| Clip Rects | 50 - 200 | 1KB - 4KB |
| Texture Atlas | 1 | 4MB - 16MB |

---

## BVH Construction Options

### Option 1: CPU LBVH (Linear BVH)

Morton code-based construction. Simple, predictable, good parallelism.

```cpp
struct AABB {
    glm::vec2 min, max;
};

struct BVHBuildEntry {
    uint32_t triangleIdx;
    uint32_t mortonCode;
    AABB aabb;
    glm::vec2 centroid;
};

class CPUBVHBuilder {
public:
    void build(const std::vector<Triangle>& triangles) {
        // Step 1: Compute AABBs and centroids
        std::vector<BVHBuildEntry> entries(triangles.size());
        AABB sceneBounds = computeSceneBounds(triangles);

        for (size_t i = 0; i < triangles.size(); i++) {
            entries[i].triangleIdx = i;
            entries[i].aabb = computeTriangleAABB(triangles[i]);
            entries[i].centroid = (entries[i].aabb.min + entries[i].aabb.max) * 0.5f;
        }

        // Step 2: Compute Morton codes (spatial hash)
        for (auto& entry : entries) {
            glm::vec2 normalized = (entry.centroid - sceneBounds.min) /
                                   (sceneBounds.max - sceneBounds.min);
            entry.mortonCode = computeMortonCode(normalized);
        }

        // Step 3: Sort by Morton code (radix sort for speed)
        std::sort(entries.begin(), entries.end(),
            [](const auto& a, const auto& b) {
                return a.mortonCode < b.mortonCode;
            });

        // Step 4: Build tree bottom-up
        buildTreeRecursive(entries, 0, entries.size());
    }

private:
    uint32_t computeMortonCode(glm::vec2 normalized) {
        // Expand 10-bit values to interleaved 20-bit Morton code
        uint32_t x = expandBits(uint32_t(normalized.x * 1023.0f));
        uint32_t y = expandBits(uint32_t(normalized.y * 1023.0f));
        return (x << 1) | y;
    }

    uint32_t expandBits(uint32_t v) {
        v = (v | (v << 8)) & 0x00FF00FF;
        v = (v | (v << 4)) & 0x0F0F0F0F;
        v = (v | (v << 2)) & 0x33333333;
        v = (v | (v << 1)) & 0x55555555;
        return v;
    }

    uint32_t buildTreeRecursive(std::vector<BVHBuildEntry>& entries,
                                 size_t start, size_t end) {
        if (end - start <= MAX_LEAF_TRIANGLES) {
            // Create leaf node
            return createLeafNode(entries, start, end);
        }

        // Find split point using Morton code differences
        size_t split = findSplit(entries, start, end);

        uint32_t leftChild = buildTreeRecursive(entries, start, split);
        uint32_t rightChild = buildTreeRecursive(entries, split, end);

        return createInternalNode(leftChild, rightChild);
    }

    size_t findSplit(std::vector<BVHBuildEntry>& entries,
                     size_t start, size_t end) {
        uint32_t firstCode = entries[start].mortonCode;
        uint32_t lastCode = entries[end - 1].mortonCode;

        if (firstCode == lastCode) {
            return (start + end) / 2;
        }

        // Find highest differing bit
        uint32_t commonPrefix = __builtin_clz(firstCode ^ lastCode);

        // Binary search for split point
        size_t split = start;
        size_t step = end - start;

        do {
            step = (step + 1) / 2;
            size_t newSplit = split + step;

            if (newSplit < end) {
                uint32_t splitCode = entries[newSplit].mortonCode;
                uint32_t splitPrefix = __builtin_clz(firstCode ^ splitCode);
                if (splitPrefix > commonPrefix) {
                    split = newSplit;
                }
            }
        } while (step > 1);

        return split + 1;
    }
};
```

**Performance (15K triangles):**
- AABB computation: ~0.05ms
- Morton codes: ~0.05ms
- Radix sort: ~0.15ms
- Tree build: ~0.15ms
- **Total: ~0.4ms**

### Option 2: CPU SAH (Surface Area Heuristic)

Better tree quality, slower to build. Best for static/cached UI.

```cpp
class SAHBVHBuilder {
public:
    uint32_t build(std::vector<BVHBuildEntry>& entries,
                   size_t start, size_t end) {
        size_t count = end - start;

        if (count <= MAX_LEAF_TRIANGLES) {
            return createLeafNode(entries, start, end);
        }

        // Compute bounds
        AABB centroidBounds = computeCentroidBounds(entries, start, end);
        int splitAxis = centroidBounds.longestAxis();

        // Try multiple split positions, pick best SAH cost
        float bestCost = FLT_MAX;
        size_t bestSplit = (start + end) / 2;

        constexpr int NUM_BUCKETS = 12;
        Bucket buckets[NUM_BUCKETS];

        // Assign triangles to buckets
        for (size_t i = start; i < end; i++) {
            float centroid = entries[i].centroid[splitAxis];
            float normalized = (centroid - centroidBounds.min[splitAxis]) /
                              (centroidBounds.max[splitAxis] - centroidBounds.min[splitAxis]);
            int bucket = std::min(int(normalized * NUM_BUCKETS), NUM_BUCKETS - 1);
            buckets[bucket].count++;
            buckets[bucket].bounds.expand(entries[i].aabb);
        }

        // Evaluate SAH cost for each split
        for (int i = 0; i < NUM_BUCKETS - 1; i++) {
            AABB leftBounds, rightBounds;
            int leftCount = 0, rightCount = 0;

            for (int j = 0; j <= i; j++) {
                leftBounds.expand(buckets[j].bounds);
                leftCount += buckets[j].count;
            }
            for (int j = i + 1; j < NUM_BUCKETS; j++) {
                rightBounds.expand(buckets[j].bounds);
                rightCount += buckets[j].count;
            }

            float cost = 0.125f + // Traversal cost
                        (leftCount * leftBounds.surfaceArea() +
                         rightCount * rightBounds.surfaceArea()) /
                        centroidBounds.surfaceArea();

            if (cost < bestCost) {
                bestCost = cost;
                bestSplit = /* compute actual split index */;
            }
        }

        // Partition and recurse
        auto mid = std::partition(entries.begin() + start, entries.begin() + end,
            [&](const auto& e) {
                float centroid = e.centroid[splitAxis];
                float normalized = (centroid - centroidBounds.min[splitAxis]) /
                                  (centroidBounds.max[splitAxis] - centroidBounds.min[splitAxis]);
                int bucket = std::min(int(normalized * NUM_BUCKETS), NUM_BUCKETS - 1);
                return bucket <= bestSplit;
            });

        size_t splitIdx = mid - entries.begin();

        uint32_t left = build(entries, start, splitIdx);
        uint32_t right = build(entries, splitIdx, end);

        return createInternalNode(left, right);
    }
};
```

**Performance (15K triangles):**
- **Total: ~2-5ms** (depends on bucket count)

Better for: static UI panels that don't change often.

### Option 3: GPU Compute LBVH

Fully parallel construction on GPU. Best for dynamic UI.

```wgsl
// Pass 1: Compute AABBs and Morton codes
@compute @workgroup_size(256)
fn computeMortonCodes(@builtin(global_invocation_id) gid: vec3<u32>) {
    let idx = gid.x;
    if (idx >= triangleCount) { return; }

    let tri = triangles[idx];

    // Compute AABB
    let minP = min(min(tri.v0, tri.v1), tri.v2);
    let maxP = max(max(tri.v0, tri.v1), tri.v2);
    let centroid = (minP + maxP) * 0.5;

    // Normalize to scene bounds
    let normalized = (centroid - sceneBounds.min) / (sceneBounds.max - sceneBounds.min);

    // Morton code
    let mortonCode = computeMorton2D(normalized);

    mortonCodes[idx] = mortonCode;
    triangleAABBs[idx] = vec4<f32>(minP, maxP);
    sortIndices[idx] = idx;
}

fn computeMorton2D(p: vec2<f32>) -> u32 {
    let x = u32(clamp(p.x, 0.0, 1.0) * 1023.0);
    let y = u32(clamp(p.y, 0.0, 1.0) * 1023.0);
    return expandBits(x) << 1u | expandBits(y);
}

fn expandBits(v: u32) -> u32 {
    var x = v;
    x = (x | (x << 8u)) & 0x00FF00FFu;
    x = (x | (x << 4u)) & 0x0F0F0F0Fu;
    x = (x | (x << 2u)) & 0x33333333u;
    x = (x | (x << 1u)) & 0x55555555u;
    return x;
}
```

```wgsl
// Pass 2: Parallel radix sort (simplified - use proper GPU sort library)
// WebGPU doesn't have native sort, implement bitonic sort or use library

@compute @workgroup_size(256)
fn bitonicSortStep(@builtin(global_invocation_id) gid: vec3<u32>,
                   @builtin(local_invocation_id) lid: vec3<u32>) {
    // Bitonic sort implementation
    // Compare-and-swap pairs based on current step
    let idx = gid.x;
    let pairIdx = idx ^ stepMask;

    if (pairIdx > idx) {
        let shouldSwap = (mortonCodes[sortIndices[idx]] > mortonCodes[sortIndices[pairIdx]])
                         == ascending;
        if (shouldSwap) {
            let temp = sortIndices[idx];
            sortIndices[idx] = sortIndices[pairIdx];
            sortIndices[pairIdx] = temp;
        }
    }
}
```

```wgsl
// Pass 3: Build tree structure (Karras 2012 algorithm)
@compute @workgroup_size(256)
fn buildBVHNodes(@builtin(global_invocation_id) gid: vec3<u32>) {
    let idx = gid.x;
    if (idx >= triangleCount - 1u) { return; }

    // Determine direction of the range
    let d = sign(i32(delta(idx, idx + 1u)) - i32(delta(idx, idx - 1u)));

    // Compute upper bound for the length of the range
    let deltaMin = delta(idx, idx - u32(d));
    var lMax = 2u;
    while (delta(idx, idx + lMax * u32(d)) > deltaMin) {
        lMax *= 2u;
    }

    // Binary search for the other end
    var l = 0u;
    var t = lMax / 2u;
    while (t >= 1u) {
        if (delta(idx, idx + (l + t) * u32(d)) > deltaMin) {
            l += t;
        }
        t /= 2u;
    }
    let j = idx + l * u32(d);

    // Find split position
    let deltaNode = delta(idx, j);
    var s = 0u;
    t = (l + 1u) / 2u;
    while (t >= 1u) {
        if (delta(idx, idx + (s + t) * u32(d)) > deltaNode) {
            s += t;
        }
        t /= 2u;
    }
    let gamma = idx + s * u32(d) + min(u32(d), 0u);

    // Output child pointers
    let leftIdx = select(gamma, gamma + triangleCount - 1u, min(idx, j) == gamma);
    let rightIdx = select(gamma + 1u, gamma + triangleCount, max(idx, j) == gamma + 1u);

    bvhNodes[idx + triangleCount - 1u].leftChild = leftIdx;
    bvhNodes[idx + triangleCount - 1u].rightChild = rightIdx;
}

fn delta(i: u32, j: u32) -> u32 {
    if (j >= triangleCount) { return 0u; }
    let codeI = mortonCodes[sortIndices[i]];
    let codeJ = mortonCodes[sortIndices[j]];
    if (codeI == codeJ) {
        return 32u + countLeadingZeros(i ^ j);
    }
    return countLeadingZeros(codeI ^ codeJ);
}
```

```wgsl
// Pass 4: Compute internal node AABBs (bottom-up)
@compute @workgroup_size(256)
fn computeInternalAABBs(@builtin(global_invocation_id) gid: vec3<u32>) {
    let idx = gid.x;
    if (idx >= triangleCount) { return; }

    // Start from leaf, walk up to root
    var nodeIdx = idx;  // Leaf index

    // Atomic counter to ensure both children processed before parent
    loop {
        let parentIdx = bvhParent[nodeIdx];
        if (parentIdx == 0xFFFFFFFFu) { break; }  // Root

        // Atomic increment - second thread to arrive computes AABB
        let count = atomicAdd(&nodeCounter[parentIdx], 1u);
        if (count == 0u) {
            // First thread - exit, let second thread handle it
            break;
        }

        // Second thread - compute parent AABB from children
        let node = bvhNodes[parentIdx];
        let leftAABB = getNodeAABB(node.leftChild);
        let rightAABB = getNodeAABB(node.rightChild);

        bvhNodes[parentIdx].aabbMin = min(leftAABB.min, rightAABB.min);
        bvhNodes[parentIdx].aabbMax = max(leftAABB.max, rightAABB.max);

        nodeIdx = parentIdx;
    }
}
```

**GPU Performance (15K triangles):**
- Pass 1 (Morton codes): ~0.02ms
- Pass 2 (Sort): ~0.1ms
- Pass 3 (Build tree): ~0.02ms
- Pass 4 (AABBs): ~0.03ms
- **Total: ~0.17ms** (GPU time, CPU nearly free)

---

## Hybrid Approaches

### Option 4: Two-Level BVH

Use ImGui's clip rects as top-level, BVH only within each region.

```cpp
struct TopLevelEntry {
    AABB clipRect;              // From ImDrawCmd
    uint32_t bottomLevelBVH;    // Index into BVH array
    uint32_t triangleStart;
    uint32_t triangleCount;
};

void buildTwoLevelBVH(ImDrawData* drawData) {
    std::vector<TopLevelEntry> topLevel;

    for (int n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList* cmdList = drawData->CmdLists[n];

        for (int i = 0; i < cmdList->CmdBuffer.Size; i++) {
            const ImDrawCmd& cmd = cmdList->CmdBuffer[i];

            TopLevelEntry entry;
            entry.clipRect = {
                {cmd.ClipRect.x, cmd.ClipRect.y},
                {cmd.ClipRect.z, cmd.ClipRect.w}
            };
            entry.triangleStart = cmd.IdxOffset / 3;
            entry.triangleCount = cmd.ElemCount / 3;

            // Build bottom-level BVH for this command's triangles
            entry.bottomLevelBVH = buildBottomLevelBVH(
                cmdList, cmd.VtxOffset, cmd.IdxOffset, cmd.ElemCount);

            topLevel.push_back(entry);
        }
    }

    uploadTopLevel(topLevel);
}
```

Fragment shader:
```wgsl
fn traverseTwoLevel(pixel: vec2<f32>) -> vec4<f32> {
    var result = vec4<f32>(0.0);

    // Test top-level (clip rects) - typically 50-200 entries
    for (var i = 0u; i < topLevelCount; i++) {
        let entry = topLevel[i];

        // Quick clip rect rejection
        if (pixel.x < entry.clipRect.min.x || pixel.x > entry.clipRect.max.x ||
            pixel.y < entry.clipRect.min.y || pixel.y > entry.clipRect.max.y) {
            continue;
        }

        // Traverse bottom-level BVH
        result = blendOver(result, traverseBVH(entry.bottomLevelBVH, pixel));
    }

    return result;
}
```

**Advantages:**
- Top-level is essentially free (already computed by ImGui)
- Bottom-level BVHs can be cached per-window
- Rebuild only changed windows

### Option 5: Incremental Updates

Cache BVH, detect changes, update incrementally.

```cpp
class IncrementalBVH {
    struct CachedWindow {
        uint64_t contentHash;
        uint32_t bvhNodeStart;
        uint32_t triangleStart;
        uint32_t triangleCount;
    };

    std::unordered_map<ImGuiID, CachedWindow> windowCache;

    void update(ImDrawData* drawData) {
        std::vector<ImGuiID> activeWindows;

        for (int n = 0; n < drawData->CmdListsCount; n++) {
            const ImDrawList* cmdList = drawData->CmdLists[n];
            ImGuiID windowId = getWindowId(cmdList);
            activeWindows.push_back(windowId);

            uint64_t hash = computeContentHash(cmdList);

            auto it = windowCache.find(windowId);
            if (it != windowCache.end() && it->second.contentHash == hash) {
                // No change - reuse cached BVH
                continue;
            }

            // Rebuild BVH for this window
            rebuildWindowBVH(windowId, cmdList);
        }

        // Remove stale windows from cache
        pruneCache(activeWindows);
    }
};
```

---

## Fragment Shader Traversal

```wgsl
fn traverseBVH(pixel: vec2<f32>) -> vec4<f32> {
    var result = vec4<f32>(0.0);

    // Stack-based traversal
    var stack: array<u32, 32>;
    var stackPtr = 0;
    stack[stackPtr] = 0u;  // Root node
    stackPtr++;

    while (stackPtr > 0) {
        stackPtr--;
        let nodeIdx = stack[stackPtr];
        let node = bvhNodes[nodeIdx];

        // AABB test
        if (pixel.x < node.aabbMin.x || pixel.x > node.aabbMax.x ||
            pixel.y < node.aabbMin.y || pixel.y > node.aabbMax.y) {
            continue;
        }

        // Check if leaf (high bit set in rightChild)
        if ((node.rightChild & 0x80000000u) != 0u) {
            // Leaf node - test triangles
            let triStart = node.leftChild;
            let triCount = node.rightChild & 0x7FFFFFFFu;

            for (var i = 0u; i < triCount; i++) {
                let tri = triangles[triStart + i];

                // Clip rect test
                let clip = clipRects[tri.clipRectIdx];
                if (pixel.x < clip.min.x || pixel.x > clip.max.x ||
                    pixel.y < clip.min.y || pixel.y > clip.max.y) {
                    continue;
                }

                // Point-in-triangle test
                let bary = barycentric(pixel, tri.v0, tri.v1, tri.v2);
                if (bary.x >= 0.0 && bary.y >= 0.0 && bary.z >= 0.0) {
                    // Interpolate UV and sample texture
                    let uv = tri.uv0 * bary.x + tri.uv1 * bary.y + tri.uv2 * bary.z;
                    let texColor = textureSample(atlas, atlasSampler, uv);
                    let vertColor = unpackColor(tri.color);

                    // Blend (back to front)
                    result = blendOver(result, texColor * vertColor);
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

    return result;
}

fn barycentric(p: vec2<f32>, a: vec2<f32>, b: vec2<f32>, c: vec2<f32>) -> vec3<f32> {
    let v0 = c - a;
    let v1 = b - a;
    let v2 = p - a;

    let dot00 = dot(v0, v0);
    let dot01 = dot(v0, v1);
    let dot02 = dot(v0, v2);
    let dot11 = dot(v1, v1);
    let dot12 = dot(v1, v2);

    let invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    let u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    let v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return vec3<f32>(1.0 - u - v, v, u);
}

fn blendOver(dst: vec4<f32>, src: vec4<f32>) -> vec4<f32> {
    let outAlpha = src.a + dst.a * (1.0 - src.a);
    if (outAlpha < 0.001) { return vec4<f32>(0.0); }
    let outRgb = (src.rgb * src.a + dst.rgb * dst.a * (1.0 - src.a)) / outAlpha;
    return vec4<f32>(outRgb, outAlpha);
}
```

---

## Performance Comparison

| Method | Build Time | Tree Quality | Best For |
|--------|------------|--------------|----------|
| CPU LBVH | ~0.4ms | Good | Dynamic UI, simple |
| CPU SAH | ~3ms | Excellent | Static/cached UI |
| GPU LBVH | ~0.17ms GPU | Good | High triangle count |
| Two-Level | ~0.1ms | Good | Mixed static/dynamic |
| Incremental | ~0.05ms avg | Good | Mostly static UI |

## Recommendations

1. **Simple implementation**: Start with CPU LBVH. Easy to debug, good performance.

2. **Production quality**: Two-level BVH with incremental updates. Best balance of performance and code complexity.

3. **Maximum performance**: GPU compute LBVH. Worth it if you have >50K triangles or need sub-millisecond updates.

4. **For terminal UI**: CPU LBVH or two-level is likely sufficient. Terminal UI is typically simple enough that even brute-force might work.
