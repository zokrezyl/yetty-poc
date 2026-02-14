#pragma once

#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace yetty {

// Forward declarations (avoid pulling in heavy headers for binary writer)
class YDrawBuilder;
class FontManager;
class GpuAllocator;

//=============================================================================
// YDrawWriter — Abstract drawing target for ydraw content
//
// Content producers (PDF renderer, etc.) target this interface.
// Two implementations:
//   - YDrawWriterInternal: wraps YDrawBuilder for in-process GPU rendering
//   - YDrawWriterBinary: serializes to binary v2 for external tools
//=============================================================================
class YDrawWriter : public base::Object {
public:
    using Ptr = std::shared_ptr<YDrawWriter>;

    const char* typeName() const override { return "YDrawWriter"; }

    // Flags (must match YDrawBuilder constants)
    static constexpr uint32_t FLAG_UNIFORM_SCALE = 16;
    static constexpr uint32_t FLAG_CUSTOM_ATLAS  = 32;

    //=========================================================================
    // Font management
    //=========================================================================

    // Add font from TTF file path. Returns fontId for use in addText.
    virtual Result<int> addFont(const std::string& ttfPath) = 0;

    // Add font from raw TTF data in memory. Returns fontId.
    virtual Result<int> addFontData(const uint8_t* data, size_t size,
                                     const std::string& name) = 0;

    //=========================================================================
    // Text
    //=========================================================================

    // Place text at position. Returns total advance width (for cursor tracking).
    virtual float addText(float x, float y, const std::string& text,
                          float fontSize, uint32_t color, int fontId = 0) = 0;

    // Measure text width without placing glyphs.
    virtual float measureText(const std::string& text,
                              float fontSize, int fontId = 0) = 0;

    // Place rotated text. Each glyph becomes an SDF primitive.
    virtual float addRotatedText(float x, float y, const std::string& text,
                                  float fontSize, uint32_t color,
                                  float rotationRadians, int fontId = 0) = 0;

    //=========================================================================
    // Primitives
    //=========================================================================

    virtual void addBox(float cx, float cy, float halfW, float halfH,
                        uint32_t fillColor, uint32_t strokeColor = 0,
                        float strokeWidth = 0) = 0;

    virtual void addSegment(float x0, float y0, float x1, float y1,
                            uint32_t strokeColor, float strokeWidth = 1) = 0;

    //=========================================================================
    // Scene configuration
    //=========================================================================

    virtual void setSceneBounds(float minX, float minY,
                                float maxX, float maxY) = 0;
    virtual void setBgColor(uint32_t color) = 0;
    virtual void setFlags(uint32_t flags) = 0;
    virtual void addFlags(uint32_t flags) = 0;

protected:
    YDrawWriter() = default;
};

//=============================================================================
// YDrawWriterInternal — Wraps YDrawBuilder for in-process GPU rendering
//
// Cards use this to produce ydraw content and access the underlying builder
// for GPU buffer lifecycle (grid staging, glyph data, atlas upload).
//=============================================================================
class YDrawWriterInternal : public YDrawWriter,
                            public base::ObjectFactory<YDrawWriterInternal> {
public:
    using Ptr = std::shared_ptr<YDrawWriterInternal>;

    static Result<Ptr> createImpl(std::shared_ptr<FontManager> fontManager,
                                  std::shared_ptr<GpuAllocator> allocator);

    const char* typeName() const override { return "YDrawWriterInternal"; }

    // Access underlying builder for GPU lifecycle
    virtual std::shared_ptr<YDrawBuilder> builder() const = 0;

protected:
    YDrawWriterInternal() = default;
};

//=============================================================================
// YDrawWriterBinary — Serializes ydraw content to binary v2 format
//
// External tools (pdf2ydraw, etc.) use this to produce binary blobs
// that ydraw cards can parse. No yetty/GPU dependencies.
//=============================================================================
class YDrawWriterBinary : public YDrawWriter,
                          public base::ObjectFactory<YDrawWriterBinary> {
public:
    using Ptr = std::shared_ptr<YDrawWriterBinary>;

    static Result<Ptr> createImpl();

    const char* typeName() const override { return "YDrawWriterBinary"; }

    // Serialize accumulated content to binary v2 format
    virtual std::vector<uint8_t> buildBinary() const = 0;

    // Convenience: base64-encode the binary output
    virtual std::string buildBase64() const = 0;

protected:
    YDrawWriterBinary() = default;
};

} // namespace yetty
