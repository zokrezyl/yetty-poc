#pragma once

#include <yetty/result.hpp>
#include <webgpu/webgpu.h>
#include <string>
#include <unordered_map>

namespace yetty {

class WebGPUContext;

//-----------------------------------------------------------------------------
// ShaderManager - caches compiled shader modules (owned by Yetty)
//
// Used by ShaderGlyphRenderer for animated glyphs and by ShaderToy plugin.
// Just a cache - no codepoint or glyph knowledge.
//-----------------------------------------------------------------------------
class ShaderManager {
public:
    ShaderManager() = default;
    ~ShaderManager();

    // Initialize with device and shader search path
    Result<void> init(WGPUDevice device, const std::string& shaderPath);
    void dispose();

    // Get compiled fragment shader by name (loads and caches)
    Result<WGPUShaderModule> getFragmentShader(const std::string& name);

    // Shared vertex shader for rendering quads
    WGPUShaderModule getQuadVertexShader();

    // Check if shader exists (without loading)
    bool hasShader(const std::string& name) const;

    // Get shader path
    const std::string& shaderPath() const { return _shaderPath; }

private:
    Result<WGPUShaderModule> loadShader(const std::string& filename);
    Result<void> createQuadVertexShader();

    WGPUDevice _device = nullptr;
    std::string _shaderPath;

    // Shared vertex shader for all quad-based rendering
    WGPUShaderModule _quadVertexShader = nullptr;

    // Cached fragment shaders by name
    std::unordered_map<std::string, WGPUShaderModule> _fragmentShaders;
};

} // namespace yetty
