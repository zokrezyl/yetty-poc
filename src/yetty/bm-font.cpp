#include <yetty/bm-font.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

// Android/Emscripten: stub implementation without FreeType/fontconfig
#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
#define YETTY_BM_FONT_STUB 1
#else
#define YETTY_BM_FONT_STUB 0
#endif

// Common emoji list for preloading (top ~200 most used)
// These are loaded into the bitmap font atlas for fast rendering
namespace {
constexpr uint32_t COMMON_EMOJIS[] = {
    // Smileys & Emotion (most used)
    0x1F600, 0x1F601, 0x1F602, 0x1F603, 0x1F604, 0x1F605, 0x1F606, 0x1F607,
    0x1F608, 0x1F609, 0x1F60A, 0x1F60B, 0x1F60C, 0x1F60D, 0x1F60E, 0x1F60F,
    0x1F610, 0x1F611, 0x1F612, 0x1F613, 0x1F614, 0x1F615, 0x1F616, 0x1F617,
    0x1F618, 0x1F619, 0x1F61A, 0x1F61B, 0x1F61C, 0x1F61D, 0x1F61E, 0x1F61F,
    0x1F620, 0x1F621, 0x1F622, 0x1F623, 0x1F624, 0x1F625, 0x1F626, 0x1F627,
    0x1F628, 0x1F629, 0x1F62A, 0x1F62B, 0x1F62C, 0x1F62D, 0x1F62E, 0x1F62F,
    0x1F630, 0x1F631, 0x1F632, 0x1F633, 0x1F634, 0x1F635, 0x1F636, 0x1F637,
    // Hearts
    0x2764, 0x1F493, 0x1F494, 0x1F495, 0x1F496, 0x1F497, 0x1F498, 0x1F499,
    0x1F49A, 0x1F49B, 0x1F49C, 0x1F49D, 0x1F49E, 0x1F49F, 0x1F5A4, 0x1F90D,
    0x1F90E, 0x1F9E1,
    // Gestures
    0x1F44D, 0x1F44E, 0x1F44A, 0x1F44B, 0x1F44C, 0x1F44F, 0x1F64F, 0x1F4AA,
    0x270B, 0x270C, 0x1F91E, 0x1F91F, 0x1F918, 0x1F919,
    // Objects & Symbols
    0x2B50, 0x1F31F, 0x1F4A5, 0x1F4A6, 0x1F4A8, 0x1F4AB, 0x1F4AC, 0x1F4AD,
    0x1F4AF, 0x1F525, 0x26A1, 0x2728,
    // Animals
    0x1F436, 0x1F431, 0x1F42D, 0x1F439, 0x1F430, 0x1F98A, 0x1F43B, 0x1F43C,
    0x1F428, 0x1F42F, 0x1F981, 0x1F42E, 0x1F437, 0x1F438, 0x1F412, 0x1F414,
    0x1F427, 0x1F426, 0x1F40D, 0x1F422, 0x1F41D, 0x1F41B, 0x1F98B,
    // Food & Drink
    0x1F34E, 0x1F34F, 0x1F350, 0x1F34A, 0x1F34B, 0x1F34C, 0x1F349, 0x1F347,
    0x1F353, 0x1F352, 0x1F351, 0x1F355, 0x1F354, 0x1F35F, 0x1F32D, 0x1F32E,
    0x1F37A, 0x1F377, 0x2615,
    // Weather & Nature
    0x2600, 0x1F31E, 0x1F31D, 0x1F31A, 0x1F319, 0x1F308, 0x2601, 0x26C5,
    0x1F327, 0x1F4A7,
    // Checkmarks & Status
    0x2705, 0x274C, 0x2714, 0x2716, 0x1F6AB, 0x26A0, 0x2139, 0x1F4A1,
    0x1F512, 0x1F513,
    // Arrows
    0x2B06, 0x2B07, 0x27A1, 0x2B05, 0x2194, 0x2195,
    // Tech
    0x1F4BB, 0x1F4F1, 0x2328, 0x1F5A5, 0x1F3AE, 0x1F680,
    // Misc
    0x1F389, 0x1F38A, 0x1F381, 0x1F4E7, 0x1F4DD, 0x1F4DA, 0x1F4D6, 0x1F4CC,
    0x1F4CE, 0x1F4C1, 0x1F4C2,
};
constexpr size_t COMMON_EMOJI_COUNT = sizeof(COMMON_EMOJIS) / sizeof(COMMON_EMOJIS[0]);
} // anonymous namespace

#if !YETTY_BM_FONT_STUB
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H
#include FT_SFNT_NAMES_H

#include <fontconfig/fontconfig.h>
#endif // !YETTY_BM_FONT_STUB

#include <cstring>
#include <algorithm>

namespace yetty {

// =============================================================================
// Stub implementation for Android/Emscripten (no FreeType/fontconfig)
// =============================================================================
#if YETTY_BM_FONT_STUB

Result<BmFont::Ptr> BmFont::create(WGPUDevice device, const std::string& fontPath,
                                   uint32_t glyphSize) noexcept {
    auto font = Ptr(new BmFont(device, fontPath, glyphSize));
    if (auto res = font->init(); !res) {
        return Err<Ptr>("Failed to initialize BmFont", res);
    }
    return Ok(std::move(font));
}

BmFont::BmFont(WGPUDevice device, const std::string& fontPath, uint32_t glyphSize) noexcept
    : _device(device), _fontPath(fontPath), _glyphSize(glyphSize) {
    _glyphsPerRow = _atlasSize / _glyphSize;
}

BmFont::~BmFont() {
    if (_metadataBuffer) wgpuBufferRelease(_metadataBuffer);
    if (_sampler) wgpuSamplerRelease(_sampler);
    if (_textureView) wgpuTextureViewRelease(_textureView);
    if (_texture) wgpuTextureRelease(_texture);
}

Result<void> BmFont::init() {
    // Stub: no font support on Android/Emscripten
    yinfo("BmFont: stub implementation (no FreeType support)");
    _atlasData.resize(_atlasSize * _atlasSize * 4, 0);
    return Ok();
}

Result<void> BmFont::findFont() noexcept {
    return Err<void>("Font not available on this platform");
}

Result<void> BmFont::loadCommonGlyphs() noexcept {
    yinfo("BmFont: no glyph support on this platform");
    if (auto res = createGPUResources(); !res) {
        return res;
    }
    return Ok();
}

Result<int> BmFont::loadGlyph(uint32_t codepoint) noexcept {
    (void)codepoint;
    return Ok(-1);
}

uint32_t BmFont::getGlyphIndex(uint32_t codepoint) {
    auto it = _codepointToIndex.find(codepoint);
    return (it != _codepointToIndex.end() && it->second >= 0)
        ? static_cast<uint32_t>(it->second) : 0;
}

uint32_t BmFont::getGlyphIndex(uint32_t codepoint, Style) {
    return getGlyphIndex(codepoint);
}

uint32_t BmFont::getGlyphIndex(uint32_t codepoint, bool, bool) {
    return getGlyphIndex(codepoint);
}

bool BmFont::hasGlyph(uint32_t codepoint) const noexcept {
    return _codepointToIndex.find(codepoint) != _codepointToIndex.end();
}

void BmFont::uploadToGpu() {
    if (!_gpuResourcesCreated) {
        createGPUResources();
    }
}

Result<void> BmFont::createGPUResources() noexcept {
    size_t textureBytes = _atlasSize * _atlasSize * 4;
    yinfo("GPU_ALLOC BmFont(stub): texture={}x{} RGBA8 = {} bytes ({:.2f} MB)",
          _atlasSize, _atlasSize, textureBytes, textureBytes / (1024.0 * 1024.0));

    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("bm font atlas stub");
    texDesc.size = {_atlasSize, _atlasSize, 1};
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;

    _texture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!_texture) {
        return Err<void>("Failed to create BmFont texture");
    }

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;

    _textureView = wgpuTextureCreateView(_texture, &viewDesc);
    if (!_textureView) {
        return Err<void>("Failed to create BmFont texture view");
    }

    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.label = WGPU_STR("bm font sampler stub");
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.mipmapFilter = WGPUMipmapFilterMode_Nearest;
    samplerDesc.maxAnisotropy = 1;

    _sampler = wgpuDeviceCreateSampler(_device, &samplerDesc);
    if (!_sampler) {
        return Err<void>("Failed to create BmFont sampler");
    }

    uint32_t maxGlyphs = _glyphsPerRow * _glyphsPerRow;
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("bm font metadata stub");
    bufDesc.size = maxGlyphs * sizeof(BitmapGlyphMetadata);
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;

    _metadataBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    if (!_metadataBuffer) {
        return Err<void>("Failed to create BmFont metadata buffer");
    }

    _gpuResourcesCreated = true;
    yinfo("BmFont: stub GPU resources created");

    return Ok();
}

Result<void> BmFont::renderGlyph(uint32_t, int, int) noexcept {
    return Ok();
}

// =============================================================================
// Full implementation for Desktop (FreeType + fontconfig)
// =============================================================================
#else // !YETTY_BM_FONT_STUB

Result<BmFont::Ptr> BmFont::create(WGPUDevice device, const std::string& fontPath,
                                   uint32_t glyphSize) noexcept {
    auto font = Ptr(new BmFont(device, fontPath, glyphSize));
    if (auto res = font->init(); !res) {
        return Err<Ptr>("Failed to initialize BmFont", res);
    }
    return Ok(std::move(font));
}

BmFont::BmFont(WGPUDevice device, const std::string& fontPath, uint32_t glyphSize) noexcept
    : _device(device), _fontPath(fontPath), _glyphSize(glyphSize) {
    _glyphsPerRow = _atlasSize / _glyphSize;
}

BmFont::~BmFont() {
    if (_metadataBuffer) wgpuBufferRelease(_metadataBuffer);
    if (_sampler) wgpuSamplerRelease(_sampler);
    if (_textureView) wgpuTextureViewRelease(_textureView);
    if (_texture) wgpuTextureRelease(_texture);

    if (_ftFace) {
        FT_Done_Face(static_cast<FT_Face>(_ftFace));
    }
    if (_ftLibrary) {
        FT_Done_FreeType(static_cast<FT_Library>(_ftLibrary));
    }
}

Result<void> BmFont::init() {
    // Initialize FreeType
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        return Err<void>("Failed to initialize FreeType");
    }
    _ftLibrary = library;

    // Log FreeType version
    FT_Int major, minor, patch;
    FT_Library_Version(library, &major, &minor, &patch);
    yinfo("BmFont: FreeType version {}.{}.{}", major, minor, patch);

    // Find font
    if (auto res = findFont(); !res) {
        return Err<void>("Failed to find font", res);
    }

    // Initialize atlas data (RGBA)
    _atlasData.resize(_atlasSize * _atlasSize * 4, 0);

    yinfo("BmFont initialized: {}x{} atlas, {}px glyphs",
          _atlasSize, _atlasSize, _glyphSize);

    return Ok();
}

Result<void> BmFont::findFont() noexcept {
    // If a specific path was provided, use it directly
    if (!_fontPath.empty()) {
        FT_Library library = static_cast<FT_Library>(_ftLibrary);
        FT_Face face = nullptr;
        FT_Error error = FT_New_Face(library, _fontPath.c_str(), 0, &face);
        if (error == 0) {
            _ftFace = face;

            if (face->num_fixed_sizes > 0) {
                FT_Select_Size(face, 0);
            }
            FT_Select_Charmap(face, FT_ENCODING_UNICODE);

            yinfo("BmFont: using font '{}'", _fontPath);
            return Ok();
        }
        return Err<void>("Failed to load font: " + _fontPath);
    }

    // Use fontconfig to find an emoji font
    FcConfig* config = FcInitLoadConfigAndFonts();
    if (!config) {
        return Err<void>("Failed to initialize fontconfig");
    }

    // Search for emoji fonts in order of preference
    const char* fontPatterns[] = {
        "Noto Color Emoji",
        "Apple Color Emoji",
        "Segoe UI Emoji",
        "Twemoji",
        "EmojiOne",
        "emoji"
    };

    FT_Library library = static_cast<FT_Library>(_ftLibrary);
    FT_Face face = nullptr;

    for (const char* pattern : fontPatterns) {
        FcPattern* fcPattern = FcNameParse(reinterpret_cast<const FcChar8*>(pattern));
        if (!fcPattern) {
            ywarn("BmFont: FcNameParse failed for '{}'", pattern);
            continue;
        }

        FcConfigSubstitute(config, fcPattern, FcMatchPattern);
        FcDefaultSubstitute(fcPattern);

        FcResult result;
        FcPattern* match = FcFontMatch(config, fcPattern, &result);
        FcPatternDestroy(fcPattern);

        if (match && result == FcResultMatch) {
            FcChar8* fontPath = nullptr;
            if (FcPatternGetString(match, FC_FILE, 0, &fontPath) == FcResultMatch) {
                yinfo("BmFont: pattern '{}' matched '{}'", pattern,
                      reinterpret_cast<const char*>(fontPath));
                FT_Error error = FT_New_Face(library, reinterpret_cast<const char*>(fontPath),
                                              0, &face);
                if (error == 0) {
                    _fontPath = reinterpret_cast<const char*>(fontPath);
                    _ftFace = face;

                    yinfo("BmFont: using font '{}'", _fontPath);

                    // Select size and charmap
                    if (face->num_fixed_sizes > 0) {
                        FT_Select_Size(face, 0);
                    }
                    FT_Select_Charmap(face, FT_ENCODING_UNICODE);

                    FcPatternDestroy(match);
                    FcConfigDestroy(config);
                    return Ok();
                }
                ywarn("BmFont: FT_New_Face failed for '{}' (error={})",
                      reinterpret_cast<const char*>(fontPath), error);
            }
            FcPatternDestroy(match);
        } else {
            ywarn("BmFont: FcFontMatch failed for pattern '{}'", pattern);
        }
    }

    FcConfigDestroy(config);
    return Err<void>("No suitable font found");
}

Result<void> BmFont::loadCommonGlyphs() noexcept {
    yinfo("BmFont: loading {} common emojis", COMMON_EMOJI_COUNT);

    int loaded = 0;
    for (size_t i = 0; i < COMMON_EMOJI_COUNT; ++i) {
        if (auto res = loadGlyph(COMMON_EMOJIS[i]); res && *res >= 0) {
            loaded++;
        }
    }

    yinfo("BmFont: loaded {}/{} common glyphs", loaded, COMMON_EMOJI_COUNT);

    // Create GPU resources and upload
    if (!_gpuResourcesCreated) {
        if (auto res = createGPUResources(); !res) {
            return res;
        }
    }

    uploadToGpu();
    return Ok();
}

Result<int> BmFont::loadGlyph(uint32_t codepoint) noexcept {
    // Check if already loaded
    auto it = _codepointToIndex.find(codepoint);
    if (it != _codepointToIndex.end()) {
        return Ok(it->second);
    }

    FT_Face face = static_cast<FT_Face>(_ftFace);
    if (!face) {
        return Err<int>("No font loaded");
    }

    // Get glyph index
    FT_UInt glyphIndex = FT_Get_Char_Index(face, codepoint);
    if (glyphIndex == 0) {
        return Ok(-1);
    }

    // Load glyph with color support
    FT_Error error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT | FT_LOAD_COLOR);
    if (error) {
        // Try without COLOR flag as fallback
        error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
        if (error) {
            return Err<int>("Failed to load glyph");
        }
    }

    FT_GlyphSlot slot = face->glyph;

    // If not a bitmap yet, try to render it
    if (slot->format != FT_GLYPH_FORMAT_BITMAP) {
        error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
        if (error) {
            return Err<int>("Failed to render glyph");
        }
    }

    FT_Bitmap& bitmap = slot->bitmap;

    // Skip empty bitmaps
    if (bitmap.width == 0 || bitmap.rows == 0) {
        return Ok(-1);
    }

    // Check if we have space in the atlas
    if (_nextGlyphY * static_cast<int>(_glyphSize) + static_cast<int>(_glyphSize) > static_cast<int>(_atlasSize)) {
        return Err<int>("Atlas is full");
    }

    // Calculate position in atlas
    int atlasX = _nextGlyphX * static_cast<int>(_glyphSize);
    int atlasY = _nextGlyphY * static_cast<int>(_glyphSize);

    // Copy bitmap to atlas
    int srcWidth = bitmap.width;
    int srcHeight = bitmap.rows;

    // Center the glyph in its cell
    int offsetX = (static_cast<int>(_glyphSize) - srcWidth) / 2;
    int offsetY = (static_cast<int>(_glyphSize) - srcHeight) / 2;
    offsetX = std::max(0, offsetX);
    offsetY = std::max(0, offsetY);

    for (int y = 0; y < srcHeight && y + offsetY < static_cast<int>(_glyphSize); ++y) {
        for (int x = 0; x < srcWidth && x + offsetX < static_cast<int>(_glyphSize); ++x) {
            int dstX = atlasX + offsetX + x;
            int dstY = atlasY + offsetY + y;
            int dstIdx = (dstY * static_cast<int>(_atlasSize) + dstX) * 4;

            if (bitmap.pixel_mode == FT_PIXEL_MODE_BGRA) {
                // Color bitmap (BGRA -> RGBA)
                int srcIdx = y * bitmap.pitch + x * 4;
                _atlasData[dstIdx + 0] = bitmap.buffer[srcIdx + 2]; // R
                _atlasData[dstIdx + 1] = bitmap.buffer[srcIdx + 1]; // G
                _atlasData[dstIdx + 2] = bitmap.buffer[srcIdx + 0]; // B
                _atlasData[dstIdx + 3] = bitmap.buffer[srcIdx + 3]; // A
            } else if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
                // Grayscale bitmap - render as white with alpha
                int srcIdx = y * bitmap.pitch + x;
                uint8_t alpha = bitmap.buffer[srcIdx];
                _atlasData[dstIdx + 0] = 255; // R
                _atlasData[dstIdx + 1] = 255; // G
                _atlasData[dstIdx + 2] = 255; // B
                _atlasData[dstIdx + 3] = alpha; // A
            }
        }
    }

    // Create metadata
    BitmapGlyphMetadata meta;
    meta.uvMinX = static_cast<float>(atlasX) / _atlasSize;
    meta.uvMinY = static_cast<float>(atlasY) / _atlasSize;
    meta.uvMaxX = static_cast<float>(atlasX + _glyphSize) / _atlasSize;
    meta.uvMaxY = static_cast<float>(atlasY + _glyphSize) / _atlasSize;
    meta.width = static_cast<float>(_glyphSize);
    meta.height = static_cast<float>(_glyphSize);
    meta._pad1 = 0;
    meta._pad2 = 0;

    int index = static_cast<int>(_glyphMetadata.size());
    _glyphMetadata.push_back(meta);
    _codepointToIndex[codepoint] = index;

    // Move to next position
    _nextGlyphX++;
    if (_nextGlyphX >= static_cast<int>(_glyphsPerRow)) {
        _nextGlyphX = 0;
        _nextGlyphY++;
    }

    _needsUpload = true;
    return Ok(index);
}

uint32_t BmFont::getGlyphIndex(uint32_t codepoint) {
    auto it = _codepointToIndex.find(codepoint);
    if (it != _codepointToIndex.end() && it->second >= 0) {
        return static_cast<uint32_t>(it->second);
    }

    // Try to load the glyph on demand
    auto res = loadGlyph(codepoint);
    if (res && *res >= 0) {
        return static_cast<uint32_t>(*res);
    }

    return 0; // Placeholder
}

uint32_t BmFont::getGlyphIndex(uint32_t codepoint, Style) {
    // Bitmap fonts don't have style variants
    return getGlyphIndex(codepoint);
}

uint32_t BmFont::getGlyphIndex(uint32_t codepoint, bool, bool) {
    // Bitmap fonts don't have style variants
    return getGlyphIndex(codepoint);
}

bool BmFont::hasGlyph(uint32_t codepoint) const noexcept {
    return _codepointToIndex.find(codepoint) != _codepointToIndex.end();
}

void BmFont::uploadToGpu() {
    if (!_needsUpload && _gpuResourcesCreated) {
        return;
    }

    if (!_gpuResourcesCreated) {
        if (auto res = createGPUResources(); !res) {
            yerror("BmFont: failed to create GPU resources: {}", error_msg(res));
            return;
        }
    }

    // Upload atlas texture data
    WGPUQueue queue = wgpuDeviceGetQueue(_device);

    WGPUTexelCopyTextureInfo destInfo = {};
    destInfo.texture = _texture;
    destInfo.mipLevel = 0;
    destInfo.origin = {0, 0, 0};
    destInfo.aspect = WGPUTextureAspect_All;

    WGPUTexelCopyBufferLayout srcLayout = {};
    srcLayout.offset = 0;
    srcLayout.bytesPerRow = _atlasSize * 4;
    srcLayout.rowsPerImage = _atlasSize;

    WGPUExtent3D extent = {_atlasSize, _atlasSize, 1};

    wgpuQueueWriteTexture(queue, &destInfo, _atlasData.data(),
                          _atlasData.size(), &srcLayout, &extent);

    // Upload metadata buffer
    if (!_glyphMetadata.empty()) {
        wgpuQueueWriteBuffer(queue, _metadataBuffer, 0,
                             _glyphMetadata.data(),
                             _glyphMetadata.size() * sizeof(BitmapGlyphMetadata));
    }

    _needsUpload = false;
    ydebug("BmFont: uploaded {} glyphs to GPU", _glyphMetadata.size());
}

Result<void> BmFont::createGPUResources() noexcept {
    // Create texture
    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("bm font atlas");
    texDesc.size = {_atlasSize, _atlasSize, 1};
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;

    _texture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!_texture) {
        return Err<void>("Failed to create BmFont texture");
    }
    size_t texBytes = static_cast<size_t>(_atlasSize) * _atlasSize * 4;
    yinfo("GPU_ALLOC BmFont: texture={}x{} RGBA8 = {} bytes ({:.2f} MB)",
          _atlasSize, _atlasSize, texBytes, texBytes / (1024.0 * 1024.0));

    // Create texture view
    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;

    _textureView = wgpuTextureCreateView(_texture, &viewDesc);
    if (!_textureView) {
        return Err<void>("Failed to create BmFont texture view");
    }

    // Create sampler
    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.label = WGPU_STR("bm font sampler");
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.mipmapFilter = WGPUMipmapFilterMode_Nearest;
    samplerDesc.maxAnisotropy = 1;

    _sampler = wgpuDeviceCreateSampler(_device, &samplerDesc);
    if (!_sampler) {
        return Err<void>("Failed to create BmFont sampler");
    }

    // Create metadata buffer (pre-allocate for max glyphs)
    uint32_t maxGlyphs = _glyphsPerRow * _glyphsPerRow;
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("bm font metadata");
    bufDesc.size = maxGlyphs * sizeof(BitmapGlyphMetadata);
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;

    _metadataBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    if (!_metadataBuffer) {
        return Err<void>("Failed to create BmFont metadata buffer");
    }
    size_t metaBytes = maxGlyphs * sizeof(BitmapGlyphMetadata);
    yinfo("GPU_ALLOC BmFont: metadataBuffer={} bytes ({:.2f} MB) for {} glyphs",
          metaBytes, metaBytes / (1024.0 * 1024.0), maxGlyphs);

    _gpuResourcesCreated = true;
    yinfo("BmFont: created GPU resources ({}x{} texture, {} max glyphs)",
          _atlasSize, _atlasSize, maxGlyphs);

    return Ok();
}

Result<void> BmFont::renderGlyph(uint32_t, int, int) noexcept {
    // This is handled by loadGlyph
    return Ok();
}

#endif // YETTY_BM_FONT_STUB

} // namespace yetty
