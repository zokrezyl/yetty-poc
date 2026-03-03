//=============================================================================
// ThorVG Gradient Unit Tests
//
// Tests gradient fill support in ThorVG-to-YDraw rendering.
//=============================================================================

#include <boost/ut.hpp>
#include "yetty/thorvg/thorvg-renderer.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <thorvg.h>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::thorvg;

// Helper to check if any primitive is a LinearGradientBox
static bool hasLinearGradientBox(const YDrawBuffer& buffer) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t /*wordCount*/) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::LinearGradientBox)) {
            found = true;
        }
    });
    return found;
}

// Helper to check if any primitive is a RadialGradientCircle
static bool hasRadialGradientCircle(const YDrawBuffer& buffer) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t /*wordCount*/) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::RadialGradientCircle)) {
            found = true;
        }
    });
    return found;
}

// Get gradient colors from LinearGradientBox primitive
static std::pair<uint32_t, uint32_t> getLinearGradientColors(const YDrawBuffer& buffer) {
    uint32_t color1 = 0, color2 = 0;
    buffer.forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t wordCount) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::LinearGradientBox) && wordCount >= 12) {
            // LinearGradientBox: color1 at offset 10, color2 at offset 11
            color1 = *reinterpret_cast<const uint32_t*>(data + 10);
            color2 = *reinterpret_cast<const uint32_t*>(data + 11);
        }
    });
    return {color1, color2};
}

//=============================================================================
// Tests
//=============================================================================

suite thorvg_gradient_tests = [] {

    "linear_gradient_rect"_test = [] {
        // Rectangle with linear gradient fill (left to right, red to blue)
        const char* svg = R"SVG(<svg width="200" height="100">
            <defs>
                <linearGradient id="grad1" x1="0%" y1="0%" x2="100%" y2="0%">
                    <stop offset="0%" style="stop-color:rgb(255,0,0);stop-opacity:1" />
                    <stop offset="100%" style="stop-color:rgb(0,0,255);stop-opacity:1" />
                </linearGradient>
            </defs>
            <rect x="10" y="10" width="180" height="80" fill="url(#grad1)"/>
        </svg>)SVG";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(hasLinearGradientBox(*buffer)) << "Should have LinearGradientBox primitive";
        
        auto [c1, c2] = getLinearGradientColors(*buffer);
        expect(c1 != 0_u) << "color1 should be set";
        expect(c2 != 0_u) << "color2 should be set";
        expect(c1 != c2) << "gradient colors should be different";
    };

    "linear_gradient_vertical"_test = [] {
        // Rectangle with vertical linear gradient (top to bottom)
        const char* svg = R"SVG(<svg width="100" height="200">
            <defs>
                <linearGradient id="grad2" x1="0%" y1="0%" x2="0%" y2="100%">
                    <stop offset="0%" style="stop-color:green;stop-opacity:1" />
                    <stop offset="100%" style="stop-color:yellow;stop-opacity:1" />
                </linearGradient>
            </defs>
            <rect x="10" y="10" width="80" height="180" fill="url(#grad2)"/>
        </svg>)SVG";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(hasLinearGradientBox(*buffer)) << "Should have LinearGradientBox primitive";
    };

    "radial_gradient_circle"_test = [] {
        // Circle with radial gradient fill
        const char* svg = R"SVG(<svg width="200" height="200">
            <defs>
                <radialGradient id="radGrad" cx="50%" cy="50%" r="50%">
                    <stop offset="0%" style="stop-color:white;stop-opacity:1" />
                    <stop offset="100%" style="stop-color:black;stop-opacity:1" />
                </radialGradient>
            </defs>
            <rect x="10" y="10" width="180" height="180" fill="url(#radGrad)"/>
        </svg>)SVG";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        // Note: Radial gradient on rect falls back to RadialGradientCircle
        expect(hasRadialGradientCircle(*buffer)) << "Should have RadialGradientCircle primitive";
    };

    "solid_fill_no_gradient"_test = [] {
        // Rectangle with solid fill (no gradient)
        const char* svg = R"(<svg width="100" height="100">
            <rect x="10" y="10" width="80" height="80" fill="red"/>
        </svg>)";
        
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);
        renderer->load(svg, "svg");
        renderer->render();
        
        expect(buffer->primCount() >= 1_u) << "Should have primitives";
        expect(!hasLinearGradientBox(*buffer)) << "Solid fill should NOT use LinearGradientBox";
        expect(!hasRadialGradientCircle(*buffer)) << "Solid fill should NOT use RadialGradientCircle";
    };
};
