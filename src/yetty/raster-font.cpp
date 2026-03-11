#include <yetty/raster-font.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>

namespace yetty {

//=============================================================================
// Internal types
//=============================================================================

namespace {

// Glyph UV offset for GPU - minimal for monospace terminal
// uvMax = uvMin + glyphSizeUV (computed in shader from uniform)
struct RasterGlyphUV {
    float uvX, uvY;  // UV top-left corner
};

static_assert(sizeof(RasterGlyphUV) == 8, "RasterGlyphUV must be 8 bytes");

} // anonymous namespace

//=============================================================================
// RasterFontImpl
//=============================================================================

class RasterFontImpl : public RasterFont {
public:
    RasterFontImpl(const GPUContext& gpu, GpuAllocator::Ptr allocator,
                   const std::string& ttfPath,
                   uint32_t cellWidth, uint32_t cellHeight)
        : _gpu(gpu), _allocator(std::move(allocator))
        , _ttfPath(ttfPath), _cellWidth(cellWidth), _cellHeight(cellHeight) {}

    ~RasterFontImpl() override {
        cleanup();
    }

    //=========================================================================
    // Initialization
    //=========================================================================

    Result<void> init() override {
        // Initialize FreeType
        if (FT_Init_FreeType(&_ftLibrary)) {
            return Err<void>("Failed to initialize FreeType");
        }

        // Load font face
        if (FT_New_Face(_ftLibrary, _ttfPath.c_str(), 0, &_ftFace)) {
            FT_Done_FreeType(_ftLibrary);
            _ftLibrary = nullptr;
            return Err<void>("Failed to load font: " + _ttfPath);
        }

        // Calculate proper font size from metrics
        updateFontSize();

        // Initialize atlas data (R8 grayscale)
        _atlasData.resize(_atlasSize * _atlasSize, 0);

        yinfo("RasterFont loaded: {} (cell={}x{}, fontSize={}, baseline={}, atlas={}x{})",
              _ttfPath, _cellWidth, _cellHeight, _fontSize, _baseline, _atlasSize, _atlasSize);

        return Ok();
    }

    void updateFontSize() {
        if (!_ftFace) return;

        // Set initial size to get metrics (use cell height as starting point)
        FT_Set_Pixel_Sizes(_ftFace, 0, _cellHeight);

        // Get font metrics in pixels (at current size)
        // FreeType metrics are in 26.6 fixed point (divide by 64)
        int ascender = _ftFace->size->metrics.ascender >> 6;
        int descender = std::abs(_ftFace->size->metrics.descender >> 6);
        int lineHeight = ascender + descender;

        // Scale font size so line height fits in cell height (with small margin)
        int targetHeight = static_cast<int>(_cellHeight) - 2;  // 1px margin top/bottom
        if (lineHeight > 0 && targetHeight > 0) {
            _fontSize = static_cast<uint32_t>(_cellHeight * targetHeight / lineHeight);
        } else {
            _fontSize = _cellHeight;
        }

        // Apply the calculated font size
        FT_Set_Pixel_Sizes(_ftFace, 0, _fontSize);

        // Recalculate metrics at new size
        ascender = _ftFace->size->metrics.ascender >> 6;
        descender = std::abs(_ftFace->size->metrics.descender >> 6);

        // Baseline position from top of cell (center the text vertically)
        int actualLineHeight = ascender + descender;
        int topMargin = (static_cast<int>(_cellHeight) - actualLineHeight) / 2;
        _baseline = topMargin + ascender;
    }

    //=========================================================================
    // Cell size management
    //=========================================================================

    void setCellSize(uint32_t cellWidth, uint32_t cellHeight) override {
        if (cellWidth == _cellWidth && cellHeight == _cellHeight) {
            return;  // No change
        }

        _cellWidth = cellWidth;
        _cellHeight = cellHeight;

        // Recalculate font size from metrics
        updateFontSize();

        // Clear and re-rasterize all loaded glyphs
        rasterizeAll();
    }

    uint32_t getCellWidth() const override { return _cellWidth; }
    uint32_t getCellHeight() const override { return _cellHeight; }

    //=========================================================================
    // Glyph loading
    //=========================================================================

    Result<void> loadGlyphs(const std::vector<uint32_t>& codepoints) override {
        for (uint32_t cp : codepoints) {
            if (_glyphIndices.count(cp)) continue;  // Already loaded

            auto result = rasterizeGlyph(cp);
            if (!result) {
                ywarn("Failed to rasterize glyph for U+{:04X}", cp);
                continue;
            }
            _loadedCodepoints.push_back(cp);
        }

        // Upload to GPU
        _dirty = true;
        return Ok();
    }

    Result<void> loadBasicLatin() override {
        std::vector<uint32_t> codepoints;

        // Basic Latin (ASCII printable: space to tilde)
        for (uint32_t cp = 0x20; cp <= 0x7E; ++cp) {
            codepoints.push_back(cp);
        }

        // Latin-1 Supplement (0xA0-0xFF: accented chars, symbols)
        for (uint32_t cp = 0xA0; cp <= 0xFF; ++cp) {
            codepoints.push_back(cp);
        }

        // Latin Extended-A (0x100-0x17F: more accented chars)
        for (uint32_t cp = 0x100; cp <= 0x17F; ++cp) {
            codepoints.push_back(cp);
        }

        // Box Drawing (0x2500-0x257F)
        for (uint32_t cp = 0x2500; cp <= 0x257F; ++cp) {
            codepoints.push_back(cp);
        }

        // Block Elements (0x2580-0x259F)
        for (uint32_t cp = 0x2580; cp <= 0x259F; ++cp) {
            codepoints.push_back(cp);
        }

        // Common math/programming symbols
        const uint32_t extraSymbols[] = {
            0x2190, 0x2191, 0x2192, 0x2193,  // arrows
            0x2194, 0x2195, 0x21D0, 0x21D2,  // double arrows
            0x2200, 0x2203, 0x2205, 0x2208,  // math: forall, exists, empty, element
            0x2260, 0x2264, 0x2265, 0x2227,  // not equal, <=, >=, and
            0x2228, 0x2229, 0x222A, 0x2248,  // or, intersection, union, approx
            0x221E, 0x2022, 0x2026, 0x00B7,  // infinity, bullet, ellipsis, middle dot
        };
        for (uint32_t cp : extraSymbols) {
            codepoints.push_back(cp);
        }

        return loadGlyphs(codepoints);
    }

    //=========================================================================
    // GPU resource access
    //=========================================================================

    WGPUTexture getTexture() const override { return _texture; }
    WGPUTextureView getTextureView() const override { return _textureView; }
    WGPUSampler getSampler() const override { return _sampler; }
    WGPUBuffer getMetadataBuffer() const override { return _metadataBuffer; }
    size_t getMetadataBufferSize() const override { return _glyphUVs.size() * sizeof(RasterGlyphUV); }

    //=========================================================================
    // Statistics
    //=========================================================================

    size_t glyphCount() const override { return _loadedCodepoints.size(); }
    uint32_t getFontSize() const override { return _cellHeight; }

    //=========================================================================
    // Font interface
    //=========================================================================

    uint32_t getGlyphIndex(uint32_t codepoint) override {
        auto it = _glyphIndices.find(codepoint);
        if (it != _glyphIndices.end()) {
            return it->second;
        }
        return 0;
    }

    uint32_t getGlyphIndex(uint32_t codepoint, Style) override {
        return getGlyphIndex(codepoint);  // Styles not supported yet
    }

    uint32_t getGlyphIndex(uint32_t codepoint, bool, bool) override {
        return getGlyphIndex(codepoint);
    }

    void uploadToGpu() override {
        if (!_dirty && _gpuResourcesCreated) {
            return;
        }

        if (!_gpuResourcesCreated) {
            if (auto res = createGPUResources(); !res) {
                yerror("RasterFont: failed to create GPU resources: {}", error_msg(res));
                return;
            }
        }

        WGPUQueue queue = _gpu.queue;

        // Upload atlas texture data
        WGPUTexelCopyTextureInfo destInfo = {};
        destInfo.texture = _texture;
        destInfo.mipLevel = 0;
        destInfo.origin = {0, 0, 0};
        destInfo.aspect = WGPUTextureAspect_All;

        WGPUTexelCopyBufferLayout srcLayout = {};
        srcLayout.offset = 0;
        srcLayout.bytesPerRow = _atlasSize;  // R8 = 1 byte per pixel
        srcLayout.rowsPerImage = _atlasSize;

        WGPUExtent3D extent = {_atlasSize, _atlasSize, 1};

        wgpuQueueWriteTexture(queue, &destInfo, _atlasData.data(),
                              _atlasData.size(), &srcLayout, &extent);

        // Upload UV buffer (indexed by codepoint directly)
        if (!_glyphUVs.empty()) {
            wgpuQueueWriteBuffer(queue, _metadataBuffer, 0,
                                 _glyphUVs.data(),
                                 _glyphUVs.size() * sizeof(RasterGlyphUV));
        }

        _dirty = false;
        yinfo("RasterFont: uploaded {} glyphs to GPU", _glyphUVs.size());
    }

    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }

    uint32_t getAtlasWidth() const override { return _atlasSize; }
    uint32_t getAtlasHeight() const override { return _atlasSize; }

    const std::vector<uint8_t>& getAtlasData() const override {
        return _atlasData;
    }

private:
    //=========================================================================
    // Re-rasterize all loaded glyphs (after cell size change)
    //=========================================================================

    void rasterizeAll() {
        // Clear atlas
        std::fill(_atlasData.begin(), _atlasData.end(), 0);
        _nextSlotIdx = 0;
        _glyphUVs.clear();
        _glyphIndices.clear();

        // Re-rasterize all loaded codepoints
        for (uint32_t cp : _loadedCodepoints) {
            auto result = rasterizeGlyph(cp);
            if (!result) {
                ywarn("Failed to re-rasterize glyph for U+{:04X}", cp);
            }
        }

        _dirty = true;
        yinfo("RasterFont: re-rasterized {} glyphs at cell size {}x{}",
              _loadedCodepoints.size(), _cellWidth, _cellHeight);
    }

    //=========================================================================
    // Rasterize a single glyph into a cell-sized slot
    //=========================================================================

    Result<void> rasterizeGlyph(uint32_t codepoint) {
        // Load glyph
        FT_UInt glyphIndex = FT_Get_Char_Index(_ftFace, codepoint);
        if (glyphIndex == 0) {
            return Err<void>("Glyph not found in font");
        }

        if (FT_Load_Glyph(_ftFace, glyphIndex, FT_LOAD_RENDER)) {
            return Err<void>("Failed to render glyph");
        }

        FT_GlyphSlot slot = _ftFace->glyph;
        FT_Bitmap& bitmap = slot->bitmap;

        // Ensure UV array is large enough (index by codepoint directly)
        if (codepoint >= _glyphUVs.size()) {
            _glyphUVs.resize(codepoint + 1, {-1.0f, -1.0f});
        }

        // Handle empty glyphs (e.g., space) - use invalid UV
        if (bitmap.width == 0 || bitmap.rows == 0) {
            _glyphUVs[codepoint] = {-1.0f, -1.0f};
            _glyphIndices[codepoint] = codepoint;
            return Ok();
        }

        // Fixed cell-sized slots in atlas with 1px padding to prevent bleed
        constexpr int SLOT_PADDING = 1;
        int cellW = static_cast<int>(_cellWidth);
        int cellH = static_cast<int>(_cellHeight);
        int slotW = cellW + SLOT_PADDING * 2;  // Cell + padding on both sides
        int slotH = cellH + SLOT_PADDING * 2;
        int slotsPerRow = static_cast<int>(_atlasSize) / slotW;

        if (slotsPerRow == 0) {
            return Err<void>("Cell width too large for atlas");
        }

        int slotCol = _nextSlotIdx % slotsPerRow;
        int slotRow = _nextSlotIdx / slotsPerRow;

        // Slot position includes padding
        int slotX = slotCol * slotW + SLOT_PADDING;
        int slotY = slotRow * slotH + SLOT_PADDING;

        if (slotY + cellH > static_cast<int>(_atlasSize)) {
            return Err<void>("Atlas is full");
        }

        // Clear the entire cell slot (including padding area) to prevent artifacts
        for (int y = -SLOT_PADDING; y < cellH + SLOT_PADDING; ++y) {
            for (int x = -SLOT_PADDING; x < cellW + SLOT_PADDING; ++x) {
                int dstIdx = (slotY + y) * static_cast<int>(_atlasSize) + (slotX + x);
                if (dstIdx >= 0 && dstIdx < static_cast<int>(_atlasData.size())) {
                    _atlasData[dstIdx] = 0;
                }
            }
        }

        // Position glyph using font metrics
        int glyphW = static_cast<int>(bitmap.width);
        int glyphH = static_cast<int>(bitmap.rows);
        int bearingX = slot->bitmap_left;
        int bearingY = slot->bitmap_top;

        // Use stored baseline (calculated from font metrics in updateFontSize)
        int offsetX = bearingX;  // Use glyph's bearing for horizontal
        int offsetY = _baseline - bearingY;  // Position relative to baseline

        // Clamp offsets to stay within cell
        offsetX = std::max(0, std::min(offsetX, cellW - glyphW));
        offsetY = std::max(0, std::min(offsetY, cellH - glyphH));

        // Copy bitmap to atlas at cell slot position + offset
        for (int y = 0; y < glyphH && (offsetY + y) < cellH; ++y) {
            for (int x = 0; x < glyphW && (offsetX + x) < cellW; ++x) {
                int srcIdx = y * bitmap.pitch + x;
                int dstIdx = (slotY + offsetY + y) * static_cast<int>(_atlasSize) + (slotX + offsetX + x);
                if (dstIdx >= 0 && dstIdx < static_cast<int>(_atlasData.size())) {
                    _atlasData[dstIdx] = bitmap.buffer[srcIdx];
                }
            }
        }

        // Store UV at codepoint index directly (points to cell slot top-left)
        _glyphUVs[codepoint] = {
            static_cast<float>(slotX) / _atlasSize,
            static_cast<float>(slotY) / _atlasSize
        };
        _glyphIndices[codepoint] = codepoint;

        _nextSlotIdx++;
        _dirty = true;
        return Ok();
    }

    //=========================================================================
    // Create GPU resources
    //=========================================================================

    Result<void> createGPUResources() {
        WGPUDevice device = _gpu.device;

        // Create R8 texture
        WGPUTextureDescriptor texDesc = {};
        texDesc.label = WGPU_STR("RasterFont Atlas");
        texDesc.size = {_atlasSize, _atlasSize, 1};
        texDesc.mipLevelCount = 1;
        texDesc.sampleCount = 1;
        texDesc.dimension = WGPUTextureDimension_2D;
        texDesc.format = WGPUTextureFormat_R8Unorm;
        texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;

        _texture = _allocator->createTexture(texDesc);
        if (!_texture) {
            return Err<void>("Failed to create RasterFont texture");
        }

        // Create texture view
        WGPUTextureViewDescriptor viewDesc = {};
        viewDesc.format = WGPUTextureFormat_R8Unorm;
        viewDesc.dimension = WGPUTextureViewDimension_2D;
        viewDesc.mipLevelCount = 1;
        viewDesc.arrayLayerCount = 1;

        _textureView = wgpuTextureCreateView(_texture, &viewDesc);
        if (!_textureView) {
            return Err<void>("Failed to create RasterFont texture view");
        }

        // Create sampler
        WGPUSamplerDescriptor samplerDesc = {};
        samplerDesc.label = WGPU_STR("RasterFont Sampler");
        samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
        samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
        samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
        samplerDesc.magFilter = WGPUFilterMode_Linear;
        samplerDesc.minFilter = WGPUFilterMode_Linear;
        samplerDesc.mipmapFilter = WGPUMipmapFilterMode_Nearest;
        samplerDesc.maxAnisotropy = 1;

        _sampler = wgpuDeviceCreateSampler(device, &samplerDesc);
        if (!_sampler) {
            return Err<void>("Failed to create RasterFont sampler");
        }

        // Create metadata buffer (indexed by codepoint, need space for highest codepoint)
        // Box drawing is at 0x2500-0x259F, block elements 0x2580-0x259F = up to 0x259F = 9631
        // Allocate for codepoints up to 0x3000 (12288) to be safe
        uint32_t maxCodepoint = 0x3000;
        WGPUBufferDescriptor bufDesc = {};
        bufDesc.label = WGPU_STR("RasterFont Metadata");
        bufDesc.size = maxCodepoint * sizeof(RasterGlyphUV);
        bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;

        _metadataBuffer = _allocator->createBuffer(bufDesc);
        if (!_metadataBuffer) {
            return Err<void>("Failed to create RasterFont metadata buffer");
        }

        _gpuResourcesCreated = true;
        return Ok();
    }

    //=========================================================================
    // Cleanup
    //=========================================================================

    void cleanup() {
        if (_metadataBuffer) {
            _allocator->releaseBuffer(_metadataBuffer);
            _metadataBuffer = nullptr;
        }
        if (_sampler) {
            wgpuSamplerRelease(_sampler);
            _sampler = nullptr;
        }
        if (_textureView) {
            wgpuTextureViewRelease(_textureView);
            _textureView = nullptr;
        }
        if (_texture) {
            _allocator->releaseTexture(_texture);
            _texture = nullptr;
        }
        if (_ftFace) {
            FT_Done_Face(_ftFace);
            _ftFace = nullptr;
        }
        if (_ftLibrary) {
            FT_Done_FreeType(_ftLibrary);
            _ftLibrary = nullptr;
        }
    }

    //=========================================================================
    // Private data
    //=========================================================================

    const GPUContext& _gpu;
    GpuAllocator::Ptr _allocator;
    std::string _ttfPath;
    uint32_t _cellWidth;
    uint32_t _cellHeight;
    uint32_t _fontSize = 0;   // Calculated from font metrics to fit cell
    int _baseline = 0;        // Baseline position from top of cell

    // FreeType
    FT_Library _ftLibrary = nullptr;
    FT_Face _ftFace = nullptr;

    // Atlas
    static constexpr uint32_t _atlasSize = 1024;  // 1024x1024 R8 texture
    std::vector<uint8_t> _atlasData;

    // Cell slot packer state
    int _nextSlotIdx = 0;

    // Glyph data
    std::vector<RasterGlyphUV> _glyphUVs;
    std::unordered_map<uint32_t, uint32_t> _glyphIndices;  // codepoint -> index
    std::vector<uint32_t> _loadedCodepoints;  // track for re-rasterization

    // GPU resources
    WGPUTexture _texture = nullptr;
    WGPUTextureView _textureView = nullptr;
    WGPUSampler _sampler = nullptr;
    WGPUBuffer _metadataBuffer = nullptr;
    bool _gpuResourcesCreated = false;
    bool _dirty = false;
};

//=============================================================================
// RasterFont::createImpl - ObjectFactory entry point
//=============================================================================

Result<RasterFont::Ptr> RasterFont::createImpl(ContextType&,
                                               const GPUContext& gpu,
                                               GpuAllocator::Ptr allocator,
                                               const std::string& ttfPath,
                                               uint32_t cellWidth,
                                               uint32_t cellHeight) {
    auto font = Ptr(new RasterFontImpl(gpu, std::move(allocator), ttfPath, cellWidth, cellHeight));
    if (auto res = static_cast<RasterFontImpl*>(font.get())->init(); !res) {
        return Err<Ptr>("Failed to initialize RasterFont", res);
    }
    return Ok(std::move(font));
}

} // namespace yetty
