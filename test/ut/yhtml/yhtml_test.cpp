//=============================================================================
// YHtml Buffer Unit Tests
//
// Tests that buffer primitives are written correctly.
// Verifies primitive types, layer values, and coordinates.
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>

#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include "yetty/ydraw/ydraw-prim-writer.gen.h"
#include "html-renderer.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <map>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::card;

//=============================================================================
// Helper: read u32 from float buffer at word index
//=============================================================================
static uint32_t readU32(const float* buf, uint32_t wordIdx) {
    uint32_t val;
    std::memcpy(&val, &buf[wordIdx], sizeof(uint32_t));
    return val;
}

//=============================================================================
// Helper: get primitive type name
//=============================================================================
static const char* primTypeName(uint32_t type) {
    switch (type) {
        case 0: return "Circle";
        case 1: return "Box";
        case 2: return "Segment";
        case 3: return "Triangle";
        case 4: return "Bezier2";
        case 5: return "Bezier3";
        case 6: return "Ellipse";
        case 7: return "Arc";
        case 8: return "RoundedBox";
        default: return "Unknown";
    }
}

//=============================================================================
// Helper: dump buffer primitives for debugging
//=============================================================================
static void dumpBufferPrimitives(YDrawBuffer::Ptr buffer) {
    std::map<uint32_t, int> typeCounts;

    buffer->forEachPrim([&](uint32_t id, const float* data, uint32_t wc) {
        uint32_t type = readU32(data, 0);
        uint32_t layer = readU32(data, 1);
        typeCounts[type]++;

        std::printf("  Prim id=%u type=%u(%s) layer=%u wc=%u\n",
                    id, type, primTypeName(type), layer, wc);

        // Print coordinates based on type
        if (type == 1) { // Box
            std::printf("    Box: cx=%.1f cy=%.1f hw=%.1f hh=%.1f fill=0x%08X stroke=0x%08X sw=%.1f round=%.1f\n",
                        data[2], data[3], data[4], data[5],
                        readU32(data, 6), readU32(data, 7), data[8], data[9]);
        } else if (type == 2) { // Segment
            std::printf("    Segment: x0=%.1f y0=%.1f x1=%.1f y1=%.1f fill=0x%08X stroke=0x%08X sw=%.1f round=%.1f\n",
                        data[2], data[3], data[4], data[5],
                        readU32(data, 6), readU32(data, 7), data[8], data[9]);
        }
    });

    std::printf("  Primitive counts:\n");
    for (auto& [type, count] : typeCounts) {
        std::printf("    %s: %d\n", primTypeName(type), count);
    }

    std::printf("  Text spans: %u\n", buffer->textSpanCount());
    std::printf("  Images: %u\n", buffer->imageCount());
}

//=============================================================================
// Test: Simple table HTML produces correct primitives
//=============================================================================
suite yhtml_buffer_tests = [] {
    "simple_table_produces_boxes_and_segments"_test = [] {
        // Create buffer
        auto bufferRes = YDrawBuffer::create();
        expect(bufferRes.has_value());
        auto buffer = *bufferRes;

        // Simple HTML with a table that should produce:
        // - Background boxes for cells
        // - Segment borders
        const char* html = R"(
            <html>
            <head><style>
                table { border-collapse: collapse; }
                td { border: 1px solid black; background: #cccccc; padding: 5px; }
            </style></head>
            <body>
                <table>
                    <tr><td>Cell1</td><td>Cell2</td></tr>
                </table>
            </body>
            </html>
        )";

        // Render HTML to buffer
        auto result = renderHtmlToBuffer(html, buffer, 16.0f, 400.0f, nullptr);

        std::printf("\n=== Simple Table Test ===\n");
        std::printf("Document: %dx%d\n", result.documentWidth, result.documentHeight);
        std::printf("Primitives in buffer: %u\n", buffer->primCount());

        dumpBufferPrimitives(buffer);

        // Verify we have primitives
        expect(buffer->primCount() > 0_u) << "Buffer should have primitives";

        // Count primitive types
        uint32_t boxCount = 0, segmentCount = 0, otherCount = 0;
        buffer->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            uint32_t type = readU32(data, 0);
            uint32_t layer = readU32(data, 1);

            // Verify layer is 0 (yhtml uses layer 0)
            expect(layer == 0_u) << "Layer should be 0, got " << layer;

            if (type == 1) boxCount++;
            else if (type == 2) segmentCount++;
            else otherCount++;
        });

        std::printf("  Boxes: %u, Segments: %u, Other: %u\n", boxCount, segmentCount, otherCount);

        // For a simple 2-cell table, we expect:
        // - At least 2 background boxes (one per cell)
        // - At least some border segments
        expect(boxCount >= 2_u) << "Should have at least 2 boxes for cell backgrounds";
        expect(segmentCount >= 4_u) << "Should have border segments";
        expect(otherCount == 0_u) << "Should not have unexpected primitive types";
    };

    "table_html_file_test"_test = [] {
        // Load actual table.html
        std::string htmlPath = std::string(CMAKE_SOURCE_DIR) + "/demo/assets/yhtml/table.html";
        std::ifstream file(htmlPath);
        if (!file) {
            std::printf("SKIP: Cannot open %s\n", htmlPath.c_str());
            return;
        }

        std::stringstream ss;
        ss << file.rdbuf();
        std::string html = ss.str();

        // Create buffer
        auto bufferRes = YDrawBuffer::create();
        expect(bufferRes.has_value());
        auto buffer = *bufferRes;

        // Render
        auto result = renderHtmlToBuffer(html, buffer, 13.0f, 600.0f, nullptr);

        std::printf("\n=== table.html Test ===\n");
        std::printf("Document: %dx%d\n", result.documentWidth, result.documentHeight);
        std::printf("Primitives in buffer: %u\n", buffer->primCount());

        dumpBufferPrimitives(buffer);

        // Verify primitives
        expect(buffer->primCount() > 0_u) << "Buffer should have primitives";

        // Count and verify types
        uint32_t boxCount = 0, segmentCount = 0;
        bool hasInvalidType = false;
        bool hasInvalidLayer = false;

        buffer->forEachPrim([&](uint32_t id, const float* data, uint32_t wc) {
            uint32_t type = readU32(data, 0);
            uint32_t layer = readU32(data, 1);

            if (layer != 0) {
                hasInvalidLayer = true;
                std::printf("  ERROR: Prim %u has layer=%u (expected 0)\n", id, layer);
            }

            if (type == 1) boxCount++;
            else if (type == 2) segmentCount++;
            else if (type != 0) { // Circle is OK for list markers
                hasInvalidType = true;
                std::printf("  ERROR: Prim %u has unexpected type=%u (%s)\n",
                            id, type, primTypeName(type));
            }
        });

        expect(!hasInvalidType) << "Should not have unexpected primitive types";
        expect(!hasInvalidLayer) << "All layers should be 0";

        // Table should have backgrounds and borders
        std::printf("  Boxes: %u, Segments: %u\n", boxCount, segmentCount);
        expect(boxCount > 0_u) << "Should have background boxes";
        expect(segmentCount > 0_u) << "Should have border segments";
    };

    "form_html_file_test"_test = [] {
        // Load actual form.html
        std::string htmlPath = std::string(CMAKE_SOURCE_DIR) + "/demo/assets/yhtml/form.html";
        std::ifstream file(htmlPath);
        if (!file) {
            std::printf("SKIP: Cannot open %s\n", htmlPath.c_str());
            return;
        }

        std::stringstream ss;
        ss << file.rdbuf();
        std::string html = ss.str();

        // Create buffer
        auto bufferRes = YDrawBuffer::create();
        expect(bufferRes.has_value());
        auto buffer = *bufferRes;

        // Render
        auto result = renderHtmlToBuffer(html, buffer, 16.0f, 600.0f, nullptr);

        std::printf("\n=== form.html Test ===\n");
        std::printf("Document: %dx%d\n", result.documentWidth, result.documentHeight);
        std::printf("Primitives in buffer: %u\n", buffer->primCount());

        dumpBufferPrimitives(buffer);

        // Verify primitives
        expect(buffer->primCount() > 0_u) << "Buffer should have primitives";

        // Count and verify
        uint32_t boxCount = 0, segmentCount = 0;
        bool hasInvalidType = false;

        buffer->forEachPrim([&](uint32_t id, const float* data, uint32_t) {
            uint32_t type = readU32(data, 0);

            if (type == 1) boxCount++;
            else if (type == 2) segmentCount++;
            else if (type != 0) {
                hasInvalidType = true;
                std::printf("  ERROR: Prim %u has unexpected type=%u (%s)\n",
                            id, type, primTypeName(type));
            }
        });

        expect(!hasInvalidType) << "Should not have unexpected primitive types";
        std::printf("  Boxes: %u, Segments: %u\n", boxCount, segmentCount);
    };
};
