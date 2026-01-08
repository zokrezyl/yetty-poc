//=============================================================================
// Plugin Layer Tests
//
// Tests for Widget position, visibility, screen type, and input handling
//=============================================================================

#include <boost/ut.hpp>
#include "harness/mock_plugin.h"

using namespace boost::ut;
using namespace yetty;
using namespace yetty::test;

suite plugin_layer_tests = [] {
    "widget has default position 0,0"_test = [] {
        MockPluginWidget widget;

        expect(widget.getX() == 0_i);
        expect(widget.getY() == 0_i);
    };

    "widget position can be set"_test = [] {
        MockPluginWidget widget;

        widget.setPosition(10, 20);

        expect(widget.getX() == 10_i);
        expect(widget.getY() == 20_i);
    };

    "widget has default cell size 1x1"_test = [] {
        MockPluginWidget widget;

        expect(widget.getWidthCells() == 1_u);
        expect(widget.getHeightCells() == 1_u);
    };

    "widget cell size can be set"_test = [] {
        MockPluginWidget widget;

        widget.setCellSize(40, 20);

        expect(widget.getWidthCells() == 40_u);
        expect(widget.getHeightCells() == 20_u);
    };

    "widget pixel size can be set"_test = [] {
        MockPluginWidget widget;

        widget.setPixelSize(800, 600);

        expect(widget.getPixelWidth() == 800_u);
        expect(widget.getPixelHeight() == 600_u);
    };

    "widget is visible by default"_test = [] {
        MockPluginWidget widget;

        expect(widget.isVisible());
    };

    "widget visibility can be toggled"_test = [] {
        MockPluginWidget widget;

        widget.setVisible(false);
        expect(!widget.isVisible());

        widget.setVisible(true);
        expect(widget.isVisible());
    };

    "widget has default position mode Absolute"_test = [] {
        MockPluginWidget widget;

        expect(widget.getPositionMode() == PositionMode::Absolute);
    };

    "widget position mode can be set to Relative"_test = [] {
        MockPluginWidget widget;

        widget.setPositionMode(PositionMode::Relative);

        expect(widget.getPositionMode() == PositionMode::Relative);
    };

    "widget has default screen type Main"_test = [] {
        MockPluginWidget widget;

        expect(widget.getScreenType() == ScreenType::Main);
    };

    "widget screen type can be set to Alternate"_test = [] {
        MockPluginWidget widget;

        widget.setScreenType(ScreenType::Alternate);

        expect(widget.getScreenType() == ScreenType::Alternate);
    };

    "widget ID can be set and retrieved"_test = [] {
        MockPluginWidget widget;

        widget.setId(42);

        expect(widget.id() == 42_u);
    };

    "widget has no focus by default"_test = [] {
        MockPluginWidget widget;

        expect(!widget.hasFocus());
    };

    "widget focus can be set"_test = [] {
        MockPluginWidget widget;

        widget.setFocus(true);
        expect(widget.hasFocus());

        widget.setFocus(false);
        expect(!widget.hasFocus());
    };

    "widget payload can be set"_test = [] {
        MockPluginWidget widget;

        widget.setPayload("test_payload");

        expect(widget.getPayload() == "test_payload");
    };

    "widget init is called with payload"_test = [] {
        MockPluginWidget widget;

        auto result = widget.init("my_payload");

        expect(result.has_value());
        expect(widget.initCalled());
        expect(widget.getPayload() == "my_payload");
    };

    "widget dispose marks widget as disposed"_test = [] {
        MockPluginWidget widget;
        widget.init("test");

        auto result = widget.dispose();

        expect(result.has_value());
        expect(widget.disposeCalled());
    };

    "widget mouse move is tracked"_test = [] {
        MockPluginWidget widget;
        widget.setWantsMouse(true);

        widget.onMouseMove(100.5f, 200.5f);

        expect(widget.lastMouseX() == 100.5_f);
        expect(widget.lastMouseY() == 200.5_f);
        expect(widget.mouseMoveCount() == 1_i);
    };

    "widget mouse button is tracked"_test = [] {
        MockPluginWidget widget;
        widget.setWantsMouse(true);

        widget.onMouseButton(0, true);
        widget.onMouseButton(0, false);

        expect(widget.mouseButtonCount() == 2_i);
    };

    "widget wantsKeyboard returns configured value"_test = [] {
        MockPluginWidget widget;

        expect(!widget.wantsKeyboard());

        widget.setWantsKeyboard(true);
        expect(widget.wantsKeyboard());
    };

    "widget wantsMouse returns configured value"_test = [] {
        MockPluginWidget widget;

        expect(!widget.wantsMouse());

        widget.setWantsMouse(true);
        expect(widget.wantsMouse());
    };

    "widget onResize updates pixel dimensions"_test = [] {
        MockPluginWidget widget;

        widget.onResize(1024, 768);

        expect(widget.getPixelWidth() == 1024_u);
        expect(widget.getPixelHeight() == 768_u);
    };

    "relative widget position updated on scroll simulation"_test = [] {
        MockPluginWidget widget;
        widget.setPositionMode(PositionMode::Relative);
        widget.setPosition(5, 10);

        // Simulate scroll: content moves up by 3 rows
        // Layer Y should decrease by 3
        int scrollDelta = 3;
        widget.setPosition(widget.getX(), widget.getY() - scrollDelta);

        expect(widget.getY() == 7_i);
    };

    "absolute widget position not affected by scroll simulation"_test = [] {
        MockPluginWidget widget;
        widget.setPositionMode(PositionMode::Absolute);
        widget.setPosition(5, 10);

        // For absolute layers, position should not change
        // (scroll handling is done by PluginManager, not widget)
        expect(widget.getY() == 10_i);
    };
};
