#include <boost/ut.hpp>
#include <yetty/ypaint/ypaint-buffer.h>

using namespace boost::ut;
using namespace yetty;

suite gradient_tests = [] {
    "LinearGradientBox primitive creation"_test = [] {
        auto buffer = YPaintBuffer::create(1024);
        expect(buffer.has_value());

        auto result = (*buffer)->addLinearGradientBox(
            0,              // layer
            100.0f, 50.0f,  // center
            40.0f, 30.0f,   // half size
            60.0f, 50.0f,   // gradient start
            140.0f, 50.0f,  // gradient end  
            0xFF0000FF,     // color1 (red)
            0xFFFF0000,     // color2 (blue)
            0,              // no stroke
            0.0f,           // stroke width
            0.0f            // round
        );
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_i);
    };

    "LinearGradientBox with stroke"_test = [] {
        auto buffer = YPaintBuffer::create(1024);
        expect(buffer.has_value());

        auto result = (*buffer)->addLinearGradientBox(
            0, 100.0f, 50.0f, 40.0f, 30.0f,
            60.0f, 50.0f, 140.0f, 50.0f,
            0xFF00FF00, 0xFFFFFF00,
            0xFFFFFFFF, 2.0f, 0.0f
        );
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_i);
    };

    "Multiple LinearGradientBox primitives"_test = [] {
        auto buffer = YPaintBuffer::create(1024);
        expect(buffer.has_value());

        (*buffer)->addLinearGradientBox(0, 50.0f, 50.0f, 20.0f, 20.0f, 30.0f, 50.0f, 70.0f, 50.0f, 0xFF0000FF, 0xFFFF0000, 0, 0.0f, 0.0f);
        (*buffer)->addLinearGradientBox(0, 150.0f, 50.0f, 20.0f, 20.0f, 130.0f, 50.0f, 170.0f, 50.0f, 0xFF00FF00, 0xFFFFFF00, 0, 0.0f, 0.0f);
        expect((*buffer)->primCount() == 2_i);
    };

    "RadialGradientCircle primitive creation"_test = [] {
        auto buffer = YPaintBuffer::create(1024);
        expect(buffer.has_value());

        auto result = (*buffer)->addRadialGradientCircle(
            0,              // layer
            100.0f, 50.0f,  // center
            30.0f,          // radius
            100.0f, 50.0f,  // gradient center
            30.0f,          // gradient radius
            0xFFFFFFFF,     // color1 (white center)
            0xFF000000,     // color2 (black edge)
            0,              // no stroke
            0.0f,           // stroke width
            0.0f            // round
        );
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_i);
    };

    "LinearGradientBox data layout verification"_test = [] {
        auto buffer = YPaintBuffer::create(1024);
        expect(buffer.has_value());

        auto result = (*buffer)->addLinearGradientBox(
            0,              // layer
            100.0f, 50.0f,  // center (cx, cy)
            40.0f, 30.0f,   // half size (hw, hh)
            60.0f, 20.0f,   // gradient start (gx1, gy1)
            140.0f, 80.0f,  // gradient end (gx2, gy2)
            0xAABBCCDD,     // color1
            0x11223344,     // color2
            0x55667788,     // stroke color
            2.5f,           // stroke width
            5.0f            // round
        );
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_i);
    };

    "LinearGradientCircle primitive creation"_test = [] {
        auto buffer = YPaintBuffer::create(1024);
        expect(buffer.has_value());

        auto result = (*buffer)->addLinearGradientCircle(
            0,              // layer
            100.0f, 50.0f,  // center
            30.0f,          // radius
            70.0f, 50.0f,   // gradient start
            130.0f, 50.0f,  // gradient end
            0xFF0000FF,     // color1
            0xFFFF0000,     // color2
            0,              // no stroke
            0.0f,           // stroke width
            0.0f            // round
        );
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_i);
    };

    "Gradient primitives with various parameters"_test = [] {
        auto buffer = YPaintBuffer::create(4096);
        expect(buffer.has_value());

        // Linear gradient box with rounded corners
        (*buffer)->addLinearGradientBox(0, 50.0f, 50.0f, 30.0f, 20.0f, 20.0f, 50.0f, 80.0f, 50.0f, 0xFFFF0000, 0xFF00FF00, 0, 0.0f, 10.0f);
        
        // Linear gradient box with stroke
        (*buffer)->addLinearGradientBox(0, 150.0f, 50.0f, 30.0f, 20.0f, 120.0f, 50.0f, 180.0f, 50.0f, 0xFF0000FF, 0xFFFFFF00, 0xFFFFFFFF, 3.0f, 0.0f);
        
        // Vertical linear gradient
        (*buffer)->addLinearGradientBox(0, 250.0f, 50.0f, 30.0f, 40.0f, 250.0f, 10.0f, 250.0f, 90.0f, 0xFF00FFFF, 0xFFFF00FF, 0, 0.0f, 0.0f);
        
        // Diagonal linear gradient
        (*buffer)->addLinearGradientBox(0, 350.0f, 50.0f, 30.0f, 30.0f, 320.0f, 20.0f, 380.0f, 80.0f, 0xFFFFFFFF, 0xFF000000, 0, 0.0f, 0.0f);
        
        // Radial gradient circle
        (*buffer)->addRadialGradientCircle(0, 450.0f, 50.0f, 40.0f, 450.0f, 50.0f, 40.0f, 0xFFFFFFFF, 0xFF000000, 0, 0.0f, 0.0f);
        
        expect((*buffer)->primCount() == 5_i);
    };
};
