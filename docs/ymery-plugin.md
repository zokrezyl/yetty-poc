# Ymery Plugin Integration

This document describes the integration of ymery-cpp (YAML-based ImGui framework) as a yetty terminal plugin.

## Overview

Ymery is a declarative UI framework that uses YAML to define ImGui-based interfaces. Integrating it as a yetty plugin allows rendering ImGui windows/widgets as overlays within the terminal.

## Architecture

### Current ymery Architecture

```
┌─────────────────────────────────────────────────┐
│                    ymery App                     │
├─────────────────────────────────────────────────┤
│  Window (GLFW)                                  │
│  ├── WebGPU Instance/Adapter/Device/Surface    │
│  ├── Event Loop (glfwPollEvents)               │
│  └── Present (wgpuSurfacePresent)              │
├─────────────────────────────────────────────────┤
│  ImGui Context                                  │
│  ├── imgui_impl_glfw (input)                   │
│  └── imgui_impl_wgpu (rendering)               │
├─────────────────────────────────────────────────┤
│  Widget Tree (from YAML)                        │
│  └── Renders via ImGui calls                   │
└─────────────────────────────────────────────────┘
```

### Target Architecture (Embedded Mode)

```
┌─────────────────────────────────────────────────┐
│                     yetty                        │
├─────────────────────────────────────────────────┤
│  Window (GLFW)                                  │
│  WebGPU Context (device, queue, surface)        │
│  Event Loop                                      │
│  Terminal Rendering                              │
├─────────────────────────────────────────────────┤
│  Plugin: YmeryPlugin                            │
│  ├── Receives: device, queue, target view       │
│  ├── Receives: forwarded input events           │
│  └── Renders: to provided texture               │
│      │                                          │
│      ▼                                          │
│  ┌─────────────────────────────────────────┐   │
│  │         ymery EmbeddedApp               │   │
│  │  ├── ImGui Context (separate)           │   │
│  │  ├── imgui_impl_wgpu (no glfw backend)  │   │
│  │  └── Widget Tree                        │   │
│  └─────────────────────────────────────────┘   │
└─────────────────────────────────────────────────┘
```

## Required Changes to ymery-cpp

### 1. New EmbeddedApp Class

Location: `src/ymery/embedded_app.hpp`, `src/ymery/embedded_app.cpp`

```cpp
#pragma once

#include "result.hpp"
#include "lang.hpp"
#include "dispatcher.hpp"
#include "plugin_manager.hpp"
#include "frontend/widget.hpp"
#include <webgpu/webgpu.h>
#include <filesystem>
#include <memory>

namespace ymery {

struct EmbeddedConfig {
    // WebGPU context (provided by host)
    WGPUDevice device = nullptr;
    WGPUQueue queue = nullptr;
    WGPUTextureFormat format = WGPUTextureFormat_BGRA8Unorm;

    // Layout configuration
    std::vector<std::filesystem::path> layout_paths;
    std::vector<std::filesystem::path> plugin_paths;
    std::string main_module = "app";

    // Initial size (can be updated via resize)
    uint32_t width = 800;
    uint32_t height = 600;
};

class EmbeddedApp {
public:
    static Result<std::shared_ptr<EmbeddedApp>> create(const EmbeddedConfig& config);

    // Lifecycle
    Result<void> init();
    void dispose();

    // Frame rendering (called by host each frame)
    void begin_frame(float deltaTime);
    void render_widgets();
    void end_frame(WGPURenderPassEncoder pass);

    // Resize handling
    void resize(uint32_t width, uint32_t height);

    // Input forwarding (from host)
    void on_mouse_pos(float x, float y);
    void on_mouse_button(int button, bool pressed);
    void on_mouse_scroll(float xoffset, float yoffset);
    void on_key(int key, int scancode, int action, int mods);
    void on_char(unsigned int codepoint);
    void on_focus(bool focused);

    // State queries
    bool wants_keyboard() const;  // ImGui wants keyboard input
    bool wants_mouse() const;     // ImGui wants mouse input

private:
    EmbeddedApp() = default;

    Result<void> init_imgui();
    void shutdown_imgui();

    EmbeddedConfig config_;

    // ImGui context (separate from host)
    ImGuiContext* imgui_ctx_ = nullptr;
    ImPlotContext* implot_ctx_ = nullptr;

    // ymery components
    std::shared_ptr<Lang> lang_;
    std::shared_ptr<Dispatcher> dispatcher_;
    std::shared_ptr<WidgetFactory> widget_factory_;
    std::shared_ptr<PluginManager> plugin_manager_;
    std::shared_ptr<TreeLike> data_tree_;
    WidgetPtr root_widget_;

    uint32_t width_ = 800;
    uint32_t height_ = 600;
};

using EmbeddedAppPtr = std::shared_ptr<EmbeddedApp>;

} // namespace ymery
```

### 2. ImGui Backend Changes

The embedded mode needs a modified ImGui setup:

```cpp
Result<void> EmbeddedApp::init_imgui() {
    // Create separate ImGui context
    imgui_ctx_ = ImGui::CreateContext();
    ImGui::SetCurrentContext(imgui_ctx_);

    implot_ctx_ = ImPlot::CreateContext();
    ImPlot::SetCurrentContext(implot_ctx_);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Set display size
    io.DisplaySize = ImVec2(static_cast<float>(width_), static_cast<float>(height_));

    // Initialize WebGPU backend ONLY (no GLFW backend)
    ImGui_ImplWGPU_InitInfo wgpu_info = {};
    wgpu_info.Device = config_.device;
    wgpu_info.NumFramesInFlight = 3;
    wgpu_info.RenderTargetFormat = config_.format;
    wgpu_info.DepthStencilFormat = WGPUTextureFormat_Undefined;
    ImGui_ImplWGPU_Init(&wgpu_info);

    ImGui::StyleColorsDark();

    return Ok();
}
```

### 3. Frame Rendering Without GLFW

```cpp
void EmbeddedApp::begin_frame(float deltaTime) {
    ImGui::SetCurrentContext(imgui_ctx_);
    ImPlot::SetCurrentContext(implot_ctx_);

    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = deltaTime > 0.0f ? deltaTime : 1.0f / 60.0f;

    ImGui_ImplWGPU_NewFrame();
    ImGui::NewFrame();
}

void EmbeddedApp::render_widgets() {
    if (root_widget_) {
        root_widget_->render();
    }
}

void EmbeddedApp::end_frame(WGPURenderPassEncoder pass) {
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
}
```

### 4. Input Forwarding

```cpp
void EmbeddedApp::on_mouse_pos(float x, float y) {
    ImGui::SetCurrentContext(imgui_ctx_);
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(x, y);
}

void EmbeddedApp::on_mouse_button(int button, bool pressed) {
    ImGui::SetCurrentContext(imgui_ctx_);
    ImGuiIO& io = ImGui::GetIO();
    if (button >= 0 && button < ImGuiMouseButton_COUNT) {
        io.AddMouseButtonEvent(button, pressed);
    }
}

void EmbeddedApp::on_mouse_scroll(float xoffset, float yoffset) {
    ImGui::SetCurrentContext(imgui_ctx_);
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(xoffset, yoffset);
}

void EmbeddedApp::on_key(int key, int scancode, int action, int mods) {
    ImGui::SetCurrentContext(imgui_ctx_);
    ImGuiIO& io = ImGui::GetIO();

    // Translate GLFW key to ImGui key
    ImGuiKey imgui_key = /* translation function */;
    bool pressed = (action == GLFW_PRESS || action == GLFW_REPEAT);

    io.AddKeyEvent(imgui_key, pressed);

    // Modifiers
    io.AddKeyEvent(ImGuiMod_Ctrl, (mods & GLFW_MOD_CONTROL) != 0);
    io.AddKeyEvent(ImGuiMod_Shift, (mods & GLFW_MOD_SHIFT) != 0);
    io.AddKeyEvent(ImGuiMod_Alt, (mods & GLFW_MOD_ALT) != 0);
    io.AddKeyEvent(ImGuiMod_Super, (mods & GLFW_MOD_SUPER) != 0);
}

void EmbeddedApp::on_char(unsigned int codepoint) {
    ImGui::SetCurrentContext(imgui_ctx_);
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(codepoint);
}

bool EmbeddedApp::wants_keyboard() const {
    ImGui::SetCurrentContext(imgui_ctx_);
    return ImGui::GetIO().WantCaptureKeyboard;
}

bool EmbeddedApp::wants_mouse() const {
    ImGui::SetCurrentContext(imgui_ctx_);
    return ImGui::GetIO().WantCaptureMouse;
}
```

## Yetty Plugin Implementation

### Plugin Class

Location: `src/yetty/plugins/YmeryPlugin.h`, `src/yetty/plugins/YmeryPlugin.cpp`

```cpp
#pragma once

#include "../Plugin.h"
#include <ymery/embedded_app.hpp>
#include <memory>

namespace yetty {

class YmeryPlugin : public Plugin {
public:
    YmeryPlugin() = default;
    ~YmeryPlugin() override;

    const char* pluginName() const override { return "ymery"; }

    Result<void> init(const std::string& payload) override;
    void dispose() override;
    void update(double deltaTime) override;

    void render(WebGPUContext& ctx,
                WGPUTextureView targetView, WGPUTextureFormat targetFormat,
                uint32_t screenWidth, uint32_t screenHeight,
                float pixelX, float pixelY, float pixelW, float pixelH) override;

    void onResize(uint32_t newWidth, uint32_t newHeight) override;

    // Input handling
    bool onMouseMove(float x, float y);
    bool onMouseButton(int button, bool pressed);
    bool onMouseScroll(float xoffset, float yoffset);
    bool onKey(int key, int scancode, int action, int mods);
    bool onChar(unsigned int codepoint);

private:
    ymery::EmbeddedAppPtr app_;
    double lastTime_ = 0.0;
};

} // namespace yetty
```

### Plugin Implementation

```cpp
#include "YmeryPlugin.h"
#include "../WebGPUContext.h"

namespace yetty {

Result<void> YmeryPlugin::init(const std::string& payload) {
    // Parse payload for configuration
    // Expected format: "layout_path=/path/to/layouts;main=app"

    ymery::EmbeddedConfig config;
    // TODO: Parse payload into config

    // Get WebGPU context from yetty (need to pass this somehow)
    // config.device = ...;
    // config.queue = ...;
    // config.format = ...;

    config.width = pixelWidth_;
    config.height = pixelHeight_;

    auto res = ymery::EmbeddedApp::create(config);
    if (!res) {
        return Err<void>("Failed to create ymery EmbeddedApp");
    }
    app_ = *res;

    return Ok();
}

void YmeryPlugin::dispose() {
    if (app_) {
        app_->dispose();
        app_.reset();
    }
}

void YmeryPlugin::update(double deltaTime) {
    // Nothing to do here - rendering happens in render()
}

void YmeryPlugin::render(WebGPUContext& ctx,
                         WGPUTextureView targetView, WGPUTextureFormat targetFormat,
                         uint32_t screenWidth, uint32_t screenHeight,
                         float pixelX, float pixelY, float pixelW, float pixelH) {
    if (!app_) return;

    // Calculate delta time
    double currentTime = glfwGetTime();
    float deltaTime = static_cast<float>(currentTime - lastTime_);
    lastTime_ = currentTime;

    // Begin ImGui frame
    app_->begin_frame(deltaTime);

    // Render widgets
    app_->render_widgets();

    // Create render pass for this plugin's area
    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = targetView;
    colorAttachment.loadOp = WGPULoadOp_Load;  // Preserve existing content
    colorAttachment.storeOp = WGPUStoreOp_Store;

    WGPURenderPassDescriptor renderPassDesc = {};
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &colorAttachment;

    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(
        ctx.getDevice(), nullptr);
    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(
        encoder, &renderPassDesc);

    // Set viewport to plugin area
    wgpuRenderPassEncoderSetViewport(pass, pixelX, pixelY, pixelW, pixelH, 0.0f, 1.0f);
    wgpuRenderPassEncoderSetScissorRect(pass,
        static_cast<uint32_t>(pixelX), static_cast<uint32_t>(pixelY),
        static_cast<uint32_t>(pixelW), static_cast<uint32_t>(pixelH));

    // Render ImGui
    app_->end_frame(pass);

    wgpuRenderPassEncoderEnd(pass);
    wgpuRenderPassEncoderRelease(pass);

    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, nullptr);
    wgpuQueueSubmit(ctx.getQueue(), 1, &cmdBuffer);

    wgpuCommandBufferRelease(cmdBuffer);
    wgpuCommandEncoderRelease(encoder);
}

void YmeryPlugin::onResize(uint32_t newWidth, uint32_t newHeight) {
    Plugin::onResize(newWidth, newHeight);
    if (app_) {
        app_->resize(newWidth, newHeight);
    }
}

bool YmeryPlugin::onMouseMove(float x, float y) {
    if (!app_) return false;
    app_->on_mouse_pos(x, y);
    return app_->wants_mouse();
}

bool YmeryPlugin::onMouseButton(int button, bool pressed) {
    if (!app_) return false;
    app_->on_mouse_button(button, pressed);
    return app_->wants_mouse();
}

bool YmeryPlugin::onMouseScroll(float xoffset, float yoffset) {
    if (!app_) return false;
    app_->on_mouse_scroll(xoffset, yoffset);
    return app_->wants_mouse();
}

bool YmeryPlugin::onKey(int key, int scancode, int action, int mods) {
    if (!app_) return false;
    app_->on_key(key, scancode, action, mods);
    return app_->wants_keyboard();
}

bool YmeryPlugin::onChar(unsigned int codepoint) {
    if (!app_) return false;
    app_->on_char(codepoint);
    return app_->wants_keyboard();
}

} // namespace yetty
```

## Build Integration

### Option 1: Link ymery as Static Library

```cmake
# In yetty CMakeLists.txt

# Add ymery as subdirectory or find package
add_subdirectory(${YMERY_PATH} ymery)

# Or if ymery is built separately:
find_package(ymery REQUIRED)

# Link to yetty
target_link_libraries(yetty PRIVATE ymery)
```

### Option 2: Dynamic Plugin Loading

Build ymery plugin as a separate .so:

```cmake
# In yetty plugins CMakeLists.txt

add_library(yetty_ymery_plugin SHARED
    YmeryPlugin.cpp
)
target_link_libraries(yetty_ymery_plugin PRIVATE
    ymery
    # ... other deps
)
```

## OSC Sequence Format

To create a ymery plugin instance via terminal escape sequence:

```
ESC ] 99999 ; ymery ; relative ; x,y,w,h ; layout_path=/path;main=module ST
```

Example:
```bash
printf '\e]99999;ymery;relative;0,0,40,20;layout_path=/home/user/layouts;main=dashboard\e\\'
```

## Considerations

### 1. ImGui Context Isolation

Each ymery plugin instance has its own ImGui context. This ensures:
- No conflicts with other ImGui users (if yetty uses ImGui elsewhere)
- Clean state management per instance
- Independent font atlases

### 2. Input Routing

Yetty must route input events to the ymery plugin when:
- Mouse is within plugin bounds
- Plugin has keyboard focus (via `wants_keyboard()`)

The plugin returns `true` from input handlers when it consumes the event.

### 3. Rendering Order

Ymery plugin renders AFTER the terminal content but uses `WGPULoadOp_Load` to preserve the terminal background. This allows:
- Transparent ImGui windows over terminal
- Proper layering with other plugins

### 4. Performance

- ImGui rendering is lightweight
- Consider caching if layout is static
- Use `needsRender_` flag to skip unchanged frames

## Future Enhancements

1. **Shared Font Atlas**: Share font textures with yetty to reduce memory
2. **Theme Integration**: Match ymery theme to terminal colors
3. **Bidirectional Communication**: ymery widgets can send commands to terminal
4. **Hot Reload**: Watch layout YAML files for changes
