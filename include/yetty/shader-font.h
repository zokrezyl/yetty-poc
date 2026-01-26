#pragma once

#include <yetty/font.h>
#include <yetty/shader-provider.h>
#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace yetty {

class ShaderManager;

/**
 * ShaderFont implements both YettyFont and ShaderProvider interfaces.
 *
 * It manages procedurally rendered glyphs (shader glyphs or card glyphs)
 * that are rendered directly in the terminal shader rather than from a texture atlas.
 *
 * There are two categories:
 * - Category::Glyph: Single-cell animated glyphs (spinner, pulse, etc.)
 * - Category::Card: Multi-cell widget glyphs (image card, video player, etc.)
 *
 * Shader files are loaded from disk and merged into the main shader module.
 * File naming convention: 0xNNNN-name.wgsl where NNNN is hex offset within category.
 */
class ShaderFont : public Font,
                   public ShaderProvider,
                   public base::ObjectFactory<ShaderFont>,
                   public std::enable_shared_from_this<ShaderFont> {
public:
    using Ptr = std::shared_ptr<ShaderFont>;

    /**
     * Category determines the glyph type and base codepoint.
     */
    enum class Category {
        Glyph,  // Single-cell animated glyphs (base: 0x101000)
        Card    // Multi-cell widget glyphs (base: 0x100000)
    };

    /**
     * Create a ShaderFont for the specified category.
     *
     * @param ctx Factory context
     * @param shaderMgr ShaderManager to register with
     * @param category Glyph or Card category
     * @param shaderDir Directory containing shader files
     * @return ShaderFont instance or error
     */
    static Result<Ptr> createImpl(ContextType& ctx,
                                  std::shared_ptr<ShaderManager> shaderMgr,
                                  Category category,
                                  const std::string& shaderDir);

    ~ShaderFont() override = default;

    // =========================================================================
    // YettyFont interface
    // =========================================================================

    virtual uint32_t getGlyphIndex(uint32_t codepoint) = 0;
    virtual uint32_t getGlyphIndex(uint32_t codepoint, Style style) = 0;
    virtual uint32_t getGlyphIndex(uint32_t codepoint, bool bold, bool italic) = 0;

    virtual void uploadToGpu() = 0;
    virtual bool isDirty() const = 0;
    virtual void clearDirty() = 0;

    virtual uint32_t getAtlasWidth() const = 0;
    virtual uint32_t getAtlasHeight() const = 0;
    virtual const std::vector<uint8_t>& getAtlasData() const = 0;

    // =========================================================================
    // ShaderProvider interface
    // =========================================================================

    virtual std::string getCode() const = 0;
    virtual std::string getDispatchCode() const = 0;
    virtual uint32_t getFunctionCount() const = 0;

    // =========================================================================
    // ShaderFont specific methods
    // =========================================================================

    /**
     * Get the category of this shader font.
     */
    virtual Category getCategory() const = 0;

    /**
     * Get the base codepoint for this category.
     */
    virtual uint32_t getBaseCodepoint() const = 0;

    /**
     * Reload shaders from disk (for hot-reload support).
     */
    virtual Result<void> reload() = 0;

    /**
     * Get list of loaded shader names.
     */
    virtual std::vector<std::string> getShaderNames() const = 0;

protected:
    ShaderFont() = default;
};

} // namespace yetty
