//=============================================================================
// Triangulation Unit Tests
//
// Tests ear-clipping triangulation for simple polygons and polygons with holes.
//=============================================================================

#include <boost/ut.hpp>
#include "../../../src/yetty/ypaint/triangulate.h"
#include <cmath>
#include <algorithm>

using namespace boost::ut;
using namespace yetty::ypaint;

//=============================================================================
// Helper: check if triangulation produces valid triangles
//=============================================================================
static bool isValidTriangulation(const std::vector<uint32_t>& indices, uint32_t vertexCount) {
    // Must have 3 indices per triangle
    if (indices.size() % 3 != 0) return false;
    // Expected triangles for simple polygon: vertexCount - 2
    // For polygons with holes, this is more complex
    // All indices must be valid
    for (uint32_t idx : indices) {
        if (idx >= vertexCount) return false;
    }
    return true;
}

// Calculate signed area of triangle
static float triangleArea(float x0, float y0, float x1, float y1, float x2, float y2) {
    return 0.5f * ((x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0));
}

// Calculate total area from triangulation
static float totalTriangulatedArea(const float* vertices, const std::vector<uint32_t>& indices) {
    float total = 0.0f;
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        uint32_t i0 = indices[i], i1 = indices[i + 1], i2 = indices[i + 2];
        total += std::abs(triangleArea(
            vertices[i0 * 2], vertices[i0 * 2 + 1],
            vertices[i1 * 2], vertices[i1 * 2 + 1],
            vertices[i2 * 2], vertices[i2 * 2 + 1]
        ));
    }
    return total;
}

// Calculate polygon area using shoelace formula
static float polygonArea(const float* vertices, uint32_t vertexCount) {
    float area = 0.0f;
    for (uint32_t i = 0; i < vertexCount; ++i) {
        uint32_t j = (i + 1) % vertexCount;
        area += vertices[i * 2] * vertices[j * 2 + 1];
        area -= vertices[j * 2] * vertices[i * 2 + 1];
    }
    return std::abs(area) * 0.5f;
}

//=============================================================================
// Tests
//=============================================================================

suite triangulation_tests = [] {
    "triangle"_test = [] {
        // Simplest case: 3 vertices
        float vertices[] = {0, 0, 100, 0, 50, 100};
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygon(vertices, 3, indices);
        
        expect(result);
        expect(indices.size() == 3_u);
        expect(isValidTriangulation(indices, 3));
    };
    
    "square"_test = [] {
        // Square: 4 vertices -> 2 triangles
        float vertices[] = {0, 0, 100, 0, 100, 100, 0, 100};
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygon(vertices, 4, indices);
        
        expect(result);
        expect(indices.size() == 6_u); // 2 triangles * 3 indices
        expect(isValidTriangulation(indices, 4));
        
        // Area should match
        float polyArea = polygonArea(vertices, 4);
        float triArea = totalTriangulatedArea(vertices, indices);
        expect(std::abs(polyArea - triArea) < 0.01f) << "Area mismatch: " << polyArea << " vs " << triArea;
    };
    
    "pentagon"_test = [] {
        // Pentagon: 5 vertices -> 3 triangles
        float vertices[] = {
            100, 0,    // top
            195, 69,   // top-right
            159, 181,  // bottom-right
            41, 181,   // bottom-left
            5, 69      // top-left
        };
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygon(vertices, 5, indices);
        
        expect(result);
        expect(indices.size() == 9_u); // 3 triangles * 3 indices
        expect(isValidTriangulation(indices, 5));
    };
    
    "hexagon"_test = [] {
        // Regular hexagon: 6 vertices -> 4 triangles
        float vertices[] = {
            100, 0,
            187, 50,
            187, 150,
            100, 200,
            13, 150,
            13, 50
        };
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygon(vertices, 6, indices);
        
        expect(result);
        expect(indices.size() == 12_u); // 4 triangles * 3 indices
        expect(isValidTriangulation(indices, 6));
    };
    
    "concave_arrow"_test = [] {
        // Arrow shape (concave): 7 vertices -> 5 triangles
        float vertices[] = {
            0, 50,     // left edge top
            50, 50,    // shaft top
            50, 0,     // arrow top
            100, 50,   // arrow tip
            50, 100,   // arrow bottom
            50, 50,    // shaft bottom (same as shaft top for this shape)
            0, 50      // left edge bottom
        };
        // Actually let's use a proper concave shape
        float arrow[] = {
            0, 40,     // 0: left top
            60, 40,    // 1: shaft top-right
            60, 20,    // 2: notch top
            100, 50,   // 3: tip
            60, 80,    // 4: notch bottom
            60, 60,    // 5: shaft bottom-right
            0, 60      // 6: left bottom
        };
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygon(arrow, 7, indices);
        
        expect(result);
        expect(indices.size() == 15_u); // 5 triangles * 3 indices
        expect(isValidTriangulation(indices, 7));
    };
    
    "star_concave"_test = [] {
        // 5-point star (very concave): 10 vertices -> 8 triangles
        float vertices[] = {
            100, 0,    // outer top
            115, 35,   // inner
            150, 40,   // outer right-top
            125, 60,   // inner
            140, 100,  // outer right-bottom
            100, 75,   // inner bottom
            60, 100,   // outer left-bottom
            75, 60,    // inner
            50, 40,    // outer left-top
            85, 35     // inner
        };
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygon(vertices, 10, indices);
        
        expect(result);
        expect(indices.size() == 24_u); // 8 triangles * 3 indices
        expect(isValidTriangulation(indices, 10));
    };
    
    "square_with_square_hole"_test = [] {
        // Outer square + inner square hole
        // Outer: 4 vertices, Inner: 4 vertices = 8 total
        float vertices[] = {
            // Outer (CCW)
            0, 0,
            100, 0,
            100, 100,
            0, 100,
            // Inner hole (CW - opposite winding)
            30, 30,
            30, 70,
            70, 70,
            70, 30
        };
        uint32_t contourStarts[] = {0, 4};
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygonGroup(contourStarts, 2, vertices, 8, indices);
        
        expect(result);
        expect(isValidTriangulation(indices, 8));
        
        // Area should be outer - inner = 10000 - 1600 = 8400
        float triArea = totalTriangulatedArea(vertices, indices);
        float expectedArea = 100.0f * 100.0f - 40.0f * 40.0f; // 10000 - 1600 = 8400
        expect(std::abs(triArea - expectedArea) < 10.0f) << "Area mismatch: " << triArea << " vs " << expectedArea;
    };
    
    "single_contour_group"_test = [] {
        // PolygonGroup with single contour should work like regular polygon
        float vertices[] = {0, 0, 100, 0, 100, 100, 0, 100};
        uint32_t contourStarts[] = {0};
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygonGroup(contourStarts, 1, vertices, 4, indices);
        
        expect(result);
        expect(indices.size() == 6_u);
        expect(isValidTriangulation(indices, 4));
    };
    
    "degenerate_cases"_test = [] {
        std::vector<uint32_t> indices;
        
        // Too few vertices
        float twoVerts[] = {0, 0, 100, 0};
        expect(!triangulatePolygon(twoVerts, 2, indices));
        
        // Empty
        expect(!triangulatePolygon(nullptr, 0, indices));
        
        // Zero contours
        float verts[] = {0, 0, 100, 0, 50, 100};
        expect(!triangulatePolygonGroup(nullptr, 0, verts, 3, indices));
    };
    
    "polygon_signed_area"_test = [] {
        // Test signed area calculation for winding order detection
        // CCW polygon should have negative signed area (in our coordinate system)
        float outerCCW[] = {0, 0, 100, 0, 100, 100, 0, 100};
        float areaCCW = 0.0f;
        for (int i = 0; i < 4; ++i) {
            int j = (i + 1) % 4;
            areaCCW += (outerCCW[j*2] - outerCCW[i*2]) * (outerCCW[j*2+1] + outerCCW[i*2+1]);
        }
        areaCCW /= 2.0f;
        expect(areaCCW < 0) << "CCW square should have negative signed area: " << areaCCW;
        
        // CW hole should have positive signed area
        float holeCW[] = {30, 30, 30, 70, 70, 70, 70, 30};
        float areaCW = 0.0f;
        for (int i = 0; i < 4; ++i) {
            int j = (i + 1) % 4;
            areaCW += (holeCW[j*2] - holeCW[i*2]) * (holeCW[j*2+1] + holeCW[i*2+1]);
        }
        areaCW /= 2.0f;
        expect(areaCW > 0) << "CW hole should have positive signed area: " << areaCW;
    };
    
    "correct_merged_polygon_area"_test = [] {
        // Verify the correct merged polygon [0,1,2,3,6,7,4,5,6,3] gives expected area
        float vertices[] = {
            0, 0,     // 0
            100, 0,   // 1
            100, 100, // 2
            0, 100,   // 3
            30, 30,   // 4
            30, 70,   // 5
            70, 70,   // 6
            70, 30    // 7
        };
        
        // Compute shoelace area for correct merged polygon [0,1,2,3,6,7,4,5,6,3]
        int merged[] = {0, 1, 2, 3, 6, 7, 4, 5, 6, 3};
        float mergedArea = 0.0f;
        for (int i = 0; i < 10; ++i) {
            int j = (i + 1) % 10;
            int vi = merged[i];
            int vj = merged[j];
            mergedArea += vertices[vi*2] * vertices[vj*2+1];
            mergedArea -= vertices[vj*2] * vertices[vi*2+1];
        }
        mergedArea = std::abs(mergedArea) / 2.0f;
        
        // With proper bridge insertion, area should be 8400 (outer - hole)
        expect(std::abs(mergedArea - 8400.0f) < 1.0f) << "Merged polygon should have area 8400, got: " << mergedArea;
    };
    
    "multiple_holes"_test = [] {
        // Outer square with two small square holes
        float vertices[] = {
            // Outer (CCW)
            0, 0,
            100, 0,
            100, 100,
            0, 100,
            // Hole 1 (CW) - left side
            10, 40,
            10, 60,
            30, 60,
            30, 40,
            // Hole 2 (CW) - right side
            70, 40,
            70, 60,
            90, 60,
            90, 40
        };
        uint32_t contourStarts[] = {0, 4, 8};
        std::vector<uint32_t> indices;
        
        bool result = triangulatePolygonGroup(contourStarts, 3, vertices, 12, indices);
        
        expect(result);
        expect(isValidTriangulation(indices, 12));
        
        // Area should be outer - hole1 - hole2 = 10000 - 400 - 400 = 9200
        float triArea = totalTriangulatedArea(vertices, indices);
        float expectedArea = 10000.0f - 400.0f - 400.0f;
        expect(std::abs(triArea - expectedArea) < 20.0f) << "Area mismatch: " << triArea << " vs " << expectedArea;
    };
};
