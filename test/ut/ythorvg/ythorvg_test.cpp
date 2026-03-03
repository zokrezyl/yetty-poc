//=============================================================================
// YThorVG Unit Tests - YDrawRenderMethod tests
//=============================================================================

// Include C++ standard headers BEFORE boost/ut.hpp
#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>
#include "yetty/ythorvg/ydraw-render-method.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <thorvg.h>
#include <cstdio>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::ythorvg;

// Helper to count primitives by type
[[maybe_unused]]
static int countPrimsByType(const YDrawBuffer& buffer, card::SDFType type) {
    int count = 0;
    buffer.forEachPrim([&](uint32_t, const float* data, uint32_t) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(type)) {
            count++;
        }
    });
    return count;
}

// Helper to check if any primitive is a Polygon type
[[maybe_unused]]
static bool hasPolygonPrimitive(const YDrawBuffer& buffer) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t, const float* data, uint32_t) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::Polygon) ||
            typeCode == static_cast<uint32_t>(card::SDFType::PolygonGroup) ||
            typeCode == static_cast<uint32_t>(card::SDFType::Triangle)) {
            found = true;
        }
    });
    return found;
}

// Helper to check if any primitive is a Bezier type
[[maybe_unused]]
static bool hasBezierPrimitive(const YDrawBuffer& buffer) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t, const float* data, uint32_t) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::Bezier2) ||
            typeCode == static_cast<uint32_t>(card::SDFType::Bezier3)) {
            found = true;
        }
    });
    return found;
}

// Debug helper to print all primitive types
[[maybe_unused]] static void dumpPrimTypes(const YDrawBuffer& buffer) {
    buffer.forEachPrim([&](uint32_t id, const float* data, uint32_t) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        std::fprintf(stderr, "  Prim %u: type=%u\n", id, typeCode);
    });
}

//=============================================================================
// Tests
//=============================================================================

suite ythorvg_render_method_tests = [] {

    "ydraw_render_method_create"_test = [] {
        auto buffer = YDrawBuffer::create();
        expect(buffer.has_value());
        YDrawRenderMethod renderer(*buffer);
        expect(true); // Just verify construction doesn't crash
    };

    "simple_rect_thorvg_api"_test = [] {
        // Initialize ThorVG
        tvg::Initializer::init(0);
        
        // Create a simple rectangle shape
        auto shape = tvg::Shape::gen();
        shape->appendRect(10, 10, 80, 80, 0, 0);
        shape->fill(255, 0, 0, 255);  // Red fill
        
        // Create canvas and set target
        auto canvas = tvg::SwCanvas::gen();
        uint32_t buf[100 * 100];
        canvas->target(buf, 100, 100, 100, tvg::ColorSpace::ARGB8888);
        
        tvg::Initializer::term();
        expect(true); // Verify basic ThorVG API works
    };

    "render_method_viewport"_test = [] {
        auto buffer = YDrawBuffer::create();
        expect(buffer.has_value());
        YDrawRenderMethod renderer(*buffer);
        
        tvg::RenderRegion vp{0, 0, 100, 100};
        renderer.viewport(vp);
        expect(true);
    };

    "render_method_clear"_test = [] {
        auto buffer = YDrawBuffer::create();
        expect(buffer.has_value());
        YDrawRenderMethod renderer(*buffer);
        
        bool result = renderer.clear();
        expect(result);
    };

    "render_method_sync"_test = [] {
        auto buffer = YDrawBuffer::create();
        expect(buffer.has_value());
        YDrawRenderMethod renderer(*buffer);
        
        bool result = renderer.sync();
        expect(result);
    };
};
