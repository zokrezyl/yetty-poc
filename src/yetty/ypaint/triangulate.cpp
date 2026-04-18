#include "triangulate.h"
#include <cmath>
#include <algorithm>

namespace yetty::ypaint {

namespace {

// Cross product of vectors (p1-p0) and (p2-p0)
inline float cross2d(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y) {
    return (p1x - p0x) * (p2y - p0y) - (p1y - p0y) * (p2x - p0x);
}

// Check if polygon winding is counter-clockwise (positive area)
bool isCounterClockwise(const float* verts, const std::vector<uint32_t>& indices) {
    if (indices.size() < 3) return false;
    float area = 0.0f;
    for (size_t i = 0; i < indices.size(); ++i) {
        size_t j = (i + 1) % indices.size();
        uint32_t vi = indices[i];
        uint32_t vj = indices[j];
        area += verts[vi * 2] * verts[vj * 2 + 1];
        area -= verts[vj * 2] * verts[vi * 2 + 1];
    }
    return area > 0.0f;
}

// Check if point p is inside triangle abc
bool pointInTriangle(float px, float py,
                     float ax, float ay, float bx, float by, float cx, float cy) {
    float d1 = cross2d(px, py, ax, ay, bx, by);
    float d2 = cross2d(px, py, bx, by, cx, cy);
    float d3 = cross2d(px, py, cx, cy, ax, ay);
    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(hasNeg && hasPos);
}

// Check if vertex at index i is a convex vertex (forms a left turn)
bool isConvex(const float* verts, const std::vector<uint32_t>& indices, size_t i, bool ccw) {
    size_t n = indices.size();
    size_t prev = (i + n - 1) % n;
    size_t next = (i + 1) % n;
    uint32_t vi = indices[i];
    uint32_t vp = indices[prev];
    uint32_t vn = indices[next];
    float cross = cross2d(verts[vp * 2], verts[vp * 2 + 1],
                          verts[vi * 2], verts[vi * 2 + 1],
                          verts[vn * 2], verts[vn * 2 + 1]);
    return ccw ? (cross > 0.0f) : (cross < 0.0f);
}

// Check if ear triangle at index i contains any other polygon vertices
bool isEar(const float* verts, const std::vector<uint32_t>& indices, size_t i, bool ccw) {
    size_t n = indices.size();
    if (n < 3) return false;
    
    size_t prev = (i + n - 1) % n;
    size_t next = (i + 1) % n;
    
    uint32_t vi = indices[i];
    uint32_t vp = indices[prev];
    uint32_t vn = indices[next];
    
    float ax = verts[vp * 2], ay = verts[vp * 2 + 1];
    float bx = verts[vi * 2], by = verts[vi * 2 + 1];
    float cx = verts[vn * 2], cy = verts[vn * 2 + 1];
    
    // Check if this is a convex vertex
    if (!isConvex(verts, indices, i, ccw)) {
        return false;
    }
    
    // Check if any other vertex is inside this triangle
    for (size_t j = 0; j < n; ++j) {
        if (j == prev || j == i || j == next) continue;
        float px = verts[indices[j] * 2];
        float py = verts[indices[j] * 2 + 1];
        
        // Skip vertices that are coincident with triangle vertices (bridge duplicates)
        const float eps = 1e-6f;
        if ((std::abs(px - ax) < eps && std::abs(py - ay) < eps) ||
            (std::abs(px - bx) < eps && std::abs(py - by) < eps) ||
            (std::abs(px - cx) < eps && std::abs(py - cy) < eps)) {
            continue;
        }
        
        if (pointInTriangle(px, py, ax, ay, bx, by, cx, cy)) {
            return false;
        }
    }
    return true;
}

} // anonymous namespace

bool triangulatePolygon(const float* vertices, uint32_t vertexCount,
                        std::vector<uint32_t>& outIndices) {
    outIndices.clear();
    if (vertexCount < 3) return false;
    if (vertexCount == 3) {
        outIndices = {0, 1, 2};
        return true;
    }
    
    // Build index list
    std::vector<uint32_t> indices(vertexCount);
    for (uint32_t i = 0; i < vertexCount; ++i) {
        indices[i] = i;
    }
    
    // Determine winding order
    bool ccw = isCounterClockwise(vertices, indices);
    
    // Ear clipping
    size_t remaining = indices.size();
    size_t attempts = 0;
    size_t maxAttempts = remaining * remaining; // Safety limit
    
    while (remaining > 3 && attempts < maxAttempts) {
        bool earFound = false;
        
        for (size_t i = 0; i < indices.size(); ++i) {
            if (indices[i] == UINT32_MAX) continue; // Skip removed vertices
            
            // Find actual prev/next in remaining list
            size_t prev = i;
            do {
                prev = (prev + indices.size() - 1) % indices.size();
            } while (indices[prev] == UINT32_MAX && prev != i);
            
            size_t next = i;
            do {
                next = (next + 1) % indices.size();
            } while (indices[next] == UINT32_MAX && next != i);
            
            if (prev == i || next == i) continue;
            
            // Build compact index list for ear test
            std::vector<uint32_t> compact;
            for (size_t j = 0; j < indices.size(); ++j) {
                if (indices[j] != UINT32_MAX) {
                    compact.push_back(indices[j]);
                }
            }
            
            // Find position of current vertex in compact list
            size_t compactI = 0;
            for (size_t j = 0; j < compact.size(); ++j) {
                if (compact[j] == indices[i]) {
                    compactI = j;
                    break;
                }
            }
            
            if (isEar(vertices, compact, compactI, ccw)) {
                size_t compactPrev = (compactI + compact.size() - 1) % compact.size();
                size_t compactNext = (compactI + 1) % compact.size();
                
                outIndices.push_back(compact[compactPrev]);
                outIndices.push_back(compact[compactI]);
                outIndices.push_back(compact[compactNext]);
                
                indices[i] = UINT32_MAX;
                remaining--;
                earFound = true;
                break;
            }
        }
        
        if (!earFound) {
            attempts++;
        }
    }
    
    // Add last triangle
    if (remaining == 3) {
        std::vector<uint32_t> last;
        for (uint32_t idx : indices) {
            if (idx != UINT32_MAX) {
                last.push_back(idx);
            }
        }
        if (last.size() == 3) {
            outIndices.push_back(last[0]);
            outIndices.push_back(last[1]);
            outIndices.push_back(last[2]);
            return true;
        }
    }
    
    return !outIndices.empty();
}

// Find rightmost vertex in a contour
static uint32_t findRightmostVertex(const float* vertices, uint32_t start, uint32_t count) {
    uint32_t rightmost = start;
    float maxX = vertices[start * 2];
    for (uint32_t i = start + 1; i < start + count; ++i) {
        if (vertices[i * 2] > maxX) {
            maxX = vertices[i * 2];
            rightmost = i;
        }
    }
    return rightmost;
}

// Find a visible vertex on the outer contour from a hole vertex
static uint32_t findVisibleVertex(const float* vertices, 
                                   const std::vector<uint32_t>& outerIndices,
                                   float holeX, float holeY) {
    // Simple approach: find closest vertex on outer contour
    // A proper implementation would do ray casting to find visible edge
    uint32_t closest = outerIndices[0];
    float minDist = 1e20f;
    
    for (uint32_t idx : outerIndices) {
        float dx = vertices[idx * 2] - holeX;
        float dy = vertices[idx * 2 + 1] - holeY;
        float dist = dx * dx + dy * dy;
        // Prefer vertices to the left of the hole point for bridge
        if (vertices[idx * 2] <= holeX && dist < minDist) {
            minDist = dist;
            closest = idx;
        }
    }
    
    // If no vertex found to the left, find any closest
    if (minDist > 1e19f) {
        for (uint32_t idx : outerIndices) {
            float dx = vertices[idx * 2] - holeX;
            float dy = vertices[idx * 2 + 1] - holeY;
            float dist = dx * dx + dy * dy;
            if (dist < minDist) {
                minDist = dist;
                closest = idx;
            }
        }
    }
    
    return closest;
}

bool triangulatePolygonGroup(const uint32_t* contourStarts, uint32_t contourCount,
                             const float* vertices, uint32_t vertexCount,
                             std::vector<uint32_t>& outIndices) {
    outIndices.clear();
    if (contourCount == 0 || vertexCount < 3) return false;
    
    // For single contour, just triangulate directly
    if (contourCount == 1) {
        return triangulatePolygon(vertices, vertexCount, outIndices);
    }
    
    // Build contour ranges
    std::vector<std::pair<uint32_t, uint32_t>> contourRanges;
    for (uint32_t i = 0; i < contourCount; ++i) {
        uint32_t start = contourStarts[i];
        uint32_t end = (i + 1 < contourCount) ? contourStarts[i + 1] : vertexCount;
        contourRanges.push_back({start, end});
    }
    
    // Start with outer contour vertices
    std::vector<uint32_t> mergedIndices;
    uint32_t outerStart = contourRanges[0].first;
    uint32_t outerCount = contourRanges[0].second - outerStart;
    
    for (uint32_t i = 0; i < outerCount; ++i) {
        mergedIndices.push_back(outerStart + i);
    }
    
    // Sort holes by rightmost x-coordinate (descending) for proper merging
    std::vector<uint32_t> holeOrder;
    for (uint32_t h = 1; h < contourCount; ++h) {
        holeOrder.push_back(h);
    }
    std::sort(holeOrder.begin(), holeOrder.end(), [&](uint32_t a, uint32_t b) {
        uint32_t aStart = contourRanges[a].first;
        uint32_t aCount = contourRanges[a].second - aStart;
        uint32_t bStart = contourRanges[b].first;
        uint32_t bCount = contourRanges[b].second - bStart;
        float aMaxX = vertices[findRightmostVertex(vertices, aStart, aCount) * 2];
        float bMaxX = vertices[findRightmostVertex(vertices, bStart, bCount) * 2];
        return aMaxX > bMaxX; // Sort descending by rightmost x
    });
    
    // Process each hole in sorted order
    for (uint32_t h : holeOrder) {
        uint32_t holeStart = contourRanges[h].first;
        uint32_t holeEnd = contourRanges[h].second;
        uint32_t holeCount = holeEnd - holeStart;
        
        if (holeCount < 3) continue;
        
        // Find rightmost vertex of hole
        uint32_t holeRightmost = findRightmostVertex(vertices, holeStart, holeCount);
        float holeX = vertices[holeRightmost * 2];
        float holeY = vertices[holeRightmost * 2 + 1];
        
        // Find visible vertex on merged polygon
        uint32_t bridgeTarget = findVisibleVertex(vertices, mergedIndices, holeX, holeY);
        
        // Find the BEST position of bridge target in merged indices
        // When a vertex appears multiple times (from previous hole merging),
        // we want to pick the occurrence that gives the best (shortest) bridge
        size_t bridgePos = 0;
        float bestDist = 1e20f;
        for (size_t i = 0; i < mergedIndices.size(); ++i) {
            if (mergedIndices[i] == bridgeTarget) {
                // Pick the LAST occurrence of the bridge target
                // This ensures we insert after any previously merged holes
                // that also connected to this vertex
                bridgePos = i;
            }
        }
        
        // Insert hole into merged polygon at bridge position
        // Bridge technique: cut from outer to hole, traverse hole completely, 
        // return to bridge vertex, then continue outer polygon
        std::vector<uint32_t> newMerged;
        
        // Copy up to and including bridge point
        for (size_t i = 0; i <= bridgePos; ++i) {
            newMerged.push_back(mergedIndices[i]);
        }
        
        // Add hole vertices: traverse FORWARDS (keep CW direction)
        // When outer is CCW and hole is CW, traversing hole forward (CW) 
        // and returning via bridge gives correct area subtraction
        uint32_t holeOffset = holeRightmost - holeStart;
        for (uint32_t i = 0; i < holeCount; ++i) {
            // Go forwards from rightmost
            uint32_t idx = holeStart + ((holeOffset + i) % holeCount);
            newMerged.push_back(idx);
        }
        
        // Add return bridge: hole entry point (rightmost) and outer bridge vertex
        newMerged.push_back(holeRightmost);
        newMerged.push_back(bridgeTarget);
        
        // Continue with rest of outer polygon (if any vertices remain)
        for (size_t i = bridgePos + 1; i < mergedIndices.size(); ++i) {
            newMerged.push_back(mergedIndices[i]);
        }
        
        mergedIndices = std::move(newMerged);
    }
    
    // Now triangulate the merged polygon
    // Build a vertex array with the merged indices
    // Add tiny offsets to duplicate vertex positions to avoid degenerate edges
    std::vector<float> mergedVerts;
    std::vector<std::pair<float, float>> seenPositions;
    
    for (uint32_t idx : mergedIndices) {
        float x = vertices[idx * 2];
        float y = vertices[idx * 2 + 1];
        
        // Check if we've seen this position before
        int dupCount = 0;
        for (const auto& pos : seenPositions) {
            if (std::abs(pos.first - x) < 1e-6f && std::abs(pos.second - y) < 1e-6f) {
                dupCount++;
            }
        }
        
        // Add tiny offset for duplicates to avoid zero-length edges
        if (dupCount > 0) {
            x += dupCount * 0.01f;
            y += dupCount * 0.01f;
        }
        
        seenPositions.push_back({x, y});
        mergedVerts.push_back(x);
        mergedVerts.push_back(y);
    }
    
    std::vector<uint32_t> localIndices;
    if (!triangulatePolygon(mergedVerts.data(), static_cast<uint32_t>(mergedIndices.size()), localIndices)) {
        return false;
    }
    
    // Map local indices back to original vertex indices
    for (uint32_t localIdx : localIndices) {
        outIndices.push_back(mergedIndices[localIdx]);
    }
    
    return !outIndices.empty();
}

} // namespace yetty::ypaint
