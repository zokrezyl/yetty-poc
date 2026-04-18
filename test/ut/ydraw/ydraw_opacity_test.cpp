#include <boost/ut.hpp>
#include <yetty/ydraw/ydraw-buffer.h>

using namespace boost::ut;
using namespace yetty;

suite opacity_tests = [] {
    "Circle with 50% alpha"_test = [] {
        auto buffer = YDrawBuffer::create(1024);
        expect(buffer.has_value());
        
        // ABGR with 50% alpha (128/255): 0x80BBGGRR
        uint32_t fillColor = 0x80FF0000;  // blue at 50% alpha
        auto result = (*buffer)->addCircle(0, 100.0f, 100.0f, 50.0f, fillColor, 0, 0.0f, 0.0f);
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_u);
    };

    "Box with 75% alpha"_test = [] {
        auto buffer = YDrawBuffer::create(1024);
        expect(buffer.has_value());
        
        // 75% alpha (192/255)
        uint32_t fillColor = 0xC000FF00;  // green at 75% alpha
        auto result = (*buffer)->addBox(0, 50.0f, 50.0f, 30.0f, 20.0f, fillColor, 0, 0.0f, 0.0f);
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_u);
    };

    "Fully opaque color"_test = [] {
        auto buffer = YDrawBuffer::create(1024);
        expect(buffer.has_value());
        
        // Full opacity (255/255)
        uint32_t fillColor = 0xFF0000FF;  // red at 100% alpha
        auto result = (*buffer)->addCircle(0, 0.0f, 0.0f, 10.0f, fillColor, 0, 0.0f, 0.0f);
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_u);
    };

    "Zero alpha primitive"_test = [] {
        auto buffer = YDrawBuffer::create(1024);
        expect(buffer.has_value());
        
        // Zero alpha (invisible)
        uint32_t fillColor = 0x00FF00FF;  // invisible
        auto result = (*buffer)->addCircle(0, 0.0f, 0.0f, 10.0f, fillColor, 0, 0.0f, 0.0f);
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_u);
    };

    "Stroke with alpha"_test = [] {
        auto buffer = YDrawBuffer::create(1024);
        expect(buffer.has_value());
        
        uint32_t fillColor = 0xFFFF0000;   // blue, fully opaque
        uint32_t strokeColor = 0x8000FF00; // green, 50% alpha
        float strokeWidth = 2.0f;
        
        auto result = (*buffer)->addCircle(0, 100.0f, 100.0f, 50.0f, fillColor, strokeColor, strokeWidth, 0.0f);
        expect(result.has_value());
        expect((*buffer)->primCount() == 1_u);
    };

    "Multiple shapes with varying alpha"_test = [] {
        auto buffer = YDrawBuffer::create(1024);
        expect(buffer.has_value());
        
        // Red at 100%
        (*buffer)->addCircle(0, 50.0f, 50.0f, 30.0f, 0xFF0000FF, 0, 0.0f, 0.0f);
        // Green at 75%
        (*buffer)->addCircle(0, 70.0f, 50.0f, 30.0f, 0xC000FF00, 0, 0.0f, 0.0f);
        // Blue at 50%
        (*buffer)->addCircle(0, 60.0f, 70.0f, 30.0f, 0x80FF0000, 0, 0.0f, 0.0f);
        
        expect((*buffer)->primCount() == 3_u);
    };
};
