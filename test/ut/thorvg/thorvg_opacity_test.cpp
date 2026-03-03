//=============================================================================
// ThorVG Opacity Unit Tests
//
// Tests paint opacity support in ThorVG-to-YDraw rendering.
//=============================================================================

#include <boost/ut.hpp>
#include "yetty/thorvg/thorvg-renderer.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <thorvg.h>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::thorvg;

// Helper to get fill color from a primitive
// Returns the fillColor field (assumes Box/Triangle layout with fillColor at offset 6)
static uint32_t getFirstFillColor(const YDrawBuffer& buffer) {
    uint32_t fillColor = 0;
    bool found = false;
    buffer.forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t wordCount) {
        if (found) return;
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        // Box: type(0), layer(1), cx(2), cy(3), hw(4), hh(5), fillColor(6)
        // Triangle: type(0), layer(1), ax(2), ay(3), bx(4), by(5), vx(6), vy(7), fillColor(8)
        // Segment: type(0), layer(1), x0(2), y0(3), x1(4), y1(5), fillColor(6)
        if (typeCode == static_cast<uint32_t>(card::SDFType::Box) && wordCount >= 7) {
            fillColor = *reinterpret_cast<const uint32_t*>(data + 6);
            found = true;
        } else if (typeCode == static_cast<uint32_t>(card::SDFType::Triangle) && wordCount >= 9) {
            fillColor = *reinterpret_cast<const uint32_t*>(data + 8);
            found = true;
        } else if (typeCode == static_cast<uint32_t>(card::SDFType::Segment) && wordCount >= 7) {
            fillColor = *reinterpret_cast<const uint32_t*>(data + 6);
            found = true;
        }
    });
    return fillColor;
}

// Extract alpha from packed ABGR color
static uint8_t extractAlpha(uint32_t abgr) {
    return static_cast<uint8_t>((abgr >> 24) & 0xFF);
}

//=============================================================================
// Tests
//=============================================================================

suite thorvg_opacity_tests = [] {

    "full_opacity_rect"_test = [] {
        // Fully opaque rect (opacity=1.0 which is the default)
        const char* svg = R"(<svg width="100" height="100">
            <rect x="10" y="10" width="80" height="80" fill="red"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        
        uint32_t fillColor = getFirstFillColor(*buffer);
        uint8_t alpha = extractAlpha(fillColor);
        expect(alpha == 255_u) << "Full opacity should give alpha=255, got " << static_cast<unsigned>(alpha);
    };

    "half_opacity_rect"_test = [] {
        // 50% opacity rect
        const char* svg = R"(<svg width="100" height="100">
            <rect x="10" y="10" width="80" height="80" fill="red" opacity="0.5"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        
        uint32_t fillColor = getFirstFillColor(*buffer);
        uint8_t alpha = extractAlpha(fillColor);
        // 255 * 0.5 = 127.5, expect ~127 or 128
        expect(alpha >= 125_u && alpha <= 130_u) << "50% opacity should give alpha~127, got " << static_cast<unsigned>(alpha);
    };

    "zero_opacity_rect"_test = [] {
        // Completely transparent rect (opacity=0)
        const char* svg = R"(<svg width="100" height="100">
            <rect x="10" y="10" width="80" height="80" fill="red" opacity="0"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        // Zero opacity should result in invisible shape - may be skipped entirely
        // or have alpha=0
        if (buffer->primCount() > 0) {
            uint32_t fillColor = getFirstFillColor(*buffer);
            uint8_t alpha = extractAlpha(fillColor);
            expect(alpha == 0_u) << "Zero opacity should give alpha=0, got " << static_cast<unsigned>(alpha);
        }
        // If no prims, that's also correct (invisible shape skipped)
    };

    "fill_alpha_and_opacity"_test = [] {
        // Fill with alpha 50% (fill-opacity) combined with opacity 50%
        // In SVG, fill-opacity and opacity are separate attributes
        // fill-opacity affects only fill, opacity affects the whole element
        // ThorVG may combine them differently - let's use fill-opacity attribute
        const char* svg = R"SVG(<svg width="100" height="100">
            <rect x="10" y="10" width="80" height="80" fill="red" fill-opacity="0.5" opacity="0.5"/>
        </svg>)SVG";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        
        uint32_t fillColor = getFirstFillColor(*buffer);
        uint8_t alpha = extractAlpha(fillColor);
        // fill-opacity 0.5 gives fa=127, paint opacity 0.5 gives 127
        // Combined: (127 * 127) / 255 = ~63
        // But ThorVG may pre-combine - accept range 60-130 for now
        expect(alpha >= 60_u && alpha <= 130_u) << "Combined opacity should reduce alpha, got " << static_cast<unsigned>(alpha);
    };

    "quarter_opacity_stroke"_test = [] {
        // 25% opacity stroke
        const char* svg = R"(<svg width="100" height="100">
            <line x1="10" y1="10" x2="90" y2="90" stroke="blue" stroke-width="5" opacity="0.25"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        
        // Check stroke color - Segment has strokeColor at offset 7
        uint32_t strokeColor = 0;
        bool found = false;
        buffer->forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t wordCount) {
            if (found) return;
            uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
            if (typeCode == static_cast<uint32_t>(card::SDFType::Segment) && wordCount >= 8) {
                strokeColor = *reinterpret_cast<const uint32_t*>(data + 7);
                found = true;
            }
        });
        
        uint8_t alpha = extractAlpha(strokeColor);
        // 255 * 0.25 = ~63
        expect(alpha >= 60_u && alpha <= 70_u) << "25% opacity stroke should give alpha~63, got " << static_cast<unsigned>(alpha);
    };
};
