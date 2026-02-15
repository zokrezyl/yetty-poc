#include "ymery/renderer.h"
#include <yetty/base/event-loop.h>
#include <yetty/gpu-monitor.h>
#include <yetty/imgui-manager.h>
#include <yetty/yetty-context.h>
#include <ytrace/ytrace.hpp>

#include <imgui.h>
#include <imgui_impl_wgpu.h>

#include <chrono>
#include <vector>

namespace yetty {

//-----------------------------------------------------------------------------
// ImguiManagerImpl
//-----------------------------------------------------------------------------
class ImguiManagerImpl : public ImguiManager {
public:
  explicit ImguiManagerImpl(const YettyContext &ctx);
  ~ImguiManagerImpl() override;

  Result<void> init() noexcept;
  void registerForEvents();

  // EventListener interface
  Result<bool> onEvent(const base::Event &event) override;

  // ImguiManager interface
  void updateDisplaySize(uint32_t width, uint32_t height) override;
  void beginContextMenu(float x, float y) override;
  void addContextMenuItem(const ContextMenuItem &item) override;
  void clearContextMenu() override;
  void setStatusText(const std::string &text) override;
  void setFps(uint32_t fps) override;
  float getStatusbarHeight() const override { return STATUSBAR_HEIGHT; }
  Result<void> render(WGPURenderPassEncoder pass) override;

  void showGpuStatsDialog(std::function<std::string()> provider) override;

  static constexpr float STATUSBAR_HEIGHT = 22.0f;
  bool isContextMenuOpen() const override { return _menuOpen; }
  ImGuiContext *context() const override { return _imguiContext; }

private:
  void dispatchEvent(const base::Event &event);

  const YettyContext &_ctx;
  ImGuiContext *_imguiContext = nullptr;
  base::ObjectId _id;
  uint32_t _displayWidth = 0;
  uint32_t _displayHeight = 0;

  // Context menu state
  std::vector<ContextMenuItem> _menuItems;
  float _menuX = 0.0f;
  float _menuY = 0.0f;
  bool _menuOpen = false;
  bool _rightClickPending = false;

  // GPU Stats dialog
  bool _showGpuStats = false;
  std::function<std::string()> _gpuStatsProvider;

  // Statusbar
  std::string _statusText;
  uint32_t _fps = 0;

  // GPU usage monitoring (sampled periodically, not every frame)
  gpu::GpuStats _gpuUsageStats;
  std::chrono::steady_clock::time_point _lastGpuSample;
  static constexpr auto GPU_SAMPLE_INTERVAL = std::chrono::milliseconds(500);
};

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

static base::ObjectId s_nextImguiId = 0xFFFF0000;

ImguiManagerImpl::ImguiManagerImpl(const YettyContext &ctx)
    : _ctx(ctx), _id(s_nextImguiId++) {}

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
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.IniFilename = nullptr; // Don't save settings

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
  // Priority 100: ImguiManager must see mouse events BEFORE GPUScreen (priority
  // 0) so it can forward them to ImGui and consume them when ImGui wants the
  // mouse
  loop->registerListener(base::Event::Type::MouseDown,
                         sharedAs<base::EventListener>(), 100);
  loop->registerListener(base::Event::Type::MouseUp,
                         sharedAs<base::EventListener>(), 100);
  loop->registerListener(base::Event::Type::MouseMove,
                         sharedAs<base::EventListener>(), 100);
  loop->registerListener(base::Event::Type::KeyDown,
                         sharedAs<base::EventListener>(), 100);
  yinfo("ImguiManager registered for mouse and key events");
}

constexpr int GLFW_KEY_ESCAPE = 256;

Result<bool> ImguiManagerImpl::onEvent(const base::Event &event) {
  if (!_imguiContext)
    return Ok(false);

  ImGui::SetCurrentContext(_imguiContext);
  ImGuiIO &io = ImGui::GetIO();

  if (event.type == base::Event::Type::MouseMove) {
    io.AddMousePosEvent(event.mouse.x, event.mouse.y);
  } else if (event.type == base::Event::Type::MouseDown) {
    ydebug("ImguiManager::onEvent: MouseDown button={} at ({}, {}) "
           "WantCaptureMouse={}",
           event.mouse.button, event.mouse.x, event.mouse.y,
           io.WantCaptureMouse);
    io.AddMousePosEvent(event.mouse.x, event.mouse.y);
    io.AddMouseButtonEvent(event.mouse.button, true);
  } else if (event.type == base::Event::Type::MouseUp) {
    ydebug("ImguiManager::onEvent: MouseUp button={} at ({}, {}) "
           "WantCaptureMouse={}",
           event.mouse.button, event.mouse.x, event.mouse.y,
           io.WantCaptureMouse);
    io.AddMousePosEvent(event.mouse.x, event.mouse.y);
    io.AddMouseButtonEvent(event.mouse.button, false);
  } else if (event.type == base::Event::Type::KeyDown) {
    if (event.key.key == GLFW_KEY_ESCAPE && (_menuOpen || _showGpuStats)) {
      clearContextMenu();
      _showGpuStats = false;
      return Ok(true);
    }
  }

  // When ImGui wants the mouse (popup/dialog open), consume the event
  // so GPUScreen doesn't process it (focus, selection, etc.)
  if (io.WantCaptureMouse && (event.type == base::Event::Type::MouseDown ||
                              event.type == base::Event::Type::MouseUp ||
                              event.type == base::Event::Type::MouseMove)) {
    return Ok(true);
  }

  return Ok(false);
}

void ImguiManagerImpl::updateDisplaySize(uint32_t width, uint32_t height) {
  _displayWidth = width;
  _displayHeight = height;
}

void ImguiManagerImpl::beginContextMenu(float x, float y) {
  clearContextMenu();
  _menuX = x;
  _menuY = y;
  _rightClickPending = true;
  // Steal focus from GPUScreen so it stops processing mouse/keyboard events
  auto loopResult = base::EventLoop::instance();
  if (loopResult) {
    (*loopResult)->dispatch(base::Event::focusEvent(_id));
  }
  ydebug("ImguiManager: beginContextMenu at ({}, {})", x, y);
}

void ImguiManagerImpl::addContextMenuItem(const ContextMenuItem &item) {
  _menuItems.push_back(item);
}

void ImguiManagerImpl::clearContextMenu() {
  _menuItems.clear();
  _menuOpen = false;
  _rightClickPending = false;
}

void ImguiManagerImpl::setStatusText(const std::string &text) {
  _statusText = text;
}

void ImguiManagerImpl::setFps(uint32_t fps) {
  _fps = fps;
}

void ImguiManagerImpl::showGpuStatsDialog(
    std::function<std::string()> provider) {
  _gpuStatsProvider = std::move(provider);
  _showGpuStats = true;
}

void ImguiManagerImpl::dispatchEvent(const base::Event &event) {
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
    return Ok(); // Not initialized yet
  }

  ImGui::SetCurrentContext(_imguiContext);

  // Setup frame
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = ImVec2(static_cast<float>(_displayWidth),
                          static_cast<float>(_displayHeight));
  io.DeltaTime = 1.0f / 60.0f;

  ImGui_ImplWGPU_NewFrame();
  ImGui::NewFrame();

  // Handle context menu - if there are items, show popup
  if (!_menuItems.empty()) {
    ydebug("ImguiManager::render: _menuItems.size()={} _menuOpen={}",
           _menuItems.size(), _menuOpen);
    if (!_menuOpen) {
      ydebug("ImguiManager::render: calling OpenPopup at ({}, {})", _menuX,
             _menuY);
      ImGui::OpenPopup("ContextMenu");
      _menuOpen = true;
      _rightClickPending = false;
    }

    ImGui::SetNextWindowPos(ImVec2(_menuX, _menuY), ImGuiCond_Appearing);
    if (ImGui::BeginPopup("ContextMenu")) {
      ydebug(
          "ImguiManager::render: BeginPopup returned true, rendering {} items",
          _menuItems.size());
      for (size_t i = 0; i < _menuItems.size(); i++) {
        const auto &item = _menuItems[i];
        bool clicked = ImGui::MenuItem(item.label.c_str());
        ydebug("ImguiManager::render: MenuItem '{}' clicked={} hasCallback={}",
               item.label, clicked, (bool)item.callback);
        if (clicked) {
          ydebug("ImguiManager::render: CLICKED '{}'", item.label);
          if (item.callback) {
            ydebug("ImguiManager::render: calling callback for '{}'",
                   item.label);
            item.callback();
            ydebug("ImguiManager::render: callback done, _showGpuStats={}",
                   _showGpuStats);
          } else {
            ydebug("ImguiManager::render: dispatching event for '{}'",
                   item.label);
            dispatchEvent(item.event);
          }
          clearContextMenu();
          ImGui::CloseCurrentPopup();
          break;
        }
      }
      ImGui::EndPopup();
    } else {
      ydebug("ImguiManager::render: BeginPopup returned false, closing menu");
      clearContextMenu();
    }
  }

  // GPU Stats dialog - call provider every frame for live data
  if (_showGpuStats && _gpuStatsProvider) {
    std::string stats = _gpuStatsProvider();
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("GPU Memory Stats", &_showGpuStats)) {
      ImGui::TextUnformatted(stats.c_str());
    }
    ImGui::End();
    if (!_showGpuStats) {
      _gpuStatsProvider = nullptr;
    }
  }

  // Ymery: render YAML-driven widgets
  if (_ctx.ymeryRenderer) {
    if (auto res = _ctx.ymeryRenderer->renderFrame(); !res) {
      ydebug("ymeryRenderer->renderFrame() failed: {}",
             res.error().to_string());
    }
  }

  // Sample GPU usage periodically (not every frame)
  auto now = std::chrono::steady_clock::now();
  if (_ctx.gpuMonitor && (now - _lastGpuSample >= GPU_SAMPLE_INTERVAL)) {
    _gpuUsageStats = _ctx.gpuMonitor->sample();
    _lastGpuSample = now;
  }

  // Statusbar at bottom of screen
  {
    float yPos = static_cast<float>(_displayHeight) - STATUSBAR_HEIGHT;
    ImGui::SetNextWindowPos(ImVec2(0, yPos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(
        ImVec2(static_cast<float>(_displayWidth), STATUSBAR_HEIGHT),
        ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.9f));
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;
    if (ImGui::Begin("##Statusbar", nullptr, flags)) {
      // Left side: status text
      if (_statusText.empty()) {
        ImGui::Text("Ready");
      } else {
        ImGui::Text("%s", _statusText.c_str());
      }

      // Right side: FPS
      if (_fps > 0) {
        char fpsText[32];
        snprintf(fpsText, sizeof(fpsText), "%u FPS", _fps);
        float textWidth = ImGui::CalcTextSize(fpsText).x;
        float windowWidth = ImGui::GetWindowWidth();
        ImGui::SameLine(windowWidth - textWidth - 8.0f);
        ImGui::Text("%s", fpsText);
      }

      // Right side: GPU usage
      if (0 and _gpuUsageStats.available) {
        // Calculate position for right-aligned GPU text
        char gpuText[64];
        snprintf(gpuText, sizeof(gpuText), "GPU: %.0f%%",
                 _gpuUsageStats.utilization);
        float textWidth = ImGui::CalcTextSize(gpuText).x;
        float windowWidth = ImGui::GetWindowWidth();
        float rightPadding = 8.0f;

        ImGui::SameLine(windowWidth - textWidth - rightPadding);

        // Color based on utilization: green < 50%, yellow 50-80%, red > 80%
        ImVec4 color;
        if (_gpuUsageStats.utilization < 50.0f) {
          color = ImVec4(0.4f, 0.9f, 0.4f, 1.0f); // Green
        } else if (_gpuUsageStats.utilization < 80.0f) {
          color = ImVec4(0.9f, 0.9f, 0.4f, 1.0f); // Yellow
        } else {
          color = ImVec4(0.9f, 0.4f, 0.4f, 1.0f); // Red
        }
        ImGui::TextColored(color, "%s", gpuText);
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
  wgpuRenderPassEncoderSetScissorRect(pass, 0, 0, _displayWidth,
                                      _displayHeight);

  return Ok();
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------

Result<ImguiManager::Ptr>
ImguiManager::createImpl(ContextType & /*ctx*/,
                         const YettyContext &yettyCtx) noexcept {
  auto impl = Ptr(new ImguiManagerImpl(yettyCtx));
  if (auto res = static_cast<ImguiManagerImpl *>(impl.get())->init(); !res) {
    return Err<Ptr>("ImguiManager init failed", res);
  }
  static_cast<ImguiManagerImpl *>(impl.get())->registerForEvents();
  return Ok(std::move(impl));
}

} // namespace yetty
