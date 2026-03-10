#include <yetty/msdf-cdb-provider.h>
#include <yetty/cdb-wrapper.h>
#include <yetty/msdf-glyph-data.h>
#include <ytrace/ytrace.hpp>

#include <filesystem>
#include <cmath>
#include <cstring>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

#ifndef YETTY_SHADERS_DIR
#define YETTY_SHADERS_DIR CMAKE_SOURCE_DIR "/src/yetty/shaders"
#endif

// CPU provider — msdfgen library (internal, not CDB-dependent)
#include "msdf-gen/generator.h"

// GPU provider — msdf-wgsl library
#include <msdf-wgsl.h>

namespace yetty {

// ---------------------------------------------------------------------------
// CpuMsdfCdbProvider - uses msdfgen library (which handles CDB writing internally)
// ---------------------------------------------------------------------------

Result<void> CpuMsdfCdbProvider::generate(const MsdfCdbConfig& config) {
    if (std::filesystem::exists(config.cdbPath)) {
        yinfo("CpuMsdfCdbProvider: CDB already exists: {}", config.cdbPath);
        return Ok();
    }

    auto outputDir = std::filesystem::path(config.cdbPath).parent_path().string();
    std::filesystem::create_directories(outputDir);

    yinfo("CpuMsdfCdbProvider: generating {} from {}", config.cdbPath, config.ttfPath);

    msdfgen::GeneratorConfig genCfg;
    genCfg.fontPath   = config.ttfPath;
    genCfg.outputDir  = outputDir;
    genCfg.fontSize   = config.fontSize;
    genCfg.pixelRange = config.pixelRange;

    auto result = msdfgen::generate(genCfg, [](size_t cur, size_t total, const std::string&) {
        if (cur % 1000 == 0 || cur == total) {
            yinfo("  progress: {}/{}", cur, total);
        }
    });

    if (!result.success) {
        return Err<void>("CPU MSDF generation failed for " + config.ttfPath + ": " + result.error);
    }

    // msdfgen names output from TTF stem; rename to requested cdbPath if different
    auto msdfgenOutput = outputDir + "/"
        + std::filesystem::path(config.ttfPath).stem().string() + ".cdb";
    if (msdfgenOutput != config.cdbPath && std::filesystem::exists(msdfgenOutput)) {
        std::filesystem::rename(msdfgenOutput, config.cdbPath);
    }

    yinfo("CpuMsdfCdbProvider: generated {} glyphs", result.glyphsGenerated);
    return Ok();
}

// ---------------------------------------------------------------------------
// GpuMsdfCdbProvider - uses msdf-wgsl compute shaders + CdbWriter
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
    if (std::filesystem::exists(config.cdbPath)) {
        yinfo("GpuMsdfCdbProvider: CDB already exists: {}", config.cdbPath);
        return Ok();
    }

    auto outputDir = std::filesystem::path(config.cdbPath).parent_path().string();
    std::filesystem::create_directories(outputDir);

    // Create dedicated device with high buffer limits
    if (auto res = initDevice(); !res) {
        return res;
    }

    // Set shader path
    std::string shaderPath = std::string(YETTY_SHADERS_DIR) + "/msdf_gen.wgsl";
    msdf::setShaderPath(shaderPath);

    msdf::Context ctx(_device, _instance);

    yinfo("GpuMsdfCdbProvider: generating {} from {}", config.cdbPath, config.ttfPath);

    // Compute correct scale: fontSize * 64 / unitsPerEm
    // so atlas pixel dimensions match display pixel dimensions
    FT_Library ftLib;
    FT_Face ftFace;
    FT_Init_FreeType(&ftLib);
    FT_New_Face(ftLib, config.ttfPath.c_str(), 0, &ftFace);
    float unitsPerEm = static_cast<float>(ftFace->units_per_EM);
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);

    msdf::FontConfig fontCfg;
    fontCfg.scale = config.fontSize * 64.0f / unitsPerEm;
    fontCfg.range = config.pixelRange;

    yinfo("GpuMsdfCdbProvider: unitsPerEm={} scale={}", unitsPerEm, fontCfg.scale);

    auto font = ctx.loadFont(config.ttfPath, fontCfg);
    if (!font) {
        releaseDevice();
        return Err<void>("Failed to load font: " + config.ttfPath);
    }

    auto charset = font->getAllCodepoints();
    yinfo("GpuMsdfCdbProvider: generating {} glyphs", charset.size());

    int generated = ctx.generateGlyphs(*font, charset);
    yinfo("GpuMsdfCdbProvider: generated {} glyphs", generated);

    // Read atlas back to CPU
    auto atlasData = ctx.readAtlasToRGBA8(*font->getAtlas());
    if (atlasData.empty()) {
        releaseDevice();
        return Err<void>("Failed to read atlas from GPU for " + config.ttfPath);
    }

    // Write CDB using the wrapper (works on all platforms)
    std::string tmpPath = config.cdbPath + ".tmp";
    auto writer = CdbWriter::create(tmpPath);
    if (!writer) {
        releaseDevice();
        return Err<void>("Failed to create CDB file: " + tmpPath);
    }

    const auto& glyphs = font->getGlyphs();
    int atlasW = font->getAtlas()->getWidth();
    int written = 0;

    // Convert from FreeType font design units to scaled pixel values
    float fontScale = config.fontSize / unitsPerEm;
    int padding = static_cast<int>(std::ceil(config.pixelRange));

    for (const auto& glyph : glyphs) {
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

        if (!writer->add(glyph.codepoint, value.data(), value.size())) {
            releaseDevice();
            return Err<void>("Failed to add glyph to CDB: " + std::to_string(glyph.codepoint));
        }
        written++;
    }

    if (!writer->finish()) {
        releaseDevice();
        return Err<void>("Failed to finalize CDB file: " + tmpPath);
    }

    std::filesystem::rename(tmpPath, config.cdbPath);
    auto fileSize = std::filesystem::file_size(config.cdbPath);
    yinfo("GpuMsdfCdbProvider: wrote {} glyphs to {} ({} MB)",
          written, config.cdbPath, fileSize / 1024 / 1024);

    // Release dedicated device after generation
    releaseDevice();

    return Ok();
}

} // namespace yetty
