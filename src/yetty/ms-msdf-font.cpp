#include <yetty/ms-msdf-font.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <algorithm>

extern "C" {
#include <cdb.h>
}

namespace yetty {

// Style name suffixes for CDB files
static const char* STYLE_SUFFIXES[] = {
    "-Regular.cdb",
    "-Bold.cdb",
    "-Oblique.cdb",
    "-BoldOblique.cdb"
};

MsMsdfFont::MsMsdfFont(const std::string& cdbBasePath)
    : _cdbBasePath(cdbBasePath)
{
    // Initialize atlas
    _atlasData.resize(_atlasWidth * _atlasHeight * 4, 0);

    // Reserve space for glyphs
    _glyphMetadata.reserve(512);

    // Add placeholder glyph at index 0 (empty 1x1)
    GlyphMetadataGPU placeholder{};
    placeholder._uvMinX = 0;
    placeholder._uvMinY = 0;
    placeholder._uvMaxX = 0;
    placeholder._uvMaxY = 0;
    placeholder._sizeX = 0;
    placeholder._sizeY = 0;
    placeholder._bearingX = 0;
    placeholder._bearingY = 0;
    placeholder._advance = 0;
    placeholder._pad = 0;
    _glyphMetadata.push_back(placeholder);
}

MsMsdfFont::~MsMsdfFont() {
    // Clean up CDB files
    for (int i = 0; i < 4; ++i) {
        closeCdb(static_cast<Style>(i));
    }

    // Clean up WebGPU resources
    if (_glyphMetadataBuffer) wgpuBufferRelease(_glyphMetadataBuffer);
    if (_sampler) wgpuSamplerRelease(_sampler);
    if (_textureView) wgpuTextureViewRelease(_textureView);
    if (_texture) wgpuTextureRelease(_texture);
}

Result<MsMsdfFont::Ptr> MsMsdfFont::create(const std::string& cdbBasePath) {
    auto font = MsMsdfFont::Ptr(new MsMsdfFont(cdbBasePath));
    auto initResult = font->init();
    if (!initResult) {
        return Err<MsMsdfFont::Ptr>("Failed to initialize MsMsdfFont", initResult);
    }
    return Ok(std::move(font));
}

Result<void> MsMsdfFont::init() {
    // Open all style CDBs (all required)
    auto regularResult = openCdb(Style::Regular);
    if (!regularResult) {
        return Err<void>("Failed to open Regular CDB", regularResult);
    }

    auto boldResult = openCdb(Style::Bold);
    if (!boldResult) {
        return Err<void>("Failed to open Bold CDB", boldResult);
    }

    auto italicResult = openCdb(Style::Italic);
    if (!italicResult) {
        return Err<void>("Failed to open Italic/Oblique CDB", italicResult);
    }

    auto boldItalicResult = openCdb(Style::BoldItalic);
    if (!boldItalicResult) {
        return Err<void>("Failed to open BoldItalic/BoldOblique CDB", boldItalicResult);
    }

    // Pre-load basic Latin
    initBasicLatin();

    _dirty = true;
    return Ok();
}

Result<void> MsMsdfFont::openCdb(Style style) {
    int idx = styleIndex(style);
    std::string path = _cdbBasePath + STYLE_SUFFIXES[idx];

    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        return Err<void>("Cannot open CDB file: " + path);
    }

    // Get file size
    struct stat st;
    if (fstat(fd, &st) < 0) {
        close(fd);
        return Err<void>("Cannot stat CDB file: " + path);
    }

    // Memory map the file
    void* mapped = mmap(nullptr, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        close(fd);
        return Err<void>("Cannot mmap CDB file: " + path);
    }

    // Initialize CDB structure
    auto* cdbPtr = new struct cdb;
    cdb_init(cdbPtr, fd);

    // Store in our array
    _cdbFiles[idx].fd = fd;
    _cdbFiles[idx].mapped = mapped;
    _cdbFiles[idx].size = st.st_size;
    _cdbFiles[idx].cdb = cdbPtr;

    yinfo("Opened CDB: {} ({} bytes)", path, st.st_size);
    return Ok();
}

void MsMsdfFont::closeCdb(Style style) {
    int idx = styleIndex(style);
    auto& cf = _cdbFiles[idx];

    if (cf.cdb) {
        cdb_free(cf.cdb);
        delete cf.cdb;
        cf.cdb = nullptr;
    }

    if (cf.mapped && cf.mapped != MAP_FAILED) {
        munmap(cf.mapped, cf.size);
        cf.mapped = nullptr;
    }

    if (cf.fd >= 0) {
        close(cf.fd);
        cf.fd = -1;
    }

    cf.size = 0;
}

void MsMsdfFont::initBasicLatin() {
    // Pre-load ASCII printable characters (32-126)
    for (uint32_t cp = 0x20; cp <= 0x7E; ++cp) {
        loadGlyphFromCdb(cp, Style::Regular);
    }
    yinfo("Pre-loaded {} basic Latin glyphs", _glyphMetadata.size() - 1);
}

uint32_t MsMsdfFont::loadGlyphFromCdb(uint32_t codepoint, Style style) {
    int idx = styleIndex(style);

    // Check if already loaded
    auto it = _codepointToIndex[idx].find(codepoint);
    if (it != _codepointToIndex[idx].end()) {
        return it->second;
    }

    // Check if CDB is available for this style
    if (!_cdbFiles[idx].cdb) {
        // Fall back to regular if variant not available
        if (style != Style::Regular) {
            return loadGlyphFromCdb(codepoint, Style::Regular);
        }
        return 0;  // Placeholder
    }

    // Prepare key: 4-byte little-endian codepoint
    char key[4];
    key[0] = codepoint & 0xFF;
    key[1] = (codepoint >> 8) & 0xFF;
    key[2] = (codepoint >> 16) & 0xFF;
    key[3] = (codepoint >> 24) & 0xFF;

    // Look up in CDB
    struct cdb* cdbPtr = _cdbFiles[idx].cdb;
    if (cdb_find(cdbPtr, key, 4) <= 0) {
        // Not found - fall back to regular if variant
        if (style != Style::Regular) {
            return loadGlyphFromCdb(codepoint, Style::Regular);
        }
        // Mark as missing so we don't retry
        _codepointToIndex[idx][codepoint] = 0;
        return 0;  // Placeholder
    }

    // Read glyph data
    unsigned int dataLen = cdb_datalen(cdbPtr);
    unsigned int dataPos = cdb_datapos(cdbPtr);

    if (dataLen < sizeof(MsdfGlyphData)) {
        ywarn("Invalid glyph data size for codepoint {}", codepoint);
        _codepointToIndex[idx][codepoint] = 0;
        return 0;
    }

    // Read header
    MsdfGlyphData header;
    if (cdb_read(cdbPtr, reinterpret_cast<char*>(&header), sizeof(header), dataPos) < 0) {
        yerror("Failed to read glyph header for codepoint {}", codepoint);
        _codepointToIndex[idx][codepoint] = 0;
        return 0;
    }

    // Create glyph metadata - CDB stores final values, use directly
    GlyphMetadataGPU meta{};
    meta._sizeX = header.sizeX;
    meta._sizeY = header.sizeY;
    meta._bearingX = header.bearingX;
    meta._bearingY = header.bearingY;
    meta._advance = header.advance;
    meta._pad = 0;

    // Handle empty glyphs (like space)
    if (header.width == 0 || header.height == 0) {
        meta._uvMinX = 0;
        meta._uvMinY = 0;
        meta._uvMaxX = 0;
        meta._uvMaxY = 0;

        uint32_t glyphIndex = static_cast<uint32_t>(_glyphMetadata.size());
        _glyphMetadata.push_back(meta);
        _codepointToIndex[idx][codepoint] = glyphIndex;
        return glyphIndex;
    }

    // Check if we need to grow atlas or wrap to next shelf
    if (_shelfX + header.width + ATLAS_PADDING > _atlasWidth) {
        // Move to next shelf
        _shelfX = ATLAS_PADDING;
        _shelfY += _shelfHeight + ATLAS_PADDING;
        _shelfHeight = 0;
    }

    // Check if we need more height (would require atlas resize - not implemented)
    if (_shelfY + header.height + ATLAS_PADDING > _atlasHeight) {
        ywarn("Atlas full, cannot add glyph for codepoint {}", codepoint);
        _codepointToIndex[idx][codepoint] = 0;
        return 0;
    }

    // Read pixel data
    size_t pixelDataSize = header.width * header.height * 4;
    size_t expectedSize = sizeof(MsdfGlyphData) + pixelDataSize;
    if (dataLen < expectedSize) {
        ywarn("Glyph data truncated for codepoint {}", codepoint);
        _codepointToIndex[idx][codepoint] = 0;
        return 0;
    }

    std::vector<uint8_t> pixels(pixelDataSize);
    if (cdb_read(cdbPtr, reinterpret_cast<char*>(pixels.data()), pixelDataSize,
                 dataPos + sizeof(MsdfGlyphData)) < 0) {
        yerror("Failed to read pixel data for codepoint {}", codepoint);
        _codepointToIndex[idx][codepoint] = 0;
        return 0;
    }

    // Copy to atlas
    uint32_t atlasX = _shelfX;
    uint32_t atlasY = _shelfY;

    for (uint32_t y = 0; y < header.height; ++y) {
        for (uint32_t x = 0; x < header.width; ++x) {
            size_t srcIdx = (y * header.width + x) * 4;
            size_t dstIdx = ((atlasY + y) * _atlasWidth + (atlasX + x)) * 4;

            _atlasData[dstIdx + 0] = pixels[srcIdx + 0];
            _atlasData[dstIdx + 1] = pixels[srcIdx + 1];
            _atlasData[dstIdx + 2] = pixels[srcIdx + 2];
            _atlasData[dstIdx + 3] = pixels[srcIdx + 3];
        }
    }

    // Update UV coordinates (normalized 0-1)
    meta._uvMinX = static_cast<float>(atlasX) / _atlasWidth;
    meta._uvMinY = static_cast<float>(atlasY) / _atlasHeight;
    meta._uvMaxX = static_cast<float>(atlasX + header.width) / _atlasWidth;
    meta._uvMaxY = static_cast<float>(atlasY + header.height) / _atlasHeight;

    // Update shelf packer
    _shelfX = atlasX + header.width + ATLAS_PADDING;
    _shelfHeight = std::max(_shelfHeight, static_cast<uint32_t>(header.height));

    // Add to metadata and index
    uint32_t glyphIndex = static_cast<uint32_t>(_glyphMetadata.size());
    _glyphMetadata.push_back(meta);
    _codepointToIndex[idx][codepoint] = glyphIndex;

    _dirty = true;
    return glyphIndex;
}

uint32_t MsMsdfFont::getGlyphIndex(uint32_t codepoint) {
    return getGlyphIndex(codepoint, Style::Regular);
}

uint32_t MsMsdfFont::getGlyphIndex(uint32_t codepoint, Style style) {
    int idx = styleIndex(style);

    // Check if already loaded
    auto it = _codepointToIndex[idx].find(codepoint);
    if (it != _codepointToIndex[idx].end()) {
        return it->second;
    }

    // Load from CDB
    return loadGlyphFromCdb(codepoint, style);
}

uint32_t MsMsdfFont::getGlyphIndex(uint32_t codepoint, bool bold, bool italic) {
    Style style = static_cast<Style>(styleIndex(bold, italic));
    return getGlyphIndex(codepoint, style);
}

void MsMsdfFont::uploadToGpu() {
    // Note: Actual GPU upload would be done by the caller using getAtlasData()
    // This is a placeholder for the interface requirement
    _dirty = false;
}

Result<void> MsMsdfFont::createTexture(WGPUDevice device, WGPUQueue queue) {
    // Release old resources if they exist
    if (_textureView) {
        wgpuTextureViewRelease(_textureView);
        _textureView = nullptr;
    }
    if (_texture) {
        wgpuTextureRelease(_texture);
        _texture = nullptr;
    }
    if (_sampler) {
        wgpuSamplerRelease(_sampler);
        _sampler = nullptr;
    }

    // Create texture descriptor
    size_t textureBytes = _atlasWidth * _atlasHeight * 4;  // RGBA8
    yinfo("GPU_ALLOC MsMsdfFont: atlasTexture={}x{} RGBA8 = {} bytes ({:.2f} MB)",
          _atlasWidth, _atlasHeight, textureBytes, textureBytes / (1024.0 * 1024.0));

    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("MsMsdfFont Atlas");
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.size = {_atlasWidth, _atlasHeight, 1};
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;

    _texture = wgpuDeviceCreateTexture(device, &texDesc);
    if (!_texture) {
        return Err<void>("Failed to create MsMsdfFont texture");
    }

    // Upload atlas data
    WGPUTexelCopyTextureInfo dest = {};
    dest.texture = _texture;
    dest.mipLevel = 0;
    dest.origin = {0, 0, 0};
    dest.aspect = WGPUTextureAspect_All;

    WGPUTexelCopyBufferLayout layout = {};
    layout.offset = 0;
    layout.bytesPerRow = _atlasWidth * 4;
    layout.rowsPerImage = _atlasHeight;

    WGPUExtent3D extent = {_atlasWidth, _atlasHeight, 1};
    wgpuQueueWriteTexture(queue, &dest, _atlasData.data(), _atlasData.size(), &layout, &extent);

    // Create texture view
    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.baseMipLevel = 0;
    viewDesc.mipLevelCount = 1;
    viewDesc.baseArrayLayer = 0;
    viewDesc.arrayLayerCount = 1;
    viewDesc.aspect = WGPUTextureAspect_All;

    _textureView = wgpuTextureCreateView(_texture, &viewDesc);
    if (!_textureView) {
        return Err<void>("Failed to create MsMsdfFont texture view");
    }

    // Create sampler
    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.label = WGPU_STR("MsMsdfFont Sampler");
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.mipmapFilter = WGPUMipmapFilterMode_Linear;
    samplerDesc.lodMinClamp = 0.0f;
    samplerDesc.lodMaxClamp = 1.0f;
    samplerDesc.maxAnisotropy = 1;

    _sampler = wgpuDeviceCreateSampler(device, &samplerDesc);
    if (!_sampler) {
        return Err<void>("Failed to create MsMsdfFont sampler");
    }

    _resourceVersion++;
    yinfo("Created MsMsdfFont texture {}x{}", _atlasWidth, _atlasHeight);
    return Ok();
}

Result<void> MsMsdfFont::createGlyphMetadataBuffer(WGPUDevice device) {
    // Release old buffer if it exists
    if (_glyphMetadataBuffer) {
        wgpuBufferRelease(_glyphMetadataBuffer);
        _glyphMetadataBuffer = nullptr;
    }

    // Create buffer with space for current glyphs plus some headroom
    _bufferGlyphCount = static_cast<uint32_t>(_glyphMetadata.size()) + 256;
    size_t bufferSize = _bufferGlyphCount * sizeof(GlyphMetadataGPU);

    yinfo("GPU_ALLOC MsMsdfFont: glyphMetadataBuffer={} glyphs * {} bytes = {} bytes ({:.2f} MB)",
          _bufferGlyphCount, sizeof(GlyphMetadataGPU), bufferSize, bufferSize / (1024.0 * 1024.0));

    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("MsMsdfFont Glyph Metadata");
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    bufDesc.size = bufferSize;
    bufDesc.mappedAtCreation = true;

    _glyphMetadataBuffer = wgpuDeviceCreateBuffer(device, &bufDesc);
    if (!_glyphMetadataBuffer) {
        return Err<void>("Failed to create MsMsdfFont metadata buffer");
    }

    // Copy current metadata
    void* mapped = wgpuBufferGetMappedRange(_glyphMetadataBuffer, 0, bufferSize);
    if (!mapped) {
        wgpuBufferRelease(_glyphMetadataBuffer);
        _glyphMetadataBuffer = nullptr;
        return Err<void>("Failed to map glyph metadata buffer (mappedAtCreation=true but range is null)");
    }
    std::memcpy(mapped, _glyphMetadata.data(), _glyphMetadata.size() * sizeof(GlyphMetadataGPU));
    wgpuBufferUnmap(_glyphMetadataBuffer);

    _resourceVersion++;
    yinfo("Created MsMsdfFont metadata buffer ({} glyphs, {} bytes)", _bufferGlyphCount, bufferSize);
    return Ok();
}

Result<void> MsMsdfFont::uploadPendingGlyphs(WGPUDevice device, WGPUQueue queue) {
    if (!_dirty) return Ok();

    // Re-upload texture
    if (_texture) {
        WGPUTexelCopyTextureInfo dest = {};
        dest.texture = _texture;
        dest.mipLevel = 0;
        dest.origin = {0, 0, 0};
        dest.aspect = WGPUTextureAspect_All;

        WGPUTexelCopyBufferLayout layout = {};
        layout.offset = 0;
        layout.bytesPerRow = _atlasWidth * 4;
        layout.rowsPerImage = _atlasHeight;

        WGPUExtent3D extent = {_atlasWidth, _atlasHeight, 1};
        wgpuQueueWriteTexture(queue, &dest, _atlasData.data(), _atlasData.size(), &layout, &extent);
    }

    // Update metadata buffer if it exists and has space
    if (_glyphMetadataBuffer) {
        if (_glyphMetadata.size() <= _bufferGlyphCount) {
            wgpuQueueWriteBuffer(queue, _glyphMetadataBuffer, 0,
                                 _glyphMetadata.data(),
                                 _glyphMetadata.size() * sizeof(GlyphMetadataGPU));
        } else {
            // Need to recreate buffer with more space
            auto result = createGlyphMetadataBuffer(device);
            if (!result) {
                return Err<void>("Failed to recreate glyph metadata buffer", result);
            }
        }
    }

    _dirty = false;
    _resourceVersion++;
    return Ok();
}

} // namespace yetty
