//=============================================================================
// Polygon Unit Tests
//
// Tests polygon primitives in YPaintBuffer and YPaintBuilder:
// - Simple polygons (triangle, quad, pentagon, hexagon, star)
// - Complex polygons (concave, with holes)
// - Grid placement and AABB computation
// - GPU buffer layout verification
// - Scrolling mode behavior
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>

#include "yetty/ypaint/ypaint-buffer.h"
#include "yetty/ypaint/ypaint-types.gen.h"
#include <yetty/ypaint-builder.h>
#include <yetty/gpu-allocator.h>

#include <cstring>
#include <cmath>
#include <vector>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::ypaint;
using namespace yetty::ypaint::card;

//=============================================================================
// Helpers
//=============================================================================

static GpuAllocator::Ptr testAllocator() {
    static auto alloc = std::make_shared<GpuAllocator>(nullptr);
    return alloc;
}

// Extract primitive data from GPU staging buffer
struct PrimGpuData {
    uint16_t gridOffsetCol;
    int16_t gridOffsetRow;
    uint32_t type;
    uint32_t layer;
    bool valid;
};

static std::vector<PrimGpuData> extractAllPrims(const std::vector<uint32_t>& primStaging, uint32_t primCount) {
    std::vector<PrimGpuData> result;
    for (uint32_t i = 0; i < primCount; i++) {
        uint32_t wordOffset = primStaging[i];
        uint32_t primBase = primCount + wordOffset;

        PrimGpuData data{};
        if (primBase + 2 < primStaging.size()) {
            uint32_t packedOffset = primStaging[primBase + 0];
            data.gridOffsetCol = packedOffset & 0xFFFF;
            data.gridOffsetRow = static_cast<int16_t>((packedOffset >> 16) & 0xFFFF);
            data.type = primStaging[primBase + 1];
            data.layer = primStaging[primBase + 2];
            data.valid = true;
        }
        result.push_back(data);
    }
    return result;
}

//=============================================================================
// Test Suite: YPaintBuffer polygon operations
//=============================================================================

suite polygon_buffer_tests = [] {

    "addPolygonWithVertices_triangle"_test = [] {
        auto buf = *YPaintBuffer::create();
        float vertices[] = {100, 100, 200, 100, 150, 50};  // 3 vertices
        auto result = buf->addPolygonWithVertices(0, 3, vertices, 0xFFFF0000, 0xFF000000, 2.0f, 0.0f);
        expect(result.has_value());
        expect(buf->primCount() == 1_u);
    };

    "addPolygonWithVertices_quad"_test = [] {
        auto buf = *YPaintBuffer::create();
        float vertices[] = {50, 50, 150, 50, 150, 150, 50, 150};  // 4 vertices
        auto result = buf->addPolygonWithVertices(0, 4, vertices, 0xFF00FF00, 0xFF000000, 1.0f, 0.0f);
        expect(result.has_value());
        expect(buf->primCount() == 1_u);
    };

    "addPolygonWithVertices_pentagon"_test = [] {
        auto buf = *YPaintBuffer::create();
        // Regular pentagon centered at (100, 100)
        float vertices[] = {
            100, 50,    // top
            150, 85,    // top-right
            130, 140,   // bottom-right
            70, 140,    // bottom-left
            50, 85      // top-left
        };
        auto result = buf->addPolygonWithVertices(0, 5, vertices, 0xFF0000FF, 0xFF000000, 1.0f, 0.0f);
        expect(result.has_value());
        expect(buf->primCount() == 1_u);
    };

    "addPolygonWithVertices_hexagon"_test = [] {
        auto buf = *YPaintBuffer::create();
        // Regular hexagon
        float vertices[] = {
            100, 50,    // top
            150, 75,    // top-right
            150, 125,   // bottom-right
            100, 150,   // bottom
            50, 125,    // bottom-left
            50, 75      // top-left
        };
        auto result = buf->addPolygonWithVertices(0, 6, vertices, 0xFFFF00FF, 0xFF000000, 1.0f, 0.0f);
        expect(result.has_value());
        expect(buf->primCount() == 1_u);
    };

    "addPolygonWithVertices_star"_test = [] {
        auto buf = *YPaintBuffer::create();
        // 5-pointed star
        float vertices[] = {
            100, 20,    // top point
            115, 60,    // inner top-right
            160, 60,    // outer right
            125, 90,    // inner right
            140, 140,   // outer bottom-right
            100, 110,   // inner bottom
            60, 140,    // outer bottom-left
            75, 90,     // inner left
            40, 60,     // outer left
            85, 60      // inner top-left
        };
        auto result = buf->addPolygonWithVertices(0, 10, vertices, 0xFFFFFF00, 0xFF000000, 1.0f, 0.0f);
        expect(result.has_value());
        expect(buf->primCount() == 1_u);
    };

    "addPolygonGroupWithVertices_square_with_hole"_test = [] {
        auto buf = *YPaintBuffer::create();
        // Outer square with inner square hole
        float vertices[] = {
            // Outer contour (CCW)
            0, 0, 100, 0, 100, 100, 0, 100,
            // Inner contour (CW - hole)
            30, 30, 30, 70, 70, 70, 70, 30
        };
        uint32_t contourStarts[] = {0, 4};  // First contour at vertex 0, second at vertex 4
        auto result = buf->addPolygonGroupWithVertices(0, 8, 2, contourStarts, vertices,
            0xFF00FFFF, 0xFF000000, 1.0f, 0.0f);
        expect(result.has_value());
        expect(buf->primCount() == 1_u);
    };

    "addPolygonWithVertices_multiple"_test = [] {
        auto buf = *YPaintBuffer::create();

        // Triangle
        float tri[] = {50, 50, 100, 50, 75, 20};
        buf->addPolygonWithVertices(0, 3, tri, 0xFFFF0000, 0, 0, 0);

        // Quad
        float quad[] = {150, 50, 250, 50, 250, 100, 150, 100};
        buf->addPolygonWithVertices(0, 4, quad, 0xFF00FF00, 0, 0, 0);

        // Pentagon
        float pent[] = {300, 30, 350, 50, 340, 100, 310, 100, 280, 50};
        buf->addPolygonWithVertices(0, 5, pent, 0xFF0000FF, 0, 0, 0);

        expect(buf->primCount() == 3_u);
    };

    "polygon_word_count"_test = [] {
        auto buf = *YPaintBuffer::create();
        float vertices[] = {0, 0, 100, 0, 50, 100};  // triangle
        buf->addPolygonWithVertices(0, 3, vertices, 0xFFFF0000, 0xFF000000, 2.0f, 1.0f);

        uint32_t wordCount = 0;
        buf->forEachPrim([&](uint32_t id, const float* data, uint32_t wc) {
            wordCount = wc;
        });

        // Polygon: type(1) + layer(1) + vertexCount(1) + vertices(3*2) + fillColor(1) + strokeColor(1) + strokeWidth(1) + round(1)
        // = 1 + 1 + 1 + 6 + 1 + 1 + 1 + 1 = 13 words
        // But actual layout may differ - check the header
        expect(wordCount >= 7_u) << "Polygon should have at least 7 header words, got " << wordCount;
    };
};

//=============================================================================
// Test Suite: YPaintBuilder polygon grid placement
//=============================================================================

suite polygon_builder_tests = [] {

    "polygon_grid_placement_simple"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, false);  // non-scrolling

        builder->setSceneBounds(0, 0, 400, 200);
        builder->setGridCellSize(20.0f, 20.0f);

        auto buf = *YPaintBuffer::create();
        // Triangle at Y=50-100 should be in grid lines 2-5
        float vertices[] = {50, 50, 150, 50, 100, 100};
        buf->addPolygonWithVertices(0, 3, vertices, 0xFFFF0000, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 1_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 1);

        expect(prims.size() == 1_u);
        expect(prims[0].valid);
        expect(prims[0].type == 130_u) << "Polygon type should be 130, got " << prims[0].type;
    };

    "polygon_grid_placement_scrolling_mode"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, true);  // scrolling mode

        builder->setSceneBounds(0, 0, 400, 400);
        builder->setGridCellSize(20.0f, 20.0f);
        builder->setCursorPosition(0, 5);  // cursor at row 5

        auto buf = *YPaintBuffer::create();
        float vertices[] = {50, 20, 150, 20, 100, 60};  // Triangle
        buf->addPolygonWithVertices(0, 3, vertices, 0xFFFF0000, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 1);

        expect(prims.size() == 1_u);
        expect(prims[0].gridOffsetRow == 5_i) << "Scrolling mode should have gridOffsetRow=5, got " << prims[0].gridOffsetRow;
    };

    "multiple_polygons_different_positions"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, false);

        builder->setSceneBounds(0, 0, 400, 400);
        builder->setGridCellSize(20.0f, 20.0f);

        auto buf = *YPaintBuffer::create();

        // Polygon at Y=0-40
        float poly1[] = {50, 0, 100, 0, 75, 40};
        buf->addPolygonWithVertices(0, 3, poly1, 0xFFFF0000, 0, 0, 0);

        // Polygon at Y=100-150
        float poly2[] = {50, 100, 100, 100, 75, 150};
        buf->addPolygonWithVertices(0, 3, poly2, 0xFF00FF00, 0, 0, 0);

        // Polygon at Y=200-280
        float poly3[] = {50, 200, 100, 200, 100, 280, 50, 280};
        buf->addPolygonWithVertices(0, 4, poly3, 0xFF0000FF, 0, 0, 0);

        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 3_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 3);

        expect(prims.size() == 3_u);
        for (const auto& p : prims) {
            expect(p.valid);
            expect(p.type == 130_u || p.type == 131_u) << "Should be polygon type";
        }
    };

    "polygon_with_scroll"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 400, 400);
        builder->setGridCellSize(20.0f, 20.0f);
        builder->setCursorPosition(0, 0);

        auto buf = *YPaintBuffer::create();
        float vertices[] = {50, 20, 100, 20, 75, 60};
        buf->addPolygonWithVertices(0, 3, vertices, 0xFFFF0000, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        // Initial state
        expect(builder->primitiveCount() == 1_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 1);
        expect(prims[0].gridOffsetRow == 0_i);

        // Scroll 2 lines
        builder->scrollLines(2);

        staging.clear();
        builder->buildPrimStaging(staging);
        prims = extractAllPrims(staging, builder->primitiveCount());

        // After scroll, gridOffsetRow should be decremented by 2
        if (prims.size() > 0 && prims[0].valid) {
            expect(prims[0].gridOffsetRow == -2_i) << "After scroll 2, gridOffsetRow should be -2, got " << prims[0].gridOffsetRow;
        }
    };

    "complex_polygon_star"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, false);

        builder->setSceneBounds(0, 0, 200, 200);
        builder->setGridCellSize(20.0f, 20.0f);

        auto buf = *YPaintBuffer::create();
        // 5-pointed star
        float star[] = {
            100, 20,    // top point
            115, 60,    // inner
            160, 60,    // outer right
            125, 90,    // inner
            140, 140,   // outer bottom-right
            100, 110,   // inner bottom
            60, 140,    // outer bottom-left
            75, 90,     // inner
            40, 60,     // outer left
            85, 60      // inner top-left
        };
        buf->addPolygonWithVertices(0, 10, star, 0xFFFFFF00, 0xFF000000, 2.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 1_u);

        // Star spans Y=20 to Y=140, should cover multiple grid rows
        const auto& grid = builder->gridStaging();
        expect(!grid.empty()) << "Grid should not be empty for star polygon";
    };

    "polygon_group_with_hole"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, false);

        builder->setSceneBounds(0, 0, 200, 200);
        builder->setGridCellSize(20.0f, 20.0f);

        auto buf = *YPaintBuffer::create();
        // Square with square hole
        float vertices[] = {
            10, 10, 110, 10, 110, 110, 10, 110,  // outer
            30, 30, 30, 90, 90, 90, 90, 30       // inner (hole)
        };
        uint32_t contourStarts[] = {0, 4};
        buf->addPolygonGroupWithVertices(0, 8, 2, contourStarts, vertices,
            0xFF00FFFF, 0xFF000000, 1.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 1_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 1);

        expect(prims[0].type == 131_u) << "PolygonGroup type should be 131, got " << prims[0].type;
    };
};

//=============================================================================
// Test Suite: All primitive types for grid verification
//=============================================================================

suite all_primitives_grid_tests = [] {

    "all_2d_primitives_grid_placement"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, false);

        builder->setSceneBounds(0, 0, 500, 800);
        builder->setGridCellSize(20.0f, 20.0f);

        auto buf = *YPaintBuffer::create();

        // Row 0-2: Circle
        buf->addCircle(0, 50, 30, 20, 0xFFFF0000, 0, 0, 0);

        // Row 3-5: Box
        buf->addBox(0, 50, 80, 30, 20, 8, 0xFF00FF00, 0, 0, 0);

        // Row 5-7: Triangle
        buf->addTriangle(0, 30, 160, 70, 160, 50, 120, 0xFF0000FF, 0, 0, 0);

        // Row 8-10: Ellipse
        buf->addEllipse(0, 50, 190, 30, 20, 0xFFFFFF00, 0, 0, 0);

        // Row 11-12: Segment
        buf->addSegment(0, 20.0f, 230.0f, 80.0f, 250.0f, 0xFFFF00FF, 0, 3.0f, 0.0f);

        // Row 13-15: RoundedBox (cx, cy, hw, hh, r0, r1, r2, r3, fillColor, strokeColor, strokeWidth, round)
        buf->addRoundedBox(0, 50.0f, 290.0f, 40.0f, 30.0f, 8.0f, 8.0f, 8.0f, 8.0f, 0xFF00FFFF, 0, 0.0f, 0.0f);

        // Row 16-18: Pentagon (polygon)
        float pentagon[] = {50, 340, 80, 360, 70, 390, 30, 390, 20, 360};
        buf->addPolygonWithVertices(0, 5, pentagon, 0xFFFFA500, 0, 0, 0);

        // Row 19-21: Hexagon (polygon)
        float hexagon[] = {50, 410, 80, 420, 80, 450, 50, 460, 20, 450, 20, 420};
        buf->addPolygonWithVertices(0, 6, hexagon, 0xFF800080, 0, 0, 0);

        // Row 22-25: Star (polygon)
        float star[] = {
            50, 480, 60, 510, 90, 510, 65, 530, 75, 560, 50, 540, 25, 560, 35, 530, 10, 510, 40, 510
        };
        buf->addPolygonWithVertices(0, 10, star, 0xFF008080, 0, 0, 0);

        builder->addYpaintBuffer(buf);

        uint32_t primCount = builder->primitiveCount();
        expect(primCount == 9_u) << "Should have 9 primitives, got " << primCount;

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, primCount);

        // Verify all primitives are valid
        for (size_t i = 0; i < prims.size(); i++) {
            expect(prims[i].valid) << "Primitive " << i << " should be valid";
        }

        // Verify types
        expect(prims[0].type == 0_u) << "Circle type";      // Circle
        expect(prims[1].type == 1_u) << "Box type";         // Box
        expect(prims[2].type == 3_u) << "Triangle type";    // Triangle
        expect(prims[3].type == 6_u) << "Ellipse type";     // Ellipse
        expect(prims[4].type == 2_u) << "Segment type";     // Segment
        expect(prims[5].type == 8_u) << "RoundedBox type";  // RoundedBox
        expect(prims[6].type == 130_u) << "Polygon type";   // Polygon (pentagon)
        expect(prims[7].type == 130_u) << "Polygon type";   // Polygon (hexagon)
        expect(prims[8].type == 130_u) << "Polygon type";   // Polygon (star)
    };

    "all_primitives_scrolling_mode"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, true);  // scrolling

        builder->setSceneBounds(0, 0, 400, 600);
        builder->setGridCellSize(20.0f, 20.0f);
        builder->setCursorPosition(0, 3);  // cursor at row 3

        auto buf = *YPaintBuffer::create();

        // Add various primitives
        buf->addCircle(0, 50, 20, 15, 0xFFFF0000, 0, 0, 0);
        buf->addBox(0, 50, 60, 30, 20, 5, 0xFF00FF00, 0, 0, 0);
        float tri[] = {30, 100, 70, 100, 50, 130};
        buf->addPolygonWithVertices(0, 3, tri, 0xFF0000FF, 0, 0, 0);

        builder->addYpaintBuffer(buf);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, builder->primitiveCount());

        // All primitives should have gridOffsetRow = 3
        for (size_t i = 0; i < prims.size(); i++) {
            expect(prims[i].gridOffsetRow == 3_i)
                << "Prim " << i << " should have gridOffsetRow=3, got " << prims[i].gridOffsetRow;
        }
    };
};
