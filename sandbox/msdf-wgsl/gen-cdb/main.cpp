/**
 * MSDF-WGSL CDB Generator
 * Generates MSDF atlas on GPU and exports to CDB format
 */

#include <msdf-wgsl.h>
#include <yetty/wgpu-compat.h>
#include <yetty/msdf-glyph-data.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#include <iostream>
#include <cstring>
#include <chrono>
#include <vector>
#include <fstream>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>

extern "C" {
#include <cdb.h>
#include <cdb_make.h>
}

using yetty::MsdfGlyphData;

// WebGPU globals
WGPUInstance instance = nullptr;
WGPUAdapter adapter = nullptr;
WGPUDevice device = nullptr;
WGPUQueue queue = nullptr;

bool adapterReady = false;
bool deviceReady = false;

void onAdapterReady(WGPURequestAdapterStatus status, WGPUAdapter result,
                    WGPUStringView, void*, void*) {
    if (status == WGPURequestAdapterStatus_Success) adapter = result;
    adapterReady = true;
}

void onDeviceReady(WGPURequestDeviceStatus status, WGPUDevice result,
                   WGPUStringView, void*, void*) {
    if (status == WGPURequestDeviceStatus_Success) device = result;
    deviceReady = true;
}

void onUncapturedError(WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void*, void*) {
    const char* typeStr = "Unknown";
    switch(type) {
        case WGPUErrorType_Validation: typeStr = "Validation"; break;
        case WGPUErrorType_OutOfMemory: typeStr = "OutOfMemory"; break;
        case WGPUErrorType_Internal: typeStr = "Internal"; break;
        default: break;
    }
    std::cerr << "[WebGPU ERROR] " << typeStr << ": ";
    if (message.data && message.length > 0) {
        std::cerr << std::string(message.data, message.length);
    }
    std::cerr << std::endl;
}

bool initWebGPU() {
    std::cout << "[WebGPU] Initializing..." << std::endl;

    WGPUInstanceDescriptor instanceDesc = {};
    instance = wgpuCreateInstance(&instanceDesc);

    WGPURequestAdapterOptions opts = {};
    WGPURequestAdapterCallbackInfo adapterCb = {};
    adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCb.callback = onAdapterReady;

    wgpuInstanceRequestAdapter(instance, &opts, adapterCb);
    while (!adapterReady) {}
    if (!adapter) return false;

    // Set up error callback in device descriptor
    WGPUUncapturedErrorCallbackInfo errorInfo = {};
    errorInfo.callback = onUncapturedError;

    // Request higher buffer size limit for large fonts
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
    limits.maxStorageBufferBindingSize = 2ULL * 1024 * 1024 * 1024;  // 2GB for large fonts
    limits.minUniformBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
    limits.minStorageBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxVertexBuffers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBufferSize = 2ULL * 1024 * 1024 * 1024;  // 2GB
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
    deviceDesc.uncapturedErrorCallbackInfo = errorInfo;
    deviceDesc.requiredLimits = &limits;

    WGPURequestDeviceCallbackInfo deviceCb = {};
    deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCb.callback = onDeviceReady;

    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCb);
    while (!deviceReady) {}
    if (!device) return false;

    queue = wgpuDeviceGetQueue(device);
    std::cout << "[WebGPU] Initialized" << std::endl;
    return true;
}

int main(int argc, char* argv[]) {
    std::string fontPath;
    std::string outputPath;
    bool nerdFonts = true;
    int limit = 0;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if ((arg == "--font" || arg == "-f") && i + 1 < argc) {
            fontPath = argv[++i];
        } else if ((arg == "--output" || arg == "-o") && i + 1 < argc) {
            outputPath = argv[++i];
        } else if (arg == "--no-nerd") {
            nerdFonts = false;
        } else if (arg == "--limit" && i + 1 < argc) {
            limit = std::atoi(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "MSDF-WGSL CDB Generator\n\n"
                      << "Usage: " << argv[0] << " --font <file.ttf> --output <file.cdb>\n\n"
                      << "Options:\n"
                      << "  -f, --font FILE    Input TTF font file\n"
                      << "  -o, --output FILE  Output CDB file\n"
                      << "  --no-nerd          Exclude Nerd Font symbols\n"
                      << "  --limit N          Limit number of glyphs\n";
            return 0;
        }
    }

    if (fontPath.empty() || outputPath.empty()) {
        std::cerr << "Error: --font and --output are required\n";
        return 1;
    }

    std::cout << "========================================\n"
              << "MSDF-WGSL CDB Generator\n"
              << "========================================\n"
              << "Font: " << fontPath << "\n"
              << "Output: " << outputPath << "\n"
              << "Nerd Fonts: " << (nerdFonts ? "yes" : "no") << "\n"
              << "========================================\n\n";

    if (!initWebGPU()) {
        std::cerr << "Failed to initialize WebGPU\n";
        return 1;
    }

    // Set shader path relative to current directory
    msdf::setShaderPath("shaders/msdf_gen.wgsl");

    try {
        // Create context with instance for event processing
        msdf::Context ctx(device, instance);

        // Load font
        msdf::FontConfig config;
        config.scale = 2.0f;
        config.range = 4.0f;

        auto font = ctx.loadFont(fontPath, config);
        if (!font) {
            std::cerr << "Failed to load font\n";
            return 1;
        }

        // Get all codepoints from the font
        auto charset = font->getAllCodepoints();
        if (limit > 0 && static_cast<size_t>(limit) < charset.size()) {
            charset.resize(limit);
        }
        (void)nerdFonts;  // No longer needed - we enumerate all glyphs

        std::cout << "[ATLAS] Generating " << charset.size() << " glyphs...\n";
        auto startTime = std::chrono::steady_clock::now();

        int generated = ctx.generateGlyphs(*font, charset);

        auto endTime = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(endTime - startTime).count();

        std::cout << "[ATLAS] Generated " << generated << " glyphs in "
                  << elapsed << "s (" << static_cast<int>(generated / elapsed) << " glyphs/sec)\n";
        std::cout << "[ATLAS] Size: " << font->getAtlas()->getWidth() << "x"
                  << font->getAtlas()->getHeight() << "\n";

        // Read atlas back
        std::cout << "[CDB] Reading atlas from GPU...\n";
        auto atlasData = ctx.readAtlasToRGBA8(*font->getAtlas());
        if (atlasData.empty()) {
            std::cerr << "Failed to read atlas\n";
            return 1;
        }

        // Write CDB
        std::cout << "[CDB] Writing to: " << outputPath << "\n";

        std::string tmpPath = outputPath + ".tmp";
        int fd = open(tmpPath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            std::cerr << "Failed to create output file\n";
            return 1;
        }

        struct cdb_make cdbm;
        cdb_make_start(&cdbm, fd);

        const auto& glyphs = font->getGlyphs();
        int atlasW = font->getAtlas()->getWidth();
        int written = 0;

        for (const auto& glyph : glyphs) {
            // Build CDB key (4-byte little-endian codepoint)
            char key[4];
            key[0] = glyph.codepoint & 0xFF;
            key[1] = (glyph.codepoint >> 8) & 0xFF;
            key[2] = (glyph.codepoint >> 16) & 0xFF;
            key[3] = (glyph.codepoint >> 24) & 0xFF;

            // Build value: header + pixel data
            MsdfGlyphData header;
            header.width = glyph.atlasW;
            header.height = glyph.atlasH;
            header.bearingX = glyph.bearingX;
            header.bearingY = glyph.bearingY;
            header.advance = glyph.advance;

            size_t pixelBytes = glyph.atlasW * glyph.atlasH * 4;
            std::vector<char> value(sizeof(MsdfGlyphData) + pixelBytes);
            memcpy(value.data(), &header, sizeof(MsdfGlyphData));

            // Copy pixel data from atlas
            for (int y = 0; y < glyph.atlasH; y++) {
                const uint8_t* src = atlasData.data() + ((glyph.atlasY + y) * atlasW + glyph.atlasX) * 4;
                char* dst = value.data() + sizeof(MsdfGlyphData) + y * glyph.atlasW * 4;
                memcpy(dst, src, glyph.atlasW * 4);
            }

            cdb_make_add(&cdbm, key, 4, value.data(), value.size());
            written++;
        }

        cdb_make_finish(&cdbm);
        close(fd);

        // Rename temp to final
        std::filesystem::rename(tmpPath, outputPath);

        auto fileSize = std::filesystem::file_size(outputPath);
        std::cout << "[CDB] Written " << written << " glyphs\n";
        std::cout << "[CDB] File size: " << (fileSize / 1024 / 1024) << " MB\n";

        std::cout << "\n========================================\n"
                  << "Done! Output: " << outputPath << "\n"
                  << "========================================\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
