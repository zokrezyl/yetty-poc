#include <yetty/msdf-atlas.h>
#include <yetty/cdb-wrapper.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

#include <cstring>
#include <algorithm>
#include <filesystem>
#include <unordered_map>

namespace yetty {

//=============================================================================
// MsdfAtlasImpl - concrete implementation of MsdfAtlas
//=============================================================================

class MsdfAtlasImpl : public MsdfAtlas {
public:
    MsdfAtlasImpl() {
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

    ~MsdfAtlasImpl() override {
        closeAllCdbs();

        // Clean up WebGPU resources
        if (_glyphMetadataBuffer) wgpuBufferRelease(_glyphMetadataBuffer);
        if (_sampler) wgpuSamplerRelease(_sampler);
        if (_textureView) wgpuTextureViewRelease(_textureView);
        if (_texture) wgpuTextureRelease(_texture);
    }

    //=========================================================================
    // CDB file management
    //=========================================================================

    int openCdb(const std::string& path) override {
        auto reader = CdbReader::open(path);
        if (!reader) {
            ywarn("MsdfAtlas: Cannot open CDB file: {}", path);
            return -1;
        }

        int fontId = static_cast<int>(_cdbFiles.size());
        _cdbFiles.push_back(std::move(reader));
        _codepointToIndex.emplace_back();  // New empty map for this fontId

        yinfo("MsdfAtlas: Opened CDB [fontId={}]: {}", fontId, path);
        return fontId;
    }

    void closeCdb(int fontId) override {
        if (fontId < 0 || fontId >= static_cast<int>(_cdbFiles.size())) return;
        _cdbFiles[fontId].reset();
    }

    void closeAllCdbs() override {
        _cdbFiles.clear();
    }

    //=========================================================================
    // Glyph loading
    //=========================================================================

    uint32_t loadGlyph(int fontId, uint32_t codepoint) override {
        if (fontId < 0 || fontId >= static_cast<int>(_cdbFiles.size())) {
            return 0;
        }

        // Check if already loaded for this fontId
        auto it = _codepointToIndex[fontId].find(codepoint);
        if (it != _codepointToIndex[fontId].end()) {
            return it->second;
        }

        // Check if CDB is available
        auto& reader = _cdbFiles[fontId];
        if (!reader) {
            _codepointToIndex[fontId][codepoint] = 0;
            return 0;
        }

        // Look up in CDB
        auto data = reader->get(codepoint);
        if (!data) {
            _codepointToIndex[fontId][codepoint] = 0;
            return 0;
        }

        // Validate data size
        if (data->size() < sizeof(MsdfGlyphData)) {
            ywarn("Invalid glyph data size for codepoint {}", codepoint);
            _codepointToIndex[fontId][codepoint] = 0;
            return 0;
        }

        // Read header
        MsdfGlyphData header;
        std::memcpy(&header, data->data(), sizeof(header));

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
            _codepointToIndex[fontId][codepoint] = glyphIndex;
            _indexToCodepoint[glyphIndex] = codepoint;
            return glyphIndex;
        }

        // Check if we need to grow atlas or wrap to next shelf
        if (_shelfX + header.width + ATLAS_PADDING > _atlasWidth) {
            _shelfX = _shelfMinX + ATLAS_PADDING;
            _shelfY += _shelfHeight + ATLAS_PADDING;
            _shelfHeight = 0;
        }

        // Check if we need more height - grow atlas if needed
        if (_shelfY + header.height + ATLAS_PADDING > _atlasHeight) {
            growAtlas();
        }

        // Validate pixel data size
        size_t pixelDataSize = header.width * header.height * 4;
        size_t expectedSize = sizeof(MsdfGlyphData) + pixelDataSize;
        if (data->size() < expectedSize) {
            ywarn("Glyph data truncated for codepoint {}", codepoint);
            _codepointToIndex[fontId][codepoint] = 0;
            return 0;
        }

        // Get pixel data pointer
        const uint8_t* pixels = data->data() + sizeof(MsdfGlyphData);

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
        _codepointToIndex[fontId][codepoint] = glyphIndex;
        _indexToCodepoint[glyphIndex] = codepoint;

        _dirty = true;
        return glyphIndex;
    }

    //=========================================================================
    // CDB registration with optional auto-generation
    //=========================================================================

    int registerCdb(const std::string& cdbPath,
                    const std::string& ttfPath,
                    MsdfCdbProvider::Ptr cdbProvider) override {
        // Generate CDB if missing
        if (!std::filesystem::exists(cdbPath) && !ttfPath.empty() && cdbProvider) {
            MsdfCdbConfig cfg;
            cfg.ttfPath = ttfPath;
            cfg.cdbPath = cdbPath;
            if (auto res = cdbProvider->generate(cfg); !res) {
                yerror("CDB generation failed for {}: {}", cdbPath, res.error().message());
                return -1;
            }
        }

        return openCdb(cdbPath);
    }

    //=========================================================================
    // Reverse lookup
    //=========================================================================

    uint32_t getCodepoint(uint32_t glyphIndex) const override {
        auto it = _indexToCodepoint.find(glyphIndex);
        if (it != _indexToCodepoint.end()) {
            return it->second;
        }
        return 0;
    }

    //=========================================================================
    // GPU resources
    //=========================================================================

    Result<void> createTexture(WGPUDevice device, WGPUQueue queue) override {
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

        size_t textureBytes = _atlasWidth * _atlasHeight * 4;
        yinfo("GPU_ALLOC MsdfAtlas: atlasTexture={}x{} RGBA8 = {} bytes ({:.2f} MB)",
              _atlasWidth, _atlasHeight, textureBytes, textureBytes / (1024.0 * 1024.0));

        WGPUTextureDescriptor texDesc = {};
        texDesc.label = WGPU_STR("MsdfAtlas");
        texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
        texDesc.dimension = WGPUTextureDimension_2D;
        texDesc.size = {_atlasWidth, _atlasHeight, 1};
        texDesc.format = WGPUTextureFormat_RGBA8Unorm;
        texDesc.mipLevelCount = 1;
        texDesc.sampleCount = 1;

        _texture = wgpuDeviceCreateTexture(device, &texDesc);
        if (!_texture) {
            return Err<void>("Failed to create MsdfAtlas texture");
        }

        // Upload atlas data
        {
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
            wgpuQueueWriteTexture(queue, &dest,
                                  _atlasData.data(), _atlasData.size(),
                                  &layout, &extent);
        }

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
            return Err<void>("Failed to create MsdfAtlas texture view");
        }

        // Create sampler
        WGPUSamplerDescriptor samplerDesc = {};
        samplerDesc.label = WGPU_STR("MsdfAtlas Sampler");
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
            return Err<void>("Failed to create MsdfAtlas sampler");
        }

        _textureWidth = _atlasWidth;
        _textureHeight = _atlasHeight;
        _resourceVersion++;
        yinfo("Created MsdfAtlas texture {}x{}", _atlasWidth, _atlasHeight);
        return Ok();
    }

    Result<void> createGlyphMetadataBuffer(WGPUDevice device) override {
        if (_glyphMetadataBuffer) {
            wgpuBufferRelease(_glyphMetadataBuffer);
            _glyphMetadataBuffer = nullptr;
        }

        _bufferGlyphCount = static_cast<uint32_t>(_glyphMetadata.size()) + 256;
        size_t bufferSize = _bufferGlyphCount * sizeof(GlyphMetadataGPU);

        yinfo("GPU_ALLOC MsdfAtlas: glyphMetadataBuffer={} glyphs * {} bytes = {} bytes ({:.2f} MB)",
              _bufferGlyphCount, sizeof(GlyphMetadataGPU), bufferSize, bufferSize / (1024.0 * 1024.0));

        WGPUBufferDescriptor bufDesc = {};
        bufDesc.label = WGPU_STR("MsdfAtlas Glyph Metadata");
        bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
        bufDesc.size = bufferSize;
        bufDesc.mappedAtCreation = true;

        _glyphMetadataBuffer = wgpuDeviceCreateBuffer(device, &bufDesc);
        if (!_glyphMetadataBuffer) {
            return Err<void>("Failed to create MsdfAtlas metadata buffer");
        }

        void* mapped = wgpuBufferGetMappedRange(_glyphMetadataBuffer, 0, bufferSize);
        if (!mapped) {
            wgpuBufferRelease(_glyphMetadataBuffer);
            _glyphMetadataBuffer = nullptr;
            return Err<void>("Failed to map glyph metadata buffer");
        }
        std::memcpy(mapped, _glyphMetadata.data(), _glyphMetadata.size() * sizeof(GlyphMetadataGPU));
        wgpuBufferUnmap(_glyphMetadataBuffer);

        _resourceVersion++;
        yinfo("Created MsdfAtlas metadata buffer ({} glyphs, {} bytes)", _bufferGlyphCount, bufferSize);
        return Ok();
    }

    Result<void> uploadPendingGlyphs(WGPUDevice device, WGPUQueue queue) override {
        if (!_dirty) return Ok();

        // Check if atlas grew and texture needs to be recreated
        if (_atlasWidth > _textureWidth || _atlasHeight > _textureHeight) {
            yinfo("Atlas grew from {}x{} to {}x{}, recreating GPU texture",
                  _textureWidth, _textureHeight, _atlasWidth, _atlasHeight);
            auto result = createTexture(device, queue);
            if (!result) {
                return Err<void>("Failed to recreate texture after atlas growth", result);
            }
        } else if (_texture) {
            // Re-upload to existing texture
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
            wgpuQueueWriteTexture(queue, &dest,
                                  _atlasData.data(), _atlasData.size(),
                                  &layout, &extent);
        }

        // Update metadata buffer if it exists and has space
        if (_glyphMetadataBuffer) {
            if (_glyphMetadata.size() <= _bufferGlyphCount) {
                wgpuQueueWriteBuffer(queue, _glyphMetadataBuffer, 0,
                                     _glyphMetadata.data(),
                                     _glyphMetadata.size() * sizeof(GlyphMetadataGPU));
            } else {
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

    //=========================================================================
    // Accessors
    //=========================================================================

    WGPUTexture getTexture() const override { return _texture; }
    WGPUTextureView getTextureView() const override { return _textureView; }
    WGPUSampler getSampler() const override { return _sampler; }
    WGPUBuffer getGlyphMetadataBuffer() const override { return _glyphMetadataBuffer; }

    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }
    bool hasPendingGlyphs() const override { return _dirty; }
    uint32_t getAtlasWidth() const override { return _atlasWidth; }
    uint32_t getAtlasHeight() const override { return _atlasHeight; }
    const std::vector<uint8_t>& getAtlasData() const override { return _atlasData; }

    float getFontSize() const override { return _fontSize; }
    float getLineHeight() const override { return _lineHeight; }
    float getPixelRange() const override { return _pixelRange; }

    const std::vector<GlyphMetadataGPU>& getGlyphMetadata() const override { return _glyphMetadata; }
    uint32_t getGlyphCount() const override { return static_cast<uint32_t>(_glyphMetadata.size()); }
    uint32_t getBufferGlyphCount() const override { return _bufferGlyphCount; }
    uint32_t getResourceVersion() const override { return _resourceVersion; }

private:
    //=========================================================================
    // Atlas growth
    //=========================================================================

    void growAtlas() {
        uint32_t oldWidth = _atlasWidth;
        uint32_t oldHeight = _atlasHeight;
        uint32_t newWidth = _atlasWidth;
        uint32_t newHeight = _atlasHeight;

        bool canGrowWidth = (newWidth + ATLAS_GROW_INCREMENT <= ATLAS_MAX_DIMENSION);
        bool canGrowHeight = (newHeight + ATLAS_GROW_INCREMENT <= ATLAS_MAX_DIMENSION);

        if (!canGrowWidth && !canGrowHeight) {
            yerror("MSDF atlas at maximum size {}x{}, cannot grow further",
                   _atlasWidth, _atlasHeight);
            return;
        }

        // Grow both dimensions when possible
        if (canGrowHeight) newHeight += ATLAS_GROW_INCREMENT;
        if (canGrowWidth)  newWidth  += ATLAS_GROW_INCREMENT;

        // Width-only growth (height maxed): pack into the new right column
        if (canGrowWidth && !canGrowHeight) {
            _shelfX = oldWidth;
            _shelfY = 0;
            _shelfHeight = 0;
            _shelfMinX = oldWidth;  // prevent wrapping back into old data
        }

        yinfo("Growing MSDF atlas from {}x{} to {}x{}", oldWidth, oldHeight, newWidth, newHeight);

        std::vector<uint8_t> newAtlasData(newWidth * newHeight * 4, 0);

        for (uint32_t y = 0; y < oldHeight; ++y) {
            std::memcpy(
                newAtlasData.data() + y * newWidth * 4,
                _atlasData.data() + y * oldWidth * 4,
                oldWidth * 4
            );
        }

        _atlasData = std::move(newAtlasData);
        _atlasWidth = newWidth;
        _atlasHeight = newHeight;

        // Recalculate UV coordinates for all existing glyphs if dimensions changed
        float scaleX = static_cast<float>(oldWidth) / static_cast<float>(newWidth);
        float scaleY = static_cast<float>(oldHeight) / static_cast<float>(newHeight);
        for (auto& meta : _glyphMetadata) {
            if (oldWidth != newWidth) {
                meta._uvMinX *= scaleX;
                meta._uvMaxX *= scaleX;
            }
            if (oldHeight != newHeight) {
                meta._uvMinY *= scaleY;
                meta._uvMaxY *= scaleY;
            }
        }

        _dirty = true;
    }

    //=========================================================================
    // Private data
    //=========================================================================

    // CDB readers (indexed by fontId)
    std::vector<CdbReader::Ptr> _cdbFiles;

    // Per-font codepoint-to-glyph-index cache (indexed by fontId)
    std::vector<std::unordered_map<uint32_t, uint32_t>> _codepointToIndex;

    // Reverse mapping: glyph index -> codepoint (shared across all fonts)
    std::unordered_map<uint32_t, uint32_t> _indexToCodepoint;

    // GPU metadata for all loaded glyphs
    std::vector<GlyphMetadataGPU> _glyphMetadata;

    // Dirty flag
    bool _dirty = false;

    // Font metrics
    float _fontSize = 32.0f;
    float _lineHeight = 40.0f;
    float _pixelRange = 4.0f;

    // Runtime atlas (built on demand from CDB, grows dynamically)
    std::vector<uint8_t> _atlasData;
    uint32_t _atlasWidth = 1024;
    uint32_t _atlasHeight = 512;
    static constexpr uint32_t ATLAS_GROW_INCREMENT = 512;
    static constexpr uint32_t ATLAS_MAX_DIMENSION = 16384;

    // Shelf packer state for atlas
    uint32_t _shelfX = 0;
    uint32_t _shelfY = 0;
    uint32_t _shelfHeight = 0;
    uint32_t _shelfMinX = 0;  // Left bound for shelf wrapping (advances on width-only growth)
    static constexpr uint32_t ATLAS_PADDING = 2;

    // WebGPU resources
    WGPUTexture _texture = nullptr;
    WGPUTextureView _textureView = nullptr;
    WGPUSampler _sampler = nullptr;
    WGPUBuffer _glyphMetadataBuffer = nullptr;
    uint32_t _bufferGlyphCount = 0;
    uint32_t _resourceVersion = 0;
    uint32_t _textureWidth = 0;
    uint32_t _textureHeight = 0;
};

//=============================================================================
// MsdfAtlas::createImpl - ObjectFactory entry point
//=============================================================================

Result<MsdfAtlas::Ptr> MsdfAtlas::createImpl(ContextType&) {
    return Ok(Ptr(new MsdfAtlasImpl()));
}

} // namespace yetty
