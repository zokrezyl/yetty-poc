#include <yetty/bm-font.h>
#include <yetty/gpu-allocator.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

// Android/Emscripten: stub implementation without FreeType/fontconfig
#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
#define YETTY_BM_FONT_STUB 1
#else
#define YETTY_BM_FONT_STUB 0
#endif

// BmFont atlas is built progressively - glyphs loaded on demand via getGlyphIndex()

#if !YETTY_BM_FONT_STUB
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H
#include FT_SFNT_NAMES_H

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif
#endif // !YETTY_BM_FONT_STUB

#include <cstring>
#include <algorithm>

namespace yetty {

// =============================================================================
// Stub implementation for Android/Emscripten (no FreeType/fontconfig)
// =============================================================================
#if YETTY_BM_FONT_STUB

Result<BmFont::Ptr> BmFont::create(WGPUDevice device, GpuAllocator::Ptr allocator,
                                   const std::string& fontPath,
                                   uint32_t glyphSize) noexcept {
    auto font = Ptr(new BmFont(device, std::move(allocator), fontPath, glyphSize));
    if (auto res = font->init(); !res) {
        return Err<Ptr>("Failed to initialize BmFont", res);
    }
    return Ok(std::move(font));
}

BmFont::BmFont(WGPUDevice device, GpuAllocator::Ptr allocator, const std::string& fontPath,
               uint32_t glyphSize) noexcept
    : _device(device), _allocator(std::move(allocator)), _fontPath(fontPath),
      _glyphSize(glyphSize) {
    _glyphsPerRow = _atlasSize / _glyphSize;
}

BmFont::~BmFont() {
    if (_metadataBuffer) _allocator->releaseBuffer(_metadataBuffer);
    if (_sampler) wgpuSamplerRelease(_sampler);
    if (_textureView) wgpuTextureViewRelease(_textureView);
    if (_texture) _allocator->releaseTexture(_texture);
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
    yinfo("BmFont: stub - GPU resources created lazily");
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

    _texture = _allocator->createTexture(texDesc);
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

    _metadataBuffer = _allocator->createBuffer(bufDesc);
    if (!_metadataBuffer) {
        return Err<void>("Failed to create BmFont metadata buffer");
    }

    _gpuResourcesCreated = true;
    _gpuAtlasSize = _atlasSize;
    yinfo("BmFont: stub GPU resources created");

    return Ok();
}

void BmFont::releaseGPUResources() noexcept {
    if (_metadataBuffer) { _allocator->releaseBuffer(_metadataBuffer); _metadataBuffer = nullptr; }
    if (_sampler) { wgpuSamplerRelease(_sampler); _sampler = nullptr; }
    if (_textureView) { wgpuTextureViewRelease(_textureView); _textureView = nullptr; }
    if (_texture) { _allocator->releaseTexture(_texture); _texture = nullptr; }
    _gpuResourcesCreated = false;
    _gpuAtlasSize = 0;
}

void BmFont::growAtlas() {}

Result<void> BmFont::renderGlyph(uint32_t, int, int) noexcept {
    return Ok();
}

// =============================================================================
// Full implementation for Desktop (FreeType + fontconfig)
// =============================================================================
#else // !YETTY_BM_FONT_STUB

Result<BmFont::Ptr> BmFont::create(WGPUDevice device, GpuAllocator::Ptr allocator,
                                   const std::string& fontPath,
                                   uint32_t glyphSize) noexcept {
    auto font = Ptr(new BmFont(device, std::move(allocator), fontPath, glyphSize));
    if (auto res = font->init(); !res) {
        return Err<Ptr>("Failed to initialize BmFont", res);
    }
    return Ok(std::move(font));
}

BmFont::BmFont(WGPUDevice device, GpuAllocator::Ptr allocator, const std::string& fontPath,
               uint32_t glyphSize) noexcept
    : _device(device), _allocator(std::move(allocator)), _fontPath(fontPath),
      _glyphSize(glyphSize) {
    _glyphsPerRow = _atlasSize / _glyphSize;
}

BmFont::~BmFont() {
    if (_metadataBuffer) _allocator->releaseBuffer(_metadataBuffer);
    if (_sampler) wgpuSamplerRelease(_sampler);
    if (_textureView) wgpuTextureViewRelease(_textureView);
    if (_texture) _allocator->releaseTexture(_texture);

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

#ifdef YETTY_USE_FONTCONFIG
    // Use fontconfig to find an emoji font (Linux)
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
#else
    // macOS/Windows: font path must be specified explicitly
    return Err<void>("No font path specified and fontconfig not available");
#endif
}

Result<void> BmFont::loadCommonGlyphs() noexcept {
    // Atlas is built progressively - glyphs loaded on demand via getGlyphIndex()
    if (!_gpuResourcesCreated) {
        if (auto res = createGPUResources(); !res) {
            return res;
        }
    }
    yinfo("BmFont: progressive atlas ready (glyphs loaded on demand)");
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

    // Check if we have space in the atlas - grow if needed
    if (_nextGlyphY * static_cast<int>(_glyphSize) + static_cast<int>(_glyphSize) > static_cast<int>(_atlasSize)) {
        if (_atlasSize >= ATLAS_MAX_SIZE) {
            yerror("BmFont: atlas at maximum size {}x{}, cannot grow", _atlasSize, _atlasSize);
            return Ok(-1);
        }
        growAtlas();
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

    // Atlas grew beyond current GPU texture - recreate
    if (_gpuResourcesCreated && _atlasSize > _gpuAtlasSize) {
        yinfo("BmFont: atlas grew {}->{}x{}, recreating GPU resources",
              _gpuAtlasSize, _atlasSize, _atlasSize);
        releaseGPUResources();
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

    _texture = _allocator->createTexture(texDesc);
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

    _metadataBuffer = _allocator->createBuffer(bufDesc);
    if (!_metadataBuffer) {
        return Err<void>("Failed to create BmFont metadata buffer");
    }
    size_t metaBytes = maxGlyphs * sizeof(BitmapGlyphMetadata);
    yinfo("GPU_ALLOC BmFont: metadataBuffer={} bytes ({:.2f} MB) for {} glyphs",
          metaBytes, metaBytes / (1024.0 * 1024.0), maxGlyphs);

    _gpuResourcesCreated = true;
    _gpuAtlasSize = _atlasSize;
    yinfo("BmFont: created GPU resources ({}x{} texture, {} max glyphs)",
          _atlasSize, _atlasSize, maxGlyphs);

    return Ok();
}

void BmFont::releaseGPUResources() noexcept {
    if (_metadataBuffer) { _allocator->releaseBuffer(_metadataBuffer); _metadataBuffer = nullptr; }
    if (_sampler) { wgpuSamplerRelease(_sampler); _sampler = nullptr; }
    if (_textureView) { wgpuTextureViewRelease(_textureView); _textureView = nullptr; }
    if (_texture) { _allocator->releaseTexture(_texture); _texture = nullptr; }
    _gpuResourcesCreated = false;
    _gpuAtlasSize = 0;
}

void BmFont::growAtlas() {
    uint32_t oldSize = _atlasSize;
    uint32_t newSize = std::min(_atlasSize * 2, ATLAS_MAX_SIZE);

    yinfo("BmFont: growing atlas {}x{} -> {}x{}", oldSize, oldSize, newSize, newSize);

    // Create new atlas data and copy old rows
    std::vector<uint8_t> newData(static_cast<size_t>(newSize) * newSize * 4, 0);
    for (uint32_t y = 0; y < oldSize; ++y) {
        std::memcpy(&newData[y * newSize * 4], &_atlasData[y * oldSize * 4], oldSize * 4);
    }
    _atlasData = std::move(newData);

    // Rescale UV coordinates for all existing glyphs
    float scale = static_cast<float>(oldSize) / static_cast<float>(newSize);
    for (auto& meta : _glyphMetadata) {
        meta.uvMinX *= scale;
        meta.uvMinY *= scale;
        meta.uvMaxX *= scale;
        meta.uvMaxY *= scale;
    }

    _atlasSize = newSize;
    _glyphsPerRow = _atlasSize / _glyphSize;
    _needsUpload = true;
}

Result<void> BmFont::renderGlyph(uint32_t, int, int) noexcept {
    // This is handled by loadGlyph
    return Ok();
}

#endif // YETTY_BM_FONT_STUB

} // namespace yetty
