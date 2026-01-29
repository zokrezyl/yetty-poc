#include <yetty/msdf-cdb-provider.h>
#include <yetty/msdf-glyph-data.h>
#include <ytrace/ytrace.hpp>

#include <filesystem>
#include <cmath>
#include <cstring>
#include <array>

// CPU provider — msdfgen library
#include "msdf-gen/generator.h"

// GPU provider — msdf-wgsl library
#include <msdf-wgsl.h>

extern "C" {
#include <cdb.h>
#include <cdb_make.h>
}

#include <fcntl.h>
#include <unistd.h>

namespace yetty {

static const std::array<std::string, 4> kStyleSuffixes = {
    "Regular", "Bold", "Oblique", "BoldOblique"
};

// ---------------------------------------------------------------------------
// CpuMsdfCdbProvider
// ---------------------------------------------------------------------------

Result<void> CpuMsdfCdbProvider::generate(const MsdfCdbConfig& config) {
    std::filesystem::create_directories(config.outputDir);

    for (size_t i = 0; i < config.ttfPaths.size() && i < kStyleSuffixes.size(); ++i) {
        const auto& ttf = config.ttfPaths[i];
        if (ttf.empty() || !std::filesystem::exists(ttf)) continue;

        std::string cdbPath = config.outputDir + "/" + config.fontName
                            + "-" + kStyleSuffixes[i] + ".cdb";
        if (std::filesystem::exists(cdbPath)) {
            yinfo("CpuMsdfCdbProvider: CDB already exists: {}", cdbPath);
            continue;
        }

        yinfo("CpuMsdfCdbProvider: generating {} from {}", cdbPath, ttf);

        msdfgen::GeneratorConfig genCfg;
        genCfg.fontPath   = ttf;
        genCfg.outputDir  = config.outputDir;
        genCfg.fontSize   = config.fontSize;
        genCfg.pixelRange = config.pixelRange;

        auto result = msdfgen::generate(genCfg, [](size_t cur, size_t total, const std::string&) {
            if (cur % 1000 == 0 || cur == total) {
                yinfo("  progress: {}/{}", cur, total);
            }
        });

        if (!result.success) {
            return Err<void>("CPU MSDF generation failed for " + ttf + ": " + result.error);
        }

        yinfo("CpuMsdfCdbProvider: generated {} glyphs for {}",
              result.glyphsGenerated, kStyleSuffixes[i]);
    }

    return Ok();
}

// ---------------------------------------------------------------------------
// GpuMsdfCdbProvider
// ---------------------------------------------------------------------------

GpuMsdfCdbProvider::GpuMsdfCdbProvider(WGPUInstance instance)
    : _instance(instance) {}

Result<void> GpuMsdfCdbProvider::initDevice() {
    if (_device) return Ok();

    // Request adapter
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

    bool adapterReady = false;
    WGPURequestAdapterCallbackInfo adapterCb = {};
    adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                            WGPUStringView, void* userdata1, void*) {
        if (status == WGPURequestAdapterStatus_Success) {
            *static_cast<WGPUAdapter*>(userdata1) = adapter;
        }
    };
    adapterCb.userdata1 = &_adapter;
    adapterCb.userdata2 = &adapterReady;

    wgpuInstanceRequestAdapter(_instance, &adapterOpts, adapterCb);
    // Spin until callback fires (AllowSpontaneous means it fires immediately for Dawn)
    if (!_adapter) {
        return Err<void>("GpuMsdfCdbProvider: failed to get WebGPU adapter");
    }

    // Request device with high buffer size limits (needed for large font atlases)
    WGPULimits limits = {};
    limits.maxTextureDimension1D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureDimension2D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureDimension3D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureArrayLayers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindGroups = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindGroupsPlusVertexBuffers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindingsPerBindGroup = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxDynamicUniformBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxDynamicStorageBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxSampledTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxSamplersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxStorageBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxStorageTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxUniformBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxUniformBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED;
    limits.maxStorageBufferBindingSize = 2ULL * 1024 * 1024 * 1024;
    limits.minUniformBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
    limits.minStorageBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxVertexBuffers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBufferSize = 2ULL * 1024 * 1024 * 1024;
    limits.maxVertexAttributes = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxVertexBufferArrayStride = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxInterStageShaderVariables = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxColorAttachments = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxColorAttachmentBytesPerSample = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupStorageSize = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeInvocationsPerWorkgroup = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupSizeX = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupSizeY = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupSizeZ = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupsPerDimension = WGPU_LIMIT_U32_UNDEFINED;

    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.requiredLimits = &limits;

    WGPURequestDeviceCallbackInfo deviceCb = {};
    deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCb.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                           WGPUStringView, void* userdata1, void*) {
        if (status == WGPURequestDeviceStatus_Success) {
            *static_cast<WGPUDevice*>(userdata1) = device;
        }
    };
    deviceCb.userdata1 = &_device;

    wgpuAdapterRequestDevice(_adapter, &deviceDesc, deviceCb);
    if (!_device) {
        return Err<void>("GpuMsdfCdbProvider: failed to create WebGPU device with high limits");
    }

    yinfo("GpuMsdfCdbProvider: created dedicated device with 2GB buffer limit");
    return Ok();
}

void GpuMsdfCdbProvider::releaseDevice() {
    if (_device) {
        wgpuDeviceRelease(_device);
        _device = nullptr;
    }
    if (_adapter) {
        wgpuAdapterRelease(_adapter);
        _adapter = nullptr;
    }
}

Result<void> GpuMsdfCdbProvider::generate(const MsdfCdbConfig& config) {
    std::filesystem::create_directories(config.outputDir);

    // Create dedicated device with high buffer limits
    if (auto res = initDevice(); !res) {
        return res;
    }

    // Set shader path
    std::string shaderPath = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/msdf_gen.wgsl";
    msdf::setShaderPath(shaderPath);

    msdf::Context ctx(_device, _instance);

    for (size_t i = 0; i < config.ttfPaths.size() && i < kStyleSuffixes.size(); ++i) {
        const auto& ttf = config.ttfPaths[i];
        if (ttf.empty() || !std::filesystem::exists(ttf)) continue;

        std::string cdbPath = config.outputDir + "/" + config.fontName
                            + "-" + kStyleSuffixes[i] + ".cdb";
        if (std::filesystem::exists(cdbPath)) {
            yinfo("GpuMsdfCdbProvider: CDB already exists: {}", cdbPath);
            continue;
        }

        yinfo("GpuMsdfCdbProvider: generating {} from {}", cdbPath, ttf);

        // Compute correct scale: fontSize * 64 / unitsPerEm
        // so atlas pixel dimensions match display pixel dimensions
        FT_Library ftLib;
        FT_Face ftFace;
        FT_Init_FreeType(&ftLib);
        FT_New_Face(ftLib, ttf.c_str(), 0, &ftFace);
        float unitsPerEm = static_cast<float>(ftFace->units_per_EM);
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLib);

        msdf::FontConfig fontCfg;
        fontCfg.scale = config.fontSize * 64.0f / unitsPerEm;
        fontCfg.range = config.pixelRange;

        yinfo("GpuMsdfCdbProvider: unitsPerEm={} scale={}", unitsPerEm, fontCfg.scale);

        auto font = ctx.loadFont(ttf, fontCfg);
        if (!font) {
            return Err<void>("Failed to load font: " + ttf);
        }

        auto charset = font->getAllCodepoints();
        yinfo("GpuMsdfCdbProvider: generating {} glyphs for {}", charset.size(), kStyleSuffixes[i]);

        int generated = ctx.generateGlyphs(*font, charset);
        yinfo("GpuMsdfCdbProvider: generated {} glyphs", generated);

        // Read atlas back to CPU
        auto atlasData = ctx.readAtlasToRGBA8(*font->getAtlas());
        if (atlasData.empty()) {
            return Err<void>("Failed to read atlas from GPU for " + ttf);
        }

        // Write CDB
        std::string tmpPath = cdbPath + ".tmp";
        int fd = open(tmpPath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            return Err<void>("Failed to create CDB file: " + tmpPath);
        }

        struct cdb_make cdbm;
        cdb_make_start(&cdbm, fd);

        const auto& glyphs = font->getGlyphs();
        int atlasW = font->getAtlas()->getWidth();
        int written = 0;

        // Convert from FreeType font design units to scaled pixel values
        float fontScale = config.fontSize / unitsPerEm;
        int padding = static_cast<int>(std::ceil(config.pixelRange));

        for (const auto& glyph : glyphs) {
            char key[4];
            key[0] = glyph.codepoint & 0xFF;
            key[1] = (glyph.codepoint >> 8) & 0xFF;
            key[2] = (glyph.codepoint >> 16) & 0xFF;
            key[3] = (glyph.codepoint >> 24) & 0xFF;

            MsdfGlyphData header{};
            header.codepoint = glyph.codepoint;
            header.width     = glyph.atlasW;
            header.height    = glyph.atlasH;
            header.bearingX  = static_cast<float>(glyph.bearingX) * fontScale - padding;
            header.bearingY  = static_cast<float>(glyph.bearingY) * fontScale + padding;
            // With scale = fontSize*64/unitsPerEm, atlas dimensions match display dimensions
            header.sizeX     = static_cast<float>(glyph.atlasW);
            header.sizeY     = static_cast<float>(glyph.atlasH);
            header.advance   = glyph.advance * fontScale;

            size_t pixelBytes = glyph.atlasW * glyph.atlasH * 4;
            std::vector<char> value(sizeof(MsdfGlyphData) + pixelBytes);
            std::memcpy(value.data(), &header, sizeof(MsdfGlyphData));

            for (int y = 0; y < glyph.atlasH; y++) {
                const uint8_t* src = atlasData.data()
                    + ((glyph.atlasY + y) * atlasW + glyph.atlasX) * 4;
                char* dst = value.data() + sizeof(MsdfGlyphData) + y * glyph.atlasW * 4;
                std::memcpy(dst, src, glyph.atlasW * 4);
            }

            cdb_make_add(&cdbm, key, 4, value.data(), value.size());
            written++;
        }

        cdb_make_finish(&cdbm);
        close(fd);

        std::filesystem::rename(tmpPath, cdbPath);
        auto fileSize = std::filesystem::file_size(cdbPath);
        yinfo("GpuMsdfCdbProvider: wrote {} glyphs to {} ({} MB)",
              written, cdbPath, fileSize / 1024 / 1024);
    }

    // Release dedicated device after generation
    releaseDevice();

    return Ok();
}

} // namespace yetty
