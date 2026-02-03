#include <yetty/shader-manager.h>
#include <yetty/wgpu-compat.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/bm-font.h>
#include <ytrace/ytrace.hpp>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>
#include <filesystem>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

namespace yetty {

// Placeholder markers in the base shader
static const char* FUNCTIONS_PLACEHOLDER = "// SHADER_GLYPH_FUNCTIONS_PLACEHOLDER";
static const char* DISPATCH_PLACEHOLDER = "// SHADER_GLYPH_DISPATCH_PLACEHOLDER";
static const char* PRE_EFFECT_FUNCTIONS_PLACEHOLDER = "// PRE_EFFECT_FUNCTIONS_PLACEHOLDER";
static const char* PRE_EFFECT_APPLY_PLACEHOLDER = "// PRE_EFFECT_APPLY_PLACEHOLDER";
static const char* POST_EFFECT_FUNCTIONS_PLACEHOLDER = "// POST_EFFECT_FUNCTIONS_PLACEHOLDER";
static const char* POST_EFFECT_APPLY_PLACEHOLDER = "// POST_EFFECT_APPLY_PLACEHOLDER";

class ShaderManagerImpl : public ShaderManager {
public:
    ShaderManagerImpl() = default;
    ~ShaderManagerImpl() override;

    Result<void> init(const GPUContext& gpu) noexcept;

    void addProvider(std::shared_ptr<ShaderProvider> provider) override;
    void addLibrary(const std::string& name, const std::string& code) override;
    bool needsRecompile() const override;
    Result<void> compile() override;
    void update() override;

    WGPUShaderModule getShaderModule() const override { return _shaderModule; }
    WGPURenderPipeline getGridPipeline() const override { return _gridPipeline; }
    WGPUBindGroupLayout getGridBindGroupLayout() const override { return _gridBindGroupLayout; }
    WGPUBuffer getQuadVertexBuffer() const override { return _quadVertexBuffer; }
    const std::string& getMergedSource() const override { return _mergedSource; }

private:
    Result<void> loadBaseShader(const std::string& path);
    Result<void> createPipelineResources();
    Result<void> recreatePipeline();
    std::string mergeShaders() const;

    GPUContext _gpu = {};
    std::string _baseShader;
    std::vector<std::shared_ptr<ShaderProvider>> _providers;
    std::map<std::string, std::string> _libraries;
    std::string _mergedSource;

    // Effect files: index → {functionName, code}
    struct EffectFile {
        uint32_t index;
        std::string name;       // e.g., "scanlines"
        std::string funcName;   // e.g., "postEffect_scanlines"
        std::string code;
    };
    std::vector<EffectFile> _preEffects;
    std::vector<EffectFile> _postEffects;

    // Shared GPU resources
    WGPUShaderModule _shaderModule = nullptr;
    WGPURenderPipeline _gridPipeline = nullptr;
    WGPUPipelineLayout _pipelineLayout = nullptr;
    WGPUBindGroupLayout _gridBindGroupLayout = nullptr;
    WGPUBuffer _quadVertexBuffer = nullptr;

    bool _initialized = false;
};

// Factory implementation
Result<ShaderManager::Ptr> ShaderManager::createImpl(ContextType&, const GPUContext& gpu) noexcept {
    auto impl = Ptr(new ShaderManagerImpl());
    if (auto res = static_cast<ShaderManagerImpl*>(impl.get())->init(gpu); !res) {
        return Err<Ptr>("ShaderManager init failed", res);
    }
    return Ok(std::move(impl));
}

ShaderManagerImpl::~ShaderManagerImpl() {
    if (_shaderModule) {
        wgpuShaderModuleRelease(_shaderModule);
        _shaderModule = nullptr;
    }
    if (_gridPipeline) {
        wgpuRenderPipelineRelease(_gridPipeline);
        _gridPipeline = nullptr;
    }
    if (_pipelineLayout) {
        wgpuPipelineLayoutRelease(_pipelineLayout);
        _pipelineLayout = nullptr;
    }
    if (_gridBindGroupLayout) {
        wgpuBindGroupLayoutRelease(_gridBindGroupLayout);
        _gridBindGroupLayout = nullptr;
    }
    if (_quadVertexBuffer) {
        wgpuBufferRelease(_quadVertexBuffer);
        _quadVertexBuffer = nullptr;
    }
}

Result<void> ShaderManagerImpl::init(const GPUContext& gpu) noexcept {
    if (_initialized) {
        return Ok();
    }

    if (!gpu.device) {
        return Err<void>("ShaderManager::init: null device");
    }

    _gpu = gpu;

    // Load base shader
    std::string shaderPath = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/gpu-screen.wgsl";
    if (auto res = loadBaseShader(shaderPath); !res) {
        return res;
    }

    // Load all shader libraries from lib directory
    std::string libDir = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/lib";
    if (std::filesystem::exists(libDir) && std::filesystem::is_directory(libDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(libDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".wgsl") {
                std::ifstream libFile(entry.path());
                if (libFile.is_open()) {
                    std::stringstream buffer;
                    buffer << libFile.rdbuf();
                    std::string libCode = buffer.str();
                    if (!libCode.empty()) {
                        std::string libName = entry.path().stem().string();
                        addLibrary(libName, libCode);
                        yinfo("ShaderManager: loaded library '{}' ({} bytes)", libName, libCode.size());
                    }
                }
            }
        }
    } else {
        ywarn("ShaderManager: lib directory not found at {}", libDir);
    }

    // Load pre-effect and post-effect shaders
    auto loadEffects = [this](const std::string& dir, std::vector<EffectFile>& dest, const std::string& prefix) {
        if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
            yinfo("ShaderManager: no {} directory at {}", prefix, dir);
            return;
        }
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (!entry.is_regular_file() || entry.path().extension() != ".wgsl") continue;
            std::ifstream f(entry.path());
            if (!f.is_open()) continue;
            std::stringstream buf;
            buf << f.rdbuf();
            std::string code = buf.str();
            if (code.empty()) continue;

            // Parse "// Index: N" from file header
            uint32_t idx = 0;
            auto idxPos = code.find("// Index: ");
            if (idxPos != std::string::npos) {
                try { idx = std::stoul(code.substr(idxPos + 10, 10)); } catch (...) {}
            }
            if (idx == 0) {
                // Try to parse from filename: "N-name.wgsl"
                std::string stem = entry.path().stem().string();
                auto dash = stem.find('-');
                if (dash != std::string::npos) {
                    try { idx = std::stoul(stem.substr(0, dash)); } catch (...) {}
                }
            }
            if (idx == 0) {
                ywarn("ShaderManager: skipping effect {} (no valid index)", entry.path().string());
                continue;
            }

            // Extract function name: find "fn <prefix>Effect_" or "fn <prefix>_"
            std::string funcName;
            auto fnPos = code.find("fn " + prefix + "Effect_");
            if (fnPos != std::string::npos) {
                auto nameStart = fnPos + 3;
                auto paren = code.find('(', nameStart);
                if (paren != std::string::npos) {
                    funcName = code.substr(nameStart, paren - nameStart);
                }
            }

            std::string name = entry.path().stem().string();
            dest.push_back({idx, name, funcName, code});
            yinfo("ShaderManager: loaded {} effect '{}' index={} func={}", prefix, name, idx, funcName);
        }
        // Sort by index
        std::sort(dest.begin(), dest.end(), [](const EffectFile& a, const EffectFile& b) {
            return a.index < b.index;
        });
    };

    std::string preEffectsDir = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/pre-effects";
    std::string postEffectsDir = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/post-effects";
    loadEffects(preEffectsDir, _preEffects, "pre");
    loadEffects(postEffectsDir, _postEffects, "post");

    _initialized = true;
    yinfo("ShaderManager: initialized ({} pre-effects, {} post-effects)", _preEffects.size(), _postEffects.size());
    return Ok();
}

void ShaderManagerImpl::addProvider(std::shared_ptr<ShaderProvider> provider) {
    if (provider) {
        _providers.push_back(std::move(provider));
        ydebug("ShaderManager: provider registered ({} total)", _providers.size());
    }
}

void ShaderManagerImpl::addLibrary(const std::string& name, const std::string& code) {
    _libraries[name] = code;
    yinfo("ShaderManager: added library '{}'", name);
}

Result<void> ShaderManagerImpl::loadBaseShader(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return Err<void>("Failed to open shader file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    _baseShader = buffer.str();

    if (_baseShader.empty()) {
        return Err<void>("Shader file is empty: " + path);
    }

    yinfo("ShaderManager: loaded base shader from {} ({} lines)",
          path, std::count(_baseShader.begin(), _baseShader.end(), '\n') + 1);

    return Ok();
}

bool ShaderManagerImpl::needsRecompile() const {
    // Check if any provider is dirty
    for (const auto& provider : _providers) {
        if (provider && provider->isDirty()) {
            return true;
        }
    }
    // Also need initial compile if no shader module exists
    return _shaderModule == nullptr;
}

void ShaderManagerImpl::update() {
    if (needsRecompile()) {
        if (auto res = compile(); !res) {
            yerror("ShaderManager::update: recompile failed: {}", res.error().message());
        }
    }
}

std::string ShaderManagerImpl::mergeShaders() const {
    std::string result = _baseShader;

    // Collect all function code
    std::string allFunctions;
    allFunctions.reserve(64 * 1024);

    // Add library code first
    for (const auto& [name, code] : _libraries) {
        allFunctions += "// Library: " + name + "\n";
        allFunctions += code;
        allFunctions += "\n\n";
    }

    // Add provider function code
    for (const auto& provider : _providers) {
        if (provider) {
            allFunctions += provider->getCode();
        }
    }

    // Collect all dispatch code
    std::string allDispatch;
    allDispatch.reserve(16 * 1024);

    for (const auto& provider : _providers) {
        if (provider) {
            std::string dispatch = provider->getDispatchCode();
            if (!dispatch.empty()) {
                if (!allDispatch.empty()) {
                    allDispatch += " else ";
                }
                allDispatch += dispatch;
            }
        }
    }

    // Replace placeholders
    auto replacePlaceholder = [](std::string& str, const std::string& placeholder,
                                  const std::string& replacement) {
        size_t pos = str.find(placeholder);
        if (pos != std::string::npos) {
            str.replace(pos, placeholder.length(), replacement);
            return true;
        }
        return false;
    };

    if (!replacePlaceholder(result, FUNCTIONS_PLACEHOLDER, allFunctions)) {
        ywarn("ShaderManager: functions placeholder not found in base shader");
    }

    yinfo("ShaderManager: dispatch code length={}", allDispatch.size());
    // Log first and last parts of dispatch code to see all glyphs
    if (allDispatch.size() > 200) {
        yinfo("  dispatch start: '{}'", allDispatch.substr(0, 200));
        yinfo("  dispatch end: '{}'", allDispatch.substr(allDispatch.size() - 200));
    } else {
        yinfo("  dispatch: '{}'", allDispatch);
    }

    if (!replacePlaceholder(result, DISPATCH_PLACEHOLDER, allDispatch)) {
        ywarn("ShaderManager: dispatch placeholder not found in base shader");
    }

    // --- Effect placeholders ---

    // Pre-effect functions
    std::string preEffectFunctions;
    for (const auto& ef : _preEffects) {
        preEffectFunctions += "// Pre-effect: " + ef.name + "\n";
        preEffectFunctions += ef.code + "\n\n";
    }
    if (!replacePlaceholder(result, PRE_EFFECT_FUNCTIONS_PLACEHOLDER, preEffectFunctions)) {
        ywarn("ShaderManager: pre-effect functions placeholder not found");
    }

    // Pre-effect apply dispatch
    // Pre-effects modify glyphIndex: glyphIndex = preEffect_xxx(glyphIndex, cellCol, cellRow, time, params)
    std::string preEffectApply;
    if (!_preEffects.empty()) {
        preEffectApply = "if (grid.preEffectIndex != 0u) {\n";
        for (size_t i = 0; i < _preEffects.size(); i++) {
            const auto& ef = _preEffects[i];
            if (ef.funcName.empty()) continue;
            if (i > 0) preEffectApply += " else ";
            preEffectApply += "    if (grid.preEffectIndex == " + std::to_string(ef.index) + "u) {\n";
            preEffectApply += "        glyphIndex = " + ef.funcName + "(glyphIndex, cellCol, cellRow, globals.time, array<f32, 6>(grid.preEffectP0, grid.preEffectP1, grid.preEffectP2, grid.preEffectP3, grid.preEffectP4, grid.preEffectP5));\n";
            preEffectApply += "    }";
        }
        preEffectApply += "\n    }\n";
    }
    if (!replacePlaceholder(result, PRE_EFFECT_APPLY_PLACEHOLDER, preEffectApply)) {
        ywarn("ShaderManager: pre-effect apply placeholder not found");
    }

    // Post-effect functions
    std::string postEffectFunctions;
    for (const auto& ef : _postEffects) {
        postEffectFunctions += "// Post-effect: " + ef.name + "\n";
        postEffectFunctions += ef.code + "\n\n";
    }
    if (!replacePlaceholder(result, POST_EFFECT_FUNCTIONS_PLACEHOLDER, postEffectFunctions)) {
        ywarn("ShaderManager: post-effect functions placeholder not found");
    }

    // Post-effect apply dispatch
    // Post-effects modify finalColor: finalColor = postEffect_xxx(finalColor, pixelPos, screenSize, time, params)
    std::string postEffectApply;
    if (!_postEffects.empty()) {
        postEffectApply = "if (grid.postEffectIndex != 0u) {\n";
        for (size_t i = 0; i < _postEffects.size(); i++) {
            const auto& ef = _postEffects[i];
            if (ef.funcName.empty()) continue;
            if (i > 0) postEffectApply += " else ";
            postEffectApply += "    if (grid.postEffectIndex == " + std::to_string(ef.index) + "u) {\n";
            postEffectApply += "        finalColor = " + ef.funcName + "(finalColor, fbPixelPos, vec2<f32>(globals.screenWidth, globals.screenHeight), globals.time, array<f32, 6>(grid.postEffectP0, grid.postEffectP1, grid.postEffectP2, grid.postEffectP3, grid.postEffectP4, grid.postEffectP5));\n";
            postEffectApply += "    }";
        }
        postEffectApply += "\n    }\n";
    }
    if (!replacePlaceholder(result, POST_EFFECT_APPLY_PLACEHOLDER, postEffectApply)) {
        ywarn("ShaderManager: post-effect apply placeholder not found");
    }

    return result;
}

Result<void> ShaderManagerImpl::compile() {
    if (_baseShader.empty()) {
        return Err<void>("ShaderManager: no base shader loaded");
    }

    WGPUDevice device = _gpu.device;

    // Merge all shader code
    _mergedSource = mergeShaders();

    int lineCount = static_cast<int>(std::count(_mergedSource.begin(), _mergedSource.end(), '\n')) + 1;
    yinfo("ShaderManager: compiling merged shader ({} lines)", lineCount);

    // Release old shader module if exists
    if (_shaderModule) {
        wgpuShaderModuleRelease(_shaderModule);
        _shaderModule = nullptr;
    }

    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    WGPU_SHADER_CODE(wgslDesc, _mergedSource);

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.label = WGPU_STR("terminal shader");
    shaderDesc.nextInChain = &wgslDesc.chain;

    _shaderModule = wgpuDeviceCreateShaderModule(device, &shaderDesc);
    if (!_shaderModule) {
        yerror("ShaderManager: failed to compile shader - dumping source:");
        std::istringstream iss(_mergedSource);
        std::string line;
        int lineNum = 1;
        while (std::getline(iss, line)) {
            yerror("{:4d}: {}", lineNum++, line);
        }
        return Err<void>("Failed to compile shader module");
    }

    // Clear dirty flags on all providers
    for (auto& provider : _providers) {
        if (provider) {
            provider->clearDirty();
        }
    }

    // Recreate pipeline with new shader module
    // The pipeline references the shader module, so it must be recreated when
    // the shader is recompiled. Keep layouts and vertex buffer intact.
    if (_gridPipeline) {
        wgpuRenderPipelineRelease(_gridPipeline);
        _gridPipeline = nullptr;
    }
    if (!_gridBindGroupLayout || !_pipelineLayout || !_quadVertexBuffer) {
        // First compile — create everything
        if (auto res = createPipelineResources(); !res) {
            return res;
        }
    } else {
        // Recompile — only recreate the pipeline with new shader module
        if (auto res = recreatePipeline(); !res) {
            return res;
        }
    }

    // Count total functions
    uint32_t totalFunctions = 0;
    for (const auto& provider : _providers) {
        if (provider) {
            totalFunctions += provider->getFunctionCount();
        }
    }

    yinfo("ShaderManager: compiled successfully ({} shader functions)", totalFunctions);
    return Ok();
}

Result<void> ShaderManagerImpl::createPipelineResources() {
    WGPUDevice device = _gpu.device;

    // 1. Create quad vertex buffer
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
    _quadVertexBuffer = wgpuDeviceCreateBuffer(device, &quadDesc);
    if (!_quadVertexBuffer) {
        return Err<void>("Failed to create quad vertex buffer");
    }
    void* mapped = wgpuBufferGetMappedRange(_quadVertexBuffer, 0, sizeof(quadVertices));
    memcpy(mapped, quadVertices, sizeof(quadVertices));
    wgpuBufferUnmap(_quadVertexBuffer);

    // 2. Create grid bind group layout (8 bindings)
    WGPUBindGroupLayoutEntry entries[8] = {};

    // 0: Grid uniforms
    entries[0].binding = 0;
    entries[0].visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    entries[0].buffer.type = WGPUBufferBindingType_Uniform;

    // 1: Font atlas texture
    entries[1].binding = 1;
    entries[1].visibility = WGPUShaderStage_Fragment;
    entries[1].texture.sampleType = WGPUTextureSampleType_Float;
    entries[1].texture.viewDimension = WGPUTextureViewDimension_2D;

    // 2: Font sampler
    entries[2].binding = 2;
    entries[2].visibility = WGPUShaderStage_Fragment;
    entries[2].sampler.type = WGPUSamplerBindingType_Filtering;

    // 3: Glyph metadata SSBO
    entries[3].binding = 3;
    entries[3].visibility = WGPUShaderStage_Fragment;
    entries[3].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // 4: Cell buffer SSBO
    entries[4].binding = 4;
    entries[4].visibility = WGPUShaderStage_Fragment;
    entries[4].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // 5: Bitmap/Emoji atlas texture
    entries[5].binding = 5;
    entries[5].visibility = WGPUShaderStage_Fragment;
    entries[5].texture.sampleType = WGPUTextureSampleType_Float;
    entries[5].texture.viewDimension = WGPUTextureViewDimension_2D;

    // 6: Bitmap/Emoji sampler
    entries[6].binding = 6;
    entries[6].visibility = WGPUShaderStage_Fragment;
    entries[6].sampler.type = WGPUSamplerBindingType_Filtering;

    // 7: Bitmap/Emoji metadata SSBO
    entries[7].binding = 7;
    entries[7].visibility = WGPUShaderStage_Fragment;
    entries[7].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 8;
    layoutDesc.entries = entries;
    _gridBindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &layoutDesc);
    if (!_gridBindGroupLayout) {
        return Err<void>("Failed to create grid bind group layout");
    }

    // 3. Create pipeline layout (group 0: shared, group 1: grid)
    WGPUBindGroupLayout layouts[2] = { _gpu.sharedBindGroupLayout, _gridBindGroupLayout };
    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.bindGroupLayoutCount = 2;
    pipelineLayoutDesc.bindGroupLayouts = layouts;
    _pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &pipelineLayoutDesc);
    if (!_pipelineLayout) {
        return Err<void>("Failed to create pipeline layout");
    }

    // 4. Create render pipeline
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
    colorTarget.format = _gpu.surfaceFormat;
    colorTarget.blend = &blendState;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragState = {};
    fragState.module = _shaderModule;
    fragState.entryPoint = WGPU_STR("fs_main");
    fragState.targetCount = 1;
    fragState.targets = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.label = WGPU_STR("grid pipeline");
    pipelineDesc.layout = _pipelineLayout;
    pipelineDesc.vertex.module = _shaderModule;
    pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
    pipelineDesc.vertex.bufferCount = 1;
    pipelineDesc.vertex.buffers = &vertexLayout;
    pipelineDesc.fragment = &fragState;
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
    pipelineDesc.primitive.cullMode = WGPUCullMode_None;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = 0xFFFFFFFF;

    // Use error scope to catch deferred validation errors synchronously
    wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);

    _gridPipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

    bool hadError = false;
    std::string scopeErrorMsg;
    WGPUPopErrorScopeCallbackInfo popInfo = {};
    popInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    popInfo.callback = [](WGPUPopErrorScopeStatus status, WGPUErrorType type,
                          WGPUStringView message, void* userdata1, void* userdata2) {
        if (type != WGPUErrorType_NoError) {
            *static_cast<bool*>(userdata1) = true;
            auto* msg = static_cast<std::string*>(userdata2);
            *msg = message.data ? std::string(message.data, message.length) : "unknown";
        }
    };
    popInfo.userdata1 = &hadError;
    popInfo.userdata2 = &scopeErrorMsg;
    wgpuDevicePopErrorScope(device, popInfo);

    if (hadError || !_gridPipeline) {
        return Err<void>("Failed to create grid render pipeline: " + scopeErrorMsg);
    }

    yinfo("ShaderManager: created shared pipeline resources");
    return Ok();
}

Result<void> ShaderManagerImpl::recreatePipeline() {
    WGPUDevice device = _gpu.device;

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
    colorTarget.format = _gpu.surfaceFormat;
    colorTarget.blend = &blendState;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragState = {};
    fragState.module = _shaderModule;
    fragState.entryPoint = WGPU_STR("fs_main");
    fragState.targetCount = 1;
    fragState.targets = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.label = WGPU_STR("grid pipeline");
    pipelineDesc.layout = _pipelineLayout;
    pipelineDesc.vertex.module = _shaderModule;
    pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
    pipelineDesc.vertex.bufferCount = 1;
    pipelineDesc.vertex.buffers = &vertexLayout;
    pipelineDesc.fragment = &fragState;
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
    pipelineDesc.primitive.cullMode = WGPUCullMode_None;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = 0xFFFFFFFF;

    wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);

    _gridPipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

    bool hadError = false;
    std::string scopeErrorMsg;
    WGPUPopErrorScopeCallbackInfo popInfo = {};
    popInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    popInfo.callback = [](WGPUPopErrorScopeStatus status, WGPUErrorType type,
                          WGPUStringView message, void* userdata1, void* userdata2) {
        if (type != WGPUErrorType_NoError) {
            *static_cast<bool*>(userdata1) = true;
            auto* msg = static_cast<std::string*>(userdata2);
            *msg = message.data ? std::string(message.data, message.length) : "unknown";
        }
    };
    popInfo.userdata1 = &hadError;
    popInfo.userdata2 = &scopeErrorMsg;
    wgpuDevicePopErrorScope(device, popInfo);

    if (hadError || !_gridPipeline) {
        return Err<void>("Failed to recreate grid render pipeline: " + scopeErrorMsg);
    }

    yinfo("ShaderManager: recreated pipeline after shader recompile");
    return Ok();
}

} // namespace yetty
