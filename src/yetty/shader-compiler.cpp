#include <yetty/shader-compiler.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

#include <fstream>
#include <sstream>
#include <algorithm>

namespace yetty {

// Placeholder markers in the base shader
static const char* FUNCTIONS_PLACEHOLDER = "// SHADER_GLYPH_FUNCTIONS_PLACEHOLDER";
static const char* DISPATCH_PLACEHOLDER = "// SHADER_GLYPH_DISPATCH_PLACEHOLDER";

ShaderCompiler::ShaderCompiler(WGPUDevice device) : _device(device) {}

ShaderCompiler::~ShaderCompiler() {
    if (_shaderModule) {
        wgpuShaderModuleRelease(_shaderModule);
        _shaderModule = nullptr;
    }
}

Result<ShaderCompiler::Ptr> ShaderCompiler::create(WGPUDevice device) {
    if (!device) {
        return Err<Ptr>("ShaderCompiler: null device");
    }
    return Ok(Ptr(new ShaderCompiler(device)));
}

void ShaderCompiler::addProvider(ShaderProvider* provider) {
    if (provider) {
        _providers.push_back(provider);
    }
}

void ShaderCompiler::addLibrary(const std::string& name, const std::string& code) {
    _libraries[name] = code;
    yinfo("ShaderCompiler: added library '{}'", name);
}

void ShaderCompiler::setBaseShader(const std::string& code) {
    _baseShader = code;
}

Result<void> ShaderCompiler::loadBaseShader(const std::string& path) {
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

    yinfo("ShaderCompiler: loaded base shader from {} ({} lines)",
          path, std::count(_baseShader.begin(), _baseShader.end(), '\n') + 1);

    return Ok();
}

bool ShaderCompiler::needsRecompile() const {
    // Check if any provider is dirty
    for (const auto* provider : _providers) {
        if (provider && provider->isDirty()) {
            return true;
        }
    }
    // Also need initial compile if no shader module exists
    return _shaderModule == nullptr;
}

std::string ShaderCompiler::mergeShaders() const {
    std::string result = _baseShader;

    // Collect all function code
    std::string allFunctions;
    allFunctions.reserve(64 * 1024); // Pre-allocate

    // Add library code first
    for (const auto& [name, code] : _libraries) {
        allFunctions += "// Library: " + name + "\n";
        allFunctions += code;
        allFunctions += "\n\n";
    }

    // Add provider function code
    for (const auto* provider : _providers) {
        if (provider) {
            allFunctions += provider->getCode();
        }
    }

    // Collect all dispatch code
    std::string allDispatch;
    allDispatch.reserve(16 * 1024);

    for (const auto* provider : _providers) {
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
        ywarn("ShaderCompiler: functions placeholder not found in base shader");
    }

    if (!replacePlaceholder(result, DISPATCH_PLACEHOLDER, allDispatch)) {
        ywarn("ShaderCompiler: dispatch placeholder not found in base shader");
    }

    return result;
}

Result<void> ShaderCompiler::compile() {
    if (_baseShader.empty()) {
        return Err<void>("ShaderCompiler: no base shader set");
    }

    // Merge all shader code
    _mergedSource = mergeShaders();

    int lineCount = static_cast<int>(std::count(_mergedSource.begin(), _mergedSource.end(), '\n')) + 1;
    yinfo("ShaderCompiler: compiling merged shader ({} lines)", lineCount);

    // Release old shader module if exists
    if (_shaderModule) {
        wgpuShaderModuleRelease(_shaderModule);
        _shaderModule = nullptr;
    }

    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = { .data = _mergedSource.c_str(), .length = _mergedSource.size() };

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.label = WGPU_STR("terminal shader");
    shaderDesc.nextInChain = &wgslDesc.chain;

    _shaderModule = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
    if (!_shaderModule) {
        // Dump source with line numbers for debugging
        yerror("ShaderCompiler: failed to compile shader - dumping source:");
        std::istringstream iss(_mergedSource);
        std::string line;
        int lineNum = 1;
        while (std::getline(iss, line)) {
            yerror("{:4d}: {}", lineNum++, line);
        }
        return Err<void>("Failed to compile shader module");
    }

    // Get compilation info for error/warning reporting
    WGPUCompilationInfoCallbackInfo cbInfo = {};
    cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    cbInfo.callback = [](WGPUCompilationInfoRequestStatus status,
                         WGPUCompilationInfo const* info,
                         void* userdata1, void* userdata2) {
        (void)status;
        (void)userdata2;
        auto* src = static_cast<std::string*>(userdata1);
        if (info && info->messageCount > 0) {
            for (size_t i = 0; i < info->messageCount; i++) {
                auto& msg = info->messages[i];
                const char* typeStr = "info";
                if (msg.type == WGPUCompilationMessageType_Error) typeStr = "error";
                else if (msg.type == WGPUCompilationMessageType_Warning) typeStr = "warning";

                std::string message(msg.message.data, msg.message.length);
                yinfo("Shader {}: line {}: {}", typeStr, msg.lineNum, message);

                if (msg.type == WGPUCompilationMessageType_Error && src) {
                    // Dump context around the error
                    std::istringstream iss(*src);
                    std::string line;
                    int lineNum = 1;
                    while (std::getline(iss, line)) {
                        if (static_cast<uint64_t>(lineNum) >= msg.lineNum - 2 &&
                            static_cast<uint64_t>(lineNum) <= msg.lineNum + 2) {
                            yerror("{:4d}: {}", lineNum, line);
                        }
                        lineNum++;
                    }
                }
            }
        }
    };
    cbInfo.userdata1 = new std::string(_mergedSource);
    cbInfo.userdata2 = nullptr;
    wgpuShaderModuleGetCompilationInfo(_shaderModule, cbInfo);

    // Clear dirty flags on all providers
    for (auto* provider : _providers) {
        if (provider) {
            provider->clearDirty();
        }
    }

    // Count total functions
    uint32_t totalFunctions = 0;
    for (const auto* provider : _providers) {
        if (provider) {
            totalFunctions += provider->getFunctionCount();
        }
    }

    yinfo("ShaderCompiler: compiled successfully ({} shader functions)", totalFunctions);
    return Ok();
}

} // namespace yetty
