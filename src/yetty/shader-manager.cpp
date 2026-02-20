#include <yetty/shader-manager.h>
#include <yetty/gpu-allocator.h>
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
#include <atomic>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

// Shader directory path - can be overridden per platform
#ifndef YETTY_SHADERS_DIR
#define YETTY_SHADERS_DIR CMAKE_SOURCE_DIR "/src/yetty/shaders"
#endif

namespace yetty {

// Stub shader preamble for validating individual effect files
// Contains minimal declarations so effect code can reference grid.*, globals.*, and util_* functions
static const char* VALIDATION_STUB_PREAMBLE = R"(
// Minimal stubs for effect validation
struct SharedUniforms {
    time: f32,
    deltaTime: f32,
    screenWidth: f32,
    screenHeight: f32,
    mouseX: f32,
    mouseY: f32,
    lastChar: u32,
    lastCharTime: f32,
};

struct GridUniforms {
    projection: mat4x4<f32>,
    screenSize: vec2<f32>,
    cellSize: vec2<f32>,
    gridSize: vec2<f32>,
    pixelRange: f32,
    scale: f32,
    cursorPos: vec2<f32>,
    cursorVisible: f32,
    cursorShape: f32,
    viewportOrigin: vec2<f32>,
    cursorBlink: f32,
    hasSelection: f32,
    selStart: vec2<f32>,
    selEnd: vec2<f32>,
    preEffectIndex: u32,
    postEffectIndex: u32,
    preEffectP0: f32,
    preEffectP1: f32,
    preEffectP2: f32,
    preEffectP3: f32,
    preEffectP4: f32,
    preEffectP5: f32,
    postEffectP0: f32,
    postEffectP1: f32,
    postEffectP2: f32,
    postEffectP3: f32,
    postEffectP4: f32,
    postEffectP5: f32,
    defaultFg: u32,
    defaultBg: u32,
    spaceGlyph: u32,
    effectIndex: u32,
    effectP0: f32,
    effectP1: f32,
    effectP2: f32,
    effectP3: f32,
    effectP4: f32,
    effectP5: f32,
};

@group(0) @binding(0) var<uniform> globals: SharedUniforms;
@group(1) @binding(0) var<uniform> grid: GridUniforms;

// Stub utility functions (from lib/util.wgsl)
fn util_hash11(p_in: f32) -> f32 {
    var p = fract(p_in * 0.1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

fn util_hash21(p: vec2<f32>) -> f32 {
    var p3 = fract(vec3<f32>(p.x, p.y, p.x) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

fn util_hash31(p: vec3<f32>) -> f32 {
    var p3 = fract(p * 0.1031);
    p3 += dot(p3, p3.zyx + 31.32);
    return fract((p3.x + p3.y) * p3.z);
}

fn util_hash22(p: vec2<f32>) -> vec2<f32> {
    var p3 = fract(vec3<f32>(p.x, p.y, p.x) * vec3<f32>(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xx + p3.yz) * p3.zy);
}

fn util_hash33(p: vec3<f32>) -> vec3<f32> {
    var p3 = fract(p * vec3<f32>(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yxz + 33.33);
    return fract((p3.xxy + p3.yxx) * p3.zyx);
}

fn util_valueNoise(p: vec2<f32>) -> f32 {
    let i = floor(p);
    let f = fract(p);
    let u = f * f * (3.0 - 2.0 * f);
    return mix(
        mix(util_hash21(i + vec2<f32>(0.0, 0.0)),
            util_hash21(i + vec2<f32>(1.0, 0.0)), u.x),
        mix(util_hash21(i + vec2<f32>(0.0, 1.0)),
            util_hash21(i + vec2<f32>(1.0, 1.0)), u.x),
        u.y
    );
}

fn util_valueNoise3(p: vec3<f32>) -> f32 {
    let i = floor(p);
    let f = fract(p);
    let u = f * f * (3.0 - 2.0 * f);
    return mix(
        mix(mix(util_hash31(i + vec3<f32>(0.0, 0.0, 0.0)),
                util_hash31(i + vec3<f32>(1.0, 0.0, 0.0)), u.x),
            mix(util_hash31(i + vec3<f32>(0.0, 1.0, 0.0)),
                util_hash31(i + vec3<f32>(1.0, 1.0, 0.0)), u.x), u.y),
        mix(mix(util_hash31(i + vec3<f32>(0.0, 0.0, 1.0)),
                util_hash31(i + vec3<f32>(1.0, 0.0, 1.0)), u.x),
            mix(util_hash31(i + vec3<f32>(0.0, 1.0, 1.0)),
                util_hash31(i + vec3<f32>(1.0, 1.0, 1.0)), u.x), u.y),
        u.z
    );
}

fn util_colorNoise(uv: vec2<f32>) -> vec3<f32> {
    return vec3<f32>(
        util_valueNoise(uv * 256.0),
        util_valueNoise(uv * 256.0 + vec2<f32>(37.0, 17.0)),
        util_valueNoise(uv * 256.0 + vec2<f32>(59.0, 83.0))
    );
}

// Dummy entry points (required for valid shader module)
@vertex fn vs_validation_stub(@builtin(vertex_index) idx: u32) -> @builtin(position) vec4<f32> {
    return vec4<f32>(0.0);
}
@fragment fn fs_validation_stub() -> @location(0) vec4<f32> {
    return vec4<f32>(0.0);
}

// === EFFECT CODE BELOW ===
)";

// Validate a single shader file using WebGPU device
// Returns true if valid, false if compilation failed
// This catches errors early before merging all shaders together
static bool validateShaderFile(WGPUDevice device, const std::string& code,
                               const std::string& filename, std::string& errorMsg) {
    if (!device) {
        errorMsg = "No device for validation";
        return false;
    }

    // Build complete test shader
    std::string testShader = VALIDATION_STUB_PREAMBLE;
    testShader += code;

    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    WGPU_SHADER_CODE(wgslDesc, testShader);

    WGPUShaderModuleDescriptor desc = {};
    desc.label = WGPU_STR(filename.c_str());
    desc.nextInChain = &wgslDesc.chain;

    // Use error scope to capture validation errors
    wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);

    WGPUShaderModule module = wgpuDeviceCreateShaderModule(device, &desc);

    std::atomic<bool> callbackDone{false};
    bool hadError = false;
    std::string scopeError;

    WGPUPopErrorScopeCallbackInfo popInfo = {};
    popInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    popInfo.callback = [](WGPUPopErrorScopeStatus status, WGPUErrorType type,
                          WGPUStringView message, void* userdata1, void* userdata2) {
        if (type != WGPUErrorType_NoError) {
            *static_cast<bool*>(userdata1) = true;
            auto* msg = static_cast<std::string*>(userdata2);
            if (message.data && message.length > 0) {
                *msg = std::string(message.data, message.length);
            } else {
                *msg = "Unknown shader error";
            }
        }
    };
    popInfo.userdata1 = &hadError;
    popInfo.userdata2 = &scopeError;

    wgpuDevicePopErrorScope(device, popInfo);

    // Clean up test module
    if (module) {
        wgpuShaderModuleRelease(module);
    }

    if (hadError || !module) {
        // Adjust line numbers in error message (subtract preamble lines)
        // Count lines in preamble
        int preambleLines = 0;
        for (char c : std::string(VALIDATION_STUB_PREAMBLE)) {
            if (c == '\n') preambleLines++;
        }

        // Try to adjust line numbers in error message
        errorMsg = scopeError;
        // Common error format: ":123:45 error:" - adjust line number
        size_t pos = 0;
        while ((pos = errorMsg.find(':', pos)) != std::string::npos) {
            size_t numStart = pos + 1;
            size_t numEnd = numStart;
            while (numEnd < errorMsg.size() && std::isdigit(errorMsg[numEnd])) {
                numEnd++;
            }
            if (numEnd > numStart && numEnd < errorMsg.size() && errorMsg[numEnd] == ':') {
                // Found a line number
                int lineNum = std::stoi(errorMsg.substr(numStart, numEnd - numStart));
                int adjustedLine = lineNum - preambleLines;
                if (adjustedLine > 0) {
                    errorMsg = errorMsg.substr(0, numStart) +
                               std::to_string(adjustedLine) +
                               errorMsg.substr(numEnd);
                }
                break;  // Only adjust first line number
            }
            pos = numEnd;
        }

        return false;
    }

    return true;
}

// Placeholder markers in the base shader
static const char* FUNCTIONS_PLACEHOLDER = "// SHADER_GLYPH_FUNCTIONS_PLACEHOLDER";
static const char* PRE_EFFECT_FUNCTIONS_PLACEHOLDER = "// PRE_EFFECT_FUNCTIONS_PLACEHOLDER";
static const char* PRE_EFFECT_APPLY_PLACEHOLDER = "// PRE_EFFECT_APPLY_PLACEHOLDER";
static const char* EFFECT_FUNCTIONS_PLACEHOLDER = "// EFFECT_FUNCTIONS_PLACEHOLDER";
static const char* EFFECT_APPLY_PLACEHOLDER = "// EFFECT_APPLY_PLACEHOLDER";
static const char* POST_EFFECT_FUNCTIONS_PLACEHOLDER = "// POST_EFFECT_FUNCTIONS_PLACEHOLDER";
static const char* POST_EFFECT_APPLY_PLACEHOLDER = "// POST_EFFECT_APPLY_PLACEHOLDER";

class ShaderManagerImpl : public ShaderManager {
public:
    ShaderManagerImpl() = default;
    ~ShaderManagerImpl() override;

    Result<void> init(const GPUContext& gpu, GpuAllocator::Ptr allocator) noexcept;

    void addProvider(std::shared_ptr<ShaderProvider> provider, const std::string& dispatchName) override;
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
    GpuAllocator::Ptr _allocator;
    std::string _baseShader;
    struct ProviderEntry {
        std::shared_ptr<ShaderProvider> provider;
        std::string dispatchName;
    };
    std::vector<ProviderEntry> _providers;
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
    std::vector<EffectFile> _effects;      // coord distortion effects (no prefix)
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
Result<ShaderManager::Ptr> ShaderManager::createImpl(ContextType&, const GPUContext& gpu, GpuAllocator::Ptr allocator) noexcept {
    auto impl = Ptr(new ShaderManagerImpl());
    if (auto res = static_cast<ShaderManagerImpl*>(impl.get())->init(gpu, std::move(allocator)); !res) {
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
        _allocator->releaseBuffer(_quadVertexBuffer);
        _quadVertexBuffer = nullptr;
    }
}

Result<void> ShaderManagerImpl::init(const GPUContext& gpu, GpuAllocator::Ptr allocator) noexcept {
    if (_initialized) {
        return Ok();
    }

    if (!gpu.device) {
        return Err<void>("ShaderManager::init: null device");
    }

    _gpu = gpu;
    _allocator = std::move(allocator);

    // Load base shader
    std::string shaderPath = std::string(YETTY_SHADERS_DIR) + "/gpu-screen.wgsl";
    if (auto res = loadBaseShader(shaderPath); !res) {
        return res;
    }

    // Load all shader libraries from lib directory
    std::string libDir = std::string(YETTY_SHADERS_DIR) + "/lib";
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

    // Load pre-effect and post-effect shaders with optional validation
    // Validation is enabled when WEBGPU_BACKEND_DAWN is defined (compile-time check)
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

            // Extract function name: find "fn <prefix>Effect_" (or "fn effect_" for empty prefix)
            std::string funcName;
            std::string searchPattern = prefix.empty() ? "fn effect_" : ("fn " + prefix + "Effect_");
            auto fnPos = code.find(searchPattern);
            if (fnPos != std::string::npos) {
                auto nameStart = fnPos + 3;
                auto paren = code.find('(', nameStart);
                if (paren != std::string::npos) {
                    funcName = code.substr(nameStart, paren - nameStart);
                }
            }

            std::string name = entry.path().stem().string();
            std::string filename = entry.path().filename().string();

            // TODO: Per-file WGSL validation is currently disabled.
            //
            // The intention was to validate each effect file individually before merging,
            // so that syntax errors would show file-specific error messages instead of
            // cryptic line numbers in the merged shader (e.g., ":932:9 error").
            //
            // However, the current implementation validates files in isolation with
            // incomplete stubs. Effects that use library functions (e.g., renderGlyphInCell
            // from lib/text.wgsl) fail validation even though they work fine in the
            // merged shader where all libraries are available.
            //
            // To fix: build validation stubs dynamically from the already-loaded _libraries
            // map, so all library functions are available during per-file validation.
            //
            // See: validateShaderFile() and VALIDATION_STUB_PREAMBLE above.
#if 0  // Disabled - needs complete library stubs
            std::string validationError;
            if (!validateShaderFile(_gpu.device, code, filename, validationError)) {
                yerror("ShaderManager: WGSL validation FAILED for {}", filename);
                yerror("  Error: {}", validationError);
                std::istringstream iss(code);
                std::string line;
                int lineNum = 1;
                yerror("  --- Source code ---");
                while (std::getline(iss, line)) {
                    yerror("  {:3d}: {}", lineNum++, line);
                }
                yerror("  --- End of source ---");
                ywarn("ShaderManager: skipping invalid effect {}", name);
                continue;
            }
            ydebug("ShaderManager: validated {} effect '{}'", prefix.empty() ? "coord" : prefix, name);
#endif

            dest.push_back({idx, name, funcName, code});
            yinfo("ShaderManager: loaded {} effect '{}' index={} func={}", prefix, name, idx, funcName);
        }
        // Sort by index
        std::sort(dest.begin(), dest.end(), [](const EffectFile& a, const EffectFile& b) {
            return a.index < b.index;
        });
    };

    std::string preEffectsDir = std::string(YETTY_SHADERS_DIR) + "/pre-effects";
    std::string effectsDir = std::string(YETTY_SHADERS_DIR) + "/effects";
    std::string postEffectsDir = std::string(YETTY_SHADERS_DIR) + "/post-effects";
    loadEffects(preEffectsDir, _preEffects, "pre");
    loadEffects(effectsDir, _effects, "");  // no prefix for coord effects
    loadEffects(postEffectsDir, _postEffects, "post");

    _initialized = true;
    yinfo("ShaderManager: initialized ({} pre-effects, {} effects, {} post-effects)",
          _preEffects.size(), _effects.size(), _postEffects.size());
    return Ok();
}

void ShaderManagerImpl::addProvider(std::shared_ptr<ShaderProvider> provider, const std::string& dispatchName) {
    if (provider) {
        _providers.push_back({std::move(provider), dispatchName});
        ydebug("ShaderManager: provider registered for '{}' ({} total)", dispatchName, _providers.size());
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
    for (const auto& entry : _providers) {
        if (entry.provider && entry.provider->isDirty()) {
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
    for (const auto& entry : _providers) {
        if (entry.provider) {
            allFunctions += entry.provider->getCode();
        }
    }

    // Collect dispatch code grouped by dispatch name
    std::map<std::string, std::string> dispatchByName;

    for (const auto& entry : _providers) {
        if (entry.provider) {
            std::string dispatch = entry.provider->getDispatchCode();
            if (!dispatch.empty()) {
                auto& target = dispatchByName[entry.dispatchName];
                if (!target.empty()) {
                    target += " else ";
                }
                target += dispatch;
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

    // Replace dispatch placeholders for each dispatch name
    for (const auto& [dispatchName, dispatchCode] : dispatchByName) {
        std::string placeholder = "// " + dispatchName;
        yinfo("ShaderManager: dispatch '{}' code length={}", dispatchName, dispatchCode.size());
        if (dispatchCode.size() > 200) {
            yinfo("  dispatch start: '{}'", dispatchCode.substr(0, 200));
        } else {
            yinfo("  dispatch: '{}'", dispatchCode);
        }
        if (!replacePlaceholder(result, placeholder, dispatchCode)) {
            ywarn("ShaderManager: dispatch placeholder '{}' not found in base shader", placeholder);
        }
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

    // Coord effect functions (no prefix)
    std::string effectFunctions;
    for (const auto& ef : _effects) {
        effectFunctions += "// Effect: " + ef.name + "\n";
        effectFunctions += ef.code + "\n\n";
    }
    if (!replacePlaceholder(result, EFFECT_FUNCTIONS_PLACEHOLDER, effectFunctions)) {
        ywarn("ShaderManager: effect functions placeholder not found");
    }

    // Coord effect apply dispatch
    // Effects modify distortedPos: distortedPos = effect_xxx(pixelPos)
    std::string effectApply;
    if (!_effects.empty()) {
        effectApply = "if (grid.effectIndex != 0u) {\n";
        for (size_t i = 0; i < _effects.size(); i++) {
            const auto& ef = _effects[i];
            if (ef.funcName.empty()) continue;
            if (i > 0) effectApply += " else ";
            effectApply += "    if (grid.effectIndex == " + std::to_string(ef.index) + "u) {\n";
            effectApply += "        distortedPos = " + ef.funcName + "(pixelPos);\n";
            effectApply += "    }";
        }
        effectApply += "\n    }\n";
    }
    if (!replacePlaceholder(result, EFFECT_APPLY_PLACEHOLDER, effectApply)) {
        ywarn("ShaderManager: effect apply placeholder not found");
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
    for (auto& entry : _providers) {
        if (entry.provider) {
            entry.provider->clearDirty();
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
    for (const auto& entry : _providers) {
        if (entry.provider) {
            totalFunctions += entry.provider->getFunctionCount();
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
    _quadVertexBuffer = _allocator->createBuffer(quadDesc);
    if (!_quadVertexBuffer) {
        return Err<void>("Failed to create quad vertex buffer");
    }
    void* mapped = wgpuBufferGetMappedRange(_quadVertexBuffer, 0, sizeof(quadVertices));
    memcpy(mapped, quadVertices, sizeof(quadVertices));
    wgpuBufferUnmap(_quadVertexBuffer);

    // 2. Create grid bind group layout (15 bindings)
    WGPUBindGroupLayoutEntry entries[15] = {};

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

    // 8: Vector font glyph buffer SSBO
    entries[8].binding = 8;
    entries[8].visibility = WGPUShaderStage_Fragment;
    entries[8].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // 9: Vector font offset table SSBO
    entries[9].binding = 9;
    entries[9].visibility = WGPUShaderStage_Fragment;
    entries[9].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // 10: Coverage font glyph buffer SSBO
    entries[10].binding = 10;
    entries[10].visibility = WGPUShaderStage_Fragment;
    entries[10].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // 11: Coverage font offset table SSBO
    entries[11].binding = 11;
    entries[11].visibility = WGPUShaderStage_Fragment;
    entries[11].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // 12: Raster font texture (R8Unorm)
    entries[12].binding = 12;
    entries[12].visibility = WGPUShaderStage_Fragment;
    entries[12].texture.sampleType = WGPUTextureSampleType_Float;
    entries[12].texture.viewDimension = WGPUTextureViewDimension_2D;

    // 13: Raster font sampler
    entries[13].binding = 13;
    entries[13].visibility = WGPUShaderStage_Fragment;
    entries[13].sampler.type = WGPUSamplerBindingType_Filtering;

    // 14: Raster font UV metadata SSBO
    entries[14].binding = 14;
    entries[14].visibility = WGPUShaderStage_Fragment;
    entries[14].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 15;
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
