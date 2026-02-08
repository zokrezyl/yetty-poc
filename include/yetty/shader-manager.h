#pragma once

#include <yetty/shader-provider.h>
#include <yetty/gpu-context.h>
#include <yetty/gpu-allocator.h>
#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <webgpu/webgpu.h>
#include <string>
#include <memory>

namespace yetty {

/**
 * ShaderManager manages the compilation and sharing of the terminal grid shader.
 *
 * It collects shader code from ShaderProvider instances (like ShaderFont) and merges
 * them with the base shader template to produce a shared shader module and pipeline.
 *
 * Features:
 * - Created via ObjectFactory with GPUContext
 * - Stored in YettyContext for access by higher-level components
 * - Providers self-register via addProvider(shared_from_this())
 * - Per-frame dirty check for hot-reload support
 * - Owns shared grid pipeline resources
 */
class ShaderManager : public base::ObjectFactory<ShaderManager> {
public:
    using Ptr = std::shared_ptr<ShaderManager>;

    virtual ~ShaderManager() = default;

    // Factory - creates ShaderManagerImpl
    static Result<Ptr> createImpl(ContextType& ctx, const GPUContext& gpu, GpuAllocator::Ptr allocator) noexcept;

    /**
     * Register a shader provider.
     * Providers self-register during their init via shared_from_this().
     * @param provider The shader provider
     * @param dispatchName Name of the dispatch placeholder (e.g., "shaderGlyphDispatch", "plotDispatch")
     */
    virtual void addProvider(std::shared_ptr<ShaderProvider> provider, const std::string& dispatchName) = 0;

    /**
     * Add a shared library.
     * Libraries provide reusable functions (e.g., SDF utilities).
     */
    virtual void addLibrary(const std::string& name, const std::string& code) = 0;

    /**
     * Check if any provider needs recompilation.
     */
    virtual bool needsRecompile() const = 0;

    /**
     * Compile all shaders and create pipeline.
     * Call after all providers are registered.
     */
    virtual Result<void> compile() = 0;

    /**
     * Per-frame update - checks dirty providers and recompiles if needed.
     */
    virtual void update() = 0;

    // =========================================================================
    // Shared resources for GPUScreen instances
    // =========================================================================

    virtual WGPUShaderModule getShaderModule() const = 0;
    virtual WGPURenderPipeline getGridPipeline() const = 0;
    virtual WGPUBindGroupLayout getGridBindGroupLayout() const = 0;
    virtual WGPUBuffer getQuadVertexBuffer() const = 0;

    /**
     * Get the merged shader source code (for debugging).
     */
    virtual const std::string& getMergedSource() const = 0;

protected:
    ShaderManager() = default;
};

} // namespace yetty
