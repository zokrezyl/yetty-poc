#include "ymery.h"
#include <yetty/yetty-context.h>
#include <yetty/base/event-loop.h>
#include "../../ymery/lang.h"
#include "../../ymery/renderer.h"
#include "../../ymery/dispatcher.h"
#include "../../ymery/simple-data-tree.h"
#include <ytrace/ytrace.hpp>

#include <imgui.h>
#include <imgui_impl_wgpu.h>

#include <filesystem>
#include <sstream>
#include <cstring>
#include <yetty/wgpu-compat.h>

namespace fs = std::filesystem;

namespace yetty::card {

//=============================================================================
// YmeryImpl
//=============================================================================

class YmeryImpl : public Ymery {
public:
    YmeryImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : Ymery(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _ctx(ctx)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~YmeryImpl() override {
        dispose();
    }

    const char* typeName() const override { return "ymery"; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    void setCellSize(float cellWidth, float cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            _needsResize = true;
            _dirty = true;
        }
    }

    //=========================================================================
    // Init
    //=========================================================================

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("Ymery::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        // Parse args for -i/--input flag
        parseArgs(_argsStr);

        // Load YAML via Lang
        if (_inputSource == "-") {
            // Inline content: payload contains YAML data
            if (_payloadStr.empty()) {
                return Err<void>("Ymery::init: empty payload (use -i - to read from payload)");
            }
            auto langRes = ymery::Lang::createFromString(_payloadStr);
            if (!langRes) {
                return Err<void>("Ymery::init: failed to load inline YAML", langRes);
            }
            _lang = *langRes;
            yinfo("Ymery::init: loaded {} bytes of inline YAML", _payloadStr.size());
        } else {
            // File path: from -i <path> or legacy payload-as-path
            std::string pathStr = !_inputSource.empty() ? _inputSource : _payloadStr;
            if (pathStr.empty()) {
                return Err<void>("Ymery::init: no input specified (use -i - or -i <path>)");
            }

            fs::path inputPath = fs::absolute(pathStr);
            if (!fs::exists(inputPath)) {
                return Err<void>("Ymery::init: path not found: " + inputPath.string());
            }

            std::string mainModule = "app";
            std::vector<fs::path> searchPaths;

            if (fs::is_regular_file(inputPath)) {
                mainModule = inputPath.stem().string();
                fs::path moduleDir = inputPath.parent_path();
                fs::path parentDir = moduleDir.parent_path();
                searchPaths = {moduleDir, parentDir};
            } else {
                fs::path parentDir = inputPath.parent_path();
                searchPaths = {inputPath, parentDir};
            }

            yinfo("Ymery::init: searchPaths={}, mainModule='{}'",
                  searchPaths.size(), mainModule);

            auto langRes = ymery::Lang::create(searchPaths, mainModule);
            if (!langRes) {
                return Err<void>("Ymery::init: failed to load YAML", langRes);
            }
            _lang = *langRes;
        }

        // Create dispatcher
        auto dispRes = ymery::Dispatcher::create();
        if (!dispRes) {
            return Err<void>("Ymery::init: failed to create dispatcher");
        }
        _dispatcher = *dispRes;

        // Create data tree
        auto treeRes = ymery::SimpleDataTree::create();
        if (!treeRes) {
            return Err<void>("Ymery::init: failed to create data tree");
        }
        _dataTree = *treeRes;

        // Create renderer with external trees (config from YettyContext)
        std::map<std::string, ymery::TreeLikePtr> externalTrees;
        if (_ctx.config) {
            externalTrees["config"] = _ctx.config;
        }
        auto rendererRes = ymery::Renderer::create(_lang, _dispatcher, _dataTree, std::move(externalTrees));
        if (!rendererRes) {
            return Err<void>("Ymery::init: failed to create renderer", rendererRes);
        }
        _renderer = *rendererRes;

        // Create ImGui context for offscreen rendering
        _prevContext = ImGui::GetCurrentContext();
        _imguiContext = ImGui::CreateContext();
        if (!_imguiContext) {
            return Err<void>("Ymery::init: failed to create ImGui context");
        }
        ImGui::SetCurrentContext(_imguiContext);

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;
        ImGui::StyleColorsDark();

        ImGui_ImplWGPU_InitInfo wgpuInfo = {};
        wgpuInfo.Device = _gpu.device;
        wgpuInfo.NumFramesInFlight = 3;
        wgpuInfo.RenderTargetFormat = WGPUTextureFormat_RGBA8Unorm;
        wgpuInfo.DepthStencilFormat = WGPUTextureFormat_Undefined;

        if (!ImGui_ImplWGPU_Init(&wgpuInfo)) {
            ImGui::DestroyContext(_imguiContext);
            _imguiContext = nullptr;
            ImGui::SetCurrentContext(_prevContext);
            return Err<void>("Ymery::init: failed to init ImGui WGPU backend");
        }

        // Restore previous context
        ImGui::SetCurrentContext(_prevContext);
        _prevContext = nullptr;

        // Offscreen texture, texture handle, and pixel buffer will be created
        // in allocateTextures() after setCellSize() provides correct dimensions.
        _dirty = true;
        _needsResize = true;
        _metadataDirty = true;

        // Register for events
        if (auto res = _registerForEvents(); !res) {
            return Err<void>("Ymery::init: failed to register for events", res);
        }

        yinfo("Ymery::init: ready ({}x{} cells, {}x{} pixels)",
              _widthCells, _heightCells, _pixelWidth, _pixelHeight);

        return Ok();
    }

    //=========================================================================
    // Suspend (dormancy)
    //=========================================================================

    void suspend() override {
        _destroyOffscreenTexture();
        _destroyReadbackBuffer();

        _cpuPixels.clear();

        yinfo("Ymery::suspend: deallocated offscreen texture + texture handle");
    }

    //=========================================================================
    // Dispose
    //=========================================================================

    Result<void> dispose() override {
        _deregisterFromEvents();

        if (_imguiContext) {
            auto prev = ImGui::GetCurrentContext();
            ImGui::SetCurrentContext(_imguiContext);
            ImGui_ImplWGPU_Shutdown();
            ImGui::DestroyContext(_imguiContext);
            _imguiContext = nullptr;
            if (prev != _imguiContext) {
                ImGui::SetCurrentContext(prev);
            }
        }

        _destroyOffscreenTexture();
        _destroyReadbackBuffer();

        _cpuPixels.clear();
        _cpuPixels.shrink_to_fit();

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        _renderer.reset();
        _lang.reset();
        _dispatcher.reset();
        _dataTree.reset();

        return Ok();
    }

    //=========================================================================
    // Update (per-frame)
    //=========================================================================

    Result<void> allocateTextures() override {
        _textureHandle = TextureHandle::invalid();
        // Recompute pixel dimensions if cell size changed
        if (_needsResize) {
            uint32_t newW = _widthCells * _cellWidth;
            uint32_t newH = _heightCells * _cellHeight;
            if (newW != _pixelWidth || newH != _pixelHeight) {
                _pixelWidth = newW;
                _pixelHeight = newH;
                _destroyOffscreenTexture();
                _destroyReadbackBuffer();
                _cpuPixels.clear();
            }
            _needsResize = false;
            _dirty = true;
        }

        if (_pixelWidth > 0 && _pixelHeight > 0) {
            if (!_offscreenTexture) {
                if (auto res = _createOffscreenTexture(); !res) {
                    return Err<void>("Ymery::allocateTextures: failed to recreate offscreen texture", res);
                }
            }
            if (!_textureHandle.isValid()) {
                auto texResult = _cardMgr->textureManager()->allocate(_pixelWidth, _pixelHeight);
                if (!texResult) {
                    return Err<void>("Ymery::allocateTextures: failed to allocate texture handle", texResult);
                }
                _textureHandle = *texResult;
            }
            if (_cpuPixels.empty()) {
                _cpuPixels.resize(static_cast<size_t>(_pixelWidth) * _pixelHeight * 4);
                _dirty = true;
            }
            _metadataDirty = true;
        }
        return Ok();
    }

    Result<void> writeTextures() override {
        if (_textureHandle.isValid() && !_cpuPixels.empty()) {
            if (auto res = _cardMgr->textureManager()->write(_textureHandle, _cpuPixels.data()); !res) {
                return Err<void>("Ymery::writeTextures: write failed", res);
            }
        }
        return Ok();
    }

    void renderToStaging(float time) override {
        (void)time;

        if (!_imguiContext || !_renderer) return;

        // First few frames: always render (ImGui needs frames to init font atlas)
        if (_initFrames > 0) {
            --_initFrames;
        } else if (!_dirty && !_metadataDirty) {
            return;
        }

        if (_pixelWidth == 0 || _pixelHeight == 0) return;

        // Save current ImGui context and switch to ours
        ImGuiContext* prevCtx = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(_imguiContext);

        // ImGui frame
        ImGui_ImplWGPU_NewFrame();
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(_pixelWidth), static_cast<float>(_pixelHeight));
        io.DeltaTime = 1.0f / 60.0f;
        ImGui::NewFrame();

        // Force root window to fill the card
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(_pixelWidth),
                                        static_cast<float>(_pixelHeight)), ImGuiCond_Always);

        // Render ymery widgets
        if (auto res = _renderer->renderFrame(); !res) {
            ydebug("Ymery::renderToStaging: renderFrame failed: {}", res.error().to_string());
        }

        ImGui::Render();

        // Create command encoder
        WGPUCommandEncoderDescriptor encDesc = {};
        encDesc.label = WGPU_STR("YmeryEncoder");
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_gpu.device, &encDesc);

        // Begin render pass targeting offscreen texture
        WGPURenderPassColorAttachment colorAttach = {};
        colorAttach.view = _offscreenView;
        colorAttach.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
        colorAttach.loadOp = WGPULoadOp_Clear;
        colorAttach.storeOp = WGPUStoreOp_Store;
        colorAttach.clearValue = {0.1, 0.1, 0.1, 1.0};

        WGPURenderPassDescriptor passDesc = {};
        passDesc.label = WGPU_STR("YmeryRenderPass");
        passDesc.colorAttachmentCount = 1;
        passDesc.colorAttachments = &colorAttach;

        WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
        ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), renderPass);
        wgpuRenderPassEncoderEnd(renderPass);
        wgpuRenderPassEncoderRelease(renderPass);

        // Copy offscreen texture → readback buffer for CPU access
        uint32_t bytesPerRow = _pixelWidth * 4;
        uint32_t alignedBytesPerRow = (bytesPerRow + 255) & ~255u;
        uint32_t readbackSize = alignedBytesPerRow * _pixelHeight;

        if (!_readbackBuffer || _readbackSize != readbackSize) {
            _destroyReadbackBuffer();
            WGPUBufferDescriptor rbDesc = {};
            rbDesc.label = WGPU_STR("YmeryReadback");
            rbDesc.size = readbackSize;
            rbDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
            _readbackBuffer = wgpuDeviceCreateBuffer(_gpu.device, &rbDesc);
            _readbackSize = readbackSize;
        }

        WGPUTexelCopyTextureInfo src = {};
        src.texture = _offscreenTexture;

        WGPUTexelCopyBufferInfo dst = {};
        dst.buffer = _readbackBuffer;
        dst.layout.offset = 0;
        dst.layout.bytesPerRow = alignedBytesPerRow;
        dst.layout.rowsPerImage = _pixelHeight;

        WGPUExtent3D copySize = {_pixelWidth, _pixelHeight, 1};
        wgpuCommandEncoderCopyTextureToBuffer(encoder, &src, &dst, &copySize);

        // Submit
        WGPUCommandBufferDescriptor cmdDesc = {};
        cmdDesc.label = WGPU_STR("YmeryCmd");
        WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
        wgpuCommandEncoderRelease(encoder);
        wgpuQueueSubmit(_gpu.queue, 1, &cmdBuffer);
        wgpuCommandBufferRelease(cmdBuffer);

        // Restore ImGui context
        ImGui::SetCurrentContext(prevCtx);

        // Map readback buffer synchronously → _cpuPixels staging
        struct MapCtx { bool done = false; WGPUMapAsyncStatus status; };
        MapCtx mapCtx;
        WGPUBufferMapCallbackInfo cbInfo = {};
        cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        cbInfo.callback = [](WGPUMapAsyncStatus status, WGPUStringView, void* ud1, void*) {
            auto* ctx = static_cast<MapCtx*>(ud1);
            ctx->status = status;
            ctx->done = true;
        };
        cbInfo.userdata1 = &mapCtx;

        wgpuBufferMapAsync(_readbackBuffer, WGPUMapMode_Read, 0, readbackSize, cbInfo);

        // Poll until map completes
        while (!mapCtx.done) {
            wgpuDeviceTick(_gpu.device);
        }

        if (mapCtx.status == WGPUMapAsyncStatus_Success) {
            const uint8_t* mapped = static_cast<const uint8_t*>(
                wgpuBufferGetConstMappedRange(_readbackBuffer, 0, readbackSize));

            if (mapped && !_cpuPixels.empty()) {
                // Copy rows (strip alignment padding if any)
                for (uint32_t row = 0; row < _pixelHeight; ++row) {
                    std::memcpy(_cpuPixels.data() + row * bytesPerRow,
                                mapped + row * alignedBytesPerRow,
                                bytesPerRow);
                }
            }

            wgpuBufferUnmap(_readbackBuffer);
        } else {
            ydebug("Ymery::renderToStaging: readback map failed (status={})", static_cast<int>(mapCtx.status));
        }

        _dirty = false;
    }

    Result<void> finalize() override {
        // Write CPU pixels to card texture atlas
        if (_textureHandle.isValid() && !_cpuPixels.empty()) {
            if (auto res = _cardMgr->textureManager()->write(_textureHandle, _cpuPixels.data()); !res) {
                return Err<void>("Ymery::render: texture write failed", res);
            }
        }

        if (_metadataDirty) {
            if (auto res = _uploadMetadata(); !res) {
                return Err<void>("Ymery::render: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // Events
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        if (!_imguiContext) return Ok(false);

        // Handle SetFocus — don't consume, all listeners need to see it
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                if (!_focused) {
                    _focused = true;
                    _subscribeCardMouseEvents();
                    yinfo("Ymery::onEvent: focused (id={})", id());
                }
            } else if (_focused) {
                _focused = false;
                _unsubscribeCardMouseEvents();
                yinfo("Ymery::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        // Card mouse events — already have local coordinates from GPUScreen
        if (event.type == base::Event::Type::CardMouseDown) {
            if (event.cardMouse.targetId != id()) return Ok(false);
            ImGuiContext* prev = ImGui::GetCurrentContext();
            ImGui::SetCurrentContext(_imguiContext);
            ImGuiIO& io = ImGui::GetIO();
            ydebug("Ymery {} MouseDown: x={:.1f} y={:.1f} btn={} displaySize=({:.0f},{:.0f})",
                  id(), event.cardMouse.x, event.cardMouse.y, event.cardMouse.button,
                  io.DisplaySize.x, io.DisplaySize.y);
            io.AddMousePosEvent(event.cardMouse.x, event.cardMouse.y);
            io.AddMouseButtonEvent(std::min(event.cardMouse.button, 4), true);
            _dirty = true;
            ImGui::SetCurrentContext(prev);
            return Ok(true);
        }

        if (event.type == base::Event::Type::CardMouseUp) {
            if (event.cardMouse.targetId != id()) return Ok(false);
            ImGuiContext* prev = ImGui::GetCurrentContext();
            ImGui::SetCurrentContext(_imguiContext);
            ImGuiIO& io = ImGui::GetIO();
            io.AddMousePosEvent(event.cardMouse.x, event.cardMouse.y);
            io.AddMouseButtonEvent(std::min(event.cardMouse.button, 4), false);
            _dirty = true;
            ImGui::SetCurrentContext(prev);
            return Ok(true);
        }

        if (event.type == base::Event::Type::CardMouseMove) {
            if (event.cardMouse.targetId != id()) return Ok(false);
            ImGuiContext* prev = ImGui::GetCurrentContext();
            ImGui::SetCurrentContext(_imguiContext);
            ImGuiIO& io = ImGui::GetIO();
            io.AddMousePosEvent(event.cardMouse.x, event.cardMouse.y);
            _dirty = true;
            ImGui::SetCurrentContext(prev);
            return Ok(false); // Don't consume moves
        }

        if (event.type == base::Event::Type::CardScroll) {
            if (event.cardScroll.targetId != id()) return Ok(false);
            ImGuiContext* prev = ImGui::GetCurrentContext();
            ImGui::SetCurrentContext(_imguiContext);
            ImGuiIO& io = ImGui::GetIO();
            io.AddMouseWheelEvent(event.cardScroll.dx, event.cardScroll.dy);
            _dirty = true;
            ImGui::SetCurrentContext(prev);
            return Ok(true);
        }

        return Ok(false);
    }

private:
    //=========================================================================
    // Offscreen texture management
    //=========================================================================

    Result<void> _createOffscreenTexture() {
        if (_pixelWidth == 0 || _pixelHeight == 0) {
            return Err<void>("Ymery: zero-size texture");
        }

        WGPUTextureDescriptor texDesc = {};
        texDesc.label = WGPU_STR("YmeryOffscreen");
        texDesc.size = {_pixelWidth, _pixelHeight, 1};
        texDesc.mipLevelCount = 1;
        texDesc.sampleCount = 1;
        texDesc.dimension = WGPUTextureDimension_2D;
        texDesc.format = WGPUTextureFormat_RGBA8Unorm;
        texDesc.usage = WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_CopySrc;

        _offscreenTexture = wgpuDeviceCreateTexture(_gpu.device, &texDesc);
        if (!_offscreenTexture) {
            return Err<void>("Ymery: failed to create offscreen texture");
        }

        WGPUTextureViewDescriptor viewDesc = {};
        viewDesc.label = WGPU_STR("YmeryOffscreenView");
        viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
        viewDesc.dimension = WGPUTextureViewDimension_2D;
        viewDesc.mipLevelCount = 1;
        viewDesc.arrayLayerCount = 1;

        _offscreenView = wgpuTextureCreateView(_offscreenTexture, &viewDesc);
        if (!_offscreenView) {
            wgpuTextureRelease(_offscreenTexture);
            _offscreenTexture = nullptr;
            return Err<void>("Ymery: failed to create texture view");
        }

        yinfo("Ymery: created offscreen texture {}x{}", _pixelWidth, _pixelHeight);
        return Ok();
    }

    void _destroyOffscreenTexture() {
        if (_offscreenView) {
            wgpuTextureViewRelease(_offscreenView);
            _offscreenView = nullptr;
        }
        if (_offscreenTexture) {
            wgpuTextureRelease(_offscreenTexture);
            _offscreenTexture = nullptr;
        }
    }

    void _destroyReadbackBuffer() {
        if (_readbackBuffer) {
            wgpuBufferRelease(_readbackBuffer);
            _readbackBuffer = nullptr;
            _readbackSize = 0;
        }
    }

    //=========================================================================
    // Event registration
    //=========================================================================

    Result<void> _registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("Ymery: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res)
            return Err<void>("Ymery: failed to register SetFocus", res);

        yinfo("Ymery {} registered for SetFocus (priority 1000)", id());
        return Ok();
    }

    void _subscribeCardMouseEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        loop->registerListener(base::Event::Type::CardMouseDown, self, 1000);
        loop->registerListener(base::Event::Type::CardMouseUp, self, 1000);
        loop->registerListener(base::Event::Type::CardMouseMove, self, 1000);
        loop->registerListener(base::Event::Type::CardScroll, self, 1000);
        yinfo("Ymery {} subscribed to card mouse events", id());
    }

    void _unsubscribeCardMouseEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        loop->deregisterListener(base::Event::Type::CardMouseDown, self);
        loop->deregisterListener(base::Event::Type::CardMouseUp, self);
        loop->deregisterListener(base::Event::Type::CardMouseMove, self);
        loop->deregisterListener(base::Event::Type::CardScroll, self);
        yinfo("Ymery {} unsubscribed from card mouse events", id());
    }

    void _deregisterFromEvents() {
        if (weak_from_this().expired()) return;
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;
        loop->deregisterListener(sharedAs<base::EventListener>());
    }

    //=========================================================================
    // Args parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--input" || token == "-i") {
                std::string val;
                if (iss >> val) {
                    _inputSource = val;
                }
            }
        }
    }

    //=========================================================================
    // Metadata
    //=========================================================================

    struct Metadata {
        uint32_t textureDataOffset;
        uint32_t textureWidth;
        uint32_t textureHeight;
        uint32_t atlasX;
        uint32_t atlasY;
        uint32_t widthCells;
        uint32_t heightCells;
        float zoom;
        float centerX;
        float centerY;
        uint32_t flags;
        uint32_t bgColor;
        uint32_t scaledWidth;
        uint32_t scaledHeight;
        uint32_t _reserved[2];
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    Result<void> _uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("Ymery: invalid metadata handle");
        }

        Metadata meta = {};
        meta.textureDataOffset = 0;  // No longer used
        meta.textureWidth = _pixelWidth;
        meta.textureHeight = _pixelHeight;

        if (_textureHandle.isValid()) {
            auto pos = _cardMgr->textureManager()->getAtlasPosition(_textureHandle);
            meta.atlasX = pos.x;
            meta.atlasY = pos.y;
        } else {
            meta.atlasX = 0;
            meta.atlasY = 0;
        }
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.zoom = 1.0f;
        meta.centerX = 0.5f;
        meta.centerY = 0.5f;
        meta.flags = 0;
        meta.bgColor = 0xFF1A1A1A;
        meta.scaledWidth = _pixelWidth;
        meta.scaledHeight = _pixelHeight;

        return _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta));
    }

    //=========================================================================
    // Members
    //=========================================================================

    const YettyContext& _ctx;
    std::string _argsStr;
    std::string _payloadStr;
    std::string _inputSource;  // "-" for payload, or file path

    // Ymery
    ymery::LangPtr _lang;
    ymery::DispatcherPtr _dispatcher;
    ymery::TreeLikePtr _dataTree;
    ymery::Renderer::Ptr _renderer;

    // ImGui offscreen context
    ImGuiContext* _imguiContext = nullptr;
    ImGuiContext* _prevContext = nullptr;

    // Offscreen texture
    WGPUTexture _offscreenTexture = nullptr;
    WGPUTextureView _offscreenView = nullptr;

    // Readback buffer for GPU → CPU pixel transfer
    WGPUBuffer _readbackBuffer = nullptr;
    uint32_t _readbackSize = 0;

    // Pixel dimensions
    uint32_t _pixelWidth = 0;
    uint32_t _pixelHeight = 0;
    uint32_t _cellWidth = 10;
    uint32_t _cellHeight = 20;

    // CPU pixel buffer and texture handle for atlas packing
    std::vector<uint8_t> _cpuPixels;
    TextureHandle _textureHandle = TextureHandle::invalid();

    // State
    bool _needsResize = false;
    bool _metadataDirty = true;
    bool _focused = false;
    bool _dirty = true;
    int _initFrames = 3;
};

//=============================================================================
// Factory
//=============================================================================

Result<Ymery::Ptr> Ymery::createImpl(
    ContextType& /*ctx*/,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    if (!yettyCtx.cardManager) {
        return Err<Ptr>("Ymery::createImpl: null CardBufferManager");
    }
    if (false) { // cardManager always valid
        return Err<Ptr>("Ymery::createImpl: null CardTextureManager");
    }

    auto card = std::make_shared<YmeryImpl>(
        yettyCtx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        return Err<Ptr>("Ymery::createImpl: init failed", res);
    }

    return Ok<Ptr>(card);
}

} // namespace yetty::card
