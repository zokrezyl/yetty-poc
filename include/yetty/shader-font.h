#pragma once

#include <yetty/yetty-font.h>
#include <yetty/shader-provider.h>
#include <yetty/result.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>

namespace yetty {

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
class ShaderFont : public YettyFont,
                   public ShaderProvider,
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
     * @param category Glyph or Card category
     * @param shaderDir Directory containing shader files
     * @return ShaderFont instance or error
     */
    static Result<Ptr> create(Category category, const std::string& shaderDir);

    ~ShaderFont() override = default;

    // Non-copyable
    ShaderFont(const ShaderFont&) = delete;
    ShaderFont& operator=(const ShaderFont&) = delete;

    // =========================================================================
    // YettyFont interface
    // =========================================================================

    uint32_t getGlyphIndex(uint32_t codepoint) override;
    uint32_t getGlyphIndex(uint32_t codepoint, Style style) override;
    uint32_t getGlyphIndex(uint32_t codepoint, bool bold, bool italic) override;

    void uploadToGpu() override;
    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }

    // Shader fonts don't have texture atlases - return empty/zero
    uint32_t getAtlasWidth() const override { return 0; }
    uint32_t getAtlasHeight() const override { return 0; }
    const std::vector<uint8_t>& getAtlasData() const override { return _empty; }

    // =========================================================================
    // ShaderProvider interface
    // =========================================================================

    std::string getCode() const override;
    std::string getDispatchCode() const override;
    uint32_t getFunctionCount() const override;

    // =========================================================================
    // ShaderFont specific methods
    // =========================================================================

    /**
     * Get the category of this shader font.
     */
    Category getCategory() const { return _category; }

    /**
     * Get the base codepoint for this category.
     */
    uint32_t getBaseCodepoint() const { return _baseCodepoint; }

    /**
     * Reload shaders from disk (for hot-reload support).
     *
     * @return Success or error
     */
    Result<void> reload();

    /**
     * Get list of loaded shader names.
     */
    std::vector<std::string> getShaderNames() const;

protected:
    Result<void> init() override;

private:
    ShaderFont(Category category, const std::string& shaderDir);

    /**
     * Load all shader files from the configured directory.
     */
    Result<void> loadShaders();

    /**
     * Parse a shader file name to extract hex offset and name.
     * Expected format: 0xNNNN-name.wgsl or NNNN-name.wgsl
     *
     * @param filename File name to parse
     * @param offset Output: hex offset value
     * @param name Output: shader name
     * @return true if parsed successfully
     */
    bool parseShaderFilename(const std::string& filename, uint32_t& offset, std::string& name);

    Category _category;
    std::string _shaderDir;
    uint32_t _baseCodepoint;  // 0x100000 for cards, 0x101000 for glyphs

    struct ShaderEntry {
        uint32_t offset;      // Hex value from filename
        uint32_t codepoint;   // Full codepoint (base + offset)
        std::string name;     // e.g., "spinner"
        std::string code;     // Shader function code
    };
    std::vector<ShaderEntry> _entries;

    // Codepoint -> dispatch index (index into _entries)
    std::unordered_map<uint32_t, uint32_t> _codepointToIndex;

    bool _dirty = true;
    std::vector<uint8_t> _empty;  // Empty atlas placeholder
};

} // namespace yetty
