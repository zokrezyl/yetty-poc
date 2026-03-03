//=============================================================================
// ThorVG Circle/Ellipse Unit Tests
//
// Tests that SVG circles are properly rendered to YDraw Ellipse primitives
// with correct position, size, and colors.
//=============================================================================

#include <boost/ut.hpp>
#include "yetty/thorvg/thorvg-renderer.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <thorvg.h>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::thorvg;

// Ellipse field offsets (from ydraw-types.gen.h)
namespace EllipseField {
    constexpr uint32_t kCx = 2;
    constexpr uint32_t kCy = 3;
    constexpr uint32_t kRx = 4;
    constexpr uint32_t kRy = 5;
    constexpr uint32_t kFillColor = 6;
    constexpr uint32_t kStrokeColor = 7;
    constexpr uint32_t kStrokeWidth = 8;
}

// Helper to extract ellipse primitive data
struct EllipseData {
    float cx, cy, rx, ry;
    uint32_t fillColor;
    uint32_t strokeColor;
    float strokeWidth;
};

static std::vector<EllipseData> getEllipsePrimitives(const YDrawBuffer& buffer) {
    std::vector<EllipseData> ellipses;
    buffer.forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t /*wordCount*/) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::Ellipse)) {
            EllipseData e;
            e.cx = data[EllipseField::kCx];
            e.cy = data[EllipseField::kCy];
            e.rx = data[EllipseField::kRx];
            e.ry = data[EllipseField::kRy];
            e.fillColor = *reinterpret_cast<const uint32_t*>(&data[EllipseField::kFillColor]);
            e.strokeColor = *reinterpret_cast<const uint32_t*>(&data[EllipseField::kStrokeColor]);
            e.strokeWidth = data[EllipseField::kStrokeWidth];
            ellipses.push_back(e);
        }
    });
    return ellipses;
}

static bool hasEllipsePrimitive(const YDrawBuffer& buffer) {
    return !getEllipsePrimitives(buffer).empty();
}

// Helper to extract alpha from packed ABGR color
static uint8_t getAlpha(uint32_t color) {
    return (color >> 24) & 0xFF;
}

//=============================================================================
// Tests
//=============================================================================

suite thorvg_circle_tests = [] {

    "circle_renders_as_ellipse"_test = [] {
        const char* svg = R"(<svg width="100" height="100">
            <circle cx="50" cy="50" r="25" fill="red"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        
        auto loadResult = renderer->load(svg, "svg");
        expect(loadResult.has_value()) << "Load should succeed";
        
        auto renderResult = renderer->render();
        expect(renderResult.has_value()) << "Render should succeed";
        
        expect(buffer->primCount() >= 1_u) << "Should have at least 1 primitive";
        expect(hasEllipsePrimitive(*buffer)) << "Circle should render as Ellipse primitive";
    };

    "circle_position_correct"_test = [] {
        const char* svg = R"(<svg width="200" height="200">
            <circle cx="100" cy="75" r="30" fill="blue"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        auto ellipses = getEllipsePrimitives(*buffer);
        expect(ellipses.size() == 1_u) << "Should have exactly 1 ellipse";
        
        if (!ellipses.empty()) {
            const auto& e = ellipses[0];
            expect(std::abs(e.cx - 100.0f) < 1.0f) << "cx should be ~100, got " << e.cx;
            expect(std::abs(e.cy - 75.0f) < 1.0f) << "cy should be ~75, got " << e.cy;
            expect(std::abs(e.rx - 30.0f) < 1.0f) << "rx should be ~30, got " << e.rx;
            expect(std::abs(e.ry - 30.0f) < 1.0f) << "ry should be ~30, got " << e.ry;
        }
    };

    "circle_fill_color_correct"_test = [] {
        // Red fill: RGB(255,0,0) -> ABGR: 0xFF0000FF
        const char* svg = R"(<svg width="100" height="100">
            <circle cx="50" cy="50" r="20" fill="red"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        auto ellipses = getEllipsePrimitives(*buffer);
        expect(ellipses.size() == 1_u) << "Should have 1 ellipse";
        
        if (!ellipses.empty()) {
            uint32_t fill = ellipses[0].fillColor;
            expect(fill != 0u) << "Fill color should not be zero";
            expect(getAlpha(fill) == 255u) << "Alpha should be 255 (fully opaque)";
            // Check red component (lowest byte in ABGR)
            expect((fill & 0xFF) == 255u) << "Red component should be 255";
        }
    };

    "circle_with_opacity"_test = [] {
        // 50% opacity -> alpha = 127 or 128
        const char* svg = R"(<svg width="100" height="100">
            <circle cx="50" cy="50" r="20" fill="blue" opacity="0.5"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        auto ellipses = getEllipsePrimitives(*buffer);
        expect(ellipses.size() == 1_u) << "Should have 1 ellipse";
        
        if (!ellipses.empty()) {
            uint8_t alpha = getAlpha(ellipses[0].fillColor);
            // 0.5 * 255 = 127.5, could round to 127 or 128
            expect(alpha >= 125u && alpha <= 130u) << "Alpha should be ~128, got " << static_cast<int>(alpha);
        }
    };

    "circle_with_stroke"_test = [] {
        const char* svg = R"(<svg width="100" height="100">
            <circle cx="50" cy="50" r="30" fill="green" stroke="black" stroke-width="3"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        auto ellipses = getEllipsePrimitives(*buffer);
        expect(ellipses.size() == 1_u) << "Should have 1 ellipse";
        
        if (!ellipses.empty()) {
            const auto& e = ellipses[0];
            expect(e.strokeColor != 0u) << "Stroke color should not be zero";
            expect(std::abs(e.strokeWidth - 3.0f) < 0.1f) << "Stroke width should be ~3";
        }
    };

    "multiple_circles"_test = [] {
        const char* svg = R"(<svg width="200" height="100">
            <circle cx="50" cy="50" r="20" fill="red"/>
            <circle cx="100" cy="50" r="20" fill="green"/>
            <circle cx="150" cy="50" r="20" fill="blue"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        auto ellipses = getEllipsePrimitives(*buffer);
        expect(ellipses.size() == 3_u) << "Should have 3 ellipses, got " << ellipses.size();
    };

    "ellipse_renders_correctly"_test = [] {
        const char* svg = R"(<svg width="200" height="100">
            <ellipse cx="100" cy="50" rx="80" ry="30" fill="purple"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        auto ellipses = getEllipsePrimitives(*buffer);
        expect(ellipses.size() == 1_u) << "Should have 1 ellipse";
        
        if (!ellipses.empty()) {
            const auto& e = ellipses[0];
            expect(std::abs(e.rx - 80.0f) < 1.0f) << "rx should be ~80";
            expect(std::abs(e.ry - 30.0f) < 1.0f) << "ry should be ~30";
        }
    };

    "circle_near_edge_of_viewport"_test = [] {
        // Circle partially outside viewport - should still render
        const char* svg = R"(<svg width="100" height="100">
            <circle cx="90" cy="90" r="30" fill="cyan"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        auto ellipses = getEllipsePrimitives(*buffer);
        expect(ellipses.size() == 1_u) << "Circle near edge should still render";
        
        if (!ellipses.empty()) {
            expect(std::abs(ellipses[0].cx - 90.0f) < 1.0f) << "cx should be 90";
            expect(std::abs(ellipses[0].cy - 90.0f) < 1.0f) << "cy should be 90";
        }
    };

    "circle_60_percent_opacity"_test = [] {
        // Test specific opacity value from opacity.svg
        const char* svg = R"(<svg width="100" height="100">
            <circle cx="50" cy="50" r="40" fill="red" opacity="0.6"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        auto ellipses = getEllipsePrimitives(*buffer);
        expect(ellipses.size() == 1_u) << "Should have 1 ellipse";
        
        if (!ellipses.empty()) {
            uint8_t alpha = getAlpha(ellipses[0].fillColor);
            // 0.6 * 255 = 153
            expect(alpha >= 150u && alpha <= 156u) << "Alpha should be ~153, got " << static_cast<int>(alpha);
        }
    };
};
