#include <yetty/terminal-view.h>
#include <yetty/webgpu-context.h>
#include <yetty/wgpu-compat.h>
#include "gpu-screen.h"
#include "terminal.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <ytrace/ytrace.hpp>

namespace yetty {

struct TerminalView::Impl {
    std::shared_ptr<Terminal> terminal;
    MsMsdfFont::Ptr font;
    BmFont::Ptr bitmapFont;
    YettyFontManager::Ptr fontManager;
    WebGPUContext* ctx = nullptr;
#if !YETTY_WEB && !defined(__ANDROID__)
    uv_loop_t* uvLoop = nullptr;
#endif

    uint32_t cols = 80;
    uint32_t rows = 24;
    float cellWidth = 10.0f;
    float cellHeight = 20.0f;

    WGPUDevice device = nullptr;
    WGPUShaderModule shaderModule = nullptr;
    WGPURenderPipeline pipeline = nullptr;
    WGPUBindGroupLayout bindGroupLayout = nullptr;
    WGPUBindGroup bindGroup = nullptr;
    WGPUPipelineLayout pipelineLayout = nullptr;
    WGPUBuffer uniformBuffer = nullptr;
    WGPUBuffer cellBuffer = nullptr;
    WGPUBuffer quadVertexBuffer = nullptr;
    size_t cellBufferSize = 0;

    WGPUBindGroupLayout sharedBindGroupLayout = nullptr;
    WGPUBindGroup sharedBindGroup = nullptr;

    struct Uniforms {
        glm::mat4 projection;
        glm::vec2 screenSize;
        glm::vec2 cellSize;
        glm::vec2 gridSize;
        float pixelRange;
        float scale;
        glm::vec2 cursorPos;
        float cursorVisible;
        float _pad;
        glm::vec2 viewportOrigin;
        glm::vec2 _pad2;
    };
    Uniforms uniforms = {};

    uint32_t textureCols = 0;
    uint32_t textureRows = 0;
    uint32_t screenWidth = 800;
    uint32_t screenHeight = 600;

    ~Impl() {
        if (cellBuffer) wgpuBufferRelease(cellBuffer);
        if (bindGroup) wgpuBindGroupRelease(bindGroup);
        if (quadVertexBuffer) wgpuBufferRelease(quadVertexBuffer);
        if (uniformBuffer) wgpuBufferRelease(uniformBuffer);
        if (pipelineLayout) wgpuPipelineLayoutRelease(pipelineLayout);
        if (bindGroupLayout) wgpuBindGroupLayoutRelease(bindGroupLayout);
        if (pipeline) wgpuRenderPipelineRelease(pipeline);
        if (shaderModule) wgpuShaderModuleRelease(shaderModule);
    }

    Result<void> createShaderModule() {
        const char* shaderPath = CMAKE_SOURCE_DIR "/src/yetty/shaders/shaders.wgsl";

        std::ifstream file(shaderPath);
        if (!file.is_open()) {
            return Err<void>(std::string("Failed to open shader: ") + shaderPath);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string shaderSource = buffer.str();

        size_t funcPos = shaderSource.find("// SHADER_GLYPH_FUNCTIONS_PLACEHOLDER");
        if (funcPos != std::string::npos) {
            shaderSource.replace(funcPos, 37, "");
        }
        size_t dispPos = shaderSource.find("// SHADER_GLYPH_DISPATCH_PLACEHOLDER");
        if (dispPos != std::string::npos) {
            shaderSource.replace(dispPos, 36, "");
        }

        WGPUShaderSourceWGSL wgslDesc = {};
        wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
        WGPU_SHADER_CODE(wgslDesc, shaderSource);

        WGPUShaderModuleDescriptor moduleDesc = {};
        moduleDesc.nextInChain = &wgslDesc.chain;
        moduleDesc.label = WGPU_STR("terminal shader");

        shaderModule = wgpuDeviceCreateShaderModule(device, &moduleDesc);
        if (!shaderModule) {
            return Err<void>("Failed to create shader module");
        }

        return Ok();
    }

    Result<void> createBuffers() {
        WGPUBufferDescriptor uniformDesc = {};
        uniformDesc.label = WGPU_STR("terminal uniforms");
        uniformDesc.size = sizeof(Uniforms);
        uniformDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
        uniformBuffer = wgpuDeviceCreateBuffer(device, &uniformDesc);
        if (!uniformBuffer) {
            return Err<void>("Failed to create uniform buffer");
        }

        float quadVertices[] = {
            -1.0f, -1.0f,
             1.0f, -1.0f,
            -1.0f,  1.0f,
            -1.0f,  1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,
        };

        WGPUBufferDescriptor quadDesc = {};
        quadDesc.label = WGPU_STR("quad vertices");
        quadDesc.size = sizeof(quadVertices);
        quadDesc.usage = WGPUBufferUsage_Vertex;
        quadDesc.mappedAtCreation = true;
        quadVertexBuffer = wgpuDeviceCreateBuffer(device, &quadDesc);
        if (!quadVertexBuffer) {
            return Err<void>("Failed to create quad vertex buffer");
        }
        void* mapped = wgpuBufferGetMappedRange(quadVertexBuffer, 0, sizeof(quadVertices));
        memcpy(mapped, quadVertices, sizeof(quadVertices));
        wgpuBufferUnmap(quadVertexBuffer);

        return Ok();
    }

    Result<void> createCellBuffer(uint32_t c, uint32_t r) {
        size_t requiredSize = static_cast<size_t>(c) * r * 12;

        if (cellBuffer && cellBufferSize >= requiredSize) {
            textureCols = c;
            textureRows = r;
            return Ok();
        }

        if (cellBuffer) {
            wgpuBufferRelease(cellBuffer);
            cellBuffer = nullptr;
        }

        textureCols = c;
        textureRows = r;

        WGPUBufferDescriptor bufferDesc = {};
        bufferDesc.label = WGPU_STR("cell buffer");
        bufferDesc.size = requiredSize;
        bufferDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
        cellBuffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
        if (!cellBuffer) {
            return Err<void>("Failed to create cell buffer");
        }
        cellBufferSize = requiredSize;

        return Ok();
    }

    Result<void> createBindGroupLayout() {
        WGPUBindGroupLayoutEntry entries[8] = {};

        entries[0].binding = 0;
        entries[0].visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
        entries[0].buffer.type = WGPUBufferBindingType_Uniform;
        entries[0].buffer.minBindingSize = sizeof(Uniforms);

        entries[1].binding = 1;
        entries[1].visibility = WGPUShaderStage_Fragment;
        entries[1].texture.sampleType = WGPUTextureSampleType_Float;
        entries[1].texture.viewDimension = WGPUTextureViewDimension_2D;

        entries[2].binding = 2;
        entries[2].visibility = WGPUShaderStage_Fragment;
        entries[2].sampler.type = WGPUSamplerBindingType_Filtering;

        entries[3].binding = 3;
        entries[3].visibility = WGPUShaderStage_Fragment;
        entries[3].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
        entries[3].buffer.minBindingSize = sizeof(GlyphMetadataGPU);

        entries[4].binding = 4;
        entries[4].visibility = WGPUShaderStage_Fragment;
        entries[4].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
        entries[4].buffer.minBindingSize = sizeof(Cell);

        entries[5].binding = 5;
        entries[5].visibility = WGPUShaderStage_Fragment;
        entries[5].texture.sampleType = WGPUTextureSampleType_Float;
        entries[5].texture.viewDimension = WGPUTextureViewDimension_2D;

        entries[6].binding = 6;
        entries[6].visibility = WGPUShaderStage_Fragment;
        entries[6].sampler.type = WGPUSamplerBindingType_Filtering;

        entries[7].binding = 7;
        entries[7].visibility = WGPUShaderStage_Fragment;
        entries[7].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
        entries[7].buffer.minBindingSize = 0;

        WGPUBindGroupLayoutDescriptor layoutDesc = {};
        layoutDesc.entryCount = 8;
        layoutDesc.entries = entries;
        bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &layoutDesc);
        if (!bindGroupLayout) {
            return Err<void>("Failed to create bind group layout");
        }

        WGPUBindGroupLayout layouts[2] = { sharedBindGroupLayout, bindGroupLayout };
        WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
        pipelineLayoutDesc.bindGroupLayoutCount = 2;
        pipelineLayoutDesc.bindGroupLayouts = layouts;
        pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &pipelineLayoutDesc);
        if (!pipelineLayout) {
            return Err<void>("Failed to create pipeline layout");
        }

        return Ok();
    }

    Result<void> createBindGroup() {
        if (bindGroup) {
            wgpuBindGroupRelease(bindGroup);
            bindGroup = nullptr;
        }

        WGPUBindGroupEntry entries[8] = {};

        entries[0].binding = 0;
        entries[0].buffer = uniformBuffer;
        entries[0].size = sizeof(Uniforms);

        entries[1].binding = 1;
        entries[1].textureView = font->getTextureView();

        entries[2].binding = 2;
        entries[2].sampler = font->getSampler();

        entries[3].binding = 3;
        entries[3].buffer = font->getGlyphMetadataBuffer();
        entries[3].size = font->getBufferGlyphCount() * sizeof(GlyphMetadataGPU);

        entries[4].binding = 4;
        entries[4].buffer = cellBuffer;
        entries[4].size = cellBufferSize;

        entries[5].binding = 5;
        entries[5].textureView = bitmapFont->getTextureView();

        entries[6].binding = 6;
        entries[6].sampler = bitmapFont->getSampler();

        entries[7].binding = 7;
        entries[7].buffer = bitmapFont->getMetadataBuffer();
        entries[7].size = wgpuBufferGetSize(bitmapFont->getMetadataBuffer());

        WGPUBindGroupDescriptor bindGroupDesc = {};
        bindGroupDesc.layout = bindGroupLayout;
        bindGroupDesc.entryCount = 8;
        bindGroupDesc.entries = entries;
        bindGroup = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);
        if (!bindGroup) {
            return Err<void>("Failed to create bind group");
        }

        return Ok();
    }

    Result<void> createPipeline(WGPUTextureFormat format) {
        WGPUVertexAttribute posAttr = {};
        posAttr.format = WGPUVertexFormat_Float32x2;
        posAttr.offset = 0;
        posAttr.shaderLocation = 0;

        WGPUVertexBufferLayout vertexLayout = {};
        vertexLayout.arrayStride = 2 * sizeof(float);
        vertexLayout.stepMode = WGPUVertexStepMode_Vertex;
        vertexLayout.attributeCount = 1;
        vertexLayout.attributes = &posAttr;

        WGPUBlendState blendState = {};
        blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
        blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
        blendState.color.operation = WGPUBlendOperation_Add;
        blendState.alpha.srcFactor = WGPUBlendFactor_One;
        blendState.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
        blendState.alpha.operation = WGPUBlendOperation_Add;

        WGPUColorTargetState colorTarget = {};
        colorTarget.format = format;
        colorTarget.blend = &blendState;
        colorTarget.writeMask = WGPUColorWriteMask_All;

        WGPUFragmentState fragState = {};
        fragState.module = shaderModule;
        fragState.entryPoint = WGPU_STR("fs_main");
        fragState.targetCount = 1;
        fragState.targets = &colorTarget;

        WGPURenderPipelineDescriptor pipelineDesc = {};
        pipelineDesc.label = WGPU_STR("terminal pipeline");
        pipelineDesc.layout = pipelineLayout;
        pipelineDesc.vertex.module = shaderModule;
        pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
        pipelineDesc.vertex.bufferCount = 1;
        pipelineDesc.vertex.buffers = &vertexLayout;
        pipelineDesc.fragment = &fragState;
        pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
        pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
        pipelineDesc.primitive.cullMode = WGPUCullMode_None;
        pipelineDesc.multisample.count = 1;
        pipelineDesc.multisample.mask = 0xFFFFFFFF;

        pipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
        if (!pipeline) {
            return Err<void>("Failed to create render pipeline");
        }

        return Ok();
    }

    Result<void> init() {
        auto fontResult = fontManager->getMsMsdfFont("DejaVuSansMNerdFontMono");
        if (!fontResult) {
            return Err<void>("Failed to get terminal font", fontResult);
        }
        font = *fontResult;

        if (auto r = font->createTexture(device, ctx->getQueue()); !r) {
            return Err<void>("Failed to create font texture", r);
        }
        if (auto r = font->createGlyphMetadataBuffer(device); !r) {
            return Err<void>("Failed to create glyph metadata buffer", r);
        }

        cellWidth = font->getFontSize() * 0.6f * 0.5f;
        cellHeight = font->getLineHeight() * 0.5f;

        if (auto r = createShaderModule(); !r) {
            return Err<void>("Failed to create shader module", r);
        }
        if (auto r = createBuffers(); !r) {
            return Err<void>("Failed to create buffers", r);
        }
        if (auto r = createCellBuffer(cols, rows); !r) {
            return Err<void>("Failed to create cell buffer", r);
        }
        if (auto r = createBindGroupLayout(); !r) {
            return Err<void>("Failed to create bind group layout", r);
        }
        if (auto r = createBindGroup(); !r) {
            return Err<void>("Failed to create bind group", r);
        }
        if (auto r = createPipeline(ctx->getSurfaceFormat()); !r) {
            return Err<void>("Failed to create pipeline", r);
        }

#if !YETTY_WEB && !defined(__ANDROID__)
        auto defaultFont = fontManager->getDefaultFont();
        auto termResult = Terminal::create(0, cols, rows, defaultFont, uvLoop, nullptr);
        if (!termResult) {
            return Err<void>("Failed to create Terminal", termResult);
        }
        terminal = *termResult;
        terminal->start();
#endif

        yinfo("TerminalView initialized: {}x{} cells, {:.1f}x{:.1f} cell size",
              cols, rows, cellWidth, cellHeight);

        return Ok();
    }

    void resize(uint32_t c, uint32_t r) {
        cols = c;
        rows = r;
#if !YETTY_WEB && !defined(__ANDROID__)
        if (terminal) {
            terminal->resize(c, r);
        }
#endif
    }
};

TerminalView::TerminalView() : _impl(std::make_unique<Impl>()) {}

TerminalView::~TerminalView() = default;

Result<TerminalView::Ptr> TerminalView::create(
    base::ObjectId paneId,
    WebGPUContext& ctx,
    YettyFontManager::Ptr fontManager,
    BmFont::Ptr bitmapFont,
    uint32_t cols,
    uint32_t rows,
    WGPUBindGroupLayout sharedBindGroupLayout,
    WGPUBindGroup sharedBindGroup
#if !YETTY_WEB && !defined(__ANDROID__)
    , uv_loop_t* loop
#endif
) noexcept {
    (void)paneId;

    auto view = Ptr(new TerminalView());
    view->_impl->ctx = &ctx;
    view->_impl->device = ctx.getDevice();
    view->_impl->fontManager = fontManager;
    view->_impl->bitmapFont = bitmapFont;
    view->_impl->cols = cols;
    view->_impl->rows = rows;
    view->_impl->sharedBindGroupLayout = sharedBindGroupLayout;
    view->_impl->sharedBindGroup = sharedBindGroup;
#if !YETTY_WEB && !defined(__ANDROID__)
    view->_impl->uvLoop = loop;
#endif

    if (auto r = view->_impl->init(); !r) {
        return Err<Ptr>("Failed to initialize TerminalView", r);
    }
    return Ok(view);
}

Result<void> TerminalView::render(WGPURenderPassEncoder pass, WebGPUContext& ctx) {
#if !YETTY_WEB && !defined(__ANDROID__)
    if (!_impl->terminal) return Ok();
    if (_bounds.width <= 0 || _bounds.height <= 0) return Ok();

    GPUScreen* screen = _impl->terminal->getGPUScreen();
    if (!screen) return Ok();

    const Cell* cells = screen->getCellData();
    if (!cells) return Ok();

    uint32_t cols = static_cast<uint32_t>(screen->getCols());
    uint32_t rows = static_cast<uint32_t>(screen->getRows());

    if (cols != _impl->textureCols || rows != _impl->textureRows) {
        if (auto r = _impl->createCellBuffer(cols, rows); !r) {
            return Err<void>("Failed to resize cell buffer", r);
        }
        if (auto r = _impl->createBindGroup(); !r) {
            return Err<void>("Failed to recreate bind group", r);
        }
    }

    size_t cellDataSize = static_cast<size_t>(cols) * rows * 12;
    wgpuQueueWriteBuffer(ctx.getQueue(), _impl->cellBuffer, 0, cells, cellDataSize);

    _impl->uniforms.projection = glm::ortho(0.0f, _bounds.width, _bounds.height, 0.0f, -1.0f, 1.0f);
    _impl->uniforms.screenSize = {_bounds.width, _bounds.height};
    _impl->uniforms.cellSize = {_impl->cellWidth, _impl->cellHeight};
    _impl->uniforms.gridSize = {static_cast<float>(cols), static_cast<float>(rows)};
    _impl->uniforms.pixelRange = _impl->font ? _impl->font->getPixelRange() : 2.0f;
    _impl->uniforms.scale = _impl->cellHeight / _impl->font->getLineHeight();
    _impl->uniforms.cursorPos = {static_cast<float>(_impl->terminal->getCursorCol()),
                                  static_cast<float>(_impl->terminal->getCursorRow())};
    _impl->uniforms.cursorVisible = _impl->terminal->isCursorVisible() ? 1.0f : 0.0f;
    _impl->uniforms.viewportOrigin = {_bounds.x, _bounds.y};
    wgpuQueueWriteBuffer(ctx.getQueue(), _impl->uniformBuffer, 0, &_impl->uniforms, sizeof(Impl::Uniforms));

    wgpuRenderPassEncoderSetViewport(pass, _bounds.x, _bounds.y, _bounds.width, _bounds.height, 0.0f, 1.0f);
    wgpuRenderPassEncoderSetScissorRect(pass,
        static_cast<uint32_t>(_bounds.x), static_cast<uint32_t>(_bounds.y),
        static_cast<uint32_t>(_bounds.width), static_cast<uint32_t>(_bounds.height));

    wgpuRenderPassEncoderSetPipeline(pass, _impl->pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, _impl->sharedBindGroup, 0, nullptr);
    wgpuRenderPassEncoderSetBindGroup(pass, 1, _impl->bindGroup, 0, nullptr);
    wgpuRenderPassEncoderSetVertexBuffer(pass, 0, _impl->quadVertexBuffer, 0, WGPU_WHOLE_SIZE);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
#else
    (void)pass;
    (void)ctx;
#endif

    return Ok();
}

Result<bool> TerminalView::onEvent(const Event& event) {
    switch (event.type) {
        case Event::Type::Char:
#if !YETTY_WEB && !defined(__ANDROID__)
            if (_impl->terminal) {
                _impl->terminal->sendKey(event.chr.codepoint);
            }
#endif
            return Ok(true);

        case Event::Type::Scroll:
#if !YETTY_WEB && !defined(__ANDROID__)
            if (_impl->terminal) {
                if (event.scroll.dy > 0) {
                    _impl->terminal->scrollUp(static_cast<int>(event.scroll.dy));
                } else if (event.scroll.dy < 0) {
                    _impl->terminal->scrollDown(static_cast<int>(-event.scroll.dy));
                }
            }
#endif
            return Ok(true);

        default:
            return Ok(false);
    }
}

void TerminalView::setBounds(Rect r) {
    _bounds = r;
    onBoundsChanged();
}

void TerminalView::onBoundsChanged() {
    _impl->screenWidth = static_cast<uint32_t>(_bounds.width);
    _impl->screenHeight = static_cast<uint32_t>(_bounds.height);

    if (_impl->cellWidth > 0 && _impl->cellHeight > 0) {
        uint32_t newCols = static_cast<uint32_t>(_bounds.width / _impl->cellWidth);
        uint32_t newRows = static_cast<uint32_t>(_bounds.height / _impl->cellHeight);

        if (newCols > 0 && newRows > 0 && (newCols != _impl->cols || newRows != _impl->rows)) {
            _impl->resize(newCols, newRows);
        }
    }
}

} // namespace yetty
