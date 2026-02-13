#pragma once

#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <memory>
#include <string>
#include <cstdint>
#include <vector>

namespace yetty {
struct YettyContext;
}

namespace yetty::card {

//=============================================================================
// TextureProvider - Interface for texture content providers
//
// Providers render content to RGBA pixel buffers. The Texture card owns
// the GPU resources and calls the provider to fill pixel data when needed.
//
// Examples of providers:
//   - ImageProvider: loads images via stb_image (PNG, JPEG, etc.)
//   - PdfProvider: renders PDF pages via PDFium
//   - VideoProvider: decodes video frames
//   - GeneratedProvider: procedurally generated textures
//
// The provider receives:
//   - Card dimensions (widthCells, heightCells)
//   - Cell size in pixels (from terminal)
//   - Zoom level (card-level zoom, separate from terminal zoom)
//   - Args string for provider-specific options
//   - Payload (binary data, e.g., image file contents)
//
// The provider outputs:
//   - RGBA8 pixel data
//   - Texture dimensions (width, height)
//   - isDirty flag for dynamic content
//=============================================================================

class TextureProvider : public base::ObjectFactory<TextureProvider> {
public:
    using Ptr = std::shared_ptr<TextureProvider>;

    //=========================================================================
    // Factory method - creates provider based on type name
    //=========================================================================
    static Result<Ptr> create(
        const std::string& providerType,
        const YettyContext& ctx,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    // Auto-detect provider from payload magic bytes
    static Result<Ptr> createAuto(
        const YettyContext& ctx,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    // Detect provider type from payload magic bytes
    // Returns empty string if unknown
    static std::string detectProviderType(const std::string& payload);

    virtual ~TextureProvider() = default;

    //=========================================================================
    // Lifecycle
    //=========================================================================
    virtual Result<void> init() = 0;
    virtual void dispose() = 0;

    //=========================================================================
    // Rendering
    //=========================================================================

    // Render content to RGBA buffer
    // Called when isDirty() returns true or on first render
    // Provider should resize buffer and set dimensions
    virtual Result<void> render(
        std::vector<uint8_t>& buffer,
        uint32_t& outWidth,
        uint32_t& outHeight) = 0;

    // Check if provider needs to re-render
    // For static content (images), returns true once after init
    // For dynamic content (video), may return true each frame
    virtual bool isDirty() const = 0;

    // Mark as clean after Texture card has consumed the render
    virtual void markClean() = 0;

    //=========================================================================
    // Update (for animated/dynamic content)
    //=========================================================================
    virtual void update(float time) = 0;

    //=========================================================================
    // Properties
    //=========================================================================
    virtual uint32_t textureWidth() const = 0;
    virtual uint32_t textureHeight() const = 0;

    // Provider type name (e.g., "image", "pdf")
    virtual const char* providerType() const = 0;

protected:
    TextureProvider() = default;
};

} // namespace yetty::card
