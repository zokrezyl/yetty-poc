# Ymery as Yetty Plugin

This document describes how to integrate ymery widget rendering into the yetty terminal emulator.

## Overview

Ymery can run in two modes:
1. **Standalone** - ymery owns the window, WebGPU context, and render loop (`App` class)
2. **Embedded** - another app owns the render loop, ymery renders widgets with its own ImGui context (`EmbeddedApp` class)

For yetty integration, we use embedded mode with separate ImGui context per instance.

## Architecture

### Current ymery Architecture (Standalone)

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
│  └── Renders: to provided render pass           │
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

## EmbeddedApp API (Required)

Location: `src/ymery/embedded.hpp`

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

    // Access components
    std::shared_ptr<Lang> lang() const;
    std::shared_ptr<Dispatcher> dispatcher() const;
    std::shared_ptr<WidgetFactory> widget_factory() const;
    std::shared_ptr<PluginManager> plugin_manager() const;
    std::shared_ptr<TreeLike> data_tree() const;

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

## Implementation Details

### 1. Separate ImGui Context

Each EmbeddedApp instance creates its own ImGui/ImPlot context:

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

### 2. Frame Rendering Without GLFW

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

### 3. Input Forwarding

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

    ImGuiKey imgui_key = glfw_key_to_imgui_key(key);
    bool pressed = (action != 0);  // GLFW_PRESS or GLFW_REPEAT

    io.AddKeyEvent(imgui_key, pressed);

    // Modifiers
    io.AddKeyEvent(ImGuiMod_Ctrl, (mods & 0x0002) != 0);   // GLFW_MOD_CONTROL
    io.AddKeyEvent(ImGuiMod_Shift, (mods & 0x0001) != 0);  // GLFW_MOD_SHIFT
    io.AddKeyEvent(ImGuiMod_Alt, (mods & 0x0004) != 0);    // GLFW_MOD_ALT
    io.AddKeyEvent(ImGuiMod_Super, (mods & 0x0008) != 0);  // GLFW_MOD_SUPER
}

void EmbeddedApp::on_char(unsigned int codepoint) {
    ImGui::SetCurrentContext(imgui_ctx_);
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(codepoint);
}

void EmbeddedApp::on_focus(bool focused) {
    ImGui::SetCurrentContext(imgui_ctx_);
    ImGuiIO& io = ImGui::GetIO();
    io.AddFocusEvent(focused);
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

### 4. Resize Handling

```cpp
void EmbeddedApp::resize(uint32_t width, uint32_t height) {
    width_ = width;
    height_ = height;

    ImGui::SetCurrentContext(imgui_ctx_);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
}
```

## Changes Required

### In ymery-cpp

Update `src/ymery/embedded.hpp` and `src/ymery/embedded.cpp` to match the API above:

1. Add `ImGuiContext*` and `ImPlotContext*` members
2. Add `init_imgui()` / `shutdown_imgui()` private methods
3. Change `render_widgets()` to just render (no NewFrame/Render)
4. Add `begin_frame(float deltaTime)` method
5. Add `end_frame(WGPURenderPassEncoder pass)` method
6. Add `resize(uint32_t, uint32_t)` method
7. Add input forwarding methods (on_mouse_pos, on_key, etc.)
8. Add `wants_keyboard()` / `wants_mouse()` queries
9. Add GLFW key to ImGui key translation function

### In yetty-poc

Create `src/yetty/plugins/YmeryPlugin.h` and `.cpp` as a yetty plugin that:
1. Creates EmbeddedApp with yetty's WebGPU device/queue
2. Forwards input events when mouse is in plugin bounds
3. Calls begin_frame/render_widgets/end_frame in render()
4. Uses wants_keyboard/wants_mouse to route input

## Usage Flow

```
Host (yetty)                          ymery EmbeddedApp
    │                                        │
    │  create(config)  ───────────────────►  │
    │                                        │ init()
    │                                        │ init_imgui()
    │                                        │
    │  ──── per frame ─────────────────────  │
    │                                        │
    │  on_mouse_pos(x, y) ────────────────►  │ io.AddMousePosEvent()
    │  on_key(key, ...) ──────────────────►  │ io.AddKeyEvent()
    │                                        │
    │  begin_frame(dt) ───────────────────►  │ SetCurrentContext()
    │                                        │ ImGui_ImplWGPU_NewFrame()
    │                                        │ ImGui::NewFrame()
    │                                        │
    │  render_widgets() ──────────────────►  │ root_widget_->render()
    │                                        │
    │  end_frame(pass) ───────────────────►  │ ImGui::Render()
    │                                        │ ImGui_ImplWGPU_RenderDrawData()
    │                                        │
    │  ◄────── wants_keyboard() ───────────  │ return io.WantCaptureKeyboard
    │  ◄────── wants_mouse() ──────────────  │ return io.WantCaptureMouse
    │                                        │
    │  ──── on shutdown ───────────────────  │
    │                                        │
    │  dispose() ─────────────────────────►  │ shutdown_imgui()
    │                                        │ cleanup widgets
```

## Key Design Decisions

1. **Separate ImGui context per instance** - No conflicts with host or other plugins
2. **No GLFW backend** - Input is forwarded explicitly by host
3. **Host owns render pass** - ymery just renders to provided encoder
4. **Input routing via wants_* queries** - Host decides when to forward input
5. **Plugins still work** - Loaded from plugin paths, same as standalone mode
