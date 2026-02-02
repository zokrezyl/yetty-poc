#include "image.h"
#include <yetty/yetty-context.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <stb_image.h>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>

// GLFW modifier constants
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

// WebGPU string helper
#define WGPU_STR(s) WGPUStringView{.data = s, .length = WGPU_STRLEN}

namespace yetty::card {

//=============================================================================
// ImageImpl - Implementation of Image card
//=============================================================================

class ImageImpl : public Image {
public:
    ImageImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : Image(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _ctx(ctx)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~ImageImpl() override {
        dispose();
    }

    const char* typeName() const override { return "image"; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    void setCellSize(uint32_t cellWidth, uint32_t cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            recalculateScaledDimensions();
            _needsScaling = true;
        }
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("Image::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("Image::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Create compute pipeline for GPU scaling
        if (auto res = createScalePipeline(); !res) {
            return Err<void>("Image::init: failed to create scale pipeline", res);
        }

        // Load image from payload if present
        if (!_payloadStr.empty()) {
            if (auto res = loadImageFromPayload(); !res) {
                yerror("Image::init: failed to load image: {}", error_msg(res));
                return Err<void>("Image::init: failed to load image", res);
            }
        }

        // Scaling + texture linking will happen via allocateTextures() in the lifecycle.
        // setCellSize() sets _needsScaling=true, allocateTextures() picks it up.
        _needsScaling = true;
        _metadataDirty = true;

        // Register for events - cards have priority 1000, above gpu-screen
        if (auto res = registerForEvents(); !res) {
            return Err<void>("Image::init: failed to register for events", res);
        }

        return Ok();
    }

    Result<void> dispose() override {
        // Deregister from events
        if (auto res = deregisterFromEvents(); !res) {
            return Err<void>("Image::dispose: failed to deregister", res);
        }

        // Release GPU resources
        if (_tempSrcBuffer) { wgpuBufferRelease(_tempSrcBuffer); _tempSrcBuffer = nullptr; }
        if (_scaleBindGroup) { wgpuBindGroupRelease(_scaleBindGroup); _scaleBindGroup = nullptr; }
        if (_scaleUniformBuffer) { wgpuBufferRelease(_scaleUniformBuffer); _scaleUniformBuffer = nullptr; }
        if (_scalePipeline) { wgpuComputePipelineRelease(_scalePipeline); _scalePipeline = nullptr; }
        if (_scaleBindGroupLayout) { wgpuBindGroupLayoutRelease(_scaleBindGroupLayout); _scaleBindGroupLayout = nullptr; }
        if (_scaleShaderModule) { wgpuShaderModuleRelease(_scaleShaderModule); _scaleShaderModule = nullptr; }

        // Release readback resources
        if (_readbackBuffer) { wgpuBufferRelease(_readbackBuffer); _readbackBuffer = nullptr; }
        if (_scaledGpuBuffer) { wgpuBufferRelease(_scaledGpuBuffer); _scaledGpuBuffer = nullptr; }

        // Release texture handle
        if (_textureHandle.isValid() && _cardMgr) {
            _cardMgr->textureManager()->deallocate(_textureHandle);
            _textureHandle = TextureHandle::invalid();
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        // Clear CPU memory
        _originalPixels.clear();
        _originalPixels.shrink_to_fit();
        _scaledPixels.clear();
        _scaledPixels.shrink_to_fit();

        return Ok();
    }

    void suspend() override {
        // Release compute GPU resources
        if (_tempSrcBuffer) { wgpuBufferRelease(_tempSrcBuffer); _tempSrcBuffer = nullptr; }
        if (_scaleBindGroup) { wgpuBindGroupRelease(_scaleBindGroup); _scaleBindGroup = nullptr; }
        if (_scaleUniformBuffer) { wgpuBufferRelease(_scaleUniformBuffer); _scaleUniformBuffer = nullptr; }
        if (_scalePipeline) { wgpuComputePipelineRelease(_scalePipeline); _scalePipeline = nullptr; }
        if (_scaleBindGroupLayout) { wgpuBindGroupLayoutRelease(_scaleBindGroupLayout); _scaleBindGroupLayout = nullptr; }
        if (_scaleShaderModule) { wgpuShaderModuleRelease(_scaleShaderModule); _scaleShaderModule = nullptr; }

        // Release readback resources
        if (_readbackBuffer) { wgpuBufferRelease(_readbackBuffer); _readbackBuffer = nullptr; }
        if (_scaledGpuBuffer) { wgpuBufferRelease(_scaledGpuBuffer); _scaledGpuBuffer = nullptr; }

        // Deallocate texture handle
        if (_textureHandle.isValid() && _cardMgr) {
            _cardMgr->textureManager()->deallocate(_textureHandle);
            _textureHandle = TextureHandle::invalid();
        }
        _scaledPixels.clear();
        _needsScaling = true;
        yinfo("Image::suspend: deallocated texture handle + compute resources, _originalPixels has {} bytes",
              _originalPixels.size());
    }

    Result<void> allocateTextures() override {
        if (!_originalPixels.empty()) {
            // Scale if needed (first time, after suspend, or cell size changed)
            if (_needsScaling || !_textureHandle.isValid() || _scaledPixels.empty()) {
                if (!_scalePipeline) {
                    if (auto res = createScalePipeline(); !res) {
                        return Err<void>("Image::allocateTextures: failed to recreate scale pipeline", res);
                    }
                }
                recalculateScaledDimensions();
                if (auto res = runScaleCompute(); !res) {
                    return Err<void>("Image::allocateTextures: failed to reconstruct texture", res);
                }
                _needsScaling = false;
            } else {
                // Re-register existing pixels with texture manager
                _cardMgr->textureManager()->write(_textureHandle, _scaledPixels.data());
            }
            _metadataDirty = true;
        }
        return Ok();
    }

    Result<void> render(float time) override {
        (void)time;

        if (_needsScaling && !_originalPixels.empty()) {
            if (auto res = runScaleCompute(); !res) {
                return Err<void>("Image::update: scaling failed", res);
            }
            _needsScaling = false;
        }

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("Image::update: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // EventListener interface
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        // Handle SetFocus events
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                if (!_focused) {
                    _focused = true;
                    ydebug("Image::onEvent: focused (id={})", id());
                }
                return Ok(true);
            } else if (_focused) {
                _focused = false;
                ydebug("Image::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        // Handle scroll events (only when focused)
        if (_focused && event.type == base::Event::Type::Scroll) {
            if (event.scroll.mods & GLFW_MOD_CONTROL) {
                // Ctrl+Scroll: zoom
                float zoomDelta = event.scroll.dy * 0.1f;
                float newZoom = std::clamp(_contentZoom + zoomDelta, 0.1f, 10.0f);
                if (newZoom != _contentZoom) {
                    _contentZoom = newZoom;
                    _metadataDirty = true;
                    yinfo("Image::onEvent: content zoom={:.2f}", _contentZoom);
                }
                return Ok(true);
            } else if (event.scroll.mods & GLFW_MOD_SHIFT) {
                // Shift+Scroll: pan horizontally
                float dx = event.scroll.dy * 0.05f / _contentZoom;
                _centerX = std::clamp(_centerX + dx, 0.0f, 1.0f);
                _metadataDirty = true;
                return Ok(true);
            } else {
                // Plain scroll: pan vertically
                float dy = event.scroll.dy * 0.05f / _contentZoom;
                _centerY = std::clamp(_centerY + dy, 0.0f, 1.0f);
                _metadataDirty = true;
                return Ok(true);
            }
        }

        return Ok(false);
    }

private:
    //=========================================================================
    // Event registration
    //=========================================================================

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("Image::registerForEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
            return Err<void>("Image::registerForEvents: failed to register SetFocus", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::Scroll, self, 1000); !res) {
            return Err<void>("Image::registerForEvents: failed to register Scroll", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::MouseDown, self, 1000); !res) {
            return Err<void>("Image::registerForEvents: failed to register MouseDown", res);
        }

        yinfo("Image card {} registered for events (priority 1000)", id());
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        if (weak_from_this().expired()) {
            return Ok();
        }

        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("Image::deregisterFromEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;

        if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
            return Err<void>("Image::deregisterFromEvents: failed to deregister", res);
        }
        return Ok();
    }

    //=========================================================================
    // Scaling dimensions
    //=========================================================================

    void recalculateScaledDimensions() {
        if (_originalWidth == 0 || _originalHeight == 0) {
            _scaledWidth = 0;
            _scaledHeight = 0;
            return;
        }

        uint32_t targetWidth = _widthCells * _cellWidth;
        uint32_t targetHeight = _heightCells * _cellHeight;

        if (targetWidth == 0 || targetHeight == 0) {
            _scaledWidth = _originalWidth;
            _scaledHeight = _originalHeight;
            return;
        }

        float imageAspect = static_cast<float>(_originalWidth) / static_cast<float>(_originalHeight);
        float targetAspect = static_cast<float>(targetWidth) / static_cast<float>(targetHeight);

        if (imageAspect > targetAspect) {
            _scaledWidth = targetWidth;
            _scaledHeight = static_cast<uint32_t>(targetWidth / imageAspect);
        } else {
            _scaledHeight = targetHeight;
            _scaledWidth = static_cast<uint32_t>(targetHeight * imageAspect);
        }

        _scaledWidth = std::max(_scaledWidth, 1u);
        _scaledHeight = std::max(_scaledHeight, 1u);

        yinfo("Image::recalculateScaledDimensions: original={}x{} target={}x{} scaled={}x{}",
              _originalWidth, _originalHeight, targetWidth, targetHeight, _scaledWidth, _scaledHeight);

        _metadataDirty = true;
    }

    //=========================================================================
    // Compute pipeline for scale-image.wgsl
    //=========================================================================

    Result<void> createScalePipeline() {
        const char* shaderPath = CMAKE_SOURCE_DIR "/src/yetty/shaders/scale-image.wgsl";
        yinfo("Image: Loading scale shader from: {}", shaderPath);

        std::ifstream file(shaderPath);
        if (!file.is_open()) {
            return Err<void>("Image: Failed to open scale shader file");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string shaderSource = buffer.str();

        WGPUShaderSourceWGSL wgslDesc = {};
        wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
        wgslDesc.code = WGPU_STR(shaderSource.c_str());

        WGPUShaderModuleDescriptor shaderDesc = {};
        shaderDesc.nextInChain = &wgslDesc.chain;
        shaderDesc.label = WGPU_STR("ImageScaleShader");

        _scaleShaderModule = wgpuDeviceCreateShaderModule(_gpu.device, &shaderDesc);
        if (!_scaleShaderModule) {
            return Err<void>("Image: Failed to create scale shader module");
        }

        // binding 0: uniform ScaleParams
        // binding 1: storage<read> srcBuffer
        // binding 2: storage<read_write> dstBuffer
        WGPUBindGroupLayoutEntry entries[3] = {};

        entries[0].binding = 0;
        entries[0].visibility = WGPUShaderStage_Compute;
        entries[0].buffer.type = WGPUBufferBindingType_Uniform;
        entries[0].buffer.minBindingSize = sizeof(ScaleParams);

        entries[1].binding = 1;
        entries[1].visibility = WGPUShaderStage_Compute;
        entries[1].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
        entries[1].buffer.minBindingSize = 0;

        entries[2].binding = 2;
        entries[2].visibility = WGPUShaderStage_Compute;
        entries[2].buffer.type = WGPUBufferBindingType_Storage;
        entries[2].buffer.minBindingSize = 0;

        WGPUBindGroupLayoutDescriptor layoutDesc = {};
        layoutDesc.label = WGPU_STR("ImageScaleBindGroupLayout");
        layoutDesc.entryCount = 3;
        layoutDesc.entries = entries;

        _scaleBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_gpu.device, &layoutDesc);
        if (!_scaleBindGroupLayout) {
            return Err<void>("Image: Failed to create scale bind group layout");
        }

        WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
        pipelineLayoutDesc.label = WGPU_STR("ImageScalePipelineLayout");
        pipelineLayoutDesc.bindGroupLayoutCount = 1;
        pipelineLayoutDesc.bindGroupLayouts = &_scaleBindGroupLayout;

        WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(_gpu.device, &pipelineLayoutDesc);
        if (!pipelineLayout) {
            return Err<void>("Image: Failed to create scale pipeline layout");
        }

        WGPUComputePipelineDescriptor pipelineDesc = {};
        pipelineDesc.label = WGPU_STR("ImageScalePipeline");
        pipelineDesc.layout = pipelineLayout;
        pipelineDesc.compute.module = _scaleShaderModule;
        pipelineDesc.compute.entryPoint = WGPU_STR("main");

        _scalePipeline = wgpuDeviceCreateComputePipeline(_gpu.device, &pipelineDesc);
        wgpuPipelineLayoutRelease(pipelineLayout);

        if (!_scalePipeline) {
            return Err<void>("Image: Failed to create scale compute pipeline");
        }

        WGPUBufferDescriptor uniformDesc = {};
        uniformDesc.label = WGPU_STR("ImageScaleUniform");
        uniformDesc.size = sizeof(ScaleParams);
        uniformDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;

        _scaleUniformBuffer = wgpuDeviceCreateBuffer(_gpu.device, &uniformDesc);
        if (!_scaleUniformBuffer) {
            return Err<void>("Image: Failed to create scale uniform buffer");
        }

        yinfo("Image: Scale compute pipeline created successfully");
        return Ok();
    }

    //=========================================================================
    // GPU scaling: upload original → compute shader → per-card GPU buffer
    //              → readback to CPU → link to TextureHandle
    //=========================================================================

    Result<void> runScaleCompute() {
        if (_originalPixels.empty() || _scaledWidth == 0 || _scaledHeight == 0) {
            return Err<void>("Image::runScaleCompute: no data to scale");
        }

        yinfo("Image::runScaleCompute: scaling {}x{} -> {}x{}",
              _originalWidth, _originalHeight, _scaledWidth, _scaledHeight);

        uint32_t scaledDataSize = _scaledWidth * _scaledHeight * 4;

        // Create per-card destination GPU buffer (Storage + CopySrc for readback)
        if (_scaledGpuBuffer) {
            wgpuBufferRelease(_scaledGpuBuffer);
            _scaledGpuBuffer = nullptr;
        }

        WGPUBufferDescriptor dstBufferDesc = {};
        dstBufferDesc.label = WGPU_STR("ImageScaledGpuBuffer");
        dstBufferDesc.size = scaledDataSize;
        dstBufferDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopySrc;

        _scaledGpuBuffer = wgpuDeviceCreateBuffer(_gpu.device, &dstBufferDesc);
        if (!_scaledGpuBuffer) {
            return Err<void>("Image::runScaleCompute: failed to create scaled GPU buffer");
        }

        // Upload original pixels to temp GPU buffer
        uint32_t srcDataSize = _originalWidth * _originalHeight * 4;

        if (_tempSrcBuffer) {
            wgpuBufferRelease(_tempSrcBuffer);
            _tempSrcBuffer = nullptr;
        }

        WGPUBufferDescriptor srcBufferDesc = {};
        srcBufferDesc.label = WGPU_STR("ImageTempSrcBuffer");
        srcBufferDesc.size = srcDataSize;
        srcBufferDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;

        _tempSrcBuffer = wgpuDeviceCreateBuffer(_gpu.device, &srcBufferDesc);
        if (!_tempSrcBuffer) {
            return Err<void>("Image::runScaleCompute: failed to create temp src buffer");
        }

        wgpuQueueWriteBuffer(_gpu.queue, _tempSrcBuffer, 0,
                             _originalPixels.data(), srcDataSize);

        // Update scale params — dstOffset is 0 since we write to our own buffer
        ScaleParams params = {};
        params.srcWidth = _originalWidth;
        params.srcHeight = _originalHeight;
        params.dstWidth = _scaledWidth;
        params.dstHeight = _scaledHeight;
        params.srcOffset = 0;
        params.dstOffset = 0;

        wgpuQueueWriteBuffer(_gpu.queue, _scaleUniformBuffer, 0,
                             &params, sizeof(params));

        // Create bind group
        if (_scaleBindGroup) {
            wgpuBindGroupRelease(_scaleBindGroup);
            _scaleBindGroup = nullptr;
        }

        WGPUBindGroupEntry bindEntries[3] = {};

        bindEntries[0].binding = 0;
        bindEntries[0].buffer = _scaleUniformBuffer;
        bindEntries[0].offset = 0;
        bindEntries[0].size = sizeof(ScaleParams);

        bindEntries[1].binding = 1;
        bindEntries[1].buffer = _tempSrcBuffer;
        bindEntries[1].offset = 0;
        bindEntries[1].size = srcDataSize;

        bindEntries[2].binding = 2;
        bindEntries[2].buffer = _scaledGpuBuffer;
        bindEntries[2].offset = 0;
        bindEntries[2].size = scaledDataSize;

        WGPUBindGroupDescriptor bindGroupDesc = {};
        bindGroupDesc.label = WGPU_STR("ImageScaleBindGroup");
        bindGroupDesc.layout = _scaleBindGroupLayout;
        bindGroupDesc.entryCount = 3;
        bindGroupDesc.entries = bindEntries;

        _scaleBindGroup = wgpuDeviceCreateBindGroup(_gpu.device, &bindGroupDesc);
        if (!_scaleBindGroup) {
            return Err<void>("Image::runScaleCompute: failed to create bind group");
        }

        // Dispatch compute shader + copy to readback buffer
        WGPUCommandEncoderDescriptor encoderDesc = {};
        encoderDesc.label = WGPU_STR("ImageScaleEncoder");
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_gpu.device, &encoderDesc);

        WGPUComputePassDescriptor passDesc = {};
        passDesc.label = WGPU_STR("ImageScalePass");
        WGPUComputePassEncoder pass = wgpuCommandEncoderBeginComputePass(encoder, &passDesc);

        wgpuComputePassEncoderSetPipeline(pass, _scalePipeline);
        wgpuComputePassEncoderSetBindGroup(pass, 0, _scaleBindGroup, 0, nullptr);

        uint32_t workgroupsX = (_scaledWidth + 15) / 16;
        uint32_t workgroupsY = (_scaledHeight + 15) / 16;
        wgpuComputePassEncoderDispatchWorkgroups(pass, workgroupsX, workgroupsY, 1);

        wgpuComputePassEncoderEnd(pass);
        wgpuComputePassEncoderRelease(pass);

        // Create readback buffer and copy scaled result
        if (_readbackBuffer) {
            wgpuBufferRelease(_readbackBuffer);
            _readbackBuffer = nullptr;
        }

        WGPUBufferDescriptor rbDesc = {};
        rbDesc.label = WGPU_STR("ImageReadback");
        rbDesc.size = scaledDataSize;
        rbDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;

        _readbackBuffer = wgpuDeviceCreateBuffer(_gpu.device, &rbDesc);
        if (!_readbackBuffer) {
            wgpuCommandEncoderRelease(encoder);
            return Err<void>("Image::runScaleCompute: failed to create readback buffer");
        }

        wgpuCommandEncoderCopyBufferToBuffer(encoder,
            _scaledGpuBuffer, 0, _readbackBuffer, 0, scaledDataSize);

        WGPUCommandBufferDescriptor cmdDesc = {};
        cmdDesc.label = WGPU_STR("ImageScaleCmd");
        WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
        wgpuCommandEncoderRelease(encoder);

        wgpuQueueSubmit(_gpu.queue, 1, &cmdBuffer);
        wgpuCommandBufferRelease(cmdBuffer);

        yinfo("Image::runScaleCompute: dispatched ({}x{} workgroups), reading back",
              workgroupsX, workgroupsY);

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

        wgpuBufferMapAsync(_readbackBuffer, WGPUMapMode_Read, 0, scaledDataSize, cbInfo);

        while (!mapCtx.done) {
            wgpuDeviceTick(_gpu.device);
        }

        if (mapCtx.status != WGPUMapAsyncStatus_Success) {
            return Err<void>("Image::runScaleCompute: readback map failed");
        }

        const uint8_t* mapped = static_cast<const uint8_t*>(
            wgpuBufferGetConstMappedRange(_readbackBuffer, 0, scaledDataSize));

        if (!mapped) {
            wgpuBufferUnmap(_readbackBuffer);
            return Err<void>("Image::runScaleCompute: mapped range is null");
        }

        // Copy to CPU buffer
        _scaledPixels.resize(scaledDataSize);
        std::memcpy(_scaledPixels.data(), mapped, scaledDataSize);
        wgpuBufferUnmap(_readbackBuffer);

        // Allocate texture handle if needed, link scaled pixels
        if (!_textureHandle.isValid()) {
            auto allocResult = _cardMgr->textureManager()->allocate(_scaledWidth, _scaledHeight);
            if (!allocResult) {
                return Err<void>("Image::runScaleCompute: failed to allocate texture handle", allocResult);
            }
            _textureHandle = *allocResult;
        }
        _cardMgr->textureManager()->write(_textureHandle, _scaledPixels.data());

        _metadataDirty = true;

        yinfo("Image::runScaleCompute: done, linked to handle id={}", _textureHandle.id);
        return Ok();
    }

    //=========================================================================
    // Image loading
    //=========================================================================

    Result<void> loadImageFromPayload() {
        if (_payloadStr.empty()) {
            return Err<void>("Image::loadImageFromPayload: empty payload");
        }

        yinfo("Image::loadImageFromPayload: payload size={}", _payloadStr.size());

        int width, height, channels;
        uint8_t* pixels = nullptr;

        // Detect if payload is a file path (starts with / and has no newlines)
        if (_payloadStr.size() < 4096 &&
            _payloadStr[0] == '/' &&
            _payloadStr.find('\n') == std::string::npos) {
            yinfo("Image::loadImageFromPayload: loading from file path: {}", _payloadStr);
            pixels = stbi_load(_payloadStr.c_str(), &width, &height, &channels, 4);
        } else {
            pixels = stbi_load_from_memory(
                reinterpret_cast<const uint8_t*>(_payloadStr.data()),
                static_cast<int>(_payloadStr.size()),
                &width, &height, &channels, 4);
        }

        if (!pixels) {
            return Err<void>(std::string("Image::loadImageFromPayload: stbi_load failed: ") +
                             stbi_failure_reason());
        }

        yinfo("Image::loadImageFromPayload: decoded {}x{} ({} channels -> 4)",
              width, height, channels);

        _originalWidth = static_cast<uint32_t>(width);
        _originalHeight = static_cast<uint32_t>(height);
        size_t dataSize = _originalWidth * _originalHeight * 4;
        _originalPixels.resize(dataSize);
        std::memcpy(_originalPixels.data(), pixels, dataSize);

        stbi_image_free(pixels);

        _payloadStr.clear();
        _payloadStr.shrink_to_fit();

        yinfo("Image::loadImageFromPayload: stored {}x{} ({} bytes) in CPU memory",
              _originalWidth, _originalHeight, dataSize);

        return Ok();
    }

    //=========================================================================
    // Args parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        yinfo("Image::parseArgs: args='{}'", args);

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--zoom" || token == "-z") {
                float val;
                if (iss >> val) {
                    _contentZoom = std::clamp(val, 0.1f, 10.0f);
                }
            } else if (token == "--center" || token == "-c") {
                float x, y;
                if (iss >> x >> y) {
                    _centerX = std::clamp(x, 0.0f, 1.0f);
                    _centerY = std::clamp(y, 0.0f, 1.0f);
                }
            }
        }
    }

    //=========================================================================
    // Metadata upload
    //=========================================================================

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("Image::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};
        meta.textureDataOffset = 0;  // No longer used
        meta.textureWidth = _scaledWidth;
        meta.textureHeight = _scaledHeight;

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
        meta.zoom = _contentZoom;
        meta.centerX = _centerX;
        meta.centerY = _centerY;
        meta.flags = 0;
        meta.bgColor = 0xFF000000;
        meta.scaledWidth = _scaledWidth;
        meta.scaledHeight = _scaledHeight;

        yinfo("Image::uploadMetadata: offset={} textureSize={}x{} zoom={}",
              _metaHandle.offset, meta.textureWidth, meta.textureHeight, meta.zoom);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("Image::uploadMetadata: write failed", res);
        }

        return Ok();
    }

    //=========================================================================
    // Data structures
    //=========================================================================

    // Scale shader params (matches scale-image.wgsl)
    struct ScaleParams {
        uint32_t srcWidth;
        uint32_t srcHeight;
        uint32_t dstWidth;
        uint32_t dstHeight;
        uint32_t srcOffset;
        uint32_t dstOffset;
    };

    // Metadata structure (64 bytes, matches Texture card / shader layout)
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

    //=========================================================================
    // Member variables
    //=========================================================================

    const YettyContext& _ctx;
    std::string _argsStr;
    std::string _payloadStr;

    // Original image (CPU memory)
    std::vector<uint8_t> _originalPixels;
    uint32_t _originalWidth = 0;
    uint32_t _originalHeight = 0;

    // Scaled dimensions
    uint32_t _scaledWidth = 0;
    uint32_t _scaledHeight = 0;

    // Content zoom and pan
    float _contentZoom = 1.0f;
    float _centerX = 0.5f;
    float _centerY = 0.5f;

    // Cell dimensions
    uint32_t _cellWidth = 10;
    uint32_t _cellHeight = 20;

    // GPU resources for compute shader scaling
    WGPUShaderModule _scaleShaderModule = nullptr;
    WGPUBindGroupLayout _scaleBindGroupLayout = nullptr;
    WGPUBindGroup _scaleBindGroup = nullptr;
    WGPUComputePipeline _scalePipeline = nullptr;
    WGPUBuffer _scaleUniformBuffer = nullptr;
    WGPUBuffer _tempSrcBuffer = nullptr;
    WGPUBuffer _scaledGpuBuffer = nullptr;   // Per-card compute output
    WGPUBuffer _readbackBuffer = nullptr;    // For GPU → CPU readback

    // Scaled image (CPU memory, linked to texture handle)
    std::vector<uint8_t> _scaledPixels;

    // Texture handle for atlas packing
    TextureHandle _textureHandle = TextureHandle::invalid();

    // State
    bool _focused = false;
    bool _needsScaling = false;
    bool _metadataDirty = true;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<Image::Ptr> Image::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;

    if (!yettyCtx.cardManager) {
        return Err<Ptr>("Image::createImpl: null CardManager");
    }

    auto card = std::make_shared<ImageImpl>(
        yettyCtx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        return Err<Ptr>("Image::createImpl: init failed", res);
    }

    return Ok<Ptr>(card);
}

} // namespace yetty::card
