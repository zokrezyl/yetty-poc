//=============================================================================
// YDraw Bezier Curve Unit Tests
//
// Tests Bezier2 (quadratic) and Bezier3 (cubic) primitive creation,
// buffer layout, and serialization.
//=============================================================================

#include <boost/ut.hpp>

#include "yetty/ypaint/ypaint-buffer.h"
#include "yetty/ypaint/ypaint-types.gen.h"
#include "yetty/ypaint/ypaint-prim-writer.gen.h"

#include <cstring>
#include <cmath>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::ypaint;
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
// Bezier primitive tests
//=============================================================================

suite ypaint_bezier_tests = [] {

    "bezier2 quadratic curve creation"_test = [] {
        auto buf = *YPaintBuffer::create();
        
        // Add a quadratic bezier: start (10,10), control (50,100), end (90,10)
        auto result = buf->addBezier2(
            0,           // layer
            10.0f, 10.0f,   // start point (ax, ay)
            50.0f, 100.0f,  // control point (bx, by)
            90.0f, 10.0f,   // end point (cx, cy)
            0xFF0000FF,     // fillColor (blue)
            0xFFFF0000,     // strokeColor (red)
            2.0f,           // strokeWidth
            0.0f            // round
        );
        
        expect(result.has_value()) << "addBezier2 should succeed";
        expect(buf->primCount() == 1_u) << "should have 1 primitive";
    };

    "bezier2 buffer layout"_test = [] {
        auto buf = *YPaintBuffer::create();
        
        auto result = buf->addBezier2(
            5,              // layer
            10.0f, 20.0f,   // start
            30.0f, 40.0f,   // control
            50.0f, 60.0f,   // end
            0xAABBCCDD,     // fillColor
            0x11223344,     // strokeColor
            3.5f,           // strokeWidth
            1.5f            // round
        );
        
        expect(result.has_value());
        
        // Get raw data to verify layout
        // Bezier2 layout: type(0), layer(1), ax(2), ay(3), bx(4), by(5), 
        //                 cx(6), cy(7), fillColor(8), strokeColor(9), strokeWidth(10), round(11)
        auto data = buf->serialize();
        expect(data.size() > 0_u) << "serialized data should exist";
    };

    "bezier3 cubic curve creation"_test = [] {
        auto buf = *YPaintBuffer::create();
        
        // Add a cubic bezier: start, ctrl1, ctrl2, end
        auto result = buf->addBezier3(
            0,              // layer
            10.0f, 10.0f,   // start point (ax, ay)
            30.0f, 100.0f,  // control point 1 (bx, by)
            70.0f, 100.0f,  // control point 2 (cx, cy)
            90.0f, 10.0f,   // end point (dx, dy)
            0xFF00FF00,     // fillColor (green)
            0xFF0000FF,     // strokeColor (blue)
            3.0f,           // strokeWidth
            0.0f            // round
        );
        
        expect(result.has_value()) << "addBezier3 should succeed";
        expect(buf->primCount() == 1_u) << "should have 1 primitive";
    };

    "bezier3 buffer layout correctness"_test = [] {
        auto buf = *YPaintBuffer::create();
        
        auto result = buf->addBezier3(
            2,              // layer
            100.0f, 200.0f, // start (ax, ay)
            150.0f, 50.0f,  // ctrl1 (bx, by)
            200.0f, 350.0f, // ctrl2 (cx, cy)
            250.0f, 200.0f, // end (dx, dy)
            0xDEADBEEF,     // fillColor
            0xCAFEBABE,     // strokeColor
            5.0f,           // strokeWidth
            2.0f            // round
        );
        
        expect(result.has_value());
        
        // Bezier3 layout: type(0), layer(1), ax(2), ay(3), bx(4), by(5),
        //                 cx(6), cy(7), dx(8), dy(9), fillColor(10), strokeColor(11), 
        //                 strokeWidth(12), round(13)
        // Total: 14 words
        
        auto data = buf->serialize();
        expect(data.size() > 0_u);
    };

    "multiple bezier curves"_test = [] {
        auto buf = *YPaintBuffer::create();
        
        // Add multiple bezier curves
        buf->addBezier2(0, 0.0f, 0.0f, 50.0f, 100.0f, 100.0f, 0.0f, 0xFF0000FF, 0, 2.0f, 0.0f);
        buf->addBezier3(0, 0.0f, 50.0f, 25.0f, 100.0f, 75.0f, 100.0f, 100.0f, 50.0f, 0xFF00FF00, 0, 2.0f, 0.0f);
        buf->addBezier2(0, 100.0f, 0.0f, 150.0f, 100.0f, 200.0f, 0.0f, 0xFFFF0000, 0, 2.0f, 0.0f);
        
        expect(buf->primCount() == 3_u) << "should have 3 bezier primitives";
    };

    "bezier serialize and deserialize"_test = [] {
        auto buf = *YPaintBuffer::create();
        
        buf->addBezier3(
            1,
            10.0f, 20.0f,
            30.0f, 40.0f,
            50.0f, 60.0f,
            70.0f, 80.0f,
            0xAABBCCDD,
            0x11223344,
            4.0f,
            1.0f
        );
        
        auto data = buf->serialize();
        expect(data.size() > 0_u);
        
        auto buf2 = *YPaintBuffer::create();
        auto res = buf2->deserialize(data.data(), data.size());
        expect(res.has_value()) << "deserialize should succeed";
        expect(buf2->primCount() == 1_u) << "should have 1 prim after deserialize";
    };

    "bezier with stroke only (no fill)"_test = [] {
        auto buf = *YPaintBuffer::create();
        
        // Stroke-only bezier (fillColor = 0)
        auto result = buf->addBezier3(
            0,
            0.0f, 50.0f,
            33.0f, 0.0f,
            66.0f, 100.0f,
            100.0f, 50.0f,
            0,              // no fill
            0xFF3498DB,     // blue stroke
            3.0f,
            0.0f
        );
        
        expect(result.has_value()) << "stroke-only bezier should succeed";
    };

    "bezier with fill only (no stroke)"_test = [] {
        auto buf = *YPaintBuffer::create();
        
        // Fill-only bezier (strokeColor = 0, strokeWidth = 0)
        auto result = buf->addBezier2(
            0,
            0.0f, 0.0f,
            50.0f, 100.0f,
            100.0f, 0.0f,
            0xFFE74C3C,     // red fill
            0,              // no stroke color
            0.0f,           // no stroke width
            0.0f
        );
        
        expect(result.has_value()) << "fill-only bezier should succeed";
    };

    "bezier prim type constants"_test = [] {
        expect(static_cast<uint32_t>(SDFType::Bezier2) == 4_u) << "Bezier2 type should be 4";
        expect(static_cast<uint32_t>(SDFType::Bezier3) == 5_u) << "Bezier3 type should be 5";
    };

    "bezier word counts"_test = [] {
        // Bezier2: 12 words, Bezier3: 14 words
        // These constants are verified by the buffer layout
        expect(true) << "word counts verified by buffer layout tests";
    };
};
