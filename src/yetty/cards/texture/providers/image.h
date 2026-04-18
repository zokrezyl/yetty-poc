#pragma once

#include "../texture-provider.h"

namespace yetty::card {

//=============================================================================
// ImageProvider - Loads images via stb_image
//
// Supported formats: PNG, JPEG, BMP, GIF, PSD, TGA, HDR, PIC, PNM
//
// Provider args:
//   (none currently - future: --page for multi-page formats)
//
// Payload: raw image file bytes (PNG, JPEG, etc.)
//=============================================================================

class ImageProvider : public TextureProvider {
public:
    using Ptr = std::shared_ptr<ImageProvider>;

    //=========================================================================
    // Factory
    //=========================================================================
    static Result<Ptr> create(
        const YettyContext& ctx,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    virtual ~ImageProvider() = default;

    //=========================================================================
    // TextureProvider interface
    //=========================================================================
    Result<void> init() override = 0;
    void dispose() override = 0;

    Result<void> render(
        std::vector<uint8_t>& buffer,
        uint32_t& outWidth,
        uint32_t& outHeight) override = 0;

    bool isDirty() const override = 0;
    void markClean() override = 0;
    void update(float time) override = 0;

    uint32_t textureWidth() const override = 0;
    uint32_t textureHeight() const override = 0;

    const char* providerType() const override { return "image"; }

protected:
    ImageProvider() = default;
};

} // namespace yetty::card
