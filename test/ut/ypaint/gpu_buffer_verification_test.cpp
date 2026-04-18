//=============================================================================
// GPU Buffer Verification Tests
//
// Systematically verifies that primitives added via addYpaintBuffer() appear
// correctly in the GPU buffer with proper gridOffset, type, and geometry.
//
// Strategy:
// 1. Start with simple single-primitive tests
// 2. Increase to multiple primitives per buffer
// 3. Test all primitive types
// 4. Verify scroll operations update gridOffset correctly
// 5. Automated verification: INPUT matches OUTPUT
//=============================================================================

#include <boost/ut.hpp>
#include <yetty/ypaint/painter.h>
#include <yetty/gpu-memory-manager.h>
#include <yetty/gpu-allocator.h>
#include "yetty/ypaint/ypaint-buffer.h"
#include "yetty/ypaint/ypaint-types.gen.h"

#include <cstring>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::ypaint;
using namespace yetty::card;

//=============================================================================
// Helpers
//=============================================================================

static uint32_t readU32(const std::vector<uint32_t>& buf, uint32_t idx) {
    return buf[idx];
}

static GpuAllocator::Ptr testAllocator() {
    static auto alloc = std::make_shared<GpuAllocator>(nullptr);
    return alloc;
}

// Extract gridOffset, type, layer from primStaging for primitive at index
struct PrimGpuData {
    uint16_t gridOffsetCol;
    int16_t gridOffsetRow;
    uint32_t type;
    uint32_t layer;
    bool valid;
};

static PrimGpuData extractPrimData(const std::vector<uint32_t>& primStaging, uint32_t primIndex) {
    PrimGpuData data{};
    data.valid = false;

    uint32_t primCount = 0;
    // Count prims by finding first offset that's >= primCount
    for (uint32_t i = 0; i < primStaging.size(); i++) {
        if (primStaging[i] >= i) {
            primCount = i;
            break;
        }
    }
    if (primCount == 0) primCount = primStaging.size();

    if (primIndex >= primCount) return data;

    uint32_t wordOffset = primStaging[primIndex];
    uint32_t primBase = primCount + wordOffset;

    if (primBase + 2 >= primStaging.size()) return data;

    uint32_t packedOffset = primStaging[primBase + 0];
    data.gridOffsetCol = packedOffset & 0xFFFF;
    data.gridOffsetRow = static_cast<int16_t>((packedOffset >> 16) & 0xFFFF);
    data.type = primStaging[primBase + 1];
    data.layer = primStaging[primBase + 2];
    data.valid = true;

    return data;
}

// Get all primitives' GPU data
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
// Test Suite
//=============================================================================

suite gpu_buffer_verification = [] {

    //=========================================================================
    // LEVEL 1: Single primitive, cursor at 0
    //=========================================================================

    "L1_single_circle_cursor_0"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 200);
        builder->setGridCellSize(10.0f, 20.0f);
        builder->setCursorPosition(0, 0);

        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);  // type 0
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 1_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 1);
        expect(prims.size() == 1_u);
        expect(prims[0].valid);
        expect(prims[0].type == 0_u) << "Circle type should be 0";
        expect(prims[0].gridOffsetCol == 0_u) << "Col should be 0";
        expect(prims[0].gridOffsetRow == 0_i) << "Row should be 0";
        expect(prims[0].layer == 0_u) << "Layer should be 0";
    };

    "L1_single_box_cursor_0"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 200);
        builder->setGridCellSize(10.0f, 20.0f);
        builder->setCursorPosition(0, 0);

        auto buf = *YPaintBuffer::create();
        buf->addBox(0, 50, 10, 10, 8, 0xFF00FF00, 0, 0, 0);  // type 1
        builder->addYpaintBuffer(buf);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 1);
        expect(prims[0].type == 1_u) << "Box type should be 1";
        expect(prims[0].gridOffsetRow == 0_i);
    };

    "L1_single_triangle_cursor_0"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 200);
        builder->setGridCellSize(10.0f, 20.0f);
        builder->setCursorPosition(0, 0);

        auto buf = *YPaintBuffer::create();
        buf->addTriangle(0, 30, 5, 70, 5, 50, 15, 0xFFFF0000, 0, 0, 0);  // type 3
        builder->addYpaintBuffer(buf);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 1);
        expect(prims[0].type == 3_u) << "Triangle type should be 3";
        expect(prims[0].gridOffsetRow == 0_i);
    };

    //=========================================================================
    // LEVEL 2: Single primitive, cursor at non-zero row
    //=========================================================================

    "L2_single_circle_cursor_5"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);
        builder->setCursorPosition(0, 5);

        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 1);
        expect(prims[0].type == 0_u);
        expect(prims[0].gridOffsetRow == 5_i) << "Row should be 5 (cursor position)";
    };

    "L2_single_box_cursor_10"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);
        builder->setCursorPosition(3, 10);

        auto buf = *YPaintBuffer::create();
        buf->addBox(0, 50, 10, 10, 8, 0xFF00FF00, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 1);
        expect(prims[0].type == 1_u);
        expect(prims[0].gridOffsetCol == 3_u) << "Col should be 3";
        expect(prims[0].gridOffsetRow == 10_i) << "Row should be 10";
    };

    //=========================================================================
    // LEVEL 3: Multiple primitives in SAME buffer, cursor at 0
    //=========================================================================

    "L3_two_prims_same_buffer_cursor_0"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 200);
        builder->setGridCellSize(10.0f, 20.0f);
        builder->setCursorPosition(0, 0);

        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);   // type 0
        buf->addBox(0, 50, 10, 10, 5, 0xFF00FF00, 0, 0, 0);   // type 1
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 2_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 2);

        // Both should have same gridOffset (cursor was 0,0)
        std::map<uint32_t, PrimGpuData> byType;
        for (auto& p : prims) {
            byType[p.type] = p;
        }

        expect(byType.count(0) == 1_u) << "Should have Circle (type 0)";
        expect(byType.count(1) == 1_u) << "Should have Box (type 1)";
        expect(byType[0].gridOffsetRow == 0_i);
        expect(byType[1].gridOffsetRow == 0_i);
    };

    "L3_three_prims_same_buffer_cursor_0"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 200);
        builder->setGridCellSize(10.0f, 20.0f);
        builder->setCursorPosition(0, 0);

        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);    // type 0
        buf->addBox(0, 50, 10, 10, 5, 0xFF00FF00, 0, 0, 0);    // type 1
        buf->addTriangle(0, 30, 5, 70, 5, 50, 15, 0xFFFF0000, 0, 0, 0);  // type 3
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 3_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 3);

        std::set<uint32_t> types;
        for (auto& p : prims) {
            types.insert(p.type);
            expect(p.gridOffsetRow == 0_i) << "All prims should have row 0";
        }

        expect(types.count(0) == 1_u) << "Should have Circle";
        expect(types.count(1) == 1_u) << "Should have Box";
        expect(types.count(3) == 1_u) << "Should have Triangle";
    };

    //=========================================================================
    // LEVEL 4: Multiple primitives in SAME buffer, cursor at non-zero
    //=========================================================================

    "L4_three_prims_same_buffer_cursor_7"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);
        builder->setCursorPosition(2, 7);

        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);
        buf->addBox(0, 50, 10, 10, 5, 0xFF00FF00, 0, 0, 0);
        buf->addTriangle(0, 30, 5, 70, 5, 50, 15, 0xFFFF0000, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 3);

        for (auto& p : prims) {
            expect(p.gridOffsetCol == 2_u) << "All prims should have col 2";
            expect(p.gridOffsetRow == 7_i) << "All prims should have row 7";
        }
    };

    //=========================================================================
    // LEVEL 5: Multiple buffers added at different cursor positions
    //=========================================================================

    "L5_two_buffers_different_cursors"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);

        // First buffer at row 0
        builder->setCursorPosition(0, 0);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);  // type 0
        builder->addYpaintBuffer(buf1);

        // Second buffer at row 5
        builder->setCursorPosition(0, 5);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 50, 10, 10, 5, 0xFF00FF00, 0, 0, 0);  // type 1
        builder->addYpaintBuffer(buf2);

        expect(builder->primitiveCount() == 2_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 2);

        std::map<uint32_t, PrimGpuData> byType;
        for (auto& p : prims) byType[p.type] = p;

        expect(byType[0].gridOffsetRow == 0_i) << "Circle should be at row 0";
        expect(byType[1].gridOffsetRow == 5_i) << "Box should be at row 5";
    };

    "L5_three_buffers_different_cursors"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);

        // Buffer 1 at row 0
        builder->setCursorPosition(0, 0);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);
        builder->addYpaintBuffer(buf1);

        // Buffer 2 at row 3
        builder->setCursorPosition(0, 3);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 50, 10, 10, 5, 0xFF00FF00, 0, 0, 0);
        builder->addYpaintBuffer(buf2);

        // Buffer 3 at row 8
        builder->setCursorPosition(0, 8);
        auto buf3 = *YPaintBuffer::create();
        buf3->addTriangle(0, 30, 5, 70, 5, 50, 15, 0xFFFF0000, 0, 0, 0);
        builder->addYpaintBuffer(buf3);

        expect(builder->primitiveCount() == 3_u);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);

        auto prims = extractAllPrims(staging, 3);

        std::map<uint32_t, PrimGpuData> byType;
        for (auto& p : prims) byType[p.type] = p;

        expect(byType[0].gridOffsetRow == 0_i) << "Circle at row 0";
        expect(byType[1].gridOffsetRow == 3_i) << "Box at row 3";
        expect(byType[3].gridOffsetRow == 8_i) << "Triangle at row 8";
    };

    //=========================================================================
    // LEVEL 6: Scroll and verify gridOffset is decremented
    //=========================================================================

    "L6_scroll_1_line_updates_offset"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);

        // Add at row 5
        builder->setCursorPosition(0, 5);
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        // Verify before scroll
        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 1);
        expect(prims[0].gridOffsetRow == 5_i) << "Before scroll: row 5";

        // Scroll 1 line
        builder->scrollLines(1);

        // Verify after scroll
        staging.clear();
        builder->buildPrimStaging(staging);
        prims = extractAllPrims(staging, builder->primitiveCount());
        expect(prims[0].gridOffsetRow == 4_i) << "After scroll 1: row should be 4";
    };

    "L6_scroll_3_lines_updates_offset"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);

        // Add at row 10
        builder->setCursorPosition(0, 10);
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        // Scroll 3 lines
        builder->scrollLines(3);

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, builder->primitiveCount());
        expect(prims[0].gridOffsetRow == 7_i) << "After scroll 3: row should be 7";
    };

    "L6_scroll_removes_prim_when_line_popped"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);

        // Add at row 0
        builder->setCursorPosition(0, 0);
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 1_u) << "Before scroll: 1 prim";

        // Scroll 1 line - should remove the prim
        builder->scrollLines(1);

        expect(builder->primitiveCount() == 0_u) << "After scroll 1: prim should be gone";
    };

    //=========================================================================
    // LEVEL 7: Multiple prims, scroll, verify correct ones remain
    //=========================================================================

    "L7_two_prims_scroll_removes_first"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);

        // Prim 1 at row 0
        builder->setCursorPosition(0, 0);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);  // type 0
        builder->addYpaintBuffer(buf1);

        // Prim 2 at row 5
        builder->setCursorPosition(0, 5);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 50, 10, 10, 5, 0xFF00FF00, 0, 0, 0);  // type 1
        builder->addYpaintBuffer(buf2);

        expect(builder->primitiveCount() == 2_u);

        // Scroll 1 line - Circle at row 0 should be gone
        builder->scrollLines(1);

        expect(builder->primitiveCount() == 1_u) << "Should have 1 prim after scroll";

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 1);

        expect(prims[0].type == 1_u) << "Remaining should be Box (type 1)";
        expect(prims[0].gridOffsetRow == 4_i) << "Box row should be 4 (was 5, scroll 1)";
    };

    "L7_three_prims_scroll_2_removes_two"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 400);
        builder->setGridCellSize(10.0f, 20.0f);

        // Prim at row 0, 1, 5
        builder->setCursorPosition(0, 0);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50, 10, 5, 0xFF0000FF, 0, 0, 0);
        builder->addYpaintBuffer(buf1);

        builder->setCursorPosition(0, 1);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 50, 10, 10, 5, 0xFF00FF00, 0, 0, 0);
        builder->addYpaintBuffer(buf2);

        builder->setCursorPosition(0, 5);
        auto buf3 = *YPaintBuffer::create();
        buf3->addTriangle(0, 30, 5, 70, 5, 50, 15, 0xFFFF0000, 0, 0, 0);
        builder->addYpaintBuffer(buf3);

        expect(builder->primitiveCount() == 3_u);

        // Scroll 2 - removes prims at rows 0 and 1
        builder->scrollLines(2);

        expect(builder->primitiveCount() == 1_u) << "Should have 1 prim after scroll 2";

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 1);

        expect(prims[0].type == 3_u) << "Remaining should be Triangle (type 3)";
        expect(prims[0].gridOffsetRow == 3_i) << "Triangle row should be 3 (was 5, scroll 2)";
    };

    //=========================================================================
    // LEVEL 8: All 2D primitive types
    //=========================================================================

    "L8_all_basic_2d_types"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 200, 800);
        builder->setGridCellSize(10.0f, 20.0f);

        std::vector<std::pair<uint32_t, std::string>> expectedTypes;

        auto buf = *YPaintBuffer::create();

        // Add all basic 2D types in same buffer
        buf->addCircle(0, 50, 10, 5, 0xFF000001, 0, 0, 0);
        expectedTypes.push_back({0, "Circle"});

        buf->addBox(0, 50, 10, 10, 5, 0xFF000002, 0, 0, 0);
        expectedTypes.push_back({1, "Box"});

        buf->addSegment(0, 10, 10, 90, 10, 0xFF000003, 0, 1.0f, 0);
        expectedTypes.push_back({2, "Segment"});

        buf->addTriangle(0, 30, 5, 70, 5, 50, 15, 0xFF000004, 0, 0, 0);
        expectedTypes.push_back({3, "Triangle"});

        buf->addEllipse(0, 50, 10, 8, 5, 0xFF000005, 0, 0, 0);
        expectedTypes.push_back({6, "Ellipse"});

        buf->addRoundedBox(0, 50, 10, 15, 8, 2, 2, 2, 2, 0xFF000006, 0, 0, 0);
        expectedTypes.push_back({8, "RoundedBox"});

        buf->addPentagon(0, 50, 10, 8, 0xFF000007, 0, 0, 0);
        expectedTypes.push_back({10, "Pentagon"});

        buf->addHexagon(0, 50, 10, 8, 0xFF000008, 0, 0, 0);
        expectedTypes.push_back({11, "Hexagon"});

        builder->setCursorPosition(0, 0);
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == expectedTypes.size());

        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, builder->primitiveCount());

        std::set<uint32_t> foundTypes;
        for (auto& p : prims) {
            foundTypes.insert(p.type);
            expect(p.gridOffsetRow == 0_i) << "All should have row 0";
        }

        for (auto& [type, name] : expectedTypes) {
            expect(foundTypes.count(type) == 1_u) << "Should find " << name << " (type " << type << ")";
        }
    };

    //=========================================================================
    // LEVEL 9: Automated verification - parametric test
    //=========================================================================

    "L9_automated_scroll_verification"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 1000);
        builder->setGridCellSize(10.0f, 20.0f);

        // Add prims at rows 0, 2, 4, 6, 8, 10
        std::vector<uint16_t> rows = {0, 2, 4, 6, 8, 10};
        for (auto row : rows) {
            builder->setCursorPosition(0, row);
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50, 10, 5, 0xFF000000 | row, 0, 0, 0);
            builder->addYpaintBuffer(buf);
        }

        expect(builder->primitiveCount() == rows.size());

        // Scroll 1 at a time and verify
        for (uint16_t scroll = 1; scroll <= 11; scroll++) {
            builder->scrollLines(1);

            // Count how many rows are >= scroll
            uint32_t expectedCount = 0;
            for (auto row : rows) {
                if (row >= scroll) expectedCount++;
            }

            expect(builder->primitiveCount() == expectedCount)
                << "After scroll " << scroll << ": expected " << expectedCount << " prims";

            // Verify remaining prims have correct offsets
            if (expectedCount > 0) {
                std::vector<uint32_t> staging;
                builder->buildPrimStaging(staging);
                auto prims = extractAllPrims(staging, expectedCount);

                for (auto& p : prims) {
                    // gridOffsetRow should be original row - scroll
                    expect(p.gridOffsetRow >= 0)
                        << "After scroll " << scroll << ": row should be >= 0";
                }
            }
        }

        expect(builder->primitiveCount() == 0_u) << "All prims should be gone after scroll 11";
    };

    //=========================================================================
    // LEVEL 10: Stress test with many primitives
    //=========================================================================

    "L10_100_prims_scroll_verification"_test = [] {
        auto gpuAlloc = testAllocator();
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);

        builder->setSceneBounds(0, 0, 100, 2500);
        builder->setGridCellSize(10.0f, 20.0f);

        const uint32_t NUM_PRIMS = 100;

        // Add 100 prims, one per row
        for (uint32_t i = 0; i < NUM_PRIMS; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50, 10, 5, 0xFF000000 | i, 0, 0, 0);
            builder->addYpaintBuffer(buf);
        }

        expect(builder->primitiveCount() == NUM_PRIMS);

        // Scroll 50 lines
        builder->scrollLines(50);
        expect(builder->primitiveCount() == 50_u) << "After scroll 50: should have 50 prims";

        // Verify remaining prims have correct offsets
        std::vector<uint32_t> staging;
        builder->buildPrimStaging(staging);
        auto prims = extractAllPrims(staging, 50);

        std::set<int16_t> foundRows;
        for (auto& p : prims) {
            foundRows.insert(p.gridOffsetRow);
            expect(p.gridOffsetRow >= 0_i) << "All rows should be >= 0";
            expect(p.gridOffsetRow < 50_i) << "All rows should be < 50";
        }

        // Should have rows 0-49 (originally 50-99)
        expect(foundRows.size() == 50_u) << "Should have 50 unique rows";
    };
};
