#include <yetty/imgui-manager.h>
#include <yetty/yetty-context.h>
#include <yetty/base/event-loop.h>
#include "ymery/renderer.h"
#include <ytrace/ytrace.hpp>

#include <imgui.h>
#include <imgui_impl_wgpu.h>

#include <vector>

namespace yetty {

//-----------------------------------------------------------------------------
// ImguiManagerImpl
//-----------------------------------------------------------------------------
class ImguiManagerImpl : public ImguiManager {
public:
    explicit ImguiManagerImpl(const YettyContext& ctx);
    ~ImguiManagerImpl() override;

    Result<void> init() noexcept;
    void registerForEvents();

    // EventListener interface
    Result<bool> onEvent(const base::Event& event) override;

    // ImguiManager interface
    void updateDisplaySize(uint32_t width, uint32_t height) override;
    void openContextMenu(float x, float y, std::vector<ContextMenuItem> items) override;
    void clearContextMenu() override;
    void setStatusText(const std::string& text) override;
    float getStatusbarHeight() const override { return STATUSBAR_HEIGHT; }
    Result<void> render(WGPURenderPassEncoder pass) override;

    static constexpr float STATUSBAR_HEIGHT = 22.0f;
    bool isContextMenuOpen() const override { return _menuOpen; }
    ImGuiContext* context() const override { return _imguiContext; }

private:
    void dispatchEvent(const base::Event& event);

    const YettyContext& _ctx;
    ImGuiContext* _imguiContext = nullptr;
    uint32_t _displayWidth = 0;
    uint32_t _displayHeight = 0;

    // Context menu state
    std::vector<ContextMenuItem> _menuItems;
    float _menuX = 0.0f;
    float _menuY = 0.0f;
    bool _menuOpen = false;
    bool _rightClickPending = false;

    // Statusbar
    std::string _statusText;
};

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

ImguiManagerImpl::ImguiManagerImpl(const YettyContext& ctx)
    : _ctx(ctx) {
}

ImguiManagerImpl::~ImguiManagerImpl() {
    if (_imguiContext) {
        ImGui::SetCurrentContext(_imguiContext);
        ImGui_ImplWGPU_Shutdown();
        ImGui::DestroyContext(_imguiContext);
        _imguiContext = nullptr;
    }
}

Result<void> ImguiManagerImpl::init() noexcept {
    if (!_ctx.gpu.device || !_ctx.gpu.queue) {
        return Err<void>("ImguiManager: GPUContext not initialized");
    }

    // Create ImGui context
    _imguiContext = ImGui::CreateContext();
    if (!_imguiContext) {
        return Err<void>("Failed to create ImGui context");
    }
    ImGui::SetCurrentContext(_imguiContext);

    // Configure ImGui
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;  // Don't save settings

    ImGui::StyleColorsDark();

    // Initialize WebGPU backend
    ImGui_ImplWGPU_InitInfo wgpu_info = {};
    wgpu_info.Device = _ctx.gpu.device;
    wgpu_info.NumFramesInFlight = 3;
    wgpu_info.RenderTargetFormat = _ctx.gpu.surfaceFormat;
    wgpu_info.DepthStencilFormat = WGPUTextureFormat_Undefined;

    if (!ImGui_ImplWGPU_Init(&wgpu_info)) {
        ImGui::DestroyContext(_imguiContext);
        _imguiContext = nullptr;
        return Err<void>("Failed to init ImGui WebGPU backend");
    }

    yinfo("ImguiManager initialized");
    return Ok();
}

void ImguiManagerImpl::registerForEvents() {
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        yerror("ImguiManager: Failed to get EventLoop instance");
        return;
    }
    auto loop = *loopResult;
    loop->registerListener(base::Event::Type::MouseDown, sharedAs<base::EventListener>());
    loop->registerListener(base::Event::Type::MouseUp, sharedAs<base::EventListener>());
    loop->registerListener(base::Event::Type::MouseMove, sharedAs<base::EventListener>());
    yinfo("ImguiManager registered for mouse events");
}

Result<bool> ImguiManagerImpl::onEvent(const base::Event& event) {
    if (!_imguiContext) return Ok(false);

    ImGui::SetCurrentContext(_imguiContext);
    ImGuiIO& io = ImGui::GetIO();

    if (event.type == base::Event::Type::MouseMove) {
        io.AddMousePosEvent(event.mouse.x, event.mouse.y);
    }
    else if (event.type == base::Event::Type::MouseDown) {
        io.AddMousePosEvent(event.mouse.x, event.mouse.y);
        io.AddMouseButtonEvent(event.mouse.button, true);

        if (event.mouse.button == 0 && _menuOpen) {  // GLFW_MOUSE_BUTTON_LEFT
            // Left-click while menu is open: close the menu
            clearContextMenu();
            ydebug("ImguiManager: Left-click, closing menu");
        }
    }
    else if (event.type == base::Event::Type::MouseUp) {
        io.AddMousePosEvent(event.mouse.x, event.mouse.y);
        io.AddMouseButtonEvent(event.mouse.button, false);
    }

    return Ok(false);  // Don't consume - let other listeners handle
}

void ImguiManagerImpl::updateDisplaySize(uint32_t width, uint32_t height) {
    _displayWidth = width;
    _displayHeight = height;
}

void ImguiManagerImpl::openContextMenu(float x, float y, std::vector<ContextMenuItem> items) {
    clearContextMenu();
    _menuX = x;
    _menuY = y;
    _menuItems = std::move(items);
    ydebug("ImguiManager: Opening menu at ({}, {}) with {} items", x, y, _menuItems.size());
}

void ImguiManagerImpl::clearContextMenu() {
    _menuItems.clear();
    _menuOpen = false;
    _rightClickPending = false;
}

void ImguiManagerImpl::setStatusText(const std::string& text) {
    _statusText = text;
}

void ImguiManagerImpl::dispatchEvent(const base::Event& event) {
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        yerror("ImguiManager: Failed to get EventLoop instance");
        return;
    }
    auto loop = *loopResult;
    loop->dispatch(event);
}

Result<void> ImguiManagerImpl::render(WGPURenderPassEncoder pass) {
    if (!_imguiContext) {
        return Ok();  // Not initialized yet
    }

    ImGui::SetCurrentContext(_imguiContext);

    // Setup frame
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(_displayWidth), static_cast<float>(_displayHeight));
    io.DeltaTime = 1.0f / 60.0f;

    ImGui_ImplWGPU_NewFrame();
    ImGui::NewFrame();

    // Handle context menu - if there are items, show popup
    if (!_menuItems.empty()) {
        if (!_menuOpen) {
            // First frame with items - open popup
            ImGui::OpenPopup("ContextMenu");
            _menuOpen = true;
            _rightClickPending = false;
        }

        // SetNextWindowPos must be called immediately before BeginPopup
        ImGui::SetNextWindowPos(ImVec2(_menuX, _menuY), ImGuiCond_Appearing);
        if (ImGui::BeginPopup("ContextMenu")) {
            for (const auto& item : _menuItems) {
                if (ImGui::MenuItem(item.label.c_str())) {
                    dispatchEvent(item.event);
                    clearContextMenu();
                    ImGui::CloseCurrentPopup();
                    break;
                }
            }
            ImGui::EndPopup();
        } else {
            // Popup was closed (clicked outside)
            clearContextMenu();
        }
    }

    // Ymery: render YAML-driven widgets
    if (_ctx.ymeryRenderer) {
        if (auto res = _ctx.ymeryRenderer->renderFrame(); !res) {
            ydebug("ymeryRenderer->renderFrame() failed: {}", res.error().to_string());
        }
    }

    // Statusbar at bottom of screen
    {
        float yPos = static_cast<float>(_displayHeight) - STATUSBAR_HEIGHT;
        ImGui::SetNextWindowPos(ImVec2(0, yPos), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(_displayWidth), STATUSBAR_HEIGHT), ImGuiCond_Always);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.9f));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;
        if (ImGui::Begin("##Statusbar", nullptr, flags)) {
            if (_statusText.empty()) {
                ImGui::Text("Ready");
            } else {
                ImGui::Text("%s", _statusText.c_str());
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(3);
    }

    // Render
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);

    // Reset scissor rect - ImGui leaves it set to its last clip rect
    wgpuRenderPassEncoderSetScissorRect(pass, 0, 0, _displayWidth, _displayHeight);

    return Ok();
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------

Result<ImguiManager::Ptr> ImguiManager::createImpl(ContextType& /*ctx*/, const YettyContext& yettyCtx) noexcept {
    auto impl = Ptr(new ImguiManagerImpl(yettyCtx));
    if (auto res = static_cast<ImguiManagerImpl*>(impl.get())->init(); !res) {
        return Err<Ptr>("ImguiManager init failed", res);
    }
    static_cast<ImguiManagerImpl*>(impl.get())->registerForEvents();
    return Ok(std::move(impl));
}

} // namespace yetty
