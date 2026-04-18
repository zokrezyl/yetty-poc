#pragma once

#include <cstdint>
#include <vector>

namespace yetty {

// Simple ear-clipping triangulation for convex and simple concave polygons
// Vertices: array of x,y pairs (size = vertexCount * 2)
// Output: triangle indices (size = (vertexCount - 2) * 3)
// Returns true on success

bool triangulatePolygon(const float* vertices, uint32_t vertexCount,
                        std::vector<uint32_t>& outIndices);

// Triangulate polygon with holes
// contourStarts: vertex indices where each contour begins (first = outer, rest = holes)
// contourCount: number of contours
// vertices: all vertices from all contours concatenated
// vertexCount: total number of vertices across all contours
// Returns triangle indices into the vertices array

bool triangulatePolygonGroup(const uint32_t* contourStarts, uint32_t contourCount,
                             const float* vertices, uint32_t vertexCount,
                             std::vector<uint32_t>& outIndices);

} // namespace yetty
