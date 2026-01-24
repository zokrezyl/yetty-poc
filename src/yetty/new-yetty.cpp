#include <yetty/new-yetty.h>
#include <yetty/terminal-view.h>
#include <yetty/wgpu-compat.h>
#include "base/base.h"
#include <array>
#include <iostream>
#include <ytrace/ytrace.hpp>

namespace yetty {

NewYetty* NewYetty::s_instance = nullptr;

NewYetty::~NewYetty() {
    shutdown();
}

Result<NewYetty::Ptr> NewYetty::create(int argc, char* argv[]) noexcept {
    auto p = Ptr(new NewYetty());
    if (auto res = p->init(argc, argv); !res) {
        return Err<Ptr>("Failed to init NewYetty", res);
    }
    return Ok(p);
}

Result<void> NewYetty::init(int argc, char* argv[]) noexcept {
    yinfo("NewYetty starting...");

    if (auto res = parseArgs(argc, argv); !res) {
        return res;
    }

    if (auto res = initWindow(); !res) {
        return res;
    }

    if (auto res = initGraphics(); !res) {
        return res;
    }

    if (auto res = initFonts(); !res) {
        return res;
    }

#if !YETTY_WEB && !defined(__ANDROID__)
    initEventLoop();
#endif

    if (auto res = initWorkspace(); !res) {
        return res;
    }

    if (auto res = initCallbacks(); !res) {
        return res;
    }

    s_instance = this;
    lastFpsTime_ = glfwGetTime();

    return Ok();
}

Result<void> NewYetty::parseArgs(int argc, char* argv[]) noexcept {
    (void)argc;
    (void)argv;
    initialWidth_ = 1024;
    initialHeight_ = 768;
    return Ok();
}

Result<void> NewYetty::initWindow() noexcept {
    if (!glfwInit()) {
        return Err<void>("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(initialWidth_, initialHeight_,
                               "new-yetty", nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        return Err<void>("Failed to create window");
    }

    return Ok();
}

Result<void> NewYetty::initGraphics() noexcept {
    auto ctxResult = WebGPUContext::create(window_, initialWidth_, initialHeight_);
    if (!ctxResult) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        return Err<void>("Failed to initialize WebGPU", ctxResult);
    }
    ctx_ = *ctxResult;

#if !YETTY_WEB && !defined(__ANDROID__)
    // Create CardBufferManager for card-based widgets (plots, images, etc.)
    auto cbmResult = CardBufferManager::create(ctx_->getDevice());
    if (!cbmResult) {
        return Err<void>("Failed to create CardBufferManager", cbmResult);
    }
    cardBufferManager_ = std::move(*cbmResult);
    yinfo("CardBufferManager created");

    // Initialize the atlas (compute shader pipelines and textures)
    if (auto res = cardBufferManager_->initAtlas(); !res) {
        return Err<void>("Failed to initialize card atlas", res);
    }
    yinfo("Card image atlas initialized");
#endif

    // Create shared uniform buffer
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("Shared Uniforms");
    bufDesc.size = sizeof(SharedUniforms);
    bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    bufDesc.mappedAtCreation = false;
    sharedUniformBuffer_ = wgpuDeviceCreateBuffer(ctx_->getDevice(), &bufDesc);

#if !YETTY_WEB && !defined(__ANDROID__)
    // Create bind group layout for shared uniforms + card buffers + atlas (group 0)
    std::array<WGPUBindGroupLayoutEntry, 6> layoutEntries = {};

    layoutEntries[0].binding = 0;
    layoutEntries[0].visibility = WGPUShaderStage_Fragment;
    layoutEntries[0].buffer.type = WGPUBufferBindingType_Uniform;
    layoutEntries[0].buffer.minBindingSize = sizeof(SharedUniforms);

    layoutEntries[1].binding = 1;
    layoutEntries[1].visibility = WGPUShaderStage_Fragment;
    layoutEntries[1].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
    layoutEntries[1].buffer.minBindingSize = 0;

    layoutEntries[2].binding = 2;
    layoutEntries[2].visibility = WGPUShaderStage_Fragment;
    layoutEntries[2].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
    layoutEntries[2].buffer.minBindingSize = 0;

    layoutEntries[3].binding = 3;
    layoutEntries[3].visibility = WGPUShaderStage_Fragment;
    layoutEntries[3].texture.sampleType = WGPUTextureSampleType_Float;
    layoutEntries[3].texture.viewDimension = WGPUTextureViewDimension_2D;

    layoutEntries[4].binding = 4;
    layoutEntries[4].visibility = WGPUShaderStage_Fragment;
    layoutEntries[4].sampler.type = WGPUSamplerBindingType_Filtering;

    layoutEntries[5].binding = 5;
    layoutEntries[5].visibility = WGPUShaderStage_Fragment;
    layoutEntries[5].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
    layoutEntries[5].buffer.minBindingSize = 0;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.label = WGPU_STR("Shared Bind Group Layout");
    layoutDesc.entryCount = layoutEntries.size();
    layoutDesc.entries = layoutEntries.data();
    sharedBindGroupLayout_ = wgpuDeviceCreateBindGroupLayout(ctx_->getDevice(), &layoutDesc);

    // Create shared bind group
    std::array<WGPUBindGroupEntry, 6> bindEntries = {};

    bindEntries[0].binding = 0;
    bindEntries[0].buffer = sharedUniformBuffer_;
    bindEntries[0].size = sizeof(SharedUniforms);

    bindEntries[1].binding = 1;
    bindEntries[1].buffer = cardBufferManager_->metadataBuffer();
    bindEntries[1].size = wgpuBufferGetSize(cardBufferManager_->metadataBuffer());

    bindEntries[2].binding = 2;
    bindEntries[2].buffer = cardBufferManager_->storageBuffer();
    bindEntries[2].size = wgpuBufferGetSize(cardBufferManager_->storageBuffer());

    bindEntries[3].binding = 3;
    bindEntries[3].textureView = cardBufferManager_->atlasTextureView();

    bindEntries[4].binding = 4;
    bindEntries[4].sampler = cardBufferManager_->atlasSampler();

    bindEntries[5].binding = 5;
    bindEntries[5].buffer = cardBufferManager_->imageDataBuffer();
    bindEntries[5].size = wgpuBufferGetSize(cardBufferManager_->imageDataBuffer());

    WGPUBindGroupDescriptor bindDesc = {};
    bindDesc.label = WGPU_STR("Shared Bind Group");
    bindDesc.layout = sharedBindGroupLayout_;
    bindDesc.entryCount = bindEntries.size();
    bindDesc.entries = bindEntries.data();
    sharedBindGroup_ = wgpuDeviceCreateBindGroup(ctx_->getDevice(), &bindDesc);
#else
    // Web/Android: only shared uniforms
    WGPUBindGroupLayoutEntry layoutEntry = {};
    layoutEntry.binding = 0;
    layoutEntry.visibility = WGPUShaderStage_Fragment;
    layoutEntry.buffer.type = WGPUBufferBindingType_Uniform;
    layoutEntry.buffer.minBindingSize = sizeof(SharedUniforms);

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 1;
    layoutDesc.entries = &layoutEntry;
    sharedBindGroupLayout_ = wgpuDeviceCreateBindGroupLayout(ctx_->getDevice(), &layoutDesc);

    WGPUBindGroupEntry bindEntry = {};
    bindEntry.binding = 0;
    bindEntry.buffer = sharedUniformBuffer_;
    bindEntry.size = sizeof(SharedUniforms);

    WGPUBindGroupDescriptor bindDesc = {};
    bindDesc.layout = sharedBindGroupLayout_;
    bindDesc.entryCount = 1;
    bindDesc.entries = &bindEntry;
    sharedBindGroup_ = wgpuDeviceCreateBindGroup(ctx_->getDevice(), &bindDesc);
#endif

    return Ok();
}

Result<void> NewYetty::initFonts() noexcept {
    auto yfmRes = YettyFontManager::create();
    if (!yfmRes) {
        return Err<void>("Failed to create YettyFontManager", yfmRes);
    }
    fontManager_ = *yfmRes;

    // Create BmFont for bitmap/emoji rendering
    {
        auto bmFontResult = BmFont::create(ctx_->getDevice());
        if (!bmFontResult) {
            ywarn("Failed to create BmFont: {} - emoji rendering disabled",
                  bmFontResult.error().message());
        } else {
            bitmapFont_ = *bmFontResult;
            if (auto res = bitmapFont_->loadCommonGlyphs(); !res) {
                ywarn("Failed to load common glyphs: {}", res.error().message());
            }
            yinfo("BmFont created successfully");
        }
    }

    // Create ShaderFont for shader glyphs
    {
        std::string shaderDir = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/";
        auto shaderGlyphResult = ShaderFont::create(ShaderFont::Category::Glyph, shaderDir);
        if (!shaderGlyphResult) {
            ywarn("Failed to create ShaderFont for glyphs: {} - shader glyphs disabled",
                  shaderGlyphResult.error().message());
        } else {
            shaderGlyphFont_ = *shaderGlyphResult;
            yinfo("ShaderFont for glyphs created with {} shaders",
                  shaderGlyphFont_->getFunctionCount());
        }
    }

    // Create ShaderFont for cards
    {
        std::string shaderDir = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/";
        auto cardFontResult = ShaderFont::create(ShaderFont::Category::Card, shaderDir);
        if (!cardFontResult) {
            ywarn("Failed to create ShaderFont for cards: {} - card glyphs disabled",
                  cardFontResult.error().message());
        } else {
            cardFont_ = *cardFontResult;
            yinfo("ShaderFont for cards created with {} shaders",
                  cardFont_->getFunctionCount());
        }
    }

    // Get cell size from default font
    auto defaultFont = fontManager_->getDefaultFont();
    if (defaultFont) {
        baseCellWidth_ = 10.0f;
        baseCellHeight_ = 20.0f;
    }

    cols_ = static_cast<uint32_t>(initialWidth_ / (baseCellWidth_ * zoomLevel_));
    rows_ = static_cast<uint32_t>(initialHeight_ / (baseCellHeight_ * zoomLevel_));

    return Ok();
}

Result<void> NewYetty::initWorkspace() noexcept {
    // Create the first default workspace
    auto wsResult = createWorkspace();
    if (!wsResult) {
        return Err<void>("Failed to create default workspace", wsResult);
    }

    yinfo("Created default workspace with TerminalView: {} cols x {} rows", cols_, rows_);
    return Ok();
}

Result<Workspace::Ptr> NewYetty::createWorkspace() noexcept {
    float width = static_cast<float>(initialWidth_);
    float height = static_cast<float>(initialHeight_);

    auto wsResult = Workspace::create(
        width, height, *ctx_, fontManager_, bitmapFont_,
        sharedBindGroupLayout_, sharedBindGroup_
#if !YETTY_WEB && !defined(__ANDROID__)
        , uvLoop_
#endif
    );
    if (!wsResult) {
        return Err<Workspace::Ptr>("Failed to create Workspace", wsResult);
    }
    auto workspace = *wsResult;

#if !YETTY_WEB && !defined(__ANDROID__)
    // Create 2x2 split layout:
    // Root: vertical split (left | right)
    // Left: horizontal split (top-left / bottom-left)
    // Right: horizontal split (top-right / bottom-right)

    auto rootSplitResult = Split::create(Orientation::Vertical);
    if (!rootSplitResult) {
        return Err<Workspace::Ptr>("Failed to create root split", rootSplitResult);
    }
    auto rootSplit = *rootSplitResult;

    auto leftSplitResult = Split::create(Orientation::Horizontal);
    if (!leftSplitResult) {
        return Err<Workspace::Ptr>("Failed to create left split", leftSplitResult);
    }
    auto leftSplit = *leftSplitResult;

    auto rightSplitResult = Split::create(Orientation::Horizontal);
    if (!rightSplitResult) {
        return Err<Workspace::Ptr>("Failed to create right split", rightSplitResult);
    }
    auto rightSplit = *rightSplitResult;

    // Create 4 panes with default TerminalViews
    std::array<Pane::Ptr, 4> panes;
    for (int i = 0; i < 4; i++) {
        auto paneResult = workspace->createPane();
        if (!paneResult) {
            return Err<Workspace::Ptr>("Failed to create pane", paneResult);
        }
        panes[i] = *paneResult;
    }

    // Assemble the tree
    leftSplit->setFirst(panes[0]);   // top-left
    leftSplit->setSecond(panes[1]);  // bottom-left
    rightSplit->setFirst(panes[2]);  // top-right
    rightSplit->setSecond(panes[3]); // bottom-right
    rootSplit->setFirst(leftSplit);
    rootSplit->setSecond(rightSplit);

    // Set root - this will propagate bounds
    workspace->setRoot(rootSplit);

    yinfo("Created 2x2 split workspace with 4 terminal views");
#endif

    workspaces_.push_back(workspace);

    // If this is the first workspace, make it active
    if (workspaces_.size() == 1) {
        activeWorkspace_ = workspace;
        activeWorkspaceIndex_ = 0;
    }

    return Ok(workspace);
}

Result<void> NewYetty::switchWorkspace(size_t index) noexcept {
    if (index >= workspaces_.size()) {
        return Err<void>("Workspace index out of range");
    }
    activeWorkspaceIndex_ = index;
    activeWorkspace_ = workspaces_[index];
    return Ok();
}

Result<void> NewYetty::initCallbacks() noexcept {
    glfwSetWindowUserPointer(window_, this);

    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        (void)w;
        (void)scancode;
        auto loop = base::EventLoop::instance();
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            loop->dispatch(base::Event::keyDown(key, mods));
        } else if (action == GLFW_RELEASE) {
            loop->dispatch(base::Event::keyUp(key, mods));
        }
    });

    glfwSetCharCallback(window_, [](GLFWwindow* w, unsigned int codepoint) {
        (void)w;
        auto loop = base::EventLoop::instance();
        loop->dispatch(base::Event::charInput(codepoint));
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* w, int button, int action, int mods) {
        (void)mods;
        auto loop = base::EventLoop::instance();
        double x, y;
        glfwGetCursorPos(w, &x, &y);
        if (action == GLFW_PRESS) {
            loop->dispatch(base::Event::mouseDown(static_cast<float>(x), static_cast<float>(y), button));
        } else if (action == GLFW_RELEASE) {
            loop->dispatch(base::Event::mouseUp(static_cast<float>(x), static_cast<float>(y), button));
        }
    });

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* w, int newWidth, int newHeight) {
        auto* engine = static_cast<NewYetty*>(glfwGetWindowUserPointer(w));
        if (engine) {
            engine->handleResize(newWidth, newHeight);
        }
    });

    return Ok();
}

void NewYetty::shutdown() noexcept {
#if !YETTY_WEB && !defined(__ANDROID__)
    shutdownEventLoop();
#endif

    activeWorkspace_.reset();
    workspaces_.clear();
    fontManager_.reset();
    ctx_.reset();

    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();

    s_instance = nullptr;
}

#if !YETTY_WEB && !defined(__ANDROID__)
void NewYetty::initEventLoop() noexcept {
    uvLoop_ = new uv_loop_t;
    uv_loop_init(uvLoop_);

    frameTimer_ = new uv_timer_t;
    uv_timer_init(uvLoop_, frameTimer_);
    frameTimer_->data = this;

    lastRenderTime_ = glfwGetTime();
}

void NewYetty::shutdownEventLoop() noexcept {
    if (frameTimer_) {
        uv_timer_stop(frameTimer_);
        uv_close(reinterpret_cast<uv_handle_t*>(frameTimer_), nullptr);
    }
    if (uvLoop_) {
        uv_run(uvLoop_, UV_RUN_NOWAIT);
        uv_loop_close(uvLoop_);
        delete uvLoop_;
        uvLoop_ = nullptr;
    }
    if (frameTimer_) {
        delete frameTimer_;
        frameTimer_ = nullptr;
    }
}

void NewYetty::onFrameTimer(uv_timer_t* handle) {
    NewYetty* self = static_cast<NewYetty*>(handle->data);

    glfwPollEvents();

    if (glfwWindowShouldClose(self->window_)) {
        uv_stop(self->uvLoop_);
        return;
    }

    self->mainLoopIteration();
}
#endif

int NewYetty::run() noexcept {
    yinfo("Starting render loop...");

#if !YETTY_WEB && !defined(__ANDROID__)
    uv_timer_start(frameTimer_, onFrameTimer, 0, 16); // ~60Hz
    uv_run(uvLoop_, UV_RUN_DEFAULT);
#endif

    yinfo("Shutting down...");
    return 0;
}

void NewYetty::mainLoopIteration() noexcept {
    // Get texture view
    auto viewResult = ctx_->getCurrentTextureView();
    if (!viewResult) {
        return;
    }
    WGPUTextureView targetView = *viewResult;

    // Create command encoder
    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(ctx_->getDevice(), &encoderDesc);
    if (!encoder) {
        return;
    }

    // Render pass - just clear for now
    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = targetView;
    colorAttachment.loadOp = WGPULoadOp_Clear;
    colorAttachment.storeOp = WGPUStoreOp_Store;
    colorAttachment.clearValue = {0.1f, 0.1f, 0.2f, 1.0f}; // Dark blue
    colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    if (pass) {
        if (activeWorkspace_) {
            activeWorkspace_->render(pass, *ctx_);
        }
        wgpuRenderPassEncoderEnd(pass);
        wgpuRenderPassEncoderRelease(pass);
    }

    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    if (cmdBuffer) {
        wgpuQueueSubmit(ctx_->getQueue(), 1, &cmdBuffer);
        wgpuCommandBufferRelease(cmdBuffer);
    }
    wgpuCommandEncoderRelease(encoder);

    // Present
    if (ctx_->hasCurrentTexture()) {
        ctx_->present();
    }

    // FPS counter
    frameCount_++;
    double now = glfwGetTime();
    if (now - lastFpsTime_ >= 1.0) {
        yinfo("FPS: {}", frameCount_);
        frameCount_ = 0;
        lastFpsTime_ = now;
    }
}

void NewYetty::handleResize(int newWidth, int newHeight) noexcept {
    if (newWidth == 0 || newHeight == 0)
        return;

    if (ctx_) {
        ctx_->resize(static_cast<uint32_t>(newWidth), static_cast<uint32_t>(newHeight));
    }

    cols_ = static_cast<uint32_t>(newWidth / cellWidth());
    rows_ = static_cast<uint32_t>(newHeight / cellHeight());

    // Resize active workspace - this propagates to tile tree and views
    if (activeWorkspace_) {
        activeWorkspace_->resize(static_cast<float>(newWidth), static_cast<float>(newHeight));
    }
}

uint32_t NewYetty::windowWidth() const noexcept {
    int w, h;
    glfwGetFramebufferSize(window_, &w, &h);
    return static_cast<uint32_t>(w);
}

uint32_t NewYetty::windowHeight() const noexcept {
    int w, h;
    glfwGetFramebufferSize(window_, &w, &h);
    return static_cast<uint32_t>(h);
}

} // namespace yetty
