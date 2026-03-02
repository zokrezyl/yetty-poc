//=============================================================================
// ThorVG Rounded Rectangle Tests
//
// Tests rounded rectangle corner radius extraction from ThorVG paths.
//=============================================================================

#include <boost/ut.hpp>
#include "yetty/thorvg/thorvg-renderer.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <thorvg.h>
#include <cmath>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::thorvg;

// Helper to check if buffer has a Box primitive
static bool hasBoxPrimitive(const YDrawBuffer& buffer) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t, const float* data, uint32_t) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::Box)) {
            found = true;
        }
    });
    return found;
}

// Helper to extract Box primitive data (returns true if found)
// Box layout from ydraw-primitives.yaml:
//   type(u32), layer(u32), cx(f32), cy(f32), hw(f32), hh(f32), 
//   fillColor(u32), strokeColor(u32), strokeWidth(f32), round(f32)
static bool getBoxData(const YDrawBuffer& buffer, float& cx, float& cy, 
                       float& hw, float& hh, float& round) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t, const float* data, uint32_t wordCount) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::Box) && wordCount >= 10) {
            // Skip type[0] and layer[1]
            cx = data[2];
            cy = data[3];
            hw = data[4];
            hh = data[5];
            // data[6], data[7] are colors (u32)
            // data[8] is strokeWidth
            round = data[9];
            found = true;
        }
    });
    return found;
}

//=============================================================================
// Tests
//=============================================================================

suite thorvg_rounded_rect_tests = [] {

    "rounded_rect_basic"_test = [] {
        // Basic rounded rect verification
        const char* svg = R"(<svg width="200" height="200">
            <rect x="20" y="20" width="160" height="120" rx="15" ry="15" fill="purple"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(hasBoxPrimitive(*buffer)) << "Rounded rect should be detected as Box";
        
        float cx, cy, hw, hh, round;
        if (getBoxData(*buffer, cx, cy, hw, hh, round)) {
            expect(round > 14.0f && round < 16.0f) << "Expected round ~15, got " << round;
        }
    };

    "simple_rect_no_rounding"_test = [] {
        const char* svg = R"(<svg width="200" height="200">
            <rect x="20" y="20" width="160" height="120" fill="red"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        
        float cx, cy, hw, hh, round;
        if (getBoxData(*buffer, cx, cy, hw, hh, round)) {
            expect(round < 1.0f) << "Simple rect should have round=0, got " << round;
        }
    };

    "rounded_rect_with_rx_ry"_test = [] {
        const char* svg = R"(<svg width="200" height="200">
            <rect x="20" y="20" width="160" height="120" rx="15" ry="15" fill="purple"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(hasBoxPrimitive(*buffer)) << "Rounded rect should be detected as Box";
        
        float cx, cy, hw, hh, round;
        if (getBoxData(*buffer, cx, cy, hw, hh, round)) {
            // Corner radius should be approximately 15
            expect(round > 10.0f && round < 20.0f) 
                << "Expected round ~15, got " << round;
        }
    };

    "rounded_rect_with_rx_only"_test = [] {
        // When only rx is specified, ry defaults to rx
        const char* svg = R"(<svg width="200" height="200">
            <rect x="10" y="10" width="100" height="80" rx="20" fill="blue"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(hasBoxPrimitive(*buffer)) << "Rounded rect should be detected as Box";
        
        float cx, cy, hw, hh, round;
        if (getBoxData(*buffer, cx, cy, hw, hh, round)) {
            expect(round > 15.0f && round < 25.0f) 
                << "Expected round ~20, got " << round;
        }
    };

    "rounded_rect_large_radius"_test = [] {
        // Large corner radius (clamped to half of smaller dimension)
        const char* svg = R"(<svg width="200" height="200">
            <rect x="10" y="10" width="100" height="60" rx="30" ry="30" fill="green"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(hasBoxPrimitive(*buffer)) << "Rounded rect should be detected as Box";
        
        float cx, cy, hw, hh, round;
        if (getBoxData(*buffer, cx, cy, hw, hh, round)) {
            // Radius may be clamped to 30 (half of height 60)
            expect(round > 20.0f && round < 35.0f) 
                << "Expected round ~30, got " << round;
        }
    };

    "rounded_rect_small_radius"_test = [] {
        const char* svg = R"(<svg width="200" height="200">
            <rect x="10" y="10" width="100" height="80" rx="5" ry="5" fill="orange"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(hasBoxPrimitive(*buffer)) << "Rounded rect should be detected as Box";
        
        float cx, cy, hw, hh, round;
        if (getBoxData(*buffer, cx, cy, hw, hh, round)) {
            expect(round > 3.0f && round < 8.0f) 
                << "Expected round ~5, got " << round;
        }
    };

    "rounded_rect_with_stroke"_test = [] {
        const char* svg = R"(<svg width="200" height="200">
            <rect x="20" y="20" width="160" height="120" rx="10" ry="10" 
                  fill="yellow" stroke="black" stroke-width="2"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(hasBoxPrimitive(*buffer)) << "Rounded rect with stroke should be Box";
        
        float cx, cy, hw, hh, round;
        if (getBoxData(*buffer, cx, cy, hw, hh, round)) {
            expect(round > 7.0f && round < 13.0f) 
                << "Expected round ~10, got " << round;
        }
    };

};
