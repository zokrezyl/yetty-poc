#pragma once

#include <yetty/shader-provider.h>
#include <yetty/gpu-context.h>
#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <webgpu/webgpu.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace yetty {

/**
 * ShaderManager manages the compilation and sharing of the terminal grid shader.
 *
 * It collects shader code from ShaderProvider instances (like ShaderFont) and merges
 * them with the base shader template to produce a shared shader module and pipeline.
 *
 * Features:
 * - ThreadSingleton - single instance shared across all terminals
 * - Providers self-register via addProvider(shared_from_this())
 * - Per-frame dirty check for hot-reload support
 * - Owns shared grid pipeline resources
 */
class ShaderManager : public base::ThreadSingleton<ShaderManager> {
    friend class base::ThreadSingleton<ShaderManager>;
public:
    using Ptr = std::shared_ptr<ShaderManager>;

    virtual ~ShaderManager();

    // Factory for ThreadSingleton
    static Ptr createImpl() noexcept;

    // Non-copyable
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    /**
     * Initialize with GPU context.
     * Loads base shader from default path.
     */
    Result<void> init(const GPUContext& gpu) noexcept;

    /**
     * Register a shader provider.
     * Providers self-register during their init via shared_from_this().
     */
    void addProvider(std::shared_ptr<ShaderProvider> provider);

    /**
     * Add a shared library.
     * Libraries provide reusable functions (e.g., SDF utilities).
     */
    void addLibrary(const std::string& name, const std::string& code);

    /**
     * Load base shader from file (called by init).
     */
    Result<void> loadBaseShader(const std::string& path);

    /**
     * Check if any provider needs recompilation.
     */
    bool needsRecompile() const;

    /**
     * Compile all shaders and create pipeline.
     * Call after all providers are registered.
     */
    Result<void> compile();

    /**
     * Per-frame update - checks dirty providers and recompiles if needed.
     */
    void update();

    // =========================================================================
    // Shared resources for GPUScreen instances
    // =========================================================================

    WGPUShaderModule getShaderModule() const { return _shaderModule; }
    WGPURenderPipeline getGridPipeline() const { return _gridPipeline; }
    WGPUBindGroupLayout getGridBindGroupLayout() const { return _gridBindGroupLayout; }
    WGPUBuffer getQuadVertexBuffer() const { return _quadVertexBuffer; }

    /**
     * Get the merged shader source code (for debugging).
     */
    const std::string& getMergedSource() const { return _mergedSource; }

protected:
    ShaderManager() = default;

private:
    Result<void> createPipelineResources();
    std::string mergeShaders() const;

    GPUContext _gpu = {};
    std::string _baseShader;
    std::vector<std::shared_ptr<ShaderProvider>> _providers;
    std::map<std::string, std::string> _libraries;
    std::string _mergedSource;

    // Shared GPU resources
    WGPUShaderModule _shaderModule = nullptr;
    WGPURenderPipeline _gridPipeline = nullptr;
    WGPUPipelineLayout _pipelineLayout = nullptr;
    WGPUBindGroupLayout _gridBindGroupLayout = nullptr;
    WGPUBuffer _quadVertexBuffer = nullptr;

    bool _initialized = false;
};

} // namespace yetty
