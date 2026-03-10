//=============================================================================
// YPdf Render Pipeline Tests
//
// Loads the REAL demo PDF (demo/assets/sample-local-pdf.pdf) through
// renderPdfToBuffer(), then runs the full builder pipeline and verifies
// the GPU buffer contents match what the shader reads.
//
// This tests the actual path that produces a white card in production.
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>

extern "C" {
#include <pdfio.h>
}

#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include "yetty/ydraw/ydraw-prim-writer.gen.h"
#include "yetty/ypdf/pdf-renderer.h"
#include <yetty/ydraw-builder.h>
#include <yetty/card-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/font-manager.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include <yetty/msdf-cdb-provider.h>
#include <yetty/gpu-allocator.h>

#include <yetty/card-texture-manager.h>
#include <yetty/msdf-glyph-data.h>

#include <cstring>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <vector>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::card;

static const std::string kSamplePdf =
    std::string(CMAKE_SOURCE_DIR) + "/demo/assets/sample-local-pdf.pdf";

static bool errorHandler(pdfio_file_t*, const char*, void*) { return true; }

//=============================================================================
// Helper: read u32 from float buffer at word index
//=============================================================================
static uint32_t readU32(const float* buf, uint32_t wordIdx) {
    uint32_t val;
    std::memcpy(&val, &buf[wordIdx], sizeof(uint32_t));
    return val;
}

//=============================================================================
// Mock CardBufferManager
//=============================================================================
class MockCardBufferManager : public CardBufferManager {
public:
    void reserve(uint32_t size) override {
        _pendingReservation += (size + 15) & ~15u;
    }

    Result<void> commitReservations() override {
        uint32_t total = std::max(_pendingReservation, 16u);
        _storage.resize(total, 0);
        _nextOffset = 0;
        _allocs.clear();
        _pendingReservation = 0;
        return Ok();
    }

    Result<BufferHandle> allocateBuffer(uint32_t slotIndex,
                                         const std::string& scope,
                                         uint32_t size) override {
        // Match real BufferAllocator: reject 0-byte allocations
        if (size == 0) {
            return Err<BufferHandle>("MockCardBufferManager: cannot allocate zero bytes");
        }
        size = (size + 15) & ~15u;
        if (_nextOffset + size > _storage.size()) {
            return Err<BufferHandle>("MockCardBufferManager: out of space");
        }
        BufferHandle h{_storage.data() + _nextOffset, _nextOffset, size};
        _allocs[{slotIndex, scope}] = h;
        _nextOffset += size;
        return Ok(h);
    }

    void markBufferDirty(BufferHandle) override {}

    BufferHandle getBufferHandle(uint32_t slotIndex,
                                  const std::string& scope) const override {
        auto it = _allocs.find({slotIndex, scope});
        if (it != _allocs.end()) return it->second;
        return BufferHandle::invalid();
    }

    Result<void> flush(WGPUQueue) override { return Ok(); }
    WGPUBuffer bufferGpu() const override { return nullptr; }
    uint32_t bufferHighWaterMark() const override { return _nextOffset; }
    bool bindGroupDirty() const override { return false; }
    void clearBindGroupDirty() override {}
    SharedMemoryRegion::Ptr shm() const override { return nullptr; }
    bool usesSharedMemory() const override { return false; }
    Stats getStats() const override { return {_nextOffset, (uint32_t)_storage.size(), 0}; }
    void dumpSubAllocations() const override {}
    std::string dumpSubAllocationsToString() const override { return ""; }
    std::vector<BufferInfo> getAllBuffers() const override { return {}; }

    const float* storageAsFloat() const {
        return reinterpret_cast<const float*>(_storage.data());
    }
    uint32_t storageWordCount() const {
        return static_cast<uint32_t>(_storage.size() / sizeof(float));
    }

private:
    std::vector<uint8_t> _storage;
    uint32_t _nextOffset = 0;
    uint32_t _pendingReservation = 0;
    std::map<std::pair<uint32_t, std::string>, BufferHandle> _allocs;
};

//=============================================================================
// Mock CardTextureManager — stores atlas pixels in CPU memory
//=============================================================================
class MockCardTextureManager : public CardTextureManager {
public:
    Result<TextureHandle> allocate(uint32_t width, uint32_t height) override {
        uint32_t id = static_cast<uint32_t>(_entries.size()) + 1;
        _entries.push_back({id, width, height, {}});
        return Ok(TextureHandle{id});
    }

    Result<void> write(TextureHandle handle, const uint8_t* pixels) override {
        for (auto& e : _entries) {
            if (e.id == handle.id) {
                uint32_t bytes = e.width * e.height * 4;
                e.pixels.assign(pixels, pixels + bytes);
                return Ok();
            }
        }
        return Err<void>("MockCardTextureManager: handle not found");
    }

    AtlasPosition getAtlasPosition(TextureHandle handle) const override {
        // Return (0,0) — no packing needed in mock
        return {0, 0};
    }

    void clearHandles() override { _entries.clear(); }
    Result<void> createAtlas() override { return Ok(); }
    Result<void> uploadAtlas(WGPUQueue) override { return Ok(); }
    WGPUTexture atlasTexture() const override { return nullptr; }
    WGPUTextureView atlasTextureView() const override { return nullptr; }
    WGPUSampler atlasSampler() const override { return nullptr; }
    bool atlasInitialized() const override { return !_entries.empty(); }
    uint32_t atlasSize() const override { return 2048; }
    Stats getStats() const override { return {(uint32_t)_entries.size(), 2048, 2048, 0}; }

    // Test accessors
    uint32_t entryCount() const { return static_cast<uint32_t>(_entries.size()); }
    bool hasPixels(uint32_t idx = 0) const {
        return idx < _entries.size() && !_entries[idx].pixels.empty();
    }
    const std::vector<uint8_t>& pixels(uint32_t idx = 0) const {
        return _entries[idx].pixels;
    }
    uint32_t textureWidth(uint32_t idx = 0) const { return _entries[idx].width; }
    uint32_t textureHeight(uint32_t idx = 0) const { return _entries[idx].height; }

    // Check if pixels contain non-zero data (atlas actually rendered)
    bool hasNonZeroPixels(uint32_t idx = 0) const {
        if (idx >= _entries.size()) return false;
        for (uint8_t b : _entries[idx].pixels) {
            if (b != 0) return true;
        }
        return false;
    }

private:
    struct Entry {
        uint32_t id;
        uint32_t width, height;
        std::vector<uint8_t> pixels;
    };
    std::vector<Entry> _entries;
};

//=============================================================================
// Mock CardManager
//=============================================================================
class MockCardManager : public CardManager {
public:
    MockCardManager()
        : _bufMgr(std::make_shared<MockCardBufferManager>())
        , _texMgr(std::make_shared<MockCardTextureManager>())
        , _metadata(16 * 64, 0) {}

    Result<MetadataHandle> allocateMetadata(uint32_t size) override {
        return Ok(MetadataHandle{0, 64});
    }
    Result<void> deallocateMetadata(MetadataHandle) override { return Ok(); }

    Result<void> writeMetadata(MetadataHandle handle,
                                const void* data, uint32_t size) override {
        if (handle.offset + size > _metadata.size()) {
            return Err<void>("MockCardManager: metadata overflow");
        }
        std::memcpy(_metadata.data() + handle.offset, data, size);
        return Ok();
    }

    Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset,
                                  const void* data, uint32_t size) override {
        return writeMetadata(
            MetadataHandle{handle.offset + offset, handle.size - offset},
            data, size);
    }

    CardBufferManager::Ptr bufferManager() const override { return _bufMgr; }
    CardTextureManager::Ptr textureManager() const override { return _texMgr; }
    WGPUBuffer metadataBuffer() const override { return nullptr; }
    WGPUBindGroupLayout sharedBindGroupLayout() const override { return nullptr; }
    WGPUBindGroup sharedBindGroup() const override { return nullptr; }
    Result<void> updateBindGroup() override { return Ok(); }
    void invalidateBindGroup() override {}
    Result<void> flush(WGPUQueue) override { return Ok(); }

    std::shared_ptr<MockCardBufferManager> mockBufMgr() const { return _bufMgr; }
    std::shared_ptr<MockCardTextureManager> mockTexMgr() const { return _texMgr; }

    const uint32_t* metadataAsU32() const {
        return reinterpret_cast<const uint32_t*>(_metadata.data());
    }

    float metadataFloat(uint32_t wordIdx) const {
        float val;
        std::memcpy(&val, &metadataAsU32()[wordIdx], sizeof(float));
        return val;
    }

private:
    std::shared_ptr<MockCardBufferManager> _bufMgr;
    std::shared_ptr<MockCardTextureManager> _texMgr;
    std::vector<uint8_t> _metadata;
};

//=============================================================================
// MockFontManager
//=============================================================================
class MockFontManager : public FontManager {
public:
    MockFontManager(MsMsdfFont::Ptr font, const std::string& cacheDir)
        : _font(std::move(font)), _cacheDir(cacheDir) {}

    Result<MsMsdfFont::Ptr> getMsMsdfFont(const std::string&) noexcept override {
        return Ok(_font);
    }
    MsMsdfFont::Ptr getDefaultMsMsdfFont() noexcept override { return _font; }
    BmFont::Ptr getDefaultBmFont() noexcept override { return nullptr; }
    ShaderFont::Ptr getDefaultShaderGlyphFont() noexcept override { return nullptr; }
    ShaderFont::Ptr getDefaultCardFont() noexcept override { return nullptr; }
    Result<VectorSdfFont::Ptr> getVectorSdfFont(const std::string&) noexcept override {
        return Err<VectorSdfFont::Ptr>("stub");
    }
    VectorSdfFont::Ptr getDefaultVectorSdfFont() noexcept override { return nullptr; }
    Result<VectorCoverageFont::Ptr> getVectorCoverageFont(const std::string&) noexcept override {
        return Err<VectorCoverageFont::Ptr>("stub");
    }
    VectorCoverageFont::Ptr getDefaultVectorCoverageFont() noexcept override { return nullptr; }
    Result<RasterFont::Ptr> getRasterFont(const std::string&, uint32_t, uint32_t) noexcept override {
        return Err<RasterFont::Ptr>("stub");
    }
    RasterFont::Ptr getDefaultRasterFont() noexcept override { return nullptr; }
    void setDefaultFont(const std::string&) noexcept override {}
    bool hasFont(const std::string&) const noexcept override { return false; }
    const std::string& getCacheDir() const noexcept override { return _cacheDir; }
    MsdfCdbProvider::Ptr getCdbProvider() const noexcept override {
        static auto provider = std::make_shared<CpuMsdfCdbProvider>();
        return provider;
    }

private:
    MsMsdfFont::Ptr _font;
    std::string _cacheDir;
};

//=============================================================================
// Font infrastructure
//=============================================================================

static std::string ensureCdbBasePath() {
    std::string homeDir = std::getenv("HOME");
    std::string cacheDir = homeDir + "/.cache/yetty/msdf-font-cache";
    std::string basePath = cacheDir + "/DejaVuSansMNerdFontMono";
    std::string regularCdb = basePath + "-Regular.cdb";

    if (std::filesystem::exists(regularCdb)) return basePath;

    std::string assetsDir = std::string(CMAKE_SOURCE_DIR) + "/assets";
    auto provider = std::make_shared<CpuMsdfCdbProvider>();
    std::filesystem::create_directories(cacheDir);

    for (const char* style : {"-Regular", "-Bold", "-Oblique", "-BoldOblique"}) {
        MsdfCdbConfig cfg;
        cfg.ttfPath = assetsDir + "/DejaVuSansMNerdFontMono" + std::string(style) + ".ttf";
        cfg.cdbPath = basePath + std::string(style) + ".cdb";
        if (!std::filesystem::exists(cfg.cdbPath)) {
            provider->generate(cfg);
        }
    }
    return basePath;
}

static GpuAllocator::Ptr testAllocator() {
    static auto alloc = std::make_shared<GpuAllocator>(nullptr);
    return alloc;
}

static FontManager::Ptr createTestFontManager() {
    auto basePath = ensureCdbBasePath();
    auto fontRes = MsMsdfFont::create(basePath, testAllocator());
    if (!fontRes) return nullptr;

    std::string homeDir = std::getenv("HOME");
    std::string cacheDir = homeDir + "/.cache/yetty/msdf-font-cache";
    return std::make_shared<MockFontManager>(*fontRes, cacheDir);
}

static FontManager::Ptr testFontManager() {
    static auto fm = createTestFontManager();
    return fm;
}

//=============================================================================
// Full pipeline helper — runs the EXACT same lifecycle as ypdf card in production
// init → renderToStaging(calculate) → declareBufferNeeds → allocateBuffers
//      → allocateTextures → writeTextures → finalize(writeBuffers)
//=============================================================================
struct PipelineResult {
    std::shared_ptr<MockCardManager> cardMgr;
    YDrawBuilder::Ptr builder;
    YDrawBuffer::Ptr buffer;

    const uint32_t* meta() const { return cardMgr->metadataAsU32(); }
    const float* storage() const { return cardMgr->mockBufMgr()->storageAsFloat(); }
    auto texMgr() const { return cardMgr->mockTexMgr(); }
};

static PipelineResult runFullPipeline(const std::string& pdfPath) {
    PipelineResult r;

    auto* pdf = pdfioFileOpen(pdfPath.c_str(), nullptr, nullptr,
                              errorHandler, nullptr);
    if (!pdf) return r;

    r.buffer = *YDrawBuffer::create();
    renderPdfToBuffer(pdf, r.buffer);
    pdfioFileClose(pdf);

    auto fm = testFontManager();
    r.cardMgr = std::make_shared<MockCardManager>();
    r.builder = *YDrawBuilder::create(fm, testAllocator(), r.buffer,
                                       r.cardMgr, 0);

    // ypdf.cpp init sets viewport + view, then renderAllPages fills buffer.
    // Then renderToStaging calls calculate() when dirty.
    r.builder->setViewport(80, 24);

    // renderToStaging → calculate (ingests buffer fonts, text spans, scene bounds, bgColor)
    r.builder->calculate();

    // declareBufferNeeds
    r.builder->declareBufferNeeds();
    r.cardMgr->mockBufMgr()->commitReservations();

    // allocateBuffers → writeGPU + writeDerived
    r.builder->allocateBuffers();

    // allocateTextures → creates atlas texture handle
    r.builder->allocateTextures();

    // writeTextures → writes atlas pixels + updates atlas header in derived buffer
    r.builder->writeTextures();

    // finalize → writeBuffers → flushMetadata
    r.builder->writeBuffers();

    return r;
}

//=============================================================================
// Tests — renderPdfToBuffer produces buffer content
//=============================================================================

suite ypdf_render_buffer_tests = [] {

    "renderPdfToBuffer populates buffer with prims and text"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr) << "failed to open " << kSamplePdf;
        if (!pdf) return;

        auto buf = *YDrawBuffer::create();
        auto result = renderPdfToBuffer(pdf, buf);
        pdfioFileClose(pdf);

        expect(result.pageCount == 3) << "sample PDF has 3 pages";
        expect(result.totalHeight > 0.0_f) << "total height should be > 0";
        expect(result.firstPageHeight > 0.0_f);

        // Sample PDF is text-only — primCount may be 0.
        // Just verify the call doesn't crash and the count is consistent.
        // (A PDF with rectangles/lines would produce prims here.)

        // Buffer should have text spans
        expect(buf->textSpanCount() > 0_u)
            << "PDF should produce text spans, got " << buf->textSpanCount();

        // Buffer should have font blobs (embedded fonts)
        uint32_t fontCount = 0;
        buf->forEachFont([&](int, const uint8_t*, size_t, const std::string&) {
            fontCount++;
        });
        expect(fontCount > 0_u) << "PDF should have embedded fonts, got " << fontCount;

        // Scene bounds should be set
        expect(buf->hasSceneBounds()) << "scene bounds should be set";
        expect(buf->sceneMaxX() > 0.0_f) << "sceneMaxX > 0";
        expect(buf->sceneMaxY() > 0.0_f) << "sceneMaxY > 0";

        // bgColor should be white
        expect(buf->bgColor() == 0xFFFFFFFFu)
            << "PDF bg should be white (0xFFFFFFFF), got 0x" << std::hex << buf->bgColor();
    };

    "renderPdfToBuffer text spans have valid font IDs"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        if (!pdf) return;

        auto buf = *YDrawBuffer::create();
        renderPdfToBuffer(pdf, buf);
        pdfioFileClose(pdf);

        // Collect valid font IDs
        std::set<int> validFontIds;
        buf->forEachFont([&](int fontId, const uint8_t*, size_t, const std::string&) {
            validFontIds.insert(fontId);
        });

        // Every text span should reference a valid font ID
        uint32_t spanCount = 0;
        uint32_t spansWithValidFont = 0;
        buf->forEachTextSpan([&](const TextSpanData& span) {
            spanCount++;
            if (validFontIds.count(span.fontId) > 0 || span.fontId == -1)
                spansWithValidFont++;
        });

        expect(spanCount > 0_u) << "should have text spans";
        expect(spansWithValidFont == spanCount)
            << "all text spans should have valid font IDs: "
            << spansWithValidFont << " / " << spanCount;
    };

    "renderPdfToBuffer font blobs are valid TrueType"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        if (!pdf) return;

        auto buf = *YDrawBuffer::create();
        renderPdfToBuffer(pdf, buf);
        pdfioFileClose(pdf);

        buf->forEachFont([&](int fontId, const uint8_t* data, size_t size,
                             const std::string& name) {
            expect(size > 100_u)
                << "font '" << name << "' blob should be non-trivial";

            // Check TrueType magic
            if (size >= 4) {
                uint32_t magic = (uint32_t(data[0]) << 24) |
                                 (uint32_t(data[1]) << 16) |
                                 (uint32_t(data[2]) << 8) |
                                  uint32_t(data[3]);
                bool validMagic = (magic == 0x00010000) ||
                                  (magic == 0x74727565) ||
                                  (magic == 0x4F54544F);
                expect(validMagic)
                    << "font '" << name << "' should have valid TrueType/CFF magic";
            }
        });
    };
};

//=============================================================================
// Tests — Full pipeline: renderPdfToBuffer → builder → GPU buffers
//=============================================================================

suite ypdf_full_pipeline_tests = [] {

    "PDF full pipeline: font → atlas → texture → GPU"_test = [] {
        auto r = runFullPipeline(kSamplePdf);
        expect(r.builder != nullptr) << "pipeline should succeed";
        if (!r.builder) return;

        const uint32_t* meta = r.meta();
        const float* storage = r.storage();

        // ---- Custom atlas created from embedded fonts ----
        expect(r.builder->hasCustomAtlas())
            << "builder MUST have custom atlas for embedded PDF fonts";

        auto atlas = r.builder->customAtlas();
        expect(atlas != nullptr);
        if (!atlas) return;

        const auto& glyphMeta = atlas->getGlyphMetadata();
        expect(!glyphMeta.empty()) << "custom atlas should have glyph metadata";

        uint32_t atlasW = atlas->getAtlasWidth();
        uint32_t atlasH = atlas->getAtlasHeight();
        expect(atlasW > 0_u) << "atlas width > 0";
        expect(atlasH > 0_u) << "atlas height > 0";

        const auto& atlasData = atlas->getAtlasData();
        expect(!atlasData.empty()) << "atlas pixel data should not be empty";

        // CUSTOM_ATLAS flag in metadata (bit 5 = 32)
        uint32_t flags = meta[14] & 0xFFFF;
        expect((flags & 32u) != 0u)
            << "YDRAW_FLAG_CUSTOM_ATLAS (32) must be set in GPU metadata, flags=" << flags;

        // ---- Texture manager received atlas pixels ----
        auto texMgr = r.texMgr();
        expect(texMgr->entryCount() > 0_u)
            << "allocateTextures should have allocated a texture";
        expect(texMgr->hasPixels())
            << "writeTextures should have written pixel data";
        expect(texMgr->hasNonZeroPixels())
            << "atlas pixels should contain actual MSDF data (not all zeros)";

        // ---- Glyphs in GPU buffer ----
        uint32_t glyphCount = meta[7];
        uint32_t glyphOffset = meta[6];
        expect(glyphCount > 10_u)
            << "PDF text should produce many glyphs, got " << glyphCount;

        if (glyphCount > 0) {
            const YDrawGlyph* glyphs = reinterpret_cast<const YDrawGlyph*>(
                &storage[glyphOffset]);

            // All glyphs should have CUSTOM_ATLAS flag
            uint32_t customCount = 0;
            for (uint32_t i = 0; i < glyphCount; i++) {
                uint8_t gflags = static_cast<uint8_t>(glyphs[i].glyphLayerFlags >> 24);
                if (gflags & YDrawBuilder::GLYPH_FLAG_CUSTOM_ATLAS) customCount++;
            }
            expect(customCount == glyphCount)
                << "ALL glyphs should use custom atlas, got "
                << customCount << " / " << glyphCount;

            // Glyphs should have valid positions and sizes
            for (uint32_t i = 0; i < std::min(glyphCount, 5u); i++) {
                expect(glyphs[i].width() > 0.0_f)
                    << "glyph " << i << " width should be > 0";
                expect(glyphs[i].height() > 0.0_f)
                    << "glyph " << i << " height should be > 0";
                expect(glyphs[i].color != 0u)
                    << "glyph " << i << " color should be non-zero";
            }
        }

        // ---- Atlas header in derived buffer ----
        uint32_t atlasHeaderOff = glyphOffset + glyphCount * 5u;
        uint32_t atlasHdr0 = readU32(storage, atlasHeaderOff);
        uint32_t atlasHdr2 = readU32(storage, atlasHeaderOff + 2);

        // atlasHdr0 = (atlasX & 0xFFFF) | (atlasW << 16)
        uint32_t hdrAtlasW = atlasHdr0 >> 16;
        expect(hdrAtlasW == atlasW)
            << "atlas header width should match: got " << hdrAtlasW << " expected " << atlasW;

        // atlasHdr2 = number of glyph metadata entries
        expect(atlasHdr2 == static_cast<uint32_t>(glyphMeta.size()))
            << "atlas header glyph count: " << atlasHdr2
            << " vs " << glyphMeta.size();

        // ---- Glyph metadata UVs in derived buffer ----
        // First check the SOURCE atlas metadata (before GPU copy)
        bool anyNonZeroUV = false;
        for (size_t i = 0; i < glyphMeta.size(); i++) {
            if (glyphMeta[i]._uvMaxX > 0.0f || glyphMeta[i]._uvMaxY > 0.0f) {
                anyNonZeroUV = true;
                break;
            }
        }
        expect(anyNonZeroUV)
            << "atlas glyph metadata SOURCE should have non-zero UVs"
            << " — if false, atlas didn't rasterize glyphs (check CDB/font path)";

        // Then check what writeDerived copied into the GPU buffer
        uint32_t glyphMetaOff = atlasHeaderOff + 4;
        // Find first glyph with non-zero UV in source data
        for (size_t i = 0; i < std::min(glyphMeta.size(), size_t(10)); i++) {
            if (glyphMeta[i]._uvMaxX > 0.0f) {
                // Each GlyphMetadataGPU = 10 floats
                float uvMinX = storage[glyphMetaOff + i * 10 + 0];
                float uvMinY = storage[glyphMetaOff + i * 10 + 1];
                float uvMaxX = storage[glyphMetaOff + i * 10 + 2];
                float uvMaxY = storage[glyphMetaOff + i * 10 + 3];
                expect(uvMaxX > uvMinX)
                    << "GPU glyph[" << i << "] UV: maxX=" << uvMaxX
                    << " > minX=" << uvMinX
                    << " (source: " << glyphMeta[i]._uvMaxX << ")";
                expect(uvMaxY > uvMinY)
                    << "GPU glyph[" << i << "] UV: maxY=" << uvMaxY
                    << " > minY=" << uvMinY
                    << " (source: " << glyphMeta[i]._uvMaxY << ")";
                break;
            }
        }
    };

    "PDF full pipeline: grid has GLYPH_BIT entries"_test = [] {
        auto r = runFullPipeline(kSamplePdf);
        if (!r.builder) return;

        const uint32_t* meta = r.meta();
        const float* storage = r.storage();

        uint32_t gridOffset = meta[2];
        uint32_t gridWidth = meta[3];
        uint32_t gridHeight = meta[4];

        expect(gridWidth > 0_u) << "grid width > 0";
        expect(gridHeight > 0_u) << "grid height > 0";

        bool hasGlyphEntries = false;
        uint32_t numCells = gridWidth * gridHeight;
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(storage, gridOffset + ci);
            uint32_t cnt = readU32(storage, gridOffset + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(storage, gridOffset + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) != 0) {
                    hasGlyphEntries = true;
                }
            }
        }

        expect(hasGlyphEntries)
            << "grid MUST contain GLYPH_BIT entries for PDF text";
    };

    "PDF full pipeline: metadata bgColor white, text black"_test = [] {
        auto r = runFullPipeline(kSamplePdf);
        if (!r.builder) return;

        const uint32_t* meta = r.meta();
        const float* storage = r.storage();

        // bgColor = white
        expect(meta[15] == 0xFFFFFFFFu)
            << "bgColor should be white, got 0x" << std::hex << meta[15];

        // Text spans should NOT be white (would be invisible on white bg)
        uint32_t whiteSpans = 0, totalSpans = 0;
        r.buffer->forEachTextSpan([&](const TextSpanData& span) {
            totalSpans++;
            if (span.color == 0xFFFFFFFFu) whiteSpans++;
        });
        expect(totalSpans > 0_u);
        if (totalSpans > 0) {
            float ratio = float(whiteSpans) / float(totalSpans);
            expect(ratio < 0.1_f)
                << "text should NOT be white on white: "
                << whiteSpans << "/" << totalSpans;
        }

        // Glyph colors in GPU buffer should also not be white
        uint32_t glyphCount = meta[7];
        uint32_t glyphOffset = meta[6];
        if (glyphCount > 0) {
            const YDrawGlyph* glyphs = reinterpret_cast<const YDrawGlyph*>(
                &storage[glyphOffset]);
            uint32_t whiteGlyphs = 0;
            for (uint32_t i = 0; i < glyphCount; i++) {
                if (glyphs[i].color == 0xFFFFFFFFu) whiteGlyphs++;
            }
            float gRatio = float(whiteGlyphs) / float(glyphCount);
            expect(gRatio < 0.1_f)
                << "glyphs should NOT be white: "
                << whiteGlyphs << "/" << glyphCount;
        }
    };

    "PDF full pipeline: font ID mapping buffer→atlas"_test = [] {
        auto r = runFullPipeline(kSamplePdf);
        if (!r.builder) return;

        // Verify buffer has fonts
        std::vector<int> bufFontIds;
        r.buffer->forEachFont([&](int fontId, const uint8_t*, size_t,
                                   const std::string&) {
            bufFontIds.push_back(fontId);
        });
        expect(!bufFontIds.empty()) << "must have buffer fonts";

        // Verify text spans reference buffer font IDs
        std::set<int> spanFontIds;
        r.buffer->forEachTextSpan([&](const TextSpanData& span) {
            spanFontIds.insert(span.fontId);
        });
        for (int sfid : spanFontIds) {
            bool found = false;
            for (int bfid : bufFontIds) {
                if (bfid == sfid) { found = true; break; }
            }
            expect(found) << "span fontId " << sfid << " not in buffer fonts";
        }

        // After pipeline, all glyphs should use custom atlas
        uint32_t gc = r.builder->glyphCount();
        expect(gc > 0_u) << "must have glyphs";
    };

    // =========================================================================
    // SHADER SIMULATION — reads GPU buffer exactly like 0x0003-ydraw.wgsl
    // This test walks through the shader's rendering logic on a pixel at the
    // center of the first text glyph to verify the shader would see it.
    // =========================================================================
    "PDF shader simulation: grid lookup finds glyphs"_test = [] {
        auto r = runFullPipeline(kSamplePdf);
        if (!r.builder) return;

        const uint32_t* meta = r.meta();
        const float* cardStorage = r.storage();

        // Read metadata exactly like the shader
        uint32_t primitiveOffset = meta[0];
        uint32_t primitiveCount = meta[1];
        uint32_t gridOffset = meta[2];
        uint32_t gridWidth = meta[3];
        uint32_t gridHeight = meta[4];
        float cellSize;
        std::memcpy(&cellSize, &meta[5], sizeof(float));
        uint32_t glyphOffset = meta[6];
        uint32_t glyphCount = meta[7];
        float contentMinX, contentMinY, contentMaxX, contentMaxY;
        std::memcpy(&contentMinX, &meta[8], sizeof(float));
        std::memcpy(&contentMinY, &meta[9], sizeof(float));
        std::memcpy(&contentMaxX, &meta[10], sizeof(float));
        std::memcpy(&contentMaxY, &meta[11], sizeof(float));
        uint32_t flagsWord = meta[14];
        uint32_t flags = flagsWord & 0xFFFF;
        uint32_t bgColor = meta[15];

        expect(gridWidth > 0_u) << "shader sim: gridWidth=0";
        expect(gridHeight > 0_u) << "shader sim: gridHeight=0";
        expect(glyphCount > 0_u) << "shader sim: no glyphs";
        expect(cellSize > 0.0_f) << "shader sim: cellSize=" << cellSize;

        float invCellSize = 1.0f / cellSize;

        // Find the first non-space glyph to test against
        uint32_t glyphSize = 5;
        float testX = -1, testY = -1;
        uint32_t testColor = 0;
        for (uint32_t gi = 0; gi < glyphCount; gi++) {
            uint32_t gOff = glyphOffset + gi * glyphSize;
            float gx = cardStorage[gOff + 0];
            float gy = cardStorage[gOff + 1];
            uint32_t whPacked = readU32(cardStorage, gOff + 2);
            // Decode f16 width/height
            auto unpackF16 = [](uint16_t h) -> float {
                uint32_t sign = (h >> 15) & 1;
                uint32_t exp = (h >> 10) & 0x1F;
                uint32_t mant = h & 0x3FF;
                if (exp == 0) return 0.0f;
                float f = (1.0f + mant / 1024.0f) * std::pow(2.0f, (float)exp - 15);
                return sign ? -f : f;
            };
            float gw = unpackF16(whPacked & 0xFFFF);
            float gh = unpackF16(whPacked >> 16);
            uint32_t gColorPacked = readU32(cardStorage, gOff + 4);

            if (gw > 0.1f && gh > 0.1f && gColorPacked != 0) {
                // Test point = center of this glyph
                testX = gx + gw * 0.5f;
                testY = gy + gh * 0.5f;
                testColor = gColorPacked;
                break;
            }
        }
        expect(testX >= 0.0_f) << "shader sim: couldn't find a visible glyph";
        if (testX < 0) return;

        // ---- Shader grid lookup (exactly like 0x0003-ydraw.wgsl) ----
        uint32_t cellX = static_cast<uint32_t>(
            std::clamp((testX - contentMinX) * invCellSize,
                       0.0f, float(gridWidth - 1)));
        uint32_t cellY = static_cast<uint32_t>(
            std::clamp((testY - contentMinY) * invCellSize,
                       0.0f, float(gridHeight - 1)));
        uint32_t cellIndex = cellY * gridWidth + cellX;

        uint32_t packedStart = readU32(cardStorage, gridOffset + cellIndex);
        uint32_t cellEntryCount = readU32(cardStorage, gridOffset + packedStart);

        expect(cellEntryCount > 0_u)
            << "shader sim: cell (" << cellX << "," << cellY
            << ") for glyph at (" << testX << "," << testY
            << ") has 0 entries — glyph NOT IN GRID";

        // ---- Walk entries like shader ----
        bool foundGlyph = false;
        for (uint32_t i = 0; i < cellEntryCount; i++) {
            uint32_t rawIdx = readU32(cardStorage,
                                       gridOffset + packedStart + 1 + i);

            if ((rawIdx & 0x80000000u) == 0) continue; // SDF prim, skip

            // GLYPH_BIT entry
            uint32_t glyphIdx = rawIdx & 0x7FFFFFFFu;
            uint32_t gOff = glyphOffset + glyphIdx * glyphSize;
            float gx = cardStorage[gOff + 0];
            float gy = cardStorage[gOff + 1];
            uint32_t whPacked = readU32(cardStorage, gOff + 2);
            auto unpackF16 = [](uint16_t h) -> float {
                uint32_t sign = (h >> 15) & 1;
                uint32_t exp = (h >> 10) & 0x1F;
                uint32_t mant = h & 0x3FF;
                if (exp == 0) return 0.0f;
                float f = (1.0f + mant / 1024.0f) * std::pow(2.0f, (float)exp - 15);
                return sign ? -f : f;
            };
            float gw = unpackF16(whPacked & 0xFFFF);
            float gh = unpackF16(whPacked >> 16);

            if (testX >= gx && testX < gx + gw &&
                testY >= gy && testY < gy + gh) {
                foundGlyph = true;

                uint32_t glfPacked = readU32(cardStorage, gOff + 3);
                uint32_t gIdx = glfPacked & 0xFFFFu;
                uint32_t gFlags = (glfPacked >> 24) & 0xFFu;
                uint32_t gColorPacked = readU32(cardStorage, gOff + 4);

                expect(gColorPacked != 0u)
                    << "shader sim: glyph color is 0 (invisible)";
                expect(gColorPacked != bgColor)
                    << "shader sim: glyph color == bgColor (invisible on bg)";

                bool isCustomAtlas = (gFlags & 1u) != 0;
                bool shaderHasCustom = (flags & 32u) != 0;

                expect(isCustomAtlas)
                    << "shader sim: glyph GLYPH_FLAG_CUSTOM_ATLAS not set";
                expect(shaderHasCustom)
                    << "shader sim: YDRAW_FLAG_CUSTOM_ATLAS not set in metadata";

                if (isCustomAtlas && shaderHasCustom) {
                    // Read atlas header exactly like shader
                    uint32_t atlasHeaderOff = glyphOffset + glyphCount * 5u;
                    uint32_t atlasXW = readU32(cardStorage, atlasHeaderOff);
                    uint32_t atlasYH = readU32(cardStorage, atlasHeaderOff + 1);
                    uint32_t atlasX = atlasXW & 0xFFFFu;
                    uint32_t atlasY = atlasYH & 0xFFFFu;
                    uint32_t msdfAtlasW = atlasXW >> 16;
                    uint32_t msdfAtlasH = atlasYH >> 16;
                    uint32_t glyphMetaOff = atlasHeaderOff + 4;

                    expect(msdfAtlasW > 0_u)
                        << "shader sim: msdfAtlasW=0 in atlas header";
                    expect(msdfAtlasH > 0_u)
                        << "shader sim: msdfAtlasH=0 in atlas header";

                    // Read glyph UV metadata
                    uint32_t metaOff = glyphMetaOff + gIdx * 10u;
                    float uvMinX = cardStorage[metaOff + 0];
                    float uvMinY = cardStorage[metaOff + 1];
                    float uvMaxX = cardStorage[metaOff + 2];
                    float uvMaxY = cardStorage[metaOff + 3];

                    // UV should be valid for a visible glyph
                    expect(uvMaxX > uvMinX)
                        << "shader sim: glyph UV invalid, maxX=" << uvMaxX
                        << " minX=" << uvMinX << " gIdx=" << gIdx;
                    expect(uvMaxY > uvMinY)
                        << "shader sim: glyph UV invalid, maxY=" << uvMaxY
                        << " minY=" << uvMinY << " gIdx=" << gIdx;
                }
                break;
            }
        }

        expect(foundGlyph)
            << "shader sim: grid cell has entries but glyph NOT found"
            << " at (" << testX << "," << testY << ")";
    };

    //=========================================================================
    // Regression: text-only PDF (0 prims) must NOT fail on 0-byte prim alloc
    //=========================================================================
    "PDF text-only: derived buffer allocated despite 0 prims"_test = [] {
        auto r = runFullPipeline(kSamplePdf);
        expect(r.builder != nullptr) << "pipeline should succeed";
        if (!r.builder) return;

        // Sample PDF is text-only: buffer has 0 prims
        expect(r.buffer->primCount() == 0_u)
            << "sample PDF should have 0 graphical prims, got "
            << r.buffer->primCount();
        expect(r.buffer->gpuBufferSize() == 0_u)
            << "gpuBufferSize should be 0 for text-only PDF";
        expect(!r.buffer->empty())
            << "buffer should NOT be empty (has text spans)";

        // Despite 0 prims, the grid and glyphs MUST be written
        const uint32_t* meta = r.meta();
        uint32_t gridWidth = meta[3];
        uint32_t gridHeight = meta[4];
        uint32_t glyphCount = meta[7];

        expect(gridWidth > 0_u)
            << "REGRESSION: gridWidth must be >0 for text-only PDF, got "
            << gridWidth << " (0-byte prim alloc must not abort pipeline)";
        expect(gridHeight > 0_u)
            << "REGRESSION: gridHeight must be >0 for text-only PDF";
        expect(glyphCount > 0_u)
            << "REGRESSION: glyphCount must be >0 for text-only PDF, got "
            << glyphCount;
    };
};
