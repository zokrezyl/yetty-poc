#pragma once

#include <yetty/result.hpp>
#include <memory>
#include <cstdint>
#include <vector>

namespace yetty {

class YettyFont {
public:
    using Ptr = std::shared_ptr<YettyFont>;

    enum class Style : uint8_t {
        Regular = 0,
        Bold = 1,
        Italic = 2,
        BoldItalic = 3
    };

    virtual ~YettyFont() = default;

    // Get glyph index - handles variants with fallback
    // Returns uint32_t to support shader glyphs in Plane 16 PUA-B range
    virtual uint32_t getGlyphIndex(uint32_t codepoint) = 0;
    virtual uint32_t getGlyphIndex(uint32_t codepoint, Style style) = 0;
    virtual uint32_t getGlyphIndex(uint32_t codepoint, bool bold, bool italic) = 0;

    // Upload atlas to GPU - called when dirty
    virtual void uploadToGpu() = 0;

    // Check if atlas needs re-upload
    virtual bool isDirty() const = 0;
    virtual void clearDirty() = 0;

    // Atlas properties
    virtual uint32_t getAtlasWidth() const = 0;
    virtual uint32_t getAtlasHeight() const = 0;
    virtual const std::vector<uint8_t>& getAtlasData() const = 0;

protected:
    YettyFont() = default;

    // Init method - called by factory, can fail
    virtual Result<void> init() = 0;
};

} // namespace yetty
