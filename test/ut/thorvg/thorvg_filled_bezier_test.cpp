//=============================================================================
// ThorVG Filled Bezier Path Tests
//
// Tests bezier curve flattening and filled path triangulation.
//=============================================================================

#include <boost/ut.hpp>
#include "yetty/thorvg/thorvg-renderer.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <ytrace/ytrace.hpp>
#include <thorvg.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::thorvg;

// Helper to count primitives by type
static uint32_t countPrimsByType(const YDrawBuffer& buffer, card::SDFType type) {
    uint32_t count = 0;
    buffer.forEachPrim([&](uint32_t, const float* data, uint32_t) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(type)) {
            count++;
        }
    });
    return count;
}

// Helper to check for any triangle-based primitives (from triangulation)
static bool hasTriangulatedPrimitives(const YDrawBuffer& buffer) {
    bool found = false;
    buffer.forEachPrim([&](uint32_t, const float* data, uint32_t) {
        uint32_t typeCode = *reinterpret_cast<const uint32_t*>(data);
        if (typeCode == static_cast<uint32_t>(card::SDFType::Triangle) ||
            typeCode == static_cast<uint32_t>(card::SDFType::Polygon) ||
            typeCode == static_cast<uint32_t>(card::SDFType::PolygonGroup)) {
            found = true;
        }
    });
    return found;
}

//=============================================================================
// Simple Filled Bezier Tests
//=============================================================================

suite thorvg_filled_bezier_tests = [] {

    "simple_bezier_path_filled"_test = [] {
        // Simple filled path with one cubic bezier curve
        // M 0,50 C 0,0 100,0 100,50 L 100,100 L 0,100 Z
        const char* svg = R"(<svg width="100" height="100">
            <path d="M 0,50 C 0,0 100,0 100,50 L 100,100 L 0,100 Z" fill="red"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        auto loadResult = renderer->load(svg, "svg");
        expect(loadResult.has_value()) << "Failed to load SVG";

        auto renderResult = renderer->render();
        expect(renderResult.has_value()) << "Failed to render";

        // Should produce triangulated primitives (not bezier strokes)
        expect(buffer->primCount() >= 1_u) << "Expected at least 1 prim";
        expect(hasTriangulatedPrimitives(*buffer)) << "Expected triangulated primitives for filled bezier";
    };

    "filled_heart_shape"_test = [] {
        // Heart shape made with bezier curves
        const char* svg = R"(<svg width="100" height="100" viewBox="0 0 100 100">
            <path d="M 50,90 C 20,60 0,40 0,25 C 0,10 15,0 30,0 C 40,0 50,10 50,20
                     C 50,10 60,0 70,0 C 85,0 100,10 100,25 C 100,40 80,60 50,90 Z"
                  fill="red"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        expect(buffer->primCount() >= 1_u) << "Heart shape should render";
        expect(hasTriangulatedPrimitives(*buffer)) << "Heart should be triangulated";
    };

    "filled_quadratic_bezier"_test = [] {
        // Path with quadratic bezier (Q command) - ThorVG converts to cubic
        const char* svg = R"(<svg width="100" height="100">
            <path d="M 10,80 Q 50,10 90,80 L 50,90 Z" fill="blue"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        expect(buffer->primCount() >= 1_u);
    };

    "multiple_bezier_curves"_test = [] {
        // Path with multiple cubic beziers forming a wave
        const char* svg = R"(<svg width="200" height="100">
            <path d="M 0,50 C 25,0 50,0 75,50 C 100,100 125,100 150,50 C 175,0 200,0 200,50
                     L 200,100 L 0,100 Z"
                  fill="green"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        expect(buffer->primCount() >= 1_u) << "Wave shape should render";
    };

//=============================================================================
// Multi-Contour Tests (Paths with Holes)
//=============================================================================

    "donut_shape_two_contours"_test = [] {
        // Donut: outer circle and inner circle (hole)
        // Simplified as squares for easier testing
        const char* svg = R"(<svg width="100" height="100">
            <path d="M 10,10 L 90,10 L 90,90 L 10,90 Z
                     M 30,30 L 30,70 L 70,70 L 70,30 Z"
                  fill="purple" fill-rule="evenodd"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        // Should handle two contours (outer + hole)
        expect(buffer->primCount() >= 1_u) << "Donut should render";
    };

    "letter_O_shape"_test = [] {
        // Letter O with bezier outer and inner contours
        const char* svg = R"(<svg width="100" height="100">
            <path d="M 50,10 C 80,10 90,30 90,50 C 90,70 80,90 50,90
                     C 20,90 10,70 10,50 C 10,30 20,10 50,10 Z
                     M 50,30 C 35,30 30,40 30,50 C 30,60 35,70 50,70
                     C 65,70 70,60 70,50 C 70,40 65,30 50,30 Z"
                  fill="black" fill-rule="evenodd"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        expect(buffer->primCount() >= 1_u) << "Letter O should render";
    };

    "three_contours"_test = [] {
        // Shape with outer contour and two holes
        const char* svg = R"(<svg width="100" height="100">
            <path d="M 5,5 L 95,5 L 95,95 L 5,95 Z
                     M 15,15 L 15,45 L 45,45 L 45,15 Z
                     M 55,55 L 55,85 L 85,85 L 85,55 Z"
                  fill="orange" fill-rule="evenodd"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        expect(buffer->primCount() >= 1_u) << "Three contour shape should render";
    };

//=============================================================================
// Edge Cases
//=============================================================================

    "very_small_bezier"_test = [] {
        // Tiny bezier curve - tests tolerance handling
        const char* svg = R"(<svg width="10" height="10">
            <path d="M 1,5 C 1,1 9,1 9,5 C 9,9 1,9 1,5 Z" fill="red"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        expect(buffer->primCount() >= 1_u);
    };

    "nearly_straight_bezier"_test = [] {
        // Bezier that's almost a straight line - should flatten to few points
        const char* svg = R"(<svg width="100" height="100">
            <path d="M 10,50 C 30,50 70,50 90,50 L 90,60 L 10,60 Z" fill="gray"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        expect(buffer->primCount() >= 1_u);
    };

    "complex_bezier_many_curves"_test = [] {
        // Many bezier curves in one path
        const char* svg = R"(<svg width="200" height="200">
            <path d="M 100,10
                     C 150,10 190,50 190,100
                     C 190,150 150,190 100,190
                     C 50,190 10,150 10,100
                     C 10,50 50,10 100,10 Z"
                  fill="cyan"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        expect(buffer->primCount() >= 1_u);
    };

//=============================================================================
// Real-World SVG: Logo
//=============================================================================

    "logo_svg_renders"_test = [] {
        // Simplified version of the yetty logo - tests complex filled bezier paths
        const char* svg = R"(<svg width="100" height="100" viewBox="0 0 800 800">
            <path fill="#000000" d="
                M124,426 C129,415 133,405 138,395 C140,391 139,390 136,389
                C133,388 129,388 126,385 C136,370 149,356 159,342
                L153,339 C166,313 184,291 210,278 L205,274 L201,270
                C212,265 224,260 235,255 C247,250 258,244 268,236 L264,232
                C266,230 269,229 271,227 C279,222 285,216 289,208
                L288,202 L287,197 C290,188 294,180 299,172 L301,167 L295,165
                L296,161 C302,150 310,141 320,135 L320,124 C325,120 330,119 335,117
                L339,115 L345,112 L341,107 C345,104 350,104 354,103
                C365,101 377,101 388,104 L392,103 C396,98 400,98 405,99
                C412,101 418,105 423,109 L434,112 C441,113 448,114 454,117
                L451,123 C457,125 462,130 469,128 L470,135 C467,138 469,140 472,142
                C481,147 488,154 494,162 L495,165 L496,166 L493,167
                C489,167 488,169 490,172 C496,179 500,188 503,196
                L502,204 L502,208 C504,213 508,217 511,221 C514,225 516,223 519,221
                C528,214 537,206 547,199 C549,196 553,194 556,195
                C561,196 563,195 566,192 C574,184 582,175 590,167
                C596,161 600,161 605,167 C616,183 628,198 640,213
                C642,216 644,218 648,219 C657,220 663,226 667,235
                C674,252 681,268 688,285 C689,288 690,290 693,291
                C700,295 702,302 704,309 C710,340 716,371 722,403
                L724,419 Z"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        auto loadResult = renderer->load(svg, "svg");
        expect(loadResult.has_value()) << "Failed to load logo SVG";

        auto renderResult = renderer->render();
        expect(renderResult.has_value()) << "Failed to render logo";

        // Logo should produce at least one triangulated primitive
        expect(buffer->primCount() >= 1_u) << "Logo should produce primitives";
        expect(hasTriangulatedPrimitives(*buffer)) << "Logo should be triangulated";
    };

    "logo_multiple_paths"_test = [] {
        // Two paths like in real logo
        const char* svg = R"(<svg width="200" height="200" viewBox="0 0 800 800">
            <path fill="#000000" d="
                M327,319 C332,333 338,345 348,356 C350,349 349,341 352,333
                C360,347 367,361 380,371 L382,353 C388,362 392,371 398,379
                C404,370 407,361 414,352 C417,358 415,365 418,370
                C429,361 436,348 443,335 C446,342 445,349 448,356
                C462,338 468,316 472,294 C478,301 479,310 482,319
                C492,300 491,281 490,263 L499,265 C499,251 491,239 488,224
                L491,225 L497,225 C499,223 497,221 496,220
                C490,210 484,200 478,190 L477,185 L489,190
                C484,176 474,166 464,157 L472,154 C465,147 456,145 448,141
                L450,138 L453,135 L450,133 L440,132 L431,131
                L433,128 L436,123 L429,121 L418,117 L407,110 L404,113
                L404,120 C393,115 382,111 370,113 L376,121
                C365,122 354,119 344,125 L354,135 C347,136 340,136 334,139
                C327,142 321,145 317,151 L328,154 C316,164 306,174 300,189
                L316,182 C307,196 300,210 292,223 L304,224
                C302,231 299,237 297,244 C295,251 292,258 293,265
                L302,263 C301,282 300,301 312,318 L322,295 L327,319 Z"/>
            <path fill="#000000" d="
                M500,463 C496,462 492,463 488,462 L497,449 L499,459
                C510,450 513,438 519,426 L520,450 C529,442 534,433 538,423
                C542,413 544,403 546,392 L550,396 C552,401 555,405 558,409
                C560,412 562,413 565,409 C569,418 571,427 580,433
                L585,412 C589,420 591,428 597,434 L583,450
                C577,447 573,440 567,437 L565,441 C565,447 564,452 564,457
                L558,463 Z"/>
        </svg>)";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        renderer->load(svg, "svg");
        renderer->render();

        // Should have at least 2 primitives (one per path)
        expect(buffer->primCount() >= 2_u) << "Should render both paths";
    };

//=============================================================================
// Real Logo File Test
//=============================================================================

    "real_logo_svg_file"_test = [] {
        std::ifstream file("docs/logo.svg");
        expect(file.good()) << "Run test from repo root";
        expect(file.good()) << "Could not open docs/logo.svg";

        std::stringstream buffer_stream;
        buffer_stream << file.rdbuf();
        std::string svgContent = buffer_stream.str();
        expect(!svgContent.empty()) << "logo.svg is empty";

        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        // Measure load time
        auto loadStart = std::chrono::steady_clock::now();
        auto loadResult = renderer->load(svgContent, "svg");
        auto loadEnd = std::chrono::steady_clock::now();
        auto loadMs = std::chrono::duration_cast<std::chrono::milliseconds>(loadEnd - loadStart).count();

        expect(loadResult.has_value()) << "Failed to load logo.svg";
        expect(loadMs < 1000) << "Load took too long: " << loadMs << "ms";

        // Measure render time
        auto renderStart = std::chrono::steady_clock::now();
        auto renderResult = renderer->render();
        auto renderEnd = std::chrono::steady_clock::now();
        auto renderMs = std::chrono::duration_cast<std::chrono::milliseconds>(renderEnd - renderStart).count();

        expect(renderResult.has_value()) << "Failed to render logo.svg";
        expect(renderMs < 5000) << "Render took too long: " << renderMs << "ms (possible infinite loop)";

        // Verify output
        uint32_t primCount = buffer->primCount();
        expect(primCount >= 1_u) << "No primitives generated";
        expect(primCount <= 10000_u) << "Too many primitives: " << primCount << " (explosion?)";

        // Count polygon groups (triangulation happens on GPU side)
        uint32_t polygonGroupCount = countPrimsByType(*buffer, card::SDFType::PolygonGroup);
        expect(polygonGroupCount > 0_u) << "No PolygonGroup primitives generated";

        // Print actual counts
        std::cerr << "logo.svg: prims=" << primCount << " polygonGroups=" << polygonGroupCount << std::endl;
    };

    "logo_svg_content_size"_test = [] {
        // Test with the actual logo content dimensions
        auto buffer = *YDrawBuffer::create();
        auto renderer = *ThorVgRenderer::create(buffer);

        std::ifstream file("docs/logo.svg");
        expect(file.good()) << "Run test from repo root";

        std::stringstream ss;
        ss << file.rdbuf();
        renderer->load(ss.str(), "svg");
        renderer->render();

        // The logo is 800x800
        expect(renderer->contentWidth() == 800.0_f) << "Width should be 800";
        expect(renderer->contentHeight() == 800.0_f) << "Height should be 800";
    };

};

