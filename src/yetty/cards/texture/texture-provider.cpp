#include "texture-provider.h"
#include "providers/image.h"
#include <yetty/yetty-context.h>
#include <ytrace/ytrace.hpp>
#include <cstring>

namespace yetty::card {

//=============================================================================
// Magic byte detection
//=============================================================================

// Magic bytes for common formats
static constexpr uint8_t PNG_MAGIC[] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
static constexpr uint8_t JPEG_MAGIC[] = { 0xFF, 0xD8, 0xFF };
static constexpr uint8_t GIF87_MAGIC[] = { 'G', 'I', 'F', '8', '7', 'a' };
static constexpr uint8_t GIF89_MAGIC[] = { 'G', 'I', 'F', '8', '9', 'a' };
static constexpr uint8_t BMP_MAGIC[] = { 'B', 'M' };
static constexpr uint8_t WEBP_MAGIC[] = { 'R', 'I', 'F', 'F' };  // + "WEBP" at offset 8
static constexpr uint8_t PDF_MAGIC[] = { '%', 'P', 'D', 'F', '-' };
static constexpr uint8_t TIFF_LE_MAGIC[] = { 'I', 'I', 0x2A, 0x00 };
static constexpr uint8_t TIFF_BE_MAGIC[] = { 'M', 'M', 0x00, 0x2A };
static constexpr uint8_t PSD_MAGIC[] = { '8', 'B', 'P', 'S' };

static bool matchMagic(const std::string& payload, const uint8_t* magic, size_t len) {
    if (payload.size() < len) return false;
    return std::memcmp(payload.data(), magic, len) == 0;
}

std::string TextureProvider::detectProviderType(const std::string& payload) {
    if (payload.size() < 4) {
        return "";
    }

    // Check image formats (handled by stb_image)
    if (matchMagic(payload, PNG_MAGIC, sizeof(PNG_MAGIC))) {
        yinfo("TextureProvider::detectProviderType: detected PNG");
        return "image";
    }
    if (matchMagic(payload, JPEG_MAGIC, sizeof(JPEG_MAGIC))) {
        yinfo("TextureProvider::detectProviderType: detected JPEG");
        return "image";
    }
    if (matchMagic(payload, GIF87_MAGIC, sizeof(GIF87_MAGIC)) ||
        matchMagic(payload, GIF89_MAGIC, sizeof(GIF89_MAGIC))) {
        yinfo("TextureProvider::detectProviderType: detected GIF");
        return "image";
    }
    if (matchMagic(payload, BMP_MAGIC, sizeof(BMP_MAGIC))) {
        yinfo("TextureProvider::detectProviderType: detected BMP");
        return "image";
    }
    if (matchMagic(payload, PSD_MAGIC, sizeof(PSD_MAGIC))) {
        yinfo("TextureProvider::detectProviderType: detected PSD");
        return "image";
    }
    if (matchMagic(payload, TIFF_LE_MAGIC, sizeof(TIFF_LE_MAGIC)) ||
        matchMagic(payload, TIFF_BE_MAGIC, sizeof(TIFF_BE_MAGIC))) {
        yinfo("TextureProvider::detectProviderType: detected TIFF");
        return "image";
    }
    // WEBP: RIFF....WEBP
    if (matchMagic(payload, WEBP_MAGIC, sizeof(WEBP_MAGIC)) && payload.size() >= 12) {
        if (payload[8] == 'W' && payload[9] == 'E' &&
            payload[10] == 'B' && payload[11] == 'P') {
            yinfo("TextureProvider::detectProviderType: detected WEBP");
            return "image";
        }
    }

    // Check PDF
    if (matchMagic(payload, PDF_MAGIC, sizeof(PDF_MAGIC))) {
        yinfo("TextureProvider::detectProviderType: detected PDF");
        return "pdf";
    }

    yinfo("TextureProvider::detectProviderType: unknown format");
    return "";
}

//=============================================================================
// Factory methods
//=============================================================================

Result<TextureProvider::Ptr> TextureProvider::create(
    const std::string& providerType,
    const YettyContext& ctx,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("TextureProvider::create: type='{}' widthCells={} heightCells={}",
          providerType, widthCells, heightCells);

    if (providerType == "image") {
        auto result = ImageProvider::create(ctx, widthCells, heightCells, args, payload);
        if (!result) {
            return Err<Ptr>("Failed to create ImageProvider", result);
        }
        return Ok<Ptr>(*result);
    }

    if (providerType == "pdf") {
        // TODO: PdfProvider not yet implemented
        return Err<Ptr>("TextureProvider: pdf provider not yet implemented");
    }

    return Err<Ptr>("TextureProvider: unknown provider type: " + providerType);
}

Result<TextureProvider::Ptr> TextureProvider::createAuto(
    const YettyContext& ctx,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    std::string providerType = detectProviderType(payload);

    if (providerType.empty()) {
        return Err<Ptr>("TextureProvider::createAuto: could not detect format from payload");
    }

    return create(providerType, ctx, widthCells, heightCells, args, payload);
}

} // namespace yetty::card
