#pragma once

#include <yetty/plugin.h>
#include <yetty/hdraw.h>
#include <webgpu/webgpu.h>
#include <memory>

namespace yetty {

//-----------------------------------------------------------------------------
// HDrawPlugin - Plugin wrapper for HDrawRenderer
// Demonstrates how to use the core hdraw library in a plugin
//-----------------------------------------------------------------------------

class HDraw;

class HDrawPlugin : public Plugin {
public:
    ~HDrawPlugin() override;

    static Result<PluginPtr> create() noexcept;

    const char* pluginName() const override { return "hdraw"; }

    Result<void> dispose() override;

    Result<WidgetPtr> createWidget(
        const std::string& widgetName,
        WidgetFactory* factory,
        FontManager* fontManager,
        uv_loop_t* loop,
        int32_t x,
        int32_t y,
        uint32_t widthCells,
        uint32_t heightCells,
        const std::string& pluginArgs,
        const std::string& payload
    ) override;

private:
    HDrawPlugin() noexcept = default;
    Result<void> pluginInit() noexcept;
};

//-----------------------------------------------------------------------------
// HDraw - Plugin widget that wraps HDrawRenderer
//-----------------------------------------------------------------------------

class HDraw : public Widget {
public:
    static Result<WidgetPtr> create(
        WidgetFactory* factory,
        FontManager* fontManager,
        uv_loop_t* loop,
        int32_t x,
        int32_t y,
        uint32_t widthCells,
        uint32_t heightCells,
        const std::string& pluginArgs,
        const std::string& payload
    ) {
        (void)factory;
        (void)fontManager;
        (void)loop;
        (void)pluginArgs;
        auto w = std::shared_ptr<HDraw>(new HDraw(payload));
        w->_x = x;
        w->_y = y;
        w->_widthCells = widthCells;
        w->_heightCells = heightCells;
        if (auto res = w->init(); !res) {
            return Err<WidgetPtr>("Failed to init HDraw", res);
        }
        return Ok(std::static_pointer_cast<Widget>(w));
    }

    ~HDraw() override;

    Result<void> dispose() override;

    void prepareFrame(WebGPUContext& ctx, bool on) override;
    Result<void> render(WGPURenderPassEncoder pass, WebGPUContext& ctx, bool on) override;

    bool onMouseMove(float localX, float localY) override;
    bool onMouseButton(int button, bool pressed) override;
    bool onKey(int key, int scancode, int action, int mods) override;
    bool onChar(unsigned int codepoint) override;
    bool wantsMouse() const override { return true; }
    bool wantsKeyboard() const override { return true; }

private:
    explicit HDraw(const std::string& payload) {
        _payload = payload;
    }
    Result<void> init() override;

    std::unique_ptr<HDrawRenderer> renderer_;
    bool failed_ = false;
};

} // namespace yetty

extern "C" {
    const char* name();
    yetty::Result<yetty::PluginPtr> create();
}
