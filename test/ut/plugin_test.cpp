//=============================================================================
// Plugin Tests
//
// Tests for Plugin base class functionality
//=============================================================================

#include <boost/ut.hpp>
#include "harness/mock_plugin.h"

using namespace boost::ut;
using namespace yetty;
using namespace yetty::test;

suite plugin_tests = [] {
    "plugin can be created"_test = [] {
        auto result = MockPlugin::create(nullptr);

        expect(result.has_value());
    };

    "plugin has correct name"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        expect(std::string(plugin->pluginName()) == "mock");
    };

    "plugin is initialized after create"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        expect(plugin->isInitialized());
    };

    "plugin has no layers initially"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        expect(plugin->getWidgets().empty());
    };

    "plugin createWidget returns valid layer"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto layerResult = plugin->createWidget("test_payload");

        expect(layerResult.has_value());
    };

    "plugin addWidget adds layer to list"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto layerResult = plugin->createWidget("payload");
        plugin->addWidget(*layerResult);

        expect(plugin->getWidgets().size() == 1_ul);
    };

    "plugin can have multiple layers"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto layer1 = *plugin->createWidget("payload1");
        auto layer2 = *plugin->createWidget("payload2");
        auto layer3 = *plugin->createWidget("payload3");

        plugin->addWidget(layer1);
        plugin->addWidget(layer2);
        plugin->addWidget(layer3);

        expect(plugin->getWidgets().size() == 3_ul);
    };

    "plugin getWidget returns correct layer by ID"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto layer1 = *plugin->createWidget("p1");
        auto layer2 = *plugin->createWidget("p2");
        layer1->setId(100);
        layer2->setId(200);

        plugin->addWidget(layer1);
        plugin->addWidget(layer2);

        auto found = plugin->getWidget(200);
        expect(found != nullptr);
        expect(found->id() == 200_u);
    };

    "plugin getWidget returns nullptr for unknown ID"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto found = plugin->getWidget(999);

        expect(found == nullptr);
    };

    "plugin removeWidget removes layer by ID"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto layer = *plugin->createWidget("payload");
        layer->setId(42);
        plugin->addWidget(layer);

        expect(plugin->getWidgets().size() == 1_ul);

        auto removeResult = plugin->removeWidget(42);

        expect(removeResult.has_value());
        expect(plugin->getWidgets().empty());
    };

    "plugin removeWidget fails for unknown ID"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto removeResult = plugin->removeWidget(999);

        expect(!removeResult.has_value());
    };

    "plugin removeWidget calls dispose on layer"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto mockPlugin = std::static_pointer_cast<MockPlugin>(*result);

        auto layerResult = mockPlugin->createWidget("payload");
        auto layer = *layerResult;
        layer->setId(42);
        mockPlugin->addWidget(layer);

        mockPlugin->removeWidget(42);

        // Check the created layer was disposed
        auto& createdWidgets = mockPlugin->createdWidgets();
        expect(!createdWidgets.empty());
        expect(createdWidgets[0]->disposeCalled());
    };

    "plugin dispose disposes all layers"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto mockPlugin = std::static_pointer_cast<MockPlugin>(*result);

        auto layer1 = std::static_pointer_cast<MockPluginWidget>(*mockPlugin->createWidget("p1"));
        auto layer2 = std::static_pointer_cast<MockPluginWidget>(*mockPlugin->createWidget("p2"));
        mockPlugin->addWidget(layer1);
        mockPlugin->addWidget(layer2);

        mockPlugin->dispose();

        expect(layer1->disposeCalled());
        expect(layer2->disposeCalled());
        expect(mockPlugin->getWidgets().empty());
    };

    "plugin onTerminalResize updates layer pixel sizes"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto layer = *plugin->createWidget("payload");
        layer->setCellSize(10, 5);  // 10 cols x 5 rows
        plugin->addWidget(layer);

        plugin->onTerminalResize(12, 24);  // cell size 12x24 pixels

        expect(layer->getPixelWidth() == 120_u);  // 10 * 12
        expect(layer->getPixelHeight() == 120_u); // 5 * 24
    };

    "layer parent is set when added to plugin"_test = [] {
        auto result = MockPlugin::create(nullptr);
        auto plugin = *result;

        auto layer = *plugin->createWidget("payload");
        expect(layer->getParent() == nullptr);

        plugin->addWidget(layer);

        expect(layer->getParent() == plugin.get());
    };
};
