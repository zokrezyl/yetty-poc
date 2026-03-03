//=============================================================================
// ThorVG Stroke Dash Pattern Tests
//
// Tests dashed stroke rendering from ThorVG to YDraw.
//=============================================================================

#include <boost/ut.hpp>
#include "yetty/thorvg/thorvg-renderer.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <thorvg.h>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::thorvg;

// Helper to count Segment primitives
static uint32_t countSegmentPrimitives(const YDrawBuffer& buffer) {
    uint32_t count = 0;
    buffer.forEachPrim([&](uint32_t, const float* data, uint32_t) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::Segment)) {
            count++;
        }
    });
    return count;
}

//=============================================================================
// Tests
//=============================================================================

suite thorvg_dash_tests = [] {

    "solid_stroke_single_segment"_test = [] {
        // A simple line without dash pattern should be one segment
        const char* svg = R"(<svg width="200" height="100">
            <line x1="10" y1="50" x2="190" y2="50" stroke="red" stroke-width="2"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Solid line should be exactly 1 segment
        uint32_t segCount = countSegmentPrimitives(*buffer);
        expect(segCount == 1_u) << "Solid line should be 1 segment, got " << segCount;
    };

    "dashed_stroke_multiple_segments"_test = [] {
        // A dashed line should become multiple segments
        const char* svg = R"(<svg width="200" height="100">
            <line x1="10" y1="50" x2="190" y2="50" 
                  stroke="blue" stroke-width="2" stroke-dasharray="10,5"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Dashed line (180px long, dash=10, gap=5) should create multiple segments
        // 180 / 15 = 12 dash-gap cycles, so approximately 12 dash segments
        uint32_t segCount = countSegmentPrimitives(*buffer);
        expect(segCount > 5_u) << "Dashed line should have multiple segments, got " << segCount;
    };

    "dashed_path_stroke"_test = [] {
        // Path with dashed stroke (won't be detected as Box)
        const char* svg = R"(<svg width="200" height="200">
            <path d="M20,20 L180,20 L180,140 L20,140 Z" 
                  fill="none" stroke="green" stroke-width="3" stroke-dasharray="15,10"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Dashed path perimeter (560px) with dash=15, gap=10 should create many segments
        uint32_t segCount = countSegmentPrimitives(*buffer);
        expect(segCount > 10_u) << "Dashed path should have many segments, got " << segCount;
    };

    "dotted_line"_test = [] {
        // Dotted pattern (very short dashes)
        const char* svg = R"(<svg width="200" height="100">
            <line x1="10" y1="50" x2="190" y2="50" 
                  stroke="purple" stroke-width="4" stroke-dasharray="2,8"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Dotted line (180px, dash=2, gap=8) should create ~18 tiny segments
        uint32_t segCount = countSegmentPrimitives(*buffer);
        expect(segCount > 10_u) << "Dotted line should have many small segments, got " << segCount;
    };

    "complex_dash_pattern"_test = [] {
        // Complex dash pattern with multiple values
        const char* svg = R"(<svg width="200" height="100">
            <line x1="10" y1="50" x2="190" y2="50" 
                  stroke="orange" stroke-width="2" stroke-dasharray="20,5,5,5"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Should have segments from the complex pattern
        uint32_t segCount = countSegmentPrimitives(*buffer);
        expect(segCount > 1_u) << "Complex dash should have multiple segments, got " << segCount;
    };

};
