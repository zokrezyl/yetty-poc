#pragma once

#include <string>
#include <cstdint>

namespace yetty {

/**
 * ShaderProvider interface for classes that contribute shader code.
 *
 * Classes implementing this interface provide:
 * - Function code (the actual shader glyph/card functions)
 * - Dispatch code (if statements to call the right function based on glyph index)
 *
 * The ShaderCompiler collects code from all providers and merges them
 * into the final shader module.
 */
class ShaderProvider {
public:
    virtual ~ShaderProvider() = default;

    /**
     * Get the merged shader function code from this provider.
     * This includes all shader glyph/card function definitions.
     *
     * @return WGSL function definitions
     */
    virtual std::string getCode() const = 0;

    /**
     * Get the dispatch code for this provider.
     * This generates if/else statements to call the appropriate function
     * based on glyph index.
     *
     * @return WGSL dispatch code (if statements)
     */
    virtual std::string getDispatchCode() const = 0;

    /**
     * Check if the provider needs recompilation.
     * This should return true if shader files have been modified since
     * the last compile.
     *
     * @return true if shaders need to be recompiled
     */
    virtual bool isDirty() const = 0;

    /**
     * Clear the dirty flag after compilation.
     */
    virtual void clearDirty() = 0;

    /**
     * Get the number of shader functions provided.
     *
     * @return Number of functions
     */
    virtual uint32_t getFunctionCount() const = 0;

protected:
    ShaderProvider() = default;
};

} // namespace yetty
