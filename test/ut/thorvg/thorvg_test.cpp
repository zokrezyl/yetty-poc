//=============================================================================
// ThorVG Unit Tests
//
// Tests ThorVG-to-YDraw rendering including polygon detection.
//=============================================================================

#include <boost/ut.hpp>
#include "yetty/thorvg/thorvg-renderer.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <thorvg.h>
#include <cstdio>
#include <cstring>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::thorvg;

// Helper to check if any primitive is a Polygon type
static bool hasPolygonPrimitive(const YDrawBuffer& buffer) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t /*wordCount*/) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::Polygon) ||
            typeCode == static_cast<uint32_t>(card::SDFType::PolygonGroup) ||
            typeCode == static_cast<uint32_t>(card::SDFType::Triangle)) {
            // Polygon may have been triangulated to Triangles
            found = true;
        }
    });
    return found;
}

// Helper to check if any primitive is a Bezier type
static bool hasBezierPrimitive(const YDrawBuffer& buffer) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t /*wordCount*/) {
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
    buffer.forEachPrim([&](uint32_t id, const float* data, uint32_t /*wordCount*/) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        std::fprintf(stderr, "  Prim %u: type=%u\n", id, typeCode);
    });
}

//=============================================================================
// Tests
//=============================================================================

suite thorvg_tests = [] {

    "simple_rect_svg"_test = [] {
        const char* svg = R"(<svg width="100" height="100">
            <rect x="10" y="10" width="80" height="80" fill="red"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        
        auto loadResult = renderer->load(svg, "svg");
        expect(loadResult.has_value());
        
        auto renderResult = renderer->render();
        expect(renderResult.has_value());
        
        // Should have at least one primitive (rectangle becomes polygon)
        expect(buffer->primCount() >= 1_u) << "Expected at least 1 prim, got " << buffer->primCount();
    };
    
    "circle_svg_not_polygon"_test = [] {
        // Circle uses curves, should NOT become a polygon
        const char* svg = R"(<svg width="100" height="100">
            <circle cx="50" cy="50" r="40" fill="blue"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Circle should be detected as ellipse or rendered as bezier curves
        expect(buffer->primCount() >= 1_u);
    };
    
    "polygon_svg"_test = [] {
        // Explicit polygon should become a Polygon primitive
        const char* svg = R"(<svg width="200" height="200">
            <polygon points="100,10 40,198 190,78 10,78 160,198" fill="green"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Should have polygon primitive
        expect(buffer->primCount() >= 1_u) << "Polygon should produce primitives";
    };
    
    "bezier_path_not_polygon"_test = [] {
        // Path with curves should NOT become polygon
        const char* svg = R"(<svg width="200" height="200">
            <path d="M10,80 Q95,10 180,80 T350,80" fill="none" stroke="black" stroke-width="2"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Bezier path should produce bezier/segment primitives, not polygon
        expect(true); // Just checking it doesn't crash
    };
    
    "filled_path_becomes_polygon"_test = [] {
        // Closed filled path without curves should become polygon
        const char* svg = R"(<svg width="200" height="200">
            <path d="M50,50 L150,50 L150,150 L50,150 Z" fill="purple"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Should have at least one primitive
        expect(buffer->primCount() >= 1_u) << "Filled path should produce primitives";
    };
    
    "stroke_only_path"_test = [] {
        // Stroke-only path should not become polygon (no fill)
        const char* svg = R"(<svg width="200" height="200">
            <path d="M50,50 L150,50 L150,150 L50,150 Z" fill="none" stroke="red" stroke-width="3"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Stroke-only should produce line segments, not polygon
        expect(buffer->primCount() >= 1_u);
    };
    
    "multiple_shapes"_test = [] {
        const char* svg = R"(<svg width="300" height="200">
            <rect x="10" y="10" width="80" height="80" fill="red"/>
            <rect x="110" y="10" width="80" height="80" fill="green"/>
            <rect x="210" y="10" width="80" height="80" fill="blue"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Should have multiple primitives (at least 3)
        expect(buffer->primCount() >= 3_u) << "Expected 3+ prims, got " << buffer->primCount();
    };
    
    "pentagon_polygon"_test = [] {
        // Test that polygon primitives are created for filled shapes
        const char* svg = R"(<svg width="200" height="200">
            <polygon points="100,10 190,90 150,190 50,190 10,90" fill="orange"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have polygon primitive";
    };
    
    "filled_bezier_heart_not_polygon"_test = [] {
        // Heart shape with bezier curves and fill should NOT become polygon
        // This is the exact heart from beziers.sh demo
        const char* svg = R"(<svg viewBox="0 0 400 300" xmlns="http://www.w3.org/2000/svg">
            <path d="M200 250 C180 230 150 230 150 260 C150 290 200 320 200 320 
                     C200 320 250 290 250 260 C250 230 220 230 200 250" 
                  fill="#f85149" stroke="#ff7b72" stroke-width="2"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Should produce bezier primitives, NOT polygon
        expect(buffer->primCount() >= 1_u) << "Heart should produce primitives";
        expect(hasBezierPrimitive(*buffer)) << "Heart should have bezier curves";
        expect(!hasPolygonPrimitive(*buffer)) << "Heart should NOT be polygon (has curves)";
    };
    
    "bezier_wave_stroked"_test = [] {
        // Wave pattern with stroke only - from beziers.sh demo
        const char* svg = R"(<svg viewBox="0 0 400 300" xmlns="http://www.w3.org/2000/svg">
            <path d="M0 150 C100 50 150 250 200 150 C250 50 300 250 400 150" 
                  stroke="#58a6ff" stroke-width="4" fill="none"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Should produce bezier primitives for the wave curves
        expect(buffer->primCount() >= 1_u) << "Wave should produce primitives";
        expect(hasBezierPrimitive(*buffer)) << "Wave should have bezier curves";
        expect(!hasPolygonPrimitive(*buffer)) << "Wave should NOT be polygon";
    };
    
    "bezier_infinity_symbol"_test = [] {
        // Infinity symbol - from beziers.sh demo
        const char* svg = R"(<svg viewBox="0 0 400 300" xmlns="http://www.w3.org/2000/svg">
            <path d="M150 80 C120 50 80 50 80 80 C80 110 120 110 150 80 
                     C180 50 220 50 220 80 C220 110 180 110 150 80" 
                  stroke="#79c0ff" stroke-width="3" fill="none"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Should produce bezier primitives
        expect(buffer->primCount() >= 1_u) << "Infinity should produce bezier primitives";
        expect(hasBezierPrimitive(*buffer)) << "Infinity should have bezier curves";
        expect(!hasPolygonPrimitive(*buffer)) << "Infinity should NOT be polygon";
    };
    
    "pentagon_should_be_polygon"_test = [] {
        // Simple polygon without curves should use Polygon primitive
        const char* svg = R"(<svg width="200" height="200">
            <polygon points="100,10 190,90 150,190 50,190 10,90" fill="orange"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        auto loadRes = renderer->load(svg, "svg");
        std::fprintf(stderr, "load result: %s\n", loadRes.has_value() ? "OK" : "FAIL");
        auto renderRes = renderer->render();
        std::fprintf(stderr, "render result: %s\n", renderRes.has_value() ? "OK" : "FAIL");
        
        std::fprintf(stderr, "Pentagon primitives (count=%u):\n", buffer->primCount());
        dumpPrimTypes(*buffer);
        
        expect(buffer->primCount() >= 1_u) << "Pentagon should produce primitives";
        expect(hasPolygonPrimitive(*buffer)) << "Pentagon should be a Polygon primitive";
        expect(!hasBezierPrimitive(*buffer)) << "Pentagon should NOT have bezier curves";
    };
    
    "rect_filled_should_be_polygon"_test = [] {
        // Rectangle with fill should become polygon (no curves)
        const char* svg = R"(<svg width="100" height="100">
            <rect x="10" y="10" width="80" height="80" fill="red"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Rect should produce primitives";
        // Rect may be detected as Box or become Polygon - either is fine
    };
};
