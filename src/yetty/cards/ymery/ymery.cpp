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
#include <cstring>

#define WGPU_STR(s) WGPUStringView{.data = s, .length = WGPU_STRLEN}

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
        : Ymery(ctx.cardBufferManager, ctx.gpu, x, y, widthCells, heightCells)
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

    void setCellSize(uint32_t cellWidth, uint32_t cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            _needsResize = true;
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

        // Parse payload as YAML path
        if (_payloadStr.empty()) {
            return Err<void>("Ymery::init: empty payload (need YAML path)");
        }

        fs::path inputPath = fs::absolute(_payloadStr);
        if (!fs::exists(inputPath)) {
            return Err<void>("Ymery::init: path not found: " + inputPath.string());
        }

        // Determine search paths and main module (same logic as ymery-test)
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

        // Load YAML
        auto langRes = ymery::Lang::create(searchPaths, mainModule);
        if (!langRes) {
            return Err<void>("Ymery::init: failed to load YAML", langRes);
        }
        _lang = *langRes;

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

        // Create renderer
        auto rendererRes = ymery::Renderer::create(_lang, _dispatcher, _dataTree);
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

        // Compute initial pixel size
        _pixelWidth = _widthCells * _cellWidth;
        _pixelHeight = _heightCells * _cellHeight;

        // Create offscreen texture
        if (auto res = _createOffscreenTexture(); !res) {
            return Err<void>("Ymery::init: failed to create offscreen texture", res);
        }

        // Allocate texture data
        uint32_t dataSize = _pixelWidth * _pixelHeight * 4;
        auto texResult = _cardMgr->allocateTextureData(dataSize);
        if (!texResult) {
            return Err<void>("Ymery::init: failed to allocate textureData", texResult);
        }
        _textureDataHandle = *texResult;

        // Upload initial metadata
        if (auto res = _uploadMetadata(); !res) {
            return Err<void>("Ymery::init: failed to upload metadata", res);
        }

        // Register for events
        if (auto res = _registerForEvents(); !res) {
            return Err<void>("Ymery::init: failed to register for events", res);
        }

        yinfo("Ymery::init: ready ({}x{} cells, {}x{} pixels)",
              _widthCells, _heightCells, _pixelWidth, _pixelHeight);

        return Ok();
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

        if (_textureDataHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateTextureData(_textureDataHandle);
            _textureDataHandle = TextureDataHandle::invalid();
        }

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

    Result<void> update(float time) override {
        (void)time;

        if (!_imguiContext || !_renderer) {
            return Ok();
        }

        // Handle resize
        if (_needsResize) {
            uint32_t newW = _widthCells * _cellWidth;
            uint32_t newH = _heightCells * _cellHeight;
            if (newW != _pixelWidth || newH != _pixelHeight) {
                _pixelWidth = newW;
                _pixelHeight = newH;

                _destroyOffscreenTexture();
                _destroyReadbackBuffer();
                if (auto res = _createOffscreenTexture(); !res) {
                    return Err<void>("Ymery::update: resize failed", res);
                }

                // Reallocate textureData
                if (_textureDataHandle.isValid()) {
                    _cardMgr->deallocateTextureData(_textureDataHandle);
                    _textureDataHandle = TextureDataHandle::invalid();
                }
                uint32_t dataSize = _pixelWidth * _pixelHeight * 4;
                auto texResult = _cardMgr->allocateTextureData(dataSize);
                if (!texResult) {
                    return Err<void>("Ymery::update: realloc textureData failed", texResult);
                }
                _textureDataHandle = *texResult;
                _metadataDirty = true;
            }
            _needsResize = false;
        }

        if (_pixelWidth == 0 || _pixelHeight == 0) {
            return Ok();
        }

        // Save current ImGui context and switch to ours
        ImGuiContext* prevCtx = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(_imguiContext);

        // ImGui frame
        ImGui_ImplWGPU_NewFrame();
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(_pixelWidth), static_cast<float>(_pixelHeight));
        io.DeltaTime = 1.0f / 60.0f;
        ImGui::NewFrame();

        // Render ymery widgets
        if (auto res = _renderer->renderFrame(); !res) {
            ydebug("Ymery::update: renderFrame failed: {}", res.error().to_string());
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

        // Map readback buffer synchronously
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

            if (mapped && _textureDataHandle.data) {
                // Copy rows (strip alignment padding if any)
                uint8_t* cpuDst = reinterpret_cast<uint8_t*>(_textureDataHandle.data);
                for (uint32_t row = 0; row < _pixelHeight; ++row) {
                    std::memcpy(cpuDst + row * bytesPerRow,
                                mapped + row * alignedBytesPerRow,
                                bytesPerRow);
                }
                _cardMgr->markTextureDataDirty(_textureDataHandle);
            }

            wgpuBufferUnmap(_readbackBuffer);
        } else {
            ydebug("Ymery::update: readback map failed (status={})", static_cast<int>(mapCtx.status));
        }

        if (_metadataDirty) {
            if (auto res = _uploadMetadata(); !res) {
                return Err<void>("Ymery::update: metadata upload failed", res);
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
                    _subscribeMouseEvents();
                    yinfo("Ymery::onEvent: focused (id={})", id());
                }
            } else if (_focused) {
                _focused = false;
                _unsubscribeMouseEvents();
                yinfo("Ymery::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        if (!_focused) return Ok(false);

        // Convert window coords to card-local pixel coords
        auto toLocal = [&](float wx, float wy, float& lx, float& ly) {
            lx = wx - _screenOriginX - static_cast<float>(_x) * _cellWidth;
            ly = wy - _screenOriginY - static_cast<float>(_y) * _cellHeight;
        };

        ImGuiContext* prev = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(_imguiContext);
        ImGuiIO& io = ImGui::GetIO();

        if (event.type == base::Event::Type::MouseDown) {
            float lx, ly;
            toLocal(event.mouse.x, event.mouse.y, lx, ly);
            io.AddMousePosEvent(lx, ly);
            io.AddMouseButtonEvent(std::min(event.mouse.button, 4), true);
            ImGui::SetCurrentContext(prev);
            return Ok(true);
        }

        if (event.type == base::Event::Type::MouseUp) {
            float lx, ly;
            toLocal(event.mouse.x, event.mouse.y, lx, ly);
            io.AddMousePosEvent(lx, ly);
            io.AddMouseButtonEvent(std::min(event.mouse.button, 4), false);
            ImGui::SetCurrentContext(prev);
            return Ok(true);
        }

        if (event.type == base::Event::Type::MouseMove) {
            float lx, ly;
            toLocal(event.mouse.x, event.mouse.y, lx, ly);
            io.AddMousePosEvent(lx, ly);
            ImGui::SetCurrentContext(prev);
            return Ok(true);
        }

        if (event.type == base::Event::Type::Scroll) {
            io.AddMouseWheelEvent(event.scroll.dx, event.scroll.dy);
            ImGui::SetCurrentContext(prev);
            return Ok(true);
        }

        ImGui::SetCurrentContext(prev);
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

    void _subscribeMouseEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        loop->registerListener(base::Event::Type::MouseDown, self, 1000);
        loop->registerListener(base::Event::Type::MouseUp, self, 1000);
        loop->registerListener(base::Event::Type::MouseMove, self, 1000);
        loop->registerListener(base::Event::Type::Scroll, self, 1000);
        yinfo("Ymery {} subscribed to mouse events", id());
    }

    void _unsubscribeMouseEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        loop->deregisterListener(base::Event::Type::MouseDown, self);
        loop->deregisterListener(base::Event::Type::MouseUp, self);
        loop->deregisterListener(base::Event::Type::MouseMove, self);
        loop->deregisterListener(base::Event::Type::Scroll, self);
        yinfo("Ymery {} unsubscribed from mouse events", id());
    }

    void _deregisterFromEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;
        loop->deregisterListener(sharedAs<base::EventListener>());
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
        meta.textureDataOffset = _textureDataHandle.isValid() ? _textureDataHandle.offset : 0;
        meta.textureWidth = _pixelWidth;
        meta.textureHeight = _pixelHeight;
        meta.atlasX = 0;
        meta.atlasY = 0;
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

    // Texture data
    TextureDataHandle _textureDataHandle = TextureDataHandle::invalid();

    // State
    bool _needsResize = false;
    bool _metadataDirty = true;
    bool _focused = false;
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
    if (!yettyCtx.cardBufferManager) {
        return Err<Ptr>("Ymery::createImpl: null CardBufferManager");
    }

    auto card = std::make_shared<YmeryImpl>(
        yettyCtx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        return Err<Ptr>("Ymery::createImpl: init failed", res);
    }

    return Ok<Ptr>(card);
}

} // namespace yetty::card
