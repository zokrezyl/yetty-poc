#pragma once

#include <yetty/shader-provider.h>
#include <yetty/result.hpp>
#include <webgpu/webgpu.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace yetty {

/**
 * ShaderCompiler manages the compilation of the terminal shader module.
 *
 * It collects shader code from multiple ShaderProvider instances and merges
 * them with the base shader template to produce a complete shader module.
 *
 * Features:
 * - Collects function code from all registered providers
 * - Generates dispatch code (if/else chains) for shader glyphs
 * - Supports shared library code (SDF functions, etc.)
 * - Tracks dirty state for hot-reload support
 */
class ShaderCompiler {
public:
    using Ptr = std::shared_ptr<ShaderCompiler>;

    /**
     * Create a ShaderCompiler instance.
     *
     * @param device WebGPU device for shader module creation
     * @return ShaderCompiler instance or error
     */
    static Result<Ptr> create(WGPUDevice device);

    ~ShaderCompiler();

    // Non-copyable
    ShaderCompiler(const ShaderCompiler&) = delete;
    ShaderCompiler& operator=(const ShaderCompiler&) = delete;

    /**
     * Register a shader provider.
     * Providers contribute shader code and dispatch logic.
     *
     * @param provider Provider to register (must outlive this compiler)
     */
    void addProvider(ShaderProvider* provider);

    /**
     * Add a shared library.
     * Libraries provide reusable functions (e.g., SDF utilities).
     *
     * @param name Library name (for error messages)
     * @param code WGSL code for the library
     */
    void addLibrary(const std::string& name, const std::string& code);

    /**
     * Set the base shader template.
     * The template must contain placeholders:
     * - SHADER_GLYPH_FUNCTIONS_PLACEHOLDER - replaced with function code
     * - SHADER_GLYPH_DISPATCH_PLACEHOLDER - replaced with dispatch code
     *
     * @param code WGSL shader template
     */
    void setBaseShader(const std::string& code);

    /**
     * Load base shader from file.
     *
     * @param path Path to shader file
     * @return Success or error
     */
    Result<void> loadBaseShader(const std::string& path);

    /**
     * Check if any provider needs recompilation.
     *
     * @return true if compile() should be called
     */
    bool needsRecompile() const;

    /**
     * Compile all shaders into a shader module.
     * This merges provider code with the base shader and creates a new
     * WebGPU shader module.
     *
     * @return Success or error
     */
    Result<void> compile();

    /**
     * Get the compiled shader module.
     * Call compile() first if needsRecompile() returns true.
     *
     * @return Shader module (may be null if not compiled)
     */
    WGPUShaderModule getShaderModule() const { return _shaderModule; }

    /**
     * Get the merged shader source code (for debugging).
     *
     * @return WGSL source code
     */
    const std::string& getMergedSource() const { return _mergedSource; }

private:
    ShaderCompiler(WGPUDevice device);

    /**
     * Merge all shader code and replace placeholders.
     *
     * @return Merged WGSL source
     */
    std::string mergeShaders() const;

    WGPUDevice _device;
    std::string _baseShader;
    std::vector<ShaderProvider*> _providers;
    std::map<std::string, std::string> _libraries;
    WGPUShaderModule _shaderModule = nullptr;
    std::string _mergedSource;
};

} // namespace yetty
