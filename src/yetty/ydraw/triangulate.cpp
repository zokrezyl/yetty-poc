#include "triangulate.h"
#include <cmath>
#include <algorithm>

namespace yetty {

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

bool triangulatePolygonGroup(const uint32_t* contourStarts, uint32_t contourCount,
                             const float* vertices, uint32_t vertexCount,
                             std::vector<uint32_t>& outIndices) {
    outIndices.clear();
    if (contourCount == 0 || vertexCount < 3) return false;
    
    // For now, only handle single contour (no holes)
    // TODO: Implement hole handling with bridge edges
    if (contourCount == 1) {
        return triangulatePolygon(vertices, vertexCount, outIndices);
    }
    
    // Simple approach for polygons with holes:
    // For each contour, determine its vertex range
    std::vector<std::pair<uint32_t, uint32_t>> contourRanges;
    for (uint32_t i = 0; i < contourCount; ++i) {
        uint32_t start = contourStarts[i];
        uint32_t end = (i + 1 < contourCount) ? contourStarts[i + 1] : vertexCount;
        contourRanges.push_back({start, end});
    }
    
    // Triangulate outer contour only for now
    // The holes would require bridge edge insertion
    uint32_t outerStart = contourRanges[0].first;
    uint32_t outerEnd = contourRanges[0].second;
    uint32_t outerCount = outerEnd - outerStart;
    
    std::vector<uint32_t> localIndices;
    if (!triangulatePolygon(vertices + outerStart * 2, outerCount, localIndices)) {
        return false;
    }
    
    // Adjust indices to global vertex array
    for (uint32_t idx : localIndices) {
        outIndices.push_back(outerStart + idx);
    }
    
    return !outIndices.empty();
}

} // namespace yetty
