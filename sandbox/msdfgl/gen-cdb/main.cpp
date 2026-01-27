/**
 * MSDFGL WebGPU CDB Generator
 *
 * Generates MSDF atlas on GPU and exports to CDB format
 * Compatible with yetty's msdf-font-cache
 */

#include <msdfgl.h>
#include <yetty/wgpu-compat.h>
#include <yetty/msdf-glyph-data.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#include <iostream>
#include <cstring>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <atomic>

extern "C" {
#include <cdb.h>
#include <cdb_make.h>
}

// Window dimensions (hidden window for compute)
constexpr int WINDOW_WIDTH = 64;
constexpr int WINDOW_HEIGHT = 64;

// WebGPU globals
WGPUInstance instance = nullptr;
WGPUSurface surface = nullptr;
WGPUAdapter adapter = nullptr;
WGPUDevice device = nullptr;
WGPUQueue queue = nullptr;

// MSDFGL context and font
std::unique_ptr<msdfgl::Context> msdfContext;
std::unique_ptr<msdfgl::Font> font;

bool adapterReady = false;
bool deviceReady = false;

// Glyph info for CDB export
struct GlyphInfo {
    uint32_t codepoint;
    float atlasX, atlasY;      // Position in atlas
    float atlasW, atlasH;      // Size in atlas
    float bearingX, bearingY;
    float glyphWidth, glyphHeight;
    float advance;
};

std::vector<GlyphInfo> generatedGlyphs;

// Get default charset (same as CPU generator)
std::vector<uint32_t> getDefaultCharset(bool includeNerdFonts) {
    std::vector<uint32_t> charset;

    // Basic Latin (ASCII printable)
    for (uint32_t c = 0x20; c <= 0x7E; ++c) charset.push_back(c);

    // Latin-1 Supplement
    for (uint32_t c = 0xA0; c <= 0xFF; ++c) charset.push_back(c);

    // Latin Extended-A
    for (uint32_t c = 0x100; c <= 0x17F; ++c) charset.push_back(c);

    // Latin Extended-B (partial)
    for (uint32_t c = 0x180; c <= 0x24F; ++c) charset.push_back(c);

    // Greek and Coptic
    for (uint32_t c = 0x370; c <= 0x3FF; ++c) charset.push_back(c);

    // Cyrillic
    for (uint32_t c = 0x400; c <= 0x4FF; ++c) charset.push_back(c);

    // General Punctuation
    for (uint32_t c = 0x2000; c <= 0x206F; ++c) charset.push_back(c);

    // Currency Symbols
    for (uint32_t c = 0x20A0; c <= 0x20CF; ++c) charset.push_back(c);

    // Arrows
    for (uint32_t c = 0x2190; c <= 0x21FF; ++c) charset.push_back(c);

    // Mathematical Operators
    for (uint32_t c = 0x2200; c <= 0x22FF; ++c) charset.push_back(c);

    // Box Drawing
    for (uint32_t c = 0x2500; c <= 0x257F; ++c) charset.push_back(c);

    // Block Elements
    for (uint32_t c = 0x2580; c <= 0x259F; ++c) charset.push_back(c);

    // Geometric Shapes
    for (uint32_t c = 0x25A0; c <= 0x25FF; ++c) charset.push_back(c);

    if (includeNerdFonts) {
        // Powerline symbols
        for (uint32_t c = 0xE0A0; c <= 0xE0D7; ++c) charset.push_back(c);

        // Seti-UI + Custom
        for (uint32_t c = 0xE5FA; c <= 0xE6AC; ++c) charset.push_back(c);

        // Devicons
        for (uint32_t c = 0xE700; c <= 0xE7C5; ++c) charset.push_back(c);

        // Font Awesome Extension
        for (uint32_t c = 0xE200; c <= 0xE2A9; ++c) charset.push_back(c);

        // Material Design Icons
        for (uint32_t c = 0xF0001; c <= 0xF1AF0; ++c) charset.push_back(c);

        // Weather Icons
        for (uint32_t c = 0xE300; c <= 0xE3E3; ++c) charset.push_back(c);

        // Octicons
        for (uint32_t c = 0xF400; c <= 0xF532; ++c) charset.push_back(c);

        // Codicons
        for (uint32_t c = 0xEA60; c <= 0xEBEB; ++c) charset.push_back(c);
    }

    return charset;
}

void onAdapterReady(WGPURequestAdapterStatus status, WGPUAdapter result,
                    WGPUStringView message, void* userdata1, void* userdata2) {
    if (status == WGPURequestAdapterStatus_Success) {
        adapter = result;
    }
    adapterReady = true;
}

void onDeviceReady(WGPURequestDeviceStatus status, WGPUDevice result,
                   WGPUStringView message, void* userdata1, void* userdata2) {
    if (status == WGPURequestDeviceStatus_Success) {
        device = result;
    }
    deviceReady = true;
}

bool initWebGPU(GLFWwindow* window) {
    std::cout << "[WebGPU] Initializing..." << std::endl;

    WGPUInstanceDescriptor instanceDesc = {};
    instance = wgpuCreateInstance(&instanceDesc);
    if (!instance) {
        std::cerr << "[WebGPU] Failed to create instance" << std::endl;
        return false;
    }

    surface = glfwCreateWindowWGPUSurface(instance, window);
    if (!surface) {
        std::cerr << "[WebGPU] Failed to create surface" << std::endl;
        return false;
    }

    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = surface;
    adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

    WGPURequestAdapterCallbackInfo adapterCallback = {};
    adapterCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    adapterCallback.callback = onAdapterReady;

    wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCallback);

    while (!adapterReady) {
        wgpuInstanceProcessEvents(instance);
    }

    if (!adapter) {
        std::cerr << "[WebGPU] Failed to get adapter" << std::endl;
        return false;
    }

    // Get adapter limits as a base
    WGPULimits adapterLimits = {};
    wgpuAdapterGetLimits(adapter, &adapterLimits);

    // Increase maxBufferSize for large atlas readback (512MB needed for 8192x4096 RGBA32Float)
    // The adapter supports up to 4GB, request 1GB
    if (adapterLimits.maxBufferSize < 1024ULL * 1024ULL * 1024ULL) {
        adapterLimits.maxBufferSize = 1024ULL * 1024ULL * 1024ULL;  // 1GB
    }

    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.label = WGPU_STR("msdfgl-gen-cdb device");
    deviceDesc.requiredLimits = &adapterLimits;

    // Set up uncaptured error callback to catch GPU errors
    deviceDesc.uncapturedErrorCallbackInfo.callback = [](WGPUDevice const* dev, WGPUErrorType type, WGPUStringView message, void* userdata1, void* userdata2) {
        std::cerr << "[WebGPU ERROR] Type=" << static_cast<int>(type);
        if (message.data && message.length > 0) {
            std::cerr << " Message: " << std::string(message.data, message.length);
        }
        std::cerr << std::endl;
    };

    WGPURequestDeviceCallbackInfo deviceCallback = {};
    deviceCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    deviceCallback.callback = onDeviceReady;

    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCallback);

    while (!deviceReady) {
        wgpuInstanceProcessEvents(instance);
    }

    if (!device) {
        std::cerr << "[WebGPU] Failed to get device" << std::endl;
        return false;
    }

    queue = wgpuDeviceGetQueue(device);

    std::cout << "[WebGPU] Initialized successfully" << std::endl;
    return true;
}

bool initMSDFGL(const char* fontPath) {
    std::cout << "[MSDFGL] Creating context..." << std::endl;

    msdfgl::ContextConfig config;
    config.dpiX = 96.0f;
    config.dpiY = 96.0f;

    try {
        msdfContext = std::make_unique<msdfgl::Context>(device, config);
    } catch (const std::exception& e) {
        std::cerr << "[MSDFGL] Failed to create context: " << e.what() << std::endl;
        return false;
    }

    std::cout << "[MSDFGL] Loading font: " << fontPath << std::endl;

    msdfgl::FontConfig fontConfig;
    fontConfig.range = 4.0f;
    fontConfig.scale = 1.0f;  // Match CPU generator's scale

    font = msdfContext->loadFont(fontPath, fontConfig);
    if (!font) {
        std::cerr << "[MSDFGL] Failed to load font" << std::endl;
        return false;
    }

    std::cout << "[MSDFGL] Font loaded successfully" << std::endl;
    return true;
}

bool generateAtlas(const std::vector<uint32_t>& charset) {
    std::cout << "\n[ATLAS] Generating " << charset.size() << " glyphs..." << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();

    generatedGlyphs.clear();
    generatedGlyphs.reserve(charset.size());

    size_t successCount = 0;
    size_t lastProgress = 0;

    for (size_t i = 0; i < charset.size(); ++i) {
        uint32_t codepoint = charset[i];
        int result = font->generateGlyph(static_cast<int32_t>(codepoint));

        if (result > 0) {
            // Get glyph info from font's map
            msdfgl::MapItem* item = font->getGlyph(static_cast<int32_t>(codepoint));
            if (item && item->index >= 0) {
                GlyphInfo info;
                info.codepoint = codepoint;
                info.advance = item->advance[0];

                // We'll get atlas position from the index buffer later
                // For now, store index
                info.atlasX = static_cast<float>(item->index);
                info.atlasY = 0;
                info.atlasW = 0;
                info.atlasH = 0;

                generatedGlyphs.push_back(info);
                successCount++;
            }
        }

        // Progress report
        size_t progress = (i * 100) / charset.size();
        if (progress >= lastProgress + 10) {
            lastProgress = progress;
            auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
            double elapsedSec = std::chrono::duration<double>(elapsed).count();
            std::cout << "[ATLAS] Progress: " << progress << "% ("
                      << std::fixed << std::setprecision(0)
                      << ((i + 1) / elapsedSec) << " glyphs/sec)" << std::endl;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    double totalTime = std::chrono::duration<double>(endTime - startTime).count();

    auto atlas = font->getAtlas();
    std::cout << "\n[ATLAS] Generation complete!" << std::endl;
    std::cout << "[ATLAS] Time: " << std::fixed << std::setprecision(2) << totalTime << "s" << std::endl;
    std::cout << "[ATLAS] Success: " << successCount << "/" << charset.size() << std::endl;
    std::cout << "[ATLAS] Rate: " << std::fixed << std::setprecision(0)
              << (charset.size() / totalTime) << " glyphs/sec" << std::endl;
    if (atlas) {
        std::cout << "[ATLAS] Size: " << atlas->getTextureWidth() << "x"
                  << atlas->getTextureHeight() << std::endl;
    }

    return true;
}

// Read atlas texture back from GPU
std::vector<uint8_t> readAtlasTexture(int& outWidth, int& outHeight) {
    auto atlas = font->getAtlas();
    if (!atlas) {
        std::cerr << "[CDB] No atlas available" << std::endl;
        return {};
    }

    outWidth = atlas->getTextureWidth();
    outHeight = atlas->getTextureHeight();

    WGPUTexture texture = atlas->getAtlasTexture();
    if (!texture) {
        std::cerr << "[CDB] No atlas texture" << std::endl;
        return {};
    }

    std::cout << "[CDB] Reading atlas " << outWidth << "x" << outHeight << " from GPU..." << std::endl;

    // Create staging buffer for readback
    // Atlas is RGBA32Float (16 bytes per pixel)
    size_t bytesPerPixel = 16;  // RGBA32Float
    size_t bytesPerRow = outWidth * bytesPerPixel;
    // Dawn requires 256-byte alignment for buffer copy
    size_t alignedBytesPerRow = ((bytesPerRow + 255) / 256) * 256;
    size_t bufferSize = alignedBytesPerRow * outHeight;

    std::cout << "[CDB] Creating staging buffer: " << (bufferSize / 1024 / 1024) << " MB" << std::endl;

    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("Atlas readback buffer");
    bufDesc.size = bufferSize;
    bufDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
    bufDesc.mappedAtCreation = false;

    WGPUBuffer stagingBuffer = wgpuDeviceCreateBuffer(device, &bufDesc);
    if (!stagingBuffer) {
        std::cerr << "[CDB] Failed to create staging buffer" << std::endl;
        return {};
    }

    // Copy texture to buffer
    WGPUCommandEncoderDescriptor encDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encDesc);

    WGPUTexelCopyTextureInfo srcCopy = {};
    srcCopy.texture = texture;
    srcCopy.mipLevel = 0;
    srcCopy.origin = {0, 0, 0};
    srcCopy.aspect = WGPUTextureAspect_All;

    WGPUTexelCopyBufferInfo dstCopy = {};
    dstCopy.buffer = stagingBuffer;
    dstCopy.layout.offset = 0;
    dstCopy.layout.bytesPerRow = static_cast<uint32_t>(alignedBytesPerRow);
    dstCopy.layout.rowsPerImage = outHeight;

    WGPUExtent3D copySize = {static_cast<uint32_t>(outWidth),
                              static_cast<uint32_t>(outHeight), 1};

    wgpuCommandEncoderCopyTextureToBuffer(encoder, &srcCopy, &dstCopy, &copySize);

    WGPUCommandBufferDescriptor cmdBufDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuffer);

    wgpuCommandBufferRelease(cmdBuffer);
    wgpuCommandEncoderRelease(encoder);

    // Wait for GPU work to complete before mapping
    std::cout << "[CDB] Waiting for GPU work to complete..." << std::endl;
    std::atomic<bool> workDone{false};

    WGPUQueueWorkDoneCallbackInfo workDoneCallback = {};
    workDoneCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    workDoneCallback.callback = [](WGPUQueueWorkDoneStatus status, WGPUStringView message, void* userdata1, void* userdata2) {
        std::cout << "[CDB] Queue work done, status=" << static_cast<int>(status) << std::endl;
        if (message.data && message.length > 0) {
            std::cout << "[CDB] Work done message: " << std::string(message.data, message.length) << std::endl;
        }
        *static_cast<std::atomic<bool>*>(userdata1) = true;
    };
    workDoneCallback.userdata1 = &workDone;

    wgpuQueueOnSubmittedWorkDone(queue, workDoneCallback);

    while (!workDone) {
        wgpuInstanceProcessEvents(instance);
    }

    // Map buffer for reading
    std::cout << "[CDB] Mapping buffer..." << std::endl;
    std::atomic<bool> mapDone{false};
    std::atomic<WGPUMapAsyncStatus> mapStatus{WGPUMapAsyncStatus_Error};

    WGPUBufferMapCallbackInfo mapCallback = {};
    mapCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    mapCallback.callback = [](WGPUMapAsyncStatus status, WGPUStringView message, void* userdata1, void* userdata2) {
        std::cout << "[CDB] Map callback, status=" << static_cast<int>(status) << std::endl;
        if (message.data && message.length > 0) {
            std::cout << "[CDB] Map error: " << std::string(message.data, message.length) << std::endl;
        }
        auto* done = static_cast<std::atomic<bool>*>(userdata1);
        auto* statusOut = static_cast<std::atomic<WGPUMapAsyncStatus>*>(userdata2);
        *statusOut = status;
        *done = true;
    };
    mapCallback.userdata1 = &mapDone;
    mapCallback.userdata2 = &mapStatus;

    wgpuBufferMapAsync(stagingBuffer, WGPUMapMode_Read, 0, bufferSize, mapCallback);

    while (!mapDone) {
        wgpuInstanceProcessEvents(instance);
    }

    if (mapStatus != WGPUMapAsyncStatus_Success) {
        std::cerr << "[CDB] Failed to map buffer, status=" << static_cast<int>(mapStatus.load()) << std::endl;
        wgpuBufferRelease(stagingBuffer);
        return {};
    }

    // Read the data
    const float* mappedData = static_cast<const float*>(
        wgpuBufferGetConstMappedRange(stagingBuffer, 0, bufferSize));

    if (!mappedData) {
        std::cerr << "[CDB] Failed to get mapped range" << std::endl;
        wgpuBufferUnmap(stagingBuffer);
        wgpuBufferRelease(stagingBuffer);
        return {};
    }

    // Convert from RGBA32Float to RGBA8
    std::vector<uint8_t> result(outWidth * outHeight * 4);

    for (int y = 0; y < outHeight; ++y) {
        const float* srcRow = mappedData + (y * alignedBytesPerRow / sizeof(float));
        uint8_t* dstRow = result.data() + (y * outWidth * 4);

        for (int x = 0; x < outWidth; ++x) {
            const float* pixel = srcRow + x * 4;
            uint8_t* dst = dstRow + x * 4;

            // Clamp and convert to 8-bit
            dst[0] = static_cast<uint8_t>(std::clamp(pixel[0] * 255.0f, 0.0f, 255.0f));
            dst[1] = static_cast<uint8_t>(std::clamp(pixel[1] * 255.0f, 0.0f, 255.0f));
            dst[2] = static_cast<uint8_t>(std::clamp(pixel[2] * 255.0f, 0.0f, 255.0f));
            dst[3] = 255;  // Alpha
        }
    }

    wgpuBufferUnmap(stagingBuffer);
    wgpuBufferRelease(stagingBuffer);

    std::cout << "[CDB] Atlas read successfully (" << result.size() << " bytes)" << std::endl;
    return result;
}

bool writeCdb(const std::string& outputPath, const std::vector<uint8_t>& atlasData,
              int atlasWidth, int atlasHeight) {
    std::cout << "\n[CDB] Writing to: " << outputPath << std::endl;

    auto atlas = font->getAtlas();
    if (!atlas) {
        std::cerr << "[CDB] No atlas" << std::endl;
        return false;
    }

    // Read index buffer to get glyph positions
    // The index buffer contains IndexEntry structs for each glyph
    WGPUBuffer indexBuffer = atlas->getIndexBuffer();
    if (!indexBuffer) {
        std::cerr << "[CDB] No index buffer" << std::endl;
        return false;
    }

    size_t glyphCount = atlas->getGlyphCount();
    size_t indexBufferSize = glyphCount * sizeof(msdfgl::IndexEntry);

    std::cout << "[CDB] Reading " << glyphCount << " glyph entries..." << std::endl;

    // Create staging buffer for index data
    WGPUBufferDescriptor stagingDesc = {};
    stagingDesc.label = WGPU_STR("Index readback buffer");
    stagingDesc.size = indexBufferSize;
    stagingDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;

    WGPUBuffer stagingBuffer = wgpuDeviceCreateBuffer(device, &stagingDesc);

    // Copy index buffer to staging
    WGPUCommandEncoderDescriptor encDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encDesc);
    wgpuCommandEncoderCopyBufferToBuffer(encoder, indexBuffer, 0, stagingBuffer, 0, indexBufferSize);

    WGPUCommandBufferDescriptor cmdBufDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuffer);

    wgpuCommandBufferRelease(cmdBuffer);
    wgpuCommandEncoderRelease(encoder);

    // Wait for GPU work to complete before mapping
    std::atomic<bool> workDone{false};

    WGPUQueueWorkDoneCallbackInfo workDoneCallback = {};
    workDoneCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    workDoneCallback.callback = [](WGPUQueueWorkDoneStatus status, WGPUStringView message, void* userdata1, void* userdata2) {
        *static_cast<std::atomic<bool>*>(userdata1) = true;
    };
    workDoneCallback.userdata1 = &workDone;

    wgpuQueueOnSubmittedWorkDone(queue, workDoneCallback);

    while (!workDone) {
        wgpuInstanceProcessEvents(instance);
    }

    // Map and read
    std::atomic<bool> mapDone{false};
    std::atomic<WGPUMapAsyncStatus> mapStatus{WGPUMapAsyncStatus_Error};

    WGPUBufferMapCallbackInfo mapCallback = {};
    mapCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    mapCallback.callback = [](WGPUMapAsyncStatus status, WGPUStringView message, void* userdata1, void* userdata2) {
        auto* done = static_cast<std::atomic<bool>*>(userdata1);
        auto* statusOut = static_cast<std::atomic<WGPUMapAsyncStatus>*>(userdata2);
        *statusOut = status;
        *done = true;
    };
    mapCallback.userdata1 = &mapDone;
    mapCallback.userdata2 = &mapStatus;

    wgpuBufferMapAsync(stagingBuffer, WGPUMapMode_Read, 0, indexBufferSize, mapCallback);

    while (!mapDone) {
        wgpuInstanceProcessEvents(instance);
    }

    if (mapStatus != WGPUMapAsyncStatus_Success) {
        std::cerr << "[CDB] Failed to map index buffer" << std::endl;
        wgpuBufferRelease(stagingBuffer);
        return false;
    }

    const msdfgl::IndexEntry* indexData = static_cast<const msdfgl::IndexEntry*>(
        wgpuBufferGetConstMappedRange(stagingBuffer, 0, indexBufferSize));

    // Debug: print first few index entries
    std::cout << "[CDB DEBUG] First 5 index entries:" << std::endl;
    for (size_t i = 0; i < std::min(glyphCount, (size_t)5); ++i) {
        const auto& e = indexData[i];
        std::cout << "  [" << i << "] offset=(" << e.offsetX << "," << e.offsetY
                  << ") size=(" << e.sizeX << "x" << e.sizeY << ")"
                  << " bearing=(" << e.bearingX << "," << e.bearingY << ")"
                  << " glyphSize=(" << e.glyphWidth << "x" << e.glyphHeight << ")" << std::endl;
    }

    // Open CDB file
    int fd = open(outputPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        std::cerr << "[CDB] Failed to create file: " << outputPath << std::endl;
        wgpuBufferUnmap(stagingBuffer);
        wgpuBufferRelease(stagingBuffer);
        return false;
    }

    struct cdb_make cdbm;
    if (cdb_make_start(&cdbm, fd) < 0) {
        std::cerr << "[CDB] cdb_make_start failed" << std::endl;
        close(fd);
        wgpuBufferUnmap(stagingBuffer);
        wgpuBufferRelease(stagingBuffer);
        return false;
    }

    size_t writtenCount = 0;
    size_t skippedCount = 0;

    // Write each glyph
    for (const auto& glyphInfo : generatedGlyphs) {
        // Find the index entry for this glyph
        msdfgl::MapItem* item = font->getGlyph(static_cast<int32_t>(glyphInfo.codepoint));
        if (!item || item->index < 0 || static_cast<size_t>(item->index) >= glyphCount) {
            skippedCount++;
            continue;
        }

        const msdfgl::IndexEntry& entry = indexData[item->index];

        // Calculate bitmap dimensions
        // Empty glyphs (like space) have glyphWidth/Height == 0 - match CPU's 0x0 output
        bool isEmpty = (entry.glyphWidth == 0 || entry.glyphHeight == 0);
        int bitmapW = isEmpty ? 0 : static_cast<int>(std::ceil(entry.sizeX));
        int bitmapH = isEmpty ? 0 : static_cast<int>(std::ceil(entry.sizeY));

        // Build header matching CPU generator format
        yetty::MsdfGlyphData header;
        header.codepoint = glyphInfo.codepoint;
        header.width = static_cast<uint16_t>(bitmapW);
        header.height = static_cast<uint16_t>(bitmapH);

        // Convert from FreeType 26.6 format and adjust for padding (matching CPU generator)
        float range = font->getRange();
        if (!isEmpty) {
            header.bearingX = entry.bearingX / 64.0f - range;
            header.bearingY = entry.bearingY / 64.0f + range;
        } else {
            header.bearingX = 0.0f;
            header.bearingY = 0.0f;
        }
        header.sizeX = static_cast<float>(bitmapW);
        header.sizeY = static_cast<float>(bitmapH);
        header.advance = item->advance[0] / 64.0f;

        // Extract pixel data from atlas
        std::vector<uint8_t> pixels;
        if (bitmapW > 0 && bitmapH > 0) {
            int atlasX = static_cast<int>(entry.offsetX);
            int atlasY = static_cast<int>(entry.offsetY);

            pixels.resize(bitmapW * bitmapH * 4);

            for (int y = 0; y < bitmapH; ++y) {
                for (int x = 0; x < bitmapW; ++x) {
                    int srcX = atlasX + x;
                    int srcY = atlasY + y;

                    if (srcX >= 0 && srcX < atlasWidth && srcY >= 0 && srcY < atlasHeight) {
                        size_t srcIdx = (srcY * atlasWidth + srcX) * 4;
                        size_t dstIdx = (y * bitmapW + x) * 4;

                        pixels[dstIdx + 0] = atlasData[srcIdx + 0];
                        pixels[dstIdx + 1] = atlasData[srcIdx + 1];
                        pixels[dstIdx + 2] = atlasData[srcIdx + 2];
                        pixels[dstIdx + 3] = atlasData[srcIdx + 3];
                    }
                }
            }
        }

        // Key: 4-byte codepoint (little-endian)
        char key[4];
        key[0] = glyphInfo.codepoint & 0xFF;
        key[1] = (glyphInfo.codepoint >> 8) & 0xFF;
        key[2] = (glyphInfo.codepoint >> 16) & 0xFF;
        key[3] = (glyphInfo.codepoint >> 24) & 0xFF;

        // Value: header + pixels
        size_t valueSize = sizeof(yetty::MsdfGlyphData) + pixels.size();
        std::vector<char> value(valueSize);
        std::memcpy(value.data(), &header, sizeof(header));
        if (!pixels.empty()) {
            std::memcpy(value.data() + sizeof(header), pixels.data(), pixels.size());
        }

        if (cdb_make_add(&cdbm, key, 4, value.data(), valueSize) < 0) {
            std::cerr << "[CDB] Failed to add glyph " << glyphInfo.codepoint << std::endl;
        } else {
            writtenCount++;
        }
    }

    if (cdb_make_finish(&cdbm) < 0) {
        std::cerr << "[CDB] cdb_make_finish failed" << std::endl;
        close(fd);
        wgpuBufferUnmap(stagingBuffer);
        wgpuBufferRelease(stagingBuffer);
        return false;
    }

    close(fd);
    wgpuBufferUnmap(stagingBuffer);
    wgpuBufferRelease(stagingBuffer);

    // Get file size
    auto fileSize = std::filesystem::file_size(outputPath);

    std::cout << "[CDB] Written " << writtenCount << " glyphs (skipped " << skippedCount << ")" << std::endl;
    std::cout << "[CDB] File size: " << (fileSize / 1024.0 / 1024.0) << " MB" << std::endl;

    return true;
}

void cleanup() {
    font.reset();
    msdfContext.reset();

    if (queue) { wgpuQueueRelease(queue); queue = nullptr; }
    if (device) { wgpuDeviceRelease(device); device = nullptr; }
    if (adapter) { wgpuAdapterRelease(adapter); adapter = nullptr; }
    if (surface) { wgpuSurfaceRelease(surface); surface = nullptr; }
    if (instance) { wgpuInstanceRelease(instance); instance = nullptr; }
}

int main(int argc, char* argv[]) {
    // Default settings
    std::string fontPath = "assets/DejaVuSansMNerdFontMono-Regular.ttf";
    std::string outputPath;  // Empty means auto-generate from font name
    bool includeNerdFonts = true;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--font" && i + 1 < argc) {
            fontPath = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            outputPath = argv[++i];
        } else if (arg == "--no-nerd") {
            includeNerdFonts = false;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "MSDFGL GPU CDB Generator\n\n"
                      << "Usage: " << argv[0] << " [options]\n\n"
                      << "Options:\n"
                      << "  --font PATH     Font file (default: assets/DejaVuSansMNerdFontMono-Regular.ttf)\n"
                      << "  --output FILE   Output .cdb file (default: /tmp/<font-name>.cdb)\n"
                      << "  --no-nerd       Exclude Nerd Font symbols\n"
                      << "  --help, -h      Show this help\n";
            return 0;
        }
    }

    // Auto-generate output path if not specified
    if (outputPath.empty()) {
        std::filesystem::path fp(fontPath);
        outputPath = "/tmp/" + fp.stem().string() + ".cdb";
    }

    std::cout << "========================================\n"
              << "MSDFGL GPU CDB Generator\n"
              << "========================================\n"
              << "Font: " << fontPath << "\n"
              << "Output: " << outputPath << "\n"
              << "Nerd Fonts: " << (includeNerdFonts ? "yes" : "no") << "\n"
              << "========================================\n" << std::endl;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "MSDFGL Gen CDB", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    if (!initWebGPU(window)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    if (!initMSDFGL(fontPath.c_str())) {
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Generate atlas
    auto charset = getDefaultCharset(includeNerdFonts);
    if (!generateAtlas(charset)) {
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Read atlas back from GPU
    int atlasWidth, atlasHeight;
    auto atlasData = readAtlasTexture(atlasWidth, atlasHeight);
    if (atlasData.empty()) {
        std::cerr << "Failed to read atlas texture" << std::endl;
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Create output directory if needed
    std::filesystem::path outPath(outputPath);
    if (outPath.has_parent_path()) {
        std::filesystem::create_directories(outPath.parent_path());
    }
    std::string cdbPath = outputPath;

    if (!writeCdb(cdbPath, atlasData, atlasWidth, atlasHeight)) {
        std::cerr << "Failed to write CDB" << std::endl;
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    std::cout << "\n========================================\n"
              << "Done! Output: " << cdbPath << "\n"
              << "========================================" << std::endl;

    cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
