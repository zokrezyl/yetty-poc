#pragma once

#include <yetty/result.hpp>
#include <yetty/webgpu-context.h>
#include <yetty/yetty-font-manager.h>
#include <yetty/bm-font.h>
#include <yetty/shader-font.h>
#include <yetty/workspace.h>
#include <yetty/card-buffer-manager.h>
#include <memory>
#include <webgpu/webgpu.h>

#if !defined(__ANDROID__)
#include <GLFW/glfw3.h>
#endif

#if !YETTY_WEB && !defined(__ANDROID__)
#include <uv.h>
#endif

namespace yetty {

class Config;

class NewYetty : public std::enable_shared_from_this<NewYetty> {
public:
    using Ptr = std::shared_ptr<NewYetty>;

    ~NewYetty();

    static Result<Ptr> create(int argc, char* argv[]) noexcept;

    int run() noexcept;

    // Accessors for shared resources
    WebGPUContext::Ptr context() const noexcept { return ctx_; }
    YettyFontManager::Ptr fontManager() const noexcept { return fontManager_; }
    BmFont::Ptr bitmapFont() const noexcept { return bitmapFont_; }
    ShaderFont::Ptr shaderGlyphFont() const noexcept { return shaderGlyphFont_; }
    ShaderFont::Ptr cardFont() const noexcept { return cardFont_; }
    // Workspace management
    const std::vector<Workspace::Ptr>& workspaces() const noexcept { return workspaces_; }
    Workspace::Ptr activeWorkspace() const noexcept { return activeWorkspace_; }
    size_t activeWorkspaceIndex() const noexcept { return activeWorkspaceIndex_; }
    Result<Workspace::Ptr> createWorkspace() noexcept;
    Result<void> switchWorkspace(size_t index) noexcept;

    // Window info
    GLFWwindow* window() const noexcept { return window_; }
    uint32_t windowWidth() const noexcept;
    uint32_t windowHeight() const noexcept;
    float cellWidth() const noexcept { return baseCellWidth_ * zoomLevel_; }
    float cellHeight() const noexcept { return baseCellHeight_ * zoomLevel_; }
    uint32_t cols() const noexcept { return cols_; }
    uint32_t rows() const noexcept { return rows_; }

    // Shared bind group for all views
    WGPUBindGroupLayout sharedBindGroupLayout() const noexcept { return sharedBindGroupLayout_; }
    WGPUBindGroup sharedBindGroup() const noexcept { return sharedBindGroup_; }

#if !YETTY_WEB && !defined(__ANDROID__)
    uv_loop_t* getLoop() const noexcept { return uvLoop_; }
    CardBufferManager* cardBufferManager() const noexcept { return cardBufferManager_.get(); }
#endif

private:
    NewYetty() noexcept = default;

    Result<void> init(int argc, char* argv[]) noexcept;
    Result<void> parseArgs(int argc, char* argv[]) noexcept;
    Result<void> initWindow() noexcept;
    Result<void> initGraphics() noexcept;
    Result<void> initFonts() noexcept;
    Result<void> initRenderer() noexcept;
    Result<void> initWorkspace() noexcept;
    Result<void> initCallbacks() noexcept;
    void shutdown() noexcept;

    void mainLoopIteration() noexcept;
    void handleResize(int width, int height) noexcept;

#if !YETTY_WEB && !defined(__ANDROID__)
    void initEventLoop() noexcept;
    void shutdownEventLoop() noexcept;
    static void onFrameTimer(uv_timer_t* handle);
#endif

    // Core components
    std::shared_ptr<Config> config_;
    WebGPUContext::Ptr ctx_;
    YettyFontManager::Ptr fontManager_;

    // Font instances
    BmFont::Ptr bitmapFont_;
    ShaderFont::Ptr shaderGlyphFont_;
    ShaderFont::Ptr cardFont_;


    // Workspaces (like tmux sessions)
    std::vector<Workspace::Ptr> workspaces_;
    Workspace::Ptr activeWorkspace_;
    size_t activeWorkspaceIndex_ = 0;

#if !YETTY_WEB && !defined(__ANDROID__)
    // Card buffer manager for GPU buffers (images, plots, etc.)
    std::unique_ptr<CardBufferManager> cardBufferManager_;
#endif

    // Shared uniforms for all renderers
    struct SharedUniforms {
        float time;
        float deltaTime;
        float screenWidth;
        float screenHeight;
        float mouseX;
        float mouseY;
        float _pad1;
        float _pad2;
    };
    WGPUBuffer sharedUniformBuffer_ = nullptr;
    WGPUBindGroupLayout sharedBindGroupLayout_ = nullptr;
    WGPUBindGroup sharedBindGroup_ = nullptr;
    SharedUniforms sharedUniforms_ = {};

    // Window
    GLFWwindow* window_ = nullptr;

    // Grid dimensions
    uint32_t cols_ = 80;
    uint32_t rows_ = 24;
    float baseCellWidth_ = 0.0f;
    float baseCellHeight_ = 0.0f;
    float zoomLevel_ = 0.5f;

    // Command line options
    std::string fontPath_;
    std::string executeCommand_;
    uint32_t initialWidth_ = 1024;
    uint32_t initialHeight_ = 768;

#if !YETTY_WEB && !defined(__ANDROID__)
    uv_loop_t* uvLoop_ = nullptr;
    uv_timer_t* frameTimer_ = nullptr;
    double lastRenderTime_ = 0.0;
#endif

    // FPS tracking
    double lastFpsTime_ = 0.0;
    uint32_t frameCount_ = 0;

    static NewYetty* s_instance;
};

} // namespace yetty
