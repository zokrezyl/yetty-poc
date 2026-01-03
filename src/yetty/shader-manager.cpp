#include <yetty/shader-manager.h>
#include <yetty/wgpu-compat.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace yetty {

ShaderManager::~ShaderManager() {
    dispose();
}

Result<void> ShaderManager::init(WGPUDevice device, const std::string& shaderPath) {
    if (!device) {
        return Err<void>("ShaderManager::init: null device");
    }

    _device = device;
    _shaderPath = shaderPath;

    // Ensure path ends with separator
    if (!_shaderPath.empty() && _shaderPath.back() != '/') {
        _shaderPath += '/';
    }

    // Create shared quad vertex shader
    if (auto res = createQuadVertexShader(); !res) {
        return Err<void>("ShaderManager: failed to create quad vertex shader", res);
    }

    spdlog::info("ShaderManager: initialized with path {}", _shaderPath);
    return Ok();
}

void ShaderManager::dispose() {
    for (auto& [name, module] : _fragmentShaders) {
        if (module) {
            wgpuShaderModuleRelease(module);
        }
    }
    _fragmentShaders.clear();

    if (_quadVertexShader) {
        wgpuShaderModuleRelease(_quadVertexShader);
        _quadVertexShader = nullptr;
    }

    _device = nullptr;
}

Result<void> ShaderManager::createQuadVertexShader() {
    const char* vertCode = R"(
struct QuadUniforms {
    iTime: f32,
    iTimeDelta: f32,
    iFrame: u32,
    _pad0: u32,
    iResolution: vec2<f32>,
    iMouse: vec2<f32>,
    iParam0: u32,
    _pad1a: u32,
    _pad1b: u32,
    _pad1c: u32,
    rect: vec4<f32>,
}

@group(0) @binding(0) var<uniform> u: QuadUniforms;

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
}

@vertex
fn vs_main(@builtin(vertex_index) vertexIndex: u32) -> VertexOutput {
    var positions = array<vec2<f32>, 6>(
        vec2<f32>(0.0, 0.0), vec2<f32>(1.0, 0.0), vec2<f32>(1.0, 1.0),
        vec2<f32>(0.0, 0.0), vec2<f32>(1.0, 1.0), vec2<f32>(0.0, 1.0)
    );

    let pos = positions[vertexIndex];
    let ndcX = u.rect.x + pos.x * u.rect.z;
    let ndcY = u.rect.y - pos.y * u.rect.w;

    var output: VertexOutput;
    output.position = vec4<f32>(ndcX, ndcY, 0.0, 1.0);
    output.uv = pos;
    return output;
}
)";

    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = WGPU_STR(vertCode);

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;

    _quadVertexShader = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
    if (!_quadVertexShader) {
        return Err<void>("Failed to compile quad vertex shader");
    }

    return Ok();
}

WGPUShaderModule ShaderManager::getQuadVertexShader() {
    return _quadVertexShader;
}

bool ShaderManager::hasShader(const std::string& name) const {
    // Check cache first
    if (_fragmentShaders.count(name) > 0) {
        return true;
    }

    // Check if file exists
    std::string fullPath = _shaderPath + name;
    return std::filesystem::exists(fullPath);
}

Result<WGPUShaderModule> ShaderManager::getFragmentShader(const std::string& name) {
    // Check cache
    auto it = _fragmentShaders.find(name);
    if (it != _fragmentShaders.end()) {
        return Ok(it->second);
    }

    // Load and compile
    auto result = loadShader(name);
    if (!result) {
        return Err<WGPUShaderModule>("Failed to load shader: " + name, result);
    }

    _fragmentShaders[name] = *result;
    spdlog::debug("ShaderManager: loaded and cached shader {}", name);
    return Ok(*result);
}

Result<WGPUShaderModule> ShaderManager::loadShader(const std::string& filename) {
    std::string fullPath = _shaderPath + filename;

    // Read file
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        return Err<WGPUShaderModule>("Failed to open shader file: " + fullPath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    if (source.empty()) {
        return Err<WGPUShaderModule>("Shader file is empty: " + fullPath);
    }

    // Prepend uniforms struct and Shadertoy-compatible globals
    std::string fullSource = R"(
struct QuadUniforms {
    iTime: f32,
    iTimeDelta: f32,
    iFrame: u32,
    _pad0: u32,
    iResolution: vec2<f32>,
    iMouse: vec2<f32>,
    iParam0: u32,
    _pad1a: u32,
    _pad1b: u32,
    _pad1c: u32,
    rect: vec4<f32>,
}

@group(0) @binding(0) var<uniform> u: QuadUniforms;

// Shadertoy-compatible globals (set by entry point)
var<private> iTime: f32;
var<private> iResolution: vec2<f32>;
var<private> iMouse: vec2<f32>;

)" + source + R"(

// Entry point wrapper - calls user's fs_main
@fragment
fn fs_entry(@location(0) uv: vec2<f32>) -> @location(0) vec4<f32> {
    iTime = u.iTime;
    iResolution = u.iResolution;
    iMouse = u.iMouse;
    return fs_main(uv);
}
)";

    // Compile
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = { .data = fullSource.c_str(), .length = fullSource.size() };

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;

    WGPUShaderModule module = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
    if (!module) {
        return Err<WGPUShaderModule>("Failed to compile shader: " + filename);
    }

    spdlog::info("ShaderManager: compiled shader {}", filename);
    return Ok(module);
}

} // namespace yetty
